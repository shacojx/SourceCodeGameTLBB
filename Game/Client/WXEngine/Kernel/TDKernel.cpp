/****************************************\
*										*
*			    数据核心				*
*										*
\****************************************/
#include "StdAfx.h"
#include "TDKernel.h"
#include "TDException.h"
#include "TDUtil.h"
#include "TDPlugin.h"
#include "TDObjectSystem.h"

tKernel* tKernel::s_pMe = NULL;
tKernel::tKernel(VOID)
	: m_theRoot(_T("root"))
	, m_pFirstClass(NULL)
{
	s_pMe = this;
}

tKernel::~tKernel(VOID)
{
}

//将某个类注册到全局类检索表函数
VOID tKernel::ReisgerClass(tClass* pNewClass)
{
	TDAssert(pNewClass);

	if(m_ClassMap[STRING(pNewClass->szClassName)] != NULL)
	{
		TDThrow(_T("Multi register class:%s"), pNewClass->szClassName);
	}
	pNewClass->pNextClass = m_pFirstClass;
	m_pFirstClass = pNewClass;
	//将新的类加入到类检索表
	(m_ClassMap)[pNewClass->szClassName] = pNewClass;
}

//检查两个类是否具有继承关系
BOOL tKernel::IsKindOf(const tClass* pThisClass, const tClass* pBaseClass) const
{
	if(!pBaseClass || !pThisClass) return FALSE;

	register const tClass *pClass = pThisClass;

	while(pClass != NULL)
	{
		if(pClass == pBaseClass)
		{
			return TRUE;
		}
		pClass = pClass->pBaseClass;
	}
	return FALSE;
}
//根据字符串形式类名取得某定义类
tClass* tKernel::OpenClass(LPCTSTR szClassName)
{
	std::map< STRING, tClass* >::iterator it = m_ClassMap.find(szClassName);
	if(it == m_ClassMap.end()) return 0;
	else return it->second;
}

//根据字符串形式的路径来创建新节点
tNode*	tKernel::NewNode(LPCTSTR szClassName, LPCTSTR szPosition, LPCTSTR szNodeName)
{
	TDAssert(szClassName && szPosition && szNodeName);

	//打开类
	tClass *pClass = OpenClass(szClassName);
	if(!pClass)
	{
		TDThrow(_T("(tKernel::NewNode)Can't create new Node[%s]"), szClassName);
	}
	
	std::vector< STRING > vPathSplitBuf;
	TDU_ConvertStringToVector(szPosition, vPathSplitBuf, _T("\\/"));

	tNode *pCurrentNode = &m_theRoot;
	tNode *pFindNode = NULL;

	//搜索父节点
	for(register INT i=0; i<(INT)vPathSplitBuf.size(); i++)
	{
		tNode *pFindNode = pCurrentNode->LookUpChild((LPCTSTR)vPathSplitBuf[i].c_str());
		if(pFindNode == NULL)
		{
			tNode *pNewNode = new tNode(vPathSplitBuf[i].c_str());
			pCurrentNode->AddChild(pNewNode);
			pFindNode = pNewNode;
		}
		pCurrentNode = pFindNode;
	}

	//查找是否该节点已经存在
	if(pCurrentNode->LookUpChild(szNodeName))
	{
		TDThrow(_T("(tKernel::NewNode)The Node[%s] has exist!"), szNodeName);
	}

	tNode *pNewNode = (tNode *)pClass->CreateObject(szNodeName);
	pCurrentNode->AddChild(pNewNode);
	return pNewNode;
}

//根据父节点加入新的节点
tNode*	tKernel::NewNode(LPCTSTR szClassName, tNode* pParentNode, LPCTSTR szNodeName)
{
	TDAssert(szClassName && pParentNode && szNodeName);

	//打开类
	tClass *pClass = OpenClass(szClassName);
	if(!pClass)
	{
		TDThrow(_T("(tKernel::NewNode)Can't create new Node[%s]"), szClassName);
	}

	//查找是否该节点已经存在
	if(pParentNode->LookUpChild(szNodeName))
	{
		TDThrow(_T("(tKernel::NewNode)The Node[%s] has exist!"), szNodeName);
	}

	tNode *pNewNode = (tNode *)pClass->CreateObject(szNodeName);
	pParentNode->AddChild(pNewNode);

	return pNewNode;
}


tNode*	tKernel::GetNode(LPCTSTR szPathName)
{
	TDAssert(szPathName);

	std::vector< STRING > vPathSplitBuf;
	TDU_ConvertStringToVector(szPathName, vPathSplitBuf, _T("\\/"));

	if(vPathSplitBuf.empty()) return NULL;

	tNode *pCurrentNode = &m_theRoot;
	for(register INT i=0; i<(INT)vPathSplitBuf.size(); i++)
	{
		tNode *pFindNode = pCurrentNode->LookUpChild((LPCTSTR)vPathSplitBuf[i].c_str());
		if(pFindNode == NULL)
		{
			return NULL;
		}
		pCurrentNode = pFindNode;
	}

	return pCurrentNode;
}

BOOL tKernel::LoadPlugin(LPCTSTR szPluginName, LPVOID pParam)
{
	if(!szPluginName) return FALSE;

	HMODULE hModule = NULL;
	FUNC_DLLINIT pfnDllInit = NULL;
	FUNC_DLLRELEASE pfnDllRelease = NULL;

	try
	{
		//尝试Load该dll
		hModule = ::LoadLibrary(szPluginName);
		if(!hModule)
		{
			DWORD dLastError = ::GetLastError();
			TDThrow(_T("Load plugin dll(%s) failed!"), szPluginName);
			return FALSE;
		}

		//查看是否有合法的导出函数
		pfnDllInit = (FUNC_DLLINIT)::GetProcAddress(hModule, "DllInit");
		pfnDllRelease = (FUNC_DLLRELEASE)::GetProcAddress(hModule, "DllRelease");
		if(!pfnDllInit || !pfnDllRelease)
		{ 
			::FreeLibrary(hModule); hModule = NULL; 
			TDThrow(_T("Wrong plugin dll(%s) export function!"), szPluginName);
			return FALSE; 
		}
	}
	catch(...) 
	{
		if(hModule) ::FreeLibrary(hModule);
		throw;
		return FALSE;
	}

	//是合法的插件，执行加载函数
	(*pfnDllInit)(pParam);

	PLUGIN_INFO newPlugin;
	newPlugin.strPluginFile = szPluginName;
	newPlugin.hModule = hModule;
	newPlugin.pfnDllInit = pfnDllInit;
	newPlugin.pfnDllRelease = pfnDllRelease;

	m_vAllPlugin.push_back(newPlugin);

	return TRUE;
}


VOID tKernel::FreeAllPlugin(VOID)
{
	//释放所有插件
	for(INT i=0; i<(INT)m_vAllPlugin.size(); i++)
	{
		//执行释放函数
		(*(m_vAllPlugin[i].pfnDllRelease))();

		::FreeLibrary(m_vAllPlugin[i].hModule);
		m_vAllPlugin[i].hModule = NULL;
	}

	m_vAllPlugin.clear();
}

//关闭核心
VOID tKernel::ShutDown(VOID)
{
	//释放节点树
	m_theRoot.Release();

	//释放所有插件
	FreeAllPlugin();
}

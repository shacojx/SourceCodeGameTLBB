#include "StdAfx.h"
#include "TDProfile.h"
#include "TDException.h"


//Profile节点堆栈
#define g_theStack GetTheStack()
//Profile节点树
#define g_theTree GetTheTree()
//是否有查询请求传入
BOOL				g_bAskDump = FALSE;
//查询请求函数
FUNC_PROFILE_DUMP	g_funcDump = NULL;



PROFILE_STACK& GetTheStack()
{
	static PROFILE_STACK theStack;
	return theStack;
}

PROFILE_TREE& GetTheTree()
{
	static PROFILE_TREE theTree;
	static BOOL bFirst = TRUE;
	if(bFirst)
	{
		theTree.theRoot.strName = "ROOT";
		theTree.theRoot.pParentNode = NULL;
		theTree.theRoot.nInvokeCount = 0;
		theTree.theRoot.nTotalTime.QuadPart = 0;
		theTree.pCurrentNode = &(theTree.theRoot);
		bFirst = FALSE;
	}

	return theTree;
}

PROFILE_STACK_NODE::PROFILE_STACK_NODE(LPCTSTR szName, LPCTSTR szAttName)
{
	//记录开始时间
	QueryPerformanceCounter(&nStartTime);
	if(!szName) strName.reserve(128);
	else		
	{
		strName = szName;
		
		if(szAttName)
		{
			strName += ".";
			strName += szAttName;
		}
	}
	pTreeNode = NULL;
}

RPOFILE_TREE_NODE::~RPOFILE_TREE_NODE()
{
	//释放子节点
	HASH_TREENODE::iterator it;
	for(it=mapChild.begin(); it!=mapChild.end(); it++)
	{
		if(it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	mapChild.clear();
}

extern "C"
VOID TDProfile_PushNode(LPCTSTR name, LPCTSTR nameEx)
{
	PROFILE_STACK_NODE theNode(name, nameEx);

	PROFILE_STACK_NODE* pNode = &theNode;
	//--------------------------------------------------------
	//检查目前的堆中是否有重名
	PROFILE_STACK::HASH_NAMESET::iterator itHashName = g_theStack.vHashName.find(pNode->strName);
	if(itHashName != g_theStack.vHashName.end())
	{
		TDThrow("[PUSH]Profile multi name push:%s", pNode->strName.c_str());
		return;
	}

	//检查堆栈是否溢出
	if(g_theStack.nTopIndex >= PROFILE_STACK::MAX_STATCK_NUM)
	{
		TDThrow("[PUSH]Profile stack overflow!");
		return;
	}

	//--------------------------------------------------------
	//在树上产生新节点

	//查找是否有该节点
	RPOFILE_TREE_NODE::HASH_TREENODE::iterator itChild = 
		g_theTree.pCurrentNode->mapChild.find(pNode->strName);
	//如果没有，创建
	if(itChild == g_theTree.pCurrentNode->mapChild.end())
	{
		RPOFILE_TREE_NODE* pNewNode = new RPOFILE_TREE_NODE;
		pNewNode->strName = pNode->strName;
		pNewNode->pParentNode = g_theTree.pCurrentNode;
		pNewNode->nInvokeCount = 0;
		pNewNode->nTotalTime.QuadPart = 0;

		//加入到树中
		g_theTree.pCurrentNode->mapChild.insert(std::make_pair(pNode->strName, pNewNode));
		itChild = g_theTree.pCurrentNode->mapChild.find(pNode->strName);
	}

	//在树中记录数据
	g_theTree.pCurrentNode = itChild->second;

	//--------------------------------------------------------
	//放入堆栈中
	PROFILE_STACK_NODE& topNode = g_theStack.vNodeStack[g_theStack.nTopIndex];
	topNode.strName = pNode->strName;
	topNode.nStartTime = pNode->nStartTime;
	topNode.pTreeNode = itChild->second;
	topNode.pTreeNode->nInvokeCount ++;
	g_theStack.nTopIndex++;

	int a = g_theStack.nTopIndex;

	int b=1;
}

extern "C"
VOID TDProfile_PopNode(LPCTSTR szName)
{
	//检查堆栈是否已经空
	if(g_theStack.nTopIndex <= 0)
	{
		TDThrow("[POP]Profile stack empty!");
		return;
	}

	//--------------------------------------------------------
	//处理堆栈中节点
	PROFILE_STACK_NODE& topStackNode = g_theStack.vNodeStack[g_theStack.nTopIndex-1];
	
	//检查配对是否正确
	if(szName)
	{
		if(strncmp(topStackNode.strName.c_str(), szName, strlen(szName)))
		//if(topStackNode.strName != szName)
		{
			TDThrow("[POP]Profile statck error!");
		}
	}

	//计算过程时间
	LARGE_INTEGER tNow;
	QueryPerformanceCounter(&tNow);
	LARGE_INTEGER nProcessTime;
	nProcessTime.QuadPart = tNow.QuadPart - topStackNode.nStartTime.QuadPart;

	g_theStack.nTopIndex--;

	//--------------------------------------------------------
	//处理树中节点
	RPOFILE_TREE_NODE&  currentTreeNode = *(g_theTree.pCurrentNode);
	currentTreeNode.nTotalTime.QuadPart += nProcessTime.QuadPart;

	g_theTree.pCurrentNode = currentTreeNode.pParentNode;

	//--------------------------------------------------------
	//查询
	TDProfile_Tick();
}

VOID TDProfile_Tick(VOID)
{
	//如果有查询请求并且堆栈空
	if(g_bAskDump && g_funcDump && g_theStack.nTopIndex==0)
	{
		//计算Root总时间
		g_theTree.theRoot.nTotalTime.QuadPart = 0;
		RPOFILE_TREE_NODE::HASH_TREENODE::iterator it;
		for(it=g_theTree.theRoot.mapChild.begin(); it!=g_theTree.theRoot.mapChild.end(); it++)
		{
			g_theTree.theRoot.nTotalTime.QuadPart += it->second->nTotalTime.QuadPart;
		}

		(g_funcDump)(&g_theTree);

		g_bAskDump = FALSE;
	}
}

//请求查询Profile节点信息(当堆栈空时会调用传入的函数指针)
extern "C"
VOID TDProfile_DumpSruct(FUNC_PROFILE_DUMP func)
{
	TDAssert(func);

	g_bAskDump = TRUE;
	g_funcDump = func;
}

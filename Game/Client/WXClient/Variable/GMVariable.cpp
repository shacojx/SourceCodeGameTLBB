#include "StdAfx.h"
#pragma warning(push)
#pragma warning(disable : 4996)

#include "GMVariable.h"
#include "TDException.h"
#include "TDUtil.h"
#include "..\Event\GMEventSystem.h"
#include "..\Script\GMScriptSystem.h"


INT ConvertSectionInVector(LPCTSTR szSectionBuf, INT nLen, std::vector< STRING >& vSectionBuf);

CVariableSystem* CVariableSystem::s_pMe = NULL;
WX_IMPLEMENT_DYNAMIC(CVariableSystem, GETCLASS(tVariableSystem));
CVariableSystem::CVariableSystem()
{
	s_pMe = this;
	m_pMetaTable = 0;
}

CVariableSystem::~CVariableSystem()
{
	if(m_pMetaTable) delete m_pMetaTable;
	m_pMetaTable = 0;
}

//从文件中读取变量
VOID CVariableSystem::LoadVariable(LPCTSTR szFileName, VARIABLE_MAP& mapBuf)
{
	TDAssert(szFileName);
	mapBuf.clear();

	//-----------------------------------------------------------
	//取得配置文件的大小
	HANDLE hFile = CreateFile(szFileName,
							GENERIC_READ,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	
	if(INVALID_HANDLE_VALUE == hFile) return;

	DWORD dwHigh;
	DWORD dwFileSize = GetFileSize(hFile, &dwHigh);
	CloseHandle(hFile); hFile = NULL;
	if(0==dwFileSize) return;

	//-----------------------------------------------------------
	//分配足够的内存
	CHAR* pTempBuf = new CHAR[dwFileSize+32];
	if(!pTempBuf) return;

	//-----------------------------------------------------------
	//从配置文件中读取"Variable"节
	::GetPrivateProfileSection("Variable", pTempBuf, dwFileSize, szFileName);
	//分解
	std::vector< STRING > vRet;
	ConvertSectionInVector(pTempBuf, dwFileSize, vRet);

	delete[] pTempBuf; pTempBuf=NULL;

	//加入变量定义
	for(INT i=0; i<(INT)vRet.size(); i++)
	{
		STRING& strLine = vRet[i];

		STRING::size_type tEqu = strLine.find_first_of("= \t");
		if(tEqu == STRING::npos) continue;

		STRING strName = strLine.substr(0, tEqu);

		CHAR szTemp[1024];
		::GetPrivateProfileString("Variable", strName.c_str(), "", szTemp, 1024, szFileName);

		SetVariable(strName.c_str(), szTemp, FALSE);
	}

	//-----------------------------------------------------------
	//导出函数
	m_pMetaTable = new LuaPlus::LuaObject;
	*m_pMetaTable = CScriptSystem::GetMe()->GetLuaState()->GetGlobals().CreateTable("Variable");
	m_pMetaTable->SetObject("__index", *m_pMetaTable);

	m_pMetaTable->RegisterObjectFunctor("GetVariable", &CVariableSystem::Lua_GetGlobalVariable);
	m_pMetaTable->RegisterObjectFunctor("SetVariable", &CVariableSystem::Lua_SetGlobalVariable);
	m_pMetaTable->RegisterObjectFunctor("SetVariableDelay", &CVariableSystem::Lua_SetGlobalVariableDelay);
	LuaPlus::LuaObject obj_Variable = CScriptSystem::GetMe()->GetLuaState()->BoxPointer(this);
	obj_Variable.SetMetaTable(*m_pMetaTable);
	CScriptSystem::GetMe()->GetLuaState()->GetGlobals().SetObject("Variable", obj_Variable);
}

//将变量保存到文件中
VOID CVariableSystem::SaveVariable(LPCTSTR szFileName, VARIABLE_MAP& mapBuf)
{
	const CHAR* szSystemCfg = "./WXSystem.cfg";
	UINT dwFileAttr = GetFileAttributes(szSystemCfg);

	if(FILE_ATTRIBUTE_READONLY & dwFileAttr)
	{
		SetFileAttributes(szSystemCfg, dwFileAttr&(~FILE_ATTRIBUTE_READONLY));
	}
	VARIABLE_MAP::iterator it;
	for(it=mapBuf.begin(); it!=mapBuf.end(); it++)
	{
		if(it->second.bTemp == FALSE)
		{
			::WritePrivateProfileString("Variable", it->first.c_str(), it->second.vValue.c_str(), szSystemCfg);
		}
	}
	if(FILE_ATTRIBUTE_READONLY & dwFileAttr)
	{
		SetFileAttributes(szSystemCfg, dwFileAttr);
	}

}

//得到某个变量的值，如果不存在，则返回FALSE
BOOL CVariableSystem::GetVariable(LPCTSTR szName, STRING& strValue)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end()) return FALSE;

	strValue = itFind->second.vValue;
	return true;
}

//设置某个变量的值，如果不存在，则首先创建
VOID CVariableSystem::SetVariable(LPCTSTR szName, LPCTSTR szValue, BOOL bTemp, BOOL bFireEvent)
{
	VARIABLE varOld = m_mapCurrent[szName];
	VARIABLE new_Variable;

	new_Variable.bTemp = bTemp;
	new_Variable.vValue = szValue;

	m_mapCurrent[szName] = new_Variable;

	if(varOld.vValue != szValue && bFireEvent)
	{
		//产生事件
		std::vector< STRING > vParam;
		vParam.push_back(szName);
		vParam.push_back(szValue);
		CEventSystem::GetMe()->PushEvent(GE_VARIABLE_CHANGED, vParam);
	}
}

VOID CVariableSystem::SetAs_Float(LPCTSTR szName, FLOAT fValue, BOOL bTemp)
{
	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%f", fValue);
	SetVariable(szName, szTemp, bTemp);
}

VOID CVariableSystem::SetAs_Int(LPCTSTR szName, INT nValue, BOOL bTemp)
{
	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", nValue);
	SetVariable(szName, szTemp, bTemp);
}

VOID CVariableSystem::SetAs_Vector2(LPCTSTR szName, FLOAT fX, FLOAT fY, BOOL bTemp)
{
	CHAR szTemp[64];
	_snprintf(szTemp, 32, "%f,%f", fX, fY);
	SetVariable(szName, szTemp, bTemp);
}


//设置某个变量的值，但并不马上生效, 下一次启动时才会生效
VOID CVariableSystem::SetVariableDelay(LPCTSTR szName, LPCTSTR szValue)
{
	m_mapNeedSave[szName].bTemp = FALSE;
	m_mapNeedSave[szName].vValue = szValue;
}

VOID CVariableSystem::Initial(VOID*)
{
	LoadVariable(".\\WXSystem.cfg", m_mapCurrent);
	m_mapNeedSave.clear();
}

VOID CVariableSystem::Release(VOID)
{
	//should save m_mapNeedSave ...
	SaveVariable(".\\WXSystem.cfg", m_mapCurrent);
	SaveVariable(".\\WXSystem.cfg", m_mapNeedSave);

}

//字符串, DEF=""
const STRING& CVariableSystem::GetAs_String(LPCTSTR szName, BOOL* bHave)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		static STRING s_strBlank = "";
		if(bHave) *bHave = FALSE;
		return s_strBlank;
	}

	if(bHave) *bHave = true;
	return itFind->second.vValue;
}

//整数, DEF=0
INT	CVariableSystem::GetAs_Int(LPCTSTR szName, BOOL* bHave)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		if(bHave) *bHave = FALSE;
		return 0;
	}

	if(bHave) *bHave = true;
	return atoi(itFind->second.vValue.c_str());
}

//浮点数, DEF=0.0f
FLOAT CVariableSystem::GetAs_Float(LPCTSTR szName, BOOL* bHave)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		if(bHave) *bHave = FALSE;
		return 0.0f;
	}

	if(bHave) *bHave = true;
	return (FLOAT)atof(itFind->second.vValue.c_str());
}

fVector2 CVariableSystem::GetAs_Vector2(LPCTSTR szName, BOOL* bHave)
{
	static fVector2 fvNULL(0.0f, 0.0f);

	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		if(bHave) *bHave = FALSE;
		return fvNULL;
	}

	if(bHave) *bHave = true;

	CHAR szTemp[64];
	strncpy(szTemp, itFind->second.vValue.c_str(), 64);

	char* pDot = strchr(szTemp, ',');
	if(!pDot) return fvNULL;
	*pDot = '\0';

	return fVector2((FLOAT)atof(szTemp), (FLOAT)atof(pDot+1));
}

INT CVariableSystem::Lua_SetGlobalVariable(LuaPlus::LuaState* state)	//设置全局变量
{
	LuaPlus::LuaStack args(state);
	if(!args[2].IsString()) 
	{
		TDThrow("LUA:CVariableSystem::Lua_SetGlobalVariable[2] param parameter error");
		return 0;
	}
	if(!args[3].IsString()) 
	{
		TDThrow("LUA:CVariableSystem::Lua_SetGlobalVariable[3] param parameter error");
		return 0;
	}
	if(!args[4].IsInteger())
	{
		TDThrow("LUA:CVariableSystem::Lua_SetGlobalVariable[4] param parameter error");
		return 0;
	}

	CVariableSystem::GetMe()->SetVariable(args[2].GetString(), args[3].GetString(), args[4].GetInteger());
	return 0;
}

INT CVariableSystem::Lua_SetGlobalVariableDelay(LuaPlus::LuaState* state)	//设置全局变量重起后生效
{
	LuaPlus::LuaStack args(state);
	if(!args[2].IsString()) 
	{
		TDThrow("LUA:CVariableSystem::Lua_SetGlobalVariableDelay[2] param parameter error");
		return 0;
	}
	if(!args[3].IsString()) 
	{
		TDThrow("LUA:CVariableSystem::Lua_SetGlobalVariableDelay[3] param parameter error");
		return 0;
	}

	CVariableSystem::GetMe()->SetVariableDelay(args[2].GetString(), args[3].GetString());
	return 0;
}

INT CVariableSystem::Lua_GetGlobalVariable(LuaPlus::LuaState* state)	//取得全局变量
{
	LuaPlus::LuaStack args(state);
	if(!args[2].IsString())
	{
		state->PushNil();
		return 1;
	}
	
	STRING strRet;
	BOOL bExist = CVariableSystem::GetMe()->GetVariable(args[2].GetString(), strRet);
	if(bExist)
	{
		state->PushString(strRet.c_str());
	}
	else
	{
		state->PushNil();
	}
	return 1;
}

void CVariableSystem::SetVariableDefault(LPCTSTR szName, LPCTSTR szValue)
{
	STRING strOld;
	BOOL bExist = GetVariable(szName, strOld);
	if(bExist) return;

	SetVariable(szName, szValue);
}

//---------------------------------------------------------------------------------------------
INT ConvertSectionInVector(LPCTSTR szSectionBuf, INT nLen, std::vector< STRING >& vSectionBuf)
{
	register const char *pDot = szSectionBuf;

	vSectionBuf.clear();

	while(TRUE)
	{
		if( (*pDot == '\0' && *(pDot-1) == '\0') || (pDot-szSectionBuf >= nLen) )
		{
			break;
		}

		if(*pDot == '\0')
		{
			pDot++;
			continue;
		}

		STRING strTheSingle = pDot;
		vSectionBuf.push_back(strTheSingle);

		pDot += strlen(pDot)+1;
	}

	return (INT)vSectionBuf.size();
}

#pragma warning(pop)

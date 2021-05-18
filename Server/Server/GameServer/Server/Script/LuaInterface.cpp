#include "stdafx.h"
//-----------------------------------------------------------------------------
// 文件名 : LuaInterface.cpp
// 模块	:	Script
// 功能	 :	脚本系统提供给外界使用的Lua的接口
// 修改历史:
//-----------------------------------------------------------------------------
#include "LuaInterface.h"
#include "Scene.h"
#include "FileDataMgr.h"
#include "ScriptDef.h"
#include "Log.h"
#include "GameStruct_Script.h"

#define NOPREFIXCALL 

INT	g_Command_AlwaysLoadScript = TRUE ;

LuaInterface::LuaInterface()
{
	__ENTER_FUNCTION
	
	__LEAVE_FUNCTION
}

LuaInterface::~LuaInterface()
{
	__ENTER_FUNCTION

	Destroy();

	__LEAVE_FUNCTION
}
//#include <direct.h>

VOID LuaInterface::Init(Scene* pScene)
{	
	__ENTER_FUNCTION
	SetOwner(pScene);

	GetOwner()->mMisBuf.ResetMisBuf();
	
	BOOL bRet = mLua.Init( ) ;

	if( bRet == FALSE )
	{
		Assert(FALSE && "Lua init failed");
	}
	
	//注册C函数
	mFuncRegister.SetOwner(this);
	mFuncRegister.RegisterCFunction();

	bRet = mLua.Load( "ScriptGlobal.lua") ;
	if( bRet == FALSE )
	{
		AssertEx(FALSE,"Load ScriptGlobal.lua failed") ;
	}

	m_ScriptTable.InitTable( MAX_SCRIPT_HOLD ) ;

	__LEAVE_FUNCTION
}

VOID LuaInterface::Destroy()
{
	__ENTER_FUNCTION

	//清空LuaState
	mLua.Exit( ) ;

	__LEAVE_FUNCTION
}

BOOL LuaInterface::FindSymbol(Lua_State* L,char* funcname)
{
	__ENTER_FUNCTION

	Lua_GetGlobal(L, funcname);
	if(lua_isnil(L,-1))
	{
		lua_pop(L, 1);  
		return FALSE;
	}
	else
	{
		lua_pop(L, 1);  
		return TRUE;
	}

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LuaInterface::PrefixCall(Lua_State* L,char** funcname) 
{
	__ENTER_FUNCTION

	if(!FindSymbol(L,*funcname))
#ifdef NOPREFIXCALL
	{
		char* pSrc = strchr(*funcname,'_')+1;
		memmove(*funcname,pSrc,strlen(pSrc)+1);
		if(!FindSymbol(L,*funcname))
		{

#ifdef _DEBUG
			CHAR strBuf[256];

			lua_getglobal(L,FILENAMEKEY);
			const char* filename = lua_tostring(L,1);
			tsnprintf(strBuf,256,"cann't find %s in %s",*funcname,filename);
			lua_pop(L,1);

			AssertEx(0,strBuf);
#endif
			return FALSE;
		}
	}
#else
	{
#ifdef _DEBUG
		CHAR strBuf[256];

		char* filename = lua_getglobal(L,FILENAMEKEY);
		tsnprintf(strBuf,256,"cann't find %s in %s",*funcname,filename);
		lua_pop(L,1);
		
		AssertEx(0,strBuf);
#endif
		return FALSE;
	}
#endif

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE;
}

INT LuaInterface::ExeFile( CHAR* filename, CHAR* funcname, BOOL bLoad )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
	BOOL bRet=TRUE ;
	if( bLoad )
	{
		bRet = mLua.Load( filename ) ;
		AssertEx(bRet,filename);
		if( bRet == FALSE )
			return 0;
	}

#if _DEBUG
	lua_pushstring(mLua.m_LuaState,filename);
	lua_setglobal(mLua.m_LuaState,FILENAMEKEY);
#endif

	if(!PrefixCall(mLua.m_LuaState,&funcname))
		return 0;

	INT nTopIndex = 0;
	mLua.SafeCallBegin( &nTopIndex ) ;
	bRet = mLua.CallFunction( funcname, 1, "" ) ;
	INT result = (INT)Lua_ValueToNumber(mLua.m_LuaState,1);
	mLua.SafeCallEnd( nTopIndex ) ;

	if(!bRet)
		g_pLog->FastSaveLog(LOG_FILE_2, "stack pop error in fn : %s %s",filename, funcname) ;	

	return result ;

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeFile_D( CHAR* filename, CHAR* funcname, INT Param0, BOOL bLoad )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
	BOOL bRet=TRUE ;
	if( bLoad )
	{
		bRet = mLua.Load( filename ) ;
		AssertEx(bRet,filename);
		if( bRet == FALSE )
			return 0;
	}

#if _DEBUG
	lua_pushstring(mLua.m_LuaState,filename);
	lua_setglobal(mLua.m_LuaState,FILENAMEKEY);
#endif

	if(!PrefixCall(mLua.m_LuaState,&funcname))
		return 0;

	INT nTopIndex = 0;
	mLua.SafeCallBegin( &nTopIndex ) ;
	bRet = mLua.CallFunction( funcname, 1, "d", Param0 ) ;
	INT result = (INT)Lua_ValueToNumber(mLua.m_LuaState,1);
	mLua.SafeCallEnd( nTopIndex ) ;

	if(!bRet)
		g_pLog->FastSaveLog(LOG_FILE_2, "stack pop error in fn : %s %s %d",filename, funcname,Param0) ;	

	return result ;

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeFile_DD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, BOOL bLoad )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
	BOOL bRet=TRUE ;
	if( bLoad )
	{
		bRet = mLua.Load( filename ) ;
		AssertEx(bRet,filename);
		if( bRet == FALSE )
			return 0;
	}

#if _DEBUG
	lua_pushstring(mLua.m_LuaState,filename);
	lua_setglobal(mLua.m_LuaState,FILENAMEKEY);
#endif

	if(!PrefixCall(mLua.m_LuaState,&funcname))
		return 0;

	INT nTopIndex = 0;
	mLua.SafeCallBegin( &nTopIndex ) ;
	bRet = mLua.CallFunction( funcname, 1, "dd", Param0, Param1 ) ;
	INT result = (INT)Lua_ValueToNumber(mLua.m_LuaState,1);
	mLua.SafeCallEnd( nTopIndex ) ;

	if(!bRet)
		g_pLog->FastSaveLog(LOG_FILE_2, "stack pop error in fn : %s %s %d %d",filename, funcname,Param0,Param1) ;	

	return result ;

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeFile_DDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, BOOL bLoad )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
	BOOL bRet=TRUE ;
	if( bLoad )
	{
		bRet = mLua.Load( filename ) ;
		AssertEx(bRet,filename);
		if( bRet == FALSE )
			return 0;
	}

#if _DEBUG
	lua_pushstring(mLua.m_LuaState,filename);
	lua_setglobal(mLua.m_LuaState,FILENAMEKEY);
#endif

	if(!PrefixCall(mLua.m_LuaState,&funcname))
		return 0;

	INT nTopIndex = 0;
	mLua.SafeCallBegin( &nTopIndex ) ;
	bRet = mLua.CallFunction( funcname, 1, "ddd", Param0, Param1, Param2 ) ;
	INT result = (INT)Lua_ValueToNumber(mLua.m_LuaState,1);
	mLua.SafeCallEnd( nTopIndex ) ;

	if(!bRet)
		g_pLog->FastSaveLog(LOG_FILE_2, "stack pop error in fn : %s %s %d %d %d",filename,funcname,Param0,Param1,Param2) ;	

	return result ;

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeFile_DDDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, BOOL bLoad )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
	BOOL bRet=TRUE ;
	if( bLoad )
	{
		bRet = mLua.Load( filename ) ;
		AssertEx(bRet,filename);
		if( bRet == FALSE )
			return 0;
	}

#if _DEBUG
	lua_pushstring(mLua.m_LuaState,filename);
	lua_setglobal(mLua.m_LuaState,FILENAMEKEY);
#endif

	if(!PrefixCall(mLua.m_LuaState,&funcname))
		return 0;

	INT nTopIndex = 0;
	mLua.SafeCallBegin( &nTopIndex ) ;
	bRet = mLua.CallFunction( funcname, 1, "dddd", Param0, Param1, Param2, Param3 ) ;
	INT result = (INT)Lua_ValueToNumber(mLua.m_LuaState,1);
	mLua.SafeCallEnd( nTopIndex ) ;

	if(!bRet)
		g_pLog->FastSaveLog(LOG_FILE_2, "stack pop error in fn : %s %s %d %d %d %d",filename,funcname,Param0,Param1,Param2,Param3) ;	

	return result ;

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeFile_DDDDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, BOOL bLoad )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
	BOOL bRet=TRUE ;
	if( bLoad )
	{
		bRet = mLua.Load( filename ) ;
		AssertEx(bRet,filename);
		if( bRet == FALSE )
			return 0;
	}

#if _DEBUG
	lua_pushstring(mLua.m_LuaState,filename);
	lua_setglobal(mLua.m_LuaState,FILENAMEKEY);
#endif

	if(!PrefixCall(mLua.m_LuaState,&funcname))
		return 0;

	INT nTopIndex = 0;
	mLua.SafeCallBegin( &nTopIndex ) ;
	bRet = mLua.CallFunction( funcname, 1, "ddddd", Param0, Param1, Param2, Param3, Param4 ) ;
	INT result = (INT)Lua_ValueToNumber(mLua.m_LuaState,1);
	mLua.SafeCallEnd( nTopIndex ) ;

	if(!bRet)
		g_pLog->FastSaveLog(LOG_FILE_2, "stack pop error in fn : %s %s %d %d %d %d %d",filename,funcname,Param0,Param1,Param2,Param3,Param4) ;	

	return result ;

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeFile_DDDDDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5, BOOL bLoad )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
	BOOL bRet=TRUE ;
	if( bLoad )
	{
		bRet = mLua.Load( filename ) ;
		AssertEx(bRet,filename);
		if( bRet == FALSE )
			return 0;
	}

#if _DEBUG
	lua_pushstring(mLua.m_LuaState,filename);
	lua_setglobal(mLua.m_LuaState,FILENAMEKEY);
#endif

	if(!PrefixCall(mLua.m_LuaState,&funcname))
		return 0;

	INT nTopIndex = 0;
	mLua.SafeCallBegin( &nTopIndex ) ;
	bRet = mLua.CallFunction( funcname, 1, "dddddd", Param0, Param1, Param2, Param3, Param4, Param5 ) ;
	INT result = (INT)Lua_ValueToNumber(mLua.m_LuaState,1);
	mLua.SafeCallEnd( nTopIndex ) ;

	if(!bRet)
		g_pLog->FastSaveLog(LOG_FILE_2, "stack pop error in fn : %s %s %d %d %d %d %d %d",filename,funcname,Param0,Param1,Param2,Param3,Param4,Param5) ;	

	return result ;

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeFile_DDDDDDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5, INT Param6, BOOL bLoad )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
	BOOL bRet=TRUE ;
	if( bLoad )
	{
		bRet = mLua.Load( filename ) ;
		AssertEx(bRet,filename);
		if( bRet == FALSE )
			return 0;
	}

#if _DEBUG
	lua_pushstring(mLua.m_LuaState,filename);
	lua_setglobal(mLua.m_LuaState,FILENAMEKEY);
#endif

	if(!PrefixCall(mLua.m_LuaState,&funcname))
		return 0;

	INT nTopIndex = 0;
	mLua.SafeCallBegin( &nTopIndex ) ;
	bRet = mLua.CallFunction( funcname, 1, "ddddddd", Param0, Param1, Param2, Param3, Param4, Param5, Param6 ) ;
	INT result = (INT)Lua_ValueToNumber(mLua.m_LuaState,1);
	mLua.SafeCallEnd( nTopIndex ) ;

	if(!bRet)
		g_pLog->FastSaveLog(LOG_FILE_2, "stack pop error in fn : %s %s %d %d %d %d %d %d %d",filename,funcname,Param0,Param1,Param2,Param3,Param4,Param5,Param6) ;	

	return result ;

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeFile_DDDDDDDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5, INT Param6, INT Param7, BOOL bLoad )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
	BOOL bRet=TRUE ;
	if( bLoad )
	{
		bRet = mLua.Load( filename ) ;
		AssertEx(bRet,filename);
		if( bRet == FALSE )
			return 0;
	}

#if _DEBUG
	lua_pushstring(mLua.m_LuaState,filename);
	lua_setglobal(mLua.m_LuaState,FILENAMEKEY);
#endif

	if(!PrefixCall(mLua.m_LuaState,&funcname))
		return 0;

	INT nTopIndex = 0;
	mLua.SafeCallBegin( &nTopIndex ) ;
	bRet = mLua.CallFunction( funcname, 1, "dddddddd", Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7 ) ;
	INT result = (INT)Lua_ValueToNumber(mLua.m_LuaState,1);
	mLua.SafeCallEnd( nTopIndex ) ;

	if(!bRet)
		g_pLog->FastSaveLog(LOG_FILE_2, "stack pop error in fn : %s %s %d %d %d %d %d %d %d %d",filename,funcname,Param0,Param1,Param2,Param3,Param4,Param5,Param6,Param7) ;	

	return result ;

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeFile_DDNN( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, FLOAT fParam2, FLOAT fParam3, BOOL bLoad )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
	BOOL bRet=TRUE ;
	if( bLoad )
	{
		bRet = mLua.Load( filename ) ;
		AssertEx(bRet,filename);
		if( bRet == FALSE )
			return 0;
	}

#if _DEBUG
	lua_pushstring(mLua.m_LuaState,filename);
	lua_setglobal(mLua.m_LuaState,FILENAMEKEY);
#endif

	if(!PrefixCall(mLua.m_LuaState,&funcname))
		return 0;

	INT nTopIndex = 0;
	mLua.SafeCallBegin( &nTopIndex ) ;
	bRet = mLua.CallFunction( funcname, 1, "ddnn", Param0, Param1, fParam2, fParam3 ) ;
	INT result = (INT)Lua_ValueToNumber(mLua.m_LuaState,1);
	mLua.SafeCallEnd( nTopIndex ) ;

	if(!bRet)
		g_pLog->FastSaveLog(LOG_FILE_2, "stack pop error in fn : %s %s %d %d %f %f",filename,funcname,Param0,Param1,fParam2,fParam3) ;	

	return result ;

__LEAVE_FUNCTION

	return 0 ;
}


INT LuaInterface::ExeScript( ScriptID_t scriptid, CHAR* funcname )
{
__ENTER_FUNCTION

	SceneID_t sceneId = GetOwner()->SceneID();

	BOOL bLoad = FALSE ;
	SFileData* pSFileData = (SFileData*)(m_ScriptTable.Get(scriptid)) ;
	if( pSFileData==NULL )
	{
		pSFileData = GetOwner()->GetScriptFileMgr()->GetFileData(scriptid);
		m_ScriptTable.Add( scriptid, pSFileData ) ;
		bLoad = TRUE ;
	}

#ifdef _DEBUG
	CHAR strBuf[256];
	tsnprintf(strBuf,256,"%s,scriptid=%d",funcname,scriptid);
	AssertEx(pSFileData,strBuf);
#endif

	if(pSFileData)
	{
		CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());

		AssertEx(filename,filename);
		if(filename)
		{
			char funcnameX[128];
			memset(funcnameX,0,128);
			sprintf(funcnameX,"x%.6d_%s",scriptid,funcname);

			_MY_TRY
			{
				return ExeFile(filename,funcnameX,bLoad);
			}
			_MY_CATCH
			{
				CHAR szMsg[256] ;
				sprintf( szMsg, "ERROR:%s %s %d", filename, funcnameX, bLoad ) ;
				MyMessageBox( szMsg ) ;
			}
		}
	}

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeScript_D( ScriptID_t scriptid, CHAR* funcname, INT Param0 )
{
__ENTER_FUNCTION

	SceneID_t sceneId = GetOwner()->SceneID();

	BOOL bLoad = FALSE ;
	SFileData* pSFileData = (SFileData*)(m_ScriptTable.Get(scriptid)) ;
	if( pSFileData==NULL )
	{
		pSFileData = GetOwner()->GetScriptFileMgr()->GetFileData(scriptid);
		m_ScriptTable.Add( scriptid, pSFileData ) ;
		bLoad = TRUE ;
	}

#ifdef _DEBUG
	CHAR strBuf[256];
	tsnprintf(strBuf,256,"%s,scriptid=%d",funcname,scriptid);
	AssertEx(pSFileData,strBuf);
#endif

	if(pSFileData)
	{
		CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());

		AssertEx(filename,filename);
		if(filename)
		{
			char funcnameX[128];
			memset(funcnameX,0,128);
			sprintf(funcnameX,"x%.6d_%s",scriptid,funcname);

			_MY_TRY
			{
				return ExeFile_D(filename,funcnameX,Param0,bLoad);
			}
			_MY_CATCH
			{
				CHAR szMsg[256] ;
				sprintf( szMsg, "ERROR:%s %s %d %d", filename, funcnameX, Param0,bLoad ) ;
				MyMessageBox( szMsg ) ;
			}
		}
	}

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeScript_DD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1 )
{
__ENTER_FUNCTION

	if(scriptid == INVALID_ID)
	{
		return 0;
	}

	SceneID_t sceneId = GetOwner()->SceneID();

	BOOL bLoad = FALSE ;
	SFileData* pSFileData = (SFileData*)(m_ScriptTable.Get(scriptid)) ;
	if( pSFileData==NULL )
	{
		pSFileData = GetOwner()->GetScriptFileMgr()->GetFileData(scriptid);
		m_ScriptTable.Add( scriptid, pSFileData ) ;
		bLoad = TRUE ;
	}

#ifdef _DEBUG
	CHAR strBuf[256];
	tsnprintf(strBuf,256,"%s,scriptid=%d",funcname,scriptid);
	AssertEx(pSFileData,strBuf);
#endif

	if(pSFileData)
	{
		CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());

		AssertEx(filename,filename);
		if(filename)
		{
			char funcnameX[128];
			memset(funcnameX,0,128);
			sprintf(funcnameX,"x%.6d_%s",scriptid,funcname);

			_MY_TRY
			{
				return ExeFile_DD(filename,funcnameX,Param0,Param1,bLoad);
			}
			_MY_CATCH
			{
				CHAR szMsg[256] ;
				sprintf( szMsg, "ERROR:%s %s %d %d %d", filename, funcnameX, Param0,Param1,bLoad ) ;
				MyMessageBox( szMsg ) ;
			}
		}
	}

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeScript_DDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2 )
{
__ENTER_FUNCTION

	SceneID_t sceneId = GetOwner()->SceneID();

	BOOL bLoad = FALSE ;
	SFileData* pSFileData = (SFileData*)(m_ScriptTable.Get(scriptid)) ;
	if( pSFileData==NULL )
	{
		pSFileData = GetOwner()->GetScriptFileMgr()->GetFileData(scriptid);
		m_ScriptTable.Add( scriptid, pSFileData ) ;
		bLoad = TRUE ;
	}

#ifdef _DEBUG
	CHAR strBuf[256];
	tsnprintf(strBuf,256,"%s,scriptid=%d",funcname,scriptid);
	AssertEx(pSFileData,strBuf);
#endif

	if(pSFileData)
	{
		CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());
		AssertEx(filename,filename);
		if(filename)
		{
			char funcnameX[128];
			memset(funcnameX,0,128);
			sprintf(funcnameX,"x%.6d_%s",scriptid,funcname);

			_MY_TRY
			{
				return ExeFile_DDD(filename,funcnameX,Param0,Param1,Param2,bLoad);
			}
			_MY_CATCH
			{
				CHAR szMsg[256] ;
				sprintf( szMsg, "ERROR:%s %s %d %d %d %d", filename, funcnameX, Param0,Param1,Param2,bLoad ) ;
				MyMessageBox( szMsg ) ;
			}
		}
	}

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeScript_DDDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3 )
{
__ENTER_FUNCTION

	SceneID_t sceneId = GetOwner()->SceneID();

	BOOL bLoad = FALSE ;
	SFileData* pSFileData = (SFileData*)(m_ScriptTable.Get(scriptid)) ;
	if( pSFileData==NULL )
	{
		pSFileData = GetOwner()->GetScriptFileMgr()->GetFileData(scriptid);
		m_ScriptTable.Add( scriptid, pSFileData ) ;
		bLoad = TRUE ;
	}

#ifdef _DEBUG
	CHAR strBuf[256];
	tsnprintf(strBuf,256,"%s,scriptid=%d",funcname,scriptid);
	AssertEx(pSFileData,strBuf);
#endif

	if(pSFileData)
	{
		CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());

		AssertEx(filename,filename);
		if(filename)
		{
			char funcnameX[128];
			memset(funcnameX,0,128);
			sprintf(funcnameX,"x%.6d_%s",scriptid,funcname);

			_MY_TRY
			{
				return ExeFile_DDDD(filename,funcnameX,Param0,Param1,Param2,Param3,bLoad);
			}
			_MY_CATCH
			{
				CHAR szMsg[256] ;
				sprintf( szMsg, "ERROR:%s %s %d %d %d %d %d", filename, funcnameX, Param0,Param1,Param2,Param3,bLoad ) ;
				MyMessageBox( szMsg ) ;
			}
		}
	}

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeScript_DDDDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4 )
{
__ENTER_FUNCTION

	SceneID_t sceneId = GetOwner()->SceneID();

	BOOL bLoad = FALSE ;
	SFileData* pSFileData = (SFileData*)(m_ScriptTable.Get(scriptid)) ;
	if( pSFileData==NULL )
	{
		pSFileData = GetOwner()->GetScriptFileMgr()->GetFileData(scriptid);
		m_ScriptTable.Add( scriptid, pSFileData ) ;
		bLoad = TRUE ;
	}

#ifdef _DEBUG
	CHAR strBuf[256];
	tsnprintf(strBuf,256,"%s,scriptid=%d",funcname,scriptid);
	AssertEx(pSFileData,strBuf);
#endif

	if(pSFileData)
	{
		CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());

		AssertEx(filename,filename);
		if(filename)
		{
			char funcnameX[128];
			memset(funcnameX,0,128);
			sprintf(funcnameX,"x%.6d_%s",scriptid,funcname);

			_MY_TRY
			{
				return ExeFile_DDDDD(filename,funcnameX,Param0,Param1,Param2,Param3,Param4,bLoad);
			}
			_MY_CATCH
			{
				CHAR szMsg[256] ;
				sprintf( szMsg, "ERROR:%s %s %d %d %d %d %d %d", filename, funcnameX, Param0,Param1,Param2,Param3,Param4,bLoad ) ;
				MyMessageBox( szMsg ) ;
			}
		}
	}

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeScript_DDDDDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5 )
{
__ENTER_FUNCTION

	SceneID_t sceneId = GetOwner()->SceneID();

	BOOL bLoad = FALSE ;
	SFileData* pSFileData = (SFileData*)(m_ScriptTable.Get(scriptid)) ;
	if( pSFileData==NULL )
	{
		pSFileData = GetOwner()->GetScriptFileMgr()->GetFileData(scriptid);
		m_ScriptTable.Add( scriptid, pSFileData ) ;
		bLoad = TRUE ;
	}

#ifdef _DEBUG
	CHAR strBuf[256];
	tsnprintf(strBuf,256,"%s,scriptid=%d",funcname,scriptid);
	AssertEx(pSFileData,strBuf);
#endif

	if(pSFileData)
	{
		CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());

		AssertEx(filename,filename);
		if(filename)
		{
			char funcnameX[128];
			memset(funcnameX,0,128);
			sprintf(funcnameX,"x%.6d_%s",scriptid,funcname);

			_MY_TRY
			{
				return ExeFile_DDDDDD(filename,funcnameX,Param0,Param1,Param2,Param3,Param4,Param5,bLoad);
			}
			_MY_CATCH
			{
				CHAR szMsg[256] ;
				sprintf( szMsg, "ERROR:%s %s %d %d %d %d %d %d %d", filename, funcnameX, Param0,Param1,Param2,Param3,Param4,Param5,bLoad ) ;
				MyMessageBox( szMsg ) ;
			}
		}
	}

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeScript_DDDDDDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5, INT Param6 )
{
__ENTER_FUNCTION

	SceneID_t sceneId = GetOwner()->SceneID();

	BOOL bLoad = FALSE ;
	SFileData* pSFileData = (SFileData*)(m_ScriptTable.Get(scriptid)) ;
	if( pSFileData==NULL )
	{
		pSFileData = GetOwner()->GetScriptFileMgr()->GetFileData(scriptid);
		m_ScriptTable.Add( scriptid, pSFileData ) ;
		bLoad = TRUE ;
	}

#ifdef _DEBUG
	CHAR strBuf[256];
	tsnprintf(strBuf,256,"%s,scriptid=%d",funcname,scriptid);
	AssertEx(pSFileData,strBuf);
#endif

	if(pSFileData)
	{
		CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());

		AssertEx(filename,filename);
		if(filename)
		{
			char funcnameX[128];
			memset(funcnameX,0,128);
			sprintf(funcnameX,"x%.6d_%s",scriptid,funcname);

			_MY_TRY
			{
				return ExeFile_DDDDDDD(filename,funcnameX,Param0,Param1,Param2,Param3,Param4,Param5,Param6,bLoad);
			}
			_MY_CATCH
			{
				CHAR szMsg[256] ;
				sprintf( szMsg, "ERROR:%s %s %d %d %d %d %d %d %d %d", filename, funcnameX, Param0,Param1,Param2,Param3,Param4,Param5,Param6,bLoad ) ;
				MyMessageBox( szMsg ) ;
			}
		}
	}

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeScript_DDDDDDDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5, INT Param6, INT Param7 )
{
__ENTER_FUNCTION

	SceneID_t sceneId = GetOwner()->SceneID();

	BOOL bLoad = FALSE ;
	SFileData* pSFileData = (SFileData*)(m_ScriptTable.Get(scriptid)) ;
	if( pSFileData==NULL )
	{
		pSFileData = GetOwner()->GetScriptFileMgr()->GetFileData(scriptid);
		m_ScriptTable.Add( scriptid, pSFileData ) ;
		bLoad = TRUE ;
	}

#ifdef _DEBUG
	CHAR strBuf[256];
	tsnprintf(strBuf,256,"%s,scriptid=%d",funcname,scriptid);
	AssertEx(pSFileData,strBuf);
#endif

	if(pSFileData)
	{
		CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());

		AssertEx(filename,filename);
		if(filename)
		{
			char funcnameX[128];
			memset(funcnameX,0,128);
			sprintf(funcnameX,"x%.6d_%s",scriptid,funcname);

			_MY_TRY
			{
				return ExeFile_DDDDDDDD(filename,funcnameX,Param0,Param1,Param2,Param3,Param4,Param5,Param6,Param7,bLoad);
			}
			_MY_CATCH
			{
				CHAR szMsg[256] ;
				sprintf( szMsg, "ERROR:%s %s %d %d %d %d %d %d %d %d %d", filename, funcnameX, Param0,Param1,Param2,Param3,Param4,Param5,Param6,Param7,bLoad ) ;
				MyMessageBox( szMsg ) ;
			}
		}
	}

__LEAVE_FUNCTION

	return 0 ;
}

INT LuaInterface::ExeScript_DDNN( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, FLOAT fParam2, FLOAT fParam3 )
{
__ENTER_FUNCTION

	SceneID_t sceneId = GetOwner()->SceneID();

	BOOL bLoad = FALSE ;
	SFileData* pSFileData = (SFileData*)(m_ScriptTable.Get(scriptid)) ;
	if( pSFileData==NULL )
	{
		pSFileData = GetOwner()->GetScriptFileMgr()->GetFileData(scriptid);
		m_ScriptTable.Add( scriptid, pSFileData ) ;
		bLoad = TRUE ;
	}

#ifdef _DEBUG
	CHAR strBuf[256];
	tsnprintf(strBuf,256,"%s,scriptid=%d",funcname,scriptid);
	AssertEx(pSFileData,strBuf);
#endif

	if(pSFileData)
	{
		CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());

		AssertEx(filename,filename);
		if(filename)
		{
			char funcnameX[128];
			memset(funcnameX,0,128);
			sprintf(funcnameX,"x%.6d_%s",scriptid,funcname);

			_MY_TRY
			{
				return ExeFile_DDNN(filename,funcnameX,Param0,Param1,fParam2,fParam3,bLoad);
			}
			_MY_CATCH
			{
				CHAR szMsg[256] ;
				sprintf( szMsg, "ERROR:%s %s %d %d %f %f %d", filename, funcnameX, Param0,Param1,fParam2,fParam3,bLoad) ;
				MyMessageBox( szMsg ) ;
			}
		}
	}

__LEAVE_FUNCTION

	return 0 ;
}

VOID LuaInterface::OnEventRequest( ScriptID_t misId,ObjID_t idObj, ObjID_t targetObjId,ScriptID_t scriptId,INT exIndex)
{
	__ENTER_FUNCTION

	SceneID_t sceneId = GetOwner()->SceneID();

	Lua_PushNumber(mLua.m_LuaState,exIndex);
	lua_setglobal(mLua.m_LuaState,"global_numtext");

	ExeScript_DDDD( misId, "OnEventRequest", sceneId, idObj, targetObjId, scriptId ) ;

	__LEAVE_FUNCTION
}


Scene* LuaInterface::GetOwner()
{
	return mOwner;
}

VOID LuaInterface::SetOwner(Scene* pScene)
{
	mOwner = pScene;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


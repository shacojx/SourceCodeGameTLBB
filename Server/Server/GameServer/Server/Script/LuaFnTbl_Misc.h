//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Misc.h
// 模块	:	Script
// 功能	 :  杂项相关函数
// 修改历史:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_MISC_H__
#define __LUAFNTBL_MISC_H__

#include "LuaFnMacro.h"
#include "TimeManager.h"
#include "GWChat.h"
#include "CGChat.h"
#include "FileDataMgr.h"
#include "GUIDManager.h"
#include "GCDelObject.h"
#include "Scene.h"
#include "ServerManager.h"
#include "SceneTimer.h"
#include "PacketFactoryManager.h"
#include "HumanItemLogic.h"
#include "GCCharAllTitles.h"
#include "GCBankBegin.h"

extern INT	g_Command_AlwaysLoadScript ;

namespace LuaFnTbl
{
/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetTimer
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 selfId
	*					调用者ID,无符号整型
	*				 scriptId
	*					执行脚本ID,short类型
	*				 funcName
	*					计时器回调调函数名,字符串类型
	*				 tickTime
	*					计时器调用间隔时间
	*     功能说明 :
	*				 给玩家打开计时器,时间到时将自动调用OnTimer函数	
	*				 玩家本身身上并没有计时器，脚本程序需要计时器时向系统申请“租用”，由系统分配
	*                返回－1,表示错误
	*/
	INT LuaFnSetTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ScriptID_t scriptId = Lua_ValueToNumber(L,3);
		const CHAR* funcName = Lua_ValueToString(L,4);
		INT tickTime = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetTimer")
		if(pHuman->GetUseTimerCount() < pHuman->GetMaxTimerCount())
		{
			INT sceneTimerIndex = pScene->GetSceneTimer()->GetTimer();
			AssertEx(sceneTimerIndex>=0 && sceneTimerIndex < g_Config.m_ConfigInfo.m_MaxTimerCount,"No Timer resource for allocate");

			if(sceneTimerIndex>=0 && sceneTimerIndex < g_Config.m_ConfigInfo.m_MaxTimerCount)
			{
				pHuman->AddTimer(sceneTimerIndex);
				pScene->GetSceneTimer()->StartTheTimer(sceneTimerIndex,selfId,scriptId,funcName,tickTime);

				Lua_PushNumber(L,sceneTimerIndex);
				return 1;
			}
		}
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnStopTimer
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				sceneTimerIndex
	*					计时器在场景计时器池中的数组下标,有SetTimer时返回		
	*     功能说明 :
    *				关闭当前玩家的计时器	//每个玩家身上最多可以绑定三个计时器
	*/
	INT LuaFnStopTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT sceneTimerIndex = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnStopTimer")
		pScene->GetSceneTimer()->FreeTimer(sceneTimerIndex);
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnCheckTimer
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 TimeIndex
	*					计时器索引
	*     功能说明 : 判断TimeIndex号计时器是否已经在使用中
	*                返回1,表示在使用；返回0，表示没有使用; 返回-1，表示异常
	*/
	INT LuaFnCheckTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT TimeIndex = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnCheckTimer")
		INT nRet = pScene->GetSceneTimer()->CheckTimer(TimeIndex) ;
		Lua_PushNumber(L,nRet);
		return 1;
		ENDSCENEDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetGameTime
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
    *				获取当前玩家在线时间，单位为秒
	*				用当前时间减去上线时间
	*				没有测试
	*/
	INT LuaFnGetGameTime(Lua_State* L)	
	{	
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetGameTime")
		Lua_PushNumber(L,g_pTimeManager->CurrentTime() / 1000  - pHuman->GetDB()->GetHumanDB()->m_LastLoginTime);
		return 1;
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-1-10
	*     函数名称 : LuaFnGetCurrentTime
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*     功能说明 :
	*				获取当前时间，单位为秒
	*/
	INT LuaFnGetCurrentTime(Lua_State* L)	
	{	
		Lua_PushNumber(L, g_pTimeManager->GetANSITime());
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddGlobalCountNews
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				newstr
	*					公告内容,字符串指针类型
	*				count
	*					发送次数
	*     功能说明 :
    *				向全组服务器发送新闻公告，公告发送指定次数
	*				没有测试
	*/
	INT LuaFnAddGlobalCountNews(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		CHAR* newstr = const_cast<CHAR*>(Lua_ValueToString(L,2));
		INT count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddGlobalCountNews")

		GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

		pChatPacket->SetSourGUID( INVALID_ID ) ;
		pChatPacket->SetChatType( CHAT_TYPE_SYSTEM ) ;
		pChatPacket->SetContexSize( count ) ;
		pChatPacket->SetContex( newstr ) ;

		g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;

		return 0;

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnPrintNum
	*     参数说明 : 
	*				num
	*				消息框的提示数字
	*     功能说明 :
	*				弹出消息框,测试脚本用
	*/
	INT LuaFnPrintNum(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT num = Lua_ValueToNumber(L,1);

		CHAR msg[1024];
		memset(msg,0,1024) ;
		sprintf(msg,"%d\n",num);
		MyMessageBox(msg);
		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnPrintStr
	*     参数说明 : 
	*				strText
	*				消息框的提示字符串
	*     功能说明 :
	*				弹出消息框,测试脚本用
	*/
	INT LuaFnPrintStr(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		const CHAR *strText = Lua_ValueToString(L,1);

		CHAR msg[1024];
		memset(msg,0,1024);
		sprintf(msg,"%s\n",strText);
		MyMessageBox(msg);
		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnCallScriptFunction
	*     参数说明 : 
	*     功能说明 :
	*					调用可变参数的任意脚本函数
	*/
	INT LuaFnCallScriptFunction(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		INT argc = Lua_GetTopIndex(L);

		AssertEx(argc>=3,"参数个数小于3");

		ScriptID_t scriptId = Lua_ValueToNumber(L,1);
		const CHAR* funcname = Lua_ValueToString(L,2);
		INT sceneId = Lua_ValueToNumber(L,3);

		Scene* pScene = g_pSceneManager->GetScene(sceneId);
		Assert( pScene && "CallScriptFunction pScene = null" );

		if(pScene)
		{
			INT nTopIndex = 0;
			pScene->GetLuaInterface()->mLua.SafeCallBegin( &nTopIndex ) ;
			
			BOOL bLoad = FALSE ;
			SFileData* pSFileData = (SFileData*)(pScene->GetLuaInterface()->m_ScriptTable.Get(scriptId)) ;
			if( pSFileData==NULL )
			{
				pSFileData = pScene->GetLuaInterface()->GetOwner()->GetScriptFileMgr()->GetFileData(scriptId);
				pScene->GetLuaInterface()->m_ScriptTable.Add( scriptId, pSFileData ) ;
				bLoad = TRUE ;
			}

#ifdef _DEBUG
			CHAR buf[32];
			tsnprintf(buf,32,"文件号%d",scriptId);
			AssertEx( pSFileData && "CallScriptFunction pFileData = null" ,buf);
#endif
			const CHAR* filename = pSFileData->GetFileName();
			AssertEx( filename && "CallScriptFunction filename = null",filename );

			if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
			BOOL bRet=TRUE ;
			if( bLoad )
			{
				bRet = pScene->GetLuaInterface()->mLua.Load( const_cast<CHAR*>(filename) ) ;
				AssertEx(bRet,filename);
				if( bRet == FALSE )
					return 0;
			}

			if ( pSFileData != NULL )
			{
				if ( filename != NULL )
				{
					if ( TRUE )
					{
						CHAR argv[10];
						memset(argv,0,10);
						
						char funcnameX[128];
						memset(funcnameX,0,128);
						sprintf(funcnameX,"x%.6d_%s",scriptId,funcname);
						char* fnname = funcnameX;

						#if _DEBUG
							lua_pushstring(L,filename);
							lua_setglobal(L,FILENAMEKEY);
						#endif

						if(!(pScene->GetLuaInterface()->PrefixCall(L,&fnname)))
						{
							Lua_PushNumber(L,-1);
							return 1;
						}

						LUA_ENTER_FUNCTION

						Lua_GetGlobal(L, fnname); 

						INT nBeginParamIndex = 3;
						for(INT i=nBeginParamIndex;i<=argc;i++)
						{
							switch ( Lua_GetValueType( L, i ) )
							{
							case LUA_TUSERDATA:
								argv[i-nBeginParamIndex] = 'p';
								//Lua_PushUserTag(L,Lua_ValueToUserData(L,i));
								break;
							case LUA_TNIL:
								argv[i-nBeginParamIndex] = 'n';
								Lua_PushNil(L);
								break;
							case LUA_TNUMBER:
								argv[i-nBeginParamIndex] = 'd';
								Lua_PushNumber(L,Lua_ValueToNumber(L,i));
								break;
							case LUA_TSTRING:
								argv[i-nBeginParamIndex] = 's';
								Lua_PushString(L,Lua_ValueToString(L,i));
								break;
							case LUA_TTABLE:
								argv[i-nBeginParamIndex] = 't';
								//Lua_PushNumber(L,Lua_ValueToString(L,i));
								break;
							case LUA_TFUNCTION:
							default:
								argv[i-nBeginParamIndex] = 'f';
								Lua_PushCFunction(L,Lua_ValueToCFunction(L,i));
								break;
							}
						}

						INT nResults = -1;
						INT ret = Lua_Call(L, argc - nBeginParamIndex + 1, nResults);

						if (ret != 0)
						{
							g_pLog->FastSaveLog( LOG_FILE_2,"Lua_Call %s %s!",filename,fnname);
						}

						int argnow = Lua_GetTopIndex(L);
						nResults = argnow - argc;//(INT)Lua_ValueToNumber(L,argc);		
						for(int i=1;i<=nResults;i++)
						{
							Lua_PushNumber(L,lua_tonumber(L,i +argc));
						}
						
						pScene->GetLuaInterface()->mLua.SafeCallEnd( argnow ) ;
						return nResults;

						LUA_LEAVE_FUNCTION
					}
				}
			}
		}
		
		Lua_PushNumber(L,-1);
		return 1;	

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,-1);
		return 1;	
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-19
	*     函数名称 : LuaFnIsInDist
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					对方的ID编号,ObjID
	*				dist
	*					self与target距离
	*     功能说明 :
	*				判断selfId与targetId之间的距离是否在dist内(包括)
	*				目前用于护送任务,判断玩家和被护送者之间的距离
	*/
	INT LuaFnIsInDist(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		FLOAT dist			=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnIsInDist")

		Obj* pSelfObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
		Obj* pTargetObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));

		if(pSelfObj && pTargetObj)
		{
			FLOAT fTDist = MySqrt(pSelfObj->getWorldPos(),pTargetObj->getWorldPos());
			if(fTDist <= dist)
			{
				Lua_PushNumber(L,TRUE);
				return 1;
			}
			else
			{
				Lua_PushNumber(L,FALSE);
				return 1;
			}
		}
	
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-15
	*     函数名称 : LuaFnGetDist
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					对方的ID编号,ObjID
	*     功能说明 :
	*				得到selfId和targetId之间的距离
	*/
	INT LuaFnGetDist(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		FLOAT dist			=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnGetDist")

			Obj* pSelfObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
			Obj* pTargetObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));

			if(pSelfObj && pTargetObj)
			{
				FLOAT fTDist = MySqrt(pSelfObj->getWorldPos(),pTargetObj->getWorldPos());
				
				Lua_PushNumber(L,fTDist);
				return 1;
			}


		RETURNFALSE
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnGuid2ObjId
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				guid
	*					GUID,GUID_t类型，仅适用于玩家的GUID转换
	*     功能说明 :
	*				把Guid转换为objId
	*/
	INT LuaFnGuid2ObjId(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		GUID_t guid		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGuid2ObjId")
		Obj_Character* pCharacter = (Obj_Character*)(g_pGUIDManager->Get(guid)) ;
		if( pCharacter )
		{
			if( pCharacter->GetObjType()==Obj::OBJ_TYPE_HUMAN )
			{
				INT objId = pCharacter->GetID();
				Lua_PushNumber(L,objId);
				return 1;
			}
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnObjId2Guid
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 : 返回selfId的GUID
	*/
	INT LuaFnObjId2Guid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnObjId2Guid")
		if( pHuman )
		{
			GUID_t guid = pHuman->GetGUID() ;
			Lua_PushNumber(L,guid);
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-21
	*     函数名称 : LuaFnRemoveMonster
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				删除Obj_Monster,把Obj_Monster从地图删除
	*/
	INT LuaFnRemoveMonster(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);

		AssertEx( FALSE, "此接口已经废弃，请使用LuaFnDeleteMonster" ) ;

		BEGINCHARCTERDEFINE("LuaFnRemoveMonster")
		pCharacter->SetActiveFlag( FALSE );
		pCharacter->CleanUp();
		pCharacter->getScene()->DeleteObject( pCharacter );

		return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-2
	*     函数名称 : LuaFnAwardTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleIndex
	*					称号ID
	*				TitleId
	*					称号ID
	*     功能说明 :
	*				为selfId授予一个称号
	*/
	INT	LuaFnAwardTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT TitleIndex		=	Lua_ValueToNumber(L,3);
		ObjID_t TitleId			=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnAwardTitle")
			switch(TitleIndex)
			{
			case 0:
				pHuman->SetLevelTitle(TitleId);
				break;
			case 1:
				pHuman->SetLifeTitle(TitleId);
				break;
			case 2:
				pHuman->SetRankTitle(TitleId);
				break;
			case 3:
				pHuman->SetStoryTitle(TitleId);
				break;
			case 4:
				pHuman->SetKillBossTitle(TitleId);
				break;
			case 5:
				pHuman->SetTreasureTitle(TitleId);
				break;
			
			default:
				{
					pHuman->SetDyTitle(TitleIndex-6, TitleId);
				}
				break;
			}

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-2
	*     函数名称 : LuaFnDeleteTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleIndex
	*					称号ID
	*     功能说明 :
	*				删除某个称号
	*/
	INT	LuaFnDeleteTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT TitleIndex		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDeleteTitle")
		switch(TitleIndex)
		{
			case 0:
				pHuman->SetLevelTitle(0);
				break;
			case 1:
				pHuman->SetLifeTitle(0);
				break;
			case 2:
				pHuman->SetRankTitle(0);
				break;
			case 3:
				pHuman->SetStoryTitle(0);
				break;
			case 4:
				pHuman->SetKillBossTitle(0);
				break;
			case 5:
				pHuman->SetTreasureTitle(0);
				break;

			default:
				{
					pHuman->SetDyTitle(TitleIndex-6, 0);
				}
				break;
		}


		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnDispatchAllTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				更新此人的所有称号
	*/
	INT	LuaFnDispatchAllTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnDispatchAllTitle")
		//PACKET_ALL_TITLES_MSG
		GCCharAllTitles::TitleId titlelist[MAX_TITLE_ID_SIZE];
		int num = 0;
		if(pHuman->GetLevelTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::LEVEL_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetLevelTitle());
		}
		if(pHuman->GetLifeTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::LIFE_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetLifeTitle());
		}
		if(pHuman->GetRankTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::RANK_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetRankTitle());
		}
		if(pHuman->GetStoryTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::STORY_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetStoryTitle());
		}
		if(pHuman->GetKillBossTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::KILLBOSS_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetKillBossTitle());
		}
		if(pHuman->GetTreasureTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::TREASURE_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetTreasureTitle());
		}

		for(int i = 0; i<MAX_DYNAMIC_TITLE_SIZE; i++ )
		{
			if(pHuman->GetDyTitle(i))
			{
				titlelist[num].m_titleid		=	(_TITLE::DY_TITLE0 + i);
				titlelist[num++].m_titleData	=	pHuman->GetDyTitle(i);
			}
		}

		GCCharAllTitles::TitleString titlestrlist[MAX_STRING_TITLE_SIZE];
		int strnum = 0;
		const CHAR* szTemp = NULL;
		if(strlen(pHuman->GetNickName())!=0)
		{
			szTemp = pHuman->GetNickName();
			titlestrlist[strnum].m_titleid	=	_TITLE::NICK_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(pHuman->GetSpouseName())!=0)
		{
			szTemp = pHuman->GetSpouseName();
			titlestrlist[strnum].m_titleid	=	_TITLE::SPOUSE_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(pHuman->GetBangPaiName())!=0)
		{
			szTemp = pHuman->GetBangPaiName();
			titlestrlist[strnum].m_titleid	=	_TITLE::BANGPAI_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(pHuman->GetJieBaiName())!=0)
		{
			szTemp = pHuman->GetJieBaiName();
			titlestrlist[strnum].m_titleid	=	_TITLE::JIEBAI_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(pHuman->GetShiTuName())!=0)
		{
			szTemp = pHuman->GetShiTuName();
			titlestrlist[strnum].m_titleid	=	_TITLE::SHITU_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(pHuman->GetShangDianName())!=0)
		{
			szTemp = pHuman->GetShangDianName();
			titlestrlist[strnum].m_titleid	=	_TITLE::SHANGDIAN_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}

		GCCharAllTitles Msg;
		Msg.setTargetID(selfId);
		Msg.SetTitleIdNum(num);
		Msg.SetTitleIdList(titlelist);
		Msg.SetTitleStrNum(strnum);
		Msg.SetTitleStrList(titlestrlist);
		//PACKET_ALL_TITLES_MSG

		pHuman->GetPlayer()->SendPacket(&Msg);

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}



	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnGetTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleIndex
	*					称号ID
	*     功能说明 :
	*				获得此人的某个称号
	*/
	INT	LuaFnGetTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT TitleIndex		=	Lua_ValueToNumber(L,3);
		INT	TitleId		=	0;
		BEGINHUMANDEFINE("LuaFnGetTitle")
		switch(TitleIndex)
		{
			case 0:
				TitleId = pHuman->GetLevelTitle();
				break;
			case 1:
				TitleId = pHuman->GetLifeTitle();
				break;
			case 2:
				TitleId = pHuman->GetRankTitle();
				break;
			case 3:
				TitleId = pHuman->GetStoryTitle();
				break;
			case 4:
				TitleId = pHuman->GetKillBossTitle();
				break;
			case 5:
				TitleId = pHuman->GetTreasureTitle();
				break;

			default:
				{
					TitleId = pHuman->GetDyTitle(TitleIndex-6);
				}
				break;
		}

		Lua_PushNumber(L,TitleId);
		return	1;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnGetDyTitlesNum
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得此人的动态称号个数
	*/
	INT	LuaFnGetDyTitlesNum(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT	titlenum	=	0;
		BEGINHUMANDEFINE("LuaFnGetDyTitlesNum")
			for(int i = 0; i<MAX_DYNAMIC_TITLE_SIZE; i++ )
			{
				if(pHuman->GetDyTitle(i))
				{
					titlenum++;
				}
			}
			Lua_PushNumber(L,titlenum);
			return	1;
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnAddDyTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleId
	*					称号ID
	*     功能说明 :
	*				加入新的动态称号，如果满了放在第一个
	*/
	INT	LuaFnAddDyTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t TitleId			=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddDyTitle")
			int i = 0;
			for(i = 0; i<MAX_DYNAMIC_TITLE_SIZE; i++ )
			{
				if(pHuman->GetDyTitle(i) == 0)
				{
					pHuman->SetDyTitle(i, TitleId); 
					break;
				}
			}
			if(i == MAX_DYNAMIC_TITLE_SIZE)
			{//覆盖第一个title
				pHuman->SetDyTitle(0, TitleId); 
			}

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardNickTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					签名档性质的昵称
	*     功能说明 :
	*				设置此人的签名档性质的昵称
	*/
	INT LuaFnAwardNickTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardNickTitle")
			pHuman->SetNickName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardSpouseTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					配偶名
	*     功能说明 :
	*				设置配偶名
	*/
	INT LuaFnAwardSpouseTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardSpouseTitle")
			pHuman->SetSpouseName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardJieBaiTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					结拜称号
	*     功能说明 :
	*				设置结拜称号
	*/
	INT LuaFnAwardJieBaiTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardJieBaiTitle")
			pHuman->SetJieBaiName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}



	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardBangPaiTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					帮派名
	*     功能说明 :
	*				设置此人的帮派称号
	*/
	INT LuaFnAwardBangPaiTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardBangPaiTitle")
			pHuman->SetBangPaiName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE
		
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardShiTuTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					帮派名
	*     功能说明 :
	*				设置此人的帮派称号
	*/
	INT LuaFnAwardShiTuTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardShiTuTitle")
			pHuman->SetShiTuName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardShangDianTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					帮派名
	*     功能说明 :
	*				设置此人的帮派称号
	*/
	INT LuaFnAwardShangDianTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardShangDianTitle")
			pHuman->SetShangDianName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-8
	*     函数名称 : LuaFnEnableBankRentIndex
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				RentIndex
	*					打开的租赁箱
	*     功能说明 :
	*				打开此人的租赁箱
	*/
	INT LuaFnEnableBankRentIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT RentIndex		=	Lua_ValueToNumber(L,3);

		if(RentIndex<0&&RentIndex>5)
			return 0;
		BEGINHUMANDEFINE("LuaFnEnableBankRentIndex")
			//pHuman->SetBangPaiName(strText, (BYTE)strlen(strText));
			INT	CurEndIndex = pHuman->__GetBankEndIndex();
		switch(RentIndex)
		{
		case 1:
			{
			}
			break;
		case 2:
			{
				if(CurEndIndex <= RENTBOX2_START_INDEX)
					pHuman->__SetBankEndIndex(RENTBOX3_START_INDEX);
			}
			break;
		case 3:
			{
				if(CurEndIndex <= RENTBOX3_START_INDEX)
					pHuman->__SetBankEndIndex(RENTBOX4_START_INDEX);
			}
			break;
		case 4:
			{
				if(CurEndIndex <= RENTBOX4_START_INDEX)
					pHuman->__SetBankEndIndex(RENTBOX5_START_INDEX);
			}
			break;
		case 5:
			{
				if(CurEndIndex <= RENTBOX5_START_INDEX)
					pHuman->__SetBankEndIndex(MAX_BANK_SIZE);
			}
			break;
		default:
			{
			}
			break;
		}
		ENDHUMANDEFINE
		return 0;
		LUA_LEAVE_FUNCTION

			return 0 ;

	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-12
	*     函数名称 : LuaFnGetBankRentIndex
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得当前租赁箱已打开的索引
	*/
	INT LuaFnGetBankRentIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetBankRentIndex")
		INT	CurEndIndex = pHuman->__GetBankEndIndex();
		Lua_PushNumber(L, CurEndIndex);
		return 1;
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	INT LuaFnGetRMB(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetRMB")
			INT	CurRMB = pHuman->GetRMB();
		Lua_PushNumber(L, CurRMB);
		return 1;
		ENDHUMANDEFINE
			LUA_LEAVE_FUNCTION

			RETURNFALSE
	}

	INT LuaFnSetRMB(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);
		INT NowRMB					=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetRMB")
			pHuman->SetRMB(NowRMB);
		return 0;
		ENDHUMANDEFINE
			LUA_LEAVE_FUNCTION

			RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-8
	*     函数名称 : LuaFnBankBegin
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				通知客户端开始进行银行交易
	*/
	INT LuaFnBankBegin(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		BEGINHUMANDEFINE("LuaFnBankBegin")
		GCBankBegin Msg;
		Player *pPlayer = pHuman->GetPlayer();
		Msg.SetNPCObjID(targetId);
		pPlayer->SendPacket( &Msg ) ;

		ENDHUMANDEFINE

		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-14
	*     函数名称 : LuaSetPlayerDefaultReliveInfo
	*     参数说明 : 
	*				sceneId
	*					场景编号
	*				playerId
	*					进入场景的玩家ID
	*				pszHP
	*					HP的数据(%10=HP恢复10/100，10=HP恢复10点)
	*				pszMP
	*					MP的数据(%10=HP恢复10/100，10=HP恢复10点)
	*				pszSP
	*					SP的数据(%10=HP恢复10/100，10=HP恢复10点)
	*				defaultSceneID
	*					复活的场景ID
	*				defaultPosX
	*					坐标X
	*				defaultPosZ
	*					坐标Z
	*     功能说明 :
	*				玩家的缺省复活相关的数据设置
	*/
	INT LuaSetPlayerDefaultReliveInfo(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
		SceneID_t sceneId			= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		const char *pszHP			= Lua_ValueToString( L, 3 );
		const char *pszMP			= Lua_ValueToString( L, 4 );
		const char *pszSP			= Lua_ValueToString( L, 5 );
		SceneID_t defaultSceneID	= Lua_ValueToNumber( L, 6 );
		FLOAT defaultPosX		= (FLOAT)(Lua_ValueToNumber( L, 7 ));
		FLOAT defaultPosZ		= (FLOAT)(Lua_ValueToNumber( L, 8 ));

		BEGINHUMANDEFINE("LuaSetPlayerDefaultReliveInfo")

		Obj_Human::_RELIVE_INFO infoRelive;
		infoRelive.m_bHPUsePercent = (pszHP[0] == '%')?(TRUE):(FALSE);
		infoRelive.m_bMPUsePercent = (pszMP[0] == '%')?(TRUE):(FALSE);
		infoRelive.m_bSPUsePercent = (pszSP[0] == '%')?(TRUE):(FALSE);
		if ( infoRelive.m_bHPUsePercent )
		{
			infoRelive.m_nHPPercent	= atoi( &pszHP[1] );
		}
		else
		{
			infoRelive.m_nHP		= atoi( pszHP );
		}

		if ( infoRelive.m_bMPUsePercent )
		{
			infoRelive.m_nMPPercent	= atoi( &pszMP[1] );
		}
		else
		{
			infoRelive.m_nMP		= atoi( pszMP );
		}

		if ( infoRelive.m_bSPUsePercent )
		{
			infoRelive.m_nSPPercent	= atoi( &pszSP[1] );
		}
		else
		{
			infoRelive.m_nSP		= atoi( pszSP );
		}

		infoRelive.m_SceneID		= defaultSceneID;
		infoRelive.m_Pos.m_fX		= defaultPosX;
		infoRelive.m_Pos.m_fZ		= defaultPosZ;

		pHuman->SetReliveInfo( FALSE, &infoRelive );

		ENDHUMANDEFINE
		return	0;

	LUA_LEAVE_FUNCTION

		return 0 ;

	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-20
	*     函数名称 : LuaGetDayTime
	*     参数说明 : 
	*					无
	*     功能说明 :
	*				取得当前时间的天数，例如：1140 表示2001年的第140天
	*										  6099 表示2006年的第99天
	*/
	INT LuaGetDayTime(Lua_State* L )
	{
	LUA_ENTER_FUNCTION

		UINT uRet = g_pTimeManager->GetDayTime() ;

		Lua_PushNumber(L, uRet) ;
		return	1;

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-23
	*     函数名称 : LuaGetObjCreateTime
	*     参数说明 : 
	*				sceneId
    *					调用该函数场景ID,short类型
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				取得ObjID 为 selfId的obj的创建时间
	*					
	*/
	INT LuaGetObjCreateTime(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );

		BEGINSCENEDEFINE("LuaGetObjCreateTime")
		Obj* pObj = (Obj*)(pScene->GetObjManager()->GetObj(selfId));
		if( pObj )
		{
			UINT uRet = pObj->GetCreateTime() ;
			Lua_PushNumber(L, uRet) ;
			return	1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-28
	*     函数名称 : LuaFnUpdateAttr
	*     参数说明 : 
	*				sceneId
	*					调用该函数场景ID,short类型
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				通过邮件系统更新人物属性
	*					
	*/
	INT LuaFnUpdateAttr(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		UINT param0		= Lua_ValueToNumber( L, 3 );	 
		UINT param1		= Lua_ValueToNumber( L, 4 );
		UINT param2		= Lua_ValueToNumber( L, 5 );
		UINT param3		= Lua_ValueToNumber( L, 6 );

		BEGINHUMANDEFINE("LuaFnUpdateAttr")
		switch(param1)
		{
		case MAIL_ATTR_GUILD:
			{
				pHuman->SetGuildID(param2);
				if(param2 == INVALID_ID)
				{
					pHuman->SetBangPaiName("",0);
					pHuman->UpdateTitlesToClient();
				}
			}
			break;
		case MAIL_ATTR_MONEY:
			{
				pHuman->SetMoney(pHuman->GetMoney()+(INT)param2);
				//记录
				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	param3;
				MoneyLogParam.TargetGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_BUY_PLAYERSHOP_OUTCOME;	
				MoneyLogParam.Count		=	param2;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);
			}
		default:
			break;
		}

		ENDHUMANDEFINE
		return 0 ;
		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-26	17:08
	 *	函数名称：	LuaFnIsPasswordSetup
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				flag
	 *					标记，仅当玩家未设置二级密码有效，如果 1，则让调用者设置二级密码，0 则不需要设置
	 *
	 *	功能说明：	判断玩家是否设置二级密码，0 为未设置，1 为已设置
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsPasswordSetup(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		BOOL flag		= Lua_ValueToNumber( L, 3 );	 

		BEGINHUMANDEFINE("LuaFnIsPasswordSetup")

			if( pHuman->__IsPasswordSetup() )
			{
				Lua_PushNumber(L, TRUE);
			}
			else
			{
				if( flag )
				{ // 调用接口发消息 OR_NEED_SETMINORPASSWORD
					pHuman->SendOperateResultMsg( OR_NEED_SETMINORPASSWORD );
				}

				Lua_PushNumber(L, FALSE);
			}

			return 1;

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-26	17:21
	 *	函数名称：	LuaFnIsPasswordUnlocked
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				flag
	 *					标记，仅当玩家未解锁二级密码有效，如果 1，则让调用者解锁二级密码，0 则不强制解锁
	 *
	 *	功能说明：	判断玩家是否解锁二级密码，0 为未解锁，1 为已解锁
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsPasswordUnlocked(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		BOOL flag		= Lua_ValueToNumber( L, 3 );	 

		BEGINHUMANDEFINE("LuaFnIsPasswordUnlocked")

			if( pHuman->__IsPasswordUnlock() )
			{
				Lua_PushNumber(L, TRUE);
			}
			else
			{
				if( flag )
				{ // 调用接口发消息 OR_NEED_UNLOCKMINORPASSWORD
					pHuman->SendOperateResultMsg( OR_NEED_UNLOCKMINORPASSWORD );
				}

				Lua_PushNumber(L, FALSE);
			}

			return 1;

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	
	 *	函数名称：	LuaFnSetCharacterTimer
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				nTime
	 *					时间间隔
	 *
	 *	功能说明：	设置 Character的时钟间隔，如果是0则，停止时钟
	 *				每次心跳发生时：如果是玩家，则会运行 Scene.lua里面的OnScenePlayerTimer函数
	 *				如果是宠物或者怪物，则会运行所挂脚本的 OnCharacterTimer函数
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSetCharacterTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
	    ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
	    INT nTime			= Lua_ValueToNumber( L, 3 );

		BEGINCHARCTERDEFINE("LuaFnSetCharacterTimer")
			pCharacter->SetScriptTimer( nTime ) ;
			return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	
	 *	函数名称：	LuaFnSetCharacterDieTime
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					Monster ObjID，无符号整型
	 *				nTime
	 *					时间间隔,单位（毫秒）
	 *
	 *	功能说明：	设置 Character的消失时间, 只对Monster和Pet有效
	 *				怪物消失的时候不会产生掉落及任务经验分配
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSetCharacterDieTime(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
	    ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		INT nTime			= Lua_ValueToNumber( L, 3 );

		BEGINCHARCTERDEFINE("LuaFnSetCharacterDieTime")
			if( pCharacter->GetObjType()==Obj::OBJ_TYPE_MONSTER 
				|| pCharacter->GetObjType()==Obj::OBJ_TYPE_PET )
			{
				pCharacter->SetDieTime( nTime ) ;
			}
			return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}












}

#endif
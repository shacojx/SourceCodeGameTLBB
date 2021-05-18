//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Team.h
// 模块	:	Script
// 功能	 :	组队相关导出函数
// 修改历史:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_TEAM_H__
#define __LUAFNTBL_TEAM_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "SceneManager.h"
#include "HumanItemLogic.h"
#include "MonsterManager.h"
#include "Obj_Monster.h"
#include "Config.h"
#include "ScanOperator_ActiveTeammates.h"
#include "GCChat.h"


namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetTeamId
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				返回玩家队伍ID
	*				没有测试
	*/
	INT LuaFnGetTeamId(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetTeamId")
		Lua_PushNumber(L,pHuman->__GetTeamID());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetTeamSize
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				返回玩家所在队伍人数
	*				没有测试
	*/
	INT LuaFnGetTeamSize(Lua_State* L)	
	{	
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetTeamSize")
		Lua_PushNumber(L,pHuman->__GetTeamMemberCount());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnLeaveTeam
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				让玩家离开自身队伍
	*				没有测试
	*/
	INT LuaFnLeaveTeam(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	ObjID_t selfId = Lua_ValueToNumber(L,2);

	BEGINHUMANDEFINE("LuaFnLeaveTeam")
	pHuman->GetTeamInfo()->DelMember(pHuman->GetGUID());
	Lua_PushNumber(L,1);
	return 1;
	ENDHUMANDEFINE
	RETURNFALSE
	
	LUA_LEAVE_FUNCTION
	
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnMsg2Player
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				msg
	*					聊天内容,字符串指针类型
	*				type
	*					CHAT_TYPE_NORMAL =0 ,	//普通说话消息
	*					CHAT_TYPE_TEAM = 1,		//队聊消息
	*					CHAT_TYPE_SCENE = 2,		//场景消息
	*					CHAT_TYPE_TELL = 3,		//私聊消息
	*					CHAT_TYPE_SYSTEM = 4,		//系统消息
	*					CHAT_TYPE_CHANNEL = 5,		//自建聊天频道消息
	*					CHAT_TYPE_GUILD = 6,		//帮派消息
	*					CHAT_TYPE_MENPAI = 7 ,		//门派消息
	*					CHAT_TYPE_SELF = 8,			//仅客户端使用的消息
	*					
	*     功能说明 :
	*				将消息发送给玩家
	*				消息显示在左下角的聊天框内
	*				type默认值为CHAT_TYPE_NORMAL,即0
	*
	*				金钱提示信息，需要写成#{_MONEY%d}方式
	*				例如：你获得了#{_MONEY208934}。 
	*				这句话在客户端提示的时候会自动计算出金、银、铜的数量，并用对应的图标表示出来。
	*				
	*/
	INT LuaFnMsg2Player(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		CHAR*msg = const_cast<CHAR*>(Lua_ValueToString(L,3));
		ENUM_CHAT_TYPE type;
		
		if(Lua_GetTopIndex(L) < 4)
			type = CHAT_TYPE_NORMAL;
		else
			type = (ENUM_CHAT_TYPE)((INT)(Lua_ValueToNumber(L,4)));
		
		BEGINHUMANDEFINE("LuaFnMsg2Player")
		GCChat Msg ;
		INT Size = (INT)strlen(msg) ;
		Msg.SetChatType( type) ;
		Msg.SetContexSize( Size ) ;
		Msg.SetContex( msg ) ;
		Msg.SetSourNameSize( (BYTE)strlen(pHuman->GetName())) ;
		Msg.SetSourName( pHuman->GetName()) ;
		Msg.SetSourID(pHuman->GetID()) ;
		Msg.SetSourCamp(pHuman->GetCampData()->m_nCampID);

		pHuman->GetPlayer()->SendPacket( &Msg ) ;

		Lua_PushNumber(L,1);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnMsg2Team
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				msg
	*					聊天内容,字符串指针类型
	*     功能说明 :
	*				将消息发送给玩家所在组的所有成员
	*				函数体尚未填写
	*/
	INT LuaFnMsg2Team(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		GUID_t selfId; selfId;
		const CHAR *msg; msg;
		LUA_LEAVE_FUNCTION
		return 0 ;
		
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-1-17	17:53
	 *	函数名称：	LuaFnIsTeamFollow
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到 selfId 对应的玩家是否处于组队跟随状态
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsTeamFollow(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnIsTeamFollow")
		Lua_PushNumber(L, pHuman->__GetTeamFollowFlag());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnGetFollowedMembersCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				得到跟随状态的队友数量
	*				没有测试
	*/
	INT LuaFnGetFollowedMembersCount(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetFollowedMembersCount")
		Lua_PushNumber(L,pHuman->__GetFollowedMembersCount());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     函数名称 : LuaFnGetFollowedMember
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				memberIndex
	*					在跟随队伍中的下标索引,从0起始,整型			

	*     功能说明 :
	*				得到跟随状态的memberIndex下标的队友objId
	*				没有测试
	*/
	INT LuaFnGetFollowedMember(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT memberIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetFollowedMember")
			Obj_Human* pMember = pHuman->__GetFollowedMember(memberIndex)->m_pHuman;
			if( pMember!=NULL && pMember->GetGUID()==pHuman->__GetFollowedMember(memberIndex)->m_GUID )
			{
				Lua_PushNumber(L, (INT)pMember->GetID());
				return 1 ;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnStopTeamFollow
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				停止跟随,针对selfId标志的玩家
	*				没有测试
	*/
	INT LuaFnStopTeamFollow(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnStopTeamFollow")
		pHuman->__StopTeamFollow();
		return 0;
		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnClearFollowedMembers
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				清除所有的跟随队友列表
	*				没有测试
	*/
	INT LuaFnClearFollowedMembers(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnClearFollowedMembers")
		
		pHuman->__ClearFollowedMembers();
		return 0;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnGetTeamSceneMemberCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				队伍中同场景成员数量 human
	*				没有测试
	*/
	INT LuaFnGetTeamSceneMemberCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetTeamSceneMemberCount")
		
		Lua_PushNumber(L,pHuman->__GetTeamSceneMemberCount());
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnGetTeamSceneMember
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型	
	*				memberIndex
	*					在队伍中的下标索引,从0起始,整型			
	*     功能说明 :
	*				取得队伍中某同场景成员的 OBJID
	*				没有测试
	*/
	INT LuaFnGetTeamSceneMember(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	
		INT memberIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetTeamSceneMember")
		
		Lua_PushNumber(L,pHuman->__GetTeamSceneMember(memberIndex));
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnSetTeamFollowSpeed
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				设置组队跟随的速度
	*				没有测试
	*/
	INT LuaFnSetTeamFollowSpeed(Lua_State* L)
	{

		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		FLOAT fSpeed = Lua_ValueToNumber(L,3);
	
		BEGINHUMANDEFINE("LuaFnSetTeamFollowSpeed")
		
		pHuman->__SetTeamFollowSpeed(fSpeed);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnIsTeamLeader
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				selfId玩家是否为队长
	*				返回1,是
	*				返回0,否
	*				没有测试
	*/
	INT LuaFnIsTeamLeader(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINHUMANDEFINE("LuaFnIsTeamLeader")
		
		Lua_PushNumber(L,pHuman->__IsTeamLeader());
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnGetTeamMemberGuid
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型	
	*				memberIndex
	*					在队伍中的下标索引,从0起始,整型			
	*     功能说明 :
	*				获得selfId所在Team中第memberIndex个下标玩家的Guid
	*				没有测试
	*/
	INT LuaFnGetTeamMemberGuid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	
		INT memberIndex = Lua_ValueToNumber(L,3);	

		BEGINHUMANDEFINE("LuaFnGetTeamMemberGuid")
		
		Lua_PushNumber(L,pHuman->__GetTeamMember(memberIndex)->m_GUID);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-10
	*     函数名称 : LuaFnHasTeam
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型	
	*     功能说明 :
	*				判断selfId玩家是否在组队状态
	*				没有测试
	*/
	INT LuaFnHasTeam(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINHUMANDEFINE("LuaFnHasTeam")
		
		Lua_PushNumber(L,pHuman->GetTeamInfo()->HasTeam());
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-17
	*     函数名称 : LuaFnGetNearTeamCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型	
	*     功能说明 :
	*				取得selfId玩家附近的队友数量(包括自己)
	*				如果玩家不属于某个队伍，则返回-1
	*/
	INT LuaFnGetNearTeamCount(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINHUMANDEFINE("LuaFnGetNearTeamCount")
		if( !pHuman->GetTeamInfo()->HasTeam() )
		{
			RETURNFALSE
		}
		SCANOPERATOR_ACTIVETEAMMATES_INIT	tInit;
		tInit.m_TeamID		=	pHuman->__GetTeamID() ;
		tInit.m_MemberGUID	=	pHuman->GetGUID() ;
		tInit.m_fRadius		=	g_Config.m_ConfigInfo.m_CanGetExpRange;
		tInit.m_nZoneRadius	=	MAX_RADIUS;
		tInit.m_bScanHuman	=	TRUE;
		tInit.m_Position	=	*pHuman->getWorldPos();
		tInit.m_ZoneID		=	pHuman->getZoneID();
		tInit.m_pScene		=	pScene;

		ScanOperator_ActiveTeammates	ScanOp;
		ScanOp.Init(&tInit);
		pScene->Scan(&ScanOp);
	
		pScene->m_aHumanList.m_Count = ScanOp.m_nActiveTeammateCount ;
		for( INT i=0; i<ScanOp.m_nActiveTeammateCount; i++ )
		{
			pScene->m_aHumanList.m_aHuman[i] = ScanOp.m_aActiveTeammate[i] ;
		}

 		Lua_PushNumber(L,pScene->m_aHumanList.m_Count) ;
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-10
	*     函数名称 : LuaFnGetNearTeamMember
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型	
	*				index
	*					附近队友的索引号
	*     功能说明 :
	*				取得selfId玩家附近队友的objid(包括自己)
	*/
	INT LuaFnGetNearTeamMember(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	
		INT index = Lua_ValueToNumber(L,3);	

		BEGINHUMANDEFINE("LuaFnGetNearTeamMember")
		if( index>=pScene->m_aHumanList.m_Count || index<0 )
		{
			RETURNFALSE
		}
		Obj_Human* pTeammate = pScene->m_aHumanList.m_aHuman[index] ;
		if( pTeammate==NULL )
		{
			RETURNFALSE
		}
		
		Lua_PushNumber(L,pTeammate->GetID());
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-33
	*     函数名称 : LuaFnGetMonsterGroupID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					需要取得数据的怪物的objid
	*     功能说明 :
	*				取得selfId怪物的GroupID, monster.ini里面配置
	*/
	INT LuaFnGetMonsterGroupID(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINMONSTERDEFINE("LuaFnGetMonsterGroupID")
		
		Lua_PushNumber(L,pMonster->GetGroupID());
		return 1;

		ENDMONSTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-33
	*     函数名称 : LuaFnGetMonsterTeamID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					需要取得数据的怪物的objid
	*     功能说明 :
	*				取得selfId怪物的TeamID, monster.ini里面配置
	*/
	INT LuaFnGetMonsterTeamID(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINMONSTERDEFINE("LuaFnGetMonsterTeamID")
		
		Lua_PushNumber(L,pMonster->GetTeamID());
		return 1;

		ENDMONSTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-23
	*     函数名称 : LuaFnGetMonsterDataID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					需要取得数据的怪物的objid
	*     功能说明 :
	*				取得selfId怪物的DataID, monster.ini里面配置
	*/
	INT LuaFnGetMonsterDataID(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINMONSTERDEFINE("LuaFnGetMonsterDataID")
		
		Lua_PushNumber(L,pMonster->GetDataID());
		return 1;

		ENDMONSTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


}

#endif
	

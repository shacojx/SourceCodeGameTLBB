#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "../network/netmanager.h"
#include "LuaPlus.h"
#include "../Event/GMEventSystem.h"
#include "../DataPool/GMDataPool.h"
#include "../input/gminputsystem.h"
#include "../object/objectmanager.h"
#include "../object/character/obj_playermyself.h"
#include "../datapool/gmdp_characterdata.h"
#include "cgmail.h"
#include "gamestruct.h"
#include "../datapool/GMDP_Struct_Mail.h"
#include "cgteamapply.h"
#include "CGTeamInvite.h"
#include "tddbc_struct.h"
#include "..\dbc\GMDataBase.h"
#include "../Procedure/GameProcedure.h"
#include "../DataPool/GMDP_Struct_Relation.h"

#include "CGPlayerCallOfResult.h"
#include "CGSetMoodToHead.h"

namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	Friend Friend::s_Friend;
	LuaPlus::LuaObject* Friend::s_pMetaTable = NULL;
	int Friend::m_nCurTeam = 0;
	int Friend::m_nCurSelect = 0;
	INT	Friend::SetCurrentSelect( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);
		if( !args[2].IsInteger() ) return 0;
		m_nCurSelect = args[ 2 ].GetInteger();
		return 0;
	}
	INT Friend::GetCurrentSelect( LuaPlus::LuaState* state )
	{
		state->PushInteger( m_nCurSelect );
		return 1;
	}
	INT	Friend::SetCurrentTeam( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);
		if( !args[2].IsInteger() ) return 0;
		m_nCurTeam = args[ 2 ].GetInteger();
		return 0;
	}
	INT	Friend::GetCurrentTeam( LuaPlus::LuaState* state )
	{
		if( m_nCurTeam == RELATION_GROUP_BLACK )
		{
			for( int i = 0; i < RELATION_GROUP_BLACK; i ++ )
			{
				if( CDataPool::GetMe()->GetRelation()->GetListCount( RELATION_GROUP(i) ) > 20 )
					continue;
				state->PushInteger( i );
				break;
			}
		}
		else
			state->PushInteger( m_nCurTeam );
		return 1;
	}
	//关闭
	INT Friend::Close(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_FRIEND_INFO, -1 );
		return 0;	
	}

	INT Friend::OpenGrouping( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		//得到鼠标位置
		POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();
		std::vector< STRING > vParam;
		CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
		CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);
		vParam.push_back("groupingmenu");
		vParam.push_back("-1");
		vParam.push_back( args[ 2 ].GetString() );
		vParam.push_back( args[ 3 ].GetString() );
		vParam.push_back(szXPos);
		vParam.push_back(szYPos);
		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
		return 0;
	}


	INT Friend::OpenMenu( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		//得到鼠标位置
		POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();
		std::vector< STRING > vParam;
		CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
		CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);
		vParam.push_back("friendmenu");
		vParam.push_back("-1");
		vParam.push_back( args[ 2 ].GetString() );
		vParam.push_back( args[ 3 ].GetString() );
		vParam.push_back(szXPos);
		vParam.push_back(szYPos);
		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
		return 0;
	}
	
	INT Friend::EnumName(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		//检查参数
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: Friend::EnumFriend[2] Wrong Param1");
		}
		if (!(args[3].IsString()))
		{
			TDThrow("LUA: Friend::EnumFriend[3] Wrong Param1");
		}

		INT nIndex = args[2].GetInteger();
		std::string szType = args[3].GetString();

		_OWN_RELATION Relation;

		if( szType == "friend" )
		{
			CDataPool::GetMe()->Relation_GetFriend(nIndex,&Relation);
			state->PushString(Relation.m_Member.m_szMemberName);
		}
		else if( szType == "black" )
		{
			CDataPool::GetMe()->Relation_GetBlack(nIndex,&Relation);
			state->PushString(Relation.m_Member.m_szMemberName);
		}
		else
		{
			state->PushString("");
		}
		return 1;
	}
	INT Friend::AskTeam(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if( !args[ 2 ].IsString() ) return 0;
		CGTeamApply msg;
		msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
		msg.SetDestName( args[ 2 ].GetString() );
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	INT Friend::InviteTeam(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if( !args[ 2 ].IsString() ) return 0;
		CGTeamInvite msg;
		msg.SetSourObjID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
		msg.SetDestName(args[ 2 ].GetString());
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}

	// 得到有多少条消息
	INT Friend::GetHistroyNumber( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if( !args[2].IsInteger() ) return 0;
		if( !args[3].IsInteger() ) return 0; // 第几组的第几个人
		int nNumber = CDataPool::GetMe()->GetRelation()->GetMailCount( ( RELATION_GROUP )args[2].GetInteger(), args[3].GetInteger() );
		state->PushInteger( nNumber );
		return 1; 
	}
	// 得到历史数据
	INT Friend::GetHistroyData(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if( !args[2].IsInteger() ) return 0;
		if( !args[3].IsInteger() ) return 0; // 第几组的第几个人
		if( !args[4].IsInteger() ) return 0; // 第几组的第几个人
		if( !args[5].IsString() ) return 0; // 是什么数据
		int nIndex = CDataPool::GetMe()->GetRelation()->GetMailIndex(( RELATION_GROUP )args[2].GetInteger(), args[3].GetInteger(), args[ 4 ].GetInteger() );
		const SMail* pMail = CDataPool::GetMe()->GetMailPool()->GetMail( nIndex );
		if( pMail )
		{
			std::string str = args[5].GetString();
			if( str == "CONTEX" )// GUID
			{
				state->PushString( pMail->m_szContex );
			}
			else if( str == "TIME" )
			{
				state->PushString( pMail->m_szCreateTime );
			}
			else if( str == "SENDER" )
			{
				state->PushString( pMail->m_szSourName );
			}
			else if( str == "RECIVER" )
			{
				state->PushString( pMail->m_szDestName );
			}
			else
				state->PushString( "-1" );
		}
		return 1;
	}
	//拉人界面的一个“同意”和“不同意”处理
	INT Friend::CallOf(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if( !args[2].IsString() ) return 0;

		std::string str = args[2].GetString();

		CGPlayerCallOfResult msg;


		if(str == "ok")
		{
			msg.SetResultCode(CALLOF_RESULT_CODE_ACCEPT);
		}
		else
		{
			msg.SetResultCode(CALLOF_RESULT_CODE_REFUSE);
		}
		
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	// 点击显示心情的操作
	INT Friend::ViewFeel(LuaPlus::LuaState* state)
	{
		INT nView = 1;
		if( _TITLE::MOOD_TITLE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() )
		{
			nView = 0;
		}

		//通知服务器，将心情的
		CGSetMoodToHead msg;
		msg.SetView(nView);

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//测试心情是不是在自己的头顶
	INT Friend::IsMoodInHead(LuaPlus::LuaState* state)
	{
		if( _TITLE::MOOD_TITLE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}
		return 1;
	}

	INT Friend::IsPlayerIsFriend(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		int nTmpGroup = -1, nIndex = -1;
		if( CDataPool::GetMe()->GetRelation()->GetRelationByName( args[ 2 ].GetString(), nTmpGroup , nIndex ) == RELATION_TYPE_STRANGER )
			state->PushInteger( 0 );
		else
			state->PushInteger( 1 );
		return 1;
	}
};


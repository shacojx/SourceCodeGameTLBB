#include "stdafx.h"
// GCScriptCommand.cpp
// 
/////////////////////////////////////////////////////

#include "GCScriptCommand.h"


using namespace Packets;

UINT GCScriptCommand::GetPacketSize()const
{
	UINT uSize = sizeof( m_nCmdID );
	switch ( m_nCmdID )
	{
	case SCRIPT_COMMAND_EVENT_LIST_RESPONSE:
		uSize += m_paramEnentList.GetBufSize();
		break;
	case SCRIPT_COMMAND_MISSION_RESPONSE:
		uSize += m_paramMissionInfo.GetBufSize();
		break;
	case SCRIPT_COMMAND_MISSION_REGIE:
		uSize += m_paramMissionRegie.GetBufSize();
		break;
	case SCRIPT_COMMAND_MISSION_DEMAND_RESPONSE:
		uSize += m_paramMissionDemandInfo.GetBufSize();
		break;
	case SCRIPT_COMMAND_MISSION_CONTINUE_RESPONSE:
		uSize += m_paramMissionContinueInfo.GetBufSize();
		break;
	case SCRIPT_COMMAND_MISSION_TIPS:
		uSize += m_paramMissionTips.GetBufSize();
		break;
	case SCRIPT_COMMAND_TRADE:
		uSize += m_paramTrade.GetBufSize();
		break;
	case SCRIPT_COMMAND_SKILL_STUDY:
		uSize += m_paramSkillStudy.GetBufSize();
		break;
	default:
		break;
	}
	return	uSize;
}

BOOL GCScriptCommand::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nCmdID), sizeof(m_nCmdID));
	switch ( m_nCmdID )
	{
	case SCRIPT_COMMAND_EVENT_LIST_RESPONSE:
		m_paramEnentList.Read( iStream );
		break;
	case SCRIPT_COMMAND_MISSION_RESPONSE:
		m_paramMissionInfo.Read( iStream );
		break;
	case SCRIPT_COMMAND_MISSION_REGIE:
		m_paramMissionRegie.Read( iStream );
		break;
	case SCRIPT_COMMAND_MISSION_DEMAND_RESPONSE:
		m_paramMissionDemandInfo.Read( iStream );
		break;
	case SCRIPT_COMMAND_MISSION_CONTINUE_RESPONSE:
		m_paramMissionContinueInfo.Read( iStream );
		break;
	case SCRIPT_COMMAND_MISSION_TIPS:
		m_paramMissionTips.Read( iStream );
		break;
	case SCRIPT_COMMAND_TRADE:
		m_paramTrade.Read( iStream );
		break;
	case SCRIPT_COMMAND_SKILL_STUDY:
		m_paramSkillStudy.Read( iStream );
		break;
	default:
		break;
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCScriptCommand::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nCmdID), sizeof(m_nCmdID));
	switch ( m_nCmdID )
	{
	case SCRIPT_COMMAND_EVENT_LIST_RESPONSE:
		m_paramEnentList.Write( oStream );
		break;
	case SCRIPT_COMMAND_MISSION_RESPONSE:
		m_paramMissionInfo.Write( oStream );
		break;
	case SCRIPT_COMMAND_MISSION_REGIE:
		m_paramMissionRegie.Write( oStream );
		break;
	case SCRIPT_COMMAND_MISSION_DEMAND_RESPONSE:
		m_paramMissionDemandInfo.Write( oStream );
		break;
	case SCRIPT_COMMAND_MISSION_CONTINUE_RESPONSE:
		m_paramMissionContinueInfo.Write( oStream );
		break;
	case SCRIPT_COMMAND_MISSION_TIPS:
		m_paramMissionTips.Write( oStream );
		break;
	case SCRIPT_COMMAND_TRADE:
		m_paramTrade.Write( oStream );
		break;
	case SCRIPT_COMMAND_SKILL_STUDY:
		m_paramSkillStudy.Write( oStream );
		break;
	default:
		break;
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCScriptCommand::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCScriptCommandHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


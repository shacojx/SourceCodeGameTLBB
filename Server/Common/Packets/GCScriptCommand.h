// GCScriptCommand.h
// 
// 脚本指令
// 
//////////////////////////////////////////////////////

#ifndef __GCSCRIPTCOMMAND_H__
#define __GCSCRIPTCOMMAND_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "ScriptCommandStruct.h"

namespace Packets
{
#define MAX_SCRIPT_CMD_BUF_LEN	(2560)
	class GCScriptCommand : public Packet
	{
	public:
		GCScriptCommand( ){
			m_nCmdID		= -1;
		}
		virtual ~GCScriptCommand( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SCRIPTCOMMAND ; }
		virtual UINT			GetPacketSize()const;

	public:
		VOID SetEventListResponseCmd( const ScriptParam_EventList *pEventList ){
			m_nCmdID			= SCRIPT_COMMAND_EVENT_LIST_RESPONSE;
			m_paramEnentList	= *pEventList;
		}

		VOID SetMissionInfoResponseCmd( const ScriptParam_MissionInfo *pMissionInfo ){
			m_nCmdID			= SCRIPT_COMMAND_MISSION_RESPONSE;
			m_paramMissionInfo	= *pMissionInfo;
		}

		VOID SetMissionRegieCmd( const ScriptParam_MissionRegie *pMissionRegie ){
			m_nCmdID			= SCRIPT_COMMAND_MISSION_REGIE;
			m_paramMissionRegie	= *pMissionRegie;
		}

		VOID SetMissionDemandInfoResponseCmd( const ScriptParam_MissionDemandInfo *pMissionDemandInfo ){
			m_nCmdID					= SCRIPT_COMMAND_MISSION_DEMAND_RESPONSE;
			m_paramMissionDemandInfo	= *pMissionDemandInfo;
		}

		VOID SetMissionContinueInfoResponseCmd( const ScriptParam_MissionContinueInfo *pMissionContinueInfo ){
			m_nCmdID					= SCRIPT_COMMAND_MISSION_CONTINUE_RESPONSE;
			m_paramMissionContinueInfo	= *pMissionContinueInfo;
		}

		VOID SetMissionTipsCmd( const ScriptParam_MissionTips *pMissionTips ){
			m_nCmdID					= SCRIPT_COMMAND_MISSION_TIPS;
			m_paramMissionTips			= *pMissionTips;
		}

		VOID SetTradeCmd( const ScriptParam_Trade *pTrade ){
			m_nCmdID					= SCRIPT_COMMAND_TRADE;
			m_paramTrade				= *pTrade;
		}

		VOID SetSkillStudyCmd( const ScriptParam_SkillStudy *pSkillStudy ){
			m_nCmdID					= SCRIPT_COMMAND_SKILL_STUDY;
			m_paramSkillStudy			= *pSkillStudy;
		}

		INT getCmdID( VOID )const{
			return m_nCmdID;
		}

		VOID *getBuf( VOID ){
			return &m_paramEnentList;
		}

		const ScriptParam_EventList *GetEventList( VOID )const{
			return &m_paramEnentList;
		}

		const ScriptParam_MissionInfo *GetMissionInfo( VOID )const{
			return &m_paramMissionInfo;
		}

		const ScriptParam_MissionRegie *GetMissionRegie( VOID )const{
			return &m_paramMissionRegie;
		}

		const ScriptParam_MissionDemandInfo *GetMissionDemandInfo( VOID )const{
			return &m_paramMissionDemandInfo;
		}

		const ScriptParam_MissionContinueInfo *GetMissionContinueInfo( VOID )const{
			return &m_paramMissionContinueInfo;
		}

		const ScriptParam_MissionTips *GetMissionTips( VOID )const{
			return &m_paramMissionTips;
		}

		const ScriptParam_Trade *GetTrade( VOID )const{
			return &m_paramTrade;
		}
		
		const ScriptParam_SkillStudy *GetSkillStudy( VOID )const{
			return &m_paramSkillStudy;
		}
	private:
		INT										m_nCmdID;		// ENUM_SCRIPT_COMMAND
		union{
			ScriptParam_EventList				m_paramEnentList;
			ScriptParam_MissionInfo				m_paramMissionInfo;
			ScriptParam_MissionRegie			m_paramMissionRegie;
			ScriptParam_MissionDemandInfo		m_paramMissionDemandInfo;
			ScriptParam_MissionContinueInfo		m_paramMissionContinueInfo;
			ScriptParam_MissionTips				m_paramMissionTips;
			ScriptParam_Trade					m_paramTrade;
			ScriptParam_SkillStudy				m_paramSkillStudy;
		};
	};

	class GCScriptCommandFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCScriptCommand() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SCRIPTCOMMAND; }
		UINT		GetPacketMaxSize()const { return	sizeof(INT) + MAX_SCRIPT_CMD_BUF_LEN; }
	};

	class GCScriptCommandHandler 
	{
	public:
		static UINT Execute( GCScriptCommand* pPacket, Player* pPlayer ) ;
	};
}

#endif

// GCStudyXinfa.h
// 
// 发送NPC的门派ID
// 
//////////////////////////////////////////////////////

#ifndef __GCSTUDYXINFA_H__
#define __GCSTUDYXINFA_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCStudyXinfa : public Packet
	{
	public:
		struct _STUDERESULT_INFO   //学习后的结果（改变了什么）
		{
			UINT					m_uSpareMoney;			//	升级后玩家剩余的金钱
			UINT					m_uSpareExperience;		//	升级后玩家剩余的经验
			
			short					m_idXinfa;
			short					m_StudedLevel;			// 升级之后的等级
		};

		GCStudyXinfa( ){};
		virtual ~GCStudyXinfa( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STUDYXINFA_H; }
		virtual UINT			GetPacketSize()const { return	sizeof(_STUDERESULT_INFO);}

	public:
		_STUDERESULT_INFO		GetStudeResultInfo(VOID) const {return m_StudeResult;};
		VOID					SetStudeResultInfo(_STUDERESULT_INFO *StudeResult) 
		{
			m_StudeResult.m_uSpareMoney		= StudeResult->m_uSpareMoney;
			m_StudeResult.m_uSpareExperience	= StudeResult->m_uSpareExperience;
			m_StudeResult.m_idXinfa				= StudeResult->m_idXinfa;
			m_StudeResult.m_StudedLevel			= StudeResult->m_StudedLevel;
		}

	private:

		_STUDERESULT_INFO			m_StudeResult;

	};


	class GCStudyXinfaFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStudyXinfa() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STUDYXINFA_H; };
		UINT		GetPacketMaxSize()const { return sizeof(GCStudyXinfa::_STUDERESULT_INFO);};
	};

	class GCStudyXinfaHandler 
	{
	public:
		static UINT Execute( GCStudyXinfa* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif



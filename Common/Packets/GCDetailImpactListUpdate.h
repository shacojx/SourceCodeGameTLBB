// GCDetailImpactListUpdate.h
// 
// 效果表更新
// 
//////////////////////////////////////////////////////

#ifndef __GCDETAILIMPACTLISTUPDATE_H__
#define __GCDETAILIMPACTLISTUPDATE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCDetailImpactListUpdate : public Packet
	{
	public:
		typedef struct DetailImpact
		{
			enum
			{
				ATTACHED_PARAM_NUMBER = BUFF_IMPACT_INFO_PARAM_NUMBER,
			};
			VOID CleanUp(VOID)
			{
				m_nSenderID			= INVALID_ID;
				m_nSkillID			= INVALID_ID;
				m_nBuffID			= INVALID_ID;
				m_nSN				= 0;
				m_nContinuance		= -1;
			};
			ObjID_t			m_nSenderID;	// 效果释放者的ID
			ImpactID_t		m_nBuffID;		// 特效数据的ID(索引)
			SkillID_t		m_nSkillID;  	// Skill ID
			UINT			m_nSN;			// 效果序列号
			Time_t			m_nContinuance; // 效果的持续时间
			
		} ImpactStruct_T;
		GCDetailImpactListUpdate(VOID)
		{
			CleanUp();
		};
		VOID CleanUp(VOID)
		{
			m_nOwnerID = INVALID_ID;
			m_nNumOfImpacts = 0;
			for(int idx=0; MAX_IMPACT_NUM>idx; ++idx)
			{
				m_aImpact[idx].CleanUp();
			}
		};
		virtual ~GCDetailImpactListUpdate(VOID){};
	
		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);
	
		virtual PacketID_t		GetPacketID(VOID)const {return PACKET_GC_DETAIL_IMPACT_LIST_UPDATE;}
		virtual UINT			GetPacketSize(VOID)const {return sizeof(m_nOwnerID) + sizeof(m_nNumOfImpacts) + sizeof(ImpactStruct_T) * m_nNumOfImpacts;}
	
	public:
		VOID					SetOwnerID(ObjID_t const nID) {m_nOwnerID=nID;}
		ObjID_t					GetOwnerID(VOID) const {return m_nOwnerID;}
		VOID					SetImpactList(_IMPACT_LIST const& rList);
		SHORT					NumOfImpacts(VOID) const {return m_nNumOfImpacts;};
		GCDetailImpactListUpdate::ImpactStruct_T const*	GetImpactList(VOID) const {return m_aImpact;};
	private:
		ObjID_t			m_nOwnerID;
		SHORT			m_nNumOfImpacts;
		ImpactStruct_T	m_aImpact[MAX_IMPACT_NUM];
	};
	
	class GCDetailImpactListUpdateFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() {return new GCDetailImpactListUpdate();}
		PacketID_t	GetPacketID()const {return PACKET_GC_DETAIL_IMPACT_LIST_UPDATE;}
		UINT		GetPacketMaxSize()const {return	sizeof(ObjID_t) + sizeof(SHORT) +	sizeof(GCDetailImpactListUpdate::ImpactStruct_T)* MAX_IMPACT_NUM;}
	};
	
	class GCDetailImpactListUpdateHandler 
	{
	public:
		static UINT Execute( GCDetailImpactListUpdate* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;



#endif


#ifndef __GCCHARBUFF_H__
#define __GCCHARBUFF_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCCharBuff : public Packet
{
public:
	GCCharBuff(){Reset();}
	virtual ~GCCharBuff(){}

	VOID Reset( VOID )
	{
		m_nReceiverID	= INVALID_ID;
		m_nSenderID		= INVALID_ID;
		m_nBuffID		= INVALID_ID;
		m_nSkillID		= INVALID_ID;
		m_bEnable		= FALSE;
		m_nSenderLogicCount = 0;
		m_nSN = 0;
	}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream )const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID()const {return PACKET_GC_CHAR_BUFF;}
	virtual UINT			GetPacketSize()const
							{
								if ( m_bEnable )
								{
									return	sizeof(m_nReceiverID)
										+	sizeof(m_nSenderID)
										+	sizeof(m_nBuffID)
										+	sizeof(m_nSkillID)
										+	sizeof(m_bEnable)
										+	sizeof(m_nSenderLogicCount)
										+	sizeof(m_nSN);
								}
								else
								{
									return	sizeof(m_nReceiverID)
										+ sizeof(m_nBuffID)
										+ sizeof(m_bEnable);
								}
							}
public:
	//使用数据接口
	VOID			SetReceiverID(ObjID_t id) {m_nReceiverID = id;}
	ObjID_t			GetReceiverID(VOID)const {return m_nReceiverID;}

	VOID			SetSenderID(ObjID_t id) {m_nSenderID = id;}
	ObjID_t			GetSenderID(VOID)const {return m_nSenderID;}

	VOID			SetBuffID(ImpactID_t id) {m_nBuffID = id;}
	ImpactID_t		GetBuffID(VOID)const {return m_nBuffID;}

	VOID			SetSkillID(SkillID_t id) {m_nSkillID = id;}
	ImpactID_t		GetSkillID(VOID)const {return m_nSkillID;}

	VOID			SetEnable(BOOL bEnable) {m_bEnable = bEnable;}
	BOOL			GetEnable(VOID)const {return m_bEnable;}

	VOID			SetSenderLogicCount(INT nCount) {m_nSenderLogicCount=nCount;};
	INT				GetSenderLogicCount(VOID) const {return m_nSenderLogicCount;};

	VOID			SetSN(UINT nSN) {m_nSN=nSN;}
	UINT			GetSN(VOID) const {return m_nSN;}
private:
	ObjID_t			m_nReceiverID;		// 效果接受者的ID
	ObjID_t			m_nSenderID;		// 效果施放者的ID
	ImpactID_t		m_nBuffID;			// 特效数据的ID(索引)
	SkillID_t		m_nSkillID;			// 技能ID
	BOOL			m_bEnable; 			// 是效果生效消息还是
	INT				m_nSenderLogicCount; //效果发起者的逻辑计数
	UINT			m_nSN;				//持续性效果的序列号(用于识别)
};


class GCCharBuffFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() {return new GCCharBuff();}
	PacketID_t	GetPacketID()const {return PACKET_GC_CHAR_BUFF;}
	UINT		GetPacketMaxSize()const
					{
						return 	sizeof(ObjID_t)
							+	sizeof(ObjID_t)
							+ 	sizeof(ImpactID_t)
							+	sizeof(SkillID_t)
							+ 	sizeof(BOOL)
							+	sizeof(INT)
							+	sizeof(UINT);
					}
};

class GCCharBuffHandler 
{
public:
	static UINT Execute( GCCharBuff* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif

// GCDetailXinFaList.h
// 
// 当前主角的详细技能表
// 
//////////////////////////////////////////////////////

#ifndef __GCDETAILXINFALIST_H__
#define __GCDETAILXINFALIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCSkillClass : public Packet
{
public:
	GCSkillClass( ){}
	virtual ~GCSkillClass( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_DETAILXINFALIST ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) + sizeof(WORD) +
															sizeof(_OWN_XINFA) * m_wNumXinFa; }

public:
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	WORD		getNumXinFa( VOID )const{ return m_wNumXinFa; }
	const _OWN_XINFA	*getXinFa( WORD wIndex )const{ return &m_aXinFa[wIndex]; }
	const _OWN_XINFA	*getXinFaList( VOID )const{ return &m_aXinFa[0]; }

	VOID		setXinFaList( WORD wNum, const _OWN_XINFA *paXinFa ){
					m_wNumXinFa = wNum;
					memcpy( m_aXinFa, paXinFa, sizeof(_OWN_XINFA)*m_wNumXinFa );
				}
private:
	ObjID_t		m_ObjID;	// 所有Obj类型的ObjID
	WORD		m_wNumXinFa;
	_OWN_XINFA	m_aXinFa[MAX_CHAR_XINFA_NUM];
};


class GCDetailXinFaListFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCSkillClass() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_DETAILXINFALIST; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) + sizeof(WORD) +
													sizeof(_OWN_XINFA) * MAX_CHAR_XINFA_NUM; }
};

class GCSkillClassHandler 
{
public:
	static UINT Execute( GCSkillClass* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif

// GCCoolDownUpdate.h
// 
// 当前主角的冷却表更新
// 
//////////////////////////////////////////////////////

#ifndef __GCCOOLDOWNUPDATE_H__
#define __GCCOOLDOWNUPDATE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
class GCCooldownUpdate : public Packet
{
private:
	typedef union _Unnamed
	{
		CHAR m_szBuff1[MAX_COOLDOWN_LIST_SIZE_FOR_HUMAN];
		CHAR m_szBuff2[MAX_COOLDOWN_LIST_SIZE_FOR_PET];
	} SizeHelper_T;
public:
	enum
	{
		LIST_SIZE = sizeof(SizeHelper_T),
	};
	GCCooldownUpdate( ):m_nNumCooldown(0)
	{
		for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
		{
			m_guidPet.Reset();
			m_aCooldowns[nIdx].Reset();
		}
	};
	virtual ~GCCooldownUpdate( ){};

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_COOLDOWN_UPDATE ; }
	virtual UINT			GetPacketSize()const 
		{
			CalcNumCooldown();
			return	sizeof(m_guidPet)
				+	sizeof(m_nNumCooldown)
				+ 	sizeof(m_aCooldowns[0])*m_nNumCooldown;
		}
public:
	VOID		CleanUp(VOID);
	PET_GUID_t const& GetPetGuid(VOID) const{return m_guidPet;}
	VOID		SetPetGuid(PET_GUID_t const& guidPet) {m_guidPet = guidPet;}
	BOOL		CooldownModified(Cooldown_T const& rCooldown);
	BOOL		CooldownModified(CooldownID_t nID, Time_t nTotalCooldown);
	SHORT		NumCooldown(VOID) const {return m_nNumCooldown;};
	Cooldown_T	const*	CooldownList(VOID) const {return m_aCooldowns;};
private:
	PET_GUID_t	m_guidPet;
	VOID		CalcNumCooldown(VOID) const;
	volatile SHORT			m_nNumCooldown;
	Cooldown_T	m_aCooldowns[LIST_SIZE];
};


class GCCooldownUpdateFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCCooldownUpdate() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_COOLDOWN_UPDATE; }
	UINT		GetPacketMaxSize()const {
					return 	sizeof(PET_GUID_t)
						+	sizeof(SHORT)
						+	sizeof(Cooldown_T)* GCCooldownUpdate::LIST_SIZE;
				}
};

class GCCooldownUpdateHandler 
{
public:
	static UINT Execute( GCCooldownUpdate* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif


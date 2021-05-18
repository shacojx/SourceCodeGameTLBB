// GCAbilityResult.h

#ifndef __GCABILITYRESULT_H__
#define __GCABILITYRESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class GCAbilityResult : public Packet 
{
public:
	GCAbilityResult( ){} ;
	virtual ~GCAbilityResult( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_GC_ABILITY_RESULT; }
	virtual UINT			GetPacketSize() const { return sizeof(AbilityID_t)+sizeof(INT)+sizeof(PrescriptionID_t); }
	
public:
	//使用数据接口
	VOID					SetAbilityID(AbilityID_t aid) { m_Ability = aid; }
	AbilityID_t				GetAbilityID() { return m_Ability; }

	VOID					SetPrescriptionID(PrescriptionID_t pid) { m_Prescription = pid; }
	PrescriptionID_t		GetPrescriptionID() { return m_Prescription; }

	INT						getResult(VOID) { return m_nResult; }
	VOID					setResult(INT nResult) { m_nResult = nResult; }

private:
	//数据
	AbilityID_t				m_Ability;
	PrescriptionID_t		m_Prescription;
	INT						m_nResult;		// 返回值 OPERATE_RESULT
};


class GCAbilityResultFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCAbilityResult() ; }
	PacketID_t	GetPacketID() const { return PACKET_GC_ABILITY_RESULT ; }
	UINT		GetPacketMaxSize() const { return sizeof(AbilityID_t)+sizeof(INT)+sizeof(PrescriptionID_t); }
};


class GCAbilityResultHandler 
{
public:
	static UINT Execute( GCAbilityResult* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;

#endif //  __GCABILITYRESULT_H__

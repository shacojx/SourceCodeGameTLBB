#include "stdafx.h"

#include "GCCharEquipment.h"

BOOL GCCharEquipment::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_wFlags), sizeof(WORD));

	if ( m_wFlags & (1 << HEQUIP_WEAPON) )
	{
		iStream.Read( (CHAR*)(&m_WeaponID), sizeof(m_WeaponID));
		iStream.Read( (CHAR*)(&m_WeaponGemID), sizeof(m_WeaponGemID));
	}

	if ( m_wFlags & (1 << HEQUIP_CAP) )
	{
		iStream.Read( (CHAR*)(&m_CapID), sizeof(m_CapID));
		iStream.Read( (CHAR*)(&m_CapGemID), sizeof(m_CapGemID));
	}

	if ( m_wFlags & (1 << HEQUIP_ARMOR) )
	{
		iStream.Read( (CHAR*)(&m_ArmourID), sizeof(m_ArmourID));
		iStream.Read( (CHAR*)(&m_ArmourGemID), sizeof(m_ArmourGemID));
	}

	if ( m_wFlags & (1 << HEQUIP_CUFF) )
	{
		iStream.Read( (CHAR*)(&m_CuffID), sizeof(m_CuffID));
		iStream.Read( (CHAR*)(&m_CuffGemID), sizeof(m_CuffGemID));
	}

	if ( m_wFlags & (1 << HEQUIP_BOOT) )
	{
		iStream.Read( (CHAR*)(&m_BootID), sizeof(m_BootID));
		iStream.Read( (CHAR*)(&m_BootGemID), sizeof(m_BootGemID));
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharEquipment::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_wFlags), sizeof(WORD));

	if ( m_wFlags & (1 << HEQUIP_WEAPON) )
	{
		oStream.Write( (CHAR*)(&m_WeaponID), sizeof(m_WeaponID));
		oStream.Write( (CHAR*)(&m_WeaponGemID), sizeof(m_WeaponGemID));
	}

	if ( m_wFlags & (1 << HEQUIP_CAP) )
	{
		oStream.Write( (CHAR*)(&m_CapID), sizeof(m_CapID));
		oStream.Write( (CHAR*)(&m_CapGemID), sizeof(m_CapGemID));
	}

	if ( m_wFlags & (1 << HEQUIP_ARMOR) )
	{
		oStream.Write( (CHAR*)(&m_ArmourID), sizeof(m_ArmourID));
		oStream.Write( (CHAR*)(&m_ArmourGemID), sizeof(m_ArmourGemID));
	}

	if ( m_wFlags & (1 << HEQUIP_CUFF) )
	{
		oStream.Write( (CHAR*)(&m_CuffID), sizeof(m_CuffID));
		oStream.Write( (CHAR*)(&m_CuffGemID), sizeof(m_CuffGemID));
	}

	if ( m_wFlags & (1 << HEQUIP_BOOT) )
	{
		oStream.Write( (CHAR*)(&m_BootID), sizeof(m_BootID));
		oStream.Write( (CHAR*)(&m_BootGemID), sizeof(m_BootGemID));
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID GCCharEquipment::FillParamBuf( VOID *pBuf )
{
	UINT *aBuf	= (UINT*)(pBuf);

	UINT uParamIndex;
	uParamIndex = 0;



	if ( m_wFlags & (1 << HEQUIP_WEAPON) )
		aBuf[uParamIndex++] = m_WeaponID;

	if ( m_wFlags & (1<<HEQUIP_CAP) )
		aBuf[uParamIndex++] = m_CapID;

	if ( m_wFlags & (1<<HEQUIP_ARMOR) )
		aBuf[uParamIndex++] = m_ArmourID;

	if ( m_wFlags & (1<<HEQUIP_CUFF) )
		aBuf[uParamIndex++] = m_CuffID;

	if ( m_wFlags & (1<<HEQUIP_BOOT) )
		aBuf[uParamIndex++] = m_BootID;
}

UINT GCCharEquipment::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCCharEquipmentHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


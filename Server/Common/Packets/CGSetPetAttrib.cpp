#include "stdafx.h"
// CGSetPetAttrib.cpp

#include "CGSetPetAttrib.h"

BOOL CGSetPetAttrib::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID));
	iStream.Read( (CHAR*)(&m_Flags), sizeof(m_Flags));

	if ( m_Flags.isSetBit(SET_PET_ATTRIB_NAME ) )
	{
		iStream.Read( (CHAR*)(&m_byNameSize), sizeof(m_byNameSize));
		if( m_byNameSize>=MAX_CHARACTER_NAME ) m_byNameSize=MAX_CHARACTER_NAME-1 ;
		iStream.Read( (CHAR*)(m_szName), sizeof(CHAR) * m_byNameSize);
		m_szName[m_byNameSize] = 0;
	}

	if ( m_Flags.isSetBit(SET_PET_ATTRIB_STR_INCREMENT ) )
		iStream.Read( (CHAR*)(&m_StrIncrement), sizeof(m_StrIncrement));
	if ( m_Flags.isSetBit(SET_PET_ATTRIB_CON_INCREMENT ) )
		iStream.Read( (CHAR*)(&m_ConIncrement), sizeof(m_ConIncrement));
	if ( m_Flags.isSetBit(SET_PET_ATTRIB_DEX_INCREMENT ) )
		iStream.Read( (CHAR*)(&m_DexIncrement), sizeof(m_DexIncrement));
	if ( m_Flags.isSetBit(SET_PET_ATTRIB_SPR_INCREMENT ) )
		iStream.Read( (CHAR*)(&m_SprIncrement), sizeof(m_SprIncrement));
	if ( m_Flags.isSetBit(SET_PET_ATTRIB_INT_INCREMENT ) )
		iStream.Read( (CHAR*)(&m_IntIncrement), sizeof(m_IntIncrement));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGSetPetAttrib::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION


	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID));
	oStream.Write( (CHAR*)(&m_Flags), sizeof(m_Flags));

	if ( m_Flags.isSetBit(SET_PET_ATTRIB_NAME ) )
	{
		oStream.Write( (CHAR*)(&m_byNameSize), sizeof(m_byNameSize));
		oStream.Write( (CHAR*)(m_szName), sizeof(CHAR) * m_byNameSize);
	}

	if ( m_Flags.isSetBit(SET_PET_ATTRIB_STR_INCREMENT ) )
		oStream.Write( (CHAR*)(&m_StrIncrement), sizeof(m_StrIncrement));
	if ( m_Flags.isSetBit(SET_PET_ATTRIB_CON_INCREMENT ) )
		oStream.Write( (CHAR*)(&m_ConIncrement), sizeof(m_ConIncrement));
	if ( m_Flags.isSetBit(SET_PET_ATTRIB_DEX_INCREMENT ) )
		oStream.Write( (CHAR*)(&m_DexIncrement), sizeof(m_DexIncrement));
	if ( m_Flags.isSetBit(SET_PET_ATTRIB_SPR_INCREMENT ) )
		oStream.Write( (CHAR*)(&m_SprIncrement), sizeof(m_SprIncrement));
	if ( m_Flags.isSetBit(SET_PET_ATTRIB_INT_INCREMENT ) )
		oStream.Write( (CHAR*)(&m_IntIncrement), sizeof(m_IntIncrement));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGSetPetAttrib::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGSetPetAttribHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


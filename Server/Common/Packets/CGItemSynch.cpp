#include "stdafx.h"
// CGItemSynch.cpp
// 
/////////////////////////////////////////////////////

#include "CGItemSynch.h"

BOOL CGItemSynch::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
	switch(m_Opt)
	{
	case OPT_MOVE_ITEM_AUTO:
		{
			iStream.Read( (CHAR*)(&m_FromType), sizeof(BYTE));
			switch(m_FromType)
			{
			case POS_BAG:
			case POS_EQUIP:
			case POS_BANK:
			case POS_PLAYERSHOP:
				{
					iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
					iStream.Read( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
				}
				break;
			case POS_PET:
				{
					iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
					iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
				}
				break;
			case POS_PLAYERSHOPPET:
				{
					iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
					iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
				}
				break;
			default:
				break;
			}
		}
		break;
	case OPT_MOVE_ITEM_MANU:
		{
			iStream.Read( (CHAR*)(&m_FromType), sizeof(BYTE));

			switch(m_FromType)
			{
			case POS_BAG:
			case POS_EQUIP:
			case POS_BANK:
			case POS_PLAYERSHOP:
				{
					iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
					iStream.Read( (CHAR*)(&m_ToIndex), sizeof(BYTE));
					iStream.Read( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
				}
				break;
			case POS_PET:
				{
					iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
					iStream.Read( (CHAR*)(&m_ToIndex), sizeof(BYTE));
					iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
				}
				break;
			case POS_PLAYERSHOPPET:
				{
					iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
					iStream.Read( (CHAR*)(&m_ToIndex), sizeof(BYTE));
					iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	iStream.Read( (CHAR*)(&m_ExtraInfoLength), sizeof(BYTE));
	iStream.Read( (CHAR*)(m_ExtraInfoData), m_ExtraInfoLength);
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGItemSynch::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
	switch(m_Opt)
	{
	case OPT_MOVE_ITEM_AUTO:
		{
			oStream.Write( (CHAR*)(&m_FromType), sizeof(BYTE));
			switch(m_FromType)
			{
			case POS_BAG:
			case POS_EQUIP:
			case POS_BANK:
			case POS_PLAYERSHOP:
				{
					oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
					oStream.Write( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
				}
				break;
			case POS_PET:
				{
					oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
					oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
				}
				break;
			case POS_PLAYERSHOPPET:
				{
					oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
					oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
				}
				break;
			default:
				break;
			}
		}
		break;
	case OPT_MOVE_ITEM_MANU:
		{
			oStream.Write( (CHAR*)(&m_FromType), sizeof(BYTE));

			switch(m_FromType)
			{
			case POS_BAG:
			case POS_EQUIP:
			case POS_BANK:
			case POS_PLAYERSHOP:
				{
					oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
					oStream.Write( (CHAR*)(&m_ToIndex), sizeof(BYTE));
					oStream.Write( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
				}
				break;
			case POS_PET:
				{
					oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
					oStream.Write( (CHAR*)(&m_ToIndex), sizeof(BYTE));
					oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
				}
				break;
			case POS_PLAYERSHOPPET:
				{
					oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
					oStream.Write( (CHAR*)(&m_ToIndex), sizeof(BYTE));
					oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	oStream.Write( (CHAR*)(&m_ExtraInfoLength), sizeof(BYTE));
	oStream.Write( (CHAR*)(m_ExtraInfoData), m_ExtraInfoLength);
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGItemSynch::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGItemSynchHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

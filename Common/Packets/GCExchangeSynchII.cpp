#include "stdafx.h"
// GCExchangeSynchII.cpp
// 
/////////////////////////////////////////////////////

#include "GCExchangeSynchII.h"

BOOL GCExchangeSynchII::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_IsMyself), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));

	if(m_IsMyself)
	{//给自己盒子里加东西
		switch (m_Opt)
		{
		case EXCHANGE_MSG::OPT_ADDITEM:
			{
				iStream.Read( (CHAR*)(&m_FromType), sizeof(BYTE));
				iStream.Read( (CHAR*)(&m_FromIndex), sizeof(BYTE));
				iStream.Read( (CHAR*)(&m_ToIndex), sizeof(BYTE));
			}
			break;
		case EXCHANGE_MSG::OPT_REMOVEITEM:
			{
				iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
				iStream.Read( (CHAR*)(&m_ToIndex), sizeof(BYTE));
				iStream.Read( (CHAR*)(&m_FromIndex), sizeof(BYTE));
			}
			break;
		case EXCHANGE_MSG::OPT_MONEY:
			{
				iStream.Read( (CHAR*)(&m_Money), sizeof(UINT));
			}
			break;
		case EXCHANGE_MSG::OPT_REMOVEMONEY:
			{
                iStream.Read( (CHAR*)(&m_Money), sizeof(UINT));
			}
			break;
		case EXCHANGE_MSG::OPT_ADDPET:
			{
				iStream.Read( (CHAR*)(&m_ToIndex), sizeof(BYTE));
				iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
			}
			break;
		case EXCHANGE_MSG::OPT_REMOVEPET:
			{
				iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
			}
			break;
		default:
			break;
		}
	}
	else
	{//给对方盒子里加东西
		switch (m_Opt)
		{
		case EXCHANGE_MSG::OPT_ADDITEM:
			{
				iStream.Read( (CHAR*)(&m_ToIndex), sizeof(BYTE));
				iStream.Read( (CHAR*)(&m_ExItem.isBlueEquip), sizeof(BYTE));
				iStream.Read( (CHAR*)(&m_ExItem.byNumber), sizeof(BYTE));
				if(m_ExItem.isBlueEquip)
				{
					iStream.Read( (CHAR*)(&m_ExItem.item_data), sizeof(_ITEM));
				}
				else
				{
					iStream.Read( (CHAR*)(&m_ExItem.item_guid), sizeof(UINT));
				}
			}
			break;
		case EXCHANGE_MSG::OPT_REMOVEITEM:
			{
				iStream.Read( (CHAR*)(&m_FromIndex), sizeof(BYTE));
			}
			break;
		case EXCHANGE_MSG::OPT_MONEY:
			{
				iStream.Read( (CHAR*)(&m_Money), sizeof(UINT));
			}
			break;
		case EXCHANGE_MSG::OPT_REMOVEMONEY:
			{
				iStream.Read( (CHAR*)(&m_Money), sizeof(UINT));
			}
			break;
		case EXCHANGE_MSG::OPT_ADDPET:
			break;
		case EXCHANGE_MSG::OPT_REMOVEPET:
			{
				iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
			}
			break;
		default:
			break;
		}
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCExchangeSynchII::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_IsMyself), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));

	if(m_IsMyself)
	{//给自己盒子里加东西
		switch (m_Opt)
		{
		case EXCHANGE_MSG::OPT_ADDITEM:
			{
				oStream.Write( (CHAR*)(&m_FromType), sizeof(BYTE));
				oStream.Write( (CHAR*)(&m_FromIndex), sizeof(BYTE));
				oStream.Write( (CHAR*)(&m_ToIndex), sizeof(BYTE));
			}
			break;
		case EXCHANGE_MSG::OPT_REMOVEITEM:
			{
				oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
				oStream.Write( (CHAR*)(&m_ToIndex), sizeof(BYTE));
				oStream.Write( (CHAR*)(&m_FromIndex), sizeof(BYTE));
			}
			break;
		case EXCHANGE_MSG::OPT_MONEY:
			{
				oStream.Write( (CHAR*)(&m_Money), sizeof(UINT));
			}
			break;
		case EXCHANGE_MSG::OPT_REMOVEMONEY:
			{
				oStream.Write( (CHAR*)(&m_Money), sizeof(UINT));
			}
			break;
		case EXCHANGE_MSG::OPT_ADDPET:
			{
				oStream.Write( (CHAR*)(&m_ToIndex), sizeof(BYTE));
				oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
			}
			break;
		case EXCHANGE_MSG::OPT_REMOVEPET:
			{
				oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
			}
			break;
		default:
			break;
		}
	}
	else
	{//给对方盒子里加东西
		switch (m_Opt)
		{
		case EXCHANGE_MSG::OPT_ADDITEM:
			{
				oStream.Write( (CHAR*)(&m_ToIndex), sizeof(BYTE));
				oStream.Write( (CHAR*)(&m_ExItem.isBlueEquip), sizeof(BYTE));
				oStream.Write( (CHAR*)(&m_ExItem.byNumber), sizeof(BYTE));
				if(m_ExItem.isBlueEquip)
				{
					oStream.Write( (CHAR*)(&m_ExItem.item_data), sizeof(_ITEM));
				}
				else
				{
					oStream.Write( (CHAR*)(&m_ExItem.item_guid), sizeof(UINT));
				}
			}
			break;
		case EXCHANGE_MSG::OPT_REMOVEITEM:
			{
				oStream.Write( (CHAR*)(&m_FromIndex), sizeof(BYTE));
			}
			break;
		case EXCHANGE_MSG::OPT_MONEY:
			{
				oStream.Write( (CHAR*)(&m_Money), sizeof(UINT));
			}
			break;
		case EXCHANGE_MSG::OPT_REMOVEMONEY:
			{
				oStream.Write( (CHAR*)(&m_Money), sizeof(UINT));
			}
			break;
		case EXCHANGE_MSG::OPT_ADDPET:
			break;
		case EXCHANGE_MSG::OPT_REMOVEPET:
			{
				oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
			}
			break;
		default:
			break;
		}
	}
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCExchangeSynchII::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCExchangeSynchIIHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


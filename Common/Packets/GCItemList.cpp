#include "stdafx.h"
// GCItemList.cpp
// 
/////////////////////////////////////////////////////

#include "GCItemList.h"

BOOL GCItemList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));

	switch(m_Opt)
	{

	case OPT_ADD_ONE_ITEM:
		{
			iStream.Read( (CHAR*)(&(m_ItemList[0].nType)), sizeof(BYTE));
			switch(m_ItemList[0].nType)
			{
			case TYPE_ITEM:
				{
					iStream.Read( (CHAR*)(&(m_ItemList[0].nIndex)), sizeof(BYTE));
					iStream.Read( (CHAR*)(&(m_ItemList[0].item_data)), sizeof(_ITEM));
					iStream.Read( (CHAR*)(&(m_ItemList[0].ExtraLength)), sizeof(BYTE));
					if( m_ItemList[0].ExtraLength > 0 )
					{
						iStream.Read( (CHAR*)(m_ItemList[0].ExtraInfo), m_ItemList[0].ExtraLength);
					}
				}
				break;
			case TYPE_PET:
				{

				}
				break;
			case TYPE_SERIALS:
				{
					iStream.Read( (CHAR*)(&(m_ItemList[0].nIndex)), sizeof(BYTE));
					iStream.Read( (CHAR*)(&(m_ItemList[0].ExtraLength)), sizeof(BYTE));
					if( m_ItemList[0].ExtraLength > 0 )
					{
						iStream.Read( (CHAR*)(m_ItemList[0].ExtraInfo), m_ItemList[0].ExtraLength);
					}
				}
				break;
			default:
				break;
			}
		}
		break;

	case OPT_ADD_ITEM_LIST:
		{
			iStream.Read( (CHAR*)(&m_ItemNum), sizeof(BYTE));

			for(UINT i = 0; i<m_ItemNum ; i++)
			{
				iStream.Read( (CHAR*)(&(m_ItemList[i].nType)), sizeof(BYTE));

				switch(m_ItemList[i].nType)
				{
				case TYPE_ITEM:
					{
						iStream.Read( (CHAR*)(&(m_ItemList[i].nIndex)), sizeof(BYTE));
						iStream.Read( (CHAR*)(&(m_ItemList[i].item_data)), sizeof(_ITEM));
						iStream.Read( (CHAR*)(&(m_ItemList[i].ExtraLength)), sizeof(BYTE));
						if( m_ItemList[i].ExtraLength > 0 )
						{
							iStream.Read( (CHAR*)(m_ItemList[i].ExtraInfo), m_ItemList[i].ExtraLength);
						}
					}
					break;
				case TYPE_PET:
					{

					}
					break;
				case TYPE_SERIALS:
					{
						iStream.Read( (CHAR*)(&(m_ItemList[i].nIndex)), sizeof(BYTE));
						iStream.Read( (CHAR*)(&(m_ItemList[i].ExtraLength)), sizeof(BYTE));
						if( m_ItemList[i].ExtraLength > 0 )
						{
							iStream.Read( (CHAR*)(m_ItemList[i].ExtraInfo), m_ItemList[i].ExtraLength);
						}
					}
					break;
				default:
					break;
				}
			}
		}
		break;
	default:
		break;
	}

	iStream.Read( (CHAR*)(&m_ExtraLength), sizeof(BYTE));
	if(m_ExtraLength > 0)
	{
		iStream.Read( (CHAR*)(m_ExtraInfo), m_ExtraLength);
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCItemList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));

	switch(m_Opt)
	{
	case OPT_ADD_ONE_ITEM:
		{
			oStream.Write( (CHAR*)(&(m_ItemList[0].nType)), sizeof(BYTE));
			switch(m_ItemList[0].nType)
			{
			case TYPE_ITEM:
				{
					oStream.Write( (CHAR*)(&(m_ItemList[0].nIndex)), sizeof(BYTE));
					oStream.Write( (CHAR*)(&(m_ItemList[0].item_data)), sizeof(_ITEM));
					oStream.Write( (CHAR*)(&(m_ItemList[0].ExtraLength)), sizeof(BYTE));
					if( m_ItemList[0].ExtraLength > 0 )
					{
						oStream.Write( (CHAR*)(m_ItemList[0].ExtraInfo), m_ItemList[0].ExtraLength);
					}
				}
				break;
			case TYPE_PET:
				{

				}
				break;
			case TYPE_SERIALS:
				{
					oStream.Write( (CHAR*)(&(m_ItemList[0].nIndex)), sizeof(BYTE));
					oStream.Write( (CHAR*)(&(m_ItemList[0].ExtraLength)), sizeof(BYTE));
					if( m_ItemList[0].ExtraLength > 0 )
					{
						oStream.Write( (CHAR*)(m_ItemList[0].ExtraInfo), m_ItemList[0].ExtraLength);
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case OPT_ADD_ITEM_LIST:
		{
			oStream.Write( (CHAR*)(&m_ItemNum), sizeof(BYTE));

			for(UINT i = 0; i<m_ItemNum ; i++)
			{
				oStream.Write( (CHAR*)(&(m_ItemList[i].nType)), sizeof(BYTE));

				switch(m_ItemList[i].nType)
				{
				case TYPE_ITEM:
					{
						oStream.Write( (CHAR*)(&(m_ItemList[i].nIndex)), sizeof(BYTE));
						oStream.Write( (CHAR*)(&(m_ItemList[i].item_data)), sizeof(_ITEM));
						oStream.Write( (CHAR*)(&(m_ItemList[i].ExtraLength)), sizeof(BYTE));
						if( m_ItemList[i].ExtraLength > 0 )
						{
							oStream.Write( (CHAR*)(m_ItemList[i].ExtraInfo), m_ItemList[i].ExtraLength);
						}
					}
					break;
				case TYPE_PET:
					{

					}
					break;
				case TYPE_SERIALS:
					{
						oStream.Write( (CHAR*)(&(m_ItemList[i].nIndex)), sizeof(BYTE));
						oStream.Write( (CHAR*)(&(m_ItemList[i].ExtraLength)), sizeof(BYTE));
						if( m_ItemList[i].ExtraLength > 0 )
						{
							oStream.Write( (CHAR*)(m_ItemList[i].ExtraInfo), m_ItemList[i].ExtraLength);
						}
					}
					break;
				default:
					break;
				}
			}
		}
		break;
	default:
		break;
	}

	oStream.Write( (CHAR*)(&m_ExtraLength), sizeof(BYTE));
	if(m_ExtraLength > 0)
	{
		oStream.Write( (CHAR*)(m_ExtraInfo), m_ExtraLength);
	}
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCItemList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCItemListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

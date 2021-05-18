// GCItemSynch.h
// 
//////////////////////////////////////////////////////

#ifndef __GCITEMSYNCH_H__
#define __GCITEMSYNCH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "ItemSynchStructDefine.h"
#define MAX_EXTRAINFO_LENGTH 256
namespace Packets
{
	class GCItemSynch : public Packet
	{
	public:
		enum
		{
			OPT_MOVE_ITEM = 0,
			OPT_REMOVE_ITEM,
			OPT_EXCHANGE_ITEM,
		};

		enum
		{
			POS_BAG = 0,
			POS_EQUIP,
			POS_PET,
			POS_BANK,
			POS_PLAYERSHOP,
			POS_PLAYERSHOPPET,
		};

	public:
		GCItemSynch( )
		{
			m_Opt				=	0;	
			m_FromType			=	0;
			m_ToType			=	0;
			m_ToIndex			=	0;
			m_ExtraInfoLength	=	0;
			memset(m_ExtraInfoData, 0, MAX_EXTRAINFO_LENGTH);
		}
		virtual ~GCItemSynch( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_ITEMSYNCH; }
		virtual UINT			GetPacketSize()const 
		{
			UINT	nLength = 0;
			nLength += sizeof(BYTE);//m_Opt
			switch(m_Opt)
			{
			case OPT_MOVE_ITEM:
				{
					nLength += sizeof(BYTE);//m_FromType
					switch(m_FromType)
					{
					case POS_BAG:
					case POS_EQUIP:
					case POS_BANK:
					case POS_PLAYERSHOP:
						{
							nLength += sizeof(BYTE);//m_ToType
							nLength += sizeof(BYTE);//m_ToIndex
							nLength += sizeof(_ITEM_GUID);//m_ItemGuid
						}
						break;
					case POS_PET:
					case POS_PLAYERSHOPPET:
						{
							nLength += sizeof(BYTE);//m_ToType
							nLength += sizeof(BYTE);//m_ToIndex
							nLength += sizeof(PET_GUID_t);//m_PetGuid
						}
						break;
					default:
						break;
					}
				}
				break;
			case OPT_REMOVE_ITEM:
				{
					nLength += sizeof(BYTE);//m_FromType
					nLength += sizeof(_ITEM_GUID);//m_ItemGuid
				}
				break;
			case OPT_EXCHANGE_ITEM:
				{
					nLength += sizeof(BYTE);//m_FromType
					nLength += sizeof(BYTE);//m_ToType
					nLength += sizeof(BYTE);//m_ToIndex
					nLength += sizeof(_ITEM_GUID);//m_ItemGuid
				}
				break;
			default:
				break;
			}
			nLength += sizeof(BYTE);//m_ExtraInfoLength
			nLength += m_ExtraInfoLength;//m_ExtraInfoData
			return	nLength;
		}

	public:
		BYTE					GetOpt(VOID) const {return m_Opt;};
		VOID					SetOpt(BYTE Opt) {m_Opt = Opt;};

		BYTE					GetFromType(VOID) const {return m_FromType;};
		VOID					SetFromType(BYTE FromType) {m_FromType = FromType;};

		BYTE					GetToType(VOID) const {return m_ToType;};
		VOID					SetToType(BYTE ToType) {m_ToType = ToType;};

		BYTE					GetToIndex(VOID) const {return m_ToIndex;};
		VOID					SetToIndex(BYTE ToIndex) {m_ToIndex = ToIndex;};

		_ITEM_GUID				GetItemGuid(VOID) const {return m_ItemGuid;};
		VOID					SetItemGuid(_ITEM_GUID ItemGuid) {m_ItemGuid = ItemGuid;};

		PET_GUID_t				GetPetGUID(VOID) const {return m_PetGuid;}
		VOID					SetPetGUID(PET_GUID_t PetGuid) {m_PetGuid = PetGuid;}

		BYTE					GetExtraInfoLength(VOID) const {return m_ExtraInfoLength;}
		VOID					SetExtraInfoLength(BYTE ExtraInfoLength) {m_ExtraInfoLength = ExtraInfoLength;}

		BYTE*					GetExtraInfoData(VOID) {return m_ExtraInfoData;}
		VOID					SetExtraInfoData(BYTE* ExtraInfoData)
		{
			memcpy(m_ExtraInfoData, ExtraInfoData, m_ExtraInfoLength);
		}

	private:
		BYTE					m_Opt;	
		BYTE					m_FromType;
		BYTE					m_ToType;
		BYTE					m_ToIndex;
		_ITEM_GUID				m_ItemGuid;
		PET_GUID_t				m_PetGuid;
		BYTE					m_ExtraInfoLength;
		BYTE					m_ExtraInfoData[MAX_EXTRAINFO_LENGTH];
	};

	class GCItemSynchFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCItemSynch() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_ITEMSYNCH; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*4 
													+ sizeof(_ITEM_GUID)
													+ sizeof(PET_GUID_t)
													+ sizeof(BYTE)
													+ MAX_EXTRAINFO_LENGTH;};
	};

	class GCItemSynchHandler 
	{
	public:
		static UINT Execute( GCItemSynch* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif

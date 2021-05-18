// CGSetPetAttrib.h
// 
// 设置宠物属性点
// 
//////////////////////////////////////////////////////

#ifndef _CG_SET_PETATTRIB_H_
#define _CG_SET_PETATTRIB_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	enum ENUM_SET_PET_ATTRIB
	{
		SET_PET_ATTRIB_INVALID	= -1,
		SET_PET_ATTRIB_NAME,
		SET_PET_ATTRIB_STR_INCREMENT,
		SET_PET_ATTRIB_CON_INCREMENT,
		SET_PET_ATTRIB_DEX_INCREMENT,
		SET_PET_ATTRIB_SPR_INCREMENT,
		SET_PET_ATTRIB_INT_INCREMENT,

		SET_PET_ATTRIB_NUMBERS
	};

	class  CGSetPetAttrib : public Packet
	{
	public:
		CGSetPetAttrib( )
		{
			m_Flags.CleanUp();
		}

		virtual ~CGSetPetAttrib( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SET_PETATTRIB; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uSize = sizeof(PET_GUID_t) + sizeof(Flag16) ;
			UINT i;
			for ( i = 0; i < SET_PET_ATTRIB_NUMBERS; i++ )
			{
				if(m_Flags.isSetBit(i))
				{
					switch ( i )
					{
					case SET_PET_ATTRIB_NAME:
						uSize += sizeof(BYTE) + m_byNameSize;
						break;
					case SET_PET_ATTRIB_STR_INCREMENT:
					case SET_PET_ATTRIB_CON_INCREMENT:
					case SET_PET_ATTRIB_DEX_INCREMENT:
					case SET_PET_ATTRIB_SPR_INCREMENT:
					case SET_PET_ATTRIB_INT_INCREMENT:
						uSize += sizeof( INT );	
						break;
					default:
						break;
					}
				}
			}
			return uSize;
		}

	public:
		PET_GUID_t		GetGUID( VOID )const{ return m_GUID; }
		VOID			SetGUID( PET_GUID_t GUID ){ m_GUID = GUID; }

		Flag16			GetFlags( VOID )const{ return m_Flags; }

		BOOL			IsUpdateAttrib( ENUM_SET_PET_ATTRIB eAttrib )const{ return m_Flags.isSetBit((INT)eAttrib)?(TRUE):(FALSE); }
		VOID			SetUpdateAttrib( ENUM_SET_PET_ATTRIB eAttrib, BOOL bUpdate ){ m_Flags.UpdateBits((INT)eAttrib,bUpdate); }

		VOID			SetName( const CHAR *pszName )
		{
			strncpy( m_szName, pszName, MAX_CHARACTER_NAME-1 );
			m_szName[MAX_CHARACTER_NAME-1] = 0 ;
			m_byNameSize = (BYTE)(strlen( m_szName ));

			SetUpdateAttrib(SET_PET_ATTRIB_NAME,TRUE);
		}

		const CHAR		*GetName( VOID )const{ return m_szName; }

		INT				GetStrIncrement( VOID )const{ return m_StrIncrement; }
		VOID			SetStrIncrement( INT StrIncrement ){ m_StrIncrement = StrIncrement; SetUpdateAttrib(SET_PET_ATTRIB_STR_INCREMENT, TRUE ); }

		INT				GetConIncrement( VOID )const{ return m_ConIncrement; }
		VOID			SetConIncrement( INT ConIncrement ){ m_ConIncrement = ConIncrement; SetUpdateAttrib(SET_PET_ATTRIB_CON_INCREMENT, TRUE ); }

		INT				GetDexIncrement( VOID )const{ return m_DexIncrement; }
		VOID			SetDexIncrement( INT  DexIncrement ){ m_DexIncrement = DexIncrement; SetUpdateAttrib(SET_PET_ATTRIB_DEX_INCREMENT, TRUE ); }

		INT				GetSprIncrement( VOID )const{ return m_SprIncrement; }
		VOID			SetSprIncrement( INT SprIncrement ){ m_SprIncrement = SprIncrement; SetUpdateAttrib(SET_PET_ATTRIB_SPR_INCREMENT, TRUE ); }

		INT				GetIntIncrement( VOID )const{ return m_IntIncrement; }
		VOID			SetIntIncrement( INT  IntIncrement ){ m_IntIncrement = IntIncrement; SetUpdateAttrib(SET_PET_ATTRIB_INT_INCREMENT, TRUE ); }

	private:
		PET_GUID_t		m_GUID;							// ID

		Flag16			m_Flags;						// 每个位表示一个属性是否要刷新 ENUM_DETAIL_ATTRIB

		BYTE			m_byNameSize;					// 名称长度,不包括最后的'\0'
		CHAR			m_szName[MAX_CHARACTER_NAME];	// 名称

		INT				m_StrIncrement;					// 力量增量
		INT				m_ConIncrement;					// 体力增量
		INT 			m_DexIncrement;					// 身法增量
		INT				m_SprIncrement;					// 灵气增量
		INT 			m_IntIncrement;					// 定力增量
	};

	class CGSetPetAttribFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGSetPetAttrib() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SET_PETATTRIB; }
		UINT		GetPacketMaxSize()const
		{
			UINT uSize = sizeof(CGSetPetAttrib) - sizeof(Packet) ;
			return uSize;
		}
	};

	class CGSetPetAttribHandler 
	{
	public:
		static UINT Execute( CGSetPetAttrib* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets;

#endif

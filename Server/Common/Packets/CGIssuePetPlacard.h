// CGIssuePetPlacard.h
// 
// 发布宠物征友信息
// 
//////////////////////////////////////////////////////

#ifndef _CG_ISSUEPETPLACARD_H_
#define _CG_ISSUEPETPLACARD_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GameStruct_Pet.h"

namespace Packets
{
	class  CGIssuePetPlacard : public Packet
	{
	public:
		CGIssuePetPlacard( )
		{
			m_byMessageSize = 0;
			memset(m_szMessage, 0, sizeof(m_szMessage));
		}

		virtual ~CGIssuePetPlacard( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream )const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ISSUEPETPLACARD; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uSize = sizeof(PET_GUID_t) + sizeof(ObjID_t) + sizeof(m_byMessageSize) + m_byMessageSize * sizeof(CHAR);
			return uSize;
		}

	public:
		PET_GUID_t		GetGUID( VOID )const{ return m_GUID; }
		VOID			SetGUID( PET_GUID_t GUID ){ m_GUID = GUID; }

		INT				GetNpcID(VOID)const{ return m_idNpc; }
		VOID			SetNpcID(ObjID_t idNpc){ m_idNpc = idNpc; }

		CHAR*			GetMessage( VOID )const{ return (CHAR*)m_szMessage; }
		VOID			SetMessage( const CHAR *pszMessage )
		{
			strncpy( m_szMessage, pszMessage, PET_PLACARD_ITEM_MESSAGE_SIZE-1 );
			m_szMessage[PET_PLACARD_ITEM_MESSAGE_SIZE-1] = 0 ;
			m_byMessageSize = (BYTE)(strlen( m_szMessage ));
		}

	private:
		PET_GUID_t		m_GUID;										// ID

		ObjID_t			m_idNpc;									// 目标NPC ID

		BYTE			m_byMessageSize;							// 宠主留言长度
		CHAR			m_szMessage[PET_PLACARD_ITEM_MESSAGE_SIZE];	// 宠主留言
	};

	class CGIssuePetPlacardFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGIssuePetPlacard() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ISSUEPETPLACARD; }
		UINT		GetPacketMaxSize()const
		{
			UINT uSize = sizeof(CGIssuePetPlacard) - sizeof(Packet) ;
			return uSize;
		}
	};

	class CGIssuePetPlacardHandler 
	{
	public:
		static UINT Execute( CGIssuePetPlacard* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets;

#endif // _CG_ISSUEPETPLACARD_H_

#ifndef _GCTEAMRESULT_H_
#define _GCTEAMRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCTeamResult:	public Packet
	{
		public:
			GCTeamResult()
			{
				m_NameSize = 0;
				memset( (void*)m_Name, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
			}
			virtual		~GCTeamResult(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GC_TEAMRESULT ; }
			virtual UINT			GetPacketSize()const { return	sizeof(m_Return)+
																	sizeof(m_GUID)+
																	sizeof(m_TeamID)+
																	sizeof(m_GUIDEx)+
																	sizeof(m_SceneID)+
																	sizeof(m_NameSize)+
																	sizeof(CHAR) * m_NameSize+
																	sizeof(m_nPortrait)+
																	sizeof(m_uDataID); }

	public :
		VOID				SetReturn( BYTE Return ){ m_Return = Return ; }
		BYTE				GetReturn( ){ return m_Return ; }

		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

		VOID				SetTeamID( TeamID_t tid ){	m_TeamID = tid ; }
		TeamID_t			GetTeamID( ){ return m_TeamID ; }

		VOID				SetGUIDEx( GUID_t guid ){	m_GUIDEx = guid ; }
		GUID_t				GetGUIDEx( ){ return m_GUIDEx ; }

		VOID				SetSceneID( SceneID_t SceneID ){ m_SceneID = SceneID ; }
		SceneID_t			GetSceneID( ){ return m_SceneID ; }

		VOID				SetName( const CHAR* pName )
		{
			strncpy( m_Name, pName, MAX_CHARACTER_NAME-1 );
			m_NameSize = (UCHAR)strlen(m_Name);
		}
		const CHAR*			GetName( ){ return m_Name ; }

		VOID				SetIcon( INT icon ) { m_nPortrait = icon; }
		INT					GetIcon( ) { return m_nPortrait; }

		VOID				SetDataID(USHORT dataid) { m_uDataID = dataid; }
		USHORT				GetDataID() { return m_uDataID; }

	public :
		BYTE				m_Return ;
		GUID_t				m_GUID ;
		TeamID_t			m_TeamID ;
		GUID_t				m_GUIDEx ;
		SceneID_t			m_SceneID ;
		UCHAR				m_NameSize;
		CHAR				m_Name[MAX_CHARACTER_NAME];			// 队员的名字，在队员入队的时候用
		INT					m_nPortrait;						// 头像
		USHORT				m_uDataID;							// 队员的性别

	};

	class GCTeamResultFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GCTeamResult() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GC_TEAMRESULT ; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(BYTE)+
																sizeof(GUID_t)+
																sizeof(TeamID_t)+
																sizeof(GUID_t)+
																sizeof(SceneID_t)+
																sizeof(UCHAR)+
																sizeof(CHAR) * MAX_CHARACTER_NAME +
																sizeof(INT)+
																sizeof(USHORT); }			
	};

	class GCTeamResultHandler
	{
		public:
			static UINT Execute(GCTeamResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif
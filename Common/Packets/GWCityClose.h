// GWCityClose.h
// 
// 请求申请建立新的城市
// 
//////////////////////////////////////////////////////

#ifndef __GWCITYCLOSE_H__
#define __GWCITYCLOSE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWCityClose : public Packet 
	{
	public:
		GWCityClose( )
		{
			m_CityGuid.Reset();
		}
		virtual ~GWCityClose( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_CITYCLOSE; }
		virtual UINT			GetPacketSize() const { return sizeof(_CITY_GUID) + sizeof(GUID_t); }

	public:
		_CITY_GUID				GetCityGuid(){ return m_CityGuid;}
		VOID					SetCityGuid(_CITY_GUID CityGuid)
		{
			m_CityGuid = CityGuid;
		}

		GUID_t					GetPlayerGuid(){ return m_PlayerGuid;}
		VOID					SetPlayerGuid(GUID_t PlayerGuid){m_PlayerGuid = PlayerGuid;}

	private:
		_CITY_GUID				m_CityGuid;
		GUID_t					m_PlayerGuid;
	};

	class GWCityCloseFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWCityClose() ; }
		PacketID_t	GetPacketID() const { return PACKET_GW_CITYCLOSE; }
		UINT		GetPacketMaxSize() const { return sizeof(_CITY_GUID) + sizeof(GUID_t); }
	};

	class GWCityCloseHandler 
	{
	public:
		static UINT Execute( GWCityClose* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

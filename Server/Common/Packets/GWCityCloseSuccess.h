// GWCityCloseSuccess.h
// 
// 请求申请建立新的城市
// 
//////////////////////////////////////////////////////

#ifndef __GWCITYCLOSESUCCESS_H__
#define __GWCITYCLOSESUCCESS_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWCityCloseSuccess : public Packet 
	{
	public:
		GWCityCloseSuccess( )
		{
			m_CityGuid.Reset();
		} 
		virtual ~GWCityCloseSuccess( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_CITYCLOSESUCCESS; }
		virtual UINT			GetPacketSize() const { return sizeof(_CITY_GUID); }

	public:
		_CITY_GUID				GetCityGuid(){return m_CityGuid;}
		VOID					SetCityGuid(_CITY_GUID CityGuid){m_CityGuid = CityGuid;}

	private:
		_CITY_GUID				m_CityGuid;

	};

	class GWCityCloseSuccessFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWCityCloseSuccess() ; }
		PacketID_t	GetPacketID() const { return PACKET_GW_CITYCLOSESUCCESS ; }
		UINT		GetPacketMaxSize() const { return sizeof(_CITY_GUID); }
	};

	class GWCityCloseSuccessHandler 
	{
	public:
		static UINT Execute( GWCityCloseSuccess* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

// GWCityOpt.h
// 
// 请求申请建立新的城市
// 
//////////////////////////////////////////////////////

#ifndef __GWCITYOPT_H__
#define __GWCITYOPT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define		MAX_IPARAM_NUM		4
#define		MAX_SZPARAM_LENGTH	20

namespace Packets
{
	class GWCityOpt : public Packet 
	{
	public:
		enum
		{
			OPT_NONE = 0, 
			OPT_CHANGE_BUILDING_DATA,
		};
	public:
		GWCityOpt( )
		{
			m_CityGuid.Reset();
			m_Opt = OPT_NONE;
			memset(m_iParam, 0, MAX_IPARAM_NUM*sizeof(INT));
			memset(m_szParam, 0, MAX_SZPARAM_LENGTH*sizeof(CHAR));
		}
		virtual ~GWCityOpt( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_CITYOPT; }
		virtual UINT			GetPacketSize() const { return	sizeof(_CITY_GUID) +
																sizeof(BYTE) + 
																MAX_IPARAM_NUM*sizeof(INT) +
																MAX_SZPARAM_LENGTH*sizeof(CHAR); }

	public:
		_CITY_GUID				GetCityGuid(){return m_CityGuid;}
		VOID					SetCityGuid(_CITY_GUID CityGuid){ m_CityGuid = CityGuid;}

		BYTE					GetOpt(){return m_Opt;}
		VOID					SetOpt(BYTE bOpt){ m_Opt = bOpt;}

		INT*					GetiParam(){return m_iParam;}
		VOID					SetiParam(INT* pParam)
		{
			memcpy(m_iParam, pParam, MAX_IPARAM_NUM*sizeof(INT));
		}

		CHAR*					GetszParam(){return m_szParam;}
		VOID					SetszParam(CHAR* pszParam)
		{
			memcpy(m_szParam, pszParam, MAX_SZPARAM_LENGTH*sizeof(CHAR));
		}

	private:
		_CITY_GUID		m_CityGuid;
		BYTE			m_Opt;
		INT				m_iParam[MAX_IPARAM_NUM];
		CHAR			m_szParam[MAX_SZPARAM_LENGTH];

	};

	class GWCityOptFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWCityOpt() ; }
		PacketID_t	GetPacketID() const { return PACKET_GW_CITYOPT; }
		UINT		GetPacketMaxSize() const { return	sizeof(_CITY_GUID) +
														sizeof(BYTE) + 
														MAX_IPARAM_NUM*sizeof(INT) +
														MAX_SZPARAM_LENGTH*sizeof(CHAR); }
	};

	class GWCityOptHandler 
	{
	public:
		static UINT Execute( GWCityOpt* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

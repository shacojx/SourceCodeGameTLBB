// WGCityAttr.h
// 
// 请求申请建立新的城市
// 
//////////////////////////////////////////////////////

#ifndef __WGCITYATTR_H__
#define __WGCITYATTR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGCityAttr : public Packet 
	{
	public:
		WGCityAttr( )
		{
			m_SceneID = INVALID_ID;
			m_uFlags	=	0;								//每个位表示一个属性是否要刷新 ENUM_UPDATE_CITY_ATT
			m_Level		=	0;								//城市级别
			memset(m_szCityName, 0, MAX_CITY_NAME_SIZE);	//城市名称
			m_BuildingIndex	=	0;							//建筑索引
			m_BuildingData.CleanUp();						//建筑物数据
		}
		virtual ~WGCityAttr( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_CITYATTR; }
		virtual UINT			GetPacketSize() const 
		{
			UINT uAttribSize = sizeof(SceneID_t) + sizeof(UINT);

			if ( m_uFlags & (1 << CITY_UPDATE_CITY_LEVEL) )
				uAttribSize += sizeof(BYTE);

			if ( m_uFlags & (1 << CITY_UPDATE_CITY_NAME) )
				uAttribSize += sizeof(CHAR)*MAX_CITY_NAME_SIZE;

			if ( m_uFlags & (1 << CITY_UPDATE_BUILDING) )
			{
				uAttribSize += sizeof(BYTE);
				uAttribSize += sizeof(CityBuilding_t);
			}
			return uAttribSize; 
		}

	public:

		SceneID_t		GetSceneID(){return m_SceneID;}
		VOID			SetSceneID(SceneID_t nSceneID){ m_SceneID = nSceneID;}

		UINT			getFlags( VOID )const{ return m_uFlags; }

		BOOL			IsUpdateAttrib( ENUM_UPDATE_CITY_ATT eAttrib )const{ return (m_uFlags & (1<<(INT)eAttrib))?(TRUE):(FALSE); }
		VOID			SetUpdateAttrib( ENUM_UPDATE_CITY_ATT eAttrib, BOOL bUpdate )
		{
			if ( bUpdate )
				m_uFlags |= (1<<(INT)eAttrib);
			else
				m_uFlags &= (~(1<<(INT)eAttrib));
		}

		BYTE			GetLevel(){return m_Level;}
		VOID			SetLevel(BYTE nLevel){m_Level = nLevel;}

		CHAR*			GetCityName(){return m_szCityName;}
		VOID			SetCityName(CHAR* pszCityName)
		{
			memcpy(m_szCityName, pszCityName, MAX_CITY_NAME_SIZE);
		}

		BYTE			GetBuildingIndex(){return m_BuildingIndex;}
		VOID			SetBuildingIndex(BYTE nBuildingIndex){m_BuildingIndex = nBuildingIndex;}

		CityBuilding_t	GetBuildingData(){return m_BuildingData;}
		VOID			SetBuildingData(CityBuilding_t BuildingData){m_BuildingData = BuildingData;}

	private:
		SceneID_t		m_SceneID;
		UINT			m_uFlags;							//每个位表示一个属性是否要刷新 ENUM_UPDATE_CITY_ATT
		BYTE			m_Level;							//城市级别
		CHAR			m_szCityName[MAX_CITY_NAME_SIZE];	//城市名称
		BYTE			m_BuildingIndex;					//建筑索引
		CityBuilding_t	m_BuildingData;						//建筑物数据
	};

	class WGCityAttrFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGCityAttr() ; }
		PacketID_t	GetPacketID() const { return PACKET_WG_CITYATTR; }
		UINT		GetPacketMaxSize() const { return	sizeof(SceneID_t)+
														sizeof(UINT)+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CITY_NAME_SIZE+
														sizeof(BYTE)+
														sizeof(CityBuilding_t); }
	};

	class WGCityAttrHandler 
	{
	public:
		static UINT Execute( WGCityAttr* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

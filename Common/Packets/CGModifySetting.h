

#ifndef __CGMODIFYSETTING_H__
#define __CGMODIFYSETTING_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGModifySetting : public Packet 
	{
	public:
		CGModifySetting( ){} ;
		virtual ~CGModifySetting( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MODIFYSETTING ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+
																sizeof(_OWN_SETTING) ; }

	public :

	public:
		//使用数据接口
		VOID					SetType( BYTE type ){ m_Type = type ; }
		BYTE					GetType( ){ return m_Type ; }

		VOID					SetValue( _OWN_SETTING* value ){ m_Value = *value ; }
		_OWN_SETTING*			GetValue( ){ return &m_Value ; }

	private:
		//数据
		BYTE					m_Type ;//enum SETTING_TYPE
		_OWN_SETTING			m_Value ;
	};


	class CGModifySettingFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGModifySetting() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MODIFYSETTING ; }
		UINT		GetPacketMaxSize()const { return	sizeof(BYTE)+
														sizeof(_OWN_SETTING) ; }
	};


	class CGModifySettingHandler 
	{
	public:
		static UINT Execute( CGModifySetting* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif

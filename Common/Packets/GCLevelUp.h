#ifndef _GC_LEVEL_UP_H_
#define _GC_LEVEL_UP_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
		class GCLevelUp:public Packet
		{

		public:
			GCLevelUp(){};
			virtual				~GCLevelUp(){};

			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream ) const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID() const { return PACKET_GC_LEVEUP; }

			virtual UINT			GetPacketSize() const 
			{
				return sizeof(ObjID_t)+sizeof(UINT);
			}

		public:
			
			VOID			setObjId(ObjID_t	id){m_ObjID = id;};
			ObjID_t			getObjId(){return m_ObjID;}
			
			VOID			setLevel(UINT lvl){m_Level = lvl;}
			UINT			getLevel(){return m_Level;}
			

		private:
			UINT			m_Level;
			ObjID_t			m_ObjID;

		};


		class GCLevelUpFactory:public PacketFactory
		{
		public:
			Packet*		CreatePacket() { return new GCLevelUp(); }
			PacketID_t	GetPacketID() const { return PACKET_GC_LEVEUP; }
			UINT		GetPacketMaxSize() const {return sizeof(ObjID_t)+sizeof(UINT); }
		};


		class GCLevelUpHandler
		{
		public:
			static UINT Execute( GCLevelUp* pPacket, Player* pPlayer );
		};



}


#endif
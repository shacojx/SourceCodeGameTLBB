#ifndef _CG_OPEN_ITEM_BOX_H_
#define _CG_OPEN_ITEM_BOX_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
			class CGOpenItemBox:public Packet
			{
			public:
				CGOpenItemBox(){};
				virtual	~CGOpenItemBox(){};


				virtual BOOL			Read( SocketInputStream& iStream ) ;
				virtual BOOL			Write( SocketOutputStream& oStream ) const ;
				virtual UINT			Execute( Player* pPlayer ) ;

				virtual PacketID_t		GetPacketID() const { return PACKET_CG_OPENITEMBOX ; }
				virtual UINT			GetPacketSize() const { return	sizeof(ObjID_t);}



			public:
				VOID			setObjID(ObjID_t id) { m_ObjID = id; }
				ObjID_t			getObjID(VOID) const { return m_ObjID; }


			private:
				ObjID_t			m_ObjID;		//Obj_ItemBoxµÄObjID

			};
			
			class CGOpenItemBoxFactory:public	PacketFactory
			{
			public:
				Packet*		CreatePacket() { return new CGOpenItemBox() ; }
				PacketID_t	GetPacketID() const { return PACKET_CG_OPENITEMBOX; }
				UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t); }
			};
			
			class CGOpenItemBoxHandler
			{
			public:
				static UINT	Execute( CGOpenItemBox* pPacket, Player* pPlayer );
			};



}

#endif
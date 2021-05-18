#ifndef __CG_DO_EVENT_H__
#define __CG_DO_EVENT_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
			class CGDoEvent:public Packet
			{
			public:
				CGDoEvent(){};
				virtual	~CGDoEvent(){};


				virtual BOOL			Read( SocketInputStream& iStream ) ;
				virtual BOOL			Write( SocketOutputStream& oStream ) const ;
				virtual UINT			Execute( Player* pPlayer ) ;

				virtual PacketID_t		GetPacketID() const { return PACKET_CG_DOEVENT ; }
				virtual UINT			GetPacketSize() const { return	sizeof(ObjID_t) + sizeof ( ScriptID_t );}



			public:
				VOID			setTargetID(ObjID_t id) { m_idTarget= id; }
				ObjID_t			getTargetID(VOID) const { return m_idTarget; }

				VOID			setScriptID(ObjID_t id) { m_idScript= id; }
				ObjID_t			getScriptID(VOID) const { return m_idScript; }


			private:
				ObjID_t			m_idTarget;
				ScriptID_t		m_idScript;

			};
			
			class CGDoEventFactory:public	PacketFactory
			{
			public:
				Packet*		CreatePacket() { return new CGDoEvent() ; }
				PacketID_t	GetPacketID() const { return PACKET_CG_DOEVENT; }
				UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t) + sizeof ( ScriptID_t ); }
			};
			
			class CGDoEventHandler
			{
			public:
				static UINT Execute( CGDoEvent* pPacket, Player* pPlayer );
			};



}

#endif
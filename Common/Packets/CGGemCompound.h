// CGGemCompound.h

#ifndef __CG_GEM_COMPOUND_H__
#define __CG_GEM_COMPOUND_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGGemCompound: public Packet
	{
	public:
		CGGemCompound(){};
		virtual		~CGGemCompound(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_GEM_COMPOUND; }
		virtual UINT			GetPacketSize() const { return sizeof(BYTE)+sizeof(BYTE)+sizeof(ObjID_t); }

		VOID					SetGemBagIndex1(BYTE idx) { m_GemBagIndex1 = idx; }
		BYTE					GetGemBagIndex1() { return m_GemBagIndex1; }

		VOID					SetGemBagIndex2(BYTE idx) { m_GemBagIndex2 = idx; }
		BYTE					GetGemBagIndex2() { return m_GemBagIndex2; }

		VOID					SetPlatformGUID(ObjID_t guid) { m_Platform = guid; }
		ObjID_t					GetPlatformGUID() { return m_Platform; }

	private:

		BYTE					m_GemBagIndex1;
		BYTE					m_GemBagIndex2;
		ObjID_t					m_Platform;

	};

	class CGGemCompoundFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGGemCompound() ; }
		PacketID_t	GetPacketID() const	{ return PACKET_CG_GEM_COMPOUND; }
		UINT		GetPacketMaxSize()const	{ return sizeof(BYTE)+sizeof(BYTE)+sizeof(ObjID_t); }
	};

	class CGGemCompoundHandler
	{
	public:
		static UINT Execute(CGGemCompound* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // __CG_GEM_COMPOUND_H__

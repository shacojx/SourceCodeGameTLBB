// CGApplyPrivateInfo.h
// 
//////////////////////////////////////////////////////

#ifndef __CG_APPLY_PRIVATEINFO_H__
#define __CG_APPLY_PRIVATEINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGApplyPrivateInfo : public Packet
	{
	public:
		CGApplyPrivateInfo( )
		{
			m_CharGUID		= 0;
		}
		virtual ~CGApplyPrivateInfo( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_APPLYPRIVATEINFO; }
		virtual UINT			GetPacketSize()const { return 0; }//sizeof(UINT);}

	public:
		//UINT m_CharGUID
		UINT		GetCharGUID()  {return m_CharGUID;}
		VOID		SetCharGUID(UINT CharGUID)  {m_CharGUID = CharGUID;}
	private:
		UINT		m_CharGUID;							//角色编号
	};

	class CGApplyPrivateInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGApplyPrivateInfo() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_APPLYPRIVATEINFO; };
		UINT		GetPacketMaxSize()const { return 0; } //sizeof(UINT);}
	};

	class CGApplyPrivateInfoHandler 
	{
	public:
		static UINT Execute( CGApplyPrivateInfo* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif

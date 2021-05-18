// CGPlayerCallOfResult.h
// 
// 邀请去某地的操作结果
//
//////////////////////////////////////////////////////

#ifndef __CGPLAYERCALLOFRESULT_H__
#define __CGPLAYERCALLOFRESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

enum ENUM_CALLOF_RESULT_CODE
{
	CALLOF_RESULT_CODE_INVALID	= -1,
	CALLOF_RESULT_CODE_ACCEPT,		// 接受
	CALLOF_RESULT_CODE_REFUSE,		// 拒绝
	CALLOF_RESULT_CODE_NUMBERS
};

namespace Packets
{
	class CGPlayerCallOfResult : public Packet
	{
	public:
		CGPlayerCallOfResult( )
		{
			m_nResultCode		= CALLOF_RESULT_CODE_INVALID;
		}
		virtual ~CGPlayerCallOfResult( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYER_CALLOF_RESULT ; }
		virtual UINT			GetPacketSize()const { return	sizeof(INT); }

	public:
		//使用数据接口
		VOID				SetResultCode(INT nResult) { m_nResultCode = nResult; }
		INT					GetResultCode(VOID)const { return m_nResultCode; }

	private:
		INT					m_nResultCode;			// ENUM_CALLOF_RESULT_CODE
	};


	class CGPlayerCallOfResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerCallOfResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYER_CALLOF_RESULT; }
		UINT		GetPacketMaxSize()const { return	sizeof(INT); }
	};

	class CGPlayerCallOfResultHandler 
	{
	public:
		static UINT Execute( CGPlayerCallOfResult* pPacket, Player* pPlayer ) ;
	};
}

#endif // __CGPLAYERCALLOFRESULT_H__

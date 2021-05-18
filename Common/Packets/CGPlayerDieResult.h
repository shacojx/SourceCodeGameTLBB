// CGPlayerDieResult.h
// 
// 主角死亡的操作结果
//
//////////////////////////////////////////////////////

#ifndef __CGPLAYERDIERESULT_H__
#define __CGPLAYERDIERESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

enum ENUM_DIE_RESULT_CODE
{
	DIE_RESULT_CODE_INVALID	= -1,
	DIE_RESULT_CODE_OUT_GHOST,		// 释放灵魂
	DIE_RESULT_CODE_RELIVE,			// 接受复活
	DIE_RESULT_CODE_NUMBERS
};

namespace Packets
{
	class CGPlayerDieResult : public Packet
	{
	public:
		CGPlayerDieResult( )
		{
			m_nResultCode		= DIE_RESULT_CODE_INVALID;
		}
		virtual ~CGPlayerDieResult( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYER_DIE_RESULT ; }
		virtual UINT			GetPacketSize()const { return	sizeof(INT); }

	public:
		//使用数据接口
		VOID				SetResultCode(INT nResult) { m_nResultCode = nResult; }
		INT					GetResultCode(VOID)const { return m_nResultCode; }

	private:
		INT					m_nResultCode;			// ENUM_DIE_RESULT_CODE
	};


	class CGPlayerDieResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerDieResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYER_DIE_RESULT; }
		UINT		GetPacketMaxSize()const { return	sizeof(INT); }
	};

	class CGPlayerDieResultHandler 
	{
	public:
		static UINT Execute( CGPlayerDieResult* pPacket, Player* pPlayer ) ;
	};
}

#endif // __CGPLAYERDIERESULT_H__

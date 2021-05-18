// GCCharMoveResult.h
// 
//////////////////////////////////////////////////////////

#ifndef __GCCHARMOVERESULT_H__
#define __GCCHARMOVERESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharMoveResult : public Packet
	{
	public:
		GCCharMoveResult( ){ m_yNumTargetPos=0; } ;
		virtual ~GCCharMoveResult( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARMOVERESULT; }
		virtual UINT			GetPacketSize()const
		{
			return sizeof(INT)*2 + sizeof(BYTE) + sizeof(WORLD_POS)*m_yNumTargetPos;
		}

	public:
		VOID				setHandleID( INT nID ) { m_nHandleID = nID; }
		INT					getHandleID( VOID )const{ return m_nHandleID; }

		VOID				setResult( INT nResult ) { m_nResult = nResult; }
		INT					getResult( VOID )const{ return m_nResult; }

		BYTE				getNumTargetPos( VOID )const{ return m_yNumTargetPos; }
		VOID				addTargetPos( const WORLD_POS *pPos )
		{
			if ( m_yNumTargetPos < MAX_CHAR_PATH_NODE_NUMBER )
			{
				m_aTargetPos[m_yNumTargetPos] = *pPos;
				m_yNumTargetPos++;
			}
		}
		const WORLD_POS		*getTargetPos(VOID)const { return m_aTargetPos; }
	private:
		INT					m_nHandleID;	// 操作ID
		INT					m_nResult;		// ORESULT
		BYTE				m_yNumTargetPos;
		WORLD_POS			m_aTargetPos[MAX_CHAR_PATH_NODE_NUMBER];		// 目标位置
	};


	class GCCharMoveResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharMoveResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARMOVERESULT; }
		UINT		GetPacketMaxSize()const { return sizeof(INT)*2 + sizeof(BYTE) + sizeof(WORLD_POS)*MAX_CHAR_PATH_NODE_NUMBER; }
	};

	class GCCharMoveResultHandler 
	{
	public:
		static UINT Execute( GCCharMoveResult* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCCHARMOVERESULT_H__

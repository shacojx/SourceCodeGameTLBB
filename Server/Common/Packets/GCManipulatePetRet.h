// GCManipulatePetRet.h
// 
// 请求操作宠物返回结果
//
//////////////////////////////////////////////////////

#ifndef __GC_MANIPULATEPETRET_H__
#define __GC_MANIPULATEPETRET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCManipulatePetRet : public Packet
	{
	public:
		enum ENUM_MANIPULATEPET_RET
		{
			MANIPULATEPET_RET_INVALID = -1,
			MANIPULATEPET_RET_CAPTUREFALID,		// 捕捉失败
			MANIPULATEPET_RET_CAPTURESUCC,		// 捕捉成功
			MANIPULATEPET_RET_CALLUPFALID,		// 召唤失败
			MANIPULATEPET_RET_CALLUPSUCC,		// 召唤成功
			MANIPULATEPET_RET_FREEFALID,		// 放生失败
			MANIPULATEPET_RET_FREESUCC,			// 放生成功
			MANIPULATEPET_RET_RECALLFALID,		// 收回失败
			MANIPULATEPET_RET_RECALLSUCC,		// 收回成功

		};

		GCManipulatePetRet( )
		{
			m_bFighting = FALSE;
			m_Ret = -1;
		}

		~GCManipulatePetRet( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MANIPULATEPETRET ; }
		virtual UINT			GetPacketSize()const { return (UINT)sizeof(PET_GUID_t)+
															  (UINT)sizeof(BOOL)+
															  (UINT)sizeof(INT); }
		
	public:
		//使用数据接口
		PET_GUID_t			GetGUID(VOID) const { return m_GUID; }
		VOID				SetGUID(PET_GUID_t guid) { m_GUID = guid; }
		BOOL				IsFighting(VOID) const { return m_bFighting; }
		INT					GetManipulateRet(VOID) const { return (ENUM_MANIPULATEPET_RET)m_Ret; }
		VOID				SetManipulateRet(INT Ret) { m_Ret = Ret; }
		VOID				SetFightingFlag(BOOL bFlag) { m_bFighting = bFlag; }

	private:
		PET_GUID_t		m_GUID;
		BOOL			m_bFighting;	// 是否处于参战状态
		INT				m_Ret;			// 返回结果
		
	};


	class GCManipulatePetRetFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCManipulatePetRet() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MANIPULATEPETRET; }
		UINT		GetPacketMaxSize()const { return (UINT)sizeof(PET_GUID_t)+
													 (UINT)sizeof(BOOL)+
													 (UINT)sizeof(INT); }
	};

	class GCManipulatePetRetHandler 
	{
	public:
		static UINT Execute( GCManipulatePetRet* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCPLAYERDIE_H__

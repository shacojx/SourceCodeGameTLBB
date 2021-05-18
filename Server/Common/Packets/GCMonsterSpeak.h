
#ifndef __GCMONSTERSPEAK_H__
#define __GCMONSTERSPEAK_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class GCMonsterSpeak : public Packet 
	{
	public:
		GCMonsterSpeak( ){} ;
		virtual ~GCMonsterSpeak( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MONSTERSPEAK ; }
		virtual UINT			GetPacketSize()const { return sizeof( ObjID_t )
															+ sizeof( BYTE )
															+ m_bySpeakContentSize; }

		VOID					setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t					getObjID(VOID)const { return m_ObjID; }
		
		inline const CHAR*		getSpeakContent(VOID)const { return m_szSpeakContent; }
		inline VOID				setSpeakContent(const CHAR* szContent) 
		{ 
			m_bySpeakContentSize = (BYTE)tsnprintf(m_szSpeakContent, MAX_MONSTERSPEAK_CONTENT-1, "%s", szContent); 
			
		}
		
	private:
		ObjID_t			m_ObjID;
		BYTE			m_bySpeakContentSize;							//泡泡说话的内容长度,不包括最后的'\0'
		CHAR			m_szSpeakContent[MAX_MONSTERSPEAK_CONTENT];		//泡泡说话的内容
	
	};


	class GCMonsterSpeakFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCMonsterSpeak() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MONSTERSPEAK ; }
		UINT		GetPacketMaxSize()const { return sizeof( ObjID_t )
													+ sizeof( BYTE )
													+ MAX_MONSTERSPEAK_CONTENT; }

	};


	class GCMonsterSpeakHandler 
	{
	public:
		static UINT Execute( GCMonsterSpeak* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif

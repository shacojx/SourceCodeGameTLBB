#ifndef _GC_DISCARDITEM_RESULT_
#define _GC_DISCARDITEM_RESULT_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCDiscardItemResult:public Packet
	{
	public:
		enum
		{
			FromBag = 0,
			FromBank,
		};
	public:
		GCDiscardItemResult()
		{
			m_Opt				=	FromBag;
			m_Result			=	0;		     //是否成功，不成功包含错误信息
			m_BagIndex			=	0;		    //成功后，丢弃物品的位置
			m_ItemTableIndex	=	0;
		};
		virtual				~GCDiscardItemResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_DISCARDITEMRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*3+sizeof(UINT);
		}

	public:
		
		VOID			setBagIndex(BYTE index){m_BagIndex = index;}
		BYTE			getBagIndex(){return m_BagIndex;}

		VOID			setResult(BYTE Result){m_Result = Result;}
		BYTE			getResult(){return m_Result;}

		VOID			setItemTableIndex(UINT	index){m_ItemTableIndex = index;}
		UINT			getItemTableIndex(){return m_ItemTableIndex;}

		VOID			setOpt(BYTE Opt){m_Opt = Opt;}
		BYTE			getOpt(){return m_Opt;}
		

	private:
		BYTE			m_Opt;		     //是否成功，不成功包含错误信息
		BYTE			m_Result;		     //是否成功，不成功包含错误信息
		BYTE			m_BagIndex;		    //成功后，丢弃物品的位置
		UINT			m_ItemTableIndex;

	};


	class GCDiscardItemResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCDiscardItemResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_DISCARDITEMRESULT; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)*3+sizeof(UINT); }
	};


	class GCDiscardItemResultHandler
	{
	public:
		static UINT	Execute( GCDiscardItemResult* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif
// GCSplitItemResult .h
// 
// 拆分物品
// 
//////////////////////////////////////////////////////

#ifndef __GCSPLITITEMRESULT_H__
#define __GCSPLITITEMRESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCSplitItemResult : public Packet
	{
	public:
		enum
		{
			RESULT_FALSE		= 0,
			RESULT_FALSE_NOGRID ,
			RESULT_SUCCEED		,
		};
		enum
		{
			BANK_CON			= 0,
			BAG_CON				,
		};

		GCSplitItemResult(){};
		virtual ~GCSplitItemResult(){};

		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream) ;
		virtual BOOL			Write(SocketOutputStream& oStream)const ;
		virtual UINT			Execute(Player* pPlayer) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SPLITITEMRESULT; }
		virtual UINT			GetPacketSize()const 
		{
			return sizeof(WORD)+GetItemRealSize(m_ITEM)+sizeof(BOOL)+sizeof(INT)+sizeof(BYTE);
		}
	public:
		VOID			setConIndex(WORD id)	{ m_ConIndex = id;}
		WORD			getConIndex(VOID)		{ return m_ConIndex;}
		
		_ITEM*			getItem(VOID)			{return &m_ITEM;}
		VOID			setItem(const _ITEM *pItem)	
		{
			Assert(pItem);
			memcpy(&m_ITEM,pItem,sizeof(_ITEM));
		}

		VOID			setIsNull(BOOL	bNull)	{m_nsNull	= bNull;}
		BOOL			getIsNull(VOID)			{return m_nsNull;}

		VOID			setContainer(INT Container) {m_Container = Container;}
		INT				getContainer(VOID)		{return m_Container;}

		VOID			setResult(BYTE Result)	{m_Result = Result;}
		BYTE			getResult(VOID)			{return m_Result;}

	private:
		WORD			m_ConIndex;		//item 的BagIndex
		BOOL			m_nsNull;		//物品是否为空		TRUE 代表没有Item,FALSE 代表有Item
		INT				m_Container;	//容器
		BYTE			m_Result;		//结果
		_ITEM			m_ITEM;

	};

	class GCSplitItemResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCSplitItemResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SPLITITEMRESULT; };
		UINT		GetPacketMaxSize()const { return	sizeof(WORD)+sizeof(_ITEM)+sizeof(BOOL)+sizeof(INT)+sizeof(BYTE); };
	};

	class GCSplitItemResultHandler 
	{
	public:
		static UINT Execute( GCSplitItemResult* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif

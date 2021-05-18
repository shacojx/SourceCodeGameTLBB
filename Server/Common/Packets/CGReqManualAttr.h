#ifndef _CG_REQMANUALATTR_H_
#define _CG_REQMANUALATTR_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class CGReqManualAttr:	public Packet
	{
		public:
					CGReqManualAttr():m_nStr(0),m_nSpr(0),m_nCon(0),m_nInt(0),m_nDex(0){};
					virtual		~CGReqManualAttr(){};

					//公用接口
					virtual BOOL			Read( SocketInputStream& iStream ) ;
					virtual BOOL			Write( SocketOutputStream& oStream )const ;
					virtual UINT			Execute( Player* pPlayer ) ;

					virtual PacketID_t		GetPacketID()const { return PACKET_CG_REQMANUALATTR ; }
					virtual UINT			GetPacketSize()const {
												return 	sizeof(m_nInt)
													+	sizeof(m_nSpr)
													+	sizeof(m_nCon)
													+	sizeof(m_nInt)
													+	sizeof(m_nDex);
											}

					VOID					SetStr(INT nValue) {m_nStr=nValue;};
					INT						GetStr(VOID) const {return m_nStr;}

					VOID					SetSpr(INT nValue) {m_nSpr=nValue;};
					INT						GetSpr(VOID) const {return m_nSpr;}

					VOID					SetCon(INT nValue) {m_nCon=nValue;};
					INT						GetCon(VOID) const {return m_nCon;}

					VOID					SetInt(INT nValue) {m_nInt=nValue;};
					INT						GetInt(VOID) const {return m_nInt;}

					VOID					SetDex(INT nValue) {m_nDex=nValue;};
					INT						GetDex(VOID) const {return m_nDex;}

		private:
					INT						m_nStr;
					INT						m_nSpr;
					INT						m_nCon;
					INT						m_nInt;
					INT						m_nDex;
	};
	
	class CGReqManualAttrFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new CGReqManualAttr() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_CG_REQMANUALATTR ; }
			UINT		GetPacketMaxSize()const		{
								return 	sizeof(INT)
									+	sizeof(INT)
									+	sizeof(INT)
									+	sizeof(INT)
									+	sizeof(INT);
							}			
	};

	class CGReqManualAttrHandler
	{
	public:
		static UINT Execute(CGReqManualAttr* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif

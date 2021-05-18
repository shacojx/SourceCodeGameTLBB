#ifndef __GCCHAREQUIPMENT_H__
#define __GCCHAREQUIPMENT_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCCharEquipment : public Packet
{
public:
	GCCharEquipment( ){
		Reset( );
	}
	virtual ~GCCharEquipment( ){}

	VOID Reset( VOID ){
		m_ObjID			= UINT_MAX;
		m_wFlags		= 0;
	}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHAREQUIPMENT; }
	virtual UINT			GetPacketSize()const {
								UINT uAttribSize = 0;
								UINT i;
								for ( i = 0; i < HEQUIP_NUMBER; i++ )
								{
									if ( m_wFlags & (1<<i) )
									{
										uAttribSize += sizeof( UINT ) * 2;
									}
								}
								uAttribSize +=	sizeof(ObjID_t) + sizeof(WORD);
								return uAttribSize;
							}

public:
	//使用数据接口
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	UINT			getFlags( VOID )const{ return (UINT)(m_wFlags); }

	BOOL			IsUpdateAttrib( HUMAN_EQUIP eAttrib )const{ return (m_wFlags & (1<<(INT)eAttrib))?(TRUE):(FALSE); }
	VOID			SetUpdateAttrib( HUMAN_EQUIP eAttrib, BOOL bUpdate ){
						if ( bUpdate )
							m_wFlags |= (1<<(INT)eAttrib);
						else
							m_wFlags &= (~(1<<(INT)eAttrib));
					}

	VOID			setWeaponID( UINT ID ){ m_WeaponID = ID; SetUpdateAttrib(HEQUIP_WEAPON,TRUE); }
	UINT			getWeaponID( VOID )const{ return m_WeaponID; }

	VOID			setWeaponGemID( UINT ID ){ m_WeaponGemID = ID; SetUpdateAttrib(HEQUIP_WEAPON,TRUE); }
	UINT			getWeaponGemID( VOID )const{ return m_WeaponGemID; }

	VOID			setCapID( UINT ID ){ m_CapID = ID; SetUpdateAttrib(HEQUIP_CAP,TRUE); }
	UINT			getCapID( VOID )const{ return m_CapID; }

	VOID			setCapGemID( UINT ID ){ m_CapGemID = ID; SetUpdateAttrib(HEQUIP_CAP,TRUE); }
	UINT			getCapGemID( VOID )const{ return m_CapGemID; }

	VOID			setArmourID( UINT ID ){ m_ArmourID = ID; SetUpdateAttrib(HEQUIP_ARMOR,TRUE); }
	UINT			getArmourID( VOID )const{ return m_ArmourID; }

	VOID			setArmourGemID( UINT ID ){ m_ArmourGemID = ID; SetUpdateAttrib(HEQUIP_ARMOR,TRUE); }
	UINT			getArmourGemID( VOID )const{ return m_ArmourGemID; }

	VOID			setCuffID( UINT ID ){ m_CuffID = ID; SetUpdateAttrib(HEQUIP_CUFF,TRUE); }
	UINT			getCuffID( VOID )const{ return m_CuffID; }

	VOID			setCuffGemID( UINT ID ){ m_CuffGemID = ID; SetUpdateAttrib(HEQUIP_CUFF,TRUE); }
	UINT			getCuffGemID( VOID )const{ return m_CuffGemID; }

	VOID			setBootID( UINT ID ){ m_BootID = ID; SetUpdateAttrib(HEQUIP_BOOT,TRUE); }
	UINT			getBootID( VOID )const{ return m_BootID; }

	VOID			setBootGemID( UINT ID ){ m_BootGemID = ID; SetUpdateAttrib(HEQUIP_BOOT,TRUE); }
	UINT			getBootGemID( VOID )const{ return m_BootGemID; }

	VOID			setID(HUMAN_EQUIP EquipPoint,UINT ID, UINT uGemID)
	{
			switch(EquipPoint) {
			case HEQUIP_WEAPON:
				{
					setWeaponID(ID);
					setWeaponGemID(uGemID);
				}
				break;
			case HEQUIP_CAP:
				{
					setCapID(ID);
					setCapGemID(uGemID);
				}
				break;
			case HEQUIP_ARMOR:
				{
					setArmourID(ID);
					setArmourGemID(uGemID);
				}
				break;
			case HEQUIP_CUFF:
				{
					setCuffID(ID);
					setCuffGemID(uGemID);
				}
				break;
			case HEQUIP_BOOT:
				{
					setBootID(ID);
					setBootGemID(uGemID);
				}
				break;
			default:
				break;
			}
	}

	VOID FillParamBuf( VOID *pBuf );

private:
	ObjID_t			m_ObjID;		// ObjID

	/*
	|  ref [HUMAN_EQUIP]
	|	 00000000 xxxxxxxx
	|             ||||||||__ 武器  WEAPON
	|             |||||||___ 帽子 	DEFENCE
	|             ||||||____ 衣服  DEFENCE
	|             |||||_____ 护腕  DEFENCE
	|             ||||______ 靴子  DEFENCE
	|             |||_______ 腰带	ADORN
	|             ||________ 戒子	ADORN
	|             |_________ 项链	ADORN
	|
	*/
	WORD			m_wFlags;		// 每个位表示一个属性是否要刷新 HUMAN_EQUIP

	UINT			m_WeaponID;		// 武器 - 资源表ID
	UINT			m_WeaponGemID;	// 武器宝石 - 资源表ID
	UINT			m_CapID;		// 帽子 - 资源表ID
	UINT			m_CapGemID;		// 帽子宝石 - 资源表ID
	UINT			m_ArmourID;		// 衣服 - 资源表ID
	UINT			m_ArmourGemID;	// 衣服宝石 - 资源表ID
	UINT			m_CuffID;		// 护腕 - 资源表ID
	UINT			m_CuffGemID;	// 护腕宝石 - 资源表ID
	UINT			m_BootID;		// 靴子 - 资源表ID
	UINT			m_BootGemID;	// 靴子宝石 - 资源表ID
};


class GCCharEquipmentFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCCharEquipment() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_CHAREQUIPMENT; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(WORD) +
													sizeof(UINT) * 10; }
};

class GCCharEquipmentHandler 
{
public:
	static UINT Execute( GCCharEquipment* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif
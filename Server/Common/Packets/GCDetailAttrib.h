// GCDetailAttrib.h
// 
// 角色的详细属性
// 
//////////////////////////////////////////////////////

#ifndef __GCDETAILATTRIB_H__
#define __GCDETAILATTRIB_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"


namespace Packets
{
	class GCDetailAttrib :
		public Packet
	{
	public:
		GCDetailAttrib( )
		{
			m_Flags.m_uHighFlags	=0;
			m_Flags.m_uLowFlags		=0;
			//m_nCampType				=INVALID_CAMP;
		}
		virtual ~GCDetailAttrib( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_DETAILATTRIB ; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uSize = sizeof(ObjID_t) + sizeof(Flag64) ;
			UINT i;
			for ( i = 0; i < DETAIL_ATTRIB_NUMBERS; i++ )
			{
				if(m_Flags.isSetBit(i))
				{
					switch ( i )
					{
					case DETAIL_ATTRIB_CAMP:
						uSize += sizeof( m_CampData );
						break;
					case DETAIL_ATTRIB_CURRENT_PET_GUID:
						uSize += sizeof( PET_GUID_t );
						break;
					default:
						uSize += sizeof( UINT );	
						break;
					}
				}
			}
			return uSize;
		}

	public:
		VOID			SetObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			GetObjID(VOID)const { return m_ObjID; }

		Flag64			GetFlags( VOID )const{ return m_Flags; }

		BOOL			IsUpdateAttrib( ENUM_DETAIL_ATTRIB eAttrib )const{ return m_Flags.isSetBit((INT)eAttrib)?(TRUE):(FALSE); }
		VOID			SetUpdateAttrib( ENUM_DETAIL_ATTRIB eAttrib, BOOL bUpdate ){ m_Flags.UpdateBits((INT)eAttrib,bUpdate); }

		UINT			GetLevel( VOID )const{ return m_uLevel; }
		VOID			SetLevel( UINT uLevel ){ m_uLevel =uLevel; SetUpdateAttrib( DETAIL_ATTRIB_LEVEL, TRUE ); }

		INT				GetMenpai(VOID)	const	{return m_nMenPai;}
		VOID			SetMenpai(INT	Menpai){ m_nMenPai	=	Menpai; SetUpdateAttrib(DETAIL_ATTRIB_MEMPAI,TRUE); }

		INT				GetGuild(VOID)	const	{return m_nGuild;}
		VOID			SetGuild(INT	nGuild){ m_nGuild	=	nGuild; SetUpdateAttrib(DETAIL_ATTRIB_GUILD,TRUE); }

		const _CAMP_DATA		*GetCampData(VOID)const{ return &m_CampData; }
		VOID					SetCampData( const _CAMP_DATA *pCampData ) { m_CampData = *pCampData; SetUpdateAttrib(DETAIL_ATTRIB_CAMP,TRUE); }
		//INT				GetCampType(VOID)	const	{ return m_nCampType; }
		//VOID			SetCampType(INT	nCampType)		{m_nCampType = nCampType; SetUpdateAttrib(DETAIL_ATTRIB_CAMP,TRUE);}

		INT				GetDataID(VOID)	const	{return m_nDataID;}
		VOID			SetDataID(INT	nDataID)		{m_nDataID = nDataID; SetUpdateAttrib(DETAIL_ATTRIB_DATAID,TRUE);}

		INT				GetPortraitID(VOID)	const	{return m_nPortraitID;}
		VOID			SetPortraitID(INT	nPortraitID)		{m_nPortraitID = nPortraitID; SetUpdateAttrib(DETAIL_ATTRIB_PORTRAITID,TRUE);}

		INT				GetModelID(VOID)	const	{return m_nModelID;}
		VOID			SetModelID(INT	nModelID)		{m_nModelID = nModelID; SetUpdateAttrib(DETAIL_ATTRIB_MODELID,TRUE);}

		INT				GetMountID(VOID)	const	{return m_nMountID;}
		VOID			SetMountID(INT	nID)		{m_nMountID = nID; SetUpdateAttrib(DETAIL_ATTRIB_MOUNTID,TRUE);}

		PET_GUID_t		GetCurrentPetGUID( VOID )const{ return m_guidCurrentPet; }
		VOID			SetCurrentPetGUID( PET_GUID_t guid ){ m_guidCurrentPet =guid; SetUpdateAttrib( DETAIL_ATTRIB_CURRENT_PET_GUID, TRUE ); }

		INT				GetExp( VOID )const{ return m_nExp; }
		VOID			SetExp( INT nExp ){ m_nExp = nExp; SetUpdateAttrib( DETAIL_ATTRIB_EXP, TRUE ); }

		INT				GetHP(VOID) const{return m_nHP;}
		VOID			SetHP(INT nHp){m_nHP	= nHp;SetUpdateAttrib(DETAIL_ATTRIB_HP,TRUE);}

		INT				GetMP(VOID) const {return m_nMP;}
		VOID			SetMP(INT nMp){m_nMP = nMp; SetUpdateAttrib(DETAIL_ATTRIB_MP,TRUE);}

		INT				GetMAXHP(VOID) const {return m_nMAXHP;}
		VOID			SetMAXHP(INT nMaxHp) {m_nMAXHP = nMaxHp;SetUpdateAttrib(DETAIL_ATTRIB_MAXHP,TRUE);}

		INT				GetMAXMP(VOID) const {return m_nMAXMP;}
		VOID			SetMAXMP(INT nMaxMp) {m_nMAXMP = nMaxMp;SetUpdateAttrib(DETAIL_ATTRIB_MAXMP,TRUE);}

		FLOAT			GetMoveSpeed(VOID) const {return m_fMoveSpeed;}
		VOID			SetMoveSpeed(FLOAT fMoveSpeed){m_fMoveSpeed = fMoveSpeed;SetUpdateAttrib(DETAIL_ATTRIB_MOVESPEED,TRUE);}

		UINT			GetMoney( VOID )const{ return m_uMoney; }
		VOID			SetMoney( UINT uMoney ){ m_uMoney =uMoney; SetUpdateAttrib( DETAIL_ATTRIB_MONEY, TRUE ); }

		INT				GetStr() const {return m_Str;}			
		VOID			SetStr(INT str){m_Str	=	str;SetUpdateAttrib(DETAIL_ATTRIB_STR,TRUE);}

		INT				GetSpr() const{return m_Spr;}			
		VOID			SetSpr(INT spr){m_Spr	=	spr;SetUpdateAttrib(DETAIL_ATTRIB_SPR,TRUE);}


		INT				GetCon() const {return m_Con;}			
		VOID			SetCon(INT con){m_Con	=	con;SetUpdateAttrib(DETAIL_ATTRIB_CON,TRUE);}

		INT				GetInt()const {return m_Int;}			
		VOID			SetInt(INT iInt){m_Int	=	iInt;SetUpdateAttrib(DETAIL_ATTRIB_INT,TRUE);}

		INT				GetDex() const {return m_Dex;}			
		VOID			SetDex(INT dex){m_Dex	=	dex;SetUpdateAttrib(DETAIL_ATTRIB_DEX,TRUE);}


		INT				GetPoint_Remain( VOID )const{ return m_nPoint_Remain; }
		VOID			SetPoint_Remain( INT nPoint_Remain ){ m_nPoint_Remain = nPoint_Remain; SetUpdateAttrib( DETAIL_ATTRIB_POINT_REMAIN, TRUE ); }

		INT				GetHP_ReSpeed( VOID )const{ return m_nHP_ReSpeed; }
		VOID			SetHP_ReSpeed( INT nHP_ReSpeed ){ m_nHP_ReSpeed = nHP_ReSpeed; SetUpdateAttrib( DETAIL_ATTRIB_HP_RESPEED, TRUE ); }

		INT				GetMP_ReSpeed( VOID )const{ return m_nMP_ReSpeed; }
		VOID			SetMP_ReSpeed( INT nMP_ReSpeed ){ m_nMP_ReSpeed = nMP_ReSpeed; SetUpdateAttrib( DETAIL_ATTRIB_MP_RESPEED, TRUE ); }


		INT				GetAtt_Physics( VOID )const{ return m_nAtt_Physics; }
		VOID			SetAtt_Physics( INT nAtt_Physics ){ m_nAtt_Physics = nAtt_Physics; SetUpdateAttrib( DETAIL_ATTRIB_ATT_PHYSICS, TRUE ); }

		INT				GetAtt_Magic( VOID )const{ return m_nAtt_Magic; }
		VOID			SetAtt_Magic( INT nAtt_Magic ){ m_nAtt_Magic = nAtt_Magic; SetUpdateAttrib( DETAIL_ATTRIB_ATT_MAGIC, TRUE ); }

		INT				GetDef_Physics( VOID )const{ return m_nDef_Physics; }
		VOID			SetDef_Physics( INT nDef_Physics ){ m_nDef_Physics = nDef_Physics; SetUpdateAttrib( DETAIL_ATTRIB_DEF_PHYSICS, TRUE ); }

		INT				GetDef_Magic( VOID )const{ return m_nDef_Magic; }
		VOID			SetDef_Magic( INT nDef_Magic ){ m_nDef_Magic = nDef_Magic; SetUpdateAttrib( DETAIL_ATTRIB_DEF_MAGIC, TRUE ); }

		INT				GetHit( VOID )const{ return m_nHit; }
		VOID			SetHit( INT nHit ){ m_nHit = nHit; SetUpdateAttrib( DETAIL_ATTRIB_HIT, TRUE ); }

		INT				GetMiss(VOID) const {return m_nMiss;}
		VOID			SetMiss(INT nMiss){m_nMiss	=	 nMiss;SetUpdateAttrib(DETAIL_ATTRIB_MISS,TRUE);}

		INT				GetCritic( VOID )const{ return m_nCritic; }
		VOID			SetCritic( INT Critic ){ m_nCritic = Critic; SetUpdateAttrib( DETAIL_ATTRIB_CRITRATE, TRUE ); }

		INT				GetRage( VOID )const{ return m_nRage; }
		VOID			SetRage( INT Rage ){ m_nRage = Rage; SetUpdateAttrib( DETAIL_ATTRIB_RAGE, TRUE ); }

		INT				GetStrikePoint( VOID )const{ return m_nStrikePoint; }
		VOID			SetStrikePoint( INT StrikePoint ){ m_nStrikePoint = StrikePoint; SetUpdateAttrib( DETAIL_ATTRIB_STRIKE_POINT, TRUE ); }

		INT				GetAttSpeed(VOID) const {return m_nAttackSpeed;}
		VOID			SetAttSpeed(INT AttSpeed){m_nAttackSpeed = AttSpeed;SetUpdateAttrib(DETAIL_ATTRIB_ATTACKSPEED,TRUE);}

		INT				GetAttCold(VOID)	const {return m_nAttCold;}
		VOID			SetAttCold(INT AttCold){ m_nAttCold = AttCold;SetUpdateAttrib(DETAIL_ATTRIB_ATTACKCOLD,TRUE);}

		INT				GetAttFire(VOID) const {return m_nAttFire;}
		VOID			SetAttFire(INT AttFire){m_nAttFire = AttFire;SetUpdateAttrib(DETAIL_ATTRIB_ATTACKFIRE,TRUE);}

		INT				GetAttLight(VOID)	const {return m_nAttLight;}
		VOID			SetAttLight(INT AttLight){m_nAttLight	=	 AttLight; SetUpdateAttrib(DETAIL_ATTRIB_ATTACKLIGHT,TRUE);}

		INT				GetAttPoison(VOID)	const {return m_nAttPoison;}
		VOID			SetAttPoison(INT AttPoison){m_nAttPoison	= AttPoison; SetUpdateAttrib(DETAIL_ATTRIB_ATTACKPOISON,TRUE);}

		INT				GetDefCold(VOID)	const {return m_nDefCold;}
		VOID			SetDefCold(INT DefCold){ m_nDefCold = DefCold;SetUpdateAttrib(DETAIL_ATTRIB_DEFENCECOLD,TRUE);}

		INT				GetDefFire(VOID) const {return m_nDefFire;}
		VOID			SetDefFire(INT DefFire){m_nDefFire = DefFire;SetUpdateAttrib(DETAIL_ATTRIB_DEFENCEFIRE,TRUE);}

		INT				GetDefLight(VOID)	const {return m_nDefLight;}
		VOID			SetDefLight(INT DefLight){m_nDefLight	=	 DefLight; SetUpdateAttrib(DETAIL_ATTRIB_DEFENCELIGHT,TRUE);}

		INT				GetDefPoison(VOID)	const {return m_nDefPoison;}
		VOID			SetDefPoison(INT DefPoison){m_nDefPoison	= DefPoison; SetUpdateAttrib(DETAIL_ATTRIB_DEFENCEPOISON,TRUE);}

		INT				GetLimitMove(VOID)	const {return m_bLimitMove;}
		VOID			SetLimitMove(BOOL bLimitMove){m_bLimitMove	= bLimitMove; SetUpdateAttrib(DETAIL_ATTRIB_LIMIT_MOVE,TRUE);}

		INT				GetCanActionFlag1(VOID)	const {return m_bCanActionFlag1;}
		VOID			SetCanActionFlag1(BOOL bFlag){m_bCanActionFlag1	= bFlag; SetUpdateAttrib(DETAIL_ATTRIB_CAN_ACTION1,TRUE);}

		INT				GetCanActionFlag2(VOID)	const {return m_bCanActionFlag2;}
		VOID			SetCanActionFlag2(BOOL bFlag){m_bCanActionFlag2	= bFlag; SetUpdateAttrib(DETAIL_ATTRIB_CAN_ACTION2,TRUE);}

		//INT			GetLimitHandle(VOID)	const {return m_bLimitHandle;}
		//VOID			SetLimitHandle(BOOL bLimitHandle){m_bLimitHandle	= bLimitHandle; SetUpdateAttrib(DETAIL_ATTRIB_LIMIT_HANDLE,TRUE);}

		INT				GetRMB(VOID)	const {return m_RMBMoney;}
		VOID			SetRMB(INT nRMB){m_RMBMoney	= nRMB; SetUpdateAttrib(DETAIL_ATTRIB_RMB,TRUE);}

		INT				GetBankRMB(VOID)	const {return m_BankMoney;}
		VOID			SetBankRMB(INT nRMB){m_BankMoney	= nRMB; SetUpdateAttrib(DETAIL_ATTRIB_BANK_RMB,TRUE);}

		INT				GetDoubleExpTime_Num(VOID)	const {return m_DoubleExpTime_Num;}
		VOID			SetDoubleExpTime_Num(INT nExpTime_Num){m_DoubleExpTime_Num	= nExpTime_Num; SetUpdateAttrib(DETAIL_ATTRIB_ENERGY,TRUE);}

		/*INT				GetGmRight(VOID)	const {return m_GmRight;}
		VOID			SetGmRight(INT nRight){m_GmRight	= nRight; SetUpdateAttrib(DETAIL_ATTRIB_MAX_ENERGY,TRUE);}*/

	private:
		ObjID_t			m_ObjID;	// 所有Obj类型的ObjID

		Flag64			m_Flags;		// 每个位表示一个属性是否要刷新 ENUM_DETAIL_ATTRIB


		UINT			m_uLevel;		// 等级
		INT				m_nExp;			// 经验值
		INT				m_nHP;			//生命点
		INT				m_nMP;			//魔法点
		INT				m_nMAXHP;		//最大生命点
		INT				m_nMAXMP;		//最大魔法点
		FLOAT			m_fMoveSpeed;	//移动速度


		UINT			m_uMoney;		// 游戏币数

		//一级战斗属性


		INT				m_Str;					//力量 力量
		INT				m_Spr;					//灵气 灵力
		INT				m_Con;					//体制 体制
		INT 			m_Int;					//定力 智力
		INT 			m_Dex;					//身法 敏捷
		INT				m_nPoint_Remain;		//剩余待分配点数

		//二级战斗属性
		INT				m_nHP_ReSpeed;		//HP恢复速度  点/秒
		INT				m_nMP_ReSpeed;		//MP恢复速度  点/秒
		INT				m_nAtt_Physics;		//物理攻击力
		INT				m_nAtt_Magic;		//魔法攻击力
		INT				m_nDef_Physics;		//物理防御力
		INT				m_nDef_Magic;		//魔法防御力
		INT				m_nHit;				//命中率
		INT				m_nMiss;			//闪避率
		INT				m_nCritic;			//致命一击率

		INT				m_nRage;			//怒气
		INT				m_nStrikePoint;		//连技点

		INT				m_nAttackSpeed;		//攻击速度

		INT				m_nAttCold;			//冰攻击
		INT				m_nDefCold;			//冰防御
		INT				m_nAttFire;			//火攻击
		INT				m_nDefFire;			//火防御
		INT				m_nAttLight;		//电攻击
		INT				m_nDefLight;		//电防御

		INT				m_nAttPoison;		//毒攻击
		INT				m_nDefPoison;		//毒防御

		INT				m_nMenPai;			//门派
		INT				m_nGuild;			//帮派


		_CAMP_DATA		m_CampData;			// 阵营	
		INT				m_nDataID;			// DataID
		INT				m_nPortraitID;		// 头像ID
		INT				m_nModelID;			// 外形
		INT				m_nMountID;			//座骑
		PET_GUID_t		m_guidCurrentPet;	//当前宠物
		
		BOOL			m_bLimitMove;		//是否限制不能移动
		BOOL			m_bCanActionFlag1;		//技能受限标记1,用于昏迷催眠
		BOOL			m_bCanActionFlag2;		//技能受限标记2,用于沉默

		INT				m_RMBMoney;			//m_Vigor ;			// 活力  //元宝
		INT				m_BankMoney;		//m_MaxVigor ;		// 活力上限 //银行中的元宝
		INT				m_DoubleExpTime_Num ;			// 双倍经验时间和倍数;
		INT				m_GmRight ;		// 精力上限

		//INT				m_nGoodBadValue;	// 善恶值
	};

	class GCDetailAttribFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCDetailAttrib() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_DETAILATTRIB; }
		UINT		GetPacketMaxSize()const { return	sizeof(GCDetailAttrib) - sizeof(Packet); }
														
	};

	class GCDetailAttribHandler 
	{
	public:
		static UINT Execute( GCDetailAttrib* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCDETAILATTRIB_H__

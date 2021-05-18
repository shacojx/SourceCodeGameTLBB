/////////////////////////////////////////////////////////////////////////////////
//文件名：Item.h
//功能描述：物品数据的封装类，通过此类来读取及设置一个物品的信息
//修改记录：2005-11-2创建
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __ITEM_H__
#define __ITEM_H__

#include "Type.h"

struct _PET_DB_LOAD;
struct  PET_GUID_t;
struct _PET_SKILL;


class _ITEM_INIT
{
public:
	_ITEM_INIT(const _ITEM*	pItem)
	{
		m_pItem = pItem ;
		m_pPet  = NULL;
	}

	_ITEM_INIT(const _PET_DB_LOAD* pPet)
	{
		m_pPet	= pPet;
		m_pItem = NULL;
	}
	const	_ITEM*				m_pItem ;//物品详细数据
	const	_PET_DB_LOAD*		m_pPet;	 //宠物详细数据
};

enum ITEM_DATA_TYPE
{
	IDT_ITEM,
	IDT_PET,
};

struct _CAMP_DATA;
class Item
{
friend class ItemOperator ;
friend class ItemContainer ;



public :
	Item( ) ;
	~Item( ) ;

	ITEM_DATA_TYPE			GetDataType(){ return m_Type;}//取得数据类型

	VOID					SaveValueTo(_ITEM* pItem);
	VOID					SaveValueTo(_PET_DB_LOAD* pPet);
	inline  _ITEM_GUID		GetGUID( ) ;					//取得物品GUID
	inline  UINT			GetItemTableIndex( ) const; 	//取得物品表里的物品索引编号
	inline	UCHAR			GetItemClass()		 const;		//取得物品的	enum ITEM_CLASS
	inline  UCHAR			GetItemQual()		 const;		//取得物品的品质
	inline	UCHAR			GetItemType()		 const;		//取得物品的小类WEAPON_TYPE DEFENCE_TYPE 等
	inline	UCHAR			GetItemIndex()		 const;		//取得物品的流水号



	BOOL					IsCanEquipBind()	const;		//取得物品是否具有绑定属性
	BOOL					IsRuler(ITEM_RULER_LIST	ruler)	const;			//取得物品是否可以执行指定规则
	
	UINT					GetPrice()	const;
	INT						GetItemParam(UINT start,ItemParamValue& ipv);
	/*=============================================
	 *	装备属性
	 =============================================*/
	inline USHORT			GetSetNum()					const;			//套装编号
	inline BYTE				GetMaxSetCount()			const;			//套装个数
	inline BYTE				GetEquipPoint()				const;			//物品装佩点
	inline BYTE				GetMaxDurPoint()			const ;			//最大耐久值
	inline BYTE				GetRequireLevel()			const;			//需求等级
	inline BYTE				GetGemSlotMax()				const;			//最大宝石个数
	inline BYTE				GetFailTimes()				const;			//修理失败次数
	inline BYTE				GetDurPoints()				const; 			//当前耐久值
	inline BYTE				GetAttrCount()				const;			//属性的数量
	inline USHORT			GetDamagePoint()			const;			//物品损伤累计值
	inline BOOL				GetItemBind()				const;			//物品是否绑定
	inline BOOL				GetItemIdent()				const;			//物品是否鉴定
	inline BYTE				GetEquipGemCount()			const;			//镶嵌宝石的数量
	inline const _ITEM_ATTR		GetEquipAttr(UCHAR	nIndex)		const;		//获得装备属性
	inline const _ITEM_GEMINFO	GetEquipGemInfo(UCHAR nIndex)	const;		//获得装备上宝石的编号信息

	/*==========================================
	 *	普通物品属性
	 ============================================*/
	
	inline BYTE				GetItemLevel()			const;				//物品级别,策划需求
	inline BYTE				GetReqSkillLevel()		const;				//获得技能级别需求
	inline INT				GetReqSkill()			const;				//需要拥有的技能号
	inline INT				GetScriptID()			const;				//获得相关脚本编号
	inline INT				GetSkillID()			const;				//获得相关技能编号
	inline BOOL				IsCosSelf()				const;				//是否消耗自己
	inline BYTE				GetTargetType()			const;
	
	/*==============================================
	 *宝石属性	
	 ===============================================*/
	inline const _ITEM_ATTR		GetGemAttr()			const;				//宝石属性

	/*==========================================
	 *藏宝图属性	
	============================================*/

	inline INT				GetMapLevel()			const;				//藏宝图级别
	inline FLOAT			GetXPos()				const;				//x坐标
	inline FLOAT			GetZPos()				const;				//z坐标
	inline INT				GetSceneID()			const;				//场景编号
	inline INT				GetGrowType()			const;				//对应生长点类型
public:
	//ITEM 和PET 公用函数
	
	BOOL					Init( const _ITEM_INIT* pInit );//初始化物品
	BOOL					IsEmpty( ) ;					//判断物品是否为空，即没有物品数据
	BOOL					IsEquip( ) const;				//判断此物品是否为一件装备
	BOOL					IsLock( ){ return m_bLock ; } ; //判断此物品是否被锁定
	BOOL					IsCanLay( )			const;		//取得物品是否可以叠放
	inline	INT				GetLayedNum( )  const;			//取得叠放物品的数量
	inline	INT				GetMaxLayedNum( ) const;		//取得能够叠放物品的最大数量
	inline  BOOL			IsFull()	const;				//是否已经达到最大叠加数目

	inline  BOOL			IsPWLock()	const;				//受密码保护
public:
	//宠物函数

	inline  PET_GUID_t			GetPetGUID( ) ;					//取得宠物GUID
	inline  PET_GUID_t			GetSpouseGUID( ) ;				//取得宠物配偶GUID
	inline	INT					GetDataID();					//取得宠物模型		
	inline  const CHAR*			GetName()	const;				//取得名字
	inline  const CHAR*			GetNick() const;				//取得昵称
	inline  INT					GetLevel() const;				//取得等级
	inline  INT					GetTakeLevel() const;			//取得携带等级
	inline  INT					GetAttackType() const;			//取得进攻类型（物/法）
	inline  INT					GetAIType() const;				//取得AI类型
	inline  const _CAMP_DATA	*GetCampData() const;			//取得阵营
	inline  INT					GetHP() const;					//取得生命值
	inline  INT					GetBaseMaxHP() const;			//取得生命值上限
	inline  INT					GetLife() const;				//取得当前寿命
	inline  BYTE				GetPetType() const;				//宝宝，变异，野生
	inline  BYTE				GetGeneration() const;			//几代宠
	inline  BYTE				GetHappiness() const;			//快乐度		
	inline  INT					GetStrPer() const;				//力量资质	
	inline  INT					GetConPer() const;				//体力资质	
	inline  INT 				GetDexPer() const;				//身法资质
	inline  INT					GetSprPer() const;				//灵气资质
	inline  INT 				GetIntPer() const;				//定力资质
	inline	INT					GetGenGu() const;				//根骨
	inline  FLOAT				GetGrowRate() const ;			//成长率
	inline  INT					GetRemainPoint() const;			//一级属性剩余点数
	inline  INT					GetExp() const;					//经验值
	inline  INT					GetLvl1Attr(CHAR_ATTR_LEVEL1 type)const;// 基础一级战斗属性（不包括技能和装备增加的部分）
	inline  _PET_SKILL			GetSkill(UINT SkillIndex);		// 宠物技能
	inline  VOID				SetCooldown(CooldownID_t nID, Time_t nTime);
	inline	BOOL				IsCooldowned(CooldownID_t nID) const;
	inline	VOID				HeartBeat_Cooldown(Time_t nDeltaTime);
	inline	Time_t				GetCooldownRemained(CooldownID_t nID) const;
	inline	VOID				SetCoinType(BYTE nType) {m_nCoinType = nType;}
	inline	BYTE				GetCoinType(VOID) { return m_nCoinType; }

	/*==============================================
	*功能接口
	===============================================*/
	//交易
	BOOL						IsInExchange(){return m_bIsInExchange;}
	VOID						SetInExchange(BOOL bIsInExchange){m_bIsInExchange = bIsInExchange;}

protected :
	VOID					Lock( ){ m_bLock = TRUE ; }
	VOID					Unlock( ){ m_bLock = FALSE ; }
	VOID					SetItemValue(_ITEM* pItem);
	VOID					CleanUp( ) ;//清除物品数据


private :
	ITEM_DATA_TYPE			m_Type;
	_ITEM*					m_pItem ;					//物品详细数据
	_PET_DB_LOAD*			m_pPet;						//
	BOOL					m_bLock ;
	BOOL					m_bIsInExchange;			//标记此物品正在被放到交易栏中
	BYTE					m_nCoinType;				//货币类型
};

#include "Item.inl"

struct _EXCHANGE_ITEM_LIST 
{
	INT				m_ListCount;
	Item*			m_ListItem[EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE];

	_EXCHANGE_ITEM_LIST()
	{
		memset(this,0,sizeof(_EXCHANGE_ITEM_LIST));
	}

	VOID	AddItem(Item* it)
	{
		INT i;

		for(i=0;i<EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE;i++)
		{
			if(m_ListItem[i] == NULL)
			{
				break;
			}
		}

		if(i == EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE)
		{
			Assert(0);
			return;
		}

		m_ListCount++;
		m_ListItem[i] = it;
	}

	VOID Init()
	{
		memset(this,0,sizeof(_EXCHANGE_ITEM_LIST));
	}
};



#endif

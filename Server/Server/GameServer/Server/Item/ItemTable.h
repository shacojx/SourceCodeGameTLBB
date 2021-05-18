/********************************************************************
	创建日期:	2005年11月2日
	创建时间:	16:54
	文件名称:	ItemTable.h
	文件路径:	d:\Prj\Server\Server\Item\ItemTable.h
	
	文件功能： 物品基础数据表操作
	修改纪录：
	
*********************************************************************/

#ifndef _ITEM_TABLE_H_
#define _ITEM_TABLE_H_

#include "ItemTypes.h"
#include "Type.h"
#include "GameStruct_Item.h"
#include "GameDefine2.h"

#define SELF_CONS(x)	x(){memset(this,0,sizeof(x));}
#define DEFAULT_CONS(theClass,parentClass)	theClass(){memset(this,0,sizeof(theClass)); parentClass();}



//装备都有的基础属性
struct EQUIP_TB 
{
	SELF_CONS(EQUIP_TB);
	INT						m_IndexID;
	_ITEM_TYPE				m_ItemType;
	BYTE					m_EquipPoint;					//装配点
	CHAR					m_RulerID;						//规则编号
	BYTE					m_ReqLevel;						//需要等级
	INT						m_MaxDur;						//最大耐久
	UINT					m_BasePrice;					//售出价格
	INT						m_RepaireLevel;					//修理价格
	INT						m_PhysicAttack;					//物理攻击
	INT						m_MagicAttack;					//魔法攻击
	INT						m_PhysicDefense;				//物理防御
	INT						m_MagicDefense;					//魔法防御
	INT						m_AttackSpeed;					//攻击速度
	INT						m_Miss;							//闪避	
	BYTE					m_CanRepaireTimes;				//可修理次数
	BYTE					m_MaxGem;						//最大镶嵌
	INT						m_EquipSetNum;					//套装编号
	INT						m_EquipSetMaxNum;				//最大套装数目
};


//掉落包结构
struct DROP_BOX_TB {

	SELF_CONS(DROP_BOX_TB);
	uint				m_DropBoxId;						//DropBox的编号表
	INT					m_DropValue;						//DropBox价值，参与掉落计算
	_ITEM_TYPE			m_DropItem[MAX_DROPBOX_CARRAGE];	//DropBox携带物品表
	INT					m_Quality[MAX_DROPBOX_CARRAGE];		//物品品质表	
};

//物品质量段结构体
struct ITEM_QUALITY_TB {

	SELF_CONS(ITEM_QUALITY_TB)
	uint		m_ItemLevel;
	INT			m_Quality[MAX_ITEM_TABLE_QUALITYS];
};


struct MINMAX_VALUE 
{
	SELF_CONS(MINMAX_VALUE)
	INT			m_minValue;
	INT			m_maxValue;
};


enum  MINMAX_TYPE
{
	VT_MIN,
	VT_MAX
};
//物品数值段结构体
struct ITEM_VALUE_TB 
{
	SELF_CONS(ITEM_VALUE_TB)
	uint					m_ValueType;
	MINMAX_VALUE			m_Values[MAX_ITEM_TABLE_VALUES];
};


//普通装备表
struct COMMON_EQUIP_TB :public	EQUIP_TB
{
	DEFAULT_CONS(COMMON_EQUIP_TB,EQUIP_TB);
	INT						m_Job;								//职业属性
};

//蓝色装备表
struct BLUE_EQUIP_TB:public EQUIP_TB 
{
	DEFAULT_CONS(BLUE_EQUIP_TB,EQUIP_TB);
	INT						m_Job;								//职业属性
	INT						m_AttrRate[IATTRIBUTE_NUMBER];		//属性随机概率
};

//绿色装备表
struct GREEN_EQUIP_TB :public	EQUIP_TB
{
	DEFAULT_CONS(GREEN_EQUIP_TB,EQUIP_TB);
	INT						m_Job;								//职业属性
	_ITEM_ATTR				m_Attr[MAX_GREEN_ITEM_EXT_ATTR];	//绿色装备附加属性
};





//怪物掉落包结构体
struct MONSTER_DROPBOX_TB 
{
	SELF_CONS(MONSTER_DROPBOX_TB);
	uint					m_MonsterId;
	uint					m_MonsterValue;
	uint					m_DropType;
	MONSTER_DROPBOXS		m_DropBoxs;
};


//宝石表结构体
struct GEMINFO_TB 
{
	GEMINFO_TB()
	{
		m_nTableIndex	=	0;
		m_ResourceID	=	0;
		m_nRulerID		=	0;
		m_nPrice		=	0;
		m_ItemType.CleanUp();
		m_GenAttr.CleanUp();
	}
	uint					m_nTableIndex;;
	_ITEM_TYPE				m_ItemType;
	WORD					m_ResourceID;
	CHAR					m_nRulerID;
	UINT					m_nPrice;
	_ITEM_ATTR				m_GenAttr;
};



//普通物品表结构体
struct COMMITEM_INFO_TB 
{
	COMMITEM_INFO_TB()
	{
		m_nTableIndex		=	0;
		m_nLevel			=	0;
		m_nBasePrice		=	0;
		m_nLayedNum			=	0;
		m_nScriptID			=	0;
		m_nSkillID			=	0;
		m_nRulerID			=	0;
		m_bCosSelf			=	FALSE;
		m_nReqSkill			=	-1;
		m_nReqSkillLevel	=	-1;
		m_TargetType		=	0;
		m_ItemType.CleanUp();
		
	}

	INT					m_nTableIndex;
	_ITEM_TYPE			m_ItemType;
	INT					m_nLevel;
	UINT				m_nBasePrice;
	CHAR				m_nRulerID;	 //规则ID
	INT					m_nLayedNum; //叠放数量
	INT					m_nScriptID;
	INT					m_nSkillID;
	BOOL				m_bCosSelf;
	INT					m_nReqSkill;
	INT					m_nReqSkillLevel;
	BYTE				m_TargetType;
};


//藏宝图结构体
struct STORE_MAP_INFO_TB 
{
	STORE_MAP_INFO_TB()
	{
		m_nTableIndex	=	0;
		m_ItemType.CleanUp();
		m_ResourceID	=	0;
		m_nRulerID		=	0;
		m_nLevel		=	0;
		m_nBasePrice	=	0;
		m_xPos			=	-1.0f;
		m_zPos			=	-1.0f;
		m_SceneID		=	-1;
		m_GrowPointType	=	-1;

	}
	INT						m_nTableIndex;;
	_ITEM_TYPE				m_ItemType;
	WORD					m_ResourceID;
	CHAR					m_nRulerID;
	INT						m_nLevel;
	UINT					m_nBasePrice;
	FLOAT					m_xPos;
	FLOAT					m_zPos;
	INT						m_SceneID;
	INT						m_GrowPointType;
	INT						m_ScriptID;
	
};

//掉落衰减表结构体
struct DROP_ATT_TB 
{
	SELF_CONS(DROP_ATT_TB);
	INT				m_DeltaLevel;
	FLOAT			m_AttValue;
};


//装备集合结构体
struct EQUIP_SET_TB
{
	SELF_CONS(EQUIP_SET_TB);
	INT				m_nEquipSetSerial;
	INT				m_nAttrCount;
	_ITEM_ATTR		m_ItemAttr[MAX_ITEM_SET_ATTR];
};


//物品规则结构体
struct ITEM_RULER_TB :public _ITEM_RULER{
	
	SELF_CONS(ITEM_RULER_TB);
	INT		m_RulerIndex;
	
};


/*
 *物品相关表资源数据类	
 */
class ItemTable
{
	public:
		ItemTable() ;
		~ItemTable() ;

		VOID	CleanUp( ) ;
		BOOL	Init();


	public:
		MONSTER_DROPBOX_TB*			GetMonsterDropTB(INT iMonsterType);
		DROP_BOX_TB*				GetDropBoxTB(INT iDropBox);
		
		GREEN_EQUIP_TB*				GetGreenItemTB(UINT itemSerial);
		COMMON_EQUIP_TB*			GetWhiteItemTB(UINT itemSerial);
		BLUE_EQUIP_TB*				GetBlueItemTB(UINT itemSerial);
		
		ITEM_QUALITY_TB*			GetItemQualityTB(INT ItemLevel);
		MINMAX_VALUE				GetItemValue(ITEM_ATTRIBUTE iAtt,INT QualityLevel);

		DROP_ATT_TB*				GetDropAttTB(INT iDeltaLevel);

		GEMINFO_TB*					GetGemInfoTB(UINT	itemSerial);
	

		COMMITEM_INFO_TB*			GetCommItemInfoTB(UINT	itemSerial);

		EQUIP_SET_TB*				GetEquipSetTB(INT EquipSetSerial);
		
		ITEM_RULER_TB*				GetRuleValueByID(INT& iIndex);

		STORE_MAP_INFO_TB*			GetStoreMapTB(UINT	itemSerial);;

	protected:

		VOID						InitWhiteItemTable();
		VOID						InitGreenItemTable();
		VOID						InitBlueItemTable();
		VOID						InitGoldenItemTable();
		VOID						InitDropBoxTable();
		VOID						InitMonsterDropBoxTable();
		VOID						InitItemLevelTable();
		VOID						InitItemValueTable();
		VOID						InitGemInfoTable();
		VOID						InitCommItemInfoTable();
		VOID						InitDropAttTable();
		VOID						InitEquipSetAttrTable();
		VOID						InitItemRulerTable();
		VOID						InitStoreMapTable();
		

	private:
		uint						m_nCommonItemCount;
		COMMON_EQUIP_TB*			m_pCommonEquipTableData;

		uint						m_nGreenItemCount;
		GREEN_EQUIP_TB*				m_pGreenEquipTableData;

		uint						m_nBlueItemCount;
		BLUE_EQUIP_TB*				m_pBlueEquipTableData;

		uint						m_nGoldenItemCount;

		uint						m_nDropBoxCount;
		DROP_BOX_TB*				m_pDropBoxTableData;

		uint						m_nMonsterDropBoxCount;
		MONSTER_DROPBOX_TB*			m_pMonsterDropTableData;


		uint						m_nItemLevelCount;
		ITEM_QUALITY_TB*			m_pItemQualityData;

		uint						m_nValueCount;
		ITEM_VALUE_TB*				m_pItemValueData;	


		uint						m_nGemCount;
		GEMINFO_TB*					m_pGemInfoData;

		uint						m_nCommItemCount;
		COMMITEM_INFO_TB*			m_pCommItemInfoData;

		uint						m_nDropAttCount;
		DROP_ATT_TB*				m_pDropAttData;

		uint						m_nEquipSetCount;
		EQUIP_SET_TB*				m_pEquipSetData;

		uint						m_nItemRulerCount;
		INT							m_nHashOffSet;
		ITEM_RULER_TB*				m_pItemRulerData;

		uint						m_nStoreMapCount;
		STORE_MAP_INFO_TB*			m_pStoreMapData;
		
};

BYTE	GetItemTileMax(_ITEM_TYPE& it);


extern ItemTable g_ItemTable ;

#endif


#ifndef __GAMESTRUCT_ITEM_H__
#define __GAMESTRUCT_ITEM_H__

#include "Type.h"


class SocketInputStream;
class SocketOutputStream;


BYTE		GetSerialType(UINT Serial);
BYTE		GetSerialClass(UINT Serial);
BYTE		GetSerialQual(UINT Serial);
UINT		GetSerialIndex(UINT Serial);







//范围
struct _RANGE_VALUE
{
	WORD				m_MinValue ;
	WORD				m_MaxValue ;
};

struct _ITEM_SKILL 
{
	BOOL				m_bActive;
	WORD				m_SkillID;
	WORD				m_SkillRate;

	BOOL				IsActive()
	{
		return m_bActive;
	}

	VOID				SetActive(BOOL bActive)
	{	
		m_bActive = bActive;
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(_ITEM_SKILL));
	}

	_ITEM_SKILL()
	{
		CleanUp();
	}
};

//物品属性值
struct _ITEM_VALUE
{
	SHORT			m_Value ;
	VOID	CleanUp( )
	{
		memset( this, 0, sizeof(*this) ) ;
	}

	BOOL	operator == (_ITEM_VALUE& iV)
	{
		return iV.m_Value = m_Value;
	}
};

//物品属性
struct _ITEM_ATTR
{
	BYTE				m_AttrType ;//如果是普通装备，则m_AttrType用enum ITEM_ATTRIBUTE
									//    来表示属性类型，m_Value表示具体属性值
									//如果是绿色的套装，则m_AttrType表示当前套装在
									//    玩家身上的装备件数，m_Value表示套装的索引号
	_ITEM_VALUE			m_Value ;

	VOID	CleanUp( )
	{
		memset( this, 0, sizeof(*this) ) ;
	}
	
	BOOL	operator==(_ITEM_ATTR& iA)
	{
		return	(iA.m_AttrType == m_AttrType) 
			   &&(iA.m_Value == m_Value);

	}
	
};



//物品唯一ID
struct _ITEM_GUID
{
	BYTE	m_World ;		//世界号: (例)101
	BYTE	m_Server ;		//服务端程序号：(例)5
	INT		m_Serial ;		//物品序列号：(例)123429
	
	_ITEM_GUID()
	{
		m_Serial = 0;
		m_Server = 0;
		m_World	 = 0;	
	}
	VOID Reset(VOID)
	{
		m_Serial = 0;
		m_Server = 0;
		m_World	 = 0;	
	}
	_ITEM_GUID& operator=(_ITEM_GUID const& rhs)
	{
		m_Serial = rhs.m_Serial;
		m_Server = rhs.m_Server;
		m_World  = rhs.m_World;
		return *this;
	}
	BOOL	operator ==(_ITEM_GUID& Ref)	const
	{
		return (Ref.m_Serial==m_Serial)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}

	BOOL	operator ==(const _ITEM_GUID& Ref)	const
	{
		return (Ref.m_Serial==m_Serial)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}

	BOOL	isNull() const
	{
		return (m_World ==0)&&(m_Serial==0)&&(m_Server == 0);
	}
	
};

struct _ITEM_TYPE
{
	
	struct  
	{
			UINT	m_Class : 7 ;		//基本类型，比如：装备类、消耗品、等
			UINT	m_Quality: 7 ;		//档次，比如：蓝色装备、绿色装备、等
			UINT	m_Type: 7 ;		//类别，比如：大刀、长枪、等
			UINT	m_Index: 11 ;		//物品号，比如：柳叶刀、鸳鸯刀、等
	};
	
	UINT	ToUINT()
	{
		UINT	uid = 0 ;

		uid	+= m_Class;
		uid= uid<<7;
		uid	+= m_Quality;
		uid= uid<<7;
		uid	+= m_Type;
		uid= uid<<11;
		uid += m_Index;

		return uid;

	}

	UINT	ToSerial()	const
	{
		UINT Serial;
		Serial = m_Class;
		Serial	= Serial*100+m_Quality;
		Serial	= Serial*100+m_Type;
		Serial	= Serial*1000+m_Index;
		return Serial;
	}

	

	BOOL	isNull() const
	{
		return (m_Class==0)&&(m_Quality==0)&&(m_Type==0)&&(m_Index == 0);
	}

	BOOL	operator==(_ITEM_TYPE& Ref) const
	{
		return (m_Class==Ref.m_Class)&&(m_Quality==Ref.m_Quality)&&(m_Type==Ref.m_Type)&&(m_Index==Ref.m_Index);
	}
	
	BOOL	operator>(_ITEM_TYPE& Ref) const
	{
		return ToSerial()>Ref.ToSerial();
	}

	BOOL	operator<(_ITEM_TYPE& Ref) const
	{
		return ToSerial()<Ref.ToSerial();		
	}
	VOID	CleanUp()
	{
		m_Class		=	0;
		m_Quality	=	0;
		m_Type		=	0;
		m_Index		=	0;
	}


};


_ITEM_TYPE	ConvertSerial2ItemType(UINT Serial);


struct _ITEM_RULER
{
	BOOL	m_Discard;		//丢弃
	BOOL	m_Tile;			//重叠
	BOOL	m_ShortCut;		//快捷
	BOOL	m_CanSell;		//出售
	BOOL	m_CanExchange;	//交易
	BOOL	m_CanUse;		//使用
	BOOL	m_PickBind;		//拾取绑定
	BOOL	m_EquipBind;	//装备绑定
	BOOL	m_Unique;		//是否唯一


	BOOL	CanDiscard()	{return m_Discard;}		//丢弃
	BOOL	CanTile()		{return m_Tile;}		//重叠
	BOOL	CanShortCut()	{return m_ShortCut;}	//快捷
	BOOL	CanSell()		{return m_CanSell;}		//出售
	BOOL	CanExchange()	{return m_CanExchange;}	//交易
	BOOL	CanUse()		{return m_CanUse;}		//使用
	BOOL	isPickBind()	{return m_PickBind;}	//拾取绑定
	BOOL	isEquipBind()	{return m_EquipBind;}	//装备绑定
	BOOL	isUnique()		{return m_Unique;}		//是否唯一
};


enum	ITEM_RULER_LIST
{
	IRL_DISCARD,			//丢弃
	IRL_TILE,				//重叠
	IRL_SHORTCUT,			//快捷
	IRL_CANSELL,			//出售
	IRL_CANEXCHANGE,		//交易
	IRL_CANUSE,				//使用
	IRL_PICKBIND,			//拾取绑定
	IRL_EQUIPBIND,			//装备绑定
	IRL_UNIQUE				//是否唯一
};



//镶嵌宝石信息
struct _ITEM_GEMINFO
{
	UINT	m_GemType;
};




struct GEM_INFO
{
	
	_ITEM_ATTR	m_Attr;
	UINT		m_nPrice;
	

	VOID	CleanUp()
	{
		m_nPrice	=	0;
		
		m_Attr.CleanUp();
	}
};


struct STORE_MAP_INFO 
{
	INT						m_nLevel;
	FLOAT					m_xPos;
	FLOAT					m_zPos;
	INT						m_SceneID;
	INT						m_GrowType;
	
};

struct MEDIC_INFO
{
	
	BYTE					m_nCount;	   //当前数量
	BYTE					m_nLevel;
	BYTE					m_nLayedNum;   //叠放数量
	BYTE					m_nReqSkillLevel;

	UINT					m_nBasePrice;
	INT						m_nScriptID;
	INT						m_nSkillID;
	BOOL					m_bCosSelf;	   //是否消耗自己
	INT						m_nReqSkill;
	BYTE					m_TargetType;  //物品选择类型
	
	INT GetTileNum()		{return m_nCount;}

	INT GetMaxTileNum()		{return m_nLayedNum;}

	VOID	CleanUp()
	{
		m_nLevel			=	0;
		m_nBasePrice		=	0;
		m_nLayedNum			=	0;
		m_nScriptID			=	INVALID_ID;
		m_nSkillID			=	0;
		m_nCount			=	0;
		m_bCosSelf			=	FALSE;
		m_nReqSkill			=	-1;
		m_nReqSkillLevel	=	-1;
		m_TargetType		=	0;
	}
};


struct EQUIP_INFO 
{
	SHORT				m_SetNum;					//套装编号
	UINT				m_BasePrice;				//售出价格
	CHAR				m_MaxNum;					//套装
	BYTE				m_EquipPoint;				//物品装佩点
	BYTE				m_MaxDurPoint ;				//最大耐久值
	BYTE				m_NeedLevel ;				//需求等级
	BYTE				m_GemMax ;					//最大宝石个数
	//////////////////////////////////////////////////////////////////////////
	//以上为固定属性
	BYTE				m_FaileTimes ;				//修理失败次数
	BYTE				m_CurDurPoint ;				//当前耐久值
	USHORT				m_CurDamagePoint;			//当前损伤度
	BYTE				m_AttrCount;				//属性的数量
	BYTE				m_StoneCount;				//镶嵌宝石的数量
	_ITEM_ATTR			m_pAttr[MAX_ITEM_ATTR];		//物品属性
	_ITEM_GEMINFO		m_pGemInfo[MAX_ITEM_GEM] ;	//物品里镶嵌的宝石
	
	

	VOID CleanUp()
	{
		memset(this,0,sizeof(EQUIP_INFO));
	};


	BOOL				CanUseSkill();
	WORD				GetUseSkillId();

	BOOL				CanRandSkill();
	WORD				GetRandSkillId();

};


#define MAX_ITEM_LIST_COUNT			8

struct _ITEM_LIST 
{
	INT				m_ListCount;
	_ITEM_TYPE		m_ListType[MAX_ITEM_LIST_COUNT];
	INT				m_TypeCount[MAX_ITEM_LIST_COUNT];

	_ITEM_LIST()
	{
		memset(this,0,sizeof(_ITEM_LIST));
	}
	
	VOID	AddType(_ITEM_TYPE it,INT Count)
	{
		Assert(Count>0);
		Assert(!it.isNull());
		
		BOOL bFindSame = FALSE;
		for(INT i=0;i<m_ListCount;i++)
		{
			if(m_ListType[i]==it)
			{
				m_TypeCount[i]+=Count;
				bFindSame = TRUE;
				break;
			}
		}

		if(!bFindSame)
		{
			m_TypeCount[m_ListCount] = Count;
			m_ListType[m_ListCount]  = it;	
			m_ListCount++;
		}
	}
	
	VOID Init()
	{
		memset(this,0,sizeof(_ITEM_LIST));
	}
};


//物品信息, 包含各种物品的所有数据
#define	ITEMISVALID(ITEM) (ITEM.m_ItemIndex != 0 )//物品是否合法
#define	ITEMPTRISVALID(PITEM) ( (PITEM == NULL) ? FALSE:PITEM->m_ItemIndex )//物品指针是否合法
#define	ITEMREFPTRISVALID(PITEM) ( (PITEM == NULL) ? FALSE:PITEM->GetItemTableIndex() )//物品指针是否合法

#define MAX_FIX_ATTR_LENGTH		100
#define MAX_PET_SKILL_LENGTH	50
#define MAX_ITEM_CREATOR_NAME	13
#define MAX_ITEM_PARAM			3
#define MAX_ITEM_STRING_LENGTH	255


enum ItemParamValue
{
	IPV_CHAR	=	0,
	IPV_SHORT	=	1,
	IPV_INT		=	2	
};

struct _ITEM
{
	_ITEM_GUID			m_ItemGUID ;				//物品固定信息（不能改变）
	UINT				m_ItemIndex;				//物品类型
													/*
													|	    1 ~ 10000		白色装备
													|	10001 ~ 20000		绿色装备	
													|	20001 ~ 30000		蓝色装备
													|	30001 ~ 35000		药瓶
													|	35001 ~ 40000		宝石
													|						...
													*/

	CHAR				m_RulerID;
	CHAR				m_nsBind;
	CHAR				m_Creator[MAX_ITEM_CREATOR_NAME];
	INT					m_Param[MAX_ITEM_PARAM];
	union 
	{
		EQUIP_INFO			m_Equip;
		GEM_INFO			m_Gem;
		MEDIC_INFO			m_Medic;
		STORE_MAP_INFO		m_StoreMap;
	};

	_ITEM( )
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset( this, 0, sizeof(*this) ) ;
	}
	
	BYTE		ItemClass( )	const
	{
			return GetSerialClass(m_ItemIndex);
	};

	BYTE		ItemType()		const
	{
		return GetSerialType(m_ItemIndex);
	}

	BYTE		GetQual()	const
	{
		return GetSerialQual(m_ItemIndex);
	}

	BYTE		GetIndex()	const
	{
		return GetSerialIndex(m_ItemIndex);
	}

	EQUIP_INFO*	   GetEquipData() const
	{
		return	(EQUIP_INFO*)(&m_Equip);
	}

	GEM_INFO*	  GetGemData()	const
	{
		return	(GEM_INFO*)(&m_Gem);
	}

	MEDIC_INFO*		GetMedicData()	const
	{
		return (MEDIC_INFO*)(&m_Medic);
	}

	STORE_MAP_INFO*	GetStoreMapData()	const
	{
		return (STORE_MAP_INFO*)(&m_StoreMap);
	}
	//////////////////////////////////////////////////////////////////////////
	// 物品个数方法
	// 能叠放物品		返回 实际大小
	// 不能叠放物品		返回 1
	//
	BYTE	GetItemCount()		const;

	VOID	SetItemCount(INT nCount);

	BYTE	GetItemTileMax()	const;
	
	BOOL	isFullTile()		const;


	//增加叠放数量
	BOOL	IncCount(UINT nCount =1);	
	//减少叠放数量
	BOOL	DecCount(UINT nCount =1);


	BOOL	IsNullType()	const
	{
		return m_ItemIndex == 0;
	}
	
	VOID	Read(SocketInputStream& iStream);
	VOID	Write(SocketOutputStream& oStream) const;
	
	BOOL	WriteFixAttr(CHAR* pIn,INT BuffLength);
	BOOL	ReadFixAttr(CHAR* pOut,INT& OutLength,INT BuffLength);

	BOOL	WriteVarAttr(CHAR* pIn,INT BuffLength);
	BOOL	ReadVarAttr (CHAR* pOut,INT& OutLength,INT BuffLength);

	BOOL	IsFixAttr(BYTE attrType);
	BYTE	GetFixAttrCount();
	
	BOOL	GetItemBind() const;
	VOID	SetItemBind(BOOL bBind);

	BOOL	GetItemIdent() const;
	VOID	SetItemIdent(BOOL bIdent);

	BOOL	GetItemPLock() const;	//密码锁定
	VOID	SetItemPLock(BOOL bIdent);
	
	INT		GetItemFailTimes() const;
	VOID	SetItemFailTimes(INT nTimes);
	//创建者
	BOOL	GetCreatorVar() const;
	VOID	SetCreatorVar(BOOL bCreator);

	INT		GetItemParamValue(UINT Start,const ItemParamValue& ipv);
	VOID	SetItemParam(UINT start,ItemParamValue& ipv,INT value);
};

//pItem			需要转换的_ITEM
//pOut			转换后输出的数据
//OutLength		输出数据的长度
//BuffLength	pOut 的实际长度
BOOL		Item2String(_ITEM* pItem,CHAR* pOut,INT& OutLength,INT BuffLength);
BOOL		String2Item(CHAR* pIn,INT BuffLength,_ITEM* pItem);


//A,B 交换
VOID	SwapItem(_ITEM* pItemA,_ITEM* pItemB);
//A 覆盖B 空间
//同时A 清空
VOID	OverWriteItem(_ITEM* pItemA,_ITEM* pItemB);

UINT	GetItemRealSize(const _ITEM& it);



struct _ITEM_EQUIP : public _ITEM
{


	_ITEM_EQUIP( )
	{
	};
};

struct _ITEM_EXPEND : public _ITEM
{

	_ITEM_EXPEND( )
	{
	};
};

struct _ITEM_MATERIAL : public _ITEM
{

	_ITEM_MATERIAL( )
	{
	};
};

struct _ITEM_GEM : public _ITEM
{

	_ITEM_GEM( )
	{
	};
};

struct _ITEM_CURIOSA : public _ITEM
{

	_ITEM_CURIOSA( )
	{
	};
};

struct _ITEM_TALISMAN : public _ITEM
{

	_ITEM_TALISMAN( )
	{
	};
};

struct _ITEM_GUILDITEM : public _ITEM
{

	_ITEM_GUILDITEM( )
	{
	};
};

struct _ITEM_TASKITEM : public _ITEM
{

	_ITEM_TASKITEM( )
	{
	};
};


struct  ItemBoxContaner
{

	ItemBoxContaner(){
		memset(m_Item,0,sizeof(_ITEM)*MAX_BOXITEM_NUMBER);
		memset(m_nQuality,0,sizeof(INT)*MAX_BOXITEM_NUMBER);
		m_nCount = 0;
	}

	VOID	AddItemType(_ITEM_TYPE& it,INT iQuality)
	{
		m_Item[m_nCount].m_ItemIndex = it.ToSerial();
		m_nQuality[m_nCount]	=	iQuality;
		m_nCount++;
	}
	_ITEM				m_Item[MAX_BOXITEM_NUMBER]; 
	INT					m_nQuality[MAX_BOXITEM_NUMBER];
	INT					m_nCount;
	UINT				m_uDropType;
};


#endif

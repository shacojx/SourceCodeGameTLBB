#pragma once
#include "TDNode.h"
#include "TDGfxSystem.h"
#include "TDCursorSystem.h"

//===================================================
//在编译时确定该类是否拥有逻辑函数
//#define TD_DECLARE_LOGICAL( bHaveLogic) \
//public: \
//	virtual inline BOOL HaveLogical(VOID) { return bHaveLogic; }

//========================================================
//物体基本状态	( OSF = Object Status Flags )
//          
#define OSF_NONE				(0x0)		//空状态
#define OSF_VISIABLE			(0x1)		//是否可见
#define OSF_OUT_VISUAL_FIELD	(0x2)		//已经不再玩家的视野范围,如果该值维持一定时间,则会被删除
#define OSF_RAY_QUERY			(0x4)		//鼠标能否选中

//===================================================
//游戏概念中的物体基类，所有衍生的Object都是可见的
struct _ITEM;
enum ITEM_CLASS;
class tObject : public tNode
{
public:
	//-----------------------------------------------------
	///根据初始化物体，并同步到渲染层
	virtual	VOID				Initial(VOID*)	= 0;
	///得到物体的ID
	virtual INT					GetID(VOID)				= 0;		//客户端ID
	virtual INT					GetServerID(VOID)const	= 0;		//与服务器同步的ID

	//-----------------------------------------------------
	///设置某项基本状态为Enable
	virtual VOID				Enable(UINT dwFlag) = 0;
	///设置某项基本状态为Disable
	virtual VOID				Disalbe(UINT dwFlag) = 0;
	///察看某项状态是否为Enable
	virtual	BOOL				IsEnable(UINT dwFlag) const = 0;
	///察看某项状态是否为Disable
	virtual	BOOL				IsDisable(UINT dwFlag) const = 0;
	//是否能够被作为主目标选择
	virtual BOOL				CanbeSelect(VOID) const = 0;

	//--------------------------------------------------------
	//渲染层指针
	virtual tEntityNode*		GetRenderInterface(VOID) = 0;

	//-----------------------------------------------------
	//针对与Tripper物体
public:
	enum TRIPPER_OBJECT_TYPE
	{
		TOT_NONE,			//非Tripper物体
		TOT_TRANSPORT,		//转送点
		TOT_ITEMBOX,		//掉落的箱子
		TOT_RESOURCE,		//生活技能中的矿物资源
		TOT_PLATFORM,		//生活技能中的合成所需要的平台
							//...
	};

	//物体类型
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	= 0;
	//能否鼠标操作
	virtual BOOL					Tripper_CanOperation(VOID) const = 0;
	//鼠标类型
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const = 0;
	//进入激活
	virtual VOID					Tripper_Active(VOID) = 0;

	WX_DECLARE_DYNAMIC(tObject);
	//声明该对象没有逻辑功能
	//TD_DECLARE_LOGICAL(FALSE);
};

//物品类物体接口
struct _ITEM;
class tObject_Item
{
public:
	//-----------------------------------------------------
	//从服务器传来的数据标示(用于和服务器通讯)
	union ITEM_GUID
	{
		//合并后的ID
		unsigned __int64	m_idUnion;
		
		//服务器传来的数据 ref GameStruct_Item.h
		struct ID_FROMSERVER
		{
			USHORT			m_idServer;			//世界号: (例)101
			USHORT			m_idWorld;			//服务端程序号：(例)5
			UINT			m_uSerial;			//物品序列号：(例)123429
		}m_idOrg;
	}m_ID;
	//-----------------------------------------------------
	//物品规则
	enum 
	{
		RULE_DROP = 0,	// 是否可丢弃
		RULE_OVERLAY,	// 是否可重叠
		RULE_QUICK,		// 是否可放入快捷栏
		RULE_SALE,		// 是否可以出售给NPC商店
		RULE_EXCHANGE,	// 是否可以交易
		RULE_USE,		// 是否可以使用
		RULE_PICKBIND,	// 是否是拾取邦定
		RULE_EQUIPBIND,	// 是否是装备邦定
		RULE_UNIQUE,	// 是否是唯一
	};

	//-----------------------------------------------------
	//物品归属
	enum ITEM_OWNER
	{
		IO_UNKNOWN,

		IO_MYSELF_EQUIP,		//玩家自己身上的装备
		IO_MYSELF_PACKET,		//玩家自己身上的包中
		IO_MYSELF_BANK,			//玩家自己身上的银行中
		IO_PLAYEROTHER_EQUIP,	//其他玩家身上的装备
		IO_ITEMBOX,				//打开的宝箱中
		IO_BOOTH,				//商人的货架
		IO_MYEXBOX,				//自己的交易盒
		IO_OTHEREXBOX,			//对方的交易盒
		//		IO_ENCHASEPAGE,			// 宝石合成/镶嵌界面
		IO_MISSIONBOX,			//任务递交盒
		IO_MYSTALLBOX,			//自己的摊位盒
		IO_OTSTALLBOX,			//对方的摊位盒
		//		IO_APPENDITEM,			//装备上附加的宝石
		IO_QUESTVIRTUALITEM,	// 任务奖励的（虚拟）物品，只用于显示。
		IO_PS_SELFBOX,			//
		IO_PS_OTHERBOX,			//
		IO_BOOTH_CALLBACK,		//Shop界面上的回购物品
		IO_GEM_SEPARATE,		//宝石摘除UI的宝石
		IO_TRANSFER_ITEM,		//物品信息传送
		IO_CITY_RESEARCH,		//城市研究显示
		IO_CITY_SHOP,			//城市商店显示
		IO_QUESTUI_DEMAND,		//Quest UI Demand
		IO_QUESTUI_REWARD,		//Quest UI Reward

	};

	//-----------------------------------------------------
	//物品类别
	// 1,3,5,6,7	放入基本包裹中
	// 2			放入材料包裹中
	// 4			放入任务包裹中
	//
	/*

		Common/GameDefine_Item.h

	enum ITEM_CLASS
	{
		IC_EQUIP		=1,	//武器WEAPON、防具DEFENCE、饰物ADORN
		IC_MATERIAL		=2,	//原料
		IC_COMITEM		=3,	//药品
		IC_TASKITEM		=4,	//任务物品
		IC_GEM			=5, //宝石
		IC_STOREMAP		=6,	//藏宝图
		IC_TALISMAN		=7,	//法宝
		IC_GUILDITEM	=8,	//帮会物品
	};

	*/
public:
	//客户端id
	virtual INT					GetID(VOID) const	= 0;
	//GUID
	virtual VOID				SetGUID(USHORT idWorld, USHORT idServer, UINT idSerial)			= 0;
	virtual VOID				GetGUID(USHORT& idWorld, USHORT& idServer, UINT& idSerail) const	= 0;
	virtual unsigned __int64	GetGUID(VOID) const = 0;

	//物品的类别
	virtual ITEM_CLASS		GetItemClass(VOID) const	= 0;
	//物品的名称
	virtual LPCTSTR			GetName(VOID) const			= 0;
	//物品解释
	virtual LPCTSTR			GetDesc(VOID) const			= 0;
	//图标名
	virtual LPCTSTR			GetIconName(VOID) const		= 0;
	//物品的详细编号，不是client用的index
	virtual INT				GetParticularID(VOID) const	= 0;
	//详细解释(可能需要服务器)
	virtual LPCTSTR			GetExtraDesc(VOID)			= 0;
	//设置详细解释
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo)= 0;
	//归属
	virtual VOID			SetTypeOwner(ITEM_OWNER owner)	= 0;
	virtual ITEM_OWNER		GetTypeOwner(VOID) const		= 0;

	//所在容器的索引
	virtual INT				GetPosIndex(VOID) const			= 0;
	virtual VOID			SetPosIndex(INT nPosIndex)		= 0;

	//物品数量
	virtual VOID			SetNumber(INT nNum)				= 0;
	virtual INT				GetNumber(VOID) const			= 0;

	//最大数量,暂时用于有限商品得显示
	virtual VOID			SetMax(INT nNum)				= 0;
	virtual INT				GetMax(VOID) const				= 0;

	//在资源表中的位置
	virtual INT				GetIdTable(VOID) const			= 0;
	//查询逻辑属性,一般用于脚本调用
	virtual STRING			GetAttributeValue(LPCTSTR szValueName) = 0;

	// 得到是否锁定(用于UI锁定)
	virtual BOOL			IsLocked()						= 0;
	virtual VOID			SetLock(BOOL bLock)				= 0;

	//物品规则验证
	virtual BOOL			Rule(INT ruletype)				= 0;
	virtual STRING			RuleFailMessage(INT ruletype)	= 0;

	//物品的二级密码保护
	virtual BOOL			GetByProtect()					= 0;
	virtual VOID			SetByProtect(BOOL bByProtect)	= 0;

	//获得物品的最大叠加数量
	virtual	INT				GetMaxOverLay(VOID) const		= 0;

	//--------------------------------------------------------------------------------------------------------------------------
	//-- super tool tip 属性
	//--

	// 得到玩家使用这个物品需要的等级
	virtual INT				GetNeedLevel()						   = 0;

	// 得到物品耐久度
	virtual INT				GetItemDur()						   = 0;

	// 得到物品最大耐久度
	virtual INT				GetItemMaxDur()						   = 0;

	// 得到物品的修理次数 
	virtual INT				GetItemRepairCount()				   = 0;

	// 得到物品的绑定信息 
	virtual INT				GetItemBindInfo()				       = 0;

	// 得到物品卖给npc的价格
	virtual INT				GetItemPrice()						   = 0;

	// 得到物品的制作人
	virtual LPCTSTR			GetManufacturer()				       = 0;

	// 得到白色属性
	virtual LPCTSTR			GetBaseWhiteAttrInfo()                 = 0;

	// 得到扩展蓝色属性
	virtual LPCTSTR			GetExtBlueAttrInfo()                   = 0;

	//得到物品使用的目标类型
	virtual INT				GetItemTargetType()					   = 0;

	// 得到物品的类型描述2006-4-28
	virtual LPCTSTR			GetItemTableTypeDesc()				   = 0;

	virtual INT				GetNeedJob(VOID) const				= 0;
};

//===================================================
//游戏概念中的物体管理器
class tObjectSystem : public tNode
{
public:

	enum DESTROY_MAIN_TARGET_TYPE
	{
		DEL_OBJECT = 0,		// 销毁对象
		DEL_RIGHT_CLICK,	// 鼠标右键销毁
	};

public:
	/// 产生新物体,并加入数据链
	virtual tObject*		NewObject(LPCTSTR szClass, INT idServer=-1, tObject* pParent=NULL) = 0;
	/// 销毁物体,并从数据链上拆除
	virtual BOOL			DestroyObject(tObject* pObject)					= 0;
	/// 根据ID得到某物体
	virtual tObject*		FindObject(INT id)							= 0;
	// 获取一个SERVER OBJ对象的指针
	virtual tObject*		FindServerObject( INT idServer )				= 0;
	// 获得物品
	virtual tObject_Item*	FindItem(INT id)								= 0;
	// 根据屏幕坐标计算选中得物体
	virtual tObject*		GetMouseOverObject(INT nX, INT nY, fVector3& fvMouseHitPlan )   = 0;	
	/// 设置主目标对象，如果id非法，取消选中物体
	virtual VOID			SetMainTarget(INT id,DESTROY_MAIN_TARGET_TYPE delType = DEL_OBJECT)	= 0;
	//取得当前选中物体
	virtual tObject*		GetMainTarget(VOID)	= 0;
	//关心某物体的指定事件 id-物体ID		bCare-关心或者取消关心
	virtual VOID			CareObject(INT id, BOOL bCare, std::string szSign) = 0;
	//从本地资源表中读取物品名字
	virtual LPCTSTR			ItemNameByTBIndex(UINT idTable) = 0;

protected:
	WX_DECLARE_DYNAMIC(tObjectSystem);
};

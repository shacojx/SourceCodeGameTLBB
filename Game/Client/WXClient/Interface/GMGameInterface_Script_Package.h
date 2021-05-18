/************************************\
*									*
*			玩家包裹接口			*
*									*
\************************************/

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}


//
//脚本函数沙箱，这些函数应该由脚本调用
//
namespace SCRIPT_SANDBOX
{

class PlayerPackage
{
public:

	//查询基本包裹中的物品
	//
	// 传入参数 
	//		0[String]	- 包裹类型
	//						"base"		基本物品
	//						"material"	材料物品
	//						"quest"		任务物品
	//
	//		1[Int]		- 包裹格数[1, 20]
	//
	// 传出参数
	//   0 - ActionItem
	//
	INT		EnumItem(LuaPlus::LuaState* state);
	// 打开摆摊（出售）界面
	INT		OpenStallSaleFrame(LuaPlus::LuaState* state);

	//--------------------------------------------------------
	// 拆分物品界面使用的接口函数
	INT		GetSplitSum(LuaPlus::LuaState* state);
	// 处理点击确定拆分后的处理
	INT		SplitItem(LuaPlus::LuaState* state);
	// 取消物品拆分
	INT		CancelSplitItem(LuaPlus::LuaState* state);
	//整理物品
	INT		PackUpPacket(LuaPlus::LuaState* state);
	//获得等待拆分物品的价格
	INT		GetSplitName(LuaPlus::LuaState* state);
	//打开宠物界面
	INT		OpenPetList(LuaPlus::LuaState* state);
	//打开锁定界面
	INT		OpenLockFrame(LuaPlus::LuaState* state);
	//锁定物品（宠物）
	INT		Lock(LuaPlus::LuaState* state);
	//获得物品是否锁定
	INT		IsLock(LuaPlus::LuaState* state);

public:
	static PlayerPackage s_PlayerPackage;
	static LuaPlus::LuaObject* s_pMetaTable;
};

};
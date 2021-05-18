/************************************\
*									*
*			系统设置接口			*
*									*
\************************************/

#include "../Global.h"
#include "GMInterface_Lua.h"

namespace SCRIPT_SANDBOX
{

//系统设置
class SystemSetup : public LuaExport< SystemSetup >
{
public:
	//---------------------------------------------------------
	//变量截获函数
	static VOID	WINAPI	_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData);

	//纹理相关的设定
	INT Lua_Texture(LuaPlus::LuaState* state);
	//显示质量相关的设定
	INT Lua_Display(LuaPlus::LuaState* state);
	//声音相关的设定
	INT Lua_Sound(LuaPlus::LuaState* state);
	//打开系统主界面
	INT Lua_OpenSetup(LuaPlus::LuaState* state);
	//打开视频设置界面
	INT Lua_ViewSetup(LuaPlus::LuaState* state);
	//打开声音设置界面
	INT Lua_SoundSetup(LuaPlus::LuaState* state);
	//打开UI设置界面
	INT Lua_UISetup(LuaPlus::LuaState* state);
	//打开按键设置界面
	INT Lua_InputSetup(LuaPlus::LuaState* state);
	//打开游戏性设置界面
	INT Lua_GameSetup(LuaPlus::LuaState* state);
	//游戏帮助
	INT Lua_GameHelp(LuaPlus::LuaState* state);
	//返回游戏
	INT Lua_BackGame(LuaPlus::LuaState* state);

	//-----------------------------------------------------------------------------
	//视频相关
	INT Lua_View_GetData(LuaPlus::LuaState* state);
	INT Lua_View_SetData(LuaPlus::LuaState* state);
	INT Lua_View_SaveToFile(LuaPlus::LuaState* state);
	//-----------------------------------------------------------------------------
	//游戏性相关
	INT Lua_GameGetData(LuaPlus::LuaState* state);
	INT Lua_SaveGameSetup(LuaPlus::LuaState* state);

	INT Set_GameData(INT nData);

	//-----------------------------------------------------------------------------
	//玩家信息

	//打开页面
	INT Lua_OpenPrivatePage(LuaPlus::LuaState* state);
	//请求玩家信息
	INT Lua_AskPrivateInfo(LuaPlus::LuaState* state);
	//提交玩家信息
	INT Lua_ApplyPrivateInfo(LuaPlus::LuaState* state);
	//获得玩家信息
	INT Lua_GetPrivateInfo(LuaPlus::LuaState* state);
	//获得玩家信息
	INT Lua_SetPrivateInfo(LuaPlus::LuaState* state);
	//从玩家信息界面切换到玩家装备界面
	INT Lua_OpenEquipFrame(LuaPlus::LuaState* state);
	//从玩家信息界面切换到玩家宠物界面
	INT Lua_OpenPetFrame(LuaPlus::LuaState* state);
	//获得玩家资料界面关心的Obj
	INT Lua_GetCaredObjId(LuaPlus::LuaState* state);

public:
	SystemSetup() : LuaExport< SystemSetup >("SystemSetup") {}

	//---------------------------------------------------------
	//设置屏幕分辨率
	static VOID	_setResolution(INT nWidth, INT nHeight);
	//设置窗口是否全屏
	static VOID	_setResolution(BOOL bFullScreen);
protected:
	//保存游戏性设置的数值（从Server传过来的）
	INT						m_GameData;
};

}

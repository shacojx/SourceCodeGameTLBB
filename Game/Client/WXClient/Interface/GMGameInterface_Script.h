#pragma once

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}
class CObject_Character;
namespace SCRIPT_SANDBOX
{

//登陆界面中，用于点击确定后的操作(-- for debug)
INT	Lua_ServerLogin(LuaPlus::LuaState* state);

//要求退出程序
INT Lua_QuitApplication(LuaPlus::LuaState* state);

//要求打开/关闭技能书
INT	Lua_ToggleSkillBook(LuaPlus::LuaState* state);
//要求打开技能书
INT Lua_OpenSkillBook(LuaPlus::LuaState* state);

//要求打开/关闭宠物界面
INT	Lua_TogglePetPage(LuaPlus::LuaState* state);
//要求打开/关闭自动打怪界面
INT Lua_ToAUTOHit(LuaPlus::LuaState* state);
//要求打开生活技能界面
INT Lua_OpenLifePage(LuaPlus::LuaState* state);

//要求打开普通技能界面
INT Lua_OpenCommonSkillPage(LuaPlus::LuaState* state);
//打开包裹
INT Lua_ToggleContainer(LuaPlus::LuaState* state);  //fujia 2007.10.24
INT Lua_ToggleContainer1(LuaPlus::LuaState* state);
INT Lua_ToggleContainer2(LuaPlus::LuaState* state);

//打开任务
INT Lua_ToggleMission(LuaPlus::LuaState* state);

//输出提示信息
INT Lua_PushDebugMessage(LuaPlus::LuaState* state);

//得到某类操作对象的数目
INT	Lua_GetActionNum(LuaPlus::LuaState* state);

//导出数据池内从服务器发送来的老师的门派信息
//INT Lua_GetMenpaiInfo(LuaPlus::LuaState* state);

//通过技能类表得到不同等级技能ID
INT GetStudySkillLevelId(int SkillId,int skillLevel);

//得到学习技能的金钱要求
INT Lua_GetStudySkillMoney(LuaPlus::LuaState* state);

//得到技能描述信息
INT Lua_GetStudySkillMS(LuaPlus::LuaState* state);

//得到学习技能的等级要求
INT Lua_GetStudySkillLevel(LuaPlus::LuaState* state);

//得到技能名称
INT Lua_GetStudySkillName(LuaPlus::LuaState* state);

//得到技能图标
INT Lua_GetSkillImage(LuaPlus::LuaState* state);

//导出心法升级需要的金钱
INT Lua_GetUplevelXinfaSpendMoney(LuaPlus::LuaState* state);

//导出心法升级需要的经验值
INT Lua_GetUplevelXinfaSpendExp(LuaPlus::LuaState* state);

//导出当前心法等级
//INT Lua_GetXinfaLevel(LuaPlus::LuaState* state);

INT Lua_GetCurrentTime( LuaPlus::LuaState* state);

//获得当前的鼠标位置（屏幕坐标）
INT Lua_GetCurMousePos( LuaPlus::LuaState* state);

//加入门派
//INT Lua_MenpaiFrameJoin(LuaPlus::LuaState* state);

//学习门派心法 
INT lua_SkillsStudyFrame_study(LuaPlus::LuaState* state);

//得到操作对象
INT	Lua_EnumAction(LuaPlus::LuaState* state);

//显示右键菜单
INT	Lua_ShowContexMenu(LuaPlus::LuaState* state);

//任务窗口选项按下
INT Lua_QuestFrameOptionClicked(LuaPlus::LuaState* state);

//窗口是否Show的状态
INT	Lua_IsWindowShow(LuaPlus::LuaState* state);

//开始合成
INT Lua_ComposeItem_Begin(LuaPlus::LuaState* state);

//取消合成
INT Lua_ComposeItem_Cancel(LuaPlus::LuaState* state);

//接受任务
INT Lua_QuestFrameAcceptClicked(LuaPlus::LuaState* state);

//谢绝任务
INT Lua_QuestFrameRefuseClicked(LuaPlus::LuaState* state);

//继续任务
INT Lua_QuestFrameMissionContinue(LuaPlus::LuaState* state);

//任务完成
INT Lua_QuestFrameMissionComplete(LuaPlus::LuaState* state);

//任务放弃
INT Lua_QuestFrameMissionAbnegate(LuaPlus::LuaState* state);

//要求打开/关闭合成界面
INT	Lua_ToggleComposeWnd(LuaPlus::LuaState* state);
//枚举所有的配方
INT	Lua_EnumMethod(LuaPlus::LuaState* state); 
// 得到某一种配方的数量
INT Lua_GetMethodNumber( LuaPlus::LuaState* state); 
//调试信息输出
INT Lua_AxTrace(LuaPlus::LuaState* state);

// 得到当前场景得尺寸
INT Lua_GetSceneSize( LuaPlus::LuaState* state); 

// 得到当前场景id
INT Lua_GetSceneID( LuaPlus::LuaState* state); 

// 得到当前小地图得尺寸
INT Lua_GetMinimapSize( LuaPlus::LuaState* state);

// 发送聊天信息
INT Lua_SendChatMessage( LuaPlus::LuaState* state);

INT Lua_GetCurChatProperty( LuaPlus::LuaState* state );
// 关闭打开大地图
INT Lua_ToggleLargeMap( LuaPlus::LuaState* state );
INT Lua_ToggleSceneMap( LuaPlus::LuaState* state );
INT Lua_OpenMinimap( LuaPlus::LuaState* state );
INT Lua_UpdateMinimap( LuaPlus::LuaState* state );
// 找北
INT Lua_ResetCamera( LuaPlus::LuaState* state );
//脚本里可以控制其他角色的功能函数

//打开关闭商人交易界面
INT Lua_OpenBooth( LuaPlus::LuaState* state );
INT Lua_CloseBooth( LuaPlus::LuaState* state);
INT Lua_RepairAll( LuaPlus::LuaState* state);
INT Lua_RepairOne( LuaPlus::LuaState* state);
INT Lua_GetCurrentSceneName( LuaPlus::LuaState* state );
INT Lua_GetCurrentSceneNameById( LuaPlus::LuaState* state );
INT Lua_GetOperationType( LuaPlus::LuaState* state );
INT Lua_GetShopType( LuaPlus::LuaState* state );


//打开关闭称号界面
INT Lua_OpenTitleList( LuaPlus::LuaState* state);
INT Lua_CloseTitleList( LuaPlus::LuaState* state); 

INT Lua_OpenCharacter( LuaPlus::LuaState* state );

INT Lua_OpenEquip( LuaPlus::LuaState* state );

INT Lua_GetActionInfo( LuaPlus::LuaState* state );

// 打开装备界面
INT Lua_OpenSelfEquip( LuaPlus::LuaState* state);

INT Lua_AutoHit(LuaPlus::LuaState* state);
//请求升级
INT Lua_AskLevelUp(LuaPlus::LuaState* state);
//发送gm指令
INT Lua_SendGMCommand(LuaPlus::LuaState* state);

INT Lua_SendAUTOMove(LuaPlus::LuaState* state);

INT Lua_ShowSupterTooltip( LuaPlus::LuaState* state);

INT Lua_Get_XParam_INT(LuaPlus::LuaState* state);

INT Lua_Get_XParam_STR(LuaPlus::LuaState* state);

INT Lua_Clear_XSCRIPT(LuaPlus::LuaState* state);

INT Lua_Send_XSCRIPT(LuaPlus::LuaState* state);

INT Lua_Set_XSCRIPT_Parameter(LuaPlus::LuaState* state);

INT Lua_Set_XSCRIPT_Function_Name(LuaPlus::LuaState* state);

INT Lua_Set_XSCRIPT_ScriptID(LuaPlus::LuaState* state);

INT Lua_Set_XSCRIPT_ParamCount(LuaPlus::LuaState* state);

//--------------------------------------------------------------------------
// 组队相关

INT Lua_ShowTeamFuncMenu(LuaPlus::LuaState* state);		// 队员或队长的功能菜单.
INT Lua_ShowTeamInfoDlg(LuaPlus::LuaState* state);		// 显示队伍信息对话框.
INT Lua_FlashTeamButton(LuaPlus::LuaState* state);		// 闪烁按钮.
INT Lua_SetCurSelMember(LuaPlus::LuaState* state);		// 设置当前选择的队员

// 组队相关
//--------------------------------------------------------------------------


// 设置一个全局变量
INT Lua_SetGlobalInteger(LuaPlus::LuaState* state);
// 获得一个全局变量
INT Lua_GetGlobalInteger(LuaPlus::LuaState* state);

// 要装备的详细信息
INT Lua_AskEquipDetial(LuaPlus::LuaState* state);

// 销毁物品
INT Lua_DiscardItem(LuaPlus::LuaState* state);

// 取消销毁
INT Lua_DiscardItemCancelLocked(LuaPlus::LuaState* state);

// 播放声音
INT Lua_PlaySound(LuaPlus::LuaState* state);

// 关闭所有窗口
INT	Lua_CloseAllWindow( LuaPlus::LuaState* state);

// target窗口弹出的菜单
INT	Lua_OpenTargetMenu( LuaPlus::LuaState* state);

INT Lua_GetMousePos( LuaPlus::LuaState* state);

// 显示系统提示对话框。2006－3－27
INT Lua_ShowSystemTipInfo( LuaPlus::LuaState* state);

// 打开更改二级密码对话框。2006－3－27
INT Lua_OpenChangeMinorPasswordDlg( LuaPlus::LuaState* state);

// 打开解锁密码。2006－3－27
INT Lua_OpenUnLockeMinorPasswordDlg( LuaPlus::LuaState* state);

// 打开设置密码界面。2006－3－28
INT Lua_OpenSetMinorPasswordDlg( LuaPlus::LuaState* state);

// 设置密码。2006－3－28
INT Lua_SendSetMinorPassword( LuaPlus::LuaState* state);

// 修改密码。2006－3－28
INT Lua_SendModifyMinorPassword( LuaPlus::LuaState* state);

// 解锁密码。2006－3－28
INT Lua_UnLockMinorPassword( LuaPlus::LuaState* state);

// 强制解锁密码。2006－3－28
INT Lua_ForceUnLockMinorPassword( LuaPlus::LuaState* state);

// 修改密码。2006－3－28
INT Lua_ModifyMinorPassword( LuaPlus::LuaState* state);


//// 播放音乐。参数是音乐的ID 2006－3－28
//INT Lua_PlayMusic( LuaPlus::LuaState* state);
//
//// 停止音乐。参数是音乐的句柄 2006－3－28
//INT Lua_StopMusic( LuaPlus::LuaState* state);//


class Character
{
public:
	
	
	//取得HP return (percent)
	INT		GetHPPercent(LuaPlus::LuaState* state);
	//取得MP return (percent)
	INT		GetMPPercent(LuaPlus::LuaState* state);
	//取得Rage return (percent)
	INT		GetRagePercent(LuaPlus::LuaState* state);

	INT		GetScenePos( LuaPlus::LuaState* state);

	
	//----------------------------------------------------------------------
	// 组队相关

	//请求组队
	INT	SendTeamRequest(LuaPlus::LuaState* state);

	//请求加入队伍
	INT	SendTeamApply(LuaPlus::LuaState* state);

	// 组队相关
	//----------------------------------------------------------------------

	// 跟随
	INT		Follow(LuaPlus::LuaState* state);
	
// 共有的数据
	// 得到角色的位置
	virtual INT		GetPos( LuaPlus::LuaState* state);
	//取得名字
	virtual INT		GetName(LuaPlus::LuaState* state);
	//对象是否存在 "target", "pet", ...
	virtual INT		IsPresent(LuaPlus::LuaState* state);
	// 得到角色身上的buff的数量
	virtual INT     GetBuffNumber( LuaPlus::LuaState* state );

	// 得到等级
	INT				GetLevel(LuaPlus::LuaState* state);
	INT				GetData(LuaPlus::LuaState* state);
	INT				SendAskDetail(LuaPlus::LuaState* state);
	INT				Set_To_Private(LuaPlus::LuaState* state);
	INT				Close_Before_TargetEquip_UI(LuaPlus::LuaState* state);
	INT				TargetEquip_ChangeModel(LuaPlus::LuaState* state);

	virtual INT		GetBuffIconNameByIndex( LuaPlus::LuaState* state );
	virtual INT		GetDialogNpcName( LuaPlus::LuaState* state );
	virtual INT		GetShopNpcName( LuaPlus::LuaState* state );
	//virtual INT		GetXinfaNpcName(LuaPlus::LuaState* state);


public:

	 // 当前的target 是否是队友2006-4-3
	 INT	IsTargetTeamMember(LuaPlus::LuaState* state);

	 // 当前的target 是否合法2006－4－12
	 INT	IsTargetValide(LuaPlus::LuaState* state);

	 // main target 窗口显示队友信息。2006－4－12

	// 显示名字
	INT TargetFrame_Update_Name_Team(LuaPlus::LuaState* state);

	// 显示hp
	INT TargetFrame_Update_HP_Team(LuaPlus::LuaState* state);

	// 显示mp
	INT	TargetFrame_Update_MP_Team(LuaPlus::LuaState* state);

	// 显示怒气
	INT TargetFrame_Update_Rage_Team(LuaPlus::LuaState* state);

	// 显示等级
	INT TargetFrame_Update_Level_Team(LuaPlus::LuaState* state);

	// 显示头像名字。
	INT TargetFrame_Update_Icon_Team(LuaPlus::LuaState* state);

	INT IsEnemy( LuaPlus::LuaState* state );			//是否是敌人

protected:
	Character() {};
	virtual ~Character() {};

private:
	CObject_Character*		_GetTargetAsCharacter(VOID);

public:
	//主选择对象
	static Character s_Target;
};

//脚本里可以控制玩家的功能函数
class PlayerMySelf : public Character
{
public:


	// 得到当前的称号.
	INT	GetCurTitle(LuaPlus::LuaState* state);

	// 不选择称号
	INT SetNullAgname(LuaPlus::LuaState* state);
	
	//----------------------------------------------------------------------------------------------------------------------------------------------
	// 组队相关
	
	// 离开队伍
	INT LeaveTeam(LuaPlus::LuaState* state);	

	// 解散队伍
	INT DismissTeam(LuaPlus::LuaState* state);	

	// 删除一个选中的队员
	INT KickTeamMember(LuaPlus::LuaState* state);
	
	// 删除当前选中队员
	INT KickTeamMemberByIndex(LuaPlus::LuaState* state);			

	//自建队伍
	INT	CreateTeamSelf(LuaPlus::LuaState* state);

	//任命队长消息
	INT	AppointLeader(LuaPlus::LuaState* state);

	// 是否处于组队跟随状态
	INT InTeamFollowMode(LuaPlus::LuaState* state);

	// 取消跟随
	INT StopFollow(LuaPlus::LuaState* state);

	// 组队跟随
	INT TeamFrame_AskTeamFollow(LuaPlus::LuaState* state);

	// 同意加入队伍(有人邀请我)
	INT	SendAgreeJoinTeam(LuaPlus::LuaState* state);
	// 拒绝加入队伍(有人邀请我)
	INT SendRejectJoinTeam(LuaPlus::LuaState* state);

	// 同意加入队伍(有人申请加入队伍)
	INT	SendAgreeJoinTeam_Apply(LuaPlus::LuaState* state);

	// 拒绝加入队伍(有人申请加入队伍)
	INT SendRejectJoinTeam_Apply(LuaPlus::LuaState* state);

	// 同意加入队伍(队员邀请某人加入队伍)
	INT	SendAgreeJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state);

	// 拒绝加入队伍(队员邀请某人加入队伍)
	INT SendRejectJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state);

	// 同意进入组队跟随模式
	INT SendAgreeTeamFollow(LuaPlus::LuaState* state);

	// 拒绝进入组队跟随模式
	INT SendRefuseTeamFollow(LuaPlus::LuaState* state);

	// 得到队员guid
	INT GetTeamMemberGUID(LuaPlus::LuaState* state);

	// 组队相关
	//-----------------------------------------------------------------------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------------------------------------------------------------------
	// 装备属性界面
	//

	// 发送手动调整属性点数 
	INT SendAskManualAttr(LuaPlus::LuaState* state);


	//
	// 装备属性界面
	//-----------------------------------------------------------------------------------------------------------------------------------------------



	virtual INT		GetName(LuaPlus::LuaState* state);
	virtual INT		GetData( LuaPlus::LuaState* state );
	// 得到角色的某项生活技能的信息
	virtual INT		GetAbilityInfo( LuaPlus::LuaState* state );
	// 得到角色的某项战斗技能的信息
	virtual INT		GetSkillInfo( LuaPlus::LuaState* state );
	// 得到角色的某项心法的信息
	virtual INT		GetSkillClassInfo( LuaPlus::LuaState* state );
	// 请求学习生活技能
	virtual INT		AskLeanAbility(LuaPlus::LuaState* state);

	// 得到角色的位置
	virtual INT		GetPos( LuaPlus::LuaState* state);
	// 角色是否存在
	virtual INT		IsPresent(LuaPlus::LuaState* state);

	//称号相关
	virtual INT		GetAgnameNum(LuaPlus::LuaState* state);
	virtual INT		EnumAgname(LuaPlus::LuaState* state);
	virtual INT		GetCurrentAgname(LuaPlus::LuaState* state);
	virtual INT		AskChangeCurrentAgname(LuaPlus::LuaState* state);


	
	// 得到角色身上的buff的数量
	virtual INT     GetBuffNumber( LuaPlus::LuaState* state );
	virtual INT		GetBuffIconNameByIndex( LuaPlus::LuaState* state );
	virtual INT		GetBuffToolTipsByIndex( LuaPlus::LuaState* state );
	virtual INT		DispelBuffByIndex( LuaPlus::LuaState* state );
	virtual INT		GetBuffTimeTextByIndex( LuaPlus::LuaState* state );

	// 复活对话框相关
	virtual INT		SendReliveMessage_OutGhost( LuaPlus::LuaState* state );
	virtual INT		SendReliveMessage_Relive( LuaPlus::LuaState* state );

	// 右键菜单相关
	virtual INT		ShowMySelfContexMenu(LuaPlus::LuaState* state);
	// 左键选中自己
	virtual INT		SelectMyselfAsTarget(LuaPlus::LuaState* state);

	// 设置二级密码 2006-4-29
	INT	SetSupperPassword(LuaPlus::LuaState* state);

protected:
	PlayerMySelf() {};
	virtual ~PlayerMySelf() {};

public:
	static PlayerMySelf s_MySelf;
};

//数据池
class DataPool
{
public:
	//取得当前对话npc数据
	INT		GetNPCEventList_Num(LuaPlus::LuaState* state);
	INT		GetNPCEventList_Item(LuaPlus::LuaState* state);

	//-----------------------------------------------------------
	//任务信息
	INT		GetMissionInfo_Num(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Text(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Bonus(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Kind(LuaPlus::LuaState* state);

	//任务需求信息
	INT		GetMissionDemand_Num(LuaPlus::LuaState* state);
	INT		CloseMissionFrame(LuaPlus::LuaState* state);
	INT		GetMissionDemand_Text(LuaPlus::LuaState* state);
	INT		GetMissionDemand_Item(LuaPlus::LuaState* state);
	//Continue
	INT		GetMissionContinue_Num(LuaPlus::LuaState* state);
	INT		GetMissionContinue_Text(LuaPlus::LuaState* state);
	INT		GetMissionContinue_Bonus(LuaPlus::LuaState* state);

	//玩家当前任务信息
	INT		GetPlayerMission_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMission_InUse(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Description(LuaPlus::LuaState* state);
	INT		GetPlayerMissionDemand_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionDemand_Item(LuaPlus::LuaState* state);
	INT		GetPlayerMissionCustom_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionCustom(LuaPlus::LuaState* state);
	INT		GetPlayerMissionBonus_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionBonus_Item(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Memo(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Name(LuaPlus::LuaState* state);
	INT		GetPlayerMission_BillName(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Info(LuaPlus::LuaState* state);

	INT		GetPlayerMission_ItemCountNow(LuaPlus::LuaState* state);

	INT		GetPlayerMission_Variable(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Display(LuaPlus::LuaState* state);
	INT		GetPlayerMission_RemainTime(LuaPlus::LuaState* state);
	INT		Mission_Abnegate_Popup(LuaPlus::LuaState* state);
	INT		Mission_Abnegate_Special_Quest_Popup(LuaPlus::LuaState* state);
	INT		EnumPlayerMission_ItemAction(LuaPlus::LuaState* state);
	INT		GetPlayerMission_DataRound(LuaPlus::LuaState* state);

	INT		GetPlayerMissionDemandKill_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionDemand_NPC(LuaPlus::LuaState* state);
/*	INT		GetPlayerMissionBonus_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionBonus_Item(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Memo(LuaPlus::LuaState* state);
*/	INT		GetPlayerMission_Level(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Kind(LuaPlus::LuaState* state);
	INT		GetPlayerMission_IsElite(LuaPlus::LuaState* state);


	//取得配方数据
	INT		GetPrescrList_Num(LuaPlus::LuaState* state);
	INT		GetPrescrList_Item(LuaPlus::LuaState* state);
	INT		GetPrescrList_Item_LifeAbility(LuaPlus::LuaState* state);
	INT		GetPrescrList_Item_Result(LuaPlus::LuaState* state);
	INT		GetPrescrList_Item_Requirement(LuaPlus::LuaState* state);

	// 获得货架的信息 
	INT		GetBoothList_Num(LuaPlus::LuaState* state); // 得到有多少个货物
	INT		GetBoothGood_ID( LuaPlus::LuaState* state ); // 得到货物的id 
	INT		GetBoothGood_Name(LuaPlus::LuaState* state); // 得到货物的名字
	INT		GetBoothGood_Price(LuaPlus::LuaState* state);// 得到货物的价格
	

	//-----------------------------------------------------------------------------------------------------------------------------------
	// 组队相关. 
	
	//------------------------------
	//这些函数供左边的头像界面显示.
	INT		GetTeamMemGUIDByUIIndex(LuaPlus::LuaState* state);			// 得到队员的信息
	INT		GetTeamMemInfo(LuaPlus::LuaState* state);			// 得到队员的信息
	INT		GetTeamMemCount(LuaPlus::LuaState* state);			// 得到队员的个数
	INT	    IsTeamLeader(LuaPlus::LuaState* state);				// 是否是队长
	INT		SelectAsTargetByUIIndex(LuaPlus::LuaState* state);	// 通过队友头像, 选中队友作为target



	//------------------------------
	// 打开邀请界面.

	//这些函数供组队信息窗口使用.  邀请接口.   
	INT		GetInviteTeamCount(LuaPlus::LuaState* state);		// 得到邀请队伍的个数.
	INT     GetInviteTeamMemberInfo(LuaPlus::LuaState* state);	// 得到队伍的信息.	
	INT		GetInviteTeamMemberCount(LuaPlus::LuaState* state);	// 得到某个邀请队伍的队员的个数.


	
	//-----------------------------
	// 打开队伍信息.

	
	// 由于队伍中的自己的信息, 不通过队伍信息返回, 每次打开界面的时候要刷新自己的信息
	INT     SetSelfInfo(LuaPlus::LuaState* state);

	// 队长打开队伍信息时, 选中的人物
	INT     SetSelectTeamMember_Leader(LuaPlus::LuaState* state);

	// 通过索引得到队员信息, 供队员打开界面使用.
	INT	GetTeamMemInfoByIndex(LuaPlus::LuaState* state);	// 得到队员的信息

	// 得到队员的ui模型名字
	INT	GetTeamMemUIModelNameByIndex(LuaPlus::LuaState* state);

	// 得到申请者的ui模型名字
	INT GetApplyMemberUIModelName(LuaPlus::LuaState* state);		

	// 得到队伍ui模型名字
	INT GetInviteTeamMemberUIModelName(LuaPlus::LuaState* state);	

	// 得到队员ui模型名字
	INT	GetTeamMemUIModelName(LuaPlus::LuaState* state);		

	// 队员与自己是否在同一个场景中
	INT	IsTeamMemberInScene(LuaPlus::LuaState* state);		

    


	//------------------------------
    // 打开申请人界面

	//这些函数供组队信息窗口使用.  申请接口. 
	INT		GetApplyMemberCount(LuaPlus::LuaState* state);		// 得到申请者个数.
	INT     GetApplyMemberInfo(LuaPlus::LuaState* state);		// 得到申请者的个数.		
	INT     SetCurSelApply(LuaPlus::LuaState* state);			// 设置当前选择的申请者.
	INT     EraseApply(LuaPlus::LuaState* state);				// 删除一个申请者
	INT     ClearAllApply(LuaPlus::LuaState* state);			// 清空所有的申请者.



	//------------------------------
	// 公共数据接口.

	// 设置组队对话框打开标记.
	INT		SetTeamFrameOpenFlag(LuaPlus::LuaState* state);		// 设置队伍对话框打开标记


	//--------------------------------------------------------------------------------------------------------------------------------------



	//包裹数据
	INT		GetPacketItem( LuaPlus::LuaState* state );
	INT		GetPacketItemMaxNumber( LuaPlus::LuaState* state );

	// 好友系统接口

	INT		GetFriendNumber( LuaPlus::LuaState* state );	// 得到某一组的好友数量
	INT		GetFriend( LuaPlus::LuaState* state );			//得到某一组的好友的具体数据

	INT		AddFriend( LuaPlus::LuaState* state );			// 添加一个好友到某一组
	INT		DelFriend( LuaPlus::LuaState* state );			// 删除一个好友

	INT     ThrowToBlackList( LuaPlus::LuaState* state );
	INT     ThrowToList( LuaPlus::LuaState* state );

	INT		EditMood( LuaPlus::LuaState* state );
	INT		ShowFriendInfo( LuaPlus::LuaState* state );
	INT		ShowChatInfo(LuaPlus::LuaState* state);
	INT		SetMood( LuaPlus::LuaState* state );
	INT		GetMood( LuaPlus::LuaState* state );
	INT		OpenFriendList(LuaPlus::LuaState* state);

	INT		GetMailNumber( LuaPlus::LuaState* state );
	INT		GetMail( LuaPlus::LuaState* state );

	INT 	OpenMail(LuaPlus::LuaState* state);

	INT		SendMail( LuaPlus::LuaState* state );

	INT		GetNextMail(LuaPlus::LuaState* state );

	INT     GetFriendByName( LuaPlus::LuaState* state );

	INT		OpenHistroy( LuaPlus::LuaState* state );
//改变发型发色
	INT		Change_MyHairColor( LuaPlus::LuaState* state );
	INT		Change_MyHairStyle( LuaPlus::LuaState* state );
	INT		Change_MyHairStyle_Item( LuaPlus::LuaState* state );
	INT		Get_MyHairColor( LuaPlus::LuaState* state );
	INT		Get_MyHairStyle( LuaPlus::LuaState* state );
//检查String是否合法
	INT		Check_StringCode( LuaPlus::LuaState* state );

public:

	INT		IsCanDoAction( LuaPlus::LuaState* state );
	INT		Change_RectifyColor( LuaPlus::LuaState* state );
	INT		Change_GetColorLumination(LuaPlus::LuaState* state);

public:

	// 显示名字
	INT TargetFrame_Update_Name_Team(LuaPlus::LuaState* state);

	// 显示hp
	INT TargetFrame_Update_HP_Team(LuaPlus::LuaState* state);

	// 显示mp
	INT	TargetFrame_Update_MP_Team(LuaPlus::LuaState* state);

	// 显示怒气
	INT TargetFrame_Update_Rage_Team(LuaPlus::LuaState* state);

	// 显示等级
	INT TargetFrame_Update_Level_Team(LuaPlus::LuaState* state);

	// 显示头像名字。
	INT TargetFrame_Update_Icon_Team(LuaPlus::LuaState* state);

	INT	ToggleShowPetList(LuaPlus::LuaState* state);

	INT	GetNPCIDByServerID(LuaPlus::LuaState* state);

	INT GetUIColor(LuaPlus::LuaState* state);
	INT GetPlayerSkill_ID(LuaPlus::LuaState* state);
	INT GetPlayerSkill_Level(LuaPlus::LuaState* state);
	INT GetNPCobj_ID(LuaPlus::LuaState* state);
	INT GetPlayerSkill_Type(LuaPlus::LuaState* state);
	INT GetPlayerSkill_Num(LuaPlus::LuaState* state);
	// 设置二级密码
	INT	SetSupperPass(LuaPlus::LuaState* state);

public:
	static DataPool s_DataPool;
	static LuaPlus::LuaObject* s_pMetaTable;
};

//当前正在对话的生活技能老师
class AbilityTeacher
{
public:
	//取得当前老师是否存在
	INT		IsPresent(LuaPlus::LuaState* state);
	//得到当前老师所能教的生活技能ID
	INT		GetAbilityID(LuaPlus::LuaState* state);
	//得到升级到下一级所需要的经验值
	INT		GetNeedExp(LuaPlus::LuaState* state);
	//得到升级到下一级所需要的金钱
	INT		GetNeedMoney(LuaPlus::LuaState* state);
	//得到升级到下一级所需要的熟练度
	INT		GetNeedSkillExp(LuaPlus::LuaState* state);
	// 获得服务器升级的一些数据
	INT		GetServerData(LuaPlus::LuaState* state);

public:
	static AbilityTeacher s_AbilityTeacher;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class Pet
{
public:
	//取得当前宠物是否存在
	INT		IsPresent(LuaPlus::LuaState* state);
	INT		GetPet_Count(LuaPlus::LuaState* state);
	INT		GetPetList_Appoint(LuaPlus::LuaState* state);
	INT		Go_Fight(LuaPlus::LuaState* state);
	INT		Go_Relax(LuaPlus::LuaState* state);
	INT		Go_Free(LuaPlus::LuaState* state);
	INT		Feed(LuaPlus::LuaState* state);
	INT		Dome(LuaPlus::LuaState* state);

	INT		GetName(LuaPlus::LuaState* state);
	INT		GetID(LuaPlus::LuaState* state);
	INT		GetConsort(LuaPlus::LuaState* state);
	INT		GetSex(LuaPlus::LuaState* state);
	INT		GetNaturalLife(LuaPlus::LuaState* state);
	INT		GetMaxLife(LuaPlus::LuaState* state);
	INT		GetLoyalgGade(LuaPlus::LuaState* state);
	INT		GetLevel(LuaPlus::LuaState* state);
	INT		GetType(LuaPlus::LuaState* state);
	INT		GetHappy(LuaPlus::LuaState* state);
	INT		GetStrAptitude(LuaPlus::LuaState* state);
	INT		GetPFAptitude(LuaPlus::LuaState* state);
	INT		GetDexAptitude(LuaPlus::LuaState* state);
	INT		GetIntAptitude(LuaPlus::LuaState* state);
	INT		GetStaAptitude(LuaPlus::LuaState* state);
	
	INT		GetHP(LuaPlus::LuaState* state);
	INT		GetMaxHP(LuaPlus::LuaState* state);
	INT		GetMP(LuaPlus::LuaState* state);
	INT		GetMaxMP(LuaPlus::LuaState* state);

	INT		GetStr(LuaPlus::LuaState* state);
	INT		GetInt(LuaPlus::LuaState* state);
	INT		GetDex(LuaPlus::LuaState* state);
	INT		GetPF(LuaPlus::LuaState* state);
	INT		GetSta(LuaPlus::LuaState* state);

	INT		GetBasic(LuaPlus::LuaState* state);
	INT		GetPotential(LuaPlus::LuaState* state);
	INT		GetExp(LuaPlus::LuaState* state);

	INT		GetPhysicsAttack(LuaPlus::LuaState* state);
	INT		GetMagicAttack(LuaPlus::LuaState* state);
	INT		GetPhysicsRecovery(LuaPlus::LuaState* state);
	INT		GetMagicRecovery(LuaPlus::LuaState* state);

	INT		GetMiss(LuaPlus::LuaState* state);
	INT		GetShootProbability(LuaPlus::LuaState* state);

	INT		Add_Attribute(LuaPlus::LuaState* state);
	INT		Change_Name(LuaPlus::LuaState* state);

	INT		GetIsFighting(LuaPlus::LuaState* state);
	INT		SetModel(LuaPlus::LuaState* state);
	INT		GetDeathWarrant(LuaPlus::LuaState* state);
	
	//yangjun add for petskillstudy
	INT		SetSkillStudyModel(LuaPlus::LuaState* state);
	INT		ShowTargetPet(LuaPlus::LuaState* state);
	INT		SkillStudy_Do(LuaPlus::LuaState* state);
	INT		SkillStudyUnlock(LuaPlus::LuaState* state);
	INT		SkillStudy_MenPaiSkill_Created(LuaPlus::LuaState* state);
	INT		SkillStudy_MenPaiSkill_Clear(LuaPlus::LuaState* state);
	INT		GetGUID(LuaPlus::LuaState* state);
	INT		ShowPetList(LuaPlus::LuaState* state);

	//yangjun add for petcontexmenu
	INT		ShowMyPetContexMenu(LuaPlus::LuaState* state);
	INT		HandlePetMenuItem(LuaPlus::LuaState* state);

	//yangjun add for petImpact
	INT		GetPetPortraitByIndex(LuaPlus::LuaState* state);
	INT		GetPetImpactNum(LuaPlus::LuaState* state);
	INT		GetPetImpactIconNameByIndex(LuaPlus::LuaState* state);

	INT		Lua_EnumPetSkill(LuaPlus::LuaState* state);
	INT		Select_Pet(LuaPlus::LuaState* state);
	INT		GetAIType(LuaPlus::LuaState* state);
	INT		GetSkillPassive(LuaPlus::LuaState* state);
	INT		Free_Confirm(LuaPlus::LuaState* state);

public:
	static Pet s_Pet;
	static LuaPlus::LuaObject* s_pMetaTable;
};


class TargetPet
{
public:
	//取得当前宠物是否存在
	INT		IsPresent(LuaPlus::LuaState* state);
//	INT		GetPet_Count(LuaPlus::LuaState* state);
//	INT		GetPetList_Appoint(LuaPlus::LuaState* state);
//	INT		Go_Fight(LuaPlus::LuaState* state);
//	INT		Go_Relax(LuaPlus::LuaState* state);

	INT		GetName(LuaPlus::LuaState* state);
	INT		GetID(LuaPlus::LuaState* state);
	INT		GetConsort(LuaPlus::LuaState* state);
	INT		GetSex(LuaPlus::LuaState* state);
	INT		GetNaturalLife(LuaPlus::LuaState* state);
	INT		GetMaxLife(LuaPlus::LuaState* state);
	INT		GetLoyalgGade(LuaPlus::LuaState* state);
	INT		GetLevel(LuaPlus::LuaState* state);
	INT		GetType(LuaPlus::LuaState* state);
	INT		GetHappy(LuaPlus::LuaState* state);
	INT		GetStrAptitude(LuaPlus::LuaState* state);
	INT		GetPFAptitude(LuaPlus::LuaState* state);
	INT		GetDexAptitude(LuaPlus::LuaState* state);
	INT		GetIntAptitude(LuaPlus::LuaState* state);
	INT		GetStaAptitude(LuaPlus::LuaState* state);
	
	INT		GetHP(LuaPlus::LuaState* state);
	INT		GetMaxHP(LuaPlus::LuaState* state);
	INT		GetMP(LuaPlus::LuaState* state);
	INT		GetMaxMP(LuaPlus::LuaState* state);

	INT		GetStr(LuaPlus::LuaState* state);
	INT		GetInt(LuaPlus::LuaState* state);
	INT		GetDex(LuaPlus::LuaState* state);
	INT		GetPF(LuaPlus::LuaState* state);
	INT		GetSta(LuaPlus::LuaState* state);

	INT		GetBasic(LuaPlus::LuaState* state);
	INT		GetPotential(LuaPlus::LuaState* state);
	INT		GetExp(LuaPlus::LuaState* state);

	INT		GetPhysicsAttack(LuaPlus::LuaState* state);
	INT		GetMagicAttack(LuaPlus::LuaState* state);
	INT		GetPhysicsRecovery(LuaPlus::LuaState* state);
	INT		GetMagicRecovery(LuaPlus::LuaState* state);

	INT		GetMiss(LuaPlus::LuaState* state);
	INT		GetShootProbability(LuaPlus::LuaState* state);

//	INT		Add_Attribute(LuaPlus::LuaState* state);
//	INT		Change_Name(LuaPlus::LuaState* state);

	INT		GetIsFighting(LuaPlus::LuaState* state);
	INT		SetModel(LuaPlus::LuaState* state);
	INT		CopyMyPet(LuaPlus::LuaState* state);
	INT		GetDeathWarrant(LuaPlus::LuaState* state);
	INT		GetAIType(LuaPlus::LuaState* state);

public:
	static TargetPet s_TargetPet;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class Bank
{
public:
	//的到银行是否存在
	INT IsPresent(LuaPlus::LuaState* state);
	//打开存钱界面（输入存钱数量的界面）
	INT OpenSaveFrame(LuaPlus::LuaState* state);
	//打开取钱界面（输入取钱数量的界面）
	INT OpenGetFrame(LuaPlus::LuaState* state);
	//金钱的转换
	INT GetInputMoney(LuaPlus::LuaState* state);
	//发送存钱消息
	INT SaveMoneyToBank(LuaPlus::LuaState* state);
	//发送取钱消息
	INT GetMoneyFromBank(LuaPlus::LuaState* state);
	//导出银行的金钱数
	INT GetBankMoney(LuaPlus::LuaState* state);
	//导出银行的元宝数
	INT GetBankRMB(LuaPlus::LuaState* state);
	//发送存元宝消息
	INT SaveRMBToBank(LuaPlus::LuaState* state);
	//发送取元宝消息
	INT GetRMBFromBank(LuaPlus::LuaState* state);
	//发送一个移动银行物品到背包的消息
	INT MoveItemToPacket(LuaPlus::LuaState* state);
	//导出现在拥有的租赁箱的个数
	INT GetRentBoxNum(LuaPlus::LuaState* state);
	//导出指定租赁箱的开始数和格子数
	INT GetRentBoxInfo(LuaPlus::LuaState* state);
	//设置当前的租赁箱
	INT SetCurRentIndex(LuaPlus::LuaState* state);
	//转换货币
	INT TransformCoin(LuaPlus::LuaState* state);
	//关闭银行
	INT Close(LuaPlus::LuaState* state);
	//获得操作物品
	INT EnumItem(LuaPlus::LuaState* state);
public:
	static Bank s_Bank;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class Exchange
{
public:
	// 的到玩家是否存在
	INT IsPresent(LuaPlus::LuaState* state);
	// 申请交易
	INT SendExchangeApply(LuaPlus::LuaState* state);
	// 打开交易对话框
	INT OpenExchangeFrame(LuaPlus::LuaState* state);
	// 导出自己放入交易的物品个数
	INT GetItemNum(LuaPlus::LuaState* state);
	// 导出Action
	INT GetItemAction(LuaPlus::LuaState* state);
	// 导出金钱数
	INT GetMoney(LuaPlus::LuaState* state);
	// 锁定交易
	INT LockExchange(LuaPlus::LuaState* state);
	//导出锁定状态
	INT IsLocked(LuaPlus::LuaState* state);
	// 确定交易
	INT AcceptExchange(LuaPlus::LuaState* state);
	// 交易取消
	INT ExchangeCancel(LuaPlus::LuaState* state);
	// 打开宠物界面（暂时用来打开金钱输入框）
	INT OpenPetFrame(LuaPlus::LuaState* state);
	// 从输入框获得金钱
	INT GetMoneyFromInput(LuaPlus::LuaState* state);
	//导出交易请求列表，是否为空
	INT IsStillAnyAppInList(LuaPlus::LuaState* state);
	//导出以自己交易的人的名字
	INT GetOthersName(LuaPlus::LuaState* state);
	//选择一个宠物列表中的宠物，点击确定后的操作
	INT AddPet(LuaPlus::LuaState* state);
	// 获得宠物的个数
	INT GetPetNum(LuaPlus::LuaState* state);
	//
	INT EnumPet(LuaPlus::LuaState* state);
	//从交易栏上拿下一个宠物
	INT DelSelectPet(LuaPlus::LuaState* state);
	//显示宠物详细信息
	INT ViewPetDesc(LuaPlus::LuaState* state);

public:
	static Exchange s_Exchange;
	static LuaPlus::LuaObject* s_pMetaTable;
};


class LifeAbility
{
public:
	// 得到生活技能的数量
//	INT GetLifeAbility_Count(LuaPlus::LuaState* state);
	// 通过编号去查Ability的编号
	INT GetLifeAbility_Number(LuaPlus::LuaState* state);
	// 通过编号去查该配方，共需要几个材料
	INT GetPrescr_Material_Number(LuaPlus::LuaState* state);
	// 通过材料的编号，查询材料的详细信息，返回图标和名称。
	INT	GetPrescr_Material(LuaPlus::LuaState* state);
	// 通过序号去查第几个配方的编号
	INT GetPrescrList_Item_FromNum(LuaPlus::LuaState* state);
	// 计算身上原料总共可以合成多少个这个配方。
	INT GetPrescr_Item_Maximum(LuaPlus::LuaState* state);
	// 计算原料数量。
	INT GetPrescr_Material_Hold_Count(LuaPlus::LuaState* state);
	// 刷新合成界面
	INT Update_Synthesize( LuaPlus::LuaState* state);
	// 打开宝石合成/镶嵌界面
	INT Open_Compose_Gem_Page( LuaPlus::LuaState* state);
	// 开始镶嵌
	INT	Do_Enchase( LuaPlus::LuaState* state);
	// 开始合成
	INT	Do_Combine( LuaPlus::LuaState* state);
	// 返回装备上，第i颗宝石的图标信息
	INT	GetEquip_Gem( LuaPlus::LuaState* state);
	// 返回装备上，总共有多少颗宝石。
	INT GetEquip_GemCount( LuaPlus::LuaState* state);
	// 返回装备上，总共可以镶嵌多少个宝石	
//	INT GetEquip_GemCount( LuaPlus::LuaState* state);
	// 返回宝石的级别。
//	INT Compound_Preparation( LuaPlus::LuaState* state );
	// 是否可以镶嵌
	INT	Can_Enchase( LuaPlus::LuaState* state );
	// 是否可以合成
	INT Can_Combine( LuaPlus::LuaState* state );
	// 物品可以放到镶嵌界面这个位置。
	INT Enchase_Preparation( LuaPlus::LuaState* state );
	// 物品可以放到合成界面这个位置。
	INT Compound_Preparation( LuaPlus::LuaState* state );
	// 把物品栏某位置的物品置灰。
	INT Lock_Packet_Item( LuaPlus::LuaState* state );
	// 取得配方的详细描述
	INT	GetPrescr_Explain(LuaPlus::LuaState* state);
	// 生活技能界面关心的Npc
	INT GetNpcId( LuaPlus::LuaState* state );

public:
	static LifeAbility s_LifeAbility;
	static LuaPlus::LuaObject* s_pMetaTable;
};

//有可能将买和买分开来处理，
//class Stall

class StallSale
{
public:
	//的到玩家是否存在
	INT IsPresent(LuaPlus::LuaState* state);
	//导出本地摊位费
	INT GetPosTax(LuaPlus::LuaState* state);
	//导出本地税率
	INT GetTradeTax(LuaPlus::LuaState* state);
	//提交商品价格
	INT ReferItemPrice(LuaPlus::LuaState* state);
	// 更改摊位名字
	INT ModifStallName(LuaPlus::LuaState* state);
	// 更改Item价格
	INT ModifItemPrice(LuaPlus::LuaState* state);
	// 收摊走人
	INT CloseStall(LuaPlus::LuaState* state);
	// 商品的重新定价
	INT ItemReprice(LuaPlus::LuaState* state);
	// 发送开始摆摊的消息开始摆摊
	INT AgreeBeginStall(LuaPlus::LuaState* state);
	//删除货架上的一个物品（下架）
	INT DeleteItem(LuaPlus::LuaState* state);
	//获得指定位置的定价
	INT GetPrice(LuaPlus::LuaState* state);
	//打开摊位信息
	INT OpenMessageSale(LuaPlus::LuaState* state);
	//更改摊位介绍（广告语）
	INT ApplyAd(LuaPlus::LuaState* state);
	//关闭
	INT CloseStallMessage(LuaPlus::LuaState* state);
	//获得摊位名
	INT GetStallName(LuaPlus::LuaState* state);
	//获得广告语
	INT GetAdvertise(LuaPlus::LuaState* state);
	//导出自己的GUID
	INT GetGuid(LuaPlus::LuaState* state);
	// 获得宠物的个数
	INT GetPetNum(LuaPlus::LuaState* state);
	// 获得宠物
	INT EnumPet(LuaPlus::LuaState* state);
	// 宠物上架
	INT PetUpStall(LuaPlus::LuaState* state);
	// 宠物更改价格
	INT PetReprice(LuaPlus::LuaState* state);
	// 获得缺省页
	INT GetDefaultPage(LuaPlus::LuaState* state);
	// 设置缺省页
	INT SetDefaultPage(LuaPlus::LuaState* state);
	// 显示宠物
	INT ViewPetDesc(LuaPlus::LuaState* state);
	//设置选中的自己摊位上的宠物
	INT SetSelectPet(LuaPlus::LuaState* state);

public:
	static StallSale s_StallSale;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class StallBuy
{
public:
	//的到玩家是否存在
	INT IsPresent(LuaPlus::LuaState* state);
	//导出本地税率
	INT GetTradeTax(LuaPlus::LuaState* state);
	//打开别人的摊位
	INT OpenStall(LuaPlus::LuaState* state);
	//获得指定位置的定价
	INT GetPrice(LuaPlus::LuaState* state);
	//购买物品
	INT BuyItem(LuaPlus::LuaState* state);
	//打开摊位信息
	INT OpenMessageBuy(LuaPlus::LuaState* state);
	//获得摊位名
	INT GetStallName(LuaPlus::LuaState* state);
	//获得广告语
	INT GetAdvertise(LuaPlus::LuaState* state);
	//关闭
	INT CloseStallMessage(LuaPlus::LuaState* state);
	//导出自己的GUID
	INT GetGuid(LuaPlus::LuaState* state);
	//导出摊主的名字
	INT GetStallerName(LuaPlus::LuaState* state);
	// 获得宠物的个数
	INT GetPetNum(LuaPlus::LuaState* state);
	// 获得宠物
	INT EnumPet(LuaPlus::LuaState* state);
	// 获得缺省页
	INT GetDefaultPage(LuaPlus::LuaState* state);
	//购买宠物
	INT BuyPet(LuaPlus::LuaState* state);

public:
	static StallBuy s_StallBuy;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class StallBbs
{
public:
	//获得信息的条数
	INT GetMessageNum(LuaPlus::LuaState* state);
	//获得
	INT EnumMessage(LuaPlus::LuaState* state);
	//添加一条新的信息
	INT AddMessage(LuaPlus::LuaState* state);
	//回复一条信息
	INT ReplyMessage(LuaPlus::LuaState* state);


public:
	static StallBbs s_StallBbs;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class MissionReply
{
public:
	//的到玩家是否存在
	INT IsPresent(LuaPlus::LuaState* state);
	//打开宠物列表
	INT OpenPetFrame(LuaPlus::LuaState* state);
	//点击"Continue的操作"
	INT OnContinue(LuaPlus::LuaState* state);
	//
	INT EnumItem(LuaPlus::LuaState* state);


public:
	static MissionReply s_MissionReply;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class NpcShop
{
public:
	//关闭NpcShop
	INT Close(LuaPlus::LuaState* state);
	//获得商店NpcID
	INT GetNpcId(LuaPlus::LuaState* state);
	//获得商店的物品
	//INT EnumCallBackItem(LuaPlus::LuaState* state);
	//获得回购物品的数量
	INT GetCallBackNum(LuaPlus::LuaState* state);
	//获得商店的物品价格
	INT EnumItemPrice(LuaPlus::LuaState* state);
	//获得商店物品叠加数量
	INT EnumItemMaxOverlay(LuaPlus::LuaState* state);
	//获得商店物品名称
	INT EnumItemName(LuaPlus::LuaState* state);
	//一次购买多个物品
	//INT BulkBuyItem(LuaPlus::LuaState* state);
	//获得商店的详细类型
	//INT GetShopType(LuaPlus::LuaState* state);
	// 获得当前玩家身上装备的所有修理价格，（需要考虑这个Npc的修理价格系数，能不能修理等条件）
	//INT GetRepairAllPrice(LuaPlus::LuaState* state);

	//得到NPC的阵营
	INT GetNpcCamp(LuaPlus::LuaState* state);

public:
	static NpcShop s_NpcShop;
	static LuaPlus::LuaObject* s_pMetaTable;
};

//玩家商店
class PlayerShop
{
public:
	INT CreateShop(LuaPlus::LuaState* state);
	// 获得商店的数量
	INT GetShopNum(LuaPlus::LuaState* state);
	// 
	INT EnumShop(LuaPlus::LuaState* state);
	// 
	INT AskOpenShop(LuaPlus::LuaState* state);
	// 
	INT EnumItem(LuaPlus::LuaState* state);
	// 
	INT EnumShopInfo(LuaPlus::LuaState* state);
	// 
	INT GetSelectIndex(LuaPlus::LuaState* state);
	// 从玩家商店取回物品到自己的背包
	INT RetackItem(LuaPlus::LuaState* state);
	//
	INT InputMoney(LuaPlus::LuaState* state);
	//上架（下架）
	INT UpStall(LuaPlus::LuaState* state);
	//
	INT	SetCurSelectPage(LuaPlus::LuaState* state);
	//
	INT	GetCurSelectPage(LuaPlus::LuaState* state);
	//
	INT DealMoney(LuaPlus::LuaState* state);
	// 向服务器提交
	INT ApplyMoney(LuaPlus::LuaState* state);
	//
	INT InfoMoney(LuaPlus::LuaState* state);
	//
	INT GetStallNum(LuaPlus::LuaState* state);
	//
	INT BuyItem(LuaPlus::LuaState* state);
	//
	INT IsSelectOnSale(LuaPlus::LuaState* state);
	//下架
	INT DownSale(LuaPlus::LuaState* state);
	//清空当前的选择
	INT ClearSelect(LuaPlus::LuaState* state);
	//获得金钱
	INT GetMoney(LuaPlus::LuaState* state);
	//获得商业指数
	INT GetCommercialFactor(LuaPlus::LuaState* state);
	//获得店名
	INT GetShopInfo(LuaPlus::LuaState* state);
	//检查一个Stall是否Open
	INT IsOpenStall(LuaPlus::LuaState* state);
	//开张一个柜台
	INT OpenStall(LuaPlus::LuaState* state);
	//请求一个Stall的数据
	INT AskStallData(LuaPlus::LuaState* state);
	//宠物
	INT EnumPet(LuaPlus::LuaState* state);
	//设置当前选中的宠物
	INT SetSelectPet(LuaPlus::LuaState* state);
	//获得当前能开的商店的类型
	INT GetCanOpenShopType(LuaPlus::LuaState* state);
	//获得当前选中的宠物Index
	INT SetCurSelectPetIndex(LuaPlus::LuaState* state);
	//获得选择物品或者宠物的价格
	INT GetObjPrice(LuaPlus::LuaState* state);
	//修改商店的内容
	INT Modify(LuaPlus::LuaState* state);
	//获得选中的物品（宠物）的名称
	INT GetObjName(LuaPlus::LuaState* state);
	//显示宠物详细信息
	INT ViewPetDesc(LuaPlus::LuaState* state);
	//清除现在选择的 Position,,在进行操作的时候，可以使用检测这个值是不是为－1 来标志是不是界面有选中物品
	INT ClearSelectPos(LuaPlus::LuaState* state);
	//盘出商店
	INT Transfer(LuaPlus::LuaState* state);
	// 查看当前的商店是不是处于盘店状态
	INT IsSaleOut(LuaPlus::LuaState* state);
	// 购买商店
	INT BuyShop(LuaPlus::LuaState* state);
	//关闭
	INT CloseShop(LuaPlus::LuaState* state);
	//打开（关闭）Message
	INT OpenMessage(LuaPlus::LuaState* state);
	//获得Message的数量
	INT GetMessageNum(LuaPlus::LuaState* state);
	//当前页的Message的数量
	INT GetCurPageMessageNum(LuaPlus::LuaState* state);
	//获得Message
	INT EnumMessage(LuaPlus::LuaState* state);
	//操作合伙人
	INT DealFriend(LuaPlus::LuaState* state);
	//获得合伙人的数量
	INT GetFriendNum(LuaPlus::LuaState* state);
	//获得合伙人
	INT EnumFriend(LuaPlus::LuaState* state);
	//扩张（缩减）店铺
	INT ChangeShopNum(LuaPlus::LuaState* state);
	// 获得访问的商店管理Npc的当前场景ID，（用来做CareObject）
	INT GetNpcId(LuaPlus::LuaState* state);

public:
	static PlayerShop s_PlayerShop;
	static LuaPlus::LuaObject* s_pMetaTable;
};

//好友相关
class Friend
{
public:
	//关闭
	INT Close(LuaPlus::LuaState* state);
	//关闭
	
	//查询名字
	INT EnumName(LuaPlus::LuaState* state);
	
	INT OpenMenu( LuaPlus::LuaState* state);
	
	INT AskTeam(LuaPlus::LuaState* state);
	INT InviteTeam(LuaPlus::LuaState* state);

	INT OpenGrouping( LuaPlus::LuaState* state);

	// 得到有多少条消息
	INT GetHistroyNumber( LuaPlus::LuaState* state);
	// 得到历史数据
	INT GetHistroyData(LuaPlus::LuaState* state);

	INT	SetCurrentTeam( LuaPlus::LuaState* state );
	INT GetCurrentTeam( LuaPlus::LuaState* state );

	INT SetCurrentSelect( LuaPlus::LuaState* state );
	INT GetCurrentSelect( LuaPlus::LuaState* state );
	//拉人界面的一个“同意”和“不同意”处理
	INT CallOf(LuaPlus::LuaState* state);
	// 点击显示心情的操作
	INT ViewFeel(LuaPlus::LuaState* state);
	//测试心情是不是在自己的头顶
	INT IsMoodInHead(LuaPlus::LuaState* state);

	INT IsPlayerIsFriend(LuaPlus::LuaState* state);

public:
	static Friend s_Friend;
	static LuaPlus::LuaObject* s_pMetaTable;
	static int m_nCurTeam; // 当前选中的组，
	static int m_nCurSelect;
	Friend()
	{
		m_nCurTeam = 1; 
		m_nCurSelect = 0;
	};

};


//---------------------------------------------------------------------------------------------------------------------------------------
//
// 登录信息导出函数.
//
class CGameProduce_Login
{

public: 

	CGameProduce_Login();
	~CGameProduce_Login();

public:
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 选择服务器
	//

	//得到区域服务器的个数.
	INT GetAreaCount(LuaPlus::LuaState* state);
	
	//得到区域服务器的名字.
	INT GetAreaName(LuaPlus::LuaState* state);

	//得到区域服务器的提供商.

	//得到区域服务器中, login server 的个数.
	INT GetAreaLoginServerCount(LuaPlus::LuaState* state);

	//得到区域服务器的信息
	INT GetAreaServerInfo(LuaPlus::LuaState* state);

	// 得到login server 的信息.
	INT GetAreaLoginServerInfo(LuaPlus::LuaState* state);

	// 选择一个服务器
	INT SelectLoginServer(LuaPlus::LuaState* state);

	// 自动选择一个与当前选择的网络供应商匹配的login server
	INT AutoSelLoginServer(LuaPlus::LuaState* state);





	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 帐号输入界面
	//

	// 连接到login server
	INT ConnectToLoginServer(LuaPlus::LuaState* state);

	// 退到服务器选择界面
	INT ExitToSelectServer(LuaPlus::LuaState* state);

	// 验证用户名和密码
	INT CheckAccount(LuaPlus::LuaState* state);

	


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 人物选择界面
	//

	// 从人物选择界面切换到帐号输入界面
	INT ChangeToAccountInputDlgFromSelectRole(LuaPlus::LuaState* state);

	// 从人物选择界面切换到人物创建界面
	INT ChangeToCreateRoleDlgFromSelectRole(LuaPlus::LuaState* state);

	// 从人物创建界面切换到人物选择界面
	INT ChangeToSelectRoleDlgFromCreateRole(LuaPlus::LuaState* state);

	// 得到角色的个数
	INT GetRoleCount(LuaPlus::LuaState* state);

	// 得到角色的信息
	INT GetRoleInfo(LuaPlus::LuaState* state);

	// 得到角色的ui模型名字
	INT GetRoleUIModleName(LuaPlus::LuaState* state);

	// 创建角色
	INT CreateRole(LuaPlus::LuaState* state);

	// 删除角色
	INT DelRole(LuaPlus::LuaState* state);

	// 发送进入游戏消息
	INT SendEnterGameMsg(LuaPlus::LuaState* state);

	// 询问是否删除角色
	INT DelRole_YesNo(LuaPlus::LuaState* state);

	// 询问是否退出游戏
	INT ExitGame_YesNo(LuaPlus::LuaState* state);

	// 询问是否退到帐号输入界面
	INT ExitToAccountInput_YesNo(LuaPlus::LuaState* state);

	// 删除选择的角色
	INT DelSelRole(LuaPlus::LuaState* state);

	


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 关于头像部分.
	//

	// 得到女主角头像的个数
	INT GetWomanFaceCount(LuaPlus::LuaState* state);

	// 得到男主角头像的个数
	INT GetManFaceCount(LuaPlus::LuaState* state);

	// 得到女主角头像的名称
	INT GetWomanFaceName(LuaPlus::LuaState* state);

	// 得到男主角头像的名称
	INT GetManFaceName(LuaPlus::LuaState* state);

	// 得到主角头像的名称
	INT GetFaceName(LuaPlus::LuaState* state);

	// 设置头像id
	INT SetFaceId(LuaPlus::LuaState* state);



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 关于脸形部分.
	//

	
	// 得到脸形model的个数
	INT GetFaceModelCount(LuaPlus::LuaState* state);

	// 得到脸形模型的名字
	INT GetFaceModelName(LuaPlus::LuaState* state);

	// 设置脸形模型id
	INT SetFaceModelId(LuaPlus::LuaState* state);



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 关于发形部分.
	//
	
	// 得到脸形model的个数
	INT GetHairModelCount(LuaPlus::LuaState* state);

	// 得到脸形模型的名字
	INT GetHairModelName(LuaPlus::LuaState* state);

	// 设置脸形模型id
	INT SetHairModelId(LuaPlus::LuaState* state);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 职业阵营部分。
	//

	// 得到玩家阵营
	INT GetPlayerCamp(LuaPlus::LuaState* state);

	// 设置玩家阵营
	INT SetPlayerCamp(LuaPlus::LuaState* state);

	// 设置玩家职业
	INT SetPlayerMenpai(LuaPlus::LuaState* state);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 网络状态部分。
	//
	
	// 关闭网络连接。2006－4－12
	INT CloseNetConnect(LuaPlus::LuaState* state);


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 声音部分。
	//
	
	//  开始播放音乐。2006－4－29
	INT PlayMusic(LuaPlus::LuaState* state);

	//  停止播放音乐。2006－4－29
	INT StopMusic(LuaPlus::LuaState* state);


public:
	static CGameProduce_Login s_GameProduceLogin;
	static LuaPlus::LuaObject* s_pMetaTable;
};

};//namespace SCRIPT_SANDBOX



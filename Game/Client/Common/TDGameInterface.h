#pragma once
#include "TDNode.h"
#include "TDMath.h"
#include "../../../Common/GameDefine2.h"

/*
GameINTerface类
整个游戏的外部控制接口类，所有的外部模块需要通过这个接口来控制游戏的数据和控制
*/


class tActionItem;
class tObject_Item;
class CObject;

#ifndef GUID_t
typedef UINT	GUID_t;
#endif

class tGameInterfaceBase : public tNode
{
public:
	//------------------------------------------------
	// 物体操作系列
	
	//将某个物体选择为主要目标
	virtual VOID	Object_SelectAsMainTarget(INT idObj, int iType = 0)	= 0;
	//显示右键菜单
	virtual VOID	Object_ShowContexMenu(INT idObj, BOOL showMyself = FALSE)		= 0;

	///------------------------------------------------
	// 主角操作系列

	//移动到场景中某个位置
	virtual VOID	Player_MoveTo(const fVector2&  fvAimPos)	= 0;
	virtual VOID	Player_AUTOHit( const int AUTOHit )	= 0;
	//使用技能(瞬发)
	virtual VOID	Player_UseSkill( INT idSkill)	= 0;
	//使用技能(对某个Obj)
	virtual VOID	Player_UseSkill( INT idSkill, INT idObj)	= 0;
	//使用技能(对某个范围)
	virtual VOID	Player_UseSkill( INT idSkill, const fVector2& fvPos) = 0;
	//使用技能(对某个方向)
	virtual VOID	Player_UseSkill( INT idSkill, FLOAT fDir )	= 0;
	//使用技能(对某个队友(GUID))
	virtual VOID	Player_UseSkill( INT idSkill, const GUID_t& fDir )	= 0;
	//合成某配方()
	virtual VOID	Player_UseLifeAbility( INT idPrescription, INT nMakeCount )	= 0;
	virtual VOID	Player_UseLifeAbility( INT idPrescription )	= 0;
	//销毁身上的装备
	virtual VOID	Player_DestroyEquip(const tObject_Item* pEquip)  = 0;
	//卸载身上的装备
	virtual VOID	Player_UnEquip(const tObject_Item* pEquip)  = 0;
	//请求升级
	virtual VOID	Player_AskLevelUp(VOID)						= 0;
	//请求洗一级属性点
	virtual VOID	Player_AskCleanBat1Point(VOID)				= 0;
	//谈话(对某个NPC)
	virtual VOID	Player_Speak(INT idObj)					= 0;

	//------------------------------------------------
	//试图对某个TripperObj进行操作，如果距离过远，会走过去
	virtual VOID	TripperObj_Active( UINT nId )				= 0;
	//试图捡起当前盒子里的第几项
	virtual VOID	ItemBox_PickItem(tObject_Item* pItem)		= 0;

	//------------------------------------------------
	//使用包裹里的物品_通用
	virtual VOID	PacketItem_UserItem(tActionItem* pActionItem, int nTargetServerID, fVector2& fvPos) = 0;
	//使用包裹里的物品_对象
	virtual VOID	PacketItem_UserItem_ToTarget( INT nIndex, UINT nTargetID)	= 0;
	//使用包裹里的物品_装备
	virtual VOID	PacketItem_UserEquip( INT nIndex )					= 0;
	//使用包裹里的物品_宝石
	virtual VOID	PacketItem_UserGem(INT nGemIndex, INT EquipIndex)	= 0;
	//销毁包裹里的物品
	virtual VOID	PacketItem_Destroy(INT nIndex)						= 0;

	//------------------------------------------------
	//发送GM指令
	virtual VOID	GM_SendCommand( LPCSTR strCommand )			= 0;
	virtual VOID    SendChatCommand(INT nChannelID, INT nChatType, INT nTeamID, LPCTSTR szContex, LPCTSTR szTargetName)							= 0;	
	//------------------------------------------------
	//场景
	virtual VOID	Scene_SetPostFilter(LPCTSTR szFilterName)	= 0;
	virtual VOID	Scene_SetLightmapQuality(INT nQuality)		= 0;

	//------------------------------------------------
	//设置当前激活技能
	virtual VOID			Skill_SetActive(tActionItem* pActiveSkill)	= 0;
	virtual tActionItem*	Skill_GetActive(VOID)				= 0;
	//恢复到缺省技能操作，用于退出修理，鉴定，等特殊鼠标命令状态
	virtual VOID			Skill_CancelAction(VOID) = 0;

	//------------------------------------------------
	//购买物品
	virtual VOID			Booth_BuyItem( tObject_Item* pItem ) = 0;
	//卖出商品
	virtual VOID			Booth_SellItem(tObject_Item* pItem ) = 0;
	//完成交易
	virtual VOID			Booth_Close(VOID)					 = 0;
	//修理
	virtual VOID			Booth_Repair(BOOL bAll, INT bagindex, BOOL isInBag = TRUE) = 0;
	//进入修理状态
	virtual VOID			Booth_EnterRepair(VOID) = 0;

	//-------------------------------------------------
	//更新称号
	virtual VOID			Title_UpdateCurTitle(BYTE titleType, SHORT titleData)	 = 0;

	virtual VOID			Stall_OpenStallSale(INT nObjId)		 = 0;
	//------------------------------------------------
	//阵营判断
	virtual ENUM_RELATION	GetCampType( CObject *pObj_A, CObject *pObj_B ) = 0;

protected:
	WX_DECLARE_DYNAMIC(tGameInterfaceBase);
};


#pragma once

#include "Obj_PlayerNPC.h"
/**
	其他玩家角色类
*/

class CObject_Item_Equip;
class CObject_PlayerOther : public CObject_PlayerNPC
{
public:
	//返回角色类型
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const { return CT_PLAYEROTHER;	}
	//发出请求更详细信息的请求
	virtual VOID			SendEuipRequest(VOID);
	// 指令接收, 外界控制角色的唯一接口
	virtual RC_RESULT		OnCommand(const SCommand_Object *pCmd );

	// 左键指令的分析
	virtual BOOL			FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	// 右键指令的分析
	virtual BOOL			FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );


	//--------------------------------------------------------
	//装配物品
	// 装配装备类物品
			VOID			EquipItem_BodyPart(HUMAN_EQUIP nPart, INT nID );	
	// 装配挂接类物品
			VOID			EquipItem_BodyLocator(HUMAN_EQUIP nPart, INT nID );	
	//卸载物品(显示缺省外型)
	virtual VOID			UnEquipItem( HUMAN_EQUIP nPart);

	//------------------------------------------
	//逻辑数据发生改变
	//------------------------------------------
protected:
	friend class CCharacterData;
	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_FaceMesh(VOID);
	virtual VOID			OnDataChanged_HairMesh(VOID);
	virtual VOID			OnDataChanged_HairColor(VOID);
	virtual VOID			OnDataChanged_EquipVer(VOID);
	virtual VOID			OnDataChanged_Equip(HUMAN_EQUIP point);
//	virtual VOID			OnDataChanged_Titles(INT index);
	virtual VOID			OnDataChanged_FaceImage(VOID);			// 头像改变事件函数, 2006-3-9

protected:
	VOID					UpdateFaceMesh(VOID);
	VOID					UpdateHairMesh(VOID);
	VOID					UpdateHairColor(VOID);
	VOID					UpdateEquip(HUMAN_EQUIP point);

protected:
	virtual VOID			OnHideWeapon(INT nAppointedWeaponID);
	virtual VOID			OnShowWeapon(VOID);

protected:
	// RenderInterface创建/删除
	virtual VOID			CreateRenderInterface(VOID);

	virtual INT				CalcCharModelID( VOID )const;

	virtual UINT			GetIdleInterval(VOID)const;

	virtual VOID			UpdateCharRace(VOID);

public:
	//更改跑步速度
			VOID			Debug_ChangeSpeed(FLOAT fStep);
	//更改跑步频率
			VOID			Debug_ChangeRate(FLOAT fStep);
			FLOAT			Debug_GetRate(VOID) { return m_fDebugRate; }
protected:

	//CharRace表中的定义
	const _DBC_CHAR_RACE*	m_pCharRace;

	//-- for debug
	//角色骨骼动画速度
	FLOAT					m_fDebugRate;
	//-- for debug

	//--------------------------------------------------------
	//核心继承
	//--------------------------------------------------------
public:
	CObject_PlayerOther();
	virtual ~CObject_PlayerOther();

	///根据初始化物体，并同步到渲染层
	virtual	VOID			Initial(VOID*);

protected:
	WX_DECLARE_DYNAMIC(CObject_PlayerOther);
	//TD_DECLARE_LOGICAL(true);
};

// Obj_PlayerPet.h
// 
//////////////////////////////////////////////////////

#pragma once

#include "Obj_PlayerNPC.h"

class CObject_PlayerPet : public CObject_PlayerNPC
{
public:
	//返回角色类型
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const { return CT_PLAYERPET;	}

	//------------------------------------------
	//逻辑数据发生改变
	//------------------------------------------
protected:
	// RenderInterface创建/删除
	virtual VOID			CreateRenderInterface(VOID);

public:
	//更改跑步速度
			VOID			Debug_ChangeSpeed(FLOAT fStep);
	//更改跑步频率
			VOID			Debug_ChangeRate(FLOAT fStep);
			FLOAT			Debug_GetRate(VOID) { return m_fDebugRate; }
protected:
	//-- for debug
	//角色骨骼动画速度
	FLOAT					m_fDebugRate;
	//-- for debug

protected:
	virtual VOID				OnNewSoundHandle(VOID);
	virtual VOID				OnDeleteSoundHandle(VOID);

	//--------------------------------------------------------
	//核心继承
	//--------------------------------------------------------
public:
	CObject_PlayerPet();
	virtual ~CObject_PlayerPet();

	///根据初始化物体，并同步到渲染层
	virtual	VOID			Initial(VOID*);
	///逻辑函数
	virtual VOID			Tick(VOID);

protected:
	TD_DECLARE_DYNAMIC(CObject_PlayerPet);
	TD_DECLARE_LOGICAL(true);
};

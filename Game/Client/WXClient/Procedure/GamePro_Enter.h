/****************************************\
*										*
* 		  进入场景前的等待流程			*
*										*
\****************************************/

#pragma once

#include "GameProcedure.h"

class CGamePro_Enter : public CGameProcedure
{
public:
	//--------------------------------------------------------------
	//登录状态
	enum ENTER_STATUS
	{
		ENTERSCENE_CONNECTING,	//!< 与game server进行连接
		ENTERSCENE_READY,		//!< 初始化状态

		ENTERSCENE_REQUESTING,	//!< 发送进入场景的请求...
		ENTERSCENE_OK,			//!< 进入场景成功，会转入下一个循环
		ENTERSCENE_FAILED,		//!< 进入场景失败
	};
	ENTER_STATUS		GetStatus(VOID) { return m_Status; }
	VOID				SetStatus(ENTER_STATUS status) { m_Status = status; }

	//--------------------------------------------------------------
	//设置所要进入的场景的信息(场景ID/城市id, 城市等级)
	VOID SetSceneID(INT nSceneID, INT nCityLevel) 
	{ 
		m_nSceneID = nSceneID; 
		m_nCityLevel = nCityLevel;
	}

	VOID				SetEnterType(UINT dwEnterType) { m_dwEnterType = dwEnterType; }
	UINT				GetEnterType(void) const { return m_dwEnterType; }

	//进入指定场景
	VOID				EnterScene(void);


protected:
	// 进入游戏服务器的ip 地址和端口号
	TCHAR	m_szLoginServerAddr[64];
	INT		m_nLoginServerPort;

	// 进入游戏服务器的场景号，如果是玩家城市，则是玩家城市ID
	INT		m_nSceneID;
	//如果是玩家城市，城市等级
	INT		m_nCityLevel;

	UINT			m_dwEnterType;
	ENTER_STATUS	m_Status;

protected:
	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
	virtual VOID	MouseInput(VOID);

public:
	CGamePro_Enter(VOID);
	virtual ~CGamePro_Enter(VOID);
};

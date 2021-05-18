#ifndef __GAMEPRO_CHANGESCENE_H__
#define __GAMEPRO_CHANGESCENE_H__
#pragma once
/****************************************\
*										*
* 			   登录流程					*
*										*
\****************************************/
#pragma once

#include "GameProcedure.h"
#include "LoginDataStructDef.h"
#include <string>

class tSoundSource;
class CGamePro_ChangeScene: public CGameProcedure
{

public:

	//登录状态
	enum PLAYER_CHANGE_SERVER_STATUS
	{
		CHANGESCENE_DISCONNECT,					// 断开状态
		CHANGESCENE_CONNECTING,					// 连接中
		CHANGESCENE_CONNECT_SUCCESS,			// 连接成功
		CHANGESCENE_SENDING_CGCONNECT,			// 正在发送握手消息
		CHANGESCENE_RECEIVE_CGCONNECT_SUCCESS,	// 允许进入服务器
		CHANGESCENE_RECEIVE_CGCONNECT_FAIL,		// 不允许进入服务器
	};

	// 流程状态
	PLAYER_CHANGE_SERVER_STATUS m_Status;

	// 设置状态
	void	SetStatus(PLAYER_CHANGE_SERVER_STATUS status);

	// 得到状态
	PLAYER_CHANGE_SERVER_STATUS		GetStatus(VOID);

	//
	void ConnectToGameServer();

	// 发送cgconnect
	void SendCGConnectMsg();

	// 切换到enter game 流程
	void ChangeToEnterGameProdure();

public:
	CGamePro_ChangeScene(void);
	~CGamePro_ChangeScene(void);

	
public:
	//处理输入
	//virtual VOID	ProcessInput(VOID);

protected:
	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
};

#endif //__GAMEPRO_CHANGESCENE_H__
/****************************************\
*										*
* 			   网络管理器				*
*										*
\****************************************/
#pragma once

#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Socket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "Packet.h"
#include "PacketFactoryManager.h"
#include "TDNode.h"
#include "..\World\WorldManager.h"

/// 网络管理器
class CNetManager : public tNode
{
public:
	/// 连接到服务器
	VOID			ConnectToServer(LPCTSTR szServerAddr, INT nServerPort);
	///　发送数据包
	VOID			SendPacket(Packet* pPacket);
	//断开连接
	VOID			Close(VOID);
	//处理消息缓冲
	VOID			ProcessInputStream(SocketInputStream& inputStream);
	//得到消息工厂
	PacketFactoryManager&	GetPacketFactory(void) { return m_PacketFactoryManager; }

public:
	//节点初始化
	virtual VOID	Initial(VOID*);
	//逻辑轮循函数
	virtual VOID	Tick(VOID);
	//释放自己所所拥有的资源
	virtual VOID	Release(VOID);

public:

public:

	enum NETMANAGER_STATUS
	{
		CONNECT_SUCESS  = 0,
		CONNECT_FAILED_CREATE_SOCKET_ERROR,
		CONNECT_FAILED_CONNECT_ERROR,
		CONNECT_FAILED_TIME_OUT,

	};

	//--------------------------------------------------------------------------------------------------------------
	//
	// Tick不同的游戏流程
	//

	// Tick 游戏登录流程.
	VOID	TickGameLoginProcedure();

	// Tick 主游戏流程
	VOID	TickGameMainProcedure();

	// Tick 人物选择流程
	VOID	TickGameSelCharcterProcedure(VOID);

	// Tick 人物创建流程
	VOID	TickGameCreateCharcterProcedure(VOID);

	// Tick 连接到服务器流程
	VOID	TickChangeGameServerProcedure(VOID);

	// Tick 进入流程
	VOID	TickGameEnterProcedure(VOID);

	//
	// Tick不同的游戏流程
	//
	//--------------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------------------------------
	//
	// 设置网络状态
	// 

	// 设置网络状态
	VOID SetNetStatus(NETMANAGER_STATUS netStatus);

	// Tick 游戏登录流程.
	VOID	SetGameLoginStatus(NETMANAGER_STATUS netStatus);

	// Tick 主游戏流程
	VOID	SetGameMainStatus(NETMANAGER_STATUS netStatus);


	//
	//  设置网络状态
	//
	//--------------------------------------------------------------------------------------------------------------
	
public:
	CNetManager();
	virtual ~CNetManager();
	static CNetManager* GetMe(VOID) { return s_pMe; }

protected:
	//连接线程,专门用来连接服务器用
	static UINT CALLBACK	_ConnectThread(LPVOID pParam);
	INT				ConnectThread(VOID);
	//连接线程句柄
	HANDLE			m_hConnectThread;
	UINT			m_timeConnectBegin;

private:
	friend VOID CWorldManager::ChangeScene(INT idTargetScene, const fVector2& fvPos, INT nDirection, BYTE bFlag);

	VOID				WaitConnecting(VOID);
	VOID				WaitPacket(VOID);
	VOID				SendHeartBeat(VOID);

	//SOCKET操作集
	BOOL				Select(VOID);
	BOOL				ProcessExcept(VOID);
	BOOL				ProcessInput(VOID);
	BOOL				ProcessOutput(VOID);
	VOID				ProcessCommands(VOID);

	//消息包执行
	UINT				executePacket_Gen_Exception(Packet* pPacket);
	UINT				executePacket_CPP_Exception(Packet* pPacket);


protected:
	static CNetManager*		s_pMe;
	///服务器信息
	STRING					m_strServerAddr;
	INT						m_nServerPort;

private:
	//可读句柄
	fd_set					m_ReadFD;
	//可写句柄
	fd_set					m_WriteFD;
	//异常句柄
	fd_set					m_ExceptFD;
	///SOCKET
	Socket					m_Socket;
	///输入缓冲区
	SocketInputStream		m_SocketInputStream;
	///输出缓冲区
	SocketOutputStream		m_SocketOutputStream;
	//SOCKET包管理器
	PacketFactoryManager	m_PacketFactoryManager;

protected:
	WX_DECLARE_DYNAMIC(CNetManager);
};

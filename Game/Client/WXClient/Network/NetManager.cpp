#include "StdAfx.h"
#include "NetManager.h"
#include <process.h>
#include "..\Procedure\GameProcedure.h"
#include "..\Procedure\GamePro_Login.h"
#include "..\Procedure\Gamepro_ChangeScene.h"
#include "..\Procedure\GamePro_Enter.h"
#include "CGHeartBeat.h"
#include "..\Global.h"
#include "TDException.h"
#include "TDTimeSystem.h"
#include "..\Script\GMScriptSystem.h"
#include "LuaPlus.h"
#include "..\Event\GMEventSystem.h"
#include "TDUtil.h"
#include "TDException.h"
#include "..\World\WorldManager.h"

#define MAX_CONNECT_TIME	(10*1000)	// 最长连接时间10s 

WX_IMPLEMENT_DYNAMIC(CNetManager, GETCLASS(tNode));
CNetManager* CNetManager::s_pMe = NULL;
extern char Error[_ESIZE] ;

CNetManager::CNetManager()
	: m_SocketInputStream(&m_Socket)
	, m_SocketOutputStream(&m_Socket)
{
	s_pMe = this;
	m_hConnectThread = NULL;
}

CNetManager::~CNetManager()
{
}

VOID CNetManager::Initial(VOID*)
{
	//初始化WinSock
	WSADATA wsaData;
	if(0!=WSAStartup( MAKEWORD( 2, 2 ), &wsaData)  ||
		(LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 2 ) )
	{
		WSACleanup();
		TDThrow(_T("Could not find a usable WinSock DLL!"));
		return;
	}

	m_hConnectThread = NULL;
	m_PacketFactoryManager.Init();
}


// Tick 游戏登录流程.
VOID	CNetManager::TickGameLoginProcedure()
{
	switch(CGameProcedure::s_pProcLogIn->GetStatus())
		{
		case CGamePro_Login::LOGIN_DEBUG_SETTING:
			{
			
				break;
			}

		case CGamePro_Login::LOGIN_SELECT_SERVER:
			{
			
				break;
			}
		
			//尚未登录,准备状态
		case CGamePro_Login::LOGIN_DISCONNECT:
			{
				break;
			}

			//SOCKET连接中...
		case CGamePro_Login::LOGIN_CONNECTING:
			{
				WaitConnecting();
				break;
			}

			//连接失败
		case CGamePro_Login::LOGIN_CONNECT_FAILED:
			{
				break;
			}

		case CGamePro_Login::LOGIN_ACCOUNT_BEGIN_REQUESTING:
			{
				break;
			}
		//连接成功后的其他状态,开始正常处理消息包
		case CGamePro_Login::LOGIN_CONNECTED_OK:
			{
				WaitPacket();
				//CGameProcedure::s_pProcLogIn->OpenCountInputDlg();
				//CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_ACCOUNT_REQUESTING);
				break;
			}
		case CGamePro_Login::LOGIN_ACCOUNT_REQUESTING:
			{
				WaitPacket();
				break;
			}
		default:
			{
				WaitPacket();
				break;
			}
		}
}



// Tick 人物选择流程
VOID	CNetManager::TickGameSelCharcterProcedure(VOID)
{
	//正常处理消息包
	WaitPacket();
}

// Tick 人物创建流程
VOID	CNetManager::TickGameCreateCharcterProcedure(VOID)
{
	//正常处理消息包
	WaitPacket(); 
}

// Tick 主游戏流程
VOID	CNetManager::TickGameMainProcedure(VOID)
{
	//正常处理消息包
	WaitPacket();

	//定时发送心跳消息
	SendHeartBeat();
}

// Tick 连接到服务器流程
VOID	CNetManager::TickChangeGameServerProcedure(VOID)
{
	switch(CGameProcedure::s_pProcChangeScene->GetStatus())
		{
		case CGamePro_ChangeScene::CHANGESCENE_DISCONNECT:
			{
			
				break;
			}
			//SOCKET连接中...
		case CGamePro_ChangeScene::CHANGESCENE_CONNECTING:
			{
				WaitConnecting();
				break;
			}

			//连接失败
		case CGamePro_ChangeScene::CHANGESCENE_CONNECT_SUCCESS:
			{
				break;
			}

		//连接成功后的其他状态,开始正常处理消息包
		case CGamePro_ChangeScene::CHANGESCENE_SENDING_CGCONNECT:
			{
				WaitPacket();
				break;
			}
		default:
			{
				break;
			}
		}
}


// Tick 进入流程
VOID	CNetManager::TickGameEnterProcedure(VOID)
{
	switch(CGameProcedure::s_pProcEnter->GetStatus())
		{
		case CGamePro_Enter::ENTERSCENE_READY:
			{
			
				break;
			}
			//SOCKET连接中...
		case CGamePro_Enter::ENTERSCENE_REQUESTING:
			{
				WaitPacket();
				break;
			}

			//连接失败
		case CGamePro_Enter::ENTERSCENE_OK:
			{
				break;
			}

		//连接成功后的其他状态,开始正常处理消息包
		case CGamePro_Enter::ENTERSCENE_FAILED:
			{
				
				break;
			}
		default:
			{
				break;
			}
		}
}


VOID CNetManager::Tick(VOID)
{
	//当前流程是登录流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcLogIn)
	{
		// Tick 游戏登录流程.
		TickGameLoginProcedure();
		//SendHeartBeat();
		
	}
	//处于主游戏循环中
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		// Tick 主游戏流程
		TickGameMainProcedure();

	}
	// 处于人物选择流程
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcCharSel)
	{
		// Tick 人物选择
		TickGameSelCharcterProcedure();
		//SendHeartBeat();
	}
	// 处于人物创建流程
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcCharCreate)
	{
		// Tick 人物创建流程
		TickGameCreateCharcterProcedure();
		//SendHeartBeat();
	}
	// tick game server 连接流程
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcChangeScene)
	{
		
		TickChangeGameServerProcedure();
		//SendHeartBeat();
	}
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcEnter)
	{
		
		// Tick 进入流程
		TickGameEnterProcedure();
		//SendHeartBeat();
	}

}

VOID CNetManager::WaitConnecting(VOID)
{
	//监测登录线程是否结束
	int nExitCode = 0;
	
	if(::GetExitCodeThread(m_hConnectThread, (DWORD*)&nExitCode))
	{

	}

	//登录线程未结束
	if( STILL_ACTIVE == nExitCode)
	{
		//检查是否超时
		UINT dwTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
		UINT dwUsed =  CGameProcedure::s_pTimeSystem->CalSubTime(m_timeConnectBegin, dwTimeNow);
		//超时
		if(dwUsed >= MAX_CONNECT_TIME)
		{
			//强制结束登录线程
			TerminateThread(m_hConnectThread, 0);
			nExitCode = -3;
		}
		//继续等待
		else 
		{
			return;
		}
	}

	//登录线程已经结束 关闭句柄
	if(CloseHandle(m_hConnectThread))
	{
		m_hConnectThread = NULL;
	}
	
	//登录过程中发生错误
	if(nExitCode < 0)
	{
		//LPCTSTR szErrorDesc;
		switch(nExitCode)
		{
		case -1:
			{
				SetNetStatus(CONNECT_FAILED_CREATE_SOCKET_ERROR);
				CGameProcedure::s_pEventSystem->PushEvent(GE_NET_CLOSE, "创建网络连接失败!");
				break;
			}
		case -2:
			{
				SetNetStatus(CONNECT_FAILED_CONNECT_ERROR);
				CGameProcedure::s_pEventSystem->PushEvent(GE_NET_CLOSE, "目的服务器可能关闭!");
				break;
			}
		case -3:
			{
				SetNetStatus(CONNECT_FAILED_TIME_OUT);
				CGameProcedure::s_pEventSystem->PushEvent(GE_NET_CLOSE, "连接超时!");
				break;
			}
		default:
			{
				SetNetStatus(CONNECT_FAILED_CONNECT_ERROR);
				CGameProcedure::s_pEventSystem->PushEvent(GE_NET_CLOSE, "未知错误!");
				break;
			}

		}
	
		this->Close();
		return;
	}

	//连接成功后设置为非阻塞模式和设置Linger参数
	if(!m_Socket.setNonBlocking() || !m_Socket.setLinger(0))
	{
		SetNetStatus(CONNECT_FAILED_CONNECT_ERROR);
		TDThrow(_T("(CNetManager::Tick)SetSocket Error"));
		return;
	}

	//通知登录流程,SOCKET连接成功
	
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcLogIn)
	{
		CGameProcedure::s_pProcLogIn->SendClConnectMsg();
		SetNetStatus(CONNECT_SUCESS);//
	}
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcChangeScene)
	{
		SetNetStatus(CONNECT_SUCESS);//
	}

	return;
}

VOID CNetManager::WaitPacket(VOID)
{
	if(!m_Socket.isValid())
	{
		return;
	}

	//网络数据流操作
	if(!Select() || !ProcessExcept() || !ProcessInput() || !ProcessOutput())
	{
		CEventSystem::GetMe()->PushEvent(GE_NET_CLOSE);
	}

	//Packet操作
	ProcessCommands( ) ;
}

VOID CNetManager::Release(VOID)
{
	//尚有登录线程在运行,强制终止
	if(m_hConnectThread) 
	{
		TerminateThread(m_hConnectThread, 0);
		CloseHandle(m_hConnectThread); m_hConnectThread = NULL;
	}

	m_Socket.close();

	WSACleanup();
}

VOID CNetManager::ConnectToServer(LPCTSTR szServerAddr, INT nServerPort)
{
	//已经有登录线程在运行中
	if(m_hConnectThread) return;

	m_strServerAddr = szServerAddr;
	m_nServerPort	= nServerPort;

	//创建登录线程
	UINT nThreadID;
	m_hConnectThread = (HANDLE)::_beginthreadex(NULL, 0, _ConnectThread, this, CREATE_SUSPENDED|THREAD_QUERY_INFORMATION, &nThreadID );
	if (m_hConnectThread == NULL)
	{
		TDThrow(_T("(CNetManager::ConnectToServer)Can't create connect thread!"));
	}

	//开始运行登录线程
	m_timeConnectBegin = CGameProcedure::s_pTimeSystem->GetTimeNow();
	ResumeThread(m_hConnectThread);
}

VOID CNetManager::SendPacket(Packet* pPacket)
{
	//如果是处于等待切换场景流程中
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain &&
		CWorldManager::GetMe()->GetStation() == CWorldManager::WS_ASK_CHANGE_SCANE)
	{
		return;
	}

	if(m_Socket.isValid())
	{
		UINT nSizeBefore = m_SocketOutputStream.Length();
		//m_SocketOutputStream.WritePacket( pPacket ) ;

		UINT t_uTail_Begin = m_SocketOutputStream.GetTail();//查询当前包尾位置。记录写包前位置

		PacketID_t packetID = pPacket->GetPacketID() ;
		UINT w = m_SocketOutputStream.Write( (CHAR*)&packetID , sizeof(PacketID_t) ) ;

		WORD packetTick = 0;//g_pTimeManager->RunTick(); //________________________________
		w = m_SocketOutputStream.Write( (CHAR*)&packetTick , sizeof(WORD) ) ;

		UINT packetUINT ;
		UINT packetSize = pPacket->GetPacketSize( ) ;
		UINT packetIndex = pPacket->GetPacketIndex( ) ;

		SET_PACKET_INDEX(packetUINT, packetIndex) ;
		SET_PACKET_LEN(packetUINT, packetSize) ;

		w = m_SocketOutputStream.Write( (CHAR*)&packetUINT, sizeof(UINT) ) ;

		BOOL ret = pPacket->Write( m_SocketOutputStream ) ;

		UINT nSizeAfter = m_SocketOutputStream.Length();

		UINT t_uTail_End = m_SocketOutputStream.GetTail();//查询当前包尾位置。记录写包后位置

		//消息加密处理--Begin
		{
			UINT t_uSize = t_uTail_End - t_uTail_Begin;
			UINT t_uHead = m_SocketOutputStream.GetHead();
			UINT t_uTail = m_SocketOutputStream.GetTail();
			UINT t_uBufferLen = m_SocketOutputStream.GetBuffLen();
			CHAR* t_szBuffer = m_SocketOutputStream.GetBuffer();
			if ( t_uHead < t_uTail ) 
			{
				ENCRYPT(&(t_szBuffer[t_uTail_Begin]), t_uSize, CLIENT_TO_LOGIN_KEY, 0);
			} 
			else
			{
				UINT rightLen = t_uBufferLen - t_uHead ;
				if( t_uSize <= rightLen ) 
				{
					ENCRYPT(&(t_szBuffer[t_uTail_Begin]), t_uSize, CLIENT_TO_LOGIN_KEY, 0);
				} 
				else 
				{
					ENCRYPT(&(t_szBuffer[t_uTail_Begin]), rightLen, CLIENT_TO_LOGIN_KEY, 0);
					ENCRYPT(t_szBuffer, t_uSize-rightLen, CLIENT_TO_LOGIN_KEY, rightLen);
				}
			}
		}
		//消息加密处理--End

		if(pPacket->GetPacketSize() != nSizeAfter-nSizeBefore-PACKET_HEADER_SIZE)
		{
			TDU_Log("Packet size error[id=%d, stream=%d, size=%d]",
				pPacket->GetPacketID(), nSizeAfter-nSizeBefore-PACKET_HEADER_SIZE, pPacket->GetPacketSize());
			//TDThrow("Packet size error[id=%d, stream=%d, size=%d]",
			//	pPacket->GetPacketID(), nSizeAfter-nSizeBefore-PACKET_HEADER_SIZE, pPacket->GetPacketSize());
		}
	}
}

UINT CNetManager::_ConnectThread(LPVOID pParam)
{
	TDAssert(pParam);
	CNetManager* pNetManager = (CNetManager*)	pParam;

	return pNetManager->ConnectThread();
}

//连接线程返回值
// 0  : 尚未连接
// 1  : 成功连接到服务器
// -1 : 创建SOCKET发生错误
// -2 : 无法连接到目的服务器
// -3 : 超时错误
INT CNetManager::ConnectThread(VOID)
{
	//关闭SOCKET
	m_Socket.close();
	//创建新的SOCKET
	if(!m_Socket.create()) 
	{
		return -1;
	}

	//连接到服务器
	if(!m_Socket.connect( m_strServerAddr.c_str(), m_nServerPort ))
	{
		m_Socket.close();
		return -2 ;
	}
	//成功连接
	return 1;
}

BOOL CNetManager::Select(VOID)
{
	FD_ZERO( &m_ReadFD ) ;
	FD_ZERO( &m_WriteFD ) ;
	FD_ZERO( &m_ExceptFD ) ;

	FD_SET( m_Socket.getSOCKET(), &m_ReadFD ) ;
	FD_SET( m_Socket.getSOCKET(), &m_WriteFD ) ;
	FD_SET( m_Socket.getSOCKET(), &m_ExceptFD ) ;
	
	timeval	Timeout ;
	Timeout.tv_sec = 0 ;
	Timeout.tv_usec = 0 ;

	if(SOCKET_ERROR == SocketAPI::select_ex(0 , 
										&m_ReadFD , 
										&m_WriteFD , 
										&m_ExceptFD , 
										&Timeout ) )
	{
		return FALSE;
	}
	return true;
}

BOOL CNetManager::ProcessInput(VOID)
{
	if( FD_ISSET( m_Socket.getSOCKET(), &m_ReadFD ) )
	{
		uint ret = m_SocketInputStream.Fill( ) ;
		if( (INT)ret <= SOCKET_ERROR )
		{
			//TDThrow("m_SocketInputStream.Fill ret %d %s", (INT)ret, Error);
			m_Socket.close() ;
			return FALSE;
		}
	}
	return true;
}

BOOL CNetManager::ProcessOutput(VOID)
{
	//如果是处于等待切换场景流程中
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain &&
		CWorldManager::GetMe()->GetStation() == CWorldManager::WS_ASK_CHANGE_SCANE)
	{
		return true;
	}

	if( FD_ISSET( m_Socket.getSOCKET(), &m_WriteFD ) )
	{
		uint ret = m_SocketOutputStream.Flush( ) ;
		if( (INT)ret <= SOCKET_ERROR )
		{
			//TDThrow("m_SocketInputStream.Fill ret %d %s", (INT)ret, Error);
			m_Socket.close() ;
			return FALSE;
		}
	}
	return true;
}

//处理消息缓冲
VOID CNetManager::ProcessInputStream(SocketInputStream& inputStream)
{
	static USHORT nPacketIDSaved=0;
	BOOL ret ;

	char PackCompart[PACKET_HEADER_SIZE];
	char header[PACKET_HEADER_SIZE];
	PacketID_t packetID;
	uint packetuint, packetSize, packetIndex;
	WORD packetTick;
	Packet* pPacket = NULL ;
	static PacketID_t packetIDList[10];
	static int packetIDListIdx = 0;

	{

		for( ;; )
		{
			if( !m_SocketInputStream.Find( PackCompart ) )
			{//数据没有消息分隔点
				break ;
			}
			if( !m_SocketInputStream.Peek(&header[0], PACKET_HEADER_SIZE) )
			{//数据不能填充消息头
				break ;
			}

			ENCRYPT_HEAD(header, LOGIN_TO_CLIENT_KEY)//封包头解密

			memcpy( &packetID, &header[0], sizeof(PacketID_t) ) ;	
			memcpy( &packetTick, &header[sizeof(WORD)], sizeof(WORD) ) ;
			memcpy( &packetuint, &header[sizeof(WORD)+sizeof(PacketID_t)], sizeof(uint) );
			packetSize = GET_PACKET_LEN(packetuint) ;
			packetIndex = GET_PACKET_INDEX(packetuint) ;

			if( packetID >= (PacketID_t)PACKET_MAX )
			{//无效的消息类型
				TDU_Log( "Invalid Packet, Old Packet List[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d], Idx=%d", 
					packetIDList[0], packetIDList[1], packetIDList[2], packetIDList[3], packetIDList[4],
					packetIDList[5], packetIDList[6], packetIDList[7], packetIDList[8], packetIDList[9], 
					packetIDListIdx );

				TDU_Log("Invalid Packet[%d]!", packetID);
				//TDThrow("Invalid Packet[%d]!", packetID);
				return ;
			}

			//消息解密处理--Begin
			{
				UINT t_uSize = packetSize+PACKET_HEADER_SIZE;
				UINT t_uHead = m_SocketInputStream.GetHead();
				UINT t_uTail = m_SocketInputStream.GetTail();
				UINT t_uBufferLen = m_SocketInputStream.GetBuffLen();
				CHAR* t_szBuffer = m_SocketInputStream.GetBuff();
				if ( t_uHead < t_uTail ) 
				{
					ENCRYPT(&t_szBuffer[t_uHead], t_uSize, LOGIN_TO_CLIENT_KEY, 0);
				} 
				else 
				{
					UINT rightLen = t_uBufferLen - t_uHead ;
					if( t_uSize <= rightLen ) 
					{
						ENCRYPT(&t_szBuffer[t_uHead], t_uSize, LOGIN_TO_CLIENT_KEY, 0);
					} 
					else 
					{
						ENCRYPT(&t_szBuffer[t_uHead], rightLen, LOGIN_TO_CLIENT_KEY, 0);
						ENCRYPT(t_szBuffer, t_uSize-rightLen, LOGIN_TO_CLIENT_KEY, rightLen);
					}
				}
			}
			//消息解密处理--End

			{
				if( m_SocketInputStream.Length()<PACKET_HEADER_SIZE+packetSize )
				{
					//消息没有接收全
					break;
				}

				if( packetSize>m_PacketFactoryManager.GetPacketMaxSize(packetID) )
				{
					TDU_Log( "Packet size error, Old Packet List[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d], Idx=%d", 
						packetIDList[0], packetIDList[1], packetIDList[2], packetIDList[3], packetIDList[4],
						packetIDList[5], packetIDList[6], packetIDList[7], packetIDList[8], packetIDList[9], 
						packetIDListIdx );
					//消息的大小出现异常，收到的消息比预定义消息的最大值还要大
					TDU_Log("Packet size error![ID=%d](%d>%d)", 
						packetID, packetSize, m_PacketFactoryManager.GetPacketMaxSize(packetID));
					//TDThrow("Packet size error![ID=%d](%d>%d)", 
					//	packetID, packetSize, m_PacketFactoryManager.GetPacketMaxSize(packetID));
//					m_SocketInputStream.Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return ;
				}

				Packet* pPacket = m_PacketFactoryManager.CreatePacket( packetID ) ;
				Assert( pPacket ) ;
				if( pPacket==NULL )
				{//不能分配到足够的内存
					TDU_Log( "Create Packet error, Old Packet List[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d], Idx=%d", 
						packetIDList[0], packetIDList[1], packetIDList[2], packetIDList[3], packetIDList[4],
						packetIDList[5], packetIDList[6], packetIDList[7], packetIDList[8], packetIDList[9], 
						packetIDListIdx );
					TDThrow("Create Packet error[%d]!", packetID);
//					m_SocketInputStream.Skip( PACKET_HEADER_SIZE+packetSize ) ;
					return ;
				}

				//设置消息序列号
				pPacket->SetPacketIndex( packetIndex ) ;

				ret = m_SocketInputStream.ReadPacket( pPacket ) ;
				if( ret==FALSE )
				{
					//读取消息内容错误
					TDU_Log( "Read Packet error, Old Packet List[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d], Idx=%d", 
						packetIDList[0], packetIDList[1], packetIDList[2], packetIDList[3], packetIDList[4],
						packetIDList[5], packetIDList[6], packetIDList[7], packetIDList[8], packetIDList[9], 
						packetIDListIdx );
					TDU_Log("Read Packet error!");
					//TDThrow("Read Packet error!");
					continue ;
				}

				packetIDList[packetIDListIdx++] = packetID;
				if( packetIDListIdx == 10 ) packetIDListIdx = 0;

				//消息包执行
				ret = executePacket_Gen_Exception(pPacket);

				if(ret != PACKET_EXE_NOTREMOVE)
				{
					m_PacketFactoryManager.RemovePacket( pPacket ) ;
				}

				nPacketIDSaved = packetID;
			}
		}
	}
}

UINT CNetManager::executePacket_Gen_Exception(Packet* pPacket)
{
	CHAR szTitle[MAX_PATH];
	_snprintf(szTitle, MAX_PATH, "Packet: %d", pPacket->GetPacketID());

	__try 
	{
		return executePacket_CPP_Exception(pPacket);
	}
	__except (tProcessInnerException(GetExceptionInformation(), g_hMainWnd, szTitle), EXCEPTION_EXECUTE_HANDLER) {}

	return PACKET_EXE_CONTINUE;
}

UINT CNetManager::executePacket_CPP_Exception(Packet* pPacket)
{
	try
	{
		return pPacket->Execute((Player*)this);
	}
	catch(const std::exception& e)
	{
		static STRING strCPPException;
		strCPPException = e.what();
		
		//转化为结构化异常
		LPVOID pException = &strCPPException;
		::RaiseException(
			0XE000C0DE, 
			EXCEPTION_NONCONTINUABLE,				// 不可继续的严重错误
			1,										// 1个参数
			(CONST ULONG_PTR *)&pException);		// 指向静态异常内存
	}
	catch(...)
	{
		throw;
	}

	return PACKET_EXE_CONTINUE;
}

VOID CNetManager::ProcessCommands(VOID)
{
	ProcessInputStream(m_SocketInputStream);
}

BOOL CNetManager::ProcessExcept( )
{
	if( FD_ISSET( m_Socket.getSOCKET(), &m_ExceptFD ) )
	{
		m_Socket.close() ;
		return FALSE;
	}
	return true;
}


VOID CNetManager::SendHeartBeat(VOID)
{
	static UINT s_dwLastSendTime = 0;
	const UINT HEART_BEAT_RATE = 60*1000;		// 1 Minutes

	UINT dwTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	if(dwTimeNow - s_dwLastSendTime >= HEART_BEAT_RATE)
	{
		CGHeartBeat msg;
		this->SendPacket(&msg);

		s_dwLastSendTime = dwTimeNow;
	}
}

VOID CNetManager::Close(VOID)
{
	m_Socket.close();
}

// 设置网络状态
VOID CNetManager::SetNetStatus(NETMANAGER_STATUS netStatus)
{
	
	//当前流程是登录流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcLogIn)
	{
		// Tick 游戏登录流程.
		SetGameLoginStatus(netStatus);
		
	}
	//处于主游戏循环中
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		// Tick 主游戏流程
		SetGameMainStatus(netStatus);
	}
	
	else if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcChangeScene)
	{
		CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_CONNECT_SUCCESS);
	}
}

// Tick 游戏登录流程.
VOID	CNetManager::SetGameLoginStatus(NETMANAGER_STATUS netStatus)
{
	switch(netStatus)
	{
	case CONNECT_SUCESS:
		{
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_CONNECTED_OK);//
			break;
		}
	case CONNECT_FAILED_CREATE_SOCKET_ERROR:
		{
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_CONNECT_FAILED);
			break;
		}
	case CONNECT_FAILED_CONNECT_ERROR:
		{
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_CONNECT_FAILED);
			break;
		}
	case CONNECT_FAILED_TIME_OUT:
		{
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_CONNECT_FAILED);
			break;
		}
	default:
		{
			break;
		}
	}
}

// Tick 主游戏流程
VOID	CNetManager::SetGameMainStatus(NETMANAGER_STATUS netStatus)
{
	switch(netStatus)
	{
	case CONNECT_SUCESS:
		{
			break;
		}
	case CONNECT_FAILED_CREATE_SOCKET_ERROR:
		{
			break;
		}
	case CONNECT_FAILED_CONNECT_ERROR:
		{
			break;
		}
	case CONNECT_FAILED_TIME_OUT:
		{
			break;
		}
	default:
		{
			break;
		}
	}
}

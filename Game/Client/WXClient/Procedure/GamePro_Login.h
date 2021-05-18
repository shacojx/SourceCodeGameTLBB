/****************************************\
*										*
* 			   登录流程					*
*										*
\****************************************/
#pragma once

#include "GameProcedure.h"
#include "LoginDataStructDef.h"
#include <string>
#include <vector>

// 网络供应商的个数
#define PROVIDE_COUNT	3

// 不合法字符串定义
typedef std::vector<std::string>  UNUSE_STRING_VECTOR;

class tSoundSource;
class CGamePro_Login : public CGameProcedure
{
public:
	//登录状态
	enum PLAYER_LOGIN_STATUS
	{
		LOGIN_DEBUG_SETTING,			//!< -- FOR DEBUG 用户参数

		LOGIN_SELECT_SERVER,			// 选择服务器界面.
		LOGIN_DISCONNECT,				//!< 尚未登录
		
		LOGIN_CONNECTING,				//!< 连接服务器中...
		LOGIN_CONNECTED_OK,				//!< 成功连接到服务器
		LOGIN_CONNECT_FAILED,			//!< 连接到服务器失败

		LOGIN_ACCOUNT_BEGIN_REQUESTING,	// 发送密码之前状态
		LOGIN_ACCOUNT_REQUESTING,		//!< 发送帐号信息数据包到服务器...
		LOGIN_ACCOUNT_OK,				//!< 帐号验证成功
		LOGIN_ACCOUNT_FAILED,			//!< 帐号验证失败

		LOGIN_WAIT_FOR_LOGIN,			// 排队进入游戏状态.

		LOGIN_FIRST_LOGIN,				// 首次登录
		LOGIN_CHANGE_SCENE,				// 切换场景的重登录
	};

	PLAYER_LOGIN_STATUS		GetStatus(VOID) { return m_Status; }
	VOID					SetStatus(PLAYER_LOGIN_STATUS status) { m_Status = status; }
	VOID					SetRelogin(BOOL bReLogin) { m_bReLogin = bReLogin; }

	// 设置登录状态为首次登录（以区分切换场景的登录状态）
	VOID					FirstLogin(VOID);

	// 设置登录状态为切换场景的登录状态
	VOID					LoginForChangeScene(VOID);

	//-----------------------------------------------------------------------------------------------------------
	//
	// 从配置文件得到, login server 的ip地址和端口号
	//
	//

	// login server 的ip地址和端口号.
	TCHAR	m_szLoginServerAddr[64];
	INT		m_nLoginServerPort;

	// 设置端口号.
	void SetPort(INT iPort);

	// 设置ip地址
	void SetIpAddr(LPCTSTR szServerAddr);


	// 区域信息
	vector<AREA_INFO> m_pAreaInfo;

	// 区域个数
	int			m_iAreaCount;


	// 读取服务器配置文件的错误信息状态
	enum LOAD_LOGIN_SERVER_ERR
	{
		LOAD_LOGIN_SERVER_SUCCESS = 0,		// 读取成功.
		LOAD_LOGIN_SERVER_FILE_NOT_FINDED,	// 找不到文件
		LOAD_LOGIN_SERVER_AREA_NOT_FINDED,	// 没有区域
		LOAD_LOGIN_SERVER_OTHER_ERR,		// 其他错误

	};


	// 读取login server信息
	//
	// 返回值: 
	//			0 -- 读取成功
	//			1 -- 失败找不到配置文件
	//          2 -- 其他错误    
	int  LoadLoginServerInfo(std::string strLoginServerInfoFile = "..\\Patch\\WXLoginServer.txt");

	// 读取区域信息.
	//int ReadAreaInfo(std::string& strLoginServer, int iAreaIndex);

	// 读取login server信息.
	//int ReadLoginInfo(FILE* pFile, int iAreaIndex, int iLoginServerIndex);

	// 初始化区域信息
	void InitLoginAreaInfo();

	//
	// 
	// 从配置文件得到, login server 的ip地址和端口号
	//
	//-----------------------------------------------------------------------------------------------------------------------------
	



	//-----------------------------------------------------------------------------------------------------------------------------
	//
	// 逻辑数据操作
	//
	//

	// 得到区域的个数
	int GetAreaCount();

	// 得到区域名字
	const TCHAR* GetAreaName(int iAreaIndex);

	// 得到区域信息
	PAREA_INFO GetAreaInfo(int iAreaIndex);

	// 得到区域中login server 的个数
	int GetAreaLoginServerCount(int iAreaIndex);

	// 得到区域中login server 的信息.
	PLOGINSERVER_INOF GetAreaLoginServerInfo(int iAreaIndex, int iLoginServerIndex);

	// 连接到login server
	int ConnectToLoginServer(int iAreaIndex, int iLoginServerIndex);

	// 连接到login server
	int ConnectToLoginServer();

	// 当前选择的区域id
	int m_iCurSelAreaId;

	// 当前选择的login server id
	int m_iCurSelLoginServerId;

	// 记录选择的服务器
	int SelectLoginServer(int iAreaIndex, int iLoginServerIndex);

	// 在界面上选择旧的服务器
	VOID SelectOldServer();



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 自动选择区域数据
	//

	// 当前选择的区域
	int m_iAutoAreaIndex[PROVIDE_COUNT];

	// 当前选择的login server 索引
	int m_iAutoLoginServerIndex[PROVIDE_COUNT];


	// 前一次找到网络状态空闲的索引.
	int m_iPreFindIdle;

	// 前一次找到网络状态正常的索引.
	int m_iPreFindNor;

	// 前一次找到网络状态拥挤的索引.
	int m_iPreFindBusy;

	// 前一次找到网络状态爆满的索引.
	int m_iPreFindFull;

	// 电信供应商
	PROVIDE_INFO m_Provide0;

	// 网通供应商
	PROVIDE_INFO m_Provide1;

	// 其他供应商
	PROVIDE_INFO m_Provide2;

	// 通过网络供应商选择一个login server
	int AutoSelLoginServer(int iProvide);

	// 构造自动选择login server需要的数据表
	int BuildAutoSelTable(); 


	// 
	//  逻辑数据操作
	//
	//-------------------------------------------------------------------------------------------------------------------------------




	//-------------------------------------------------------------------------------------------------------------------------------
	//
	// 界面操作
	//

	// 打开帐号输入界面
	int OpenCountInputDlg();

	// 打开服务器选择界面
	int OpenSelectServerDlg();

	// 打开人物选择界面
	int OpenSelectCharacter();

	// 切换到人物选择界面
	int ChangeToCharacterSel();

	//
	// 界面操作
	//
	//-------------------------------------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------------------------------------
	//
	// 网络命令消息操作
	//

	// 发送同步消息
	int SendClConnectMsg();

	// 用户名
	std::string m_strUserName;

	// 密码
	std::string m_strPassword;

	// 检查用户名和密码.
	int CheckAccount(CHAR* szUserName, CHAR* szPassword);

	// 发送check msg 消息
	int SendCheckAccountMsg();

	//
	// 网络命令消息操作
	//
	//-------------------------------------------------------------------------------------------------------------------------------


public:
	//处理输入
	virtual VOID	ProcessInput(VOID);

protected:
	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);

protected:

	// 与服务器的网络连接状态.
	PLAYER_LOGIN_STATUS	m_Status;
	//是否是从从新连接新的服务器
	BOOL				m_bReLogin;
	//背景音乐
	tSoundSource*		m_pSoundSource;

public:
	CGamePro_Login(VOID);
	virtual ~CGamePro_Login(VOID);
};

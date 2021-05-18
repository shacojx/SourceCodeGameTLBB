
#ifndef __SCENEPLAYERMANAGER_H__
#define __SCENEPLAYERMANAGER_H__


#include "Type.h"
#include "PlayerManager.h"



//场景中的玩家群管理模块
//
class ScenePlayerManager : public PlayerManager
{
public :
	ScenePlayerManager( ) ;
	~ScenePlayerManager( ) ;

	//网络侦测接口
	BOOL				Select( ) ;
	//数据接收处理
	BOOL				ProcessInputs( ) ;
	//数据发送处理
	BOOL				ProcessOutputs( ) ;
	//异常连接处理
	BOOL				ProcessExceptions( ) ;
	//消息处理
	BOOL				ProcessCommands( ) ;
	//逻辑处理
	virtual BOOL		HeartBeat( UINT uTime=0 ) ;


public :
	//通用接口

	//设置当前场景ID
	VOID				SetSceneID( SceneID_t SceneID ){ m_SceneID = SceneID ; } ;
	//读取场景ID
	SceneID_t			GetSceneID( ){ return m_SceneID ; } ;

	//增加一个玩家
	BOOL				AddPlayer( Player* pPlayer ) ;
	BOOL				AddPlayerSocket( SOCKET fd ) ;
	//删除一个玩家（并不断网，有可能转移到别的场景）
	BOOL				DelPlayer( PlayerID_t pid ) ;
	BOOL				DelPlayerSocket( SOCKET fd ) ;

	//断开一个玩家、并清理数据
	BOOL				RemovePlayer( Player* pPlayer, char* szReason, BOOL bReal=FALSE ) ;
	VOID				RemoveAllPlayer( ) ;


	INT					GetCount( ){ return m_nFDSize ; } ;


	VOID				Lock(){ m_Lock.Lock() ; } ;
	VOID				Unlock(){ m_Lock.Unlock() ; } ;

private :
	//
	//网络数据
	enum{
		SELECT_BAK = 0,	//当前系统中拥有的完整句柄数据
		SELECT_USE = 1,	//用于select调用的句柄数据
		SELECT_MAX = 2, //结构使用数量
	};
	fd_set		m_ReadFDs[SELECT_MAX];
	fd_set		m_WriteFDs[SELECT_MAX];
	fd_set		m_ExceptFDs[SELECT_MAX];

	timeval		m_Timeout[SELECT_MAX];

	SOCKET		m_MinFD;
	SOCKET		m_MaxFD;

	INT				m_nFDSize ;
	//网络数据
	//

	SceneID_t	m_SceneID ;

	MyLock		m_Lock ;

//
/////////////////////////////////////////////////////////////////////////////////
public :



};



#endif

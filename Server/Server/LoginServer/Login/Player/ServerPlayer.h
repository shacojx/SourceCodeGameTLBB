


#ifndef __SERVERPLAYER_H__
#define __SERVERPLAYER_H__

#include "Type.h"
#include "Player.h"
#include "Config.h"


//服务器玩家指的是另外的服务器端程序在当前程序内的数据表现

class ServerPlayer : public Player
{
public :
	ServerPlayer( BOOL bIsServer=TRUE ) ;
	~ServerPlayer( ) ;

	//数据接收
	virtual BOOL	ProcessInput( ) ;
	//数据发送
	virtual BOOL	ProcessOutput( ) ;
	//消息处理
	virtual BOOL	ProcessCommand( BOOL Option = TRUE ) ;

	//清除
	virtual VOID	CleanUp( ) ;

	//逻辑接口
	virtual BOOL	HeartBeat( UINT uTime=0 ) ;


public :
	//通用接口

	virtual BOOL	IsLoginPlayer( ){ return FALSE ; } ;
	virtual BOOL	IsServerPlayer( ){ return TRUE ; } ;

	//设置当前服务器端玩家的状态
	VOID			SetPlayerStatus( uint status ){ m_Status = status ; } ;

	//判断当前服务器端玩家是否有效
	virtual BOOL	IsValid( ) ;

	//向此服务器端程序发送消息
	//此接口不支持线程间同步，因此只能有当前线程来调用执行
	virtual BOOL	SendPacket( Packet* pPacket ) ;

	virtual VOID	Encrypt_SC(CHAR* header, UINT uLen, UINT uBeginPlace){;}

	virtual VOID	DecryptHead_CS(CHAR* header){;}

	virtual VOID	Decrypt_CS(CHAR* header, UINT uLen, UINT uBeginPlace){;}
private :
	//服务器端状态
	uint			m_Status ;


};




#endif

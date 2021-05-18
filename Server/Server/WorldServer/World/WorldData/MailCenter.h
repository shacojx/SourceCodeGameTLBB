
/////////////////////////////////////////////////////////////////////////////////
//文件名：MailCenter.h
//功能描述：处理世界里邮件功能
//数据结构说明：
//
// UserNode->MailNode->MailNode->MailNode......			MAIL_Pool ->MailNode[0]
// |         |_MAIL	   |_MAIL    |_MAIL								MailNode[1]
// |																MailNode[2]
// UserNode->MailNode->MailNode->MailNode......						...
// |         |_MAIL	   |_MAIL    |_MAIL
// |
// UserChain
//
// UserHashTable->Name(GUID) to UserNode[?]
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef __MAILCENTER_H__
#define __MAILCENTER_H__

#include "Type.h"
#include "MailDefine.h"


class USER ;

class MailCenter
{
public :
	MailCenter( ) ;
	~MailCenter( ) ;

	BOOL				Init( ) ;
	VOID				CleanUp( ) ;

	BOOL				HeartBeat( UINT uTime=0 ) ;


public :
//邮件处理接口
	
	//系统创建时注册邮件节点
	UINT				RegisterMailNode(MailNode* pMailNode);

	//向发送mail给目标（目标信息在mail数据里），返回值为用户当前用后的邮件数量
	UINT				MailTo( const MAIL* pMail ) ;
	//请求名字叫szName玩家的邮件信息，pList返回邮件列表, 返回值为邮件数量
	UINT				AskMail( const CHAR* szName, MAIL_LIST* pListOut ) ;
	//检查名字叫szName玩家是否有邮件
	UINT				CheckMail( const CHAR* szName ) ;
	//检查玩家是否有邮件
	UINT				CheckMail( USER* pUser ) ;
	//请求名字叫szName玩家的邮件信息，pList返回邮件列表, 返回值为邮件数量
	UINT				AskScriptMail( const CHAR* szName, MAIL_LIST* pListOut ) ;

public :
//邮件事件相应接口
	//玩家接收到新邮件
	VOID				OnUserRecvMail( const MAIL_LIST* pList ) ;
	//玩家发送了邮件
	VOID				OnUserSendMail( const MAIL* pMail ) ;

	// 发送普通邮件
	VOID				SendNormalMail(const CHAR* szReceiverName, const CHAR* szContent);

	// 发送可执行邮件
	VOID				SendScriptMail(const CHAR* szReceiverName, UINT uParam0 = 0, UINT uParam1 = 0, UINT uParam2 = 0, UINT uParam3 = 0 );

protected :
//内部功能接口
	//UINT				MailToOnlineUser( MAIL* pMail, USER* pSender, USER* pRecver ) ;
	//UINT				MailToOfflineUser( MAIL* pMail, USER* pSender, USER* pRecver ) ;

	// 供 SendNormalMail、SendScriptMail 调用
	VOID				SendMail(const MAIL* pMail);

protected :
	UserHashTable		m_UserHashTable ;
	UserChain			m_UserChain ;

};

extern MailCenter*	g_pMailCenter ;


#endif

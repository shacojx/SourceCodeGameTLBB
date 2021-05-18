
/////////////////////////////////////////////////////////////////////////////////
//文件名：MailPool.h
//功能描述：邮件数据及用户节点数据池
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __MAILPOOL_H__
#define __MAILPOOL_H__


#include "Type.h"
#include "MailDefine.h"

//没节的邮件数
#define MAX_MAILNODE_SIZE 102400

//用户节点数量
#define MAX_USERNODE_SIZE 4096


//邮件数据池
class MailNodePool
{
public :
	MailNodePool( ) ;
	~MailNodePool( ) ;

	BOOL				Init( UINT MaxMailNode = MAX_MAILNODE_SIZE ) ;
	VOID				CleanUp( ) ;

	MailNode*			NewNode( ) ;
	VOID				DeleteNode( MailNode* pNode ) ;
	
	MailNode*			GetNode(UINT Index);
protected :
	MailNode*			m_pMailNode ;
	
	INT					m_nMailNodeNumber ;
	INT					m_nCurrentOffset ;
};

extern MailNodePool* g_pMailNodePool ;


//用户节点池
class UserNodePool
{
public :
	UserNodePool( ) ;
	~UserNodePool( ) ;

	BOOL				Init( ) ;
	VOID				CleanUp( ) ;

	UserNode*			NewNode( ) ;
	VOID				DeleteNode( UserNode* pNode ) ;

protected :
	UserNode*			m_pUserNode ;

	INT					m_nUserNodeNumber ;
	INT					m_nCurrentOffset ;

};

extern UserNodePool* g_pUserNodePool ;

#endif

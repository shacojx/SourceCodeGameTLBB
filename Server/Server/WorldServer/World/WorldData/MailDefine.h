
/////////////////////////////////////////////////////////////////////////////////
//文件名：MailDefine.h
//功能描述：定义邮件处理模块的数据结构
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __MAILDEFINE_H__
#define __MAILDEFINE_H__

#include "Type.h"
#include "Chain.h"
#include "StrTable.h"

struct MailSMU;

//邮件节点
class MailNode : public CChainItem
{
public :
	MailNode( ) ;
	virtual ~MailNode( ) ;

	VOID			CleanUp( ) ;

	const MAIL*		GetMail( ){ return &m_Mail ; } ;
	VOID			SetMail( const MAIL* pMail ) ;
	VOID			ValidateShareMem();

public :
	MAIL			m_Mail ;
	MailSMU*		m_pMailSMU;
	INT				m_nPoolIndex ;
	INT				m_nUserNodePoolIndex ;//挂接在此用户节点名下
};

//用户节点
class UserNode : public CChainItem
{
public :
	UserNode( ) ;
	virtual ~UserNode( ) ;

	VOID			CleanUp( ) ;

	INT				GetMailCount( ) ;//返回此用户的邮件数量
	INT				GetScriptMailCount( ) ;//返回此用户的邮件数量


public :
	CChainHeader	m_ChainMail ;
	CChainHeader	m_ChainScriptMail ;
	INT				m_nPoolIndex ;
	UINT			m_uTickCount ;
	CHAR			m_szName[MAX_CHARACTER_NAME] ;

};

//用户链表
class UserChain
{
public :
	UserChain( ) ;
	~UserChain( ) ;

	BOOL			Init( ) ;
	VOID			CleanUp( ) ;

	INT				GetUserCount( ) ;//返回此用户的邮件数量

	UserNode*		GetHeadNode( ) ;
	UserNode*		GetPrevNode( ) ;
	UserNode*		GetNextNode( ) ;
	UserNode*		GetTailNode( ) ;

public :
	CChainHeader	m_ChainUser ;

};

//用户名字(GUID)和用户节点的Hash表
class UserHashTable
{
public :
	UserHashTable( ) ;
	~UserHashTable( ) ;

	VOID			CleanUp( ) ;
	BOOL			Init( ) ;

	BOOL			Add( CHAR* szName, UserNode* pNode ) ;
	VOID			Del( CHAR* szName ) ;
	UserNode*		Get( CHAR* szName ) ;

protected :
	StrTable		m_NameTable ;

};




#endif

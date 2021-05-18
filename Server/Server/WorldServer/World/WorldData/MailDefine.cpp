#include "stdafx.h"

#include "MailDefine.h"
#include "Config.h"
#include "MailPool.h"
#include "DB_Struct.h"
#include "ShareMemManager.h"
#include "SMUManager.h"

/////////////////////////////////////////////////////////////////////////////////
//
MailNode::MailNode( )
{
__ENTER_FUNCTION

	m_nPoolIndex = INVALID_INDEX ;
	
	_WORLD_INFO& WorldInfo = g_Config.m_WorldInfo;
	if(WorldInfo.m_EnableShareMem)
	{
		m_pMailSMU = g_MailSMUPool.NewObj();
		Assert(m_pMailSMU);
		memcpy(&m_Mail,&(m_pMailSMU->m_MailSM),sizeof(m_Mail));
	}
	else
	{
		m_pMailSMU = NULL;
	}

	

__LEAVE_FUNCTION
}

MailNode::~MailNode( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

VOID MailNode::CleanUp( )
{
__ENTER_FUNCTION

	m_Mail.CleanUp( ) ;
	if(m_pMailSMU)
	{
			ValidateShareMem();
	}
	m_nPoolIndex = INVALID_INDEX ;
	m_nUserNodePoolIndex = INVALID_INDEX ;
	
__LEAVE_FUNCTION
}

VOID MailNode::SetMail( const MAIL* pMail )
{
	m_Mail = *pMail ;
	ValidateShareMem();
}

VOID MailNode::ValidateShareMem()
{
	__ENTER_FUNCTION
	if(m_pMailSMU )
	{
		m_pMailSMU->Lock(SM_W_WRITE);
		memcpy(&(m_pMailSMU->m_MailSM),&m_Mail,sizeof(m_Mail));
		m_pMailSMU->UnLock(SM_W_WRITE);
	}
	__LEAVE_FUNCTION
	
	if(m_pMailSMU)
		m_pMailSMU->UnLock(SM_W_WRITE);
}

/////////////////////////////////////////////////////////////////////////////////
//
UserNode::UserNode( )
{
__ENTER_FUNCTION

	m_nPoolIndex = INVALID_INDEX ;
	m_ChainMail.OutAll() ;
	m_uTickCount = 0 ;
	memset( m_szName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME ) ;

__LEAVE_FUNCTION
}

UserNode::~UserNode( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

VOID UserNode::CleanUp( )
{
__ENTER_FUNCTION

	m_nPoolIndex = INVALID_INDEX ;
	m_ChainMail.Last() ;
	while( m_ChainMail.GetNum()>0)
	{
		MailNode* pMailNode = (MailNode*)(m_ChainMail.OutChain()) ;
		g_pMailNodePool->DeleteNode( pMailNode ) ;
	};
	m_uTickCount = 0 ;
	memset( m_szName, 0, MAX_CHARACTER_NAME*sizeof(CHAR) ) ;
	

__LEAVE_FUNCTION
}

INT UserNode::GetMailCount( )//返回此用户的邮件数量
{
__ENTER_FUNCTION

	return (INT)(m_ChainMail.GetNum()) ;

__LEAVE_FUNCTION

	return 0 ;
}

INT UserNode::GetScriptMailCount( )//返回此用户的邮件数量
{
__ENTER_FUNCTION

	return (INT)(m_ChainScriptMail.GetNum()) ;

__LEAVE_FUNCTION

	return 0 ;
}
/////////////////////////////////////////////////////////////////////////////////
//
UserChain::UserChain( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

UserChain::~UserChain( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL UserChain::Init( )
{
__ENTER_FUNCTION

	Assert( m_ChainUser.GetNum()==0 ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID UserChain::CleanUp( ) 
{
__ENTER_FUNCTION

	m_ChainUser.First() ;
	while( m_ChainUser.GetNum()>0 )
	{
		UserNode* pUserNode = (UserNode*)(m_ChainUser.OutChain()) ;
		pUserNode->CleanUp() ;
	};

__LEAVE_FUNCTION
}

/////////////////////////////////////////////////////////////////////////////////
//
UserHashTable::UserHashTable( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

UserHashTable::~UserHashTable( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL UserHashTable::Init( )
{
__ENTER_FUNCTION

	m_NameTable.InitTable( g_Config.m_ConfigInfo.m_nHashMailUserCount, MAX_CHARACTER_NAME ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID UserHashTable::CleanUp( )
{
__ENTER_FUNCTION

	m_NameTable.CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL UserHashTable::Add( CHAR* szName, UserNode* pNode )
{
__ENTER_FUNCTION

	m_NameTable.Add( szName, pNode ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID UserHashTable::Del( CHAR* szName )
{
__ENTER_FUNCTION

	m_NameTable.Remove( szName ) ;

__LEAVE_FUNCTION
}

UserNode* UserHashTable::Get( CHAR* szName )
{
__ENTER_FUNCTION

	return (UserNode*)(m_NameTable.Get(szName)) ;

__LEAVE_FUNCTION

	return NULL ;
}



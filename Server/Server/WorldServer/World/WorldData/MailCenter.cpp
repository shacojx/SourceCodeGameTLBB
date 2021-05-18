#include "stdafx.h"

#include "MailCenter.h"
#include "OnlineUser.h"
#include "MailPool.h"
#include "Config.h"
#include "OnlineUser.h"
#include "TimeManager.h"

const UINT uMaxHoldMailTick = 864000 ;//服务器可以保存邮件的最常时间

MailCenter*	g_pMailCenter = NULL ;

MailCenter::MailCenter( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

MailCenter::~MailCenter( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL MailCenter::Init( )
{
__ENTER_FUNCTION

	BOOL bRet = m_UserHashTable.Init( ) ;
	if( bRet==FALSE )
		return FALSE ;

	bRet = m_UserChain.Init( ) ;
	if( bRet==FALSE )
		return FALSE ;

	_WORLD_INFO& WorldInfo = g_Config.m_WorldInfo;

	if(WorldInfo.m_EnableShareMem)
	{
		MailNode* pNode;

		for(UINT i=0;i<MAX_MAILNODE_SIZE;i++)
		{
			pNode = g_pMailNodePool->GetNode(i);		
			if(pNode)
			{
				RegisterMailNode(pNode);
			}
		}
	}
	

	return bRet ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID MailCenter::CleanUp( )
{
__ENTER_FUNCTION

	m_UserHashTable.CleanUp( ) ;
	m_UserChain.CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL MailCenter::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	UserNode* pUserNode = (UserNode*)(m_UserChain.m_ChainUser.First()) ;
	while( pUserNode!=NULL )
	{
		if( pUserNode->m_ChainMail.GetNum() <= 0 )
		{//如果用户节点中没有邮件，则回收用户节点

			m_UserHashTable.Del( pUserNode->m_szName ) ;
			m_UserChain.m_ChainUser.OutChain() ;
			g_pUserNodePool->DeleteNode( pUserNode ) ;

			pUserNode = (UserNode*)(m_UserChain.m_ChainUser.Current()) ;
			continue ;
		}

		if( pUserNode->m_uTickCount > uMaxHoldMailTick )
		{//如果用户节点在服务器保留的很长时间，则回收用户节点数据
		 //并且回收当前用户节点所关联的所有邮件节点数据

			pUserNode->m_ChainMail.First() ;
			while( pUserNode->m_ChainMail.GetNum()>0 )
			{
				MailNode* pMailNode = (MailNode*)(pUserNode->m_ChainMail.OutChain()) ;
				g_pMailNodePool->DeleteNode( pMailNode ) ;
			};

			m_UserHashTable.Del( pUserNode->m_szName ) ;
			m_UserChain.m_ChainUser.OutChain() ;
			g_pUserNodePool->DeleteNode( pUserNode ) ;

			pUserNode = (UserNode*)(m_UserChain.m_ChainUser.Current()) ;
			continue ;
		}
		
		pUserNode->m_uTickCount ++ ;
		pUserNode = (UserNode*)(m_UserChain.m_ChainUser.Next()) ;
	};




	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT MailCenter::RegisterMailNode(MailNode* pMailNode)
{
	__ENTER_FUNCTION

	if(!pMailNode)
	{
		Assert(FALSE);
		return 0;
	}

	const MAIL* pMail = pMailNode->GetMail();
	if(pMail->m_GUID == INVALID_ID)
	{
		return 0;
	}

	UserNode* pUserNode = NULL ;
	CHAR szDestName[MAX_CHARACTER_NAME];
	
	memset(szDestName, 0, MAX_CHARACTER_NAME);
	strncpy(szDestName, pMail->m_szDestName, MAX_CHARACTER_NAME - 1);
	pUserNode = m_UserHashTable.Get( szDestName );

	if( pUserNode == NULL )
	{//当前没有目标玩家的邮件数据
		pUserNode = g_pUserNodePool->NewNode( ) ;//分配一个用户节点

		strncpy( pUserNode->m_szName, pMail->m_szDestName, MAX_CHARACTER_NAME-1 ) ;
		//将用户节点注册到Hash表里
		m_UserHashTable.Add( pUserNode->m_szName, pUserNode ) ;
		//将用户节点添加到节点链表里
		m_UserChain.m_ChainUser.Add( pUserNode ) ;
	}
	pMailNode->m_nUserNodePoolIndex = pUserNode->m_nPoolIndex ;//邮件节点中保存所在用户节点值

	//根据邮件类型，将邮件节点添加到用户的邮件列表里
	switch( pMail->m_uFlag )
	{
	case MAIL_TYPE_SCRIPT:
		{
			pUserNode->m_ChainScriptMail.Add( pMailNode ) ;
		}
		break ;
	case MAIL_TYPE_NORMAL:
	case MAIL_TYPE_SYSTEM:
	case MAIL_TYPE_NORMAL2:
	default :
		{
			if ( pUserNode->m_ChainMail.GetNum() >= MAX_MAIL )
			{ // 如果信件超过 MAX_MAIL 条，则删掉
				//将邮件从链表里断开
				MailNode* pOldMailNode = (MailNode*)(pUserNode->m_ChainMail.OutChain());
				if( pOldMailNode!=NULL )
				{
					//删除邮件节点
					g_pMailNodePool->DeleteNode( pOldMailNode );
				}
				else
				{
					Assert( FALSE );
				}
			}

			pUserNode->m_ChainMail.Add( pMailNode ) ;
		}
		break ;
	};


	//如果此用户节点里保存的邮件数量超出最大值，则处理
	//...未定
	OnUserSendMail( pMail ) ;

	return pUserNode->GetMailCount()+pUserNode->GetScriptMailCount() ;

	__LEAVE_FUNCTION

		return 0 ;
}

//向发送mail给目标（目标信息在mail数据里）
UINT MailCenter::MailTo( const MAIL* pMail )
{
__ENTER_FUNCTION

	UserNode* pUserNode = NULL ;
	CHAR szDestName[MAX_CHARACTER_NAME];

	memset(szDestName, 0, MAX_CHARACTER_NAME);
	strncpy(szDestName, pMail->m_szDestName, MAX_CHARACTER_NAME - 1);
	pUserNode = m_UserHashTable.Get( szDestName );

	if( pUserNode == NULL )
	{//当前没有目标玩家的邮件数据
		pUserNode = g_pUserNodePool->NewNode( ) ;//分配一个用户节点

		strncpy( pUserNode->m_szName, pMail->m_szDestName, MAX_CHARACTER_NAME-1 ) ;
		//将用户节点注册到Hash表里
		m_UserHashTable.Add( pUserNode->m_szName, pUserNode ) ;
		//将用户节点添加到节点链表里
		m_UserChain.m_ChainUser.Add( pUserNode ) ;
	}

	MailNode* pMailNode = g_pMailNodePool->NewNode()  ;//分配一个邮件节点
	pMailNode->SetMail( pMail ) ;//复制邮件数据
	pMailNode->m_nUserNodePoolIndex = pUserNode->m_nPoolIndex ;//邮件节点中保存所在用户节点值

	//根据邮件类型，将邮件节点添加到用户的邮件列表里
	switch( pMail->m_uFlag )
	{
	case MAIL_TYPE_SCRIPT:
		{
			pUserNode->m_ChainScriptMail.Add( pMailNode ) ;
		}
		break ;
	case MAIL_TYPE_NORMAL:
	case MAIL_TYPE_SYSTEM:
	case MAIL_TYPE_NORMAL2:
	default :
		{
			if ( pUserNode->m_ChainMail.GetNum() > MAX_MAIL )
			{ // 如果信件超过 MAX_MAIL 条，则删掉
				//将邮件从链表里断开
				MailNode* pOldMailNode = (MailNode*)(pUserNode->m_ChainMail.OutChain());
				if( pOldMailNode!=NULL )
				{
					//删除邮件节点
					g_pMailNodePool->DeleteNode( pOldMailNode );
				}
				else
				{
					Assert( FALSE );
				}
			}

			pUserNode->m_ChainMail.Add( pMailNode ) ;
		}
		break ;
	};


	//如果此用户节点里保存的邮件数量超出最大值，则处理
	//...未定

	OnUserSendMail( pMail ) ;

	return pUserNode->GetMailCount()+pUserNode->GetScriptMailCount() ;

__LEAVE_FUNCTION

	return 0 ;
}

//UINT MailCenter::MailToOnlineUser( MAIL* pMail, USER* pSender )
//{
//__ENTER_FUNCTION
//
//
//	return TRUE ;
//
//__LEAVE_FUNCTION
//
//	return FALSE ;
//}
//
//UINT MailCenter::MailToOfflineUser( MAIL* pMail, USER* pSender, USER* pRecver )
//{
//__ENTER_FUNCTION
//
//
//	return TRUE ;
//
//__LEAVE_FUNCTION
//
//	return FALSE ;
//}


//请求名字叫szName玩家的邮件信息，pList返回邮件列表, 返回值为邮件数量
UINT MailCenter::AskMail( const CHAR* szName, MAIL_LIST* pListOut )
{
__ENTER_FUNCTION
	
	if( pListOut==NULL || szName==NULL )
		return 0 ;

	pListOut->CleanUp( ) ;

	CHAR szTemp[MAX_CHARACTER_NAME];
	memset((void*)szTemp, 0, MAX_CHARACTER_NAME);
	strncpy(szTemp, szName, MAX_CHARACTER_NAME-1);

	UserNode* pUserNode = m_UserHashTable.Get( szTemp ) ;
	if( pUserNode == NULL )
	{
		return 0 ;
	}

	pUserNode->m_ChainMail.First() ;
	while( pUserNode->m_ChainMail.GetNum()>0 )
	{
		//将邮件从链表里断开
		MailNode* pMailNode = (MailNode*)(pUserNode->m_ChainMail.OutChain()) ;
		if( pMailNode==NULL )
		{
			Assert( FALSE ) ;
			break ;
		}
		//将邮件信息存到输出结构里
		pListOut->m_aMail[pListOut->m_Count] = *pMailNode->GetMail() ;
		pListOut->m_Count++ ;
		//删除邮件节点
		g_pMailNodePool->DeleteNode( pMailNode ) ;

		//返回一封邮件
		break ;
	}
	//将此用户还拥有的邮件数量存入输出结构
	pListOut->m_TotalLeft = pUserNode->m_ChainMail.GetNum() ;

	OnUserRecvMail( pListOut ) ;

	return pListOut->m_Count ;

__LEAVE_FUNCTION

	return 0 ;
}

//搜索所有普通邮件，查看有效的邮件数量
UINT MailCenter::CheckMail( USER* pUser )
{
	__ENTER_FUNCTION
	if( pUser == NULL ) return 0;

	CHAR szTemp[MAX_CHARACTER_NAME];
	memset((void*)szTemp, 0, MAX_CHARACTER_NAME);
	strncpy(szTemp, pUser->GetName(), MAX_CHARACTER_NAME-1);

	UserNode* pUserNode = m_UserHashTable.Get( szTemp );
	if( pUserNode == NULL )
	{
		return 0 ;
	}

	UINT nMailCount = 0;
	MailNode* pMailNode = (MailNode*)pUserNode->m_ChainMail.First();
	for( UINT i = 0; i < pUserNode->m_ChainMail.GetNum(); i ++ )
	{
		if( pMailNode == NULL )
		{
			Assert( FALSE ) ;
			break;
		}

		if( pUser->ValidateMail( pMailNode->GetMail() ) == 0 )
		{
			//有效邮件
			nMailCount ++;
			pMailNode = (MailNode*)pUserNode->m_ChainMail.Next();
		}
		else
		{
			//无效邮件，删除
			pUserNode->m_ChainMail.OutChain();
			g_pMailNodePool->DeleteNode( pMailNode ) ;

			//取得当前节点
			pMailNode = (MailNode*)pUserNode->m_ChainMail.Current();
		}
	}

	return nMailCount;
	__LEAVE_FUNCTION

	return 0 ;
}

UINT MailCenter::CheckMail( const CHAR* szName )
{
__ENTER_FUNCTION
	
	if( szName==NULL )
		return 0 ;

	CHAR szTemp[MAX_CHARACTER_NAME];
	memset((void*)szTemp, 0, MAX_CHARACTER_NAME);
	strncpy(szTemp, szName, MAX_CHARACTER_NAME-1);

	UserNode* pUserNode = m_UserHashTable.Get( szTemp ) ;
	if( pUserNode == NULL )
	{
		return 0 ;
	}

	return pUserNode->GetMailCount() ;

__LEAVE_FUNCTION

	return 0 ;
}

//请求名字叫szName玩家的邮件信息，pList返回邮件列表, 返回值为邮件数量
UINT MailCenter::AskScriptMail( const CHAR* szName, MAIL_LIST* pListOut )
{
__ENTER_FUNCTION
	
	if( pListOut==NULL || szName==NULL )
		return 0 ;

	pListOut->CleanUp( ) ;

	CHAR szTemp[MAX_CHARACTER_NAME];
	memset((void*)szTemp, 0, MAX_CHARACTER_NAME);
	strncpy(szTemp, szName, MAX_CHARACTER_NAME-1);

	UserNode* pUserNode = m_UserHashTable.Get( szTemp );
	if( pUserNode == NULL )
	{
		return 0 ;
	}

	pUserNode->m_ChainScriptMail.First() ;
	while( pUserNode->m_ChainScriptMail.GetNum()>0 )
	{
		//将邮件从链表里断开
		MailNode* pMailNode = (MailNode*)(pUserNode->m_ChainScriptMail.OutChain()) ;
		if( pMailNode==NULL )
		{
			Assert( FALSE ) ;
			break ;
		}
		//将邮件信息存到输出结构里
		pListOut->m_aMail[pListOut->m_Count] = *pMailNode->GetMail() ;
		pListOut->m_Count++ ;
		//删除邮件节点
		g_pMailNodePool->DeleteNode( pMailNode ) ;

		if( pListOut->m_Count>=MAX_MAIL_SIZE )
			break ;
	}
	//将此用户还拥有的邮件数量存入输出结构
	pListOut->m_TotalLeft = pUserNode->m_ChainScriptMail.GetNum() ;

	OnUserRecvMail( pListOut ) ;

	return pListOut->m_Count ;

__LEAVE_FUNCTION

	return 0 ;
}

//玩家接收到新邮件
VOID MailCenter::OnUserRecvMail( const MAIL_LIST* pList )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

//玩家发送了邮件
VOID MailCenter::OnUserSendMail( const MAIL* pMail )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

// 发送邮件
VOID MailCenter::SendNormalMail( const CHAR* szReceiverName, const CHAR* szContent)
{
__ENTER_FUNCTION

	Assert( szReceiverName );
	Assert( szContent );

	MAIL mail;

	mail.m_uFlag = MAIL_TYPE_NORMAL2;
	mail.m_DestSize = (BYTE)strlen( szReceiverName );
	strncpy( mail.m_szDestName, szReceiverName, MAX_CHARACTER_NAME-1 );
	mail.m_ContexSize = (BYTE)strlen( szContent );
	strncpy( mail.m_szContex, szContent, MAX_MAIL_CONTEX-1 );

	mail.m_uCreateTime = g_pTimeManager->GetANSITime();

	SendMail(&mail);

__LEAVE_FUNCTION
}

// 发送可执行邮件
VOID MailCenter::SendScriptMail(const CHAR* szReceiverName, UINT uParam0, UINT uParam1, UINT uParam2, UINT uParam3 )
{
__ENTER_FUNCTION

	Assert( szReceiverName );

	MAIL mail;

	mail.m_uFlag = MAIL_TYPE_SCRIPT;
	mail.m_DestSize = (BYTE)strlen( szReceiverName );
	strncpy( mail.m_szDestName, szReceiverName, MAX_CHARACTER_NAME-1 );
	mail.m_uParam0 = uParam0;
	mail.m_uParam1 = uParam1;
	mail.m_uParam2 = uParam2;
	mail.m_uParam3 = uParam3;

	SendMail(&mail);

__LEAVE_FUNCTION
}

// 供 SendNormalMail、SendScriptMail 调用
VOID MailCenter::SendMail(const MAIL* pMail)
{
__ENTER_FUNCTION

	Assert( pMail );

	USER* pUser = g_pOnlineUser->FindUser( pMail->m_szDestName );
	if( pUser )
	{//当前用户在线
		pUser->RecvMail( pMail );
	}
	else
	{
		MailTo( pMail );
	}

__LEAVE_FUNCTION
}

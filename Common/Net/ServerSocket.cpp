#include "stdafx.h"
#include "KeyDog.h"


//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "ServerSocket.h"


ServerSocket::ServerSocket ( UINT port , UINT backlog ) 
{
	__ENTER_FUNCTION_FOXNET

		BOOL ret = FALSE ;

#define KEY_MAC 12
#ifdef __WINDOWS__

//#ifndef GAME_CLIENT
//	char lpszMac[128] = {0};
//	char lpszKey[128] = {0};
//	Ini ini( FILE_SERVER_INFO ) ;
//	ini.ReadText( "KeyDog", "Key", lpszKey, sizeof(lpszKey) ) ;
//	GetMacByCmd(lpszMac);
//	int uret = strncmp(lpszKey, lpszMac,KEY_MAC);
//	if (0 != uret)
//	{
//		while (1)
//		{
//			MessageBox(NULL,"ÐòÁÐºÅ´íÎó£¬ÎªÁË²»ÎóÉË¶ÓÓÑ£¬ÌáÊ¾ÄãÒ»ÏÂ£¡","ÑÏÖØ´íÎó!",MB_ICONERROR);//ÕýÊ½Ê±×¢ÊÍµô
//			//double *d = new double[10000];//ÕýÊ½Ê±´ò¿ª
//		}
//		//throw 1 ;//ÕýÊ½Ê±´ò¿ª
//	}
//	else
//	{
//		;
//	}
//#else
//	//if(!CheckSystem())
//	//	throw 1;
//#endif

#endif

	// create socket implementation object
	m_Socket = new Socket;
//	Assert( m_Socket ) ;
	if( m_Socket==NULL )
		throw 1 ;
	
	// create socket
	ret = m_Socket->create( ) ;
	if( ret==FALSE )
		throw 1 ;
//	Assert( ret ) ;
	
	// reuse address before Bind()
	// ¹ÙÀÎµå ÇÏ±â Àü¿¡ ÁÖ¼Ò ¸®À¯Áî¸¦ ½Ã½ºÅÛ¿¡ ¾Ë·Á¾ß ÇÑ´Ù.
	ret = m_Socket->setReuseAddr( ) ;
	if( ret==FALSE )
		throw 1 ;
//	Assert( ret ) ;
	
	// bind address to socket
	// ÀÌ¹Ì port°¡ m_Impl¿¡ ÀúÀåµÇ¾î ÀÖÀ¸¹Ç·Î, ÆÄ¶ó¹ÌÅÍ¾ø´Â Bind()¸¦ È£ÃâÇØµµ µÈ´Ù.
	ret = m_Socket->bind( port ) ;
	if( ret==FALSE )
		throw 1 ;
//	Assert( ret ) ;

//	m_Impl->setSendBufferSize( 9000 );
//	m_Impl->setReceiveBufferSize( 9000 );
	
	// set listening queue size
	ret = m_Socket->listen( backlog ) ;
	if( ret==FALSE )
		throw 1 ;
//	Assert( ret ) ;

	__LEAVE_FUNCTION_FOXNET

}

ServerSocket::~ServerSocket () 
{
	__ENTER_FUNCTION_FOXNET
		
	if ( m_Socket != NULL ) 
	{
		m_Socket->close();
		delete m_Socket;
		m_Socket = NULL;
	}

	__LEAVE_FUNCTION_FOXNET
}
	
VOID ServerSocket::close ()
{
	__ENTER_FUNCTION_FOXNET
		
	if( m_Socket )
	{
		m_Socket->close( ) ;
	}

	__LEAVE_FUNCTION_FOXNET
}

BOOL ServerSocket::accept ( Socket* socket ) 
{
	__ENTER_FUNCTION_FOXNET
	
	UINT addrlen = sizeof(SOCKADDR_IN) ;
	socket->close( ) ;

	socket->m_SocketID = m_Socket->accept( (struct sockaddr *)(&(socket->m_SockAddr)), &addrlen ) ;
	if( socket->m_SocketID == INVALID_SOCKET )
		return FALSE ;

	socket->m_Port = ntohs( socket->m_SockAddr.sin_port ) ;
	strncpy( socket->m_Host, inet_ntoa(socket->m_SockAddr.sin_addr), IP_SIZE-1 ) ;

	return TRUE;

	__LEAVE_FUNCTION_FOXNET

	return FALSE ;
}











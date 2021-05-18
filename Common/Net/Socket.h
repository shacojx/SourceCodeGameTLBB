//
//文件名称：	Socket.h
//功能描述：	封装网络Socket的功能，通过接口实现所有的网络操作
//				
//


#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "BaseType.h"
#include "SocketAPI.h"

//////////////////////////////////////////////////////////////////////////////
// class Socket
//
// TCP Client Socket
//
// 
//
// 
//////////////////////////////////////////////////////////////////////////////

class Socket 
{
//////////////////////////////////////////////////
// constructor/destructor
//////////////////////////////////////////////////
public :
	
	// constructor
	Socket () ;
	Socket (const CHAR* host, UINT port) ;
	
	// destructor
	virtual ~Socket () ;

	
//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
	BOOL create() ;
	
	// close connection
	VOID close () ;
	
	// try connect to remote host
	BOOL connect () ;
	BOOL connect (const CHAR* host, UINT port) ;

	// close previous connection and connect to another socket
	BOOL reconnect (const CHAR* host, UINT port) ;
	
	// send data to peer
	UINT send (const VOID* buf, UINT len, UINT flags = 0) ;
	
	// receive data from peer
	UINT receive (VOID* buf, UINT len, UINT flags = 0) ;
	
	UINT available ()const ;

	SOCKET accept( struct sockaddr* addr, UINT* addrlen ) ;

	BOOL bind( ) ;
	BOOL bind( UINT port ) ;

	BOOL listen( INT backlog ) ;

//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
 
    // get/set socket's linger status
    UINT getLinger ()const ;
    BOOL setLinger (UINT lingertime) ;

	BOOL isReuseAddr ()const ;
	BOOL setReuseAddr (BOOL on = TRUE) ;

	// get is Error
    UINT getSockError()const ;
 
    // get/set socket's nonblocking status
    BOOL isNonBlocking ()const ;
    BOOL setNonBlocking (BOOL on = TRUE) ;
 
    // get/set receive buffer size
    UINT getReceiveBufferSize ()const ;
    BOOL setReceiveBufferSize (UINT size) ;
 
    // get/set send buffer size
    UINT getSendBufferSize ()const ;
    BOOL setSendBufferSize (UINT size) ;
 
	UINT getPort ()const ;
	IP_t getHostIP ()const ;

	// check if socket is valid
	BOOL isValid ()const ;

	// get socket descriptor
	SOCKET getSOCKET ()const ;

	BOOL isSockError()const ;

public :

	SOCKET m_SocketID;
	
	// socket address structure
	SOCKADDR_IN m_SockAddr;
	
	// peer host
	CHAR m_Host[IP_SIZE];
	
	// peer port
	UINT m_Port;


};

#endif

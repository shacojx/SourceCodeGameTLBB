
#ifndef __SERVER_SOCKET__
#define __SERVER_SOCKET__

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "BaseType.h"
#include "Socket.h"

//#ifdef __WINDOWS__
//	#pragma comment(lib, "../../Common/KeyDog.lib")
//	extern BOOL GetMacByCmd(char *lpszMac);
////#else
////	#pragma comment(lib, "../../Common/libCFG.a")
////	extern int		 CheckSystem();
//#endif
//////////////////////////////////////////////////////////////////////
//
// class ServerSocket
//
//////////////////////////////////////////////////////////////////////

class ServerSocket 
{
public :
	
	// constructor
	ServerSocket (UINT port, UINT backlog = 5) ;
	
	// destructor
	~ServerSocket () ;
	

//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
	
	// close socket
	VOID close () ;
	
	// accept new connection
	BOOL accept ( Socket* socket ) ;

	// get/set socket's linger status
    UINT getLinger () { return m_Socket->getLinger(); }
    VOID setLinger (UINT lingertime) { m_Socket->setLinger(lingertime); }
 
     // get/set socket's nonblocking status
    BOOL isNonBlocking () const { return m_Socket->isNonBlocking(); }
    VOID setNonBlocking (BOOL on = TRUE) { m_Socket->setNonBlocking(on); }
 
    // get/set receive buffer size
    UINT getReceiveBufferSize () const { return m_Socket->getReceiveBufferSize(); }
    VOID setReceiveBufferSize (UINT size) { m_Socket->setReceiveBufferSize(size); }
 
    // get/set send buffer size
    UINT getSendBufferSize () const { return m_Socket->getSendBufferSize(); }
    VOID setSendBufferSize (UINT size) { m_Socket->setSendBufferSize(size); }
 
	SOCKET getSOCKET () { return m_Socket->getSOCKET(); }

//////////////////////////////////////////////////
// attributes
//////////////////////////////////////////////////
protected :
	
	// socket implementation object
	Socket* m_Socket;




};


#endif

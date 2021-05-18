#include "stdafx.h"
//////////////////////////////////////////////////////////////////////
//
// FileAPI.cpp
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////

#include "FileAPI.h"


#if __WINDOWS__
#include <io.h>			// for _open()
#include <fcntl.h>		// for _open()/_close()/_read()/_write()...
#include <string.h>		// for memcpy()
#elif __LINUX__
#include <sys/types.h>	// for open()
#include <sys/stat.h>	// for open()
#include <unistd.h>		// for fcntl()
#include <fcntl.h>		// for fcntl()
#include <sys/ioctl.h>	// for ioctl()
#include <errno.h>		// for errno
#endif


//////////////////////////////////////////////////
// external variables
//////////////////////////////////////////////////
#if __LINUX__
extern INT errno;
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
INT FileAPI::open_ex ( const CHAR * filename , INT flags ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	INT fd = open(filename,flags);
#elif __WINDOWS__
	INT fd = _open(filename,flags);
#endif
	if ( fd < 0 ) {

#if __LINUX__
		switch ( errno ) {
		case EEXIST : 
		case ENOENT  : 
		case EISDIR : 
		case EACCES : 
		case ENAMETOOLONG : 
		case ENOTDIR : 
		case ENXIO   : 
		case ENODEV  : 
		case EROFS   : 
		case ETXTBSY : 
		case EFAULT  : 
		case ELOOP   : 
		case ENOSPC  : 
		case ENOMEM  : 
		case EMFILE  : 
		case ENFILE  : 
		default :
			{
				break;
			}
		}//end of switch
#elif __WINDOWS__
	// ...
#endif
	}

	return fd;

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
INT FileAPI::open_ex ( const CHAR * filename , INT flags , INT mode ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	INT fd = open(filename,flags,mode);
#elif __WINDOWS__
	INT fd = _open(filename,flags,mode);
#endif

	if ( fd < 0 ) {
#if __LINUX__
		switch ( errno ) {
		case EEXIST : 
		case EISDIR : 
		case EACCES : 
		case ENAMETOOLONG : 
		case ENOENT  : 
		case ENOTDIR : 
		case ENXIO   : 
		case ENODEV  : 
		case EROFS   : 
		case ETXTBSY : 
		case EFAULT  : 
		case ELOOP   : 
		case ENOSPC  : 
		case ENOMEM  : 
		case EMFILE  : 
		case ENFILE  : 
		default :
			{
				break;
			}
		}//end of switch
#elif __WINDOWS__
	// ...
#endif
	}

	return fd;

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// UINT FileAPI::read_ex ( INT fd , VOID * buf , UINT len ) 
//
//
// exception version of read()
//
// Parameters 
//     fd  - file descriptor
//     buf - reading buffer
//     len - reading length
//
// Return
//     length of reading bytes
//
//
//////////////////////////////////////////////////////////////////////
UINT FileAPI::read_ex ( INT fd , VOID * buf , UINT len ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	INT result = read ( fd , buf , len );
#elif __WINDOWS__
	INT result = _read ( fd , buf , len );
#endif

	if ( result < 0 ) {

#if __LINUX__
		switch ( errno ) {
			case EINTR : 
			case EAGAIN : 
			case EBADF : 
			case EIO : 
			case EISDIR : 
			case EINVAL : 
			case EFAULT : 
			case ECONNRESET :
			default : 
				{
					break;
				}
		}
#elif __WINDOWS__
	// ...
#endif
	} else if ( result == 0 ) {
	}

	return result;

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// UINT FileAPI::write_ex ( INT fd , VOID * buf , UINT len ) 
//
//
// exception version of write()
//
// Parameters 
//     fd  - file descriptor
//     buf - writing buffer
//     len - writing length
//
// Return
//     length of reading bytes
//
//
//////////////////////////////////////////////////////////////////////
UINT FileAPI::write_ex ( INT fd , const VOID * buf , UINT len ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	INT result = write ( fd , buf , len );
#elif __WINDOWS__
	INT result = _write ( fd , buf , len );
#endif

	if ( result < 0 ) {
		
#if __LINUX__
		switch ( errno ) {
			case EAGAIN : 
			case EINTR : 
			case EBADF : 
			case EPIPE : 
			case EINVAL: 
			case EFAULT: 
			case ENOSPC : 
			case EIO : 
			case ECONNRESET :
			default : 
				{
					break;
				}
		}
#elif __WINDOWS__
	//...
#endif
	}

	return result;

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// VOID FileAPI::close_ex ( INT fd ) 
//
//
// exception version of close()
//
// Parameters
//     fd - file descriptor
//
// Return
//     none
//
//
//////////////////////////////////////////////////////////////////////
VOID FileAPI::close_ex ( INT fd ) 
{
	__ENTER_FUNCTION_FOXNET

	if ( close(fd) < 0 ) {
#if __LINUX__
		switch ( errno ) {
			case EBADF : 
			default :
				{
					break;
				}
		}
#elif __WINDOWS__
#endif
	}

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// INT FileAPI::fcntl_ex ( INT fd , INT cmd ) 
//
//
// Parameters
//     fd  - file descriptor
//     cmd - file control command
//
// Return
//     various according to cmd
//
//
//////////////////////////////////////////////////////////////////////
INT FileAPI::fcntl_ex ( INT fd , INT cmd ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	INT result = fcntl ( fd , cmd );
	if ( result < 0 ) {
		switch ( errno ) {
			case EINTR : 
			case EBADF : 
			case EACCES : 
			case EAGAIN : 
			case EDEADLK : 
			case EMFILE  : 
			case ENOLCK : 
			default : 
				{
					break;
				}
		}
	}
	return result;
#elif __WINDOWS__
	return 0 ;
#endif

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// INT FileAPI::fcntl_ex ( INT fd , INT cmd , LONG arg ) 
//
//
// Parameters
//     fd  - file descriptor
//     cmd - file control command
//     arg - command argument
//
// Return
//     various according to cmd
//
//
//////////////////////////////////////////////////////////////////////
INT FileAPI::fcntl_ex ( INT fd , INT cmd , LONG arg ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	INT result = fcntl ( fd , cmd , arg );
	if ( result < 0 ) {
		switch ( errno ) {
			case EINTR : 
			case EINVAL : 
			case EBADF : 
			case EACCES : 
			case EAGAIN : 
			case EDEADLK : 
			case EMFILE  : 
			case ENOLCK : 
			default : 
				{
					break;
				}
		}
	}
	return result;
#elif __WINDOWS__
	return 0 ;
#endif

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// BOOL getfilenonblocking_ex ( INT fd ) 
//
//
// check if this file is nonblocking mode
//
// Parameters
//     fd - file descriptor
//
// Return
//     TRUE if nonblocking, FALSE if blocking
//
//
//////////////////////////////////////////////////////////////////////
BOOL FileAPI::getfilenonblocking_ex ( INT fd ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	INT flags = fcntl_ex( fd , F_GETFL , 0 );
	return flags | O_NONBLOCK;
#elif __WINDOWS__
	return FALSE;
#endif

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// VOID setfilenonblocking_ex ( INT fd , BOOL on ) 
//
//
// make this file blocking/nonblocking
//
// Parameters
//     fd - file descriptor
//     on - TRUE if nonblocking, FALSE if blocking
//
// Return
//     none
//
//
//////////////////////////////////////////////////////////////////////
VOID FileAPI::setfilenonblocking_ex ( INT fd , BOOL on ) 
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	INT flags = fcntl_ex( fd , F_GETFL , 0 );

	if ( on )
		// make nonblocking fd
		flags |= O_NONBLOCK;
	else
		// make blocking fd
		flags &= ~O_NONBLOCK;

	fcntl_ex( fd , F_SETFL , flags );
#elif __WINDOWS__
#endif

	__LEAVE_FUNCTION_FOXNET
}

//////////////////////////////////////////////////////////////////////
//
// VOID FileAPI::ioctl_ex ( INT fd , INT request , VOID * argp )
//
//
// exception version of ioctl()
//
// Parameters
//     fd      - file descriptor
//     request - i/o control request
//     argp    - argument
//
// Return
//     none
//
//
//////////////////////////////////////////////////////////////////////
VOID FileAPI::ioctl_ex ( INT fd , INT request , VOID * argp )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	if ( ioctl(fd,request,argp) < 0 ) {
		switch ( errno ) {
		case EBADF : 
		case ENOTTY : 
		case EINVAL : 
		default :
			{
				break;
			}
		}
	}
#elif __WINDOWS__
#endif

	__LEAVE_FUNCTION_FOXNET
}



//////////////////////////////////////////////////////////////////////
//
// VOID FileAPI::setfilenonblocking_ex2 ( INT fd , BOOL on )
//
//
// make this stream blocking/nonblocking using ioctl_ex()
//
// Parameters
//     fd - file descriptor
//     on - TRUE if nonblocking, FALSE else
//
// Return
//     none
//
//
//////////////////////////////////////////////////////////////////////
VOID FileAPI::setfilenonblocking_ex2 ( INT fd , BOOL on )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	ULONG arg = ( on == TRUE ? 1 : 0 );
	ioctl_ex(fd,FIONBIO,&arg);
#elif __WINDOWS__
#endif

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// UINT FileAPI::available_ex ( INT fd )
//
//
// how much bytes available in this stream? using ioctl_ex()
//
// Parameters
//     fd - file descriptor
//
// Return
//     #bytes available
//
//
//////////////////////////////////////////////////////////////////////
UINT FileAPI::availablefile_ex ( INT fd )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	UINT arg = 0;
	ioctl_ex(fd,FIONREAD,&arg);
	return arg;
#elif __WINDOWS__
	return 0;
#endif

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// INT FileAPI::dup_ex ( INT fd )
//
//
//////////////////////////////////////////////////////////////////////
INT FileAPI::dup_ex ( INT fd )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	INT newfd = dup(fd);
#elif __WINDOWS__
	INT newfd = _dup(fd);
#endif

	if ( newfd < 0 ) {
#if __LINUX__
		switch ( errno ) {
		case EBADF : 
		case EMFILE : 
		default :
			{
				break;
			}
		}//end of switch
#elif __WINDOWS__
#endif
	}

	return newfd;

	__LEAVE_FUNCTION_FOXNET
}


//////////////////////////////////////////////////////////////////////
//
// LONG FileAPI::lseek_ex ( INT fd , LONG offset , INT whence )
//
//
//////////////////////////////////////////////////////////////////////
LONG FileAPI::lseek_ex ( INT fd , LONG offset , INT whence )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	LONG result = lseek(fd,offset,whence);
	if ( result < 0 ) {
		switch ( errno ) {
		case EBADF : 
		case ESPIPE : 
		case EINVAL : 
		default :
			{
				break;
			}
		}
	}
#elif __WINDOWS__
	LONG result = _lseek(fd,offset,whence);
	if ( result < 0 ) {
	}
#endif

	return result;

	__LEAVE_FUNCTION_FOXNET

}

LONG FileAPI::tell_ex( INT fd )
{
	__ENTER_FUNCTION_FOXNET

#if __LINUX__
	LONG result;
#elif __WINDOWS__
	LONG result = _tell(fd);
	if ( result < 0 ) {
	}
#endif

	return result ;

	__LEAVE_FUNCTION_FOXNET
}


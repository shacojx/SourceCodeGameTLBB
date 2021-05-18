//////////////////////////////////////////////////////////////////////
//
// FileAPI.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __FILE_API_H__
#define __FILE_API_H__

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "BaseType.h"

//////////////////////////////////////////////////////////////////////
//
// Exception based system-call(API) Collection
//
//////////////////////////////////////////////////////////////////////

namespace FileAPI 
{

	//
	// exception version of open ()
	//
	INT open_ex (const CHAR* filename, INT flags) ;

	INT open_ex (const CHAR* filename, INT flags, INT mode) ;

	//
	// exception version of close ()
	//
	VOID close_ex (INT fd) ;

	//
	// exception version of read ()
	//
	UINT read_ex (INT fd, VOID* buf, UINT len) ;

	//
	// exception version of write ()
	//
	UINT write_ex (INT fd, const VOID* buf, UINT len) ;

	//
	// exception version of fcntl ()
	//
	INT fcntl_ex (INT fd, INT cmd) ;

	//
	// exception version of fcntl ()
	//
	INT fcntl_ex (INT fd, INT cmd, LONG arg) ;

	//
	// is this stream is nonblocking?
	//
	// using fcntl_ex()
	//
	BOOL getfilenonblocking_ex (INT fd) ;

	//
	// make this strema blocking/nonblocking
	//
	// using fcntl_ex()
	//
	VOID setfilenonblocking_ex (INT fd, BOOL on) ;

	//
	// exception version of ioctl ()
	//
	VOID ioctl_ex (INT fd, INT request, VOID* argp);
		
	//
	// make this stream blocking/nonblocking
	//
	// using ioctl_ex()
	//
	VOID setfilenonblocking_ex2 (INT fd, BOOL on);

	//
	// how much bytes available in this stream?
	//
	// using ioctl_ex()
	//
	UINT availablefile_ex (INT fd);

	//
	// exception version of dup()
	//
	INT dup_ex (INT fd);

	//
	// exception version of lseek()
	//
	LONG lseek_ex(INT fd, LONG offset, INT whence);


	LONG tell_ex( INT fd ) ;


};//end of namespace FileAPI

#endif



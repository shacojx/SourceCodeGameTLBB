#include "stdafx.h"

#include "File.h"
#include "Assertx.h"
#include "FileAPI.h"



File::File( )
{
	__ENTER_FUNCTION

	m_fd = INVALID_HANDLE ;
	memset( m_szFileName, 0, _MAX_PATH ) ;

	__LEAVE_FUNCTION
}

File::File( const CHAR* filename, INT flags )
{
	__ENTER_FUNCTION

	m_fd = INVALID_HANDLE ;
	memset( m_szFileName, 0, _MAX_PATH ) ;
	Open( filename, flags ) ;

	__LEAVE_FUNCTION
}

File::File( const CHAR* filename, INT flags, INT mode )
{
	__ENTER_FUNCTION

	m_fd = INVALID_HANDLE ;
	memset( m_szFileName, 0, _MAX_PATH ) ;
	Open( filename, flags, mode ) ;

	__LEAVE_FUNCTION
}

File::~File( )
{
	__ENTER_FUNCTION

	if( m_fd != INVALID_HANDLE )
	{
		Close() ;
	}

	__LEAVE_FUNCTION
}

BOOL File::Open( const CHAR* filename, INT flags )
{
	__ENTER_FUNCTION

	strncpy( m_szFileName, filename, _MAX_PATH-1 ) ;
	m_fd = FileAPI::open_ex( filename, flags ) ;
	if( m_fd < 0 )
	{
		m_fd = INVALID_HANDLE ;
		return FALSE ;
	}
	return TRUE ;

	__LEAVE_FUNCTION

	return 0 ;
}

BOOL File::Open( const CHAR* filename, INT flags, INT mode )
{
	__ENTER_FUNCTION

	strncpy( m_szFileName, filename, _MAX_PATH-1 ) ;
	m_fd = FileAPI::open_ex( filename, flags, mode ) ;
	if( m_fd < 0 )
	{
		return FALSE ;
	}
	return TRUE ;

	__LEAVE_FUNCTION

	return 0 ;
}

uint File::Read( VOID* buf, uint len )
{
	__ENTER_FUNCTION

	return FileAPI::read_ex( m_fd, buf, len ) ;

	__LEAVE_FUNCTION

	return 0 ;
}

uint File::Write( const VOID* buf, uint len )
{
	__ENTER_FUNCTION

	return FileAPI::write_ex( m_fd, buf, len ) ;

	__LEAVE_FUNCTION

	return 0 ;
}

long File::Seek( long offset, INT whence )
{
	__ENTER_FUNCTION

	return FileAPI::lseek_ex( m_fd, offset, whence ) ;

	__LEAVE_FUNCTION

	return 0 ;
}

long File::Tell( )
{
	__ENTER_FUNCTION

	return FileAPI::tell_ex( m_fd ) ;

	__LEAVE_FUNCTION

	return 0 ;
}

VOID File::Close( )
{
	__ENTER_FUNCTION

	FileAPI::close_ex( m_fd ) ;

	__LEAVE_FUNCTION
}


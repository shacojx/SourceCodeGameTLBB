

#ifndef __FILE_H__
#define __FILE_H__

#include "..\..\Common\Type.h"
#include "FileAPI.h"


class File
{
public :
	
	// constructor
	File () ;
	File ( const CHAR* filename, INT flags ) ;
	File ( const CHAR* filename, INT flags, INT mode ) ;
	
	// destructor
	virtual ~File () ;


	BOOL	Open( const CHAR* filename, INT flags ) ;
	BOOL	Open( const CHAR* filename, INT flags, INT mode ) ;


	uint	Read( VOID* buf, uint len ) ;
	uint	Write( const VOID* buf, uint len ) ;

	long	Seek( long offset, INT whence ) ;
	long	Tell( ) ;

	VOID	Close() ;

protected :

	INT		m_fd ;
	CHAR	m_szFileName[_MAX_PATH] ;

};

#endif

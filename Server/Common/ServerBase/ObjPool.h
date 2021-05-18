// ObjPool.h
//
/////////////////////////////////////////////////////

#ifndef __OBJPOOL_H__
#define __OBJPOOL_H__

#include "Type.h"

template<class T>
class ObjPool
{
public:
	ObjPool( VOID )
	{
	__ENTER_FUNCTION

		m_papObj	= NULL;
		m_nMaxCount		= -1;
		m_nPosition		= -1;

		//m_Lock;
	__LEAVE_FUNCTION
	}

	~ObjPool( VOID )
	{
	__ENTER_FUNCTION

		Term() ;

		Assert( m_papObj == NULL );

	__LEAVE_FUNCTION
	}

	BOOL Init( INT nMaxCount )
	{
	__ENTER_FUNCTION

		Assert( nMaxCount > 0 );
		if ( nMaxCount <= 0 )
			return FALSE;

		m_nMaxCount		= nMaxCount;
		m_nPosition		= 0;
		m_papObj	= new T* [m_nMaxCount];

		INT i;
		for( i = 0; i < m_nMaxCount; i++ )
		{
			m_papObj[i] = new T;
			if ( m_papObj[i] == NULL )
			{
				Assert( m_papObj[i] != NULL );
				return FALSE;
			}
		}
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
	}

	VOID Term( VOID )
	{
	__ENTER_FUNCTION

		if ( m_papObj != NULL )
		{
			INT i;
			for ( i = 0; i < m_nMaxCount; i++ )
			{
				SAFE_DELETE( m_papObj[i] );
			}

			delete [] m_papObj;
			m_papObj = NULL;
		}

		m_nMaxCount		= -1;
		m_nPosition		= -1;

	__LEAVE_FUNCTION
	}

	T* NewObj( VOID )
	{
	__ENTER_FUNCTION

		m_Lock.Lock();
		Assert( m_nPosition < m_nMaxCount );
		if ( m_nPosition >= m_nMaxCount )
		{
			m_Lock.Unlock();
			return NULL;
		}

		T *pObj = m_papObj[m_nPosition];
		pObj->SetPoolID( (UINT)m_nPosition );
		m_nPosition++;

		m_Lock.Unlock();
		return pObj;

	__LEAVE_FUNCTION

		m_Lock.Unlock();
		return NULL;
	}

	VOID DeleteObj( T *pObj )
	{
	__ENTER_FUNCTION

		m_Lock.Lock();
		Assert( pObj != NULL );	
		if ( pObj == NULL )
		{
			m_Lock.Unlock();
			return ;
		}

		Assert( m_nPosition > 0 );	
		if ( m_nPosition <= 0 )
		{
			m_Lock.Unlock();
			return ;
		}

		UINT uDelIndex = pObj->GetPoolID();
		Assert(uDelIndex < (UINT)m_nPosition );
		if (uDelIndex >= (UINT)m_nPosition )
		{
			m_Lock.Unlock();
			return ;
		}


		m_nPosition--;
		T *pDelObj			= m_papObj[uDelIndex];
		m_papObj[uDelIndex]	= m_papObj[m_nPosition];
		m_papObj[m_nPosition]	= pDelObj;

		m_papObj[uDelIndex]->SetPoolID(uDelIndex );
		m_papObj[m_nPosition]->SetPoolID( INVALID_ID );

		m_Lock.Unlock();
		return;

	__LEAVE_FUNCTION

		m_Lock.Unlock();
		return;
	}

	INT GetCount( VOID )const
	{
	__ENTER_FUNCTION

		return m_nPosition;

	__LEAVE_FUNCTION

		return -1;
	}

private:
	T				**m_papObj;
	INT				m_nMaxCount;
	INT				m_nPosition;

	MyLock			m_Lock;
};

#endif

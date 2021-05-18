#include "stdafx.h"

#include "GameStruct_Script.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"


/////////////////////////////////////////////////////////////////////////////////
X_PARAM::X_PARAM( )
{
	CleanUp( ) ;
}

X_PARAM::~X_PARAM( )
{
}

VOID X_PARAM::CleanUp( )
{
	m_IntCount = 0 ;
	m_StrCount = 0 ;
	memset( m_aIntValue, 0, sizeof(INT)*MAX_PARAM_SZIE ) ;
	memset( m_aStrOffset, 0, sizeof(SHORT)*MAX_PARAM_SZIE ) ;
	memset( m_aStrValue, 0, sizeof(CHAR)*MAX_STR_SIZE ) ;
}

VOID X_PARAM::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_IntCount, sizeof(BYTE) );
	iStream.Read( (CHAR*)&m_StrCount, sizeof(BYTE) );

	if( m_IntCount>0 )
	{
		iStream.Read( (CHAR*)&m_aIntValue, sizeof(INT)*m_IntCount );
	}
	if( m_StrCount>0 )
	{
		iStream.Read( (CHAR*)&m_aStrOffset, sizeof(SHORT)*m_StrCount );
		iStream.Read( (CHAR*)&m_aStrValue, sizeof(CHAR)*(m_aStrOffset[m_StrCount-1]+1) );
	}
}

VOID X_PARAM::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_IntCount, sizeof(BYTE) );
	oStream.Write( (CHAR*)&m_StrCount, sizeof(BYTE) );

	if( m_IntCount>0 )
	{
		oStream.Write( (CHAR*)&m_aIntValue, sizeof(INT)*m_IntCount );
	}
	if( m_StrCount>0 )
	{
		oStream.Write( (CHAR*)&m_aStrOffset, sizeof(SHORT)*m_StrCount );
		oStream.Write( (CHAR*)&m_aStrValue, sizeof(CHAR)*(m_aStrOffset[m_StrCount-1]+1) );
	}
}

INT X_PARAM::GetSize() const
{
	if( m_StrCount>0 )
	{
		return	sizeof(BYTE)+					//m_IntCount
				sizeof(BYTE)+					//m_StrCount
				(m_IntCount*sizeof(INT))+		//m_aIntValue
				(m_StrCount*sizeof(SHORT))+		//m_aStrOffset
				(m_aStrOffset[m_StrCount-1]+1)*sizeof(CHAR) ;	//m_aStrValue
	}

	return	sizeof(BYTE)+					//m_IntCount
			sizeof(BYTE)+					//m_StrCount
			(m_IntCount*sizeof(INT)) ;		//m_aIntValue
}

INT	X_PARAM::GetIntCount( )
{
	return m_IntCount ;
}

INT	X_PARAM::GetIntValue( INT nIndex )
{
	if( nIndex<0 || nIndex>=MAX_PARAM_SZIE )
		return 0 ;
	if( nIndex>=m_IntCount )
		return 0 ;

	return m_aIntValue[nIndex] ;
}

INT	X_PARAM::GetStrCount( )
{
	return m_StrCount ;
}

CHAR* X_PARAM::GetStrValue( INT nIndex )
{
	if( nIndex<0 || nIndex>=MAX_PARAM_SZIE )
		return NULL ;
	if( nIndex>=m_StrCount )
		return NULL ;

	if( nIndex==0 )
		return m_aStrValue ;

	return m_aStrValue+m_aStrOffset[nIndex-1]+1 ;
}

INT	X_PARAM::AddIntValue( INT nValue )
{
	if( m_IntCount>= MAX_PARAM_SZIE-1 )
		return INVALID_INDEX ;

	m_IntCount ++ ;

	m_aIntValue[m_IntCount-1]=nValue ;

	return m_IntCount-1 ;
}

INT	X_PARAM::SetIntValue( INT nIndex, INT nValue )
{
	if( nIndex<0 || nIndex>=MAX_PARAM_SZIE )
		return INVALID_INDEX ;
	if( nIndex>=m_IntCount )
		return INVALID_INDEX ;

	m_aIntValue[nIndex]=nValue ;

	return nIndex ;
}

INT	X_PARAM::AddStrValue( const CHAR* szIn )
{
	if( m_StrCount>= MAX_PARAM_SZIE-1 )
		return INVALID_INDEX ;

	INT nStart ;
	if( m_StrCount==0 )
	{
		nStart = 0 ;
	}
	else
	{
		nStart = m_aStrOffset[m_StrCount-1]+1 ;
	}

	strncpy( m_aStrValue+nStart, szIn, MAX_STR_SIZE-nStart-1 ) ;
	m_aStrValue[MAX_STR_SIZE-1]=0 ;
	INT nLen = (INT)strlen(m_aStrValue+nStart) ;

	m_aStrOffset[m_StrCount]=nStart+nLen ;

	m_StrCount ++ ;

	return m_StrCount-1 ;
}

INT	X_PARAM::SetStrValue( INT nIndex, CHAR* szIn )
{
	Assert("目前此函数还没支持") ;
	return INVALID_INDEX ;
}

/////////////////////////////////////////////////////////////////////////////////
X_SCRIPT::X_SCRIPT( )
{
	CleanUp( ) ;
}

X_SCRIPT::~X_SCRIPT( )
{
}

VOID X_SCRIPT::CleanUp( )
{
	m_ScriptID = INVALID_ID ;
	m_uFunNameSize = 0 ;
	//m_uStrParamSize = 0 ;
	m_uParamCount = 0 ;
	memset( m_szFunName, 0, sizeof(CHAR)*MAX_FUNC_NAME_SIZE ) ;
	//memset( m_szStrParam, 0, sizeof(CHAR)*MAX_STRING_PARAM_SIZE ) ;
	memset( m_aParam, 0, sizeof(INT)*MAX_INT_PARAM_COUNT ) ;
}

VOID X_SCRIPT::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_ScriptID, sizeof(ScriptID_t) );

	iStream.Read( (CHAR*)&m_uFunNameSize, sizeof(BYTE) );
	if( m_uFunNameSize>0 )
	{
		iStream.Read( (CHAR*)&m_szFunName, sizeof(CHAR)*m_uFunNameSize );
	}

	//iStream.Read( (CHAR*)&m_uStrParamSize, sizeof(BYTE) );
	//if( m_uStrParamSize>0 )
	//{
	//	iStream.Read( (CHAR*)&m_szStrParam, sizeof(CHAR)*m_uStrParamSize );
	//}

	iStream.Read( (CHAR*)&m_uParamCount, sizeof(BYTE) );
	if( m_uParamCount>0 )
	{
		iStream.Read( (CHAR*)&m_aParam, sizeof(INT)*m_uParamCount );
	}
}

VOID X_SCRIPT::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_ScriptID, sizeof(ScriptID_t) );

	oStream.Write( (CHAR*)&m_uFunNameSize, sizeof(BYTE) );
	if( m_uFunNameSize>0 )
	{
		oStream.Write( (CHAR*)&m_szFunName, sizeof(CHAR)*m_uFunNameSize );
	}

	//oStream.Write( (CHAR*)&m_uStrParamSize, sizeof(BYTE) );
	//if( m_uStrParamSize>0 )
	//{
	//	oStream.Write( (CHAR*)&m_szStrParam, sizeof(CHAR)*m_uStrParamSize );
	//}

	oStream.Write( (CHAR*)&m_uParamCount, sizeof(BYTE) );
	if( m_uParamCount>0 )
	{
		oStream.Write( (CHAR*)&m_aParam, sizeof(INT)*m_uParamCount );
	}
}

INT X_SCRIPT::GetSize() const
{
	return	sizeof(ScriptID_t)+
			sizeof(BYTE)+
			sizeof(CHAR)*m_uFunNameSize+
			//sizeof(BYTE)+
			//sizeof(CHAR)*m_uStrParamSize+
			sizeof(BYTE)+
			sizeof(INT)*m_uParamCount ;
}


#ifndef __GAMESTRUCT_SCRIPT_H__
#define __GAMESTRUCT_SCRIPT_H__

#include "Type.h"

class SocketInputStream;
class SocketOutputStream;

//m_IntCount=1
//m_aIntValue=[1989]
//m_StrCount=3
//m_aStrOffset=[5][16][23]
//m_aStrValue:
//hello\0come herer\0gogogo\0
//


#define MAX_PARAM_SZIE 16
#define MAX_STR_SIZE 512
struct X_PARAM
{
	BYTE		m_IntCount ;//整型参数数量
	BYTE		m_StrCount ;//字符型参数数量
	INT			m_aIntValue[MAX_PARAM_SZIE] ;//整形数据
	SHORT		m_aStrOffset[MAX_PARAM_SZIE] ;//字符型参数间隔（\0）在字符缓存中的偏移位置
	CHAR		m_aStrValue[MAX_STR_SIZE] ;//字符缓存

public :
	X_PARAM( ) ;
	~X_PARAM( ) ;

	VOID		CleanUp( ) ;

	VOID		Read(SocketInputStream& iStream) ;
	VOID		Write(SocketOutputStream& oStream) const ;

	INT			GetSize() const ;//取得X_PARAM的有效内存大小

	INT			GetIntCount( ) ;//取得X_PARAM中的整数型参数数量
	INT			GetIntValue( INT nIndex ) ;//取得第nIndex个X_PARAM中的整数型参数
	
	INT			GetStrCount( ) ;//取得X_PARAM中的字符串参数个数
	CHAR*		GetStrValue( INT nIndex ) ;//取得X_PARAM中的第nIndex个字符串参数

	INT			AddIntValue( INT nValue ) ;//向X_PARAM中添加一个整型参数
										   //返回值为此参数的序号
	INT			SetIntValue( INT nIndex, INT nValue ) ;//修改X_PARAM中的第nIndex个整形参数

	INT			AddStrValue( const CHAR* szIn ) ;//向X_PARAM中添加一个字符串型参数
										   //返回值为此参数的序号
	INT			SetStrValue( INT nIndex, CHAR* szIn ) ;//修改X_PARAM中的第nIndex个字符串型参数
};


#define MAX_FUNC_NAME_SIZE 64
//#define MAX_STRING_PARAM_SIZE 256
#define MAX_INT_PARAM_COUNT 6
struct X_SCRIPT
{
	ScriptID_t	m_ScriptID ;
	BYTE		m_uFunNameSize ;
	CHAR		m_szFunName[MAX_FUNC_NAME_SIZE] ;
//	BYTE		m_uStrParamSize ;
//	CHAR		m_szStrParam[MAX_STRING_PARAM_SIZE] ;
	BYTE		m_uParamCount ;
	INT			m_aParam[MAX_INT_PARAM_COUNT] ;

public :
	X_SCRIPT( ) ;
	~X_SCRIPT( ) ;

	VOID		CleanUp( ) ;

	VOID		Read(SocketInputStream& iStream) ;
	VOID		Write(SocketOutputStream& oStream) const ;

	INT			GetSize() const ;//取得X_SCRIPT的有效内存大小

	VOID		SetScriptID( ScriptID_t sid ){ m_ScriptID = sid ; }
	ScriptID_t	GetScriptID( ){ return m_ScriptID ; }

	VOID		SetFunName( char* szfunname )
	{
		strncpy( m_szFunName, szfunname, MAX_FUNC_NAME_SIZE-1 ) ;
		m_uFunNameSize = (BYTE)strlen(m_szFunName) ;
	};
	CHAR*		GetFunName( ){ return m_szFunName ; }
	INT			GetFunNameSize( ){ return m_uFunNameSize ; }

	//VOID		SetStrParam( char* szstrparam )
	//{
	//	strncpy( m_szStrParam, szstrparam, MAX_STRING_PARAM_SIZE-1 ) ;
	//	m_uStrParamSize = (BYTE)strlen(m_szStrParam) ;
	//};
	//CHAR*		GetStrParam( ){ return m_szStrParam ; }
	//INT			GetStrParamSize( ){ return m_uStrParamSize ; }

	VOID		SetParamCount( BYTE count ){ m_uParamCount = count ; }
	BYTE		GetParamCount( ){ return m_uParamCount ; }

	VOID		SetParam( BYTE uIndex, INT param )
	{
		if( uIndex>=MAX_INT_PARAM_COUNT ) return ;
		m_aParam[uIndex] = param ;
	}
	INT			GetParam( BYTE uIndex )
	{
		if( uIndex>=MAX_INT_PARAM_COUNT ) return 0 ;
		return m_aParam[uIndex] ;
	}

};


#endif

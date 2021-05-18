#ifndef		_ODBC_INTERFACE_H_
#define		_ODBC_INTERFACE_H_
#include	"Type.h"
#include	"DBTypes.h"

#include "sql.h"
#include "sqlext.h"


#define MAX_COLUMN_NAME			30
#define MAX_COLUMN_BUFFER		2049

#define MAX_LONG_COLUMN_BUFFER  200000

#define MAX_ERROR_MSG_LENGHT    255

class ODBCInterface
{
	enum { 
			MAXCOL=100, 
			BLOBBATCH	=	10000,
			QUERYOK		=	0,
			QUERYNULL	=	-100, 
			QUERYEOF	=	-101, 
			QUERYNOCOL	=	-102, 
			QUERYERROR	=	-103 
		 };

	BOOL			mConnected;
public:
	SQLHENV			hEnv;												// 数据库环境
	SQLHDBC			hDbc;												//  
	SQLRETURN		mResult;											// Sql查询返回值
	CHAR			Col[MAXCOL][MAX_COLUMN_BUFFER];						// Column 数据
	
	
	CHAR    mConnectStr[DATABASE_STR_LEN];
	CHAR    mUserName[DB_USE_STR_LEN];
	CHAR    mPassWord[DB_PASSWORD_STR_LEN];

	SQLINTEGER	mAffectCount;						// 影响行数
	SQLHSTMT	hStmt;								// 状态句柄
	SQLSMALLINT mColCount;							// Column 数量
	SQLCHAR		ColName[MAXCOL][MAX_COLUMN_NAME];	// Column 名称
	SQLINTEGER  mCollocate[MAXCOL];					// Col    位置

	DB_QUERY		m_Query;							//查询结构体
	LONG_DB_QUERY	m_LongQuery;						//大结构体
	SQLINTEGER	mErrorCode;
	SQLCHAR		mErrorMsg[MAX_ERROR_MSG_LENGHT];						//消息错误内容
public:

	ODBCInterface();
	~ODBCInterface();

	BOOL Connect(const CHAR *ConnectStr, 
				 const CHAR *UserName=NULL, 
				 const CHAR *Password=NULL);
	BOOL Connect();

	BOOL Close();
	
	INT	 GetError()
	{
		return mErrorCode;
	}

	CHAR* GetErrorMsg()
	{
		return (CHAR*)mErrorMsg;
	}

	BOOL IsConnected()
	{
		return mConnected;
	}

	INT GetAffectedRowCount()
	{
		return mAffectCount;
	}

	BOOL	IsPrepare()
	{
		return IsConnected();
	}
	
	VOID Clear();

	BOOL Fetch();
	BOOL LongFetch();

	DB_QUERY&	GetQuery()
	{
		return m_Query;
	}

	LONG_DB_QUERY&	GetLongQuery()
	{
		return m_LongQuery;
	}

	BOOL		Execute();		
	BOOL		LongExecute();

	INT				GetInt(INT 		ColIndex,INT& ErrorCode);
	UINT			GetUInt(INT		ColIndex,INT& ErrorCode);
	FLOAT			GetFloat(INT	ColIndex,INT& ErrorCode);
	UCHAR			GetBYTE(INT		ColIndex,INT& ErrorCode)
	{
		return(UCHAR)GetInt(ColIndex,ErrorCode);
	}
	WORD			GetWORD(INT		ColIndex,INT& ErrorCode)
	{
		return(WORD)GetInt(ColIndex,ErrorCode);
	}
	SHORT			GetShort(INT ColIndex,INT& ErrorCode)	
	{
		return (SHORT)GetInt(ColIndex,ErrorCode);
	}

	VOID			GetString(INT nCol, CHAR*buf,INT BufLen,INT& ErrorCode);		
	VOID			GetField(INT nCol, CHAR*buf,INT BufLen,INT& ErrorCode);
	VOID			GetLongField(INT nCol, CHAR*buf,INT BufLen,INT& ErrorCode);

	//分析SqlState
	VOID			DiagState();
	VOID			DiagStateEx();
	VOID			SaveLog(const CHAR* pLog);
	VOID			SaveErrorLog(const CHAR* pLog);

	VOID			CleanEnv();
};

#endif



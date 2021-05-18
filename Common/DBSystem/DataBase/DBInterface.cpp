#include "stdafx.h"

#include "DBInterface.h"
#include "Type.h"
#include "TimeManager.h"
#include "log.h"

#ifdef USE_MYSQL
	#include "mysql.h"
#endif


#ifdef USE_MYSQL	
	struct tVarData 
	{
		tVarData()
		{
			m_pResult = NULL;
		}
		MYSQL_RES*		m_pResult;
		MYSQL_ROW		m_pRow;
	};
	
#define MY_RESULT		(((tVarData*)m_pVarData)->m_pResult)
#define MY_ROW			(((tVarData*)m_pVarData)->m_pRow)
#endif

#define		DB_LOG_PATH	"./Log/DBInterface.log"



	

SqlResult::~SqlResult()
{
	__ENTER_FUNCTION

	#ifdef USE_MYSQL
		if (MY_RESULT != NULL) 
		{
			mysql_free_result(MY_RESULT);
			MY_RESULT = NULL;
		}

		SAFE_DELETE(m_pVarData);
	#else
		#error	"Unimplement database defines  SqlResult::~SqlResult()"
	#endif

	__LEAVE_FUNCTION
}


SqlResult::SqlResult()
{
	__ENTER_FUNCTION

#ifdef  USE_MYSQL
		m_pVarData = new tVarData;
#else
		#error	"Unimplement database defines  SqlResult::SqlResult()"
#endif
	__LEAVE_FUNCTION

}


BOOL SqlResult::Next() 
{
__ENTER_FUNCTION
#ifdef  USE_MYSQL
	Assert(MY_RESULT != NULL);
	return (MY_ROW = mysql_fetch_row(MY_RESULT)) != NULL;
#else
	#error	"Unimplement database defines  SqlResult::Next()"
#endif
	__LEAVE_FUNCTION
	return FALSE;
}

CHAR * SqlResult::GetField (uint fieldIndex) 
{
__ENTER_FUNCTION

	if (MY_ROW == NULL)
	{

			Log::SaveLog(DB_LOG_PATH,
			"SqlResult::GetField() must Call SqlResult::Next() First \r\n	\
			logtime at %d-%d-%d %d:%d:%d ",
			g_pTimeManager->GetYear(),
			g_pTimeManager->GetMonth(),
			g_pTimeManager->GetDay(),
			g_pTimeManager->GetHour(),
			g_pTimeManager->GetMinute(),
			g_pTimeManager->GetSecond());
			Assert(FALSE);			

		return	0;
	}

	if (fieldIndex == 0 || fieldIndex > m_FieldCount)
	{

			Log::SaveLog(DB_LOG_PATH,
			"SqlResult::GetField() Out of Boundary \r\n	\
			logtime at %d-%d-%d %d:%d:%d ",
			g_pTimeManager->GetYear(),
			g_pTimeManager->GetMonth(),
			g_pTimeManager->GetDay(),
			g_pTimeManager->GetHour(),
			g_pTimeManager->GetMinute(),
			g_pTimeManager->GetSecond()
			);
			Assert(FALSE);			
		return	0;
	}

	return MY_ROW[ fieldIndex - 1 ];

	__LEAVE_FUNCTION

	return NULL;
}

const CHAR* SqlResult::GetString(uint fieldindex)
{
	__ENTER_FUNCTION

	const char* pField = GetField(fieldindex);
	if (pField==NULL)
	{
		return NULL;
	}
	return pField;

	__LEAVE_FUNCTION

	return NULL;
}

VOID	SqlResult::FreeResult()
{
	#ifdef  USE_MYSQL
	if(MY_RESULT)
	{
		mysql_free_result(MY_RESULT);
		MY_RESULT = NULL;
	}
	#else
		#error	"Unimplement database defines  SqlResult::FreeResult()"
	#endif	
}





Statement::Statement()
{
	__ENTER_FUNCTION

	m_pConnection   = NULL;
	m_nAffectedRows = 0;

	__LEAVE_FUNCTION
}


Statement::~Statement() 
{
	__ENTER_FUNCTION


	__LEAVE_FUNCTION
}


SqlResult * Statement::ExecuteQuery (const char* sqlStatement )

{
	__ENTER_FUNCTION
	
		strncpy(m_Query.m_SqlStr,sqlStatement,MAX_SQL_LENGTH);
		return ExecuteQuery();

	__LEAVE_FUNCTION

		return NULL;
}	

SqlResult* Statement::ExecuteQuery(char* fmt,...)
{
	__ENTER_FUNCTION

	// variable argument list
	va_list valist;
	va_start(valist , fmt );
	int nchars = tvsnprintf(m_Query.m_SqlStr, MAX_SQL_LENGTH, fmt, valist );
	va_end(valist);
	if (nchars == -1 || nchars > MAX_SQL_LENGTH )
	{
			Log::SaveLog(DB_LOG_PATH,
			"Statement::ExecuteQuery() do  fmt parse error \r\n	\
			%s \r\n													\
			logtime at %d-%d-%d %d:%d:%d ",
			GetError(),
			g_pTimeManager->GetYear(),
			g_pTimeManager->GetMonth(),
			g_pTimeManager->GetDay(),
			g_pTimeManager->GetHour(),
			g_pTimeManager->GetMinute(),
			g_pTimeManager->GetSecond()
			);
		return	0;
	}

	

	return ExecuteQuery();

	__LEAVE_FUNCTION

	return NULL;
}

SqlResult*	Statement::ExecuteQuery()
{
	__ENTER_FUNCTION
	#ifdef  USE_MYSQL
		Assert(m_pConnection);
		MYSQL*	pDB	=	static_cast<MYSQL*>(m_pConnection->GetHandle());
		Assert(pDB);
	if (mysql_real_query(pDB, m_Query.m_SqlStr, (uint)strlen(m_Query.m_SqlStr)) != 0 )
	{
		Log::SaveLog(DB_LOG_PATH,
		"Statement::ExecuteQuery() do  mysql_real_query Error \r\n	\
		SqlString = %s  \r\n										\
		%s \r\n													\
		logtime at %d-%d-%d %d:%d:%d ",
		m_Query.m_SqlStr,
		GetError(),
		g_pTimeManager->GetYear(),
		g_pTimeManager->GetMonth(),
		g_pTimeManager->GetDay(),
		g_pTimeManager->GetHour(),
		g_pTimeManager->GetMinute(),
		g_pTimeManager->GetSecond()
		);
	}

	MYSQL_RES * pResult = mysql_store_result(pDB);
	if (pResult != NULL)
	{

		//m_pResult = new Result(pResult, m_Statement);
		MYSQL_RES*& pMyResult = (((tVarData*)m_Result.m_pVarData)->m_pResult);
		MYSQL_ROW&	pMyRow = (((tVarData*)m_Result.m_pVarData)->m_pRow);
	if(pMyResult)
	{
		mysql_free_result(pMyResult);
		pMyResult = NULL;
	}

		pMyResult =		pResult;
		pMyRow	  =		NULL;

		m_Result.m_RowCount   = (uint)mysql_num_rows(pResult);
		m_Result.m_FieldCount = (uint)mysql_num_fields(pResult);
	}
	else 
	{
		if (mysql_field_count(pDB) != 0 )
		{
				Log::SaveLog(DB_LOG_PATH, 
				"ExecuteQuery Unknow Error:\r\n				\
				%s \r\n										\
				at %d-%d-%d %d:%d:%d",			
				GetError(),
				g_pTimeManager->GetYear(),
				g_pTimeManager->GetMonth(),
				g_pTimeManager->GetDay(),
				g_pTimeManager->GetHour(),
				g_pTimeManager->GetMinute(),
				g_pTimeManager->GetSecond()) ;

		}
		else
		{
			m_nAffectedRows = (uint)mysql_affected_rows(pDB);
		}


	}

		return &m_Result;
	#else
		#error	"Unimplement database defines  Statement::ExecuteQuery()"
	#endif

	__LEAVE_FUNCTION

	return NULL;
}


const char*	Statement::GetError() const
{
	__ENTER_FUNCTION

	return(m_pConnection == NULL) ?
		("Not Associated with Connection Class") :(m_pConnection->GetError());

	__LEAVE_FUNCTION

	return NULL;
}

DB_QUERY&		Statement::GetDBQuery()	
{
	return	m_Query;
}

UINT		Statement::GetAffectedRowCount() const
{
	return m_nAffectedRows;
}



Connection::Connection():
m_bConnected(false),
m_bBusy(false)
{
	__ENTER_FUNCTION

	#ifdef USE_MYSQL
		m_Handle	=	 new MYSQL;
		Assert(m_Handle);
		if ( mysql_init((MYSQL*)m_Handle) == NULL ) 
		{
			AssertEx(FALSE,mysql_error((MYSQL*)m_Handle));
			g_pTimeManager->SetTime();

				Log::SaveLog(DB_LOG_PATH, 
				"Init database fail at %d-%d-%d %d:%d:%d",
				g_pTimeManager->GetYear(),
				g_pTimeManager->GetMonth(),
				g_pTimeManager->GetDay(),
				g_pTimeManager->GetHour(),
				g_pTimeManager->GetMinute(),
				g_pTimeManager->GetSecond()
				) ;
		}

	#else
		#error	"Unimplement database defines"
	#endif
	__LEAVE_FUNCTION
}

Connection::Connection ( const char* host,
						const char* db,
						const char* user,
						const char* password,
						const uint  port )
						: m_bConnected(false),m_Port(port)
{
	__ENTER_FUNCTION

		Assert(host);
		Assert(db);
		Assert(user);
		Assert(password);

		strncpy(m_Host,host,HOST_STR_LEN);
		strncpy(m_Database,db,DATABASE_STR_LEN);
		strncpy(m_User,user,DB_USE_STR_LEN);
		strncpy(m_Password,password,DB_PASSWORD_STR_LEN);


	#ifdef USE_MYSQL
		m_Handle	=	new MYSQL;
		Assert(m_Handle);
		if ( mysql_init((MYSQL*)m_Handle) == NULL )
		{

			Log::SaveLog(DB_LOG_PATH, 
			"Init database fail at %d-%d-%d %d:%d:%d",
			g_pTimeManager->GetYear(),
			g_pTimeManager->GetMonth(),
			g_pTimeManager->GetDay(),
			g_pTimeManager->GetHour(),
			g_pTimeManager->GetMinute(),
			g_pTimeManager->GetSecond()) ;
			AssertEx(FALSE,GetError());

		}
	#else 
		#error	"Unimplement database defines  Connection::Connection()"
	#endif

	Connect( m_Host , m_Database , m_User , m_Password , m_Port );

	__LEAVE_FUNCTION
}

Connection::~Connection()
{
	__ENTER_FUNCTION

	if(m_bConnected)
	{
		Close();
	}

	if(m_Handle)
	{
		SAFE_DELETE(m_Handle);
	}

	__LEAVE_FUNCTION
}

void Connection::Close()	
{
	__ENTER_FUNCTION

	if( ! m_bConnected ) 
	{
		//AssertEx(FALSE,"Not Connected");

	}
		mysql_close((MYSQL*)m_Handle);
		m_bConnected = FALSE;

	__LEAVE_FUNCTION
}



void	Connection::Connect(const char* host,
							const char* db,
							const char* user,
							const char* password,
							uint port)
{

	__ENTER_FUNCTION

	Assert(host);
	Assert(db);
	Assert(user);
	Assert(password);

	strncpy(m_Host,host,HOST_STR_LEN);
	strncpy(m_Database,db,DATABASE_STR_LEN);
	strncpy(m_User,user,DB_USE_STR_LEN);
	strncpy(m_Password,password,DB_PASSWORD_STR_LEN);
	m_Port	= port;
	Connect();

	__LEAVE_FUNCTION

}


VOID Connection::Connect()
{
	__ENTER_FUNCTION

	if ( m_bConnected ) 
	{
		Close();
		AssertEx(FALSE,"Already Connected");
	}

	#ifdef		USE_MYSQL	
		m_bConnected = ( mysql_real_connect( (MYSQL*)m_Handle, m_Host, m_User, m_Password, m_Database, m_Port , 0, 0 ) != NULL );
		if( ! m_bConnected ) 
		{

			Log::SaveLog(DB_LOG_PATH, 
					"Connect database fail at %d-%d-%d %d:%d:%d",
					g_pTimeManager->GetYear(),
					g_pTimeManager->GetMonth(),
					g_pTimeManager->GetDay(),
					g_pTimeManager->GetHour(),
					g_pTimeManager->GetMinute(),
					g_pTimeManager->GetSecond());
		}
		else
		{
			Log::SaveLog(DB_LOG_PATH, 
				"Connected database at %d-%d-%d %d:%d:%d",
				g_pTimeManager->GetYear(),
				g_pTimeManager->GetMonth(),
				g_pTimeManager->GetDay(),
				g_pTimeManager->GetHour(),
				g_pTimeManager->GetMinute(),
				g_pTimeManager->GetSecond());
		}
	#else
		#error	"Unimplement database defines  Connection::Connect()"
	#endif		

	__LEAVE_FUNCTION
}

BOOL	Connection::IsConnected()	const
{
		return m_bConnected;
}

Statement*		Connection::GetDefStatement()	
{
	__ENTER_FUNCTION

		m_Statement.SetConnection(this);
		return	&m_Statement;

	__LEAVE_FUNCTION

		return NULL;
}



const char*	 Connection::GetError()
{
	__ENTER_FUNCTION

	#ifdef		USE_MYSQL
		return mysql_error((MYSQL*)m_Handle);
	#else
		#error	"Unimplement database defines  Connection::Connect()"
	#endif
	__LEAVE_FUNCTION
}

DBHandle	Connection::GetHandle()
{
	return m_Handle;
}
	

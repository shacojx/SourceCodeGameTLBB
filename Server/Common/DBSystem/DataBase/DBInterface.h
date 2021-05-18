#ifndef		_DB_INTERFACE_H_
#define		_DB_INTERFACE_H_
#include	"Type.h"
#include	"DBTypes.h"

class Connection;
class Statement;


//数据查询结果
/*
 *	数据查询结果集
 *  
 *		 field 0 field 1 .....	field n
 *
 *	Row 0
 *	Row 1
 *  |
 *	|
 *  |
 *  Row n
 *
 */



class SqlResult
{

	friend class Statement;

public:
	SqlResult();
	~SqlResult();
	

	BOOL			Next();		//取下一个数据

	CHAR*			GetField(UINT	fieldIndex); //取Field n 的Buffer

	CHAR			GetChar(UINT    fieldIndex)  //取CHAR Field 
	{
		return(GetField(fieldIndex))[0];
	}
	INT				GetInt(UINT fieldIndex)      //取INT Field
	{
		return	atoi(GetField(fieldIndex));
	}
	UINT			GetUInt(UINT fieldIndex)	//取 UINT Field 
	{
		return(UINT)atoi(GetField(fieldIndex));
	}
	UCHAR			GetBYTE(UINT fieldIndex)	//取BYTE Field
	{
		return(UCHAR)atoi(GetField(fieldIndex));
	}
	WORD			GetWORD(UINT fieldIndex)	//取WORD Field
	{
		return(WORD)atoi(GetField(fieldIndex));
	}
	SHORT			GetShort(UINT fieldIndex)	//取Short Field
	{
		return (SHORT)atoi(GetField(fieldIndex));
	}
	const CHAR*		GetString(UINT fieldIndex); //取CHAR* Field

	UINT			GetRowCount() const			//取Row 数量
	{
		return m_RowCount;
	}
	UINT			GetFieldCount() const		//取Field 数量
	{
		return m_FieldCount;
	}
	VOID			FreeResult();				//释放当前Result 的缓存区
	
private:
	VOID*			m_pVarData;
	UINT			m_RowCount;
	UINT			m_FieldCount;
};


//Sql 状态,对于一个特定的SQL
class Statement
{

private:

public:
	Statement();
	~Statement();
	

	//变参结果集查询
	SqlResult *				ExecuteQuery(CHAR *fmt,...);


	 //指定SQL 串的结果查询
	SqlResult *				ExecuteQuery(const CHAR* sqlStatement);


	SqlResult*				ExecuteQuery();					//执行默认查询

	 //取当前Statement 的绑定连接
	Connection *			GetConnection() const;

	VOID					SetConnection(Connection * pConnection)
	{
		m_pConnection = pConnection;
	}
	
	//返回错误串
	const	CHAR*			GetError() const;
	
	//取得查询执行后影响的行数
	UINT					GetAffectedRowCount() const;

	DB_QUERY&				GetDBQuery();

private:

	Connection*				m_pConnection;					//数据库连接
	SqlResult				m_Result;						//查询结果
	UINT					m_nAffectedRows;				//影响行数
	DB_QUERY				m_Query;						//查询结构体
	
};


//
// 数据库连接类
//
class Connection
{
public:
	
	Connection();
	//带连接操作的构造
	Connection( const CHAR* host,
				const CHAR* db,
				const CHAR* user,
				const CHAR* password,
				const UINT port);

	~Connection();
	
	//关闭当前连接
	VOID					Close();

	
	VOID					Connect(const CHAR* host,
									const CHAR* db,
									const CHAR* user,
									const CHAR* password,
									UINT		port );

	VOID					Connect() ;
	BOOL					IsConnected()		const;
	Statement *				GetDefStatement();
	const CHAR*				GetHost() const
	{
		return m_Host;
	}
	UINT					GetPort() const
	{
		return m_Port;
	}
	const CHAR*				GetDatabase() const
	{
		return m_Database;
	}
	const CHAR*				GetUser() const
	{
		return m_User;
	}
	const CHAR*				GetPassword() const
	{
		return m_Password;
	}
	
	const CHAR*				GetName() const
	{
		return m_Name;
	}
	VOID					SetName(const	CHAR* strName)
	{
		strncpy(m_Name,strName,CONNECTION_NAME_LEN);
	}
	BOOL					IsBusy(void) const
	{
		return m_bBusy;
	}
	VOID					SetBusy(bool busy=true)
	{
		m_bBusy = busy;
	}
	const CHAR*				GetError();
	DBHandle				GetHandle();
private:
	BOOL					m_bConnected;					//已经连接上的状态标记
	CHAR					m_Host[HOST_STR_LEN];			//连接对端IP
	UINT					m_Port;							//连接对端端口
	CHAR					m_Database[DATABASE_STR_LEN];	//数据库名称
	CHAR					m_User[DB_USE_STR_LEN];			//用户名称
	CHAR					m_Password[DB_PASSWORD_STR_LEN];//密码
	CHAR					m_Name[CONNECTION_NAME_LEN];	//连接命名
	BOOL					m_bBusy;						//是否繁忙
	Statement				m_Statement;					//默认Sql状态
	DBHandle				m_Handle;						//数据库句柄


};



#endif
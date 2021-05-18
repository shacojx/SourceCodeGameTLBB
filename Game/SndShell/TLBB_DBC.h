/*
|==========================================
|	DBC数据库文件类
|		(服务器/客户端通用)
|==========================================
|
|		--------------------
|		|  数据库文件格式  |
|		--------------------
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|	Head
|		0X000	  CHAR[4]    DBC File Identity， always "TDBC"
|		0X004	  UINT      Number of records in the file 
|		0X008     UINT      Number of 4-byte fields per record
|		0X010     UINT      String block size 
|   FieldType
|		0X014     UINT[FieldNum]   
|							  The type fo fields(0-INT, 1-FLOAT, 2-string)
|   FieldBlock
|				  UINT[FieldNum*RecordNum]
|							  DataBlock
|	StringBlock
|				  CHAR[StringSize]
|							  StringBlock
|
*/
#pragma once

#include <vector>
#include <hash_map>

namespace DBC
{
	class DBCFile
	{
	public:
		//文件头
		struct FILE_HEAD
		{
			CHAR		m_Identify[4];			//标示	'TDBC'
			INT			m_nFieldsNum;			//列数
			INT			m_nRecordsNum;			//行数
			INT			m_nStringBlockSize;		//字符串区大小
		};

		//字段数据类型
		enum FIELD_TYPE
		{
			T_INT		= 0,	//整数
			T_FLOAT		= 1,	//浮点数
			T_STRING	= 2,	//字符串
		};

		//数据库格式描述
		typedef std::vector< FIELD_TYPE >	FILEDS_TYPE;

		//数据段
		union FIELD
		{
			FLOAT	fValue;
			INT		iValue;
			CHAR*	pString;	// Just for runtime!
		};
		//数据区
		typedef std::vector< FIELD >		DATA_BUF;

	public:
		//打开二进制文件，生成一个数据库
		BOOL					OpenFromDBC(const CHAR* szFileName);
		//打开文本文件，生成一个数据库
		BOOL					OpenFromTXT(const CHAR* szFileName);

	public:
		//查找
		virtual const FIELD*	Search_Index_EQU(INT nIndex) const;

		virtual const FIELD*	Search_Posistion(INT nRecordLine, INT nColumNum) const;

	public:
		//取得ID
		virtual					GetID(VOID) const				{ return m_ID; }
		//取得列数
		virtual INT				GetFieldsNum(VOID) const	    { return m_nFieldsNum; }
		//取得记录的条数
		virtual INT				GetRecordsNum(VOID) const		{ return m_nRecordsNum; }
		//生成索引列
		virtual VOID			CreateIndex(LPCTSTR szFileName, INT nColum = 0);

	protected:
		//数据库格式文件名
		UINT			m_ID;
		//数据库格式描述
		FILEDS_TYPE				m_theType;
		//行数
		INT						m_nRecordsNum;
		//列数
		INT						m_nFieldsNum;
		//数据区
		DATA_BUF				m_vDataBuf;		//size = m_nRecordsNum*m_nFieldsNum
		//字符串区
		CHAR*					m_pStringBuf;
		//字符串区大小
		INT						m_nStringBufSize;
		//索引表
		stdext::hash_map< INT, FIELD* >	m_hashIndex;
		//索引列
		INT						m_nIndexColum;

	public:
		static INT				_ConvertStringToVector(const CHAR* strStrINTgSource, std::vector< std::string >& vRet, const CHAR* szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty);

	public:
		DBCFile(UINT id);
		virtual ~DBCFile();
	};
}
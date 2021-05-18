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
|		0X000	  UINT		DBC File Identity， always 0XDDBBCC00
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
#ifndef _TLBB_DB_H_
#define _TLBB_DB_H_

#include <vector>
#ifdef __WINDOWS__
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include "Type.h"

namespace DBC
{
	class DBCFile	
	{
	public:
		//文件头
		struct FILE_HEAD
		{
			UINT		m_Identify;				//标示	0XDDBBCC00
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
		typedef vector< FIELD_TYPE >	FILEDS_TYPE;

		//数据段
		union FIELD
		{
			FLOAT		fValue;
			INT			iValue;
			const CHAR*	pString;	// Just for runtime!

			//Construct
			FIELD() {}
			FIELD(INT value) { iValue = value; }
			FIELD(FLOAT value) { fValue = value; }
			FIELD(const CHAR* value) { pString = value; }
		};
		//数据区
		typedef vector< FIELD >		DATA_BUF;

	public:
		//打开文本文件，生成一个数据库
		BOOL					OpenFromTXT(const CHAR* szFileName);
		//根据内存中的文件打开
		BOOL					OpenFromMemory(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName=0);
	protected:
		//读取文本格式内容
		BOOL					OpenFromMemoryImpl_Text(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName=0);
		//读取二进制格式内容
		BOOL					OpenFromMemoryImpl_Binary(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName=0);

	public:
		//按索引查找(第一列为索引)
		virtual const FIELD*	Search_Index_EQU(INT nValue) const;
		//按照位置查找
		virtual const FIELD*	Search_Posistion(INT nRecordLine, INT nColumNum) const;
		//查找某列等于指定值的第一行
		virtual const FIELD*	Search_First_Column_Equ(INT nColumnNum, const FIELD& value) const;

	public:
		//取得ID
		UINT GetID(VOID) const				{ return m_ID; }
		//取得列数
		INT	GetFieldsNum(VOID) const	    { return m_nFieldsNum; }
		//取得记录的条数
		INT GetRecordsNum(VOID) const		{ return m_nRecordsNum; }
		//生成索引列
		VOID CreateIndex(INT nColum = 0, const CHAR* szFileName=0);

	protected:
#ifdef __SGI_STL_PORT
		typedef std::hash_map< INT, FIELD*>	FIELD_HASHMAP;
#else
	#ifdef __WINDOWS__
		typedef stdext::hash_map< INT, FIELD*>	FIELD_HASHMAP;
	#else
		typedef __gnu_cxx::hash_map< INT,FIELD*>    FIELD_HASHMAP;
							  // hash_compare <INT, less<INT> > ,
							   //allocator< pair<const INT, FIELD*> > >
							  
	#endif
		
#endif
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
		FIELD_HASHMAP			m_hashIndex;
		//索引列
		INT						m_nIndexColum;

	public:

		static INT			_ConvertStringToVector(const CHAR* strStrINTgSource, vector< std::string >& vRet, const CHAR* szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty);
		//从内存中字符串读取一行文本(按照换行符)
		static const CHAR*	_GetLineFromMemory(CHAR* pStringBuf, INT nBufSize, const CHAR* pMemory, const CHAR* pDeadEnd);
		//比较两个值是否相等
		template < FIELD_TYPE T>	
		static bool			_FieldEqu(const FIELD& a, const FIELD& b);

	public:
		DBCFile(UINT id);
		virtual ~DBCFile();
	};
}

#endif
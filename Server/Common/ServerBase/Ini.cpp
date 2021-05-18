#include "stdafx.h"
//********************************************
//	Ini 相关函数
//********************************************


#include "Ini.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
////////////////////////////////////////////////
// 通用接口
////////////////////////////////////////////////

//初始化
Ini::Ini()
{
	__ENTER_FUNCTION

	m_lDataLen = 0;
	m_strData = NULL;
	IndexNum = 0;
	IndexList = NULL;

	__LEAVE_FUNCTION
}

//初始化
Ini::Ini( const CHAR *filename )
{
	__ENTER_FUNCTION

	m_lDataLen=0;
	m_strData=NULL;
	IndexNum=0;
	IndexList=NULL;
	memset( m_strFileName, 0, _MAX_PATH ) ;
	memset( m_szValue, 0, MAX_INI_VALUE ) ;
	memset( m_szRet, 0, MAX_INI_VALUE) ;

	Open(filename);

	__LEAVE_FUNCTION
}

//析构释放
Ini::~Ini()
{
	__ENTER_FUNCTION

	if( m_lDataLen != 0 )
	{
		SAFE_FREE( m_strData );
		m_lDataLen = 0;
	}

	if( IndexNum != 0 )
	{
		SAFE_DELETE( IndexList );
		IndexNum = 0;
	}

	__LEAVE_FUNCTION
}

//读入文件
BOOL Ini::Open( const CHAR *filename )
{
	__ENTER_FUNCTION

	strncpy(m_strFileName, filename, _MAX_PATH-1);

	SAFE_FREE( m_strData );

	//获取文件长度
	FILE* fp;
	fp = fopen(filename,"rb");
	if(fp == 0)
	{
		m_lDataLen = -1;
	}
	else
	{
		fseek( fp, 0L, SEEK_END );
		m_lDataLen	= ftell( fp );
		fclose(fp);
	}
	
	
	//文件存在
	if( m_lDataLen > 0 )
	{
		m_strData = (CHAR*)malloc( (size_t)m_lDataLen ) ;
		//m_strData = new CHAR[m_lDataLen];
		memset( m_strData, 0, m_lDataLen ) ;

		FILE *fp;
		fp=fopen(filename, "rb");
		AssertEx( fp!=NULL, filename );
		fread(m_strData, m_lDataLen, 1, fp);		//读数据
		fclose(fp);

		//初始化索引
		InitIndex();
		return TRUE;
	}
	else	// 文件不存在
	{
		// 找不到文件
		m_lDataLen=1;
		m_strData = (CHAR*)malloc( (size_t)m_lDataLen ) ;
		//m_strData = new CHAR[m_lDataLen];
		memset(m_strData, 0, 1);
		InitIndex();
	}

	return FALSE;

	__LEAVE_FUNCTION

	return 0 ;
}

//关闭文件
VOID Ini::Close()
{
	__ENTER_FUNCTION

	if( m_lDataLen != 0 )
	{
		SAFE_DELETE( m_strData );
		m_lDataLen = 0;
	}

	if( IndexNum != 0 )
	{
		SAFE_DELETE( IndexList );
		IndexNum = 0;
	}

	__LEAVE_FUNCTION
}

//写入文件
BOOL Ini::Save(CHAR *filename)
{
	__ENTER_FUNCTION

	if( filename==NULL )
	{
		filename=m_strFileName;
	}

	FILE *fp;
	fp=fopen(filename, "wb");
	AssertEx( fp!=NULL, filename );

	fwrite(m_strData, m_lDataLen, 1, fp);
	fclose(fp);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE ;
}

//返回文件内容
CHAR *Ini::GetData()
{
	__ENTER_FUNCTION

	return m_strData;

	__LEAVE_FUNCTION
}

//获得文件的行数
INT Ini::GetLines(INT cur)
{
	__ENTER_FUNCTION

	INT n=1;
	for(INT i=0; i<cur; i++)
	{
		if( m_strData[i]=='\n' )
			n++;
	}
	return n;

	__LEAVE_FUNCTION
}

//获得文件的行数
INT Ini::GetLines()
{
	__ENTER_FUNCTION

	INT		n = 0;
	INT		i;
	for(i=0; i<m_lDataLen; i++)
	{
		if( m_strData[i]=='\n' )
			n++;
	}
	if(i>=m_lDataLen)
		return n+1;
	return n;

	__LEAVE_FUNCTION
}

////////////////////////////////////////////////
// 内部函数
////////////////////////////////////////////////

//计算出所有的索引位置
VOID Ini::InitIndex()
{
	__ENTER_FUNCTION

	IndexNum=0;

	for(INT i=0; i<m_lDataLen; i++)
	{
		//找到
		if( m_strData[i]=='[' && ( m_strData[i-1]=='\n' || i==0 ) )
		{
			IndexNum++;
		}
	}

	//申请内存
	SAFE_DELETE( IndexList );
	if( IndexNum>0 )
		IndexList=new INT[IndexNum];

	INT n=0;

	for(INT i=0; i<m_lDataLen; i++)
	{
		if( m_strData[i]=='[' && ( m_strData[i-1]=='\n' || i==0 ) )
		{
			IndexList[n]=i+1;
			n++;
		}
	}

	__LEAVE_FUNCTION
}

//返回指定标题位置
INT Ini::FindIndex(CHAR *string)
{
	__ENTER_FUNCTION

	for(INT i=0; i<IndexNum; i++)
	{
		CHAR *str=ReadText( IndexList[i] );
		if( strcmp(string, str) == 0 )
		{
//			SAFE_FREE( str );
			return IndexList[i];
		}
//		SAFE_FREE( str );
	}
	return -1;

	__LEAVE_FUNCTION

	return 0 ;
}

//返回指定数据的位置
INT Ini::FindData(INT index, CHAR *string)
{
	__ENTER_FUNCTION

	INT p=index;	//指针

	while(1)
	{
		p=GotoNextLine(p);
		CHAR *name=ReadDataName(p);
		if( strcmp(string, name)==0 )
		{
			SAFE_DELETE( name );
			return p;
		}

		if ( name[0] == '[' )
		{
			SAFE_DELETE( name );
			return -1;
		}

		SAFE_DELETE( name );
		if( p>=m_lDataLen ) return -1;
	}
	return -1;

	__LEAVE_FUNCTION

	return 0 ;
}

//提行
INT Ini::GotoNextLine(INT p)
{
	__ENTER_FUNCTION

	INT i;
	for(i=p; i<m_lDataLen; i++)
	{
		if( m_strData[i]=='\n' )
			return i+1;
	}
	return i;

	__LEAVE_FUNCTION
}

//在指定位置读一数据名称
CHAR *Ini::ReadDataName(INT &p)
{
	__ENTER_FUNCTION

	CHAR chr;
	CHAR *Ret;
	INT m=0;

	Ret=new CHAR[64];
	memset(Ret, 0, 64);

	for(INT i=p; i<m_lDataLen; i++)
	{
		chr = m_strData[i];

		//结束
		if( chr == '\r' )
		{
			p=i+1;
			return Ret;
		}
		
		//结束
		if( chr == '=' || chr == ';' )
		{
			p=i+1;
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
	}
	return Ret;

	__LEAVE_FUNCTION

	return 0 ;
}

//在指定位置读一字符串
CHAR *Ini::ReadText(INT p)
{
	__ENTER_FUNCTION

	CHAR chr;
	CHAR *Ret;
	INT n=p, m=0;

	INT LineNum = GotoNextLine(p) - p + 1;
	Ret=(CHAR*)m_szValue;//new CHAR[LineNum];
	memset(Ret, 0, LineNum);

	for(INT i=0; i<m_lDataLen-p; i++)
	{
		chr = m_strData[n];

		//结束
		if( chr == ';' || chr == '\r' || chr == '\t' || chr == ']' )
		{
			//ShowMessage(Ret);
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
		n++;
	}

	return Ret;

	__LEAVE_FUNCTION

	return 0 ;
}

//加入一个索引
BOOL Ini::AddIndex(CHAR *index)
{
	__ENTER_FUNCTION

	CHAR str[256];
	memset(str, 0, 256);
	INT n=FindIndex(index);

	if( n == -1 )	//新建索引
	{
		sprintf(str,"\r\n[%s]",index);
		m_strData = (CHAR *)realloc(m_strData, m_lDataLen+strlen(str));	//重新分配内存
		sprintf(&m_strData[m_lDataLen], "%s", str);
		m_lDataLen+=(long)(strlen(str));

		InitIndex();
		return TRUE;
	}
	
	return FALSE;	//已经存在

	__LEAVE_FUNCTION

	return 0 ;
}

//在当前位置加入一个数据
BOOL Ini::AddData(INT p, CHAR *name, CHAR *string)
{
	__ENTER_FUNCTION

	CHAR *str;
	INT len=(INT)(strlen(string));
	str=new CHAR[len+256];
	memset(str, 0, len+256);
	sprintf(str,"%s=%s",name,string);
	len=(INT)(strlen(str));

	p=GotoNextLine(p);	//提行
	m_strData = (CHAR *)realloc(m_strData, m_lDataLen+len);	//重新分配内存

	CHAR *temp=new CHAR[m_lDataLen-p];
	memcpy(temp, &m_strData[p], m_lDataLen-p);
	memcpy(&m_strData[p+len], temp, m_lDataLen-p);	//把后面的搬到末尾
	memcpy(&m_strData[p], str, len);
	m_lDataLen+=len;

	SAFE_DELETE( temp );
	SAFE_DELETE( str );
	return TRUE;

	__LEAVE_FUNCTION

	return 0 ;
}

//在当前位置修改一个数据的值
BOOL Ini::ModityData(INT p, CHAR *name, CHAR *string)
{
	__ENTER_FUNCTION

	INT n=FindData(p, name);

	CHAR *t=ReadText(n);
	p=n+(INT)(strlen(t));
//	if( strlen(t)>0 ) free(t);

	INT newlen=(INT)(strlen(string));
	INT oldlen=p-n;

	m_strData = (CHAR *)realloc(m_strData, m_lDataLen+newlen-oldlen);	//重新分配内存

	CHAR *temp=new CHAR[m_lDataLen-p];
	memcpy(temp, &m_strData[p], m_lDataLen-p);
	memcpy(&m_strData[n+newlen], temp, m_lDataLen-p);			//把后面的搬到末尾
	memcpy(&m_strData[n], string, newlen);
	m_lDataLen+=newlen-oldlen;

	SAFE_DELETE( temp );
	return TRUE;

	__LEAVE_FUNCTION

	return 0 ;
}

//把指针移动到本INDEX的最后一行
INT Ini::GotoLastLine(CHAR *index)
{
	__ENTER_FUNCTION

	INT n=FindIndex(index);
	n=GotoNextLine(n);
	while(1)
	{
		if( m_strData[n] == '\r' || m_strData[n] == EOF || m_strData[n] == -3 || m_strData[n] == ' ' || m_strData[n] == '/' || m_strData[n] == '\t' || m_strData[n] == '\n' )
		{
			return n;
		}
		else
		{
			n=GotoNextLine(n);
			if( n >= m_lDataLen ) return n;
		}
	}

	__LEAVE_FUNCTION

	return 0 ;
}

/////////////////////////////////////////////////////////////////////
// 对外接口
/////////////////////////////////////////////////////////////////////

//以普通方式读一字符串数据
CHAR *Ini::ReadText(CHAR *index, CHAR *name, CHAR* str, INT size)
{
	__ENTER_FUNCTION

	CHAR szTmp[512] ;
	memset( szTmp, 0, 512 ) ;
	sprintf( szTmp, "[%s][%s][%s]", m_strFileName, index, name ) ;

	INT n=FindIndex(index);
	AssertEx( n != -1, szTmp );

	if ( n == -1 )
		return NULL;

	INT m=FindData(n, name);
	AssertEx( m != -1, szTmp );
	if ( m == -1 )
		return NULL;

	CHAR* ret = ReadText(m);
	strncpy( str, ret, size ) ;
	return ret ;

	__LEAVE_FUNCTION

	return 0 ;
}

//如果存在则读取
BOOL Ini::ReadTextIfExist(CHAR *index, CHAR *name, CHAR* str, INT size)
{
__ENTER_FUNCTION

	INT n = FindIndex(index);
	
	if( n == -1 )
		return FALSE;

	INT m = FindData(n, name);
	
	if( m == -1 )
		return FALSE;

	CHAR* ret = ReadText(m);
	strncpy( str, ret, size );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE ;
}
	
//在指定的行读一字符串
CHAR *Ini::ReadText(CHAR *index, INT lines, CHAR* str, INT size)
{
	__ENTER_FUNCTION

	CHAR szTmp[512] ;
	memset( szTmp, 0, 512 ) ;
	sprintf( szTmp, "[%s][%s][%d]", m_strFileName, index, lines ) ;


	INT n=FindIndex(index);
	AssertEx( n != -1, szTmp );

	//跳到指定行数
	n=GotoNextLine(n);
	for(INT i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	//读数据
	while( n<=m_lDataLen )
	{
		if( m_strData[n] == '=' )
		{
			n++;
			CHAR* ret = ReadText(n);
			strncpy( str, ret, size ) ;
			return ret ;
		}
		if( m_strData[n] == '\r' )
		{
			return NULL;
		}
		n++;
	}

	return NULL;

	__LEAVE_FUNCTION

	return 0 ;
}

//以普通方式读一整数
INT Ini::ReadInt(CHAR *index, CHAR *name)
{
	__ENTER_FUNCTION

	CHAR szTmp[512] ;
	memset( szTmp, 0, 512 ) ;
	sprintf( szTmp, "[%s][%s][%s]", m_strFileName, index, name ) ;

	INT n=FindIndex(index);
	AssertEx( n != -1, szTmp );

	INT m=FindData(n, name);
	AssertEx( m != -1, szTmp );

	CHAR *str=ReadText(m);
	INT ret=atoi(str);
//	free(str);
	return ret;

	__LEAVE_FUNCTION

	return 0 ;
}

BOOL Ini::ReadIntIfExist(CHAR *section, CHAR *key, INT& nResult)
{
__ENTER_FUNCTION

	INT n=FindIndex(section);

	if( n == -1 )
		return FALSE;

	INT m=FindData(n, key);

	if( m == -1 )
		return FALSE;

	CHAR *str=ReadText(m);
	nResult=atoi(str);
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

//在指定的行读一整数
INT Ini::ReadInt(CHAR *index, INT lines)
{
	__ENTER_FUNCTION

	CHAR szTmp[512] ;
	memset( szTmp, 0, 512 ) ;
	sprintf( szTmp, "[%s][%s][%d]", m_strFileName, index, lines ) ;

	INT n=FindIndex(index);
	AssertEx( n != -1, szTmp );

	//跳到指定行数
	n=GotoNextLine(n);
	for(INT i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	//读数据
	while( n<m_lDataLen )
	{
		if( m_strData[n] == '=' )
		{
			n++;
			CHAR *str=ReadText(n);
			INT ret=atoi(str);
//			free(str);
			return ret;
		}
		if( m_strData[n] == '\r' )
		{
			return ERROR_DATA;
		}
		n++;
	}

	return ERROR_DATA;

	__LEAVE_FUNCTION

	return 0 ;
}

//在指定的行读一数据名称
CHAR *Ini::ReadCaption(CHAR *index, INT lines, CHAR* str, INT size)
{
	__ENTER_FUNCTION

	CHAR szTmp[512] ;
	memset( szTmp, 0, 512 ) ;
	sprintf( szTmp, "[%s][%s][%d]", m_strFileName, index, lines ) ;

	INT n=FindIndex(index);
	AssertEx( n != -1, szTmp );

	//跳到指定行数
	n=GotoNextLine(n);
	for(INT i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	CHAR* ret = ReadDataName(n);
	strncpy( str, ret, size ) ;
	return ret ;

	__LEAVE_FUNCTION

	return 0 ;
}

//以普通方式写一字符串数据
BOOL Ini::Write(CHAR *index, CHAR *name, CHAR *string)
{
	__ENTER_FUNCTION

	INT n=FindIndex(index);
	if( n == -1 )	//新建索引
	{
		AddIndex(index);
		n=FindIndex(index);
		n=GotoLastLine(index);
		AddData(n, name, string);	//在当前位置n加一个数据
		return TRUE;
	}

	//存在索引
	INT m=FindData(n, name);
	if( m==-1 )		//新建数据
	{
		n=GotoLastLine(index);
		AddData(n, name, string);	//在当前位置n加一个数据
		return TRUE;
	}

	//存在数据
	ModityData(n, name, string);	//修改一个数据

	return TRUE;

	__LEAVE_FUNCTION

	return 0 ;
}

//以普通方式写一整数
BOOL Ini::Write(CHAR *index, CHAR *name, INT num)
{
	__ENTER_FUNCTION

	CHAR string[32];
	sprintf(string, "%d", num);

	INT n=FindIndex(index);
	if( n == -1 )	//新建索引
	{
		AddIndex(index);
		n=FindIndex(index);
		n=GotoLastLine(index);
		AddData(n, name, string);	//在当前位置n加一个数据
		return TRUE;
	}

	//存在索引
	INT m=FindData(n, name);
	if( m==-1 )		//新建数据
	{
		n=GotoLastLine(index);
		AddData(n, name, string);	//在当前位置n加一个数据
		return TRUE;
	}

	//存在数据
	ModityData(n, name, string);	//修改一个数据

	return TRUE;

	__LEAVE_FUNCTION

	return 0 ;
}

//返回连续的行数
INT Ini::GetContinueDataNum(CHAR *index)
{
	__ENTER_FUNCTION

	INT num=0;
	INT n=FindIndex(index);
	n=GotoNextLine(n);
	while(1)
	{
		if( m_strData[n] == '\r' || m_strData[n] == EOF || m_strData[n] == -3 || m_strData[n] == ' ' || m_strData[n] == '/' || m_strData[n] == '\t' || m_strData[n] == '\n' )
		{
			return num;
		}
		else
		{
			num++;
			n=GotoNextLine(n);
			if( n >= m_lDataLen ) return num;
		}
	}

	__LEAVE_FUNCTION

	return 0 ;
}
//在指定行读一字符串
CHAR* Ini::ReadOneLine(INT p)
{
__ENTER_FUNCTION

	INT start = FindOneLine(p);
	memset(m_szRet,0,sizeof(m_szRet));
	

	for(INT i=start; i<m_lDataLen; i++)
	{
		if( m_strData[i]=='\n' || m_strData[i]=='\0')
		{
			memset(m_szRet,0,sizeof(m_szRet));
			strncpy(m_szRet,&m_strData[start],i-start);
			m_szRet[i-start] = '\0';
			return m_szRet;
		}

	}

__LEAVE_FUNCTION

	return m_szRet;
}
INT Ini::FindOneLine(INT p)
{
	CHAR*	Ret = NULL;
	INT		n = 0;
	INT     m = 0;
	if(p==0)	return -1;
	if(p==1)	return 0;
	for(INT i=0; i<m_lDataLen; i++)
	{
		if ( m_strData[i]=='\n' )
			n++;
		if ( n==p-1 )				//找到要了要找的的行
			return i+1;
	}

	return -1; //没有找到
}
INT Ini::ReturnLineNum(CHAR* string)
{
	INT p = FindIndex(string);
	CHAR*	Ret = NULL;
	INT		n = 0;
	INT     m = 0;
	if(p==0)	return -1;
	if(p==1)	return 0;
	for(INT i=0; i<p; i++)
	{
		if ( m_strData[i]=='\n' )
			n++;
	}
	return n;

	return -1; //没有找到
}


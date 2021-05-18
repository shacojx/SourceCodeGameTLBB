
//********************************************
//	Ini 相关函数
//********************************************

#ifndef __INI_H__
#define __INI_H__

#include "..\..\Common\Type.h"



#define ERROR_DATA -99999999
#define MAX_INI_VALUE 1024

//配置文件类
class Ini
{
////////////////////////////////////////////////
// 内部数据
////////////////////////////////////////////////
private:
	CHAR			m_strFileName[_MAX_PATH];	//文件名
	long			m_lDataLen;					//文件长度
	CHAR*			m_strData;					//文件内容

	INT				IndexNum;					//索引数目（[]的数目）
	INT*			IndexList;					//索引点位置列表
	INT				Point;						//当前指针
	INT				Line, Word;					//当前行列

	CHAR			m_szValue[MAX_INI_VALUE] ;
	CHAR			m_szRet[MAX_INI_VALUE];

////////////////////////////////////////////////
// 通用接口
////////////////////////////////////////////////
public:
	Ini();
	Ini(const CHAR* filename);								//初始化打开配置文件
	virtual ~Ini();									//释放内存
	CHAR			*GetData();							//返回文件内容
	INT				GetLines(INT);						//返回文件的行数
	INT				GetLines();						//返回文件的行数

	BOOL			Open(const CHAR* filename);				//打开配置文件
	VOID			Close();							//关闭配置文件
	BOOL			Save(CHAR *filename=NULL);			//保存配置文件
	INT				FindIndex(CHAR *);					//返回标题位置

////////////////////////////////////////////////
// 内部函数
////////////////////////////////////////////////
private:
	VOID		InitIndex();						//初始化索引
	INT			FindData(INT, CHAR *);				//返回数据位置
	INT			GotoNextLine(INT); 					//提行
	CHAR*		ReadDataName(INT &);				//在指定位置读一数据名称
	CHAR*		ReadText(INT);						//在指定位置读字符串

	BOOL		AddIndex(CHAR *);					//加入一个索引
	BOOL		AddData(INT, CHAR *, CHAR *);		//在当前位置加入一个数据
	BOOL		ModityData(INT, CHAR *, CHAR *);	//在当前位置修改一个数据的值
	INT			GotoLastLine(CHAR *section);			//把指针移动到本INDEX的最后一行

////////////////////////////////////////////////
// 公用接口
////////////////////////////////////////////////
public:
	//读一个整数
	INT				ReadInt(CHAR *section, CHAR *key);
	//如果存在，则读一个整数
	BOOL			ReadIntIfExist(CHAR *section, CHAR *key, INT& nResult);
	//在指定的行读一整数
	INT				ReadInt(CHAR *section, INT lines);	
	//读一个字符串
	CHAR*			ReadText(CHAR *section, CHAR *key, CHAR* str, INT size);
	//如果存在则读取
	BOOL			ReadTextIfExist(CHAR *section, CHAR *key, CHAR* str, INT size);
	//在指定的行读一字符串
	CHAR*			ReadText(CHAR *section, INT lines, CHAR* str, INT size);	
	//在指定行读一字符名称
	CHAR*			ReadCaption(CHAR *section, INT lines, CHAR* str, INT size);
	//写一个整数
	BOOL			Write(CHAR *section, CHAR *key, INT num);			
	//写一个字符串
	BOOL			Write(CHAR *section, CHAR *key, CHAR *string);		
	//返回连续的行数（从INDEX到第一个空行）
	INT				GetContinueDataNum(CHAR *section);	
	//在指定位置读字符串
	CHAR*			ReadOneLine(INT);
	INT				FindOneLine(INT);
	//返回指定字符所在的行数
	INT				ReturnLineNum(CHAR*);
};



#endif




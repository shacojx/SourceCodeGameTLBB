/*
对记录进行操作的工具类，自己不保存任何信息只是一些函数的集合
*/

#ifndef _RECORD_OPT_H_
#define _RECORD_OPT_H_

#include "Type.h"
#include "Record.h"	
enum OPT_RECORD
{
	REC_EXCHANGEITEM = 0,			//交易记录购买物品
	REC_EXCHANGEPET,				//交易记录购买宠物
	REC_ADDITEM,					//上货
	REC_DELITEM,					//下货
	REC_ADDPET,						//上货
	REC_DELPET,						//下货
	REC_ONSALEITEM,					//上架
	REC_OFFSALEITEM,				//下架
	REC_ONSALEPET,					//上架
	REC_OFFSALEPET,					//下架
	REC_OPEN,						//开张
	REC_CLOSE,						//打烊
	REC_INPUT_BASE,					//冲入本金
	REC_INPUT_PROFIT,				//冲入盈利金



	MAX_RECORD,
};

namespace RecordOpt
{
	//-------------------------------------------------------------------------------------------------
	//初始化保存容器
	BOOL		InitRecordContainer(RecordContainer_t* pContainer, INT iSize);

	//-------------------------------------------------------------------------------------------------
	//获得当前记录容器中的记录数量
	INT			GetRecordNum(RecordContainer_t* pContainer);

	//-------------------------------------------------------------------------------------------------
	//操作容器
	BOOL		Excute( OPT_RECORD Opt, RecordContainer_t* pContainer, ... );

	//-------------------------------------------------------------------------------------------------
	//枚举容器中的条目
	VOID		BeginEnumEntry(RecordContainer_t* pContainer);
	CHAR*		EnumEntry( RecordContainer_t* pContainer );
	VOID		EndEnumEntry(RecordContainer_t* pContainer);
};

#endif
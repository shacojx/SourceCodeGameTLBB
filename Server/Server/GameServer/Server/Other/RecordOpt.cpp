#include "stdafx.h"

#include "RecordOpt.h"
#include "TimeManager.h"//g_pTimeManager
#include "Log.h"
using namespace RecordOpt;
//-------------------------------------------------------------------------------------------------
//初始化保存容器
BOOL RecordOpt::InitRecordContainer(RecordContainer_t* pContainer, INT iSize)
{
	if(iSize>MAX_RECORD_NUM_ENTRY)
		return FALSE;
	pContainer->CleanUp();
	pContainer->m_iMaxSize		= iSize;
	pContainer->m_iCurEntry		= 0;
	pContainer->m_iOldestEntry	= 0;
	pContainer->m_iTempEntry	= 0;
	return TRUE;
}

//-------------------------------------------------------------------------------------------------
//获得当前记录容器中的记录数量
INT RecordOpt::GetRecordNum(RecordContainer_t* pContainer)
{
	if(pContainer->m_iCurEntry==(pContainer->m_iMaxSize - 1) && pContainer->m_iOldestEntry == 0)
	{
		return pContainer->m_iMaxSize;
	}

	if(pContainer->m_iCurEntry>=pContainer->m_iOldestEntry)
	{
		return pContainer->m_iCurEntry - pContainer->m_iOldestEntry;
	}
	else
	{
		return pContainer->m_iMaxSize;
	}
}

//-------------------------------------------------------------------------------------------------
//操作容器
/*
伙伴操作行为格式：
上货：AAA上货了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
取回：AAA取回了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
上架：AAA上架了BBBC件，标价为（换行）？金？银？铜（AAA为操作者名，BBB为商品名C为件数）
下架：AAA下架了BBBC件，原标价为（换行）？金？银？铜（AAA为操作者名，BBB为商品名C为件数）
开张：AAA开张了第N间柜台（AAA为操作者名，N为柜台所属间数）
打烊：AAA打烊了第N间柜台（AAA为操作者名，N为柜台所属间数）
充入本金：AAA往店铺本金中充入（换行）？金？银？铜（AAA为操作者名）
充入赢利资金：AAA往店铺赢利资金中充入（换行）？金？银？铜（AAA为操作者名）

*/
BOOL RecordOpt::Excute( OPT_RECORD Opt, RecordContainer_t* pContainer, ... )
{
	__ENTER_FUNCTION
	if(Opt<0||Opt>=MAX_RECORD)
		return FALSE;
	if(pContainer == NULL)
		return FALSE;

    va_list ParamList;
	va_start(ParamList, pContainer);

	pContainer->m_szRecordContent[pContainer->m_iCurEntry].CleanUp();
	g_pTimeManager->SetTime();
	UINT	iCurTime = pContainer->m_szRecordContent[pContainer->m_iCurEntry].m_iTime = g_pTimeManager->Time2DWORD();
	CHAR*	szInputBuffer = static_cast<CHAR*>(pContainer->m_szRecordContent[pContainer->m_iCurEntry].m_szRecord);
	
	switch(Opt)
	{
	case REC_EXCHANGEITEM:
		{
			//售出AA，获得？金？银？铜。（AA为物品名）
			INT iItemType = va_arg( ParamList, INT);
			INT iItemNum  = va_arg( ParamList, INT);
			INT	iCost	  = va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: 售出[#{_ITEM%d}]X%d, 获得#{_MONEY%d}", iCurTime, iItemType, iItemNum, iCost);
		}
		break;
	case REC_EXCHANGEPET:
		{
			//售出AA，获得？金？银？铜。（AA为物品名）
			CHAR*	pszPetName	= va_arg( ParamList, CHAR*);
			INT		iItemNum	= va_arg( ParamList, INT);
			INT		iCost		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: 售出[%s]X%d, 获得#{_MONEY%d}", iCurTime, pszPetName, iItemNum, iCost);
		}
		break;
	case REC_ADDITEM:
		{
			//上货：AAA上货了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iItemType		= va_arg( ParamList, INT);
			INT		iItemNum		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s上货了[#{_ITEM%d}]X%d件", iCurTime, pszOperatorName, iItemType, iItemNum);
		}
		break;
	case REC_DELITEM:
		{
			//取回：AAA取回了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iItemType		= va_arg( ParamList, INT);
			INT		iItemNum		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s取回了[#{_ITEM%d}]X%d件", iCurTime, pszOperatorName, iItemType, iItemNum);
		}
		break;
	case REC_ADDPET:
		{
			//上货：AAA上货了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			CHAR*	pszPetName		= va_arg( ParamList, CHAR*);
			INT		iItemNum		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s上货了[%s]X%d件", iCurTime, pszOperatorName, pszPetName, iItemNum);
		}
		break;
	case REC_DELPET:
		{
			//取回：AAA取回了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			CHAR*	pszPetName		= va_arg( ParamList, CHAR*);
			INT		iItemNum		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s取回了[%s]X%d件", iCurTime, pszOperatorName, pszPetName, iItemNum);
		}
		break;
	case REC_ONSALEITEM:
		{
			//上架：AAA上架了BBBC件，标价为（换行）？金？银？铜（AAA为操作者名，BBB为商品名C为件数）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iItemType		= va_arg( ParamList, INT);
			INT		iItemNum		= va_arg( ParamList, INT);
			INT		iPrice			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s上架了[#{_ITEM%d}]X%d件,标价为#r #{_MONEY%d}", iCurTime, pszOperatorName, iItemType, iItemNum, iPrice);
		}
		break;
	case REC_OFFSALEITEM:
		{
			//下架：AAA下架了BBBC件，原标价为（换行）？金？银？铜（AAA为操作者名，BBB为商品名C为件数）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iItemType		= va_arg( ParamList, INT);
			INT		iItemNum		= va_arg( ParamList, INT);
			INT		iPrice			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s上架了[#{_ITEM%d}]X%d件,原标价为#r #{_MONEY%d}", iCurTime, pszOperatorName, iItemType, iItemNum, iPrice);
		}
		break;
	case REC_ONSALEPET:
		{
			//上架：AAA上架了BBBC件，标价为（换行）？金？银？铜（AAA为操作者名，BBB为商品名C为件数）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			CHAR*	pszPetName		= va_arg( ParamList, CHAR*);
			INT		iItemNum		= va_arg( ParamList, INT);
			INT		iPrice			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s上架了[%s]X%d件,标价为#r #{_MONEY%d}", iCurTime, pszOperatorName, pszPetName, iItemNum, iPrice);
		}
		break;
	case REC_OFFSALEPET:
		{
			//下架：AAA下架了BBBC件，原标价为（换行）？金？银？铜（AAA为操作者名，BBB为商品名C为件数）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			CHAR*	pszPetName		= va_arg( ParamList, CHAR*);
			INT		iItemNum		= va_arg( ParamList, INT);
			INT		iPrice			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s上架了[%s]X%d件,原标价为#r #{_MONEY%d}", iCurTime, pszOperatorName, pszPetName, iItemNum, iPrice);
		}
		break;
	case REC_OPEN:
		{
			//开张：AAA开张了第N间柜台（AAA为操作者名，N为柜台所属间数）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iStallIndex		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s开张了第%d间柜台", iCurTime, pszOperatorName, iStallIndex);
		}
		break;
	case REC_CLOSE:
		{
			//打烊：AAA打烊了第N间柜台（AAA为操作者名，N为柜台所属间数）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iStallIndex		= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s打烊了第%d间柜台", iCurTime, pszOperatorName, iStallIndex);
		}
		break;
	case REC_INPUT_BASE:
		{
			//充入本金：AAA往店铺本金中充入（换行）？金？银？铜（AAA为操作者名）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iMoney			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s往店铺本金中充入#r #{_MONEY%d}", iCurTime, pszOperatorName, iMoney);
		}
		break;
	case REC_INPUT_PROFIT:
		{
			//充入赢利资金：AAA往店铺赢利资金中充入（换行）？金？银？铜（AAA为操作者名）
			CHAR*	pszOperatorName = va_arg( ParamList, CHAR*);
			INT		iMoney			= va_arg( ParamList, INT);
			sprintf(szInputBuffer, "#{_TIME%d}: %s往店铺赢利资金中充入#r #{_MONEY%d}", iCurTime, pszOperatorName, iMoney);
		}
		break;

	default:
		{
			va_end( ParamList );  
			g_pLog->FastSaveLog( LOG_FILE_1, "记录出错" ) ;
			return FALSE;
		}
		break;
	}

	pContainer->m_iCurEntry++;
	if(pContainer->m_iCurEntry == pContainer->m_iMaxSize)
	{
		pContainer->m_iCurEntry = 0;
	}
	if(pContainer->m_iCurEntry == pContainer->m_iOldestEntry)
	{
		pContainer->m_iOldestEntry++;
		if(pContainer->m_iOldestEntry == pContainer->m_iMaxSize)
		{
			pContainer->m_iOldestEntry = 0;
		}
	}
	va_end( ParamList );  
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

//-------------------------------------------------------------------------------------------------
//枚举容器中的条目
VOID RecordOpt::BeginEnumEntry(RecordContainer_t* pContainer)
{
	pContainer->m_iTempEntry = pContainer->m_iOldestEntry;
}

CHAR* RecordOpt::EnumEntry( RecordContainer_t* pContainer )
{
	__ENTER_FUNCTION

	if(pContainer->m_iTempEntry == pContainer->m_iCurEntry)
	{
		return NULL;
	}

	INT iReturnIndex = pContainer->m_iTempEntry++;
	if(pContainer->m_iTempEntry == pContainer->m_iMaxSize)
	{
		pContainer->m_iTempEntry = 0;
	}
	return pContainer->m_szRecordContent[iReturnIndex].m_szRecord;

	__LEAVE_FUNCTION
	return NULL;

}

VOID RecordOpt::EndEnumEntry(RecordContainer_t* pContainer)
{
	pContainer->m_iTempEntry = pContainer->m_iCurEntry;
}

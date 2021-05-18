#include "stdafx.h"


#include "TLBB_DBC.h"
#include "XinfaStudySpendMgr.h"
#include "FileDef.h"

XinfaStudySpendMgr* g_pXinfaStudySpendMgr=NULL ;

XinfaStudySpendMgr::XinfaStudySpendMgr()
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}

XinfaStudySpendMgr::~XinfaStudySpendMgr()
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}

// 从文件".\PUBLIC\Config\XinFaStudySpend_V1.txt"中读取数据
BOOL XinfaStudySpendMgr::Init()
{
__ENTER_FUNCTION
	DBC::DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_XINFA_STUDY_SPEND );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	Assert(iTableCount <= MAX_XINFA_LEVEL_NUM);

	m_nMaxLevel = iTableCount;

	for(INT i=0; i<iTableCount; ++i)
	{
		m_pXinfaLevel[i+1].m_uMoney		= ThirdFile.Search_Posistion(i,1)->iValue;
		m_pXinfaLevel[i+1].m_uExperience	= ThirdFile.Search_Posistion(i,2)->iValue;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}


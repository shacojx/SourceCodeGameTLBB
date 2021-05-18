//
// 心法学习中消耗（金钱和经验值）的管理
// 记录心法每个级别学习的时候需要化肥的金钱和经验
// 所有门派心法的升级花费是相同的
//--------------------------------------------------
#ifndef __XINFASTUDYSPENDMGR_H__
#define __XINFASTUDYSPENDMGR_H__

#include "Type.h"
#include "GameDefine.h"

class XinfaStudySpendMgr
{
public :
	struct StudySpend
	{
		UINT			m_uMoney;
		UINT			m_uExperience;

		StudySpend()
		{
			m_uMoney			= 0;
			m_uExperience		= 0;
		};
	};

	XinfaStudySpendMgr();
	~XinfaStudySpendMgr();

	StudySpend		GetStudySpend(INT nXinfaLevel){
		return m_pXinfaLevel[nXinfaLevel];
	}

	BOOL			Init();

private :
	INT				m_nMaxLevel;
	StudySpend		m_pXinfaLevel[MAX_XINFA_LEVEL_NUM + 1];
	
};

extern XinfaStudySpendMgr* g_pXinfaStudySpendMgr;

#endif

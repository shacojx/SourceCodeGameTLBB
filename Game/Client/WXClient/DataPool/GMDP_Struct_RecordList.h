
#pragma once

#include "Type.h"
//------------
// 交易记录，或者操作记录
//------------
//可任命的帮派职位
struct RecordEntry_t
{
	STRING	szContent; 
	RecordEntry_t()
	{
		szContent = "";
	}
};

struct RecordList_t
{
	INT								m_nRecordNum;
	std::vector< RecordEntry_t >	m_RecordEntryList;

	RecordList_t()
	{
		CleanUp();
	}
	VOID	CleanUp()
	{
		m_nRecordNum = 0;
		RecordEntry_t	Recordinital;
		m_RecordEntryList.resize(MAX_RECORD_NUM_PER_PAGE, Recordinital);			//交易记录，或者操作记录
	}
};
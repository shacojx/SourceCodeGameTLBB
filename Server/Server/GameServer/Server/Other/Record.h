/*
操作记录：用来记录所有的用户操作供用户查询，
不依赖任何结构，只是一个简单的存储结构，必须用RecordOpt操作
*/

#ifndef _RECORD_H_
#define _RECORD_H_

//-------------------------------------------------------------------------------------------------
//每条记录
struct RecordEntry_t
{
	CHAR	m_szRecord[MAX_RECORD_LEN_ENTRY];
	INT		m_iTime;
	RecordEntry_t()
	{
		CleanUp();
	}
	VOID	CleanUp()
	{
		memset(m_szRecord, 0, MAX_RECORD_LEN_ENTRY);
		m_iTime = 0;
	}
};

//-------------------------------------------------------------------------------------------------
//记录保存容器
struct	RecordContainer_t
{
	RecordEntry_t	m_szRecordContent[MAX_RECORD_NUM_ENTRY];	
	INT				m_iMaxSize;
	INT				m_iCurEntry;
	INT				m_iOldestEntry;
	INT				m_iTempEntry;

	VOID	CleanUp()
	{
		memset(this, 0, sizeof(RecordContainer_t));
	}
};
#endif
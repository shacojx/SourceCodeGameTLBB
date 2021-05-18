/*
摊位信息管理器，每个场景挂一个。
负责摊位信息文件的读取。
摊位费用的管理，包括第一次摆摊的摊位费，每次交易的交易税，此地是否可以摆摊。
*/

#ifndef _STALLINFO_MANAGER_H_
#define _STALLINFO_MANAGER_H_
#include "Type.h"

/*
单个地形块的摊位信息
*/
struct StallInfoData_t
{
	StallInfoData_t()
	{
		memset(this, 0, sizeof(StallInfoData_t));
	}

	BYTE	m_bCanStall;	//是否可以摆摊
	BYTE	m_iTradeTax;	//交易税
	UINT	m_dwPosTax;		//摊位费
	BYTE	m_iExtraInfo;	//附加信息

	StallInfoData_t& operator=(const StallInfoData_t& stallInfo)
	{
		m_bCanStall		= stallInfo.m_bCanStall;
		m_iTradeTax		= stallInfo.m_iTradeTax;
		m_dwPosTax		= stallInfo.m_dwPosTax;
		m_iExtraInfo	= stallInfo.m_iExtraInfo;
	}
};

/*
StallInfoManager,负责摊位数据的读取，访问。
*/
class StallInfoManager
{
public:
	StallInfoManager();
	~StallInfoManager();
public:
	//-------------------------------------------------------------------------------------------------
	//Init里分配内存，对应在CleanUp中释放内存
	BOOL		Init(UINT uMapWidth = 0, UINT uMapHeight = 0);
	VOID		CleanUp();

	//-------------------------------------------------------------------------------------------------
public:
	BOOL				Load( CHAR* filename ) ;
	BOOL				CanStall(UINT pos_x, UINT pos_y);
	VOID				SetCanStall(UINT pos_x, UINT pos_y, BOOL bIsCanStall);
	BYTE				StallExchangeTax(UINT pos_x, UINT pos_y);
	UINT				StallPosPayment(UINT pos_x, UINT pos_y);
	BYTE				StallExtraInfo(UINT pos_x, UINT pos_y);

protected:
	StallInfoData_t**	m_StallMap;
	UINT				m_nMapHeight;
	UINT				m_nMapWidth;
};

#endif
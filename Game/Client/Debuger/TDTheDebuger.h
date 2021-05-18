#pragma once

#include "TDDebuger.h"
class CDlgDebuger;
class TDDebuger : public tDebuger
{
public:
	//节点初始化
	virtual void	Initial(void*);
	//逻辑轮循函数
	virtual void	Tick(void);
	//释放自己所所拥有的资源
	virtual void	Release(void);

	virtual void	InfoTrack_Reset(INT nWidth, INT nHeight);
	virtual void	InfoTrack_AddPoint(INT nX, INT nY);

protected:
	CDlgDebuger*		m_pDlgDebuger;

public:
	TDDebuger(void);
	virtual ~TDDebuger(void);

	TD_DECLARE_DYNAMIC(TDDebuger);
};

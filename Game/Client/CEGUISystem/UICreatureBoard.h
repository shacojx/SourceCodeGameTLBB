/****************************\
*							*
*	 角色头顶上的信息UI		*
*							*
\***************************/
#pragma once
#include "TDUISystem.h"
#include "ceguistring.h"

namespace CEGUI
{
	class Window;
};

enum PLAYERINFO_STATE
{
	STATE_NORMAL = 0,
	STATE_DIE,
};

class CCreatureBoard : public tCreatureBoard
{
public:
	// 称号类型
	enum TITLE_TYPE
	{
		DIMNESS_TITLE = 0,
		BANGPAI_TITLE,
		SELF_ADD_TITLE,
		MENPAI_TITLE,
		MOOD_TITLE,
		PLAYERSHOP_TITLE,

		MAX_TITLE,
	};
	enum  BANGPAI_POSITION
	{
		NO_POS = 0,
		EXCELLENCE_POS,	    // 精英
		OFFICEHOLDER_POS,	// 官员
		CHAIRMAN_POS,		// 帮主

		MAX_POS,
	};

	//删除自身
	virtual VOID				Destroy(VOID);
	//设置在屏幕上的坐标(像素)
	virtual VOID				SetPosition(FLOAT nX, FLOAT nY);
	//显示/隐藏
	virtual VOID				Show(BOOL bShow);

	//设置名字
	virtual VOID				SetElement_Name(LPCTSTR szName);	
	//设置称号
	virtual VOID                SetElement_Title(LPCTSTR szTitle, INT nType = 0);

	//设置队长标记
	virtual VOID				SetElement_LeaderFlag(BOOL bShow);

	//设置摆摊上的文字
	virtual VOID				SetElement_SaleText(LPCTSTR szSaleText);
	//设置是否显示摆摊信息
	virtual VOID				SetElement_SaleSign(BOOL bShow);

	virtual VOID				SetElement_PaoPaoText( LPCTSTR szPaoPao );

	virtual BOOL				isShow(){return m_bShow?TRUE:FALSE;};

	//设置逻辑对象ID
	virtual VOID				SetElement_ObjId(INT nObjId);

	CEGUI::Window*				GetMainWindow(void) { return m_pWindow; }

	virtual fVector2			GetPosition(void) const;

	virtual VOID				SetInfoState( int nState );

	virtual void				SetInfoDistance( float nDistance ) { m_nDistance = nDistance; }; // 设置离主角的位置，超过一定范围的位置会进行文字变淡

	virtual VOID				SetTitleType(INT nType) { m_nTitleType = nType; }
	virtual VOID				SetBangpaiFlag(INT nBangpaiFlag) { m_nBangpaiFlag = nBangpaiFlag; }

protected:
	//捕获“摆摊"按钮按下通知
	bool						handleSaleSignClicked(const CEGUI::EventArgs& e);
	//更新名称（考虑队长标志）
	void						_UpdateNameElement(void);
	//从缓冲池取出后重置数据
	void reset(void);


protected:
	CEGUI::Window*		m_pWindow;				// 窗口指针
	CEGUI::Window*		m_pNameWindow;			// 名字指针
	CEGUI::Window*		m_pTitleWindow;			// 称号窗口指针
	CEGUI::Window*		m_pSaleWindow;			// 摆摊头顶的牌子
	CEGUI::Window*		m_pLeadFlag;			// 队长图标
	CEGUI::Window*		m_pPaoPao;				// 聊天泡泡
	CEGUI::Window*		m_pBangpaiFlag;			// 帮派权限标志

	BOOL				m_bShow;				//是否显示 

	//对应的逻辑对象的ID
	BOOL				m_bShowStallSign;
	INT					m_nObjId;
	//玩家名称
	STRING				m_strName;
	//队长标志
	BOOL				m_bLeaderFlag;

	INT					m_nTitleType;			// 称号类型
	INT					m_nBangpaiFlag;			// 帮派称号上的权限标记

	int					m_InfoState;
	float				m_nDistance;

	float				m_fLastX, m_fLastY;
	int					m_nDebugIndex;
	void debugInfo(void);

private:
	friend class CUICreatureBoardSystem;
	CCreatureBoard(int nIndex);
	virtual ~CCreatureBoard();
};
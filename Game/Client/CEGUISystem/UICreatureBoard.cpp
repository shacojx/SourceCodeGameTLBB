#include "StdAfx.h"
#include "UIGlobal.h"
#include "UICreatureBoard.h"
#include "UICreatureBoardSystem.h"
#include "CEGUIWindowManager.h"
#include "ceguiwindow.h"
#include "ceguiudim.h"
#include "elements\ceguiprogressbar.h"
#include "uiwindowmng.h"
#include "TDException.h"
#include "UIString.h"
#include "CEGUIPropertyHelper.h"
#include "ceguicolourrect.h"
#include "elements\CEGUIPushButton.h"
#include "TDGameInterface.h"
#include "AxTrace.h"
#include "ceguipropertyhelper.h"
#include "uisystem.h"
#include "elements\CEGUIStaticImage.h"
#include "TDDataBase.h"
#include "TDDBC_Struct.h"
#include "FalagardSelfFitWindow.h"
#include "IFalagardSelfFitWindow.h"
#include "FalButton.h"
#include "AxTrace.h"

CCreatureBoard::CCreatureBoard(int nIndex):
	m_pWindow(0), 
	m_pNameWindow(0),
	m_pTitleWindow(0),
	m_pSaleWindow(0),
	m_bShow(TRUE),
	m_bShowStallSign(FALSE),
	m_bLeaderFlag(FALSE),
	m_nTitleType(DIMNESS_TITLE),
	m_nBangpaiFlag(NO_POS)
{
	m_nDebugIndex = nIndex;
	m_fLastX = m_fLastY = -100;

	m_pWindow = CEGUI::WindowManager::getSingleton().createWindow("WoWLook/CreatureBoard");
	TDAssert(m_pWindow);

	//设置大小
	m_pWindow->setProperty("Position", "x:0 y:0");
	m_pWindow->setProperty("Size", "w:1 h:1");

	//玩家名称
	m_pNameWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playername__");

	//摆摊按钮
	m_pSaleWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_salessign__");
	m_pSaleWindow->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&CCreatureBoard::handleSaleSignClicked, this));

	//称号
	m_pTitleWindow = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_playertitle__");

	//帮派权限小图标
	m_pBangpaiFlag = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_bangpaiflag__");

	//队长标志
	m_pLeadFlag = CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_leaderflag__");

	// 聊天泡泡
	m_pPaoPao =  CEGUI::WindowManager::getSingleton().getWindow(
		m_pWindow->getName() + "__auto_paopao__");

	m_InfoState = STATE_NORMAL;
}

CCreatureBoard::~CCreatureBoard()
{
	CEGUI::WindowManager::getSingleton().destroyWindow(m_pWindow);
}

void CCreatureBoard::reset(void)
{
	m_bShowStallSign = FALSE;
	m_bLeaderFlag = FALSE;
	m_strName = "";

	m_pNameWindow->setText("");
	m_pNameWindow->hide();

	m_pSaleWindow->setText("");
	m_pSaleWindow->hide();

	m_pTitleWindow->setText("");
	m_pTitleWindow->hide();

	m_pLeadFlag->hide();
	m_pBangpaiFlag->hide();
	
	m_pPaoPao->setText("");
	m_pPaoPao->hide();

	m_pWindow->hide();
}

//删除自身
VOID CCreatureBoard::Destroy(VOID)
{
	if( CUICreatureBoardSystem::GetMe() )
		CUICreatureBoardSystem::GetMe()->DestroyCreatureBoard(this);
	return;
	//不能再调用任何代码
	//...
}

fVector2 CCreatureBoard::GetPosition(void) const
{
	CEGUI::Point pt = CEGUI::PropertyHelper::stringToPoint(m_pNameWindow->getProperty( "FitPosition" ));

	return fVector2(pt.d_x, pt.d_y);
}

//设置在屏幕上的坐标(像素)
VOID CCreatureBoard::SetPosition(FLOAT nX, FLOAT nY)
{
	float fAlpha = 1.0f;
	if(m_nDistance > CUICreatureBoardSystem::GetMe()->GetMaxDispearDistance())
	{
		m_pWindow->hide();
		return;
	}

	if(m_nDistance > CUICreatureBoardSystem::GetMe()->GetMaxDistance())
	{
		fAlpha -= (m_nDistance - CUICreatureBoardSystem::GetMe()->GetMaxDistance() )/
					( CUICreatureBoardSystem::GetMe()->GetMaxDispearDistance() - CUICreatureBoardSystem::GetMe()->GetMaxDistance());
	}

	CEGUI::Point fPos(nX, nY);
	float fOffset = 0;
	float fY = fPos.d_y;

	((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pNameWindow)->setCenter(fPos);
	fOffset -= m_pNameWindow->getAbsoluteHeight();

	if(m_pNameWindow->isVisible() && !(m_pNameWindow->getText().empty()))
	{
		m_pWindow->show();
		//debugInfo();
	}
	else
	{
		m_pWindow->hide();
	}

	if(!(m_pTitleWindow->getText().empty()))
	{
		fPos.d_y = fY + fOffset;
		((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pTitleWindow)->setCenter(fPos);
		fOffset -= m_pTitleWindow->getAbsoluteHeight();

		m_pTitleWindow->show();
	}

	if(m_pSaleWindow->isVisible())
	{
		fPos.d_y = fY + fOffset;
		m_pSaleWindow->setProperty( "HookPosition", CEGUI::PropertyHelper::pointToString( fPos ) );
		((CEGUI::PushButton*)m_pSaleWindow)->resizeWithText();
		fOffset -= m_pSaleWindow->getAbsoluteHeight();
	}

	if(m_pPaoPao->isVisible())
	{
		fPos.d_y = fY + fOffset;
		((CEGUI::IFalagardSelfFitWindow*)(CEGUI::FalagardSelfFitWindow*)m_pPaoPao)->setCenter(fPos);
	}

	CEGUI::Size size = m_pNameWindow->getAbsoluteSize();
	fPos = m_pNameWindow->getAbsolutePosition();
	fPos.d_x -= size.d_height;
	m_pLeadFlag->setPosition(CEGUI::Absolute, fPos);
	size.d_width = size.d_height;
	m_pLeadFlag->setSize(CEGUI::Absolute, size);


	//帮派权限图标标的位置
	size = m_pTitleWindow->getAbsoluteSize();
	fPos = m_pTitleWindow->getAbsolutePosition();
	fPos.d_x -= size.d_height;
	m_pBangpaiFlag->setPosition( CEGUI::Absolute, fPos );
	size.d_width = size.d_height;
	m_pBangpaiFlag->setSize( CEGUI::Absolute, size );

	m_pBangpaiFlag->setAlpha( fAlpha );
	m_pLeadFlag->setAlpha( fAlpha );
	m_pSaleWindow->setAlpha( fAlpha );
	m_pNameWindow->setAlpha( fAlpha );
	m_pTitleWindow->setAlpha(fAlpha);
	switch( m_InfoState ) 
	{
	case STATE_NORMAL:
		break;
	case STATE_DIE:
		m_pNameWindow->setProperty( "TextColor", CEGUI::PropertyHelper::colourToString( CEGUI::colour( 0.5,0.5,0.5, fAlpha ) ) );
		break;
	default:
		break;
	}
}

//显示/隐藏
VOID CCreatureBoard::Show(BOOL bShow)
{
//	if(bShow == m_bShow) return;

	if(bShow && !(m_pNameWindow->getText().empty()))
	{
		m_pWindow->show();
//		debugInfo();

		m_pWindow->requestRedraw();
		m_pNameWindow->requestRedraw();
		m_pTitleWindow->requestRedraw();
		m_pBangpaiFlag->requestRedraw();

		if(!m_bShowStallSign)
			m_pSaleWindow->hide();
	}
	else 
	{
		m_pWindow->hide();

		m_pTitleWindow->hide();
	}

	m_bShow = bShow;
}

void CCreatureBoard::debugInfo(void)
{
	CEGUI::Point pt = m_pNameWindow->getAbsolutePosition();
	if(pt.d_x != m_fLastX || pt.d_y != m_fLastY)
	{
//		AxTrace(m_nDebugIndex, 0, "%.2f,%.2f", pt.d_x, pt.d_y);
	}

	m_fLastX = pt.d_x;
	m_fLastY = pt.d_y;
}

//设置名字
VOID CCreatureBoard::SetElement_Name(LPCTSTR szName)
{
	m_strName = szName;
	_UpdateNameElement();
	return;
}

VOID CCreatureBoard::SetElement_PaoPaoText( LPCTSTR szPaoPao )
{
	if( CUISystem::GetMe()->IsPaoPaoActive() == false )
		return ;
	CEGUI::String strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(szPaoPao, strUtf32);

	if( m_pPaoPao )
	{
		m_pPaoPao->show();
		m_pPaoPao->setText( strUtf32 );
	}
}
//设置称号
VOID CCreatureBoard::SetElement_Title(LPCTSTR szTitle, INT nType)
{
	TDAssert(szTitle);

	if(strlen(szTitle) == 0)
	{
		if(m_pTitleWindow)
		{
			m_pTitleWindow->setText("");
			m_pTitleWindow->hide();
		}
		return;
	}

	STRING strColorTitle;
	CEGUI::String strUtf32;

	strColorTitle = szTitle;
	m_nTitleType = nType;

	CUIStringSystem::GetMe()->ParserString_Runtime(strColorTitle.c_str(), strUtf32);

	if(m_pTitleWindow)
	{
		m_pTitleWindow->setText(strUtf32);

		//如果这个称号是帮派称号
		if(m_nTitleType == BANGPAI_TITLE)//BANGPAI_TITLE
		{
			switch(m_nBangpaiFlag)
			{
			case NO_POS:
				break;
			case EXCELLENCE_POS:	// 精英
				{
					((CEGUI::StaticImage*)m_pBangpaiFlag)->setImage( (CEGUI::utf8*)"common2",(CEGUI::utf8*)"Captain_Icon" );
					m_pBangpaiFlag->show();
				}
				break;
			case OFFICEHOLDER_POS:	// 官员
				{
					((CEGUI::StaticImage*)m_pBangpaiFlag)->setImage( (CEGUI::utf8*)"common2",(CEGUI::utf8*)"Captain_Icon" );
					m_pBangpaiFlag->show();
				}
				break;
			case CHAIRMAN_POS:		// 帮主
				{
					((CEGUI::StaticImage*)m_pBangpaiFlag)->setImage( (CEGUI::utf8*)"common2",(CEGUI::utf8*)"Captain_Icon" );
					m_pBangpaiFlag->show();
				}
				break;
			}
		}
	}
}

//设置队长标记
VOID CCreatureBoard::SetElement_LeaderFlag(BOOL bShow)
{
	m_bLeaderFlag = bShow;
	_UpdateNameElement();
}

//设置摆摊上的文字
VOID CCreatureBoard::SetElement_SaleText(LPCTSTR szSaleText)
{
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(szSaleText, strUtf8);

	if(m_pSaleWindow)
	{
		m_pSaleWindow->show();
		m_pSaleWindow->setText( (CEGUI::String)(CEGUI::utf8*)(strUtf8.c_str()) );
	}
	
}
//设置是否显示摆摊信息
VOID CCreatureBoard::SetElement_SaleSign(BOOL bShow)
{
	if(bShow)
	{
		m_bShowStallSign = TRUE;
		if(m_pSaleWindow) 
			m_pSaleWindow->show();
	}
	else
	{
		m_bShowStallSign = FALSE;
		if(m_pSaleWindow) 
			m_pSaleWindow->hide();
	}
}
	//设置逻辑对象ID
VOID CCreatureBoard::SetElement_ObjId(INT nObjId)
{
	m_nObjId = nObjId;
} 

bool CCreatureBoard::handleSaleSignClicked(const CEGUI::EventArgs& e)
{
	g_pGameInterface->Stall_OpenStallSale(m_nObjId);
	return false;
}

void CCreatureBoard::_UpdateNameElement(void)
{
	if(m_strName.empty())
	{
		m_pNameWindow->setText((CEGUI::utf8*)"");
		m_pWindow->hide();
		return;
	}

	CEGUI::String strUtfName32;
	CUIStringSystem::GetMe()->ParserString_Runtime(m_strName, strUtfName32);

	if(m_bLeaderFlag)
	{
		m_pLeadFlag->show();
	}
	else
	{
		m_pLeadFlag->hide();
	}
	if(m_pNameWindow && !strUtfName32.empty())
	{
		m_pNameWindow->show();
		m_pNameWindow->setText(strUtfName32);
		m_pWindow->hide();
	}
}

VOID CCreatureBoard::SetInfoState( int nState )
{
	m_InfoState = nState;
}
#include "StdAfx.h"
#include "DebugPanel.h"
#include <OgreOverlayManager.h>
#include <OgreRenderTarget.h>
#include <OgreOverlayElement.h>
#include <OgreStringConverter.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayContainer.h>
#include <OgreFrameListener.h>
#include "TDException.h"
#include "Global.h"
#include "TDVariable.h"

CDebugPanel::CDebugPanel( Ogre::Overlay*		pDebugOverlay, 
						  Ogre::OverlayElement* pStatPanel, 
						  Ogre::OverlayElement* pHitObjPanel, 
						  Ogre::OverlayElement* pHitObjTextElement, 
						  Ogre::RenderWindow*	pRenderWindow)
{
	m_pDebugOverlay		= pDebugOverlay;
	m_pStatPanel		= pStatPanel;
	m_pHitObjPanel		= pHitObjPanel;
	m_pHitObjextElement	= pHitObjTextElement;
	m_pRenderWindow		= pRenderWindow;
	TDAssert( m_pDebugOverlay && m_pStatPanel && m_pHitObjPanel && m_pHitObjextElement &&m_pRenderWindow);

	m_pDebugOverlay->show();

	m_pHitObjPanel->hide();
	m_pHitObjextElement->show();

	m_pStatPanel->hide();
}

CDebugPanel::~CDebugPanel()
{

}

VOID CDebugPanel::ToggleShow(VOID)
{
	if(m_pStatPanel->isVisible())
	{
		m_pStatPanel->hide();
	}
	else
	{
		m_pStatPanel->show();
	}
}

VOID CDebugPanel::SetHitObjString(LPCTSTR szText)
{
	if(!m_pHitObjPanel || !(m_pStatPanel->isVisible()))
	{
		m_pHitObjPanel->hide();
		return;
	}

	if(szText[0] == '\0')
	{
		m_pHitObjPanel->hide();
	}
	else
	{
		m_pHitObjextElement->setCaption(szText);
		m_pHitObjPanel->show();
	}
}

VOID CDebugPanel::SetDebugString(LPCTSTR szName, LPCTSTR szValue)
{
	if(szName == NULL || szName[0] == _T('\0')) return;

	//首先检索是否已经存在
	DEBUG_ELEMENT* itFind = m_mapPanelIndex[szName];
	if(itFind != NULL)
	{
		//已经存在, 直接设置
		itFind->strValue = szValue;

		itFind->pOgreElement->setCaption(itFind->strName + itFind->strValue); 
	}
	else
	{
		INT nTop = 5 + (INT)m_listPanelElement.size()*15;
		
		//不存在， 创建
		DEBUG_ELEMENT newElement;
		newElement.strName = szName;
		newElement.strName += _T(" : ");

		newElement.strValue = szValue;

		//创建新的Element
		Ogre::OverlayElement* pEle = Ogre::OverlayManager::getSingleton().createOverlayElement(
											"TextArea", 
											Ogre::String("TLBB/")+Ogre::String(szName));
		pEle->setParameter("metrics_mode", "pixels");
		pEle->setLeft(5);
		pEle->setTop(nTop);
		pEle->setWidth(90);
		pEle->setHeight(30);
		pEle->setParameter("font_name", "TrebuchetMSBold");
		pEle->setParameter("char_height", "16");
		pEle->setParameter("caption", newElement.strName + newElement.strValue);
		pEle->setParameter("colour_top", "1 1 0.7");
		pEle->setParameter("colour_bottom", "1 1 0.7");
		((Ogre::OverlayContainer*)m_pStatPanel)->addChild(pEle);

		newElement.pOgreElement = pEle;

		//加入
		m_listPanelElement.push_back(newElement);
		m_mapPanelIndex[szName] = &(m_listPanelElement.back());
	}
}

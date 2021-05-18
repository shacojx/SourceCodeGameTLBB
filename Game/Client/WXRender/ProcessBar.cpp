#include "StdAfx.h"
#include "ProcessBar.h"
#include "TDException.h"
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>

CProcessBar::CProcessBar(LPCTSTR szOverLayName, LPCTSTR szProcessElementName)
{
	Ogre::OverlayManager& omgr = Ogre::OverlayManager::getSingleton();
	
	m_pProcessOverlay = (Ogre::Overlay*)omgr.getByName(szOverLayName);
	m_pProcessElement = (Ogre::OverlayElement*)omgr.getOverlayElement(szProcessElementName);

	TDAssert(m_pProcessOverlay && m_pProcessElement);
}

CProcessBar::~CProcessBar()
{

}

VOID CProcessBar::Show(BOOL bShow)
{
	if(!m_pProcessOverlay) return;

	if(bShow)
		m_pProcessOverlay->show();
	else
		m_pProcessOverlay->hide();
}

VOID CProcessBar::SetWidth(FLOAT fWidth)
{
	if(!m_pProcessElement) return;
	if(fWidth < 0.0f || fWidth > 1.0f) return; 
	
	FLOAT fActWidth = fWidth*196.0f;
	m_pProcessElement->setWidth(fActWidth);
}
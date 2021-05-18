#include "StdAfx.h"
#include "HelpPanel.h"
#include "TDException.h"

#include <OgreOverlayManager.h>
#include <OgreRenderTarget.h>
#include <OgreOverlayElement.h>
#include <OgreStringConverter.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayContainer.h>
#include <OgreFrameListener.h>

CHelpPanel::CHelpPanel(	Ogre::Overlay*		pHelpOverlay, 
						Ogre::OverlayElement* pHelpPanel,
						Ogre::OverlayElement* pHelpText)
	: m_pHelpOverlay(pHelpOverlay)
	, m_pHelpPanel(pHelpPanel)
	, m_pHelpTextElement(pHelpText)
{
	TDAssert( m_pHelpOverlay && m_pHelpPanel && m_pHelpTextElement);

	m_pHelpOverlay->show();
	m_pHelpPanel->show();
	m_pHelpTextElement->show();
}


CHelpPanel::~CHelpPanel()
{
}

void CHelpPanel::Show(bool bShow)
{
	if(bShow)
	{
		m_pHelpOverlay->show();
		m_pHelpPanel->show();
	}
	else
	{
		m_pHelpOverlay->hide();
		m_pHelpPanel->hide();
	}
}

void CHelpPanel::SetHelpText(LPCTSTR szText)
{
	m_pHelpTextElement->setCaption(Ogre::String(szText));
}
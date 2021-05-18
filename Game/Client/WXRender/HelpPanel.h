#pragma once

namespace Ogre
{
	class Overlay;
	class OverlayElement;
};
#include "TDBasicType.h"

class CHelpPanel
{
public:
	void						Show(bool bShow);
	void						SetHelpText(LPCTSTR szText);

protected:
	//OGRE Overlay
	Ogre::Overlay*				m_pHelpOverlay;
	Ogre::OverlayElement*		m_pHelpPanel;
	Ogre::OverlayElement*		m_pHelpTextElement;

	STRING						m_strHelpText;

public:
	CHelpPanel( Ogre::Overlay*	pHelpOverlay, 
				Ogre::OverlayElement* pHelpPanel,
				Ogre::OverlayElement* pHelpText);
	virtual ~CHelpPanel();

};
#pragma once

#include "TDBasicType.h"
#include <deque>

namespace Ogre
{
	class Overlay;
	class OverlayElement;
	class RenderWindow;
};

class CDebugPanel
{
public:
	VOID		SetDebugString(LPCTSTR szName, LPCTSTR szValue);
	VOID		SetHitObjString(LPCTSTR szText);

	VOID		ToggleShow(VOID);

protected:
	//OGRE Overlay
	Ogre::Overlay*				m_pDebugOverlay;
	Ogre::OverlayElement*		m_pStatPanel;
	Ogre::OverlayElement*		m_pHitObjPanel;
	Ogre::OverlayElement*		m_pHitObjextElement;
	Ogre::RenderWindow*			m_pRenderWindow;

	//µ˜ ‘∞Â–≈œ¢
	struct DEBUG_ELEMENT
	{
		STRING		strName;
		STRING		strValue;
		Ogre::OverlayElement* pOgreElement;
	};
	typedef std::list< DEBUG_ELEMENT > DEBUG_ELEMENT_LIST;
	typedef std::map< STRING, DEBUG_ELEMENT* > DEBUG_ELEMENT_INDEX;

	DEBUG_ELEMENT_LIST			m_listPanelElement;
	DEBUG_ELEMENT_INDEX			m_mapPanelIndex;

public:
	CDebugPanel( Ogre::Overlay*	pDebugOverlay, 
				 Ogre::OverlayElement* pStatPanel, 
				 Ogre::OverlayElement* pHitObjPanel, 
				 Ogre::OverlayElement* pHitObjTextElement, 
				 Ogre::RenderWindow* pRenderWindow);
	virtual ~CDebugPanel();

};
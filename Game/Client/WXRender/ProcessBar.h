#pragma once

namespace Ogre
{
	class Overlay;
	class OverlayElement;
};
//½ø¶ÈÌõ
class CProcessBar
{
public:
	VOID					Show(BOOL bShow);
	VOID					SetWidth(FLOAT fWidth);

protected:
	Ogre::Overlay*			m_pProcessOverlay;
	Ogre::OverlayElement*	m_pProcessElement;

public:
	CProcessBar(LPCTSTR szOverLayName		= "TLBB/ProcessOverlay", 
				LPCTSTR szProcessElementName= "TLBB/LoadPanel/Bar/Progress");
	virtual ~CProcessBar();
};
#include "StdAfx.h"
#include "UIIconsMng.h"
#include "UIGlobal.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "OgreCEGUITexture.h"
#include "OgreHardwarePixelBuffer.h"
#include "TDDebuger.h"
#include "OgrePixelFormat.h"
#include "LuaPlus.h"

CUIIconsManager* CUIIconsManager::s_pMe = NULL;
CUIIconsManager::CUIIconsManager()
{
	s_pMe = this;
}

CUIIconsManager::~CUIIconsManager()
{
}

VOID CUIIconsManager::Initial(VOID)
{
	CEGUI::ImagesetManager&	 theImagesetMng = CEGUI::ImagesetManager::getSingleton();

	//遍历所有的Imageset
	CEGUI::ImagesetManager::ImagesetIterator it = theImagesetMng.getIterator();

	for(it.toStart(); !it.isAtEnd(); it++)
	{
		const CEGUI::String strName = it.getCurrentKey();
		const CEGUI::Imageset* pImageset = it.getCurrentValue();
		const CEGUI::String& strTextureName = pImageset->getTextureFilename();

		if(strTextureName.substr(0, 5) == CEGUI::String("Icons"))
		{
			//遍历Imageset中所有Image
			CEGUI::Imageset::ImageIterator itImage = pImageset->getIterator();
			for(itImage.toStart(); !itImage.isAtEnd(); itImage++)
			{
				const CEGUI::String strImageName = itImage.getCurrentKey();
				const CEGUI::Image* pImage = &(itImage.getCurrentValue());

				m_mapAllIcons.insert(std::make_pair(strImageName.c_str(), pImageset));
			}
		}
	}
}

STRING CUIIconsManager::GetIconFullName(LPCTSTR szIconName)
{
	MAP_ICONS::iterator it = m_mapAllIcons.find(szIconName);
	if(it == m_mapAllIcons.end()) return STRING("set:X image:X");

	const CEGUI::Imageset* pImageset = it->second;

	//set:Icons1 image:Ability_BackStab
	CHAR szRet[MAX_PATH];
	_snprintf(szRet, MAX_PATH, "set:%s image:%s", pImageset->getName().c_str(), szIconName);

	return STRING(szRet);
}

INT	CUIIconsManager::LUA_GetIconFullName(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if( !args[1].IsString() ) return 0;

	pState->PushString(CUIIconsManager::GetMe()->GetIconFullName(args[1].GetString()).c_str());

	return 1;
}

const CEGUI::Image* CUIIconsManager::GetIcon(LPCTSTR szIconName)
{
	MAP_ICONS::iterator it = m_mapAllIcons.find(szIconName);
	if(it == m_mapAllIcons.end()) return 0;

	const CEGUI::Imageset* pImageset =  it->second;
	
	return &(pImageset->getImage(szIconName));
}

HCURSOR	CUIIconsManager::CreateAsWinCursor(LPCTSTR szIconName)
{
	try{
	//-------------------------------------------------------
	//CEGUI Image
	const CEGUI::Image* pIconImage = GetIcon(szIconName);
	if(!pIconImage) return NULL;

	//-------------------------------------------------------
	//CEGUI ImageSet
	CEGUI::Imageset* pImageset = CEGUI::ImagesetManager::getSingleton().getImageset(pIconImage->getImagesetName());
	if(!pImageset) return NULL;

	//-------------------------------------------------------
	//CEGUI Ogre Texture
	CEGUI::OgreCEGUITexture* pCEGUIOgreTexture = (CEGUI::OgreCEGUITexture*)(pImageset->getTexture());
	if(!pCEGUIOgreTexture) return NULL;

	//-------------------------------------------------------
	//Ogre Texture
	Ogre::Texture* pOgreTexture = pCEGUIOgreTexture->getOgreTexture().get();
	if(!pOgreTexture) return NULL;

	//-------------------------------------------------------
	//Ogre HardwarePixelBuffer
	Ogre::HardwarePixelBuffer* pOgreHardPixBuf = pOgreTexture->getBuffer().get();
	if(!pOgreHardPixBuf) return NULL;

	//-------------------------------------------------------
	//Blt to Ogre PixelBox
	const static INT s_CursorWidth  = 32;
	const static INT s_CursorHeight = 32;

	CEGUI::Rect rectAtImage = pIconImage->getSourceTextureArea();
	const Ogre::PixelBox thePixBox(s_CursorWidth, s_CursorHeight, 1, Ogre::PF_R8G8B8A8, new char[s_CursorWidth*s_CursorHeight*Ogre::PixelUtil::getNumElemBytes(Ogre::PF_R8G8B8A8)]);
	pOgreHardPixBuf->blitToMemory(
		Ogre::Image::Box((INT)rectAtImage.d_left, (INT)rectAtImage.d_top, (INT)rectAtImage.d_right, (INT)rectAtImage.d_bottom), thePixBox);

	//-------------------------------------------------------
	//Create Windows Bitmap as XOR Mask

	HDC hMainDC			= ::GetDC(NULL);
	HDC hXorMaskDC		= ::CreateCompatibleDC(hMainDC); 
	HBITMAP hXorMask	= ::CreateCompatibleBitmap(hMainDC, s_CursorWidth, s_CursorHeight);
	::SelectObject(hXorMaskDC, hXorMask);

	//-------------------------------------------------------
	//Get per Pixel Color to XOR Mask
	for(register INT x=0; x<s_CursorWidth; x++)
	{
		for(register INT y=0; y<s_CursorHeight; y++)
		{
			Ogre::ColourValue col;
			VOID* pColData = (thePixBox.rowPitch*y + x)* Ogre::PixelUtil::getNumElemBytes(thePixBox.format) + 
							 (unsigned char*)thePixBox.data;

			Ogre::PixelUtil::unpackColour( &col, thePixBox.format, pColData);

			::SetPixel(hXorMaskDC, x, y, RGB(col.r*255, col.g*255, col.b*255));
		}
	}
	delete[] thePixBox.data;
	::DeleteDC(hXorMaskDC); hXorMaskDC = NULL;

	//-------------------------------------------------------
	//Create AND Mask
	HDC hAndMaskDC	= ::CreateCompatibleDC(hMainDC); 
	HBITMAP hAndMask	= ::CreateCompatibleBitmap(hMainDC, s_CursorWidth, s_CursorHeight);
	::SelectObject(hAndMaskDC, hAndMask);

	RECT rect;
	::SetRect(&rect, 0, 0, s_CursorWidth, s_CursorHeight);
	::FillRect(hAndMaskDC, &rect, (HBRUSH)::GetStockObject(BLACK_BRUSH));

	::DeleteDC(hAndMaskDC); hAndMaskDC = NULL;

	//-------------------------------------------------------
	//Create Crusor
	ICONINFO iconinfo = {0};
	iconinfo.fIcon		= FALSE;
	iconinfo.xHotspot	= 0;
	iconinfo.yHotspot	= 0;
	iconinfo.hbmMask	= hAndMask;
	iconinfo.hbmColor	= hXorMask;

	HCURSOR hRetCursor = ::CreateIconIndirect(&iconinfo);

	//-------------------------------------------------------
	//Release
	::DeleteObject(hAndMask);
	::DeleteObject(hXorMask);
	::ReleaseDC(NULL,hMainDC);

	return hRetCursor;
	
	}catch(...)
	{
		return 0;
	}
}


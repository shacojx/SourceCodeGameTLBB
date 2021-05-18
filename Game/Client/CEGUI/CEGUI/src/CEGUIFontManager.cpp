/************************************************************************
	filename: 	CEGUIFontManager.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the FontManager class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "CEGUIFontManager.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIFontBase.h"
#include "CEGUIFont_FreeType.h"
#include "CEGUIFont_Bitmap.h"
#include "CEGUIFontManager_implData.h"
#include "CEGUIFont_implData.h"
#include "CEGUISystem.h"

#include "CEGUIImageset.h"
#include "CEGUIImage.h"
#include "CEGUITexture.h"
#include "windows.h"

#include <ft2build.h>
#include FT_FREETYPE_H


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> FontManager* Singleton<FontManager>::ms_Singleton	= NULL;



/*************************************************************************
	constructor
*************************************************************************/
FontManager::FontManager(void)
{
	d_implData = new FontManagerImplData;

	if (FT_Init_FreeType(&d_implData->d_ftlib))
	{
		throw GenericException((utf8*)"FontManager::FontManager - Failed to initialise the FreeType library.");
	}

	Logger::getSingleton().logEvent((utf8*)"CEGUI::FontManager singleton created.");

	d_timeOld = 0.0f;
	d_totalLife = 0.8f;	//blink one cycle need 0.8 sec.
	d_minAlpha = 0.35f;	//alpha smallest value.

	d_curBlinkAlpha = 1.0f;
}


/*************************************************************************
	Destructor
*************************************************************************/
FontManager::~FontManager(void)
{
	Logger::getSingleton().logEvent((utf8*)"---- Begining cleanup of Font system ----");
	destroyAllFonts();

	FT_Done_FreeType(d_implData->d_ftlib);
	delete d_implData;

	Logger::getSingleton().logEvent((utf8*)"CEGUI::FontManager singleton destroyed.");
}


/*************************************************************************
	Create a font from a definition file
*************************************************************************/
FontBase* FontManager::createFont(FontType type, const String& filename, const String& resourceGroup)
{
	Logger::getSingleton().logEvent((utf8*)"Attempting to create Font from the information specified in file '" + filename + "'.");
	
	FontBase* temp = 0;
	if(FreeType == type)
	{
		temp = new Font_FreeType(filename, resourceGroup, new Font_FreeType::FontImplData(d_implData->d_ftlib));
	}
	else
	{
		temp = new Font_Bitmap(filename, resourceGroup);
	}

	temp->load(filename, resourceGroup);

	String name = temp->getName();

	if (isFontPresent(name))
	{
		delete temp;

		throw AlreadyExistsException((utf8*)"FontManager::createFont - A font named '" + name + "' already exists.");
	}

	d_fonts[name] = temp;

    // if this was the first font created, set it as the default font
    if (d_fonts.size() == 1)
    {
        System::getSingleton().setDefaultFont(temp);
    }

	return temp; 
}

/*************************************************************************
	Destroy the named font
*************************************************************************/
void FontManager::destroyFont(const String& name)
{
	 FontRegistry::iterator	pos = d_fonts.find(name);

	if (pos != d_fonts.end())
	{
		String tmpName(name);

		delete pos->second;
		d_fonts.erase(pos);

		Logger::getSingleton().logEvent((utf8*)"Font '" + tmpName +"' has been destroyed.");
	}

}


/*************************************************************************
	Destroys the given Font object
*************************************************************************/
void FontManager::destroyFont(FontBase* font)
{
	if (font != NULL)
	{
		destroyFont(font->getName());
	}

}


/*************************************************************************
	Destroys all Font objects registered in the system
*************************************************************************/
void FontManager::destroyAllFonts(void)
{
	while (!d_fonts.empty())
	{
		destroyFont(d_fonts.begin()->first);
	}

}


/*************************************************************************
	Check to see if a font is available
*************************************************************************/
bool FontManager::isFontPresent(const String& name) const
{
	return (d_fonts.find(name) != d_fonts.end());
}


/*************************************************************************
	Return a pointer to the named font
*************************************************************************/
FontBase* FontManager::getFont(const String& name) const
{
	FontRegistry::const_iterator pos = d_fonts.find(name);

	if (pos == d_fonts.end())
	{
		throw UnknownObjectException("FontManager::getFont - A Font object with the specified name '" + name +"' does not exist within the system");
	}

	return pos->second;
}


/*************************************************************************
	Notify the FontManager of the current (usually new) display
	resolution.
*************************************************************************/
void FontManager::notifyScreenResolution(const Size& size)
{
	// notify all attached Font objects of the change in resolution
	FontRegistry::iterator pos = d_fonts.begin(), end = d_fonts.end();

	for (; pos != end; ++pos)
	{
		pos->second->notifyScreenResolution(size);
	}

}

void FontManager::prepareFontTexture(void)
{
	// notify all attached Font objects to prepare font texture.
	FontRegistry::iterator pos = d_fonts.begin(), end = d_fonts.end();

	for (; pos != end; ++pos)
	{
		pos->second->prepareFontTexture();
	}
}

/*************************************************************************
Queue the blink txt to be drawn and create a runtime blink 
in runtime blink list.
*************************************************************************/
void FontManager::addRuntimeBlink(const Image* img, unsigned long& quadID, const Vector3& position, const Size& size, const Rect& clip_rect, const ColourRect& col_rect)
{
	FontBlinkRuntime runtime;

	runtime.d_texture = img->getImageset()->getTexture();
	runtime.d_quadID = quadID;
	runtime.d_colorRect = col_rect;
	runtime.d_originalAlpha = col_rect.d_top_left.getAlpha(); 

	Rect source_rect = img->getSourceTextureArea();
	Rect dest_rect = Rect(position.d_x, position.d_y, position.d_x + size.d_width, position.d_y + size.d_height);
	
	Size sz = img->getSize();
	Point pt = img->getOffsets();

	// get the rect area that we will actually draw to (i.e. perform clipping)
	Rect final_rect(dest_rect.getIntersection(clip_rect));

	Texture* pTex = img->getImageset()->getTexture();

	float x_scale = 1.0f / (float)pTex->getWidth();
	float y_scale = 1.0f / (float)pTex->getHeight();

	float tex_per_pix_x = source_rect.getWidth() / dest_rect.getWidth();
	float tex_per_pix_y = source_rect.getHeight() / dest_rect.getHeight();

	// calculate final, clipped, texture co-ordinates
	Rect  tex_rect((source_rect.d_left + ((final_rect.d_left - dest_rect.d_left) * tex_per_pix_x)) * x_scale,
		(source_rect.d_top + ((final_rect.d_top - dest_rect.d_top) * tex_per_pix_y)) * y_scale,
		(source_rect.d_right + ((final_rect.d_right - dest_rect.d_right) * tex_per_pix_x)) * x_scale,
		(source_rect.d_bottom + ((final_rect.d_bottom - dest_rect.d_bottom) * tex_per_pix_y)) * y_scale);

	runtime.d_textureRect = tex_rect;

	d_runtimeBlink.push_back(runtime);
}
/*************************************************************************
Clears all runtime blink.
*************************************************************************/
void FontManager::clearRuntimeBlinkList(void)
{
	d_runtimeBlink.clear();
}
/*************************************************************************
Cause all runtime blink in render queue to update itself.
*************************************************************************/
void FontManager::updateRuntimeBlinkList(void)
{
	FontBlinkRegistry::iterator it;
	for(it = d_runtimeBlink.begin(); it != d_runtimeBlink.end(); ++it)
	{
		FontBlinkRuntime& runtime = *it;
		runtime.d_colorRect.setAlpha(getBlinkAlphaValue());

		System::getSingleton().getRenderer()->adjustQuad(
			runtime.d_quadID, runtime.d_texture, runtime.d_textureRect, runtime.d_colorRect);
	}
}

/*************************************************************************
Calculate current alpha for blink. 1.0f -> 0.0f -> 1.0f ...
*************************************************************************/
void FontManager::calculateBlinkAlphaValue(void)
{
	float nowTime = System::getSingleton().getCurTimeElapsed();
	float step = 0.0f;

	static bool dir = true;

	//char stmp[64] = {0};

	step = (nowTime - d_timeOld)/d_totalLife;
	d_timeOld = nowTime;

	d_curBlinkAlpha += (dir)?step:-step;
	if(d_curBlinkAlpha > 1.0f)
	{
		d_curBlinkAlpha = 1.0f;
		dir = false;
	}

	if(d_curBlinkAlpha < d_minAlpha)
	{
		d_curBlinkAlpha = d_minAlpha;
		dir = true;
	}

	//_snprintf(stmp,63, "blink Alpha:%f\n", d_curBlinkAlpha);
	//OutputDebugString(stmp);
}

FontManager& FontManager::getSingleton(void)
{
	return Singleton<FontManager>::getSingleton();
}


FontManager* FontManager::getSingletonPtr(void)
{
	return Singleton<FontManager>::getSingletonPtr();
}


/*************************************************************************
	Return a FontManager::FontIterator object to iterate over the
	available Font objects.
*************************************************************************/
FontManager::FontIterator FontManager::getIterator(void) const
{
	return FontIterator(d_fonts.begin(), d_fonts.end());
}

} // End of  CEGUI namespace section

/************************************************************************
    filename:   CEGUIRenderCache.cpp
    created:    Fri Jun 17 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUIRenderCache.h"
#include "CEGUISystem.h"
#include "CEGUIRenderer.h"
//#include "CEGUIWindow.h"
// Start of CEGUI namespace section
namespace CEGUI
{
	class Window;
    RenderCache::RenderCache()
    {}

    RenderCache::~RenderCache()
    {}

    bool RenderCache::hasCachedImagery() const
    {
        return !(d_cachedImages.empty() && d_cachedTexts.empty());
    }

    void RenderCache::render(const Point& basePos, float baseZ, const Rect& clipper, UserSharps bMenualSharp) const
    {
        Rect displayArea(System::getSingleton().getRenderer()->getRect());
        Rect custClipper;
        const Rect* finalClipper;
        Rect finalRect;

        // Send all cached images to renderer.
        for(ImageryList::const_iterator image = d_cachedImages.begin(); image != d_cachedImages.end(); ++image)
        {
            if ((*image).usingCustomClipper)
            {
                custClipper = (*image).customClipper;
                custClipper.offset(basePos);
                custClipper = (*image).clipToDisplay ? displayArea.getIntersection(custClipper) : clipper.getIntersection(custClipper);
                finalClipper = &custClipper;
            }
            else
            {
                finalClipper = (*image).clipToDisplay ? &displayArea : &clipper;
            }

            finalRect = (*image).target_area;
            finalRect.offset(basePos);
            (*image).source_image->draw(finalRect, baseZ + (*image).z_offset, *finalClipper, (*image).colours, TopLeftToBottomRight, bMenualSharp);
        }

        // send all cached texts to renderer.
        for(TextList::const_iterator text = d_cachedTexts.begin(); text != d_cachedTexts.end(); ++text)
        {
            if ((*text).usingCustomClipper)
            {
                custClipper = (*text).customClipper;
                custClipper.offset(basePos);
                custClipper = (*text).clipToDisplay ? displayArea.getIntersection(custClipper) : clipper.getIntersection(custClipper);
                finalClipper = &custClipper;
            }
            else
            {
                finalClipper = (*text).clipToDisplay ? &displayArea : &clipper;
            }

            finalRect = (*text).target_area;
            finalRect.offset(basePos);
			(*text).source_font->drawText((const Window*)(*text).OwnerWindow, (const String&)(*text).text, 
				(const Rect&)finalRect, (float) baseZ + (*text).z_offset, (const Rect&)*finalClipper, 
				(TextFormatting)(*text).formatting, (const ColourRect&)(*text).colours, (float)(*text).fScaleX, (float)(*text).fScaleY );
        }

    }

    void RenderCache::clearCachedImagery()
    {
        d_cachedImages.clear();
        d_cachedTexts.clear();
    }

    void RenderCache::cacheImage(const Image& image, const Rect& destArea, float zOffset, const ColourRect& cols, const Rect* clipper, bool clipToDisplay)
    {
        ImageInfo imginf;
        imginf.source_image = &image;
        imginf.target_area  = destArea;
        imginf.z_offset     = zOffset;
        imginf.colours      = cols;
        imginf.clipToDisplay = clipToDisplay;

        if (clipper)
        {
            imginf.customClipper = *clipper;
            imginf.usingCustomClipper = true;
        }
        else
        {
            imginf.usingCustomClipper = false;
        }

        d_cachedImages.push_back(imginf);
    }

    void RenderCache::cacheText( const Window* OwnerWindow, const String& text, const FontBase* font, TextFormatting format, const Rect& destArea, float zOffset, const ColourRect& cols, const Rect* clipper, bool clipToDisplay, float fScaleX, float fScaleY )
    {
        TextInfo txtinf;
		utf32 color = utf32( cols.d_top_left.getARGB() );
		utf32 bkColor = 0xFC000000;
		// 在这里加入颜色转换字幅
		// 由于字体的颜色控制符又很多中,这里强行把 0xFA--0xFE作为控制符出现
		color = ( color & 0x00ffffff ) | 0xfB000000;  
		txtinf.text         = color;
		txtinf.text			+= bkColor +  text;
        txtinf.source_font  = font;
        txtinf.formatting   = format;
        txtinf.target_area  = destArea;
        txtinf.z_offset     = zOffset;
        txtinf.colours      = cols;
        txtinf.clipToDisplay = clipToDisplay;
		txtinf.fScaleX		= fScaleX;
		txtinf.fScaleY		= fScaleY;
		txtinf.OwnerWindow = OwnerWindow;

        if (clipper)
        {
            txtinf.customClipper = *clipper;
            txtinf.usingCustomClipper = true;
        }
        else
        {
            txtinf.usingCustomClipper = false;
        }

		//*-*-*-* BigCharacter
//		(( Font*)font )->preCheckString( text );
		//*-*-*-* BigCharacter

        d_cachedTexts.push_back(txtinf);
    }


} // End of  CEGUI namespace section

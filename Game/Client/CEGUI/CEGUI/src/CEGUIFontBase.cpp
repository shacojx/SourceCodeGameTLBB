/************************************************************************
	filename: 	CEGUIFontBase.cpp
	created:	15/2/2006
	author:		Jean
	
	purpose:	Implements Font class
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
#include "windows.h"
#include "CEGUIFontBase.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIFontManager.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUITexture.h"
#include "CEGUILogger.h"
#include "CEGUITextUtils.h"
#include "CEGUIFont_xmlHandler.h"
#include "CEGUIResourceProvider.h"
#include "CEGUIXMLParser.h"
#include "CEGUIAnimateManager.h"
#include "CEGUIHyperLinkManager.h"
#include "CEGUIWindow.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	static data definitions
*************************************************************************/
const argb_t	FontBase::DefaultColour			= 0xFFFFFFFF;
const uint		FontBase::InterGlyphPadSpace	= 2;
const uint		FontBase::FontGlyphImagesetSize	= 256;
const uint		FontBase::LineSpace				= 3;

static colour	g_curTextColor					= 0;	
static colour	g_curTextBakColor				= 0;
static int		g_isHyperLinkNow				= 0;
static String	g_keyHyperLink					= "";
static int		g_isKeyHyperLink				= 0;

static bool		g_isBlink						= false;

// XML related strings
const char		FontBase::FontSchemaName[]		= "Font.xsd";
// String objects hold some worlds to prepare font size.
const char		FontBase::DefaultPrepareString[]= "a\xE5\x95\x8A"; //"a啊"

class Window;
/*************************************************************************
	Constructs a new Font object from a font definition file
*************************************************************************/
FontBase::FontBase(const String& filename, const String& resourceGroup) :
	d_currentGlyphImageset(0),
    d_lineHeight(0),
    d_lineSpacing(0),
    d_max_bearingY(0),
    d_maxGlyphHeight(0),
	d_ptSize(0),
	d_ptNext_X(0),
	d_ptNext_Y(0),
	d_autoScale(false),
    d_horzScaling(1.0f),
    d_vertScaling(1.0f),
    d_nativeHorzRes(DefaultNativeHorzRes),
    d_nativeVertRes(DefaultNativeVertRes)
//	d_isHyperLink(0),
//	d_keyHyperLink("")
{
}

/*************************************************************************
	Load and complete construction of 'this' via an XML file
*************************************************************************/
void FontBase::load(const String& filename, const String& resourceGroup)
{
	// unload old data
	unload();

	if (filename.empty() || (filename == (utf8*)""))
	{
		throw InvalidRequestException((utf8*)"FontBase::load - Filename supplied for Font loading must be valid");
	}

    // create handler object
    Font_xmlHandler handler(this);

	// do parse (which uses handler to create actual data)
	try
	{
        System::getSingleton().getXMLParser()->parseXMLFile(handler, filename, FontSchemaName, resourceGroup);
	}
	catch(...)
	{
		unload();

        Logger::getSingleton().logEvent("FontBase::load - loading of Font from file '" + filename +"' failed.", Errors);
		throw;
	}
}


/*************************************************************************
	Destroys a Font object
*************************************************************************/
FontBase::~FontBase(void)
{
	unload();
}

/*************************************************************************
	Unload data associated with the font (font is then useless.
	this is intended for cleanup).
*************************************************************************/
void FontBase::unload(void)
{
	clearAllFontFaces();
}

/*************************************************************************
	Notify the Font of the current (usually new) display resolution.
*************************************************************************/
void FontBase::notifyScreenResolution(const Size& size)
{
	// notification should come from System which would have notified the Imageset anyway, but
	// in case client code decides to call us, we must pass on notification here too.
	GlyphImagesetList::iterator it, end = d_listGlyphImagesets.end();
	
	for(it=d_listGlyphImagesets.begin(); it!=end; it++)
	{
		glyphImageset* thisGlyph = *it;

		if(thisGlyph->d_imageset)
		{
			thisGlyph->d_imageset->notifyScreenResolution(size);
		}
	}

	d_horzScaling = size.d_width / d_nativeHorzRes;
	d_vertScaling = size.d_height / d_nativeVertRes;

	if (FreeType == getType() && d_autoScale)
	{
		resetFontFaces();
	}
}

/*************************************************************************
	Set the native resolution for this Font
*************************************************************************/
void FontBase::setNativeResolution(const Size& size)
{
	d_nativeHorzRes = size.d_width;
	d_nativeVertRes = size.d_height;

	// set native resolution for underlying imageset for bitmap fonts
	if(Bitmap == getType())
	{
		GlyphImagesetList::iterator it, end = d_listGlyphImagesets.end();
		
		for(it=d_listGlyphImagesets.begin(); it!=end; it++)
		{
			glyphImageset* thisGlyph = *it;

			if(thisGlyph->d_imageset)
			{
				thisGlyph->d_imageset->setNativeResolution(size);
			}
		}
	}
	// re-calculate scaling factors & notify images as required
	notifyScreenResolution(System::getSingleton().getRenderer()->getSize());
}

/*************************************************************************
	Enable or disable auto-scaling for this Font.
*************************************************************************/
void FontBase::setAutoScalingEnabled(bool setting)
{
	if (FreeType == getType() && setting != d_autoScale)
	{
		d_autoScale = setting;
		resetFontFaces();
	}
}

/*************************************************************************
	Prepare the character that need display use this font.
*************************************************************************/
void FontBase::prepareString(const String& text)
{
	if(text.empty()) return;
	size_t char_count = text.length();

	CodepointMap::const_iterator end = d_cp_map.end();
	for (size_t c = 0; c < char_count; ++c)
	{
		utf32 code = text[c];

		// check control code.
		if(code==(utf32)'\r' || code==(utf32)'\n') continue;
		utf32 control_value = (code>>24)&0XFF;
		if(control_value > 0)	continue;

		// check is in font map
		if(d_cp_map.find(code) == end)
		{
			createFontGlyph(code);
		}
	}
}

/*************************************************************************
	Prepare the font texture.
*************************************************************************/
void FontBase::prepareFontTexture(void)
{
	GlyphImagesetList::iterator it, end = d_listGlyphImagesets.end();
	
	for(it=d_listGlyphImagesets.begin(); it!=end; it++)
	{
		glyphImageset* thisGlyph = *it;

		// font glyph need update to texture.
		if(thisGlyph->d_dirty)
		{
			thisGlyph->d_imageset->getTexture()->loadFromMemory(
				thisGlyph->d_memory, FontGlyphImagesetSize, FontGlyphImagesetSize);
			thisGlyph->d_dirty = false;
		}
	}
}

/*************************************************************************
	Clean all font glyph imagesets and memory.
*************************************************************************/
void FontBase::clearAllFontFaces(void)
{
	// Clear font map
	d_cp_map.clear();

	// Clear font glyph imagesets.
	GlyphImagesetList::iterator it, end = d_listGlyphImagesets.end();
	for(it=d_listGlyphImagesets.begin(); it!=end; it++)
	{
		glyphImageset* thisGlyph = *it;

		delete[] thisGlyph->d_memory;
		thisGlyph->d_memory = NULL;

		thisGlyph->d_imageset->undefineAllImages();
		ImagesetManager::getSingleton().destroyImageset(thisGlyph->d_imageset->getName());

		delete thisGlyph;
	}

	d_listGlyphImagesets.clear();
	d_currentGlyphImageset = 0;
	d_ptNext_X = d_ptNext_Y = 0;
	d_maxGlyphHeight = 0;
	d_max_bearingY = 0;
	d_lineSpacing = 0;
	d_lineHeight = 0;
//	d_isHyperLink = 0;
//	d_keyHyperLink = "";
}

/*************************************************************************
	Create a new font glyph struct.
*************************************************************************/
FontBase::glyphImageset* FontBase::createFontImageset(void)
{
	// set dirty flag of old glyph
	if(d_currentGlyphImageset)
	{
		d_currentGlyphImageset->d_dirty = true;
	}

	// create new font glyph.
	d_currentGlyphImageset = new glyphImageset;

	d_currentGlyphImageset->d_dirty = true;

	// alloca font glyph memory.
	d_currentGlyphImageset->d_memory = new argb_t[FontGlyphImagesetSize * FontGlyphImagesetSize];
	memset(d_currentGlyphImageset->d_memory, 0, ((FontGlyphImagesetSize * FontGlyphImagesetSize) * sizeof(argb_t)));

	// create font texture(Imageset)
	char szTemp[260] = {0};
	_snprintf(szTemp, 260, "%s_bigchar_%d", d_name.c_str(), (int)d_listGlyphImagesets.size());
	d_currentGlyphImageset->d_imageset = 
		ImagesetManager::getSingleton().createImageset( 
			String(szTemp), 
			System::getSingleton().getRenderer()->createTexture(float(FontGlyphImagesetSize)));
	d_currentGlyphImageset->d_imageset->setAutoFree( false );
	d_currentGlyphImageset->d_imageset->setTextureLoadDelay( false );
	d_currentGlyphImageset->d_imageset->defineImage(
		"view", Point( 0.0f, 0.0f ), 
		Size((float)FontGlyphImagesetSize, (float)FontGlyphImagesetSize ), 
		Point(0.0f, 0.0f));

	//insert imageset list
	d_listGlyphImagesets.push_back(d_currentGlyphImageset);

	// reset next pos
	d_ptNext_X = d_ptNext_Y = 0;

	return d_currentGlyphImageset;
}

/*************************************************************************
	Return the pixel width of the specified text if rendered with this Font.
*************************************************************************/
float FontBase::getTextExtent(const String& text, float x_scale) const
{
	// prepare string
	((FontBase*)this)->prepareString(text);

    float cur_extent = 0, adv_extent = 0;
	const String KeyParserBegin	= "{";
	const String KeyParserEnd	= "}";

	bool bIsKeyHyperLink = false,bIsHyperLink = false;

    size_t char_count = text.length();
    CodepointMap::const_iterator pos, end = d_cp_map.end();

    for (size_t c = 0; c < char_count; ++c)
    {
        pos = d_cp_map.find(text[c]);

        if (pos != end)
        {
			if(bIsHyperLink)
			{
				if(text[c] == KeyParserBegin[0])
				{
					bIsKeyHyperLink = true;
					continue;
				}
				else if(text[c] ==KeyParserEnd[0])
				{
					bIsKeyHyperLink = false;
					continue;
				}
				else if(bIsKeyHyperLink)
				{
					continue;
				}
			}
            //adv_extent += (pos->second.d_image->getWidth() + pos->second.d_image->getOffsetX()) * x_scale;
            adv_extent += (float)pos->second.d_horz_advance * x_scale;
			
        }
		else
		{

			utf32 value = text[c] & 0x00FFFFFF;
			utf32 type = (text[c] >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;

					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;
					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);

					adv_extent += (float)animate->getSize().d_width * x_scale;
				}
				break;
			case 0xFD:
				{
					bIsHyperLink = true;
				}
				break;
			case 0xFE:
				{
					bIsHyperLink = false;
				}
				break;
			case 0xFC:
				break;
			case 0xFB:
				break;
			default:
				break;
			}
		}

    }

    return ceguimax(adv_extent, cur_extent);
}

/*************************************************************************
	Return the horizontal pixel extent given text would be formatted to.	
*************************************************************************/
float FontBase::getFormattedTextExtent(const String& text, const Rect& format_area, TextFormatting fmt, float x_scale) const
{
	float lineWidth;
	float widest = 0;

	size_t lineStart = 0, lineEnd = 0;
	String	currLine;

	while (lineEnd < text.length())
	{
		if ((lineEnd = text.find_first_of('\n', lineStart)) == String::npos)
		{
			lineEnd = text.length();
		}

		currLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;	// +1 to skip \n char

		switch(fmt)
		{
		case Centred:
		case RightAligned:
		case LeftAligned:
			lineWidth = getTextExtent(currLine, x_scale);
			break;

		case Justified:
			// usually we use the width of the rect but we have to ensure the current line is not wider than that
			lineWidth = ceguimax(format_area.getWidth(), getTextExtent(currLine, x_scale));
			break;

		case WordWrapLeftAligned:
		case WordWrapRightAligned:
		case WordWrapCentred:
			lineWidth = getWrappedTextExtent(currLine, format_area.getWidth(), x_scale);
			break;

		case WordWrapJustified:
			// same as above
			lineWidth = ceguimax(format_area.getWidth(), getWrappedTextExtent(currLine, format_area.getWidth(), x_scale));
			break;

		default:
			throw InvalidRequestException((utf8*)"Font::getFormattedTextExtent - Unknown or unsupported TextFormatting value specified.");
		}

		if (lineWidth > widest)
		{
			widest = lineWidth;
		}

	}

	return widest;
}


/*************************************************************************
	returns extent of widest line of wrapped text.
*************************************************************************/
float FontBase::getWrappedTextExtent(const String& text, float wrapWidth, float x_scale) const
{
	float lineWidth = getTextExtent(text, x_scale);
	if( lineWidth > wrapWidth )
		return wrapWidth;
	return lineWidth;
}

/*************************************************************************
	Return the index of the closest text character in String 'text' that
	corresponds to pixel location 'pixel' if the text were rendered.
*************************************************************************/
size_t FontBase::getCharAtPixel(const String& text, size_t start_char, float pixel, float x_scale) const
{
	float cur_extent = 0;
	size_t char_count = text.length();

	// handle simple cases
	if ((pixel <= 0) || (char_count <= start_char))
	{
		return start_char;
	}

	CodepointMap::const_iterator pos, end = d_cp_map.end();

	for (size_t c = start_char; c < char_count; ++c)
	{
		pos = d_cp_map.find(text[c]);

		if (pos != end)
		{
			cur_extent += (float)pos->second.d_horz_advance * x_scale;

			if (pixel < cur_extent)
			{
				return c;
			}

		}
		//extern control code
		else
		{
			utf32 value = text[c] & 0x00FFFFFF;
			utf32 type  = (text[c] >> 24)&0xFF;

			switch(type)
			{
			//face motion
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;

					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;

					const Animate *animate = AnimateManager::getSingleton().getAnimate(id);
					cur_extent += animate->getSize().d_width;

					if (pixel < cur_extent)
					{
						return c;
					}
				}
				break;

			default:
				break;
			}

		}

	}

	return char_count;
}

/*************************************************************************
	Return the size of the given text would be formatted to.
*************************************************************************/
Size FontBase::getFormattedSize(const String& text, const Rect& draw_area, TextFormatting fmt, float x_scale, float y_scale) const
{
	Size szRet(0.0f, 0.0f), sz;
	if(text.empty()) return szRet;

	Rect tmpDrawArea(
		PixelAligned(draw_area.d_left),
		PixelAligned(draw_area.d_top),
		PixelAligned(draw_area.d_right),
		PixelAligned(draw_area.d_bottom)
		);

	size_t lineStart = 0, lineEnd = 0;
	String	currLine;

	while (lineEnd < text.length())
	{
		if(lineStart != 0) 
		{
			szRet.d_height += LineSpace;
		}

		if ((lineEnd = text.find_first_of('\n', lineStart)) == String::npos)
		{
			lineEnd = text.length();
		}

		currLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;	// +1 to skip \n char

		switch(fmt)
		{
		case LeftAligned:
		case RightAligned:
		case Centred:
		case Justified:
			sz = getLineSize(currLine, x_scale, y_scale);
			break;

		case WordWrapLeftAligned:
		case WordWrapRightAligned:
		case WordWrapCentred:
		case WordWrapJustified:
			sz = getWrappedSize(currLine, tmpDrawArea, x_scale, y_scale);
			tmpDrawArea.d_top += sz.d_height;
			break;

		default:
			throw InvalidRequestException((utf8*)"Font::drawText - Unknown or unsupported TextFormatting value specified.");
		}

		szRet.d_height += sz.d_height;
		szRet.d_width = ceguimax(szRet.d_width, sz.d_width);
	}

	return szRet;
}

/*************************************************************************
	Return the size of the given wrapped text.
*************************************************************************/
Size FontBase::getWrappedSize(const String& text, const Rect& draw_area, float x_scale, float y_scale) const
{
	Size szRet( 0, 0 );
	float	wrap_width = draw_area.getWidth();
	((FontBase*)this)->prepareString(text);

    float cur_extent = 0;
	float maxHeight = getFontHeight(), height;

    size_t char_count = text.length();
    CodepointMap::const_iterator pos, end = d_cp_map.end();
	String thisLine;
	int  nCount = 0;
    for (size_t c = 0; c < char_count; ++c)
    {
        pos = d_cp_map.find(text[c]);

		String tempLine = thisLine + text[c];
		float adv_extent = getTextExtent( tempLine, x_scale );
        
        if ( adv_extent > cur_extent)
        {
            cur_extent = adv_extent;
        }

		if( pos != d_cp_map.end() )
		{
			const Image* img = pos->second.d_image;
			height = img->getHeight() * y_scale;
			if(height > maxHeight) maxHeight = height;
		}
		else
		{
			utf32 value = text[c] & 0x00FFFFFF;
			utf32 type = (text[c] >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;
					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;
 
					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);

					height = animate->getSize().d_height;
					if(height > maxHeight) maxHeight = height;
				}
				break;
			}
		}
		
		if( adv_extent > wrap_width )
		{
			cur_extent = wrap_width;
			adv_extent = (float)pos->second.d_horz_advance * x_scale;
			szRet.d_height += maxHeight + LineSpace;
			maxHeight = 0;
			cur_extent = wrap_width;
			
			if(thisLine.size() > 1) --c;
			thisLine.clear(); 
		}
		else thisLine += text[c];
		nCount ++;
    }

	szRet.d_width = cur_extent;
	szRet.d_height += maxHeight;
	return szRet;

}

/*************************************************************************
	Return the size of the given single line text would be formatted to.
*************************************************************************/
Size FontBase::getLineSize(const String& text, float x_scale, float y_scale) const
{
	// prepare character in this tring.
	((FontBase*)this)->prepareString(text);

    float cur_extent = 0, adv_extent = 0, width;
	float maxHeight = getFontHeight(), height;

	const String KeyParserBegin	= "{";
	const String KeyParserEnd	= "}";

	bool bIsKeyHyperLink = false,bIsHyperLink = false;

    size_t char_count = text.length();
    CodepointMap::const_iterator pos, end = d_cp_map.end();

    for (size_t c = 0; c < char_count; ++c)
    {
        pos = d_cp_map.find(text[c]);

        if (pos != end)
        {
//			if(g_isHyperLinkNow || bIsHyperLink)
			if(bIsHyperLink)
			{
				if(text[c] == KeyParserBegin[0])
				{
					bIsKeyHyperLink = true;
					continue;
				}
				else if(text[c] ==KeyParserEnd[0])
				{
					bIsKeyHyperLink = false;
					continue;
				}
				else 
//				if((bIsKeyHyperLink) || (g_isHyperLinkNow))
				if(bIsKeyHyperLink)
				{
					continue;
				}
			}
            width = (pos->second.d_image->getWidth() + pos->second.d_image->getOffsetX()) * x_scale;
            
            if (adv_extent + width > cur_extent)
            {
                cur_extent = adv_extent + width;
            }
            
            adv_extent += (float)pos->second.d_horz_advance * x_scale;

			const Image* img = pos->second.d_image;
			height = img->getHeight() * y_scale;
			if(height > maxHeight) maxHeight = height;
			if(height/y_scale > d_lineHeight) *((float*)&d_lineHeight) = height/y_scale;
        }
		else
		{
			utf32 value = text[c] & 0x00FFFFFF;
			utf32 type = (text[c] >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;
					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;
 
					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);

					height = animate->getSize().d_height;
					if(height > maxHeight) maxHeight = height;

					adv_extent += (float)animate->getSize().d_width * x_scale;
				}
				break;
			case 0xFD:
				{
					bIsHyperLink = true;
				}
				break;
			case 0xFE:
				{
					bIsHyperLink = false;
				}
				break;
 

			default:
				break;
			}
		}

    }

	Size szRet;
	szRet.d_width = ceguimax(adv_extent, cur_extent); 
	szRet.d_height = maxHeight * y_scale;

	return szRet;
}

/*************************************************************************
	helper function for renderWrappedText to get next word of a string
*************************************************************************/
size_t FontBase::getNextWord(const String& in_string, size_t start_idx, String& out_string) const
{
	out_string = in_string.substr(start_idx, 1);
	return 1;
}

/*************************************************************************
	Renders text on the display.  Return number of lines output.
*************************************************************************/
DrawTextResult FontBase::drawText(const Window* OwnerWindow, const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours, float x_scale, float y_scale ) const
{
	size_t thisCount;
	size_t lineCount = 0;

	float	y_top = draw_area.d_top;

	Rect tmpDrawArea(
		PixelAligned(draw_area.d_left),
		PixelAligned(draw_area.d_top),
		PixelAligned(draw_area.d_right),
		PixelAligned(draw_area.d_bottom)
		);

	size_t lineStart = 0, lineEnd = 0;
	String	currLine;
	float totalHeight = 0.0f;
	DrawTextResult dtmp;

	while (lineEnd < text.length())
	{
		if ((lineEnd = text.find_first_of('\n', lineStart)) == String::npos)
		{
			lineEnd = text.length();
		}

		currLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;	// +1 to skip \n char

		switch(fmt)
		{
		case LeftAligned:
			dtmp = drawTextLine(OwnerWindow, currLine, Vector3(tmpDrawArea.d_left, y_top, z), clip_rect, colours, x_scale, y_scale);
			thisCount = 1;
			y_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		case RightAligned:
			dtmp = drawTextLine(OwnerWindow, currLine, Vector3(tmpDrawArea.d_right - getTextExtent(currLine, x_scale), y_top, z), clip_rect, colours, x_scale, y_scale);
			thisCount = 1;
			y_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		case Centred:
			dtmp = drawTextLine(OwnerWindow, currLine, Vector3(PixelAligned(tmpDrawArea.d_left + ((tmpDrawArea.getWidth() - getTextExtent(currLine, x_scale)) / 2.0f))+1, y_top, z), clip_rect, colours, x_scale, y_scale);
			thisCount = 1;
			y_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;
		case WordWrapLeftAligned:
			dtmp = drawWrappedText( OwnerWindow, currLine, tmpDrawArea, z, clip_rect, LeftAligned, colours, x_scale, y_scale);
			thisCount = dtmp.d_lineCount;
			tmpDrawArea.d_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		case WordWrapRightAligned:
			dtmp = drawWrappedText( OwnerWindow, currLine, tmpDrawArea, z, clip_rect, RightAligned, colours, x_scale, y_scale);
			thisCount = dtmp.d_lineCount;
			tmpDrawArea.d_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		case WordWrapCentred:
			dtmp = drawWrappedText( OwnerWindow,currLine, tmpDrawArea, z, clip_rect, Centred, colours, x_scale, y_scale);
			thisCount = dtmp.d_lineCount;
			tmpDrawArea.d_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		case WordWrapJustified:
			// no change needed
			dtmp = drawWrappedText(OwnerWindow, currLine, tmpDrawArea, z, clip_rect, Justified, colours, x_scale, y_scale);
			thisCount = dtmp.d_lineCount;
			tmpDrawArea.d_top += dtmp.d_totalHeight;
			totalHeight += dtmp.d_totalHeight;
			break;

		default:
			throw InvalidRequestException((utf8*)"Font::drawText - Unknown or unsupported TextFormatting value specified.");
		}

		lineCount += thisCount;

	}

	DrawTextResult dret;
    // should not return 0
	dret.d_lineCount = ceguimax(lineCount, (size_t)1);
	dret.d_totalHeight = totalHeight;

    return dret;
}

/*************************************************************************
	draws wrapped text
*************************************************************************/
DrawTextResult FontBase::drawWrappedText(const Window* OwnerWindow, const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours, float x_scale, float y_scale ) const
{
	size_t	line_count = 0;
	Rect	dest_area(draw_area);
	float	wrap_width = draw_area.getWidth();
	String	thisLine, thisWord;
	float	total_height = 0.0f;
	DrawTextResult dtmp;
	size_t char_count = text.length();

	for (size_t c = 0; c < char_count; ++c)
	{
		CodepointMap::const_iterator pos = d_cp_map.find(text[c]);
		thisWord = text[ c ];

		String tempLine = thisLine + text[c];

		float warp_extern = getTextExtent(tempLine, x_scale );
		if( warp_extern > wrap_width )
		{
			// output what we had until this new word
			dtmp = drawText(OwnerWindow, thisLine, dest_area, z, clip_rect, fmt, colours, x_scale, y_scale);
			line_count += dtmp.d_lineCount;

			// reset for a new line.
			if(thisLine.size() > 1) --c;
			thisLine.clear();

			// update y co-ordinate for next line
			dest_area.d_top += dtmp.d_totalHeight;
			dest_area.d_bottom += dtmp.d_totalHeight;
			total_height += dtmp.d_totalHeight;
			warp_extern = 0;
		}
		else 
		{
			// add the next word to the line
			thisLine += text[c];
		}
	}

	// Last line is left aligned
	TextFormatting last_fmt = (fmt == Justified ? LeftAligned : fmt);
	// output last bit of string
	dtmp = drawText(OwnerWindow, thisLine, dest_area, z, clip_rect, last_fmt, colours, x_scale, y_scale );
	line_count += dtmp.d_lineCount;
	total_height += dtmp.d_totalHeight;

	DrawTextResult dret;
	dret.d_lineCount = line_count;
	dret.d_totalHeight = total_height;
	return dret;
}

void FontBase::drawExternBorder(const Image* img, const Size& szBorder, const Vector3& cur_pos, const Rect& clip_rect) const
{
	//Draw exten border
	if((g_curTextBakColor.getRed() != 0.0f ||g_curTextBakColor.getGreen() != 0.0f || g_curTextBakColor.getBlue() != 0.0f) )
	{
		float blinkAlpha = 0;
		if(FontManager::getSingletonPtr())
			blinkAlpha = FontManager::getSingleton().getBlinkAlphaValue();

		colour col = g_curTextBakColor;
		col.setAlpha(g_isBlink ? blinkAlpha : g_curTextColor.getAlpha());

		for(register int ii=-1; ii<=1; ii++)
		{
			for(register int jj=-1; jj<=1; jj++)
			{
				if(ii==0 && jj==0) continue;

				Vector3 vBorderPos = cur_pos;
				vBorderPos.d_x += jj;
				vBorderPos.d_y += ii;
				img->draw(vBorderPos, szBorder, clip_rect, ColourRect(col, col, col, col));
			}
		}
	}

}

/*************************************************************************
	Draw a line of text.  No formatting is applied. returns draw result struct.
*************************************************************************/
DrawTextResult FontBase::drawTextLine(const Window* OwnerWindow, const String& text, const Vector3& position, const Rect& clip_rect, const ColourRect& colours, float x_scale, float y_scale) const
{
	Vector3	cur_pos(position);

	float top_y = position.d_y;
	const String	KeyParser	= "{}";

	size_t char_count = text.length();
	CodepointMap::const_iterator	pos, end = d_cp_map.end();

	float			blinkAlpha = 1.0f;
	unsigned long	lquid = 0;

	//Caculate max height and line base.
	float total_height = getFontHeight();
	for (size_t c = 0; c < char_count; ++c)
	{
		pos = d_cp_map.find(text[c]);

		if (pos != end)
		{
			const Image* img = pos->second.d_image;
			float height = img->getHeight() * y_scale;
			if(height > total_height) total_height = height;
			if(height/y_scale > d_lineHeight) *((float*)&d_lineHeight) = height/y_scale;
		}
		else
		{
			utf32 value = text[c] & 0x00FFFFFF;
			utf32 type = (text[c] >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;
					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;
					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);
					
					float height = animate->getSize().d_height;
					if(height > total_height) total_height = height;
				}
				break;
			default:
				break;
			}
		}
	}

	float base_y = top_y+total_height-InterGlyphPadSpace-1;

	if(FontManager::getSingletonPtr())
		blinkAlpha = FontManager::getSingleton().getBlinkAlphaValue();

	for (size_t c = 0; c < char_count; ++c)
	{
		pos = d_cp_map.find(text[c]);

		if (pos != end)
		{
			const Image* img = pos->second.d_image;
			cur_pos.d_y = base_y - (img->getOffsetY() - img->getOffsetY() * y_scale);
			Size sz(img->getWidth() * x_scale, img->getHeight() * y_scale);

			if(g_isHyperLinkNow)
			{
				if(text[c] == KeyParser[0] )
				{
					g_isKeyHyperLink = 1;
					continue;
				}
				else if(text[c] == KeyParser[1] )
				{
					g_isKeyHyperLink = 0;
					continue;
				}
				else if (g_isKeyHyperLink == 1)
				{
					g_keyHyperLink += text[c];
					continue;
				}
				else
				{
					if( HyperLinkManager::getSingletonPtr() )
					{
						Vector2 vRange;
						vRange.d_x = cur_pos.d_x;
						vRange.d_y = top_y;
						Rect newRect(vRange,sz);
						HyperLinkManager::getSingleton().addHyperLink(OwnerWindow,g_keyHyperLink,newRect);
						colour	curTextBakColor = 0;
						utf32	curValue = 0;

						//curValue |= utf32( colours.d_top_left.getAlpha() * 255 ) << 24;
						curValue |= utf32(0xFF00CCFF);
						curTextBakColor.setARGB(curValue);
						
						drawExternBorder(img, sz, cur_pos, clip_rect);

						if(g_isBlink && FontManager::getSingletonPtr())
						{
							curTextBakColor.setAlpha(blinkAlpha);
							lquid = img->draw(cur_pos, sz, clip_rect, ColourRect(curTextBakColor, curTextBakColor, curTextBakColor, curTextBakColor));
							FontManager::getSingleton().addRuntimeBlink(img, lquid, cur_pos, sz, clip_rect, 
								ColourRect(curTextBakColor, curTextBakColor, curTextBakColor, curTextBakColor));
						}
						else
						{
							img->draw(cur_pos, sz, clip_rect, ColourRect(curTextBakColor, curTextBakColor, curTextBakColor, curTextBakColor));
						}
						cur_pos.d_x += (float)pos->second.d_horz_advance * x_scale;

						continue;
					}
				}
			}

			drawExternBorder(img, sz, cur_pos, clip_rect);

			if(g_isBlink && FontManager::getSingletonPtr())
			{
				//adjust alpha
				colour blinkColor(g_curTextColor);
				blinkColor.setAlpha(blinkAlpha);

				//draw blink txt
				lquid = img->draw(cur_pos, sz, clip_rect, ColourRect(blinkColor, blinkColor, blinkColor, blinkColor));
				FontManager::getSingleton().addRuntimeBlink(img, lquid, cur_pos, sz, clip_rect, 
					ColourRect(blinkColor, blinkColor, blinkColor, blinkColor));
			}
			else
			{
				img->draw(cur_pos, sz, clip_rect, ColourRect(g_curTextColor, g_curTextColor, g_curTextColor, g_curTextColor));
			}

			cur_pos.d_x += (float)pos->second.d_horz_advance * x_scale;
		}
		else
		{
			utf32 value = text[c] & 0x00FFFFFF;
			utf32 type = (text[c] >> 24)&0xFF;
			switch(type)
			{
			case 0xFA:
			case 0xE3:
				{
					int id = (type == 0xE3)?(-1*(int)value):(int)value;
					if(!AnimateManager::getSingleton().isAnimatePresent(id)) break;
 
					const Animate* animate = AnimateManager::getSingleton().getAnimate(id);

					cur_pos.d_y = base_y - animate->getSize().d_height+LineSpace;
					Size sz(animate->getSize());
					//超链接的{}中不允许有#1~#99这种表情出现。过滤掉，不显示。
					if( g_isHyperLinkNow == 1)
					{
						if( g_isKeyHyperLink == 1 )
							break;
						if( HyperLinkManager::getSingletonPtr() )
						{
							Vector2 vRange;
							vRange.d_x = cur_pos.d_x;
							vRange.d_y = cur_pos.d_y;
							Rect newRect(vRange,sz);
							HyperLinkManager::getSingleton().addHyperLink(OwnerWindow,g_keyHyperLink,newRect);
						}
					}

					//Defulat: 0.1sec per frame
					int time_total = animate->getFrameNumber()*100;
					int time_now = (int)(System::getSingleton().getCurTimeElapsed()*1000.0f);
					colour col( 1,1,1,1 );
					col.setAlpha( g_curTextColor.getAlpha() );
					AnimateManager::getSingleton().draw(id, 0, time_now, time_total, 
						cur_pos, sz, clip_rect, col );
					cur_pos.d_x += animate->getSize().d_width;
				}
				break;
 
			case 0xFB:
				{
					value |= utf32( colours.d_top_left.getAlpha() * 255 ) << 24;
					g_curTextColor.setARGB( value );
				}
				break;
 
			case 0xFC:
				{
					value |= utf32( colours.d_top_left.getAlpha() * 255 ) << 24;
					g_curTextBakColor.setARGB( value );
				}
				break;
			case 0xFD:
				{
					g_isHyperLinkNow = 1;
				}
				break;
			case 0xFE:
				{
					g_isHyperLinkNow = 0;
					g_keyHyperLink = "";
				}
				break;
			case 0xE0:
				{
					g_isBlink = true;
				}
				break;
			case 0xE1:
				{
					g_isBlink = false;
				}
			default:
				break;
			}
		}

	}

 
	DrawTextResult dret;
	dret.d_lineCount = 1;
	dret.d_totalHeight = total_height + LineSpace;
	return dret;
}

int FontBase::debug_GetFontImageset(std::vector< Imageset* >& vTexture)
{
	if(d_listGlyphImagesets.size() > vTexture.size()) 
		return (int)d_listGlyphImagesets.size();

	GlyphImagesetList::iterator it;
	int i=0;
	for(it=d_listGlyphImagesets.begin(); it!=d_listGlyphImagesets.end(); it++, i++)
	{
		vTexture[i] = (*it)->d_imageset;
	}
	return(int)d_listGlyphImagesets.size();
}

} // End of  CEGUI namespace section


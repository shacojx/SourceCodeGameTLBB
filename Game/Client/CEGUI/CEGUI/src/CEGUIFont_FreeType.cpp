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
#include "CEGUIFont_FreeType.h"
#include "CEGUIFont_ImplData.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUISystem.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{

Font_FreeType::Font_FreeType(const String& filename, const String& resourceGroup, FontImplData* dat) :
	FontBase(filename, resourceGroup),
	d_impldat(dat)
{

}

/*************************************************************************
    Destroys a Font object
*************************************************************************/
Font_FreeType::~Font_FreeType(void)
{
    unload();
    delete d_impldat;
}

/*************************************************************************
	Function to do real work of constructor
*************************************************************************/
void Font_FreeType::constructor_impl(const String& name, const String& fontname, const String& resourceGroup, uint size)
{
	String		errMsg;

    System::getSingleton().getResourceProvider()->loadRawDataContainer(fontname, d_impldat->fontData, resourceGroup);

	// create face using input font
	if (FT_New_Memory_Face(d_impldat->library, d_impldat->fontData.getDataPtr(), 
                (FT_Long)d_impldat->fontData.getSize(), 0, &d_impldat->fontFace) == 0)
	{
		// check that default Unicode character map is available
		if (d_impldat->fontFace->charmap != NULL)	
		{
			try
			{
				d_name = name;
				d_ptSize = size;

				// reprepare font datas.
				resetFontFaces();

				return;
			}
			catch(...)
			{
				FT_Done_Face(d_impldat->fontFace);

				// re-throw
				throw;
			}

		}
		// missing Unicode character map
		else
		{
			FT_Done_Face(d_impldat->fontFace);

			errMsg = (utf8*)"Font_FreeType::constructor_impl - The source font '" + fontname +"' does not have a Unicode charmap, and cannot be used.";
		}

	}
	// failed to create face (a problem with the font file?)
	else
	{
		errMsg = (utf8*)"Font_FreeType::constructor_impl - An error occurred while trying to create a FreeType face from source font '" + fontname + "'.";
	}

	throw GenericException(errMsg);
}


/*************************************************************************
    Unload data associated with the font (font is then useless.
    this is intended for cleanup).
*************************************************************************/
void Font_FreeType::unload(void)
{
    d_cp_map.clear();

    // cleanup FreeType face if this is a FreeType based font.
	if(d_impldat)
	{
		FT_Done_Face(d_impldat->fontFace);
	    System::getSingleton().getResourceProvider()->unloadRawDataContainer(d_impldat->fontData);
    }
}

/*************************************************************************
	Recreate font glyph data map and imageset.
*************************************************************************/
void Font_FreeType::resetFontFaces(void)
{
	if(d_impldat->fontFace == 0) return;

	uint horzDpi = System::getSingleton().getRenderer()->getHorzScreenDPI();
	uint vertDpi = System::getSingleton().getRenderer()->getVertScreenDPI();

	if (d_autoScale)
	{
		horzDpi = (uint)(((float)horzDpi) * d_horzScaling);
		vertDpi = (uint)(((float)vertDpi) * d_vertScaling);
	}

	if (FT_Set_Char_Size(d_impldat->fontFace, 0, d_ptSize * 64, horzDpi, vertDpi) != 0)
	{
		// failed to set size for font
		throw GenericException((utf8*)"Font_FreeType::createFontFromFT_Face - An error occurred while creating a source font with the requested size.");
	}

	// clear font faces and memory
	clearAllFontFaces();

	//insert default data to set font size data
	prepareString((utf8*)DefaultPrepareString);

	d_lineHeight = (float)d_maxGlyphHeight;

	// calculate spacing and base-line
    d_max_bearingY = ((float)d_impldat->fontFace->ascender / (float)d_impldat->fontFace->units_per_EM) * (float)d_impldat->fontFace->size->metrics.y_ppem;
	d_lineSpacing = ((float)d_impldat->fontFace->height / (float)d_impldat->fontFace->units_per_EM) * (float)d_impldat->fontFace->size->metrics.y_ppem;
}

/*************************************************************************
	Paint the code into the current glyphImageset, This also defines an Image for the glyph in 
	the Imageset for this font, and creates an entry in the code point to Image map.
*************************************************************************/
void Font_FreeType::createFontGlyph(const utf32 code)
{
	//Check is control code
	if(((code >> 24)&0XFF) != 0) return;

	//Load character glyph data from freetype 
	if (FT_Load_Char(d_impldat->fontFace, code, 
				FT_LOAD_RENDER|FT_LOAD_NO_AUTOHINT|FT_LOAD_TARGET_NORMAL))
	{
		return;
	}

	// update font maxheight
	if ((uint)d_impldat->fontFace->glyph->bitmap.rows > d_maxGlyphHeight)
	{
		d_maxGlyphHeight = d_impldat->fontFace->glyph->bitmap.rows;
	}

	// font glyph width
	uint nWidth = d_impldat->fontFace->glyph->bitmap.width + InterGlyphPadSpace;

	if(!d_currentGlyphImageset)// create new font imageset
	{
		d_currentGlyphImageset = createFontImageset();
	}
	else
	{
		// Check is enough space to paint new character glyph in this line
		if( d_ptNext_X + nWidth > FontGlyphImagesetSize ) // new line
		{
			d_ptNext_Y += d_maxGlyphHeight;
			d_ptNext_X = 0;

			// check is enough space in this imageset.
			if(d_ptNext_Y + d_maxGlyphHeight > FontGlyphImagesetSize)
			{
				d_currentGlyphImageset = createFontImageset();
			}
		}
	}

	// calculate offset into buffer for this glyph
	argb_t* dest_buff = 
		d_currentGlyphImageset->d_memory + d_ptNext_Y*FontGlyphImagesetSize + d_ptNext_X;

	// draw glyph into buffer
	drawGlyphToBuffer(code, dest_buff, FontGlyphImagesetSize);

	// define Image on Imageset for this glyph to save re-rendering glyph later
	Rect rect;
	rect.d_left		= (float)d_ptNext_X;
	rect.d_top		= (float)d_ptNext_Y;
	rect.d_right	= (float)(d_ptNext_X + nWidth - InterGlyphPadSpace);
	rect.d_bottom	= (float)(d_ptNext_Y + d_maxGlyphHeight);

	Point offset;
	offset.d_x		=  (float)(d_impldat->fontFace->glyph->metrics.horiBearingX >> 6);
	offset.d_y		= -(float)(d_impldat->fontFace->glyph->metrics.horiBearingY >> 6);

	String	imageName;
	imageName = code;
	d_currentGlyphImageset->d_imageset->defineImage(imageName, rect, offset);

	//Next Point
	d_ptNext_X += nWidth;

	// create entry in code-point to Image map
	glyphDat	dat;
	dat.d_image = &(d_currentGlyphImageset->d_imageset->getImage(imageName));
	dat.d_horz_advance = d_impldat->fontFace->glyph->advance.x >> 6;

	d_cp_map[code] = dat;

	//set dirty flag
	d_currentGlyphImageset->d_dirty = true;
}

/*************************************************************************
	Copy the FreeType glyph bitmap into the given memory buffer
*************************************************************************/
void Font_FreeType::drawGlyphToBuffer(utf32 /*code*/, argb_t* buffer, uint buf_width)
{
	FT_Bitmap* glyph_bitmap = &d_impldat->fontFace->glyph->bitmap;

	for (int i = 0; i < glyph_bitmap->rows; ++i)
	{
		for (int j = 0; j < glyph_bitmap->width; ++j)
		{
			switch (glyph_bitmap->pixel_mode)
			{
			case FT_PIXEL_MODE_GRAY:
				{
					uchar* bytebuff = reinterpret_cast<uchar*>(&buffer[j]);
					*bytebuff++ = 0xFF;
					*bytebuff++ = 0xFF;
					*bytebuff++ = 0xFF;
					*bytebuff = glyph_bitmap->buffer[(i * glyph_bitmap->pitch) + j];
				}
				break;

			case FT_PIXEL_MODE_MONO:
				buffer[j] = ((glyph_bitmap->buffer[(i * glyph_bitmap->pitch) + j / 8] << (j % 8)) & 0x80) ? 0xFFFFFFFF : 0x00000000;
				break;

			default:
				throw InvalidRequestException((utf8*)"Font::drawGlyphToBuffer - The glyph could not be drawn because the pixel mode is unsupported.");
				break;
			}

		}

		buffer += buf_width;
	}

}

} // End of  CEGUI namespace section

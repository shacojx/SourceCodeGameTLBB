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
#include "CEGUIFont_Bitmap.h"
#include "CEGUIDataContainer.h"
#include "CEGUISystem.h"
#include "CEGUIExceptions.h"
#include "CEGUIImageset.h"
#include "CEGUIImage.h"

#include <windows.h>

// Start of CEGUI namespace section
namespace CEGUI
{

const int g_nAsciiBegin	= 0X20;
const int g_nAsciiEnd	= 0X7F;

const int g_n1GBKBegin	= 0X81;
const int g_n1GBKEnd	= 0XFE;

const int g_n2GBKBegin1	= 0X40;
const int g_n2GBKEnd1	= 0X7E;
const int g_n2GBKBegin2	= 0X80;
const int g_n2GBKEnd2	= 0XFE;

const int g_nLineCount  = g_n1GBKEnd-g_n1GBKBegin+1;
const int g_nLineWidth1 = g_n2GBKEnd1-g_n2GBKBegin1+1;
const int g_nLineWidth2 = g_n2GBKEnd2-g_n2GBKBegin2+1;

// Invalid code "¡õ" (0xA1F5)
const int INVALID_FIRST		= 0xA1;
const int INVALID_SECOND	= 0xF5;

Font_Bitmap::Font_Bitmap(const String& filename, const String& resourceGroup) :
	FontBase(filename, resourceGroup),
	d_fontData(0),
	d_dataBuf(0)
{
	d_fontData = new RawDataContainer;
}

Font_Bitmap::~Font_Bitmap()
{
	d_dataBuf = 0;

	System::getSingleton().getResourceProvider()->unloadRawDataContainer(*d_fontData);
	delete d_fontData;
	d_fontData = 0;
}

/*************************************************************************
	Function to do real work of constructor
*************************************************************************/
void Font_Bitmap::constructor_impl(const String& name, const String& fontname, const String& resourceGroup, uint size)
{
	// check size valid
	if(12!=size && 16!=size)
	{
		throw GenericException("Font_Bitmap::constructor_impl: Unsupported font size!(" + name + ")");
	}

	d_name = name;
	d_ptSize = size;

	System::getSingleton().getResourceProvider()->loadRawDataContainer(fontname, *d_fontData, resourceGroup);

	// set data point
	d_dataBuf = (char*)(d_fontData->getDataPtr());

	// reprepare font datas.
	resetFontFaces();
}

/*************************************************************************
	Recreate font glyph data map and imageset.
*************************************************************************/
void Font_Bitmap::resetFontFaces(void)
{
	if(d_dataBuf == 0) return;

	// clear font faces and memory
	clearAllFontFaces();

	//insert default data to set font size data
	prepareString((utf8*)DefaultPrepareString);

	d_lineHeight = (float)d_ptSize+InterGlyphPadSpace;

	// calculate spacing and base-line
    d_max_bearingY = (float)d_lineHeight;
	d_lineSpacing = (float)d_ptSize+InterGlyphPadSpace;
}

/*************************************************************************
	Paint the code into the current glyphImageset, This also defines an Image for the glyph in 
	the Imageset for this font, and creates an entry in the code point to Image map.
*************************************************************************/
void Font_Bitmap::createFontGlyph(utf32 code)
{
	//Check is control code
	if(((code >> 24)&0XFF) != 0) return;

	int nWidth = (int)d_ptSize;
	int nHeight = (int)d_ptSize + InterGlyphPadSpace;

	if(!d_currentGlyphImageset)// create new font imageset
	{
		d_currentGlyphImageset = createFontImageset();
	}
	else
	{
		// Check is enough space to paint new character glyph in this line
		if( d_ptNext_X + nWidth > FontGlyphImagesetSize ) // new line
		{
			d_ptNext_Y += nHeight;
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
	rect.d_right	= (float)(d_ptNext_X + nWidth);
	rect.d_bottom	= (float)(d_ptNext_Y + nHeight);

	Point offset;
	offset.d_x		=  -1;
	offset.d_y		= -(float)(nHeight-3);

	String	imageName;
	imageName = code;
	d_currentGlyphImageset->d_imageset->defineImage(imageName, rect, offset);

	//Next Point
	d_ptNext_X += nWidth;

	if(nHeight > (int)d_maxGlyphHeight) d_maxGlyphHeight = nHeight;

	// create entry in code-point to Image map
	glyphDat	dat;
	dat.d_image = &(d_currentGlyphImageset->d_imageset->getImage(imageName));
	dat.d_horz_advance = nWidth+1;
	if(code >= g_nAsciiBegin && code <= g_nAsciiEnd) dat.d_horz_advance /= 2;

	d_cp_map[code] = dat;

	//set dirty flag
	d_currentGlyphImageset->d_dirty = true;
}

/*************************************************************************
	Copy the FreeType glyph bitmap into the given memory buffer
*************************************************************************/
void Font_Bitmap::drawGlyphToBuffer(utf32 code, argb_t* buffer, uint buf_width)
{
	int nOffset = 0;
	int nFontSize = (int)d_ptSize;

	// Check is Acsii code
	if( code >= (CEGUI::utf32)g_nAsciiBegin && code <= (CEGUI::utf32)g_nAsciiEnd )
	{
		nOffset = code-g_nAsciiBegin;
	}
	else
	{
		// UNICODE32 -> GBK (Use Windows API!)
		wchar_t wTemp[8];
		wTemp[0] = (wchar_t)(code&0xFFFF);
		wTemp[1] = 0;

		char pMBCS[8] = {0};
		::WideCharToMultiByte(CP_ACP, 0, wTemp, 1, pMBCS, 8, 0, 0);

		int nFirstCode  = (int)(unsigned char)pMBCS[0];
		int nSecondCode = (int)(unsigned char)pMBCS[1];

		// Check validity
		if(	nFirstCode < g_n1GBKBegin		|| 
			nFirstCode > g_n1GBKEnd 		||
			nSecondCode < g_n2GBKBegin1		||
			nSecondCode > g_n2GBKEnd1 && nSecondCode < g_n2GBKBegin2 ||
			nSecondCode > g_n2GBKEnd2 )
		{
			// Invalid code!
			nFirstCode = INVALID_FIRST;
			nSecondCode = INVALID_SECOND;
		}

		// Go to memory
		nOffset = (nFirstCode-g_n1GBKBegin)*(g_nLineWidth1+g_nLineWidth2) + (g_nAsciiEnd-g_nAsciiBegin+1);
		if(nSecondCode <= g_n2GBKEnd1)
		{
			nOffset += (nSecondCode-g_n2GBKBegin1);
		}
		else
		{
			nOffset += (nSecondCode-g_n2GBKBegin2 + g_nLineWidth1);
		}
	}

	const char* pPoint = d_dataBuf + nOffset*((nFontSize*nFontSize)>>3);

	switch(nFontSize)
	{
	case 12:
		{
			for(register int i=0; i<12; i++)
			{
				if(i&1)	//1, 3, 5, ...
				{
					unsigned char byTemp = 
						*((unsigned char*)(pPoint + (((unsigned int)(i-1))>>1)*3 + 2));

					// *+8
					for(register int j=0; j<8; j++)
					{
						register unsigned char* bytebuff = (unsigned char*)(buffer+j+4);

						*bytebuff++ = 0xFF;
						*bytebuff++ = 0xFF;
						*bytebuff++ = 0xFF;
						*bytebuff = (byTemp & (((unsigned char)1)<<(7-j))) ? 0xFF : 0;
					}

					buffer += buf_width;
				}
				else
				{
					unsigned short wTemp = 
						*((unsigned short*)(pPoint + (((unsigned int)i)>>1)*3));

					for(register int j=0; j<12; j++)
					{
						register unsigned char* bytebuff = (unsigned char*)(buffer+j);

						// line one
						*bytebuff++ = 0xFF;
						*bytebuff++ = 0xFF;
						*bytebuff++ = 0xFF;
						*bytebuff = (wTemp & ((unsigned short)1)<<(15-j)) ? 0xFF : 0;
					}

					buffer += buf_width;

					for(register int j=0; j<4; j++)
					{
						register unsigned char* bytebuff = (unsigned char*)(buffer+j);

						// line two
						*bytebuff++ = 0xFF;
						*bytebuff++ = 0xFF;
						*bytebuff++ = 0xFF;
						*bytebuff = (wTemp & ((unsigned short)1)<<(3-j)) ? 0xFF : 0;
					}
				}
			}
		}
		break;

	case 16:
		{
			for(register int i=0; i<16; i++)
			{
				unsigned short wTemp = *((unsigned short*)(pPoint + i*2));

				for(register int j=0; j<16; j++)
				{
					register unsigned char* bytebuff = (unsigned char*)(buffer+j);

					*bytebuff++ = 0xFF;
					*bytebuff++ = 0xFF;
					*bytebuff++ = 0xFF;
					*bytebuff = (wTemp & (1<<(15-j))) ? 0xFF : 0;
				}
		
				buffer += buf_width;
			}
		}
		break;

	default:
		assert(false);
		break;
	}


}


} // End of  CEGUI namespace section

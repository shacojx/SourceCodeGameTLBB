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
#ifndef _CEGUIFontBitmap_h_
#define _CEGUIFontBitmap_h_

#include "CEGUIBase.h"
#include "CEGUIFontBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{
class RawDataContainer;
/*!
\brief
	Class that encapsulates text rendering functionality for a typeface

	A Font object is created for each unique typeface required.  The Font class provides
	methods for loading typefaces from various sources, and then for outputting text via
	the Renderer object.
*/

class CEGUIEXPORT Font_Bitmap : public FontBase
{
	friend class FontManager;

public:
	/*************************************************************************
		Informational methods
	*************************************************************************/

	/*!
	\brief
		Return the type of this font.

	\return
		the type of this font.
	*/
	virtual FontType	getType(void) const { return Bitmap; }

private:
	/*************************************************************************
		Private forward refs
	*************************************************************************/
	struct FontImplData;


	/*************************************************************************
		Construction & Destruction
	*************************************************************************/
	/*!
	\brief
		Constructs a new Font object

	\param filename
		The filename of the "font definition file" to be used in creating this font.

    \param resourceGroup
        Resource group identifier to be passed to the resource provider to load the font
        definition file.

	\exception	FileIOException				thrown if there was some problem accessing or parsing the file \a filename
	\exception	InvalidRequestException		thrown if an invalid filename was provided.
	\exception	AlreadyExistsException		thrown if a Font Imageset clashes with one already defined in the system.
	\exception	GenericException			thrown if something goes wrong while accessing a true-type font referenced in file \a filename.
	\exception	RendererException			thrown if the Renderer can't support a texture large enough to hold the requested glyph imagery.
	\exception	MemoryException				thrown if allocation of imagery construction buffer fails.
	*/
	Font_Bitmap(const String& filename, const String& resourceGroup);

public:		// For luabind support
	/*!
	\brief
		Destroys a Font object
	*/
	virtual ~Font_Bitmap(void);

protected:
	/*************************************************************************
		Implementation Methods
	*************************************************************************/

	/*!
	\brief
		Function to do real work of constructor.  Used to save duplication in the various constructor overloads.
	*/
	virtual void	constructor_impl(const String& name, const String& fontname, const String& resourceGroup, uint size);

	/*!
	\brief
		Paint the code into the current glyphImageset, This also defines an Image for the glyph in 
		the Imageset for this font, and creates an entry in the code point to Image map.

	\param code
		which glyph are to be loaded into the buffer.

	\return
		Nothing.
	*/
	virtual void	createFontGlyph(utf32 code);

	/*!
	\brief
		Recreate font glyph data map and imageset.
	*/
	virtual void	resetFontFaces(void);

	/*!
	\brief
		Copy the current glyph data into \a buffer, which has a width of \a buf_width pixels (not bytes).

	\param buffer
		Memory buffer large enough to receive the imagery for the currently loaded glyph.

	\param buf_width
		Width of \a buffer in pixels (where each pixel is a argb_t).

	\return
		Nothing.
	*/
	virtual void	drawGlyphToBuffer(utf32 code, argb_t* buffer, uint buf_width);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
    RawDataContainer*	d_fontData;
	const char*			d_dataBuf;
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIFontFreeType_h_
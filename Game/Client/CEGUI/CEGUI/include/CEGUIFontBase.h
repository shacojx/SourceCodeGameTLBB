/************************************************************************
	filename: 	CEGUIFontBase.h
	created:	15/2/2006
	author:		Jean
	
	purpose:	Defines interface for the FontBase class
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
#ifndef _CEGUIFontBase_h_
#define _CEGUIFontBase_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIRect.h"
#include "CEGUIColour.h"
#include "CEGUIVector.h"
#include "CEGUIColourRect.h"
//#include "CEGUIWindow.h"

#include <list>
#include <map>
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Enumerated type that contains the valid flags that can be passed to createFont when creating a new font.
*/
enum FontType
{
	FreeType,	//!< Fonts generated from TrueType
	Bitmap,		//!< Fonts generated from bitmap data.
};

/*!
\brief
	Enumerated type that contains valid formatting types that can be specified when rendering text into a Rect area (the formatting Rect).
*/
enum TextFormatting
{
	LeftAligned	= 0,			//!< All text is printed on a single line.  The left-most character is aligned with the left edge of the formatting Rect.
	RightAligned,			//!< All text is printed on a single line.  The right-most character is aligned with the right edge of the formatting Rect.
	Centred,				//!< All text is printed on a single line.  The text is centred horizontally in the formatting Rect.
	Justified,				//!< All text is printed on a single line.  The left-most and right-most characters are aligned with the edges of the formatting Rect.
	WordWrapLeftAligned,	//!< Text is broken into multiple lines no wider than the formatting Rect.  The left-most character of each line is aligned with the left edge of the formatting Rect.
	WordWrapRightAligned,	//!< Text is broken into multiple lines no wider than the formatting Rect.  The right-most character of each line is aligned with the right edge of the formatting Rect.
	WordWrapCentred, 		//!< Text is broken into multiple lines no wider than the formatting Rect.  Each line is centred horizontally in the formatting Rect.
	WordWrapJustified 		//!< Text is broken into multiple lines no wider than the formatting Rect.  The left-most and right-most characters of each line are aligned with the edges of the formatting Rect.
};

/*!
\brief
	Struct that contains drawtext result.
*/
struct CEGUIEXPORT DrawTextResult
{
	int		d_lineCount;		//!< The number of lines output.  NB: This does not consider clipping, so if all text was clipped, this would still return >=1.
	float	d_totalHeight;		//!< Float value describing the pixel height of the text area.
};

/*!
\brief
	Class that encapsulates text rendering functionality for a typeface

	A Font object is created for each unique typeface required.  The Font class provides
	methods for loading typefaces from various sources, and then for outputting text via
	the Renderer object.
*/
class Window;
class CEGUIEXPORT FontBase
{
	friend class Font_xmlHandler;
	
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	static const argb_t		DefaultColour;			//!< Colour value used whenever a colour is not specified.
	static const uint		LineSpace;
	
	/*************************************************************************
		Informational methods
	*************************************************************************/

	/*!
	\brief
		Return the type of this font.

	\return
		the type of this font.
	*/
	virtual FontType	getType(void) const = 0;

	/*!
	\brief
		Return the name of this font.

	\return
		String object holding the name of this font.
	*/
	const String&	getName(void) const		{return	d_name;}

	/*!
	\brief
		Notify the Font of the current (usually new) display resolution.

	\param size
		Size object describing the display resolution

	\return
		Nothing
	*/
	void	notifyScreenResolution(const Size& size);

	/*!
	\brief
		Prepare the character that need display use this font.

	\param text
		String object that store the characters to draw later.

	\return
		Nothing
	*/
	void	prepareString(const String& text);

	/*!
	\brief
		Set the native resolution for this Font

	\param size
		Size object describing the new native screen resolution for this Font.

	\return
		Nothing
	*/
	void	setNativeResolution(const Size& size);

	/*!
	\brief
		Enable or disable auto-scaling for this Font.

	\param setting
		true to enable auto-scaling, false to disable auto-scaling.

	\return
		Nothing.
	*/
	void	setAutoScalingEnabled(bool setting);

	/*!
	\brief
		Return whether this Font can currently draw the specified code-point

	\param cp
		utf32 code point that is the subject of the query.

	\return
		true if the font contains a mapping for code point \a cp, false if it does not contain a mapping for \a cp.
	*/
	bool	isCodepointAvailable(utf32 cp) const		{return (d_cp_map.find(cp) != d_cp_map.end());}

	/*************************************************************************
		Text drawing methods
	*************************************************************************/
	/*!
	\brief
		Draw text into a specified area of the display.

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is not clipped to this Rect, but is formatted
		using this Rect depending upon the option specified in \a fmt.

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param clip_rect
		Rect object describing the clipping area for the drawing.  No drawing will occur outside this Rect.

	\param fmt
		One of the TextFormatting values specifying the text formatting required.

	\param colours
		ColourRect object describing the colours to be applied when drawing the text.  NB: The colours specified in here are applied to each glyph,
		rather than the text as a whole.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\param y_scale
		Scaling factor to be applied to each glyph's y axis, where 1.0f is considered to be 'normal'.

	\return
		DrawTextResult object that draw text result.
	*/
	DrawTextResult	drawText(const Window* OwnerWindow, const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours, float x_scale = 1.0f, float y_scale = 1.0f ) const;


	/*!
	\brief
		Draw text into a specified area of the display using default colours.

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is not clipped to this Rect, but is formatted
		using this Rect depending upon the option specified in \a fmt.

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param clip_rect
		Rect object describing the clipping area for the drawing.  No drawing will occur outside this Rect.

	\param fmt
		One of the TextFormatting values specifying the text formatting required.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\param y_scale
		Scaling factor to be applied to each glyph's y axis, where 1.0f is considered to be 'normal'.

	\return
		DrawTextResult object that draw text result.
	*/
	DrawTextResult	drawText(const Window* OwnerWindow,const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, float x_scale = 1.0f, float y_scale = 1.0f) const
	{ return drawText( OwnerWindow, text, draw_area, z, clip_rect, fmt, ColourRect(DefaultColour, DefaultColour, DefaultColour, DefaultColour), x_scale, y_scale ); }


	/*!
	\brief
		Draw text into a specified area of the display with default colours and default formatting (LeftAligned).

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is not clipped to this Rect, but is formatted
		using this Rect depending upon the option specified in \a fmt.

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param clip_rect
		Rect object describing the clipping area for the drawing.  No drawing will occur outside this Rect.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\param y_scale
		Scaling factor to be applied to each glyph's y axis, where 1.0f is considered to be 'normal'.

	\return
		Nothing.
	*/
	void	drawText(const Window* OwnerWindow, const String& text, const Rect& draw_area, float z, const Rect& clip_rect, float x_scale = 1.0f, float y_scale = 1.0f) const
	{ drawText( OwnerWindow, text, draw_area, z, clip_rect, LeftAligned, ColourRect(DefaultColour, DefaultColour, DefaultColour, DefaultColour), x_scale, y_scale); }


	/*!
	\brief
		Draw text into a specified area of the display.

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is formatted using this Rect depending
		upon the option specified in \a fmt.  Additionally, the drawn text is clipped to be within this Rect (applies to non-word wrapped formatting
		where the text may otherwise have fallen outside this Rect).

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param fmt
		One of the TextFormatting values specifying the text formatting required.

	\param colours
		ColourRect object describing the colours to be applied when drawing the text.  NB: The colours specified in here are applied to each glyph,
		rather than the text as a whole.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\param y_scale
		Scaling factor to be applied to each glyph's y axis, where 1.0f is considered to be 'normal'.

	\return
		DrawTextResult object that draw text result.
	*/
	DrawTextResult	drawText(const Window* OwnerWindow,const String& text, const Rect& draw_area, float z, TextFormatting fmt, const ColourRect& colours, float x_scale = 1.0f, float y_scale = 1.0f) const
	{ return drawText( OwnerWindow, text, draw_area, z, draw_area, fmt, colours, x_scale, y_scale); }


	/*!
	\brief
		Draw text into a specified area of the display with default colours.

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is formatted using this Rect depending
		upon the option specified in \a fmt.  Additionally, the drawn text is clipped to be within this Rect (applies to non-word wrapped formatting
		where the text may otherwise have fallen outside this Rect).

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param fmt
		One of the TextFormatting values specifying the text formatting required.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\param y_scale
		Scaling factor to be applied to each glyph's y axis, where 1.0f is considered to be 'normal'.

	\return
		DrawTextResult object that draw text result.
	*/
	DrawTextResult	drawText(const Window* OwnerWindow,const String& text, const Rect& draw_area, float z, TextFormatting fmt, float x_scale = 1.0f, float y_scale = 1.0f ) const
	{ return drawText( OwnerWindow, text, draw_area, z, draw_area, fmt, ColourRect(DefaultColour, DefaultColour, DefaultColour, DefaultColour), x_scale, y_scale); }


	/*!
	\brief
		Draw text into a specified area of the display with default colours and default formatting (LeftAligned).

	\param text
		String object containing the text to be drawn.

	\param draw_area
		Rect object describing the area of the display where the text is to be rendered.  The text is formatted using this Rect depending
		upon the option specified in \a fmt.  Additionally, the drawn text is clipped to be within this Rect (applies to non-word wrapped formatting
		where the text may otherwise have fallen outside this Rect).

	\param z
		flat value specifying the z co-ordinate for the drawn text.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\param y_scale
		Scaling factor to be applied to each glyph's y axis, where 1.0f is considered to be 'normal'.

	\return
		Nothing.
	*/
	void	drawText(const Window* OwnerWindow,const String& text, const Rect& draw_area, float z, float x_scale = 1.0f, float y_scale = 1.0f) const
	{ drawText( OwnerWindow, text, draw_area, z, draw_area, LeftAligned, ColourRect(DefaultColour, DefaultColour, DefaultColour, DefaultColour), x_scale, y_scale); }


	/*!
	\brief
		Draw text at the specified location.

	\param text
		String object containing the text to be drawn.

	\param position
		Vector3 object describing the location for the text.  NB: The position specified here corresponds to the text baseline and not the
		top of any glyph.  The baseline spacing required can be retrieved by calling getBaseline().
		
	\param clip_rect
		Rect object describing the clipping area for the drawing.  No drawing will occur outside this Rect.

	\param colours
		ColourRect object describing the colours to be applied when drawing the text.  NB: The colours specified in here are applied to each glyph,
		rather than the text as a whole.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\param y_scale
		Scaling factor to be applied to each glyph's y axis, where 1.0f is considered to be 'normal'.

	\return
		Nothing.
	*/
	void	drawText(const Window* OwnerWindow,const String& text, const Vector3& position, const Rect& clip_rect, const ColourRect& colours, float x_scale = 1.0f, float y_scale = 1.0f) const
	{ drawText( OwnerWindow, text, Rect(position.d_x, position.d_y, position.d_x, position.d_y), position.d_z, clip_rect, LeftAligned, colours, x_scale, y_scale); }


	/*!
	\brief
		Draw text at the specified location with default colours.

	\param text
		String object containing the text to be drawn.

	\param position
		Vector3 object describing the location for the text.  NB: The position specified here corresponds to the text baseline and not the
		top of any glyph.  The baseline spacing required can be retrieved by calling getBaseline().
		
	\param clip_rect
		Rect object describing the clipping area for the drawing.  No drawing will occur outside this Rect.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\param y_scale
		Scaling factor to be applied to each glyph's y axis, where 1.0f is considered to be 'normal'.

	\return
		Nothing.
	*/
	void	drawText(const Window* OwnerWindow,const String& text, const Vector3& position, const Rect& clip_rect, float x_scale = 1.0f, float y_scale = 1.0f ) const
	{ drawText(OwnerWindow, text, Rect(position.d_x, position.d_y, position.d_x, position.d_y), position.d_z, clip_rect, LeftAligned, ColourRect(DefaultColour, DefaultColour, DefaultColour, DefaultColour), x_scale, y_scale); }

	/*!
	\brief
		Return the size of the given text would be formatted to.

		Since text formatting can result in multiple lines of text being output, it can be useful to know
		how many lines would be output without actually rendering the text.

	\param text
		String object containing the text to be measured.

	\param format_area
		Rect object describing the area to be used when formatting the text depending upon the option specified in \a fmt.

	\param fmt
		One of the TextFormatting values specifying the text formatting required.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\return
		The Size object produced from the specified formatting
	*/
	Size	getFormattedSize(const String& text, const Rect& format_area, TextFormatting fmt, float x_scale = 1.0f, float y_scale = 1.0f) const;

	/*!
	\brief
		Return the pixel line spacing value for.

	\param y_scale
		Scaling factor to be applied to the line spacing, where 1.0f is considered to be 'normal'.

	\return
		Number of pixels between vertical base lines, i.e. The minimum pixel space between two lines of text.
	*/
	float	getLineSpacing(float y_scale = 1.0f) const		{return d_lineSpacing * y_scale;}


	/*!
	\brief
		return the exact pixel height of the font.

	\param y_scale
		Scaling factor to be applied to the height, where 1.0f is considered to be 'normal'.

	\return
		float value describing the pixel height of the font without any additional padding.
	*/
	float	getFontHeight(float y_scale = 1.0f) const	{return d_lineHeight * y_scale;}


	/*!
	\brief
		Return the number of pixels from the top of the highest glyph to the baseline

	\param y_scale
		Scaling factor to be applied to the baseline distance, where 1.0f is considered to be 'normal'.

	\return
		pixel spacing from top of front glyphs to baseline
	*/
	float	getBaseline(float y_scale = 1.0f) const			{return d_max_bearingY * y_scale;}

	/*!
	\brief
		Return the pixel width of the specified text if rendered with this Font.

	\param text
		String object containing the text to return the rendered pixel width for.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis when measuring the extent, where 1.0f is considered to be 'normal'.

	\return
		Number of pixels that \a text will occupy when rendered with this Font.
	*/
	float	getTextExtent(const String& text, float x_scale = 1.0f) const;

	/*!
	\brief
		Return the horizontal pixel extent given text would be formatted to.

		The value return by this method is basically the extent of the widest line within the formatted text.

	\param text
		String object containing the text to be measured.

	\param format_area
		Rect object describing the area to be used when formatting the text depending upon the option specified in \a fmt.

	\param fmt
		One of the TextFormatting values specifying the text formatting required.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\return
		The widest pixel extent of the lines produced from the specified formatting.
	*/
	float	getFormattedTextExtent(const String& text, const Rect& format_area, TextFormatting fmt, float x_scale = 1.0f) const;

	/*!
	\brief
		returns extent of widest line of wrapped text.
	*/
	float	getWrappedTextExtent(const String& text, float wrapWidth, float x_scale = 1.0f) const;

	/*!
	\brief
		Return the index of the closest text character in String \a text that corresponds to pixel location \a pixel if the text were rendered.

	\param text
		String object containing the text.

	\param pixel
		Specifies the (horizontal) pixel offset to return the character index for.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis when measuring the text extent, where 1.0f is considered to be 'normal'.

	\return
		Returns a character index into String \a text for the character that would be rendered closest to horizontal pixel offset \a pixel if the
		text were to be rendered via this Font.  Range of the return is from 0 to text.length(), so may actually return an index past the end of
		the string, which indicates \a pixel was beyond the last character.
	*/
	size_t	getCharAtPixel(const String& text, float pixel, float x_scale = 1.0f) const		{return getCharAtPixel(text, 0, pixel, x_scale);}

	/*!
	\brief
		Return the index of the closest text character in String \a text, starting at character index \a start_char, that corresponds
		to pixel location \a pixel if the text were to be rendered.

	\param text
		String object containing the text.

	\param start_char
		index of the first character to consider.  This is the lowest value that will be returned from the call.

	\param pixel
		Specifies the (horizontal) pixel offset to return the character index for.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis when measuring the text extent, where 1.0f is considered to be 'normal'.

	\return
		Returns a character index into String \a text for the character that would be rendered closest to horizontal pixel offset \a pixel if the
		text were to be rendered via this Font.  Range of the return is from 0 to text.length(), so may actually return an index past the end of
		the string, which indicates \a pixel was beyond the last character.
	*/
	size_t	getCharAtPixel(const String& text, size_t start_char, float pixel, float x_scale = 1.0f) const;

	
	/*!
	\brief
		Return the inner texture of this font(for debuger only)

	\return
		Returns imageset buf length of request.
	*/
	int debug_GetFontImageset(std::vector< Imageset* >& vTexture);

	void drawExternBorder(const Image* img, const Size& szBorder, const Vector3& cur_pos, const Rect& clip_rect) const;

protected:
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
	FontBase(const String& filename, const String& resourceGroup);

public:		// For luabind support
	/*!
	\brief
		Destroys a Font object
	*/
	virtual ~FontBase(void);

protected:
	/*************************************************************************
		Implementation Constants
	*************************************************************************/
	static const char	FontSchemaName[];			//!< Filename of the XML schema used for validating Font files.
	static const char	DefaultPrepareString[];		//!< String objects hold some worlds to prepare font size.
	static const uint	InterGlyphPadSpace;			//!< Pad space between glyphs.
	static const uint	FontGlyphImagesetSize;		//!< Font texture size.

	/*************************************************************************
		Friends so that only FontManager can create and destroy font objects
	*************************************************************************/
	friend class FontManager;

	/*************************************************************************
		Implementation structs
	*************************************************************************/
	// font glyph 
	struct glyphImageset
	{
		argb_t*		d_memory;		//!< Memory that holds font glyph
		Imageset*	d_imageset;		//!< Imageset that holds the glyphs for this font.
		bool		d_dirty;		//!< Flag need to recreate this imageset(font texture)
	};

	/*!
	\brief
		struct to hold extra details about a glyph (required for proper rendering)
	*/
	struct glyphDat
	{
		const Image*	d_image;					//!< The image which will be rendered.
		int				d_horz_advance;				//!< Amount to advance the pen after rendering this glyph
		int				d_horz_advance_unscaled;	//!< original unscaled advance value (only used with static / bitmap fonts).
		unsigned int	d_countsUsed;				//!< used counts before last waste clean up.
	};

	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
		Load and complete construction of 'this' via an XML file

	\param filename
		String object holding the name of the XML file that holds details about the font to create.

    \param resourceGroup
        Resource group identifier to be passed to the resource provider when loading the font
        definition file.

	\return
		Nothing.
	*/
	void	load(const String& filename, const String& resourceGroup);


	/*!
	\brief
		Unloads data associated with the font (font is then useless.  this is intended for cleanup).

	\return
		Nothing.
	*/
	virtual void	unload(void);

	/*!
	\brief
		Function to do real work of constructor.  Used to save duplication in the various constructor overloads.
	*/
	virtual void	constructor_impl(const String& name, const String& fontname, const String& resourceGroup, uint size) = 0;

	/*!
	\brief
		Recreate font glyph data map and imageset.
	*/
	virtual void	resetFontFaces(void) = 0;

	/*!
	\brief
		Clean all font glyph imagesets and memory.
	*/
	void	clearAllFontFaces(void);

	/*!
	\brief
		Paint the code into the current glyphImageset, This also defines an Image for the glyph in 
		the Imageset for this font, and creates an entry in the code point to Image map.

	\param code
		which glyph are to be loaded into the buffer.

	\return
		Nothing.
	*/
	virtual void	createFontGlyph(utf32 code) = 0;

	/*!
	\brief
		Create a new font glyph struct.

	\return
		Nothing.
	*/
	glyphImageset*	createFontImageset(void);

	/*!
	\brief
		Prepare the font texture.
		Check all glyph Imageset and recreate the dirtyed texture.

	\return
		Nothing
	*/
	void	prepareFontTexture(void);

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
	virtual void	drawGlyphToBuffer(utf32 code, argb_t* buffer, uint buf_width) = 0;

	/*!
	\brief
		draws wrapped text.  returns draw result struct.
	*/
	DrawTextResult	drawWrappedText(const Window* OwnerWindow, const String& text, const Rect& draw_area, float z, const Rect& clip_rect, TextFormatting fmt, const ColourRect& colours, float x_scale = 1.0f, float y_scale = 1.0f) const;


	/*!
	\brief
		helper function for renderWrappedText to get next word of a string
	*/
	size_t	getNextWord(const String& in_string, size_t start_idx, String& out_string) const;


	/*************************************************************************
		Text drawing implementation methods
	*************************************************************************/

	/*!
	\brief
		Draw a line of text.  No formatting is applied. returns draw result struct.
	*/
	DrawTextResult	drawTextLine(const Window* OwnerWindow, const String& text, const Vector3& position, const Rect& clip_rect, const ColourRect& colours, float x_scale = 1.0f, float y_scale = 1.0f) const;


	/*!
	\brief
		Draw a justified line of text.
	*/
	void	drawTextLineJustified(const Window* OwnerWindow,const String& text, const Rect& draw_area, const Vector3& position, const Rect& clip_rect, const ColourRect& colours, float x_scale = 1.0f, float y_scale = 1.0f) const;
public:
	/*!
	\brief
		Return the size of the given single line text.

	\param text
		String object containing the text to be measured.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\param y_scale
		Scaling factor to be applied to each glyph's y axis, where 1.0f is considered to be 'normal'.

	\return
		The Size object produced from the specified formatting
	*/
	Size	getLineSize(const String& text, float x_scale = 1.0f, float y_scale = 1.0f) const;

	/*!
	\brief
		Return the size of the given wrapped text.

	\param text
		String object containing the text to be measured.

	\param format_area
		Rect object describing the area to be used when formatting the text depending upon the option specified in \a fmt.

	\param x_scale
		Scaling factor to be applied to each glyph's x axis, where 1.0f is considered to be 'normal'.

	\param y_scale
		Scaling factor to be applied to each glyph's y axis, where 1.0f is considered to be 'normal'.

	\return
		The Size object produced from the specified formatting
	*/
	Size	getWrappedSize(const String& text, const Rect& format_area, float x_scale = 1.0f, float y_scale = 1.0f) const;
protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	String	d_name;				//!< Name of this font.
	uint	d_ptSize;			//!< Point size of font.
	float	d_lineHeight;		//!< Exact pixel height of font.
	float	d_lineSpacing;		//!< Spacing between multiple lines.
	float	d_max_bearingY;		//!< Maximum bearingY value (gives required spacing down to baseline).
//	int		d_isHyperLink;		//!< Whether this char is a HyperLink.
//	String	d_keyHyperLink;		//!< the key word of HyperLink.
	uint	d_maxGlyphHeight;	//!< Height of the largest glyph (calculated in getRequiredTextureSize)

	typedef		std::map<utf32, glyphDat>		CodepointMap;
	CodepointMap	d_cp_map;	//!< Contains mappings from code points to Image objects

	// font glyph fields
	typedef		std::list< glyphImageset* >		GlyphImagesetList;
	GlyphImagesetList	d_listGlyphImagesets;	//!< All font glyph images
	glyphImageset*		d_currentGlyphImageset;	//!< Current glyphdata 
	uint				d_ptNext_X, d_ptNext_Y; //!< Next pos to add new font at current glyph.

	// auto-scaling fields
	bool	d_autoScale;			//!< true when auto-scaling is enabled.
	float	d_horzScaling;			//!< current horizontal scaling factor.
	float	d_vertScaling;			//!< current vertical scaling factor.
	float	d_nativeHorzRes;		//!< native horizontal resolution for this Imageset.
	float	d_nativeVertRes;		//!< native vertical resolution for this Imageset.


};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIFontBase_h_
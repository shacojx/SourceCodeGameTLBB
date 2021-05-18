/************************************************************************
filename: 	CEGUIFont.cpp
created:	21/2/2004
author:		Paul D Turner

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
#include "CEGUIFont_xmlHandler.h"

#include "CEGUIExceptions.h"
#include "CEGUIImageset.h"
#include "CEGUILogger.h"
#include "CEGUIXMLAttributes.h"

#include <ft2build.h>
#include FT_FREETYPE_H


// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
static data definitions
*************************************************************************/

// XML related strings
const String Font_xmlHandler::FontElement( (utf8*)"Font" );
const char	Font_xmlHandler::FontNameAttribute[]			= "Name";
const char	Font_xmlHandler::FontFilenameAttribute[]		= "Filename";
const char	Font_xmlHandler::FontResourceGroupAttribute[]   = "ResourceGroup";
const char	Font_xmlHandler::FontTypeAttribute[]			= "Type";
const char	Font_xmlHandler::FontSizeAttribute[]			= "Size";
const char	Font_xmlHandler::FontNativeHorzResAttribute[]	= "NativeHorzRes";
const char	Font_xmlHandler::FontNativeVertResAttribute[]	= "NativeVertRes";
const char	Font_xmlHandler::FontAutoScaledAttribute[]		= "AutoScaled";

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
SAX2 Handler methods
*************************************************************************/
void Font_xmlHandler::elementStart(const String& element, const XMLAttributes& attributes)
{
	if (element == FontElement)
	{
		// get name of font we are creating
		String font_name(attributes.getValueAsString(FontNameAttribute));

		// get filename for the font
        String filename(attributes.getValueAsString(FontFilenameAttribute));
        // get resource group for font file.
        String resourceGroup(attributes.getValueAsString(FontResourceGroupAttribute));

		Logger::getSingleton().logEvent("Started creation of Font '" + font_name + "' via XML file.", Informative);

		//
		// load auto-scaling configuration
		//
		float hres, vres;
		bool auto_scale;

		// get native horizontal resolution
        hres = (float)attributes.getValueAsInteger(FontNativeHorzResAttribute, 640);
		// get native vertical resolution
        vres = (float)attributes.getValueAsInteger(FontNativeVertResAttribute, 480);
		// get size of font
		uint size = (uint)attributes.getValueAsInteger(FontSizeAttribute, 12);

		// perform pre-initialisation
		d_font->setNativeResolution(Size(hres, vres));

		// get auto-scaling setting
		auto_scale = attributes.getValueAsBool(FontAutoScaledAttribute, false);
		d_font->setAutoScalingEnabled(auto_scale);

		// Finalise construction of font.
		d_font->constructor_impl(font_name, filename, resourceGroup, size);
	}
	// anything else is an error which *should* have already been caught by XML validation
	else
	{
		throw FileIOException("Font::xmlHandler::startElement - Unexpected data was found while parsing the Font file: '" + element + "' is unknown.");
	}

}

void Font_xmlHandler::elementEnd(const String& element)
{
	if (element == FontElement)
	{
		Logger::getSingleton().logEvent("Finished creation of Font '" + d_font->d_name + "' via XML file.", Informative);
	}

}

} // End of  CEGUI namespace section

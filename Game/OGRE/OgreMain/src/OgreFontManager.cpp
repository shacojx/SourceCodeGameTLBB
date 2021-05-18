/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)

For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License (LGPL) as 
published by the Free Software Foundation; either version 2.1 of the 
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public 
License for more details.

You should have received a copy of the GNU Lesser General Public License 
along with this library; if not, write to the Free Software Foundation, 
Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA or go to
http://www.gnu.org/copyleft/lesser.txt
-------------------------------------------------------------------------*/
#include "OgreStableHeaders.h"

#include "OgreFontManager.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreStringVector.h"
#include "OgreException.h"
#include "OgreResourceGroupManager.h"

namespace Ogre
{
    //---------------------------------------------------------------------
    template<> FontManager * Singleton< FontManager >::ms_Singleton = 0;
    FontManager* FontManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    FontManager& FontManager::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }
    //---------------------------------------------------------------------
	FontManager::FontManager() : ResourceManager()
	{
        // Loading order
        mLoadOrder = 200.0f;
		// Scripting is supported by this manager
		mScriptPatterns.push_back("*.fontdef");
		// Register scripting with resource group manager
		ResourceGroupManager::getSingleton()._registerScriptLoader(this);

		// Resource type
		mResourceType = "Font";

		// Register with resource group manager
		ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);


	}
	//---------------------------------------------------------------------
	FontManager::~FontManager()
	{
		// Unregister with resource group manager
		ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
		// Unegister scripting with resource group manager
		ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);

	}
	//---------------------------------------------------------------------
	Resource* FontManager::createImpl(const String& name, ResourceHandle handle, 
		const String& group, bool isManual, ManualResourceLoader* loader,
        const NameValuePairList* params)
	{
		return new Font(this, name, handle, group, isManual, loader);
	}
	//---------------------------------------------------------------------
    void FontManager::parseScript(DataStreamPtr& stream, const String& groupName)
    {
        String line;
        FontPtr pFont;

        while( !stream->eof() )
        {
            line = stream->getLine();
            // Ignore blanks & comments
            if( !line.length() || line.substr( 0, 2 ) == "//" )
            {
                continue;
            }
            else
            {
			    if (pFont.isNull())
			    {
				    // No current font
				    // So first valid data should be font name
				    pFont = create(line, groupName);
					pFont->_notifyOrigin(stream->getName());
				    // Skip to and over next {
                    stream->skipLine("{");
			    }
			    else
			    {
				    // Already in font
				    if (line == "}")
				    {
					    // Finished 
					    pFont.setNull();
                        // NB font isn't loaded until required
				    }
                    else
                    {
                        parseAttribute(line, pFont);
                    }
                }
            }
        }
    }
    //---------------------------------------------------------------------
    void FontManager::parseAttribute(const String& line, FontPtr& pFont)
    {
        std::vector<String> params = StringUtil::split(line);
        String& attrib = params[0];
		StringUtil::toLowerCase(attrib);
        if (attrib == "type")
        {
            // Check params
            if (params.size() != 2)
            {
                logBadAttrib(line, pFont);
                return;
            }
            // Set
			StringUtil::toLowerCase(params[1]);
            if (params[1] == "truetype")
            {
                pFont->setType(FT_TRUETYPE);
            }
            else
            {
                pFont->setType(FT_IMAGE);
            }

        }
        else if (attrib == "source")
        {
            // Check params
            if (params.size() != 2)
            {
                logBadAttrib(line, pFont);
                return;
            }
            // Set
            pFont->setSource(params[1]);
        }
        else if (attrib == "glyph")
        {
            // Check params
            if (params.size() != 6)
            {
                logBadAttrib(line, pFont);
                return;
            }
            // Set
            pFont->setGlyphTexCoords(
                params[1].at(0), 
                StringConverter::parseReal(params[2]),
                StringConverter::parseReal(params[3]),
                StringConverter::parseReal(params[4]),
                StringConverter::parseReal(params[5]) );
        }
        else if (attrib == "size")
        {
            // Check params
            if (params.size() != 2)
            {
                logBadAttrib(line, pFont);
                return;
            }
            // Set
            pFont->setTrueTypeSize(
                StringConverter::parseReal(params[1]) );
        }
        else if (attrib == "resolution")
        {
            // Check params
            if (params.size() != 2)
            {
                logBadAttrib(line, pFont);
                return;
            }
            // Set
            pFont->setTrueTypeResolution(
                (uint)StringConverter::parseReal(params[1]) );
        }
        else if (attrib == "antialias_colour")
        {
        	// Check params
        	if (params.size() != 2)
        	{
                logBadAttrib(line, pFont);
                return;
        	}
        	// Set
            pFont->setAntialiasColour(StringConverter::parseBool(params[1]));
        }



    }
    //---------------------------------------------------------------------
    void FontManager::logBadAttrib(const String& line, FontPtr& pFont)
    {
        LogManager::getSingleton().logMessage("Bad attribute line: " + line +
            " in font " + pFont->getName());

    }

}

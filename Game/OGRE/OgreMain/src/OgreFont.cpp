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

#include "OgreFont.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTexture.h"
#include "OgreResourceGroupManager.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreRenderWindow.h"
#include "OgreException.h"
#include "OgreBlendMode.h"
#include "OgreTextureUnitState.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreMaterial.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H



namespace Ogre
{
    //---------------------------------------------------------------------
	Font::CmdType Font::msTypeCmd;
	Font::CmdSource Font::msSourceCmd;
	Font::CmdSize Font::msSizeCmd;
	Font::CmdResolution Font::msResolutionCmd;

    //---------------------------------------------------------------------
	Font::Font(ResourceManager* creator, const String& name, ResourceHandle handle,
		const String& group, bool isManual, ManualResourceLoader* loader)
		:Resource (creator, name, handle, group, isManual, loader),
		mType(FT_TRUETYPE), mTtfSize(0), mTtfResolution(0), mAntialiasColour(false)
    {

		if (createParamDictionary("Font"))
		{
			ParamDictionary* dict = getParamDictionary();
			dict->addParameter(
				ParameterDef("type", "'truetype' or 'image' based font", PT_STRING),
				&msTypeCmd);
			dict->addParameter(
				ParameterDef("source", "Filename of the source of the font.", PT_STRING),
				&msSourceCmd);
			dict->addParameter(
				ParameterDef("size", "True type size", PT_REAL),
				&msSizeCmd);
			dict->addParameter(
				ParameterDef("resolution", "True type resolution", PT_UNSIGNED_INT),
				&msResolutionCmd);
		}

    }
    //---------------------------------------------------------------------
    Font::~Font()
    {
        // have to call this here reather than in Resource destructor
        // since calling virtual methods in base destructors causes crash
        unload();
    }
    //---------------------------------------------------------------------
    void Font::setType(FontType ftype)
    {
        mType = ftype;
    }
    //---------------------------------------------------------------------
    FontType Font::getType(void) const
    {
        return mType;
    }
    //---------------------------------------------------------------------
    void Font::setSource(const String& source)
    {
        mSource = source;
    }
    //---------------------------------------------------------------------
    void Font::setTrueTypeSize(Real ttfSize)
    {
        mTtfSize = ttfSize;
    }
    //---------------------------------------------------------------------
    void Font::setTrueTypeResolution(uint ttfResolution)
    {
        mTtfResolution = ttfResolution;
    }
    //---------------------------------------------------------------------
    const String& Font::getSource(void) const
    {
        return mSource;
    }
    //---------------------------------------------------------------------
    Real Font::getTrueTypeSize(void) const
    {
        return mTtfSize;
    }
    //---------------------------------------------------------------------
    uint Font::getTrueTypeResolution(void) const
    {
        return mTtfResolution;
    }
    //---------------------------------------------------------------------
    std::pair< uint, uint > Font::StrBBox( const String & text, Real char_height, RenderWindow & window )
    {
        std::pair< uint, uint > ret( 0, 0 );
        Real vsX, vsY;
        unsigned int w, h;

        // These are not used, but are required byt the function calls.
        unsigned int cdepth;
		int left, top;

        window.getMetrics( w, h, cdepth, left, top );

        for( uint i = 0; i < text.length(); i++ )
        {
            // Calculate view-space width and height of char
            vsY = char_height;
			if (text[i] == ' ') // assume capital A is space width
				vsX = getGlyphAspectRatio( 'A' ) * char_height;
			else
	            vsX = getGlyphAspectRatio( text[ i ] ) * char_height;

            ret.second += vsX * static_cast<float>(w);
            if( vsY * h > ret.first || ( i && text[ i - 1 ] == '\n' ) )
                ret.first += vsY * static_cast<float>(h);
        }

        return ret;
    }
    //---------------------------------------------------------------------
    void Font::loadImpl()
    {
        // Create a new material
        mpMaterial =  MaterialManager::getSingleton().create(
			"Fonts/" + mName,  mGroup);

		if (mpMaterial.isNull())
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                "Error creating new material!", "Font::load" );
        }

        TextureUnitState *texLayer;
        bool blendByAlpha = true;
        if (mType == FT_TRUETYPE)
        {
            createTextureFromFont();
            texLayer = mpMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);
            // Always blend by alpha
            blendByAlpha = true;
        }
        else
        {
			// Manually load since we need to load to get alpha
			mTexture = TextureManager::getSingleton().load(mSource, mGroup, TEX_TYPE_2D, 0);
            blendByAlpha = mTexture->hasAlpha();
            texLayer = mpMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(mSource);
        }
        // Clamp to avoid fuzzy edges
        texLayer->setTextureAddressingMode( TextureUnitState::TAM_CLAMP );
		// Allow min/mag filter, but no mip
		texLayer->setTextureFiltering(FO_LINEAR, FO_LINEAR, FO_NONE);


        // Set up blending
        if (blendByAlpha)
        {
            mpMaterial->setSceneBlending( SBT_TRANSPARENT_ALPHA );
        }
        else
        {
            // Use add if no alpha (assume black background)
            mpMaterial->setSceneBlending(SBT_ADD);
        }
    }
    //---------------------------------------------------------------------
    void Font::unloadImpl()
    {
		// Cascade to the texture we created
        mTexture->unload();
    }
    //---------------------------------------------------------------------
    void Font::createTextureFromFont(void)
    {

		// Just create the texture here, and point it at ourselves for when
		// it wants to (re)load for real
		String texName = mName + "Texture";
		// Create, setting isManual to true and passing self as loader
		mTexture = TextureManager::getSingleton().create(
			texName, mGroup, true, this);
		mTexture->setTextureType(TEX_TYPE_2D);
		mTexture->setNumMipmaps(0);
		mTexture->load();

		TextureUnitState* t = mpMaterial->getTechnique(0)->getPass(0)->createTextureUnitState( texName );
		// Allow min/mag filter, but no mip
		t->setTextureFiltering(FO_LINEAR, FO_LINEAR, FO_NONE);

	}
	//---------------------------------------------------------------------
	void Font::loadResource(Resource* res)
	{
		// ManualResourceLoader implementation - load the texture
		FT_Library ftLibrary;
		// Init freetype
        if( FT_Init_FreeType( &ftLibrary ) )
            OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Could not init FreeType library!",
            "Font::Font");

		uint i, l, m, n;
        int j, k;

        FT_Face face;
        // Add a gap between letters vert and horz
        // prevents nasty artefacts when letters are too close together
        uint char_spacer = 5;

        // Locate ttf file, load it pre-buffered into memory by wrapping the
		// original DataStream in a MemoryDataStream
		DataStreamPtr dataStreamPtr =
			ResourceGroupManager::getSingleton().openResource(
				mSource, mGroup, true, this);
		MemoryDataStream ttfchunk(dataStreamPtr);

        // Load font
        if( FT_New_Memory_Face( ftLibrary, ttfchunk.getPtr(), (FT_Long)ttfchunk.size() , 0, &face ) )
            OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR,
            "Could not open font face!", "Font::createTextureFromFont" );


        // Convert our point size to freetype 26.6 fixed point format
        FT_F26Dot6 ftSize = (FT_F26Dot6)(mTtfSize * (1 << 6));
        if( FT_Set_Char_Size( face, ftSize, 0, mTtfResolution, mTtfResolution ) )
            OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR,
            "Could not set char size!", "Font::createTextureFromFont" );

        //FILE *fo_def = stdout;

        int max_height = 0, max_width = 0, max_bear = 0;

        uint startGlyph = 33;
        uint endGlyph = 167;

        // Calculate maximum width, height and bearing
        for( i = startGlyph, l = 0, m = 0, n = 0; i < endGlyph; i++ )
        {
            FT_Load_Char( face, i, FT_LOAD_RENDER );

            if( ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY ) > max_height )
                max_height = ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY );
            if( face->glyph->metrics.horiBearingY > max_bear )
                max_bear = face->glyph->metrics.horiBearingY;

            if( (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 ) > max_width)
                max_width = (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 );
        }

		// Now work out how big our texture needs to be
		size_t rawSize = (max_width + char_spacer) *
							((max_height >> 6) + char_spacer) *
							(endGlyph - startGlyph + 1);

		size_t tex_side = static_cast<size_t>(Math::Sqrt(rawSize));
		// just in case the size might chop a glyph in half, add another glyph width/height
		tex_side += std::max(max_width, (max_height>>6));
		// Now round up to nearest power of two, max out at 4096
		size_t roundUpSize = 0;
		for (i = 0; i < 12 && roundUpSize < tex_side; ++i)
            #if  OGRE_COMPILER == OGRE_COMPILER_MSVC &&  OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_64
                    roundUpSize = 1i64 << i;//  64-bit shift otherwise we get a C4334 warning
            #else
                    roundUpSize = 1 << i;
            #endif

		tex_side = roundUpSize;
		const size_t pixel_bytes = 2;
		size_t data_width = tex_side * pixel_bytes;

		LogManager::getSingleton().logMessage("Font " + mName + "using texture size " +
			StringConverter::toString(tex_side) + "x" + StringConverter::toString(tex_side));

        uchar* imageData = new uchar[tex_side * tex_side * pixel_bytes];
		// Reset content (White, transparent)
        for (i = 0; i < tex_side * tex_side * pixel_bytes; i += pixel_bytes)
        {
            imageData[i + 0] = 0xFF; // luminance
            imageData[i + 1] = 0x00; // alpha
        }

        for( i = startGlyph, l = 0, m = 0, n = 0; i < endGlyph; i++ )
        {
            FT_Error ftResult;

            // Load & render glyph
            ftResult = FT_Load_Char( face, i, FT_LOAD_RENDER );
            if (ftResult)
            {
                // problem loading this glyph, continue
                LogManager::getSingleton().logMessage("Info: cannot load character " +
                    StringConverter::toString(i) + " in font " + mName);
                continue;
            }

			FT_Int advance = (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 );

            unsigned char* buffer = face->glyph->bitmap.buffer;

            if (!buffer)
            {
                // Yuck, FT didn't detect this but generated a null pointer!
                LogManager::getSingleton().logMessage("Info: Freetype returned null for character " +
                    StringConverter::toString(i) + " in font " + mName);
                continue;
            }

            int y_bearnig = ( max_bear >> 6 ) - ( face->glyph->metrics.horiBearingY >> 6 );

            for( j = 0; j < face->glyph->bitmap.rows; j++ )
            {
                int row = j + m + y_bearnig;
                uchar* pDest = &imageData[(row * data_width) + l * pixel_bytes];
                for( k = 0; k < face->glyph->bitmap.width; k++ )
                {
                    if (mAntialiasColour)
                    {
                        // Use the same greyscale pixel for all components RGBA
                        *pDest++= *buffer;
                    }
                    else
                    {
                        // Always white whether 'on' or 'off' pixel, since alpha
                        // will turn off
                        *pDest++= 0xFF;
                    }
                    // Always use the greyscale value for alpha
                    *pDest++= *buffer++;                 }
            }

            this->setGlyphTexCoords( i,
                (Real)l / (Real)tex_side,  // u1
                (Real)m / (Real)tex_side,  // v1
                (Real)( l + ( face->glyph->advance.x >> 6 ) ) / (Real)tex_side, // u2
                ( m + ( max_height >> 6 ) ) / (Real)tex_side // v2
                );

            // Advance a column
            l += (advance + char_spacer);

            // If at end of row
            if( tex_side - 1 < l + ( advance ) )
            {
                m += ( max_height >> 6 ) + char_spacer;
                l = n = 0;
            }
        }

        DataStreamPtr memStream(
			new MemoryDataStream(imageData, tex_side * tex_side * pixel_bytes, true));

        Image img;
		img.loadRawData( memStream, tex_side, tex_side, PF_BYTE_LA );

		Texture* tex = static_cast<Texture*>(res);
		tex->loadImage(img);


		FT_Done_FreeType(ftLibrary);
    }
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	String Font::CmdType::doGet(const void* target) const
	{
		const Font* f = static_cast<const Font*>(target);
		if (f->getType() == FT_TRUETYPE)
		{
			return "truetype";
		}
		else
		{
			return "image";
		}
	}
	void Font::CmdType::doSet(void* target, const String& val)
	{
		Font* f = static_cast<Font*>(target);
		if (val == "truetype")
		{
			f->setType(FT_TRUETYPE);
		}
		else
		{
			f->setType(FT_IMAGE);
		}
	}
	//-----------------------------------------------------------------------
	String Font::CmdSource::doGet(const void* target) const
	{
		const Font* f = static_cast<const Font*>(target);
		return f->getSource();
	}
	void Font::CmdSource::doSet(void* target, const String& val)
	{
		Font* f = static_cast<Font*>(target);
		f->setSource(val);
	}
	//-----------------------------------------------------------------------
	String Font::CmdSize::doGet(const void* target) const
	{
		const Font* f = static_cast<const Font*>(target);
		return StringConverter::toString(f->getTrueTypeSize());
	}
	void Font::CmdSize::doSet(void* target, const String& val)
	{
		Font* f = static_cast<Font*>(target);
		f->setTrueTypeSize(StringConverter::parseReal(val));
	}
	//-----------------------------------------------------------------------
	String Font::CmdResolution::doGet(const void* target) const
	{
		const Font* f = static_cast<const Font*>(target);
		return StringConverter::toString(f->getTrueTypeResolution());
	}
	void Font::CmdResolution::doSet(void* target, const String& val)
	{
		Font* f = static_cast<Font*>(target);
		f->setTrueTypeResolution(StringConverter::parseUnsignedInt(val));
	}


}

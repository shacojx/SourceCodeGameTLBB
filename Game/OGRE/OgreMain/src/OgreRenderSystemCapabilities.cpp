/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"

#include "OgreRenderSystemCapabilities.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

namespace Ogre {

    //-----------------------------------------------------------------------
    RenderSystemCapabilities::RenderSystemCapabilities()
      : mNumWorldMatrices(0), mNumTextureUnits(0), mStencilBufferBitDepth(0),
        mNumVertexBlendMatrices(0), mCapabilities(0), mNumMultiRenderTargets(1), 
		mNonPOW2TexturesLimited(false)
    {
    }
    //-----------------------------------------------------------------------
    RenderSystemCapabilities::~RenderSystemCapabilities()
    {
    }
    //-----------------------------------------------------------------------
    void RenderSystemCapabilities::log(Log* pLog)
    {
        pLog->logMessage("RenderSystem capabilities");
        pLog->logMessage("-------------------------");
        pLog->logMessage(
            " * Hardware generation of mipmaps: "
            + StringConverter::toString(hasCapability(RSC_AUTOMIPMAP), true));
        pLog->logMessage(
            " * Texture blending: "
            + StringConverter::toString(hasCapability(RSC_BLENDING), true));
        pLog->logMessage(
            " * Anisotropic texture filtering: "
            + StringConverter::toString(hasCapability(RSC_ANISOTROPY), true));
        pLog->logMessage(
            " * Dot product texture operation: "
            + StringConverter::toString(hasCapability(RSC_DOT3), true));
        pLog->logMessage(
            " * Cube mapping: "
            + StringConverter::toString(hasCapability(RSC_CUBEMAPPING), true));
        pLog->logMessage(
            " * Hardware stencil buffer: "
            + StringConverter::toString(hasCapability(RSC_HWSTENCIL), true));
        if (hasCapability(RSC_HWSTENCIL))
        {
            pLog->logMessage(
                "   - Stencil depth: "
                + StringConverter::toString(getStencilBufferBitDepth()));
            pLog->logMessage(
                "   - Two sided stencil support: "
                + StringConverter::toString(hasCapability(RSC_TWO_SIDED_STENCIL), true));
            pLog->logMessage(
                "   - Wrap stencil values: "
                + StringConverter::toString(hasCapability(RSC_STENCIL_WRAP), true));
        }
        pLog->logMessage(
            " * Hardware vertex / index buffers: "
            + StringConverter::toString(hasCapability(RSC_VBO), true));
        pLog->logMessage(
            " * Vertex programs: "
            + StringConverter::toString(hasCapability(RSC_VERTEX_PROGRAM), true));
        if (hasCapability(RSC_VERTEX_PROGRAM))
        {
            pLog->logMessage(
                "   - Max vertex program version: "
                + getMaxVertexProgramVersion());
        }
        pLog->logMessage(
            " * Fragment programs: "
            + StringConverter::toString(hasCapability(RSC_FRAGMENT_PROGRAM), true));
        if (hasCapability(RSC_FRAGMENT_PROGRAM))
        {
            pLog->logMessage(
                "   - Max fragment program version: "
                + getMaxFragmentProgramVersion());
        }

        pLog->logMessage(
            " * Texture Compression: "
            + StringConverter::toString(hasCapability(RSC_TEXTURE_COMPRESSION), true));
        if (hasCapability(RSC_TEXTURE_COMPRESSION))
        {
            pLog->logMessage(
                "   - DXT: "
                + StringConverter::toString(hasCapability(RSC_TEXTURE_COMPRESSION_DXT), true));
            pLog->logMessage(
                "   - VTC: "
                + StringConverter::toString(hasCapability(RSC_TEXTURE_COMPRESSION_VTC), true));
        }

        pLog->logMessage(
            " * Scissor Rectangle: "
            + StringConverter::toString(hasCapability(RSC_SCISSOR_TEST), true));
        pLog->logMessage(
            " * Hardware Occlusion Query: "
            + StringConverter::toString(hasCapability(RSC_HWOCCLUSION), true));
        pLog->logMessage(
            " * User clip planes: "
            + StringConverter::toString(hasCapability(RSC_USER_CLIP_PLANES), true));
        pLog->logMessage(
            " * VET_UBYTE4 vertex element type: "
            + StringConverter::toString(hasCapability(RSC_VERTEX_FORMAT_UBYTE4), true));
        pLog->logMessage(
            " * Infinite far plane projection: "
            + StringConverter::toString(hasCapability(RSC_INFINITE_FAR_PLANE), true));
		pLog->logMessage(
            " * Hardware render-to-texture: "
            + StringConverter::toString(hasCapability(RSC_HWRENDER_TO_TEXTURE), true));
        pLog->logMessage(
            " * Floating point textures: "
            + StringConverter::toString(hasCapability(RSC_TEXTURE_FLOAT), true));
        pLog->logMessage(
            " * Non-power-of-two textures: "
            + StringConverter::toString(hasCapability(RSC_NON_POWER_OF_2_TEXTURES), true)
			+ (mNonPOW2TexturesLimited ? " (limited)" : ""));
		pLog->logMessage(
            " * Volume textures: "
            + StringConverter::toString(hasCapability(RSC_TEXTURE_3D), true));
		pLog->logMessage(
            " * Multiple Render Targets: "
            + StringConverter::toString(mNumMultiRenderTargets));
		pLog->logMessage(
			" * Point Sprites: "
			+ StringConverter::toString(hasCapability(RSC_POINT_SPRITES), true));
		pLog->logMessage(
			" * Extended point parameters: "
			+ StringConverter::toString(hasCapability(RSC_POINT_EXTENDED_PARAMETERS), true));
		pLog->logMessage(
			" * Max Point Size: "
			+ StringConverter::toString(mMaxPointSize));

    }
};

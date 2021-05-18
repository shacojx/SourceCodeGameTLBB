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
#include "OgreD3D9MultiRenderTarget.h"
#include "OgreD3D9HardwarePixelBuffer.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreBitwise.h"
#include "OgreD3D9RenderSystem.h"
#include "OgreRoot.h"

namespace Ogre 
{
	D3D9MultiRenderTarget::D3D9MultiRenderTarget(const String &name):
		MultiRenderTarget(name)
	{
		/// Clear targets
		for(size_t x=0; x<OGRE_MAX_MULTIPLE_RENDER_TARGETS; ++x)
		{
			targets[x] = 0;
		}
	}
	D3D9MultiRenderTarget::~D3D9MultiRenderTarget()
	{
	}

	void D3D9MultiRenderTarget::bindSurface(size_t attachment, RenderTexture *target)
	{
		assert(attachment<OGRE_MAX_MULTIPLE_RENDER_TARGETS);
		/// Get buffer and surface to bind to
		D3D9HardwarePixelBuffer *buffer = 0;
		target->getCustomAttribute("BUFFER", &buffer);
		assert(buffer);

		/// Find first non-null target
		int y;
		for(y=0; y<OGRE_MAX_MULTIPLE_RENDER_TARGETS && !targets[y]; ++y) ;
		
		if(y!=OGRE_MAX_MULTIPLE_RENDER_TARGETS)
		{
			/// If there is another target bound, compare sizes
			if(targets[y]->getWidth() != buffer->getWidth() ||
				targets[y]->getHeight() != buffer->getHeight() ||
				targets[y]->getFormat() != buffer->getFormat())
			{
				OGRE_EXCEPT(
					Exception::ERR_INVALIDPARAMS, 
					"MultiRenderTarget surfaces are not of same size or pixel format", 
					"D3D9MultiRenderTarget::bindSurface"
				);
			}
		}

		targets[attachment] = buffer;
		checkAndUpdate();
	}

	void D3D9MultiRenderTarget::unbindSurface(size_t attachment)
	{
		assert(attachment<OGRE_MAX_MULTIPLE_RENDER_TARGETS);
		targets[attachment] = 0;
		checkAndUpdate();
	}

    void D3D9MultiRenderTarget::update(void)
    {
        D3D9RenderSystem* rs = static_cast<D3D9RenderSystem*>(
            Root::getSingleton().getRenderSystem());
        if (rs->isDeviceLost())
            return;

        MultiRenderTarget::update();
    }

	void D3D9MultiRenderTarget::getCustomAttribute(const String& name, void *pData)
    {
		if(name == "DDBACKBUFFER")
        {
            IDirect3DSurface9 ** pSurf = (IDirect3DSurface9 **)pData;
			/// Transfer surfaces
			for(size_t x=0; x<OGRE_MAX_MULTIPLE_RENDER_TARGETS; ++x)
			{
				if(targets[x])
					pSurf[x] = targets[x]->getSurface();
			}
			return;
        }
	}

	void D3D9MultiRenderTarget::checkAndUpdate()
	{
		if(targets[0])
		{
			mWidth = targets[0]->getWidth();
			mHeight = targets[0]->getHeight();
		}
		else
		{
			mWidth = 0;
			mHeight = 0;
		}
	}


}


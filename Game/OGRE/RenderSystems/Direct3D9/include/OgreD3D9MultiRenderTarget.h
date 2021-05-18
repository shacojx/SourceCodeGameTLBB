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
#ifndef __D3D9MULTIRENDERTARGET_H__
#define __D3D9MULTIRENDERTARGET_H__

#include "OgreD3D9Prerequisites.h"
#include "OgreTexture.h"
#include "OgreRenderTexture.h"
#include "OgreImage.h"
#include "OgreException.h"
#include "OgreD3D9HardwarePixelBuffer.h"

#include "OgreNoMemoryMacros.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include "OgreMemoryMacros.h"

namespace Ogre {
	class D3D9MultiRenderTarget : public MultiRenderTarget
	{
	public:
		D3D9MultiRenderTarget(const String &name);
		~D3D9MultiRenderTarget();

		/** @copydoc MultiRenderTarget::bindSurface */
		virtual void bindSurface(size_t attachment, RenderTexture *target);

		/** @copydoc MultiRenderTarget::unbindSurface */
		virtual void unbindSurface(size_t attachment);

        virtual void update(void);

		virtual void getCustomAttribute( const String& name, void *pData );

		bool requiresTextureFlipping() const { return false; }
	private:
		D3D9HardwarePixelBuffer *targets[OGRE_MAX_MULTIPLE_RENDER_TARGETS];

		/** Check surfaces and update RenderTarget extent */
		void checkAndUpdate();
	};
};

#endif

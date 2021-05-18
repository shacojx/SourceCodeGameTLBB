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

#ifndef __GLRENDERTEXTURE_H__
#define __GLRENDERTEXTURE_H__

#include "OgreGLTexture.h"

namespace Ogre {
    /** GL surface descriptor. Points to a 2D surface that can be rendered to. 
    */
    struct GLSurfaceDesc
    {
    public:
        GLHardwarePixelBuffer *buffer;
        size_t zoffset;
    };
    
    /** Base class for GL Render Textures
    */
    class GLRenderTexture: public RenderTexture
    {
    public:
        GLRenderTexture(const String &name, const GLSurfaceDesc &target);
        virtual ~GLRenderTexture();
        
        bool requiresTextureFlipping() const { return true; }
    };
    
    /** Manager/factory for RenderTextures.
    */
    class GLRTTManager: public Singleton<GLRTTManager>
    {
    public:
        virtual ~GLRTTManager();
        
        /** Create a texture rendertarget object
        */
        virtual RenderTexture *createRenderTexture(const String &name, const GLSurfaceDesc &target) = 0;
        
         /** Check if a certain format is usable as rendertexture format
        */
        virtual bool checkFormat(PixelFormat format) = 0;
        
        /** Bind a certain render target.
        */
        virtual void bind(RenderTarget *target) = 0;
        
        /** Unbind a certain render target. This is called before binding another RenderTarget, and
            before the context is switched. It can be used to do a copy, or just be a noop if direct
            binding is used.
        */
        virtual void unbind(RenderTarget *target) = 0;

		/** Create a multi render target 
		*/
		virtual MultiRenderTarget* createMultiRenderTarget(const String & name);
        
        /** Get the closest supported alternative format. If format is supported, returns format.
        */
        virtual PixelFormat getSupportedAlternative(PixelFormat format);
    };
    
    /** RenderTexture for simple copying from frame buffer
    */
    class GLCopyingRTTManager;
    class GLCopyingRenderTexture: public GLRenderTexture
    {
    public:
        GLCopyingRenderTexture(GLCopyingRTTManager *manager, const String &name, const GLSurfaceDesc &target);
        
        virtual void getCustomAttribute(const String& name, void* pData);
    };
    
    /** Simple, copying manager/factory for RenderTextures. This is only used as the last fallback if
        both PBuffers and FBOs aren't supported.
    */
    class GLCopyingRTTManager: public GLRTTManager
    {
    public:
        GLCopyingRTTManager();
        virtual ~GLCopyingRTTManager();
        
        /** @copydoc GLRTTManager::createRenderTexture
        */
        virtual RenderTexture *createRenderTexture(const String &name, const GLSurfaceDesc &target);
        
         /** @copydoc GLRTTManager::checkFormat
        */
        virtual bool checkFormat(PixelFormat format);
        
        /** @copydoc GLRTTManager::bind
        */
        virtual void bind(RenderTarget *target);
        
        /** @copydoc GLRTTManager::unbind
        */
        virtual void unbind(RenderTarget *target);
    };
}

#endif // __GLTEXTURE_H__

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

#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreRoot.h"
#include "OgreStringConverter.h"

#include "OgreGLRenderSystem.h"

#include "OgreGLXRenderTexture.h"
#include "OgreGLXContext.h"
#include "OgreGLXUtils.h"

#include <iostream>

/// ATI: GLX_ATI_pixel_format_float
#ifndef GLX_RGBA_FLOAT_ATI_BIT
#define GLX_RGBA_FLOAT_ATI_BIT 0x00000100
#endif

/// ARB: GLX_ARB_fbconfig_float
#ifndef GLX_RGBA_FLOAT_BIT
#define GLX_RGBA_FLOAT_BIT 0x00000004
#endif

#ifndef GLX_RGBA_FLOAT_TYPE
#define GLX_RGBA_FLOAT_TYPE 0x20B9
#endif


namespace Ogre
{
    GLXPBuffer::GLXPBuffer(PixelComponentType format, size_t width, size_t height):
        GLPBuffer(format, width, height),
        _hPBuffer(0),
        mContext(0)
    {
        createPBuffer();
        // Create context
        mContext = new GLXContext(_pDpy, _hPBuffer, _hGLContext);
    }
        
    GLContext *GLXPBuffer::getContext()
    {
        return mContext;
    }

    void GLXPBuffer::createPBuffer() {        
        //LogManager::getSingleton().logMessage(
        //"GLXPBuffer::Creating PBuffer"
        //); 
        _pDpy = glXGetCurrentDisplay();
        ::GLXContext context = glXGetCurrentContext();
        int screen = DefaultScreen(_pDpy);
        int attribs[50], ideal[50];
        int attrib;
        
        // Process format
        int bits=0;
        bool isFloat=false;
        switch(mFormat)
        {
            case PCT_BYTE:
                bits=8; isFloat=false;
                break;
            case PCT_SHORT:
                bits=16; isFloat=false;
                break;
            case PCT_FLOAT16:
                bits=16; isFloat=true;
                break;
            case PCT_FLOAT32:
                bits=32; isFloat=true;
                break;
            default: break;
        };
        RTFType floatBuffer = RTF_NONE;
        if(isFloat)
        {
            floatBuffer = detectRTFType();
            if(floatBuffer == RTF_NONE || floatBuffer == RTF_NV)
            {
                OGRE_EXCEPT(Exception::UNIMPLEMENTED_FEATURE, "Floating point PBuffers not supported on this hardware",  "GLRenderTexture::createPBuffer");
            }
        }

        // Create base required format description
        attrib = 0;
        if (floatBuffer == RTF_ATI) {
            attribs[attrib++] = GLX_RENDER_TYPE;
            attribs[attrib++] = GLX_RGBA_FLOAT_ATI_BIT;
        } 
        else if (floatBuffer == RTF_ARB) 
        {
            attribs[attrib++] = GLX_RENDER_TYPE;
            attribs[attrib++] = GLX_RGBA_FLOAT_BIT;
        }
        else
        {
            attribs[attrib++] = GLX_RENDER_TYPE;
            attribs[attrib++] = GLX_RGBA_BIT;
        }     
        attribs[attrib++] = GLX_DRAWABLE_TYPE;
        attribs[attrib++] = GLX_PBUFFER_BIT;
        attribs[attrib++] = GLX_DOUBLEBUFFER;
        attribs[attrib++] = 0;
        /*
        if (floatBuffer == RTF_NV) {
		    attribs[attrib++] = GLX_FLOAT_COMPONENTS_NV;
		    attribs[attrib++] = 1;
	    }
        */
        attribs[attrib++] = None;
        
        // Create "ideal" format description
        attrib = 0;
        ideal[attrib++] = GLX_RED_SIZE;
        ideal[attrib++] = bits;        
        ideal[attrib++] = GLX_GREEN_SIZE;
        ideal[attrib++] = bits;
        ideal[attrib++] = GLX_BLUE_SIZE;
        ideal[attrib++] = bits;        
        ideal[attrib++] = GLX_ALPHA_SIZE;
        ideal[attrib++] = bits;
        ideal[attrib++] = GLX_DEPTH_SIZE;
        ideal[attrib++] = 24;
        ideal[attrib++] = GLX_STENCIL_SIZE;
        ideal[attrib++] = 8;
        ideal[attrib++] = GLX_ACCUM_RED_SIZE;
        ideal[attrib++] = 0;    // Accumulation buffer not used
        ideal[attrib++] = GLX_ACCUM_GREEN_SIZE;
        ideal[attrib++] = 0;    // Accumulation buffer not used
        ideal[attrib++] = GLX_ACCUM_BLUE_SIZE;
        ideal[attrib++] = 0;    // Accumulation buffer not used
        ideal[attrib++] = GLX_ACCUM_ALPHA_SIZE;
        ideal[attrib++] = 0;    // Accumulation buffer not used
        ideal[attrib++] = None;

        // Create vector of existing config data formats        
        GLXFBConfig config = GLXUtils::findBestMatch(_pDpy, screen, attribs, ideal);

        // Create the pbuffer in the best matching format
        attrib = 0;
        attribs[attrib++] = GLX_PBUFFER_WIDTH;
        attribs[attrib++] = mWidth; // Get from texture?
        attribs[attrib++] = GLX_PBUFFER_HEIGHT;
        attribs[attrib++] = mHeight; // Get from texture?
        attribs[attrib++] = GLX_PRESERVED_CONTENTS;
        attribs[attrib++] = 1;
        attribs[attrib++] = None;

        FBConfigData configData(_pDpy, config);
        LogManager::getSingleton().logMessage(
                LML_NORMAL,
                "GLXPBuffer::PBuffer chose format "+configData.toString());                   

        _hPBuffer = glXCreatePbuffer(_pDpy, config, attribs);
        if (!_hPBuffer) 
            OGRE_EXCEPT(Exception::UNIMPLEMENTED_FEATURE, "glXCreatePbuffer() failed", "GLRenderTexture::createPBuffer");

        _hGLContext = glXCreateNewContext(_pDpy, config, GLX_RGBA_TYPE, context, True);
        if (!_hGLContext) 
            OGRE_EXCEPT(Exception::UNIMPLEMENTED_FEATURE, "glXCreateContext() failed", "GLRenderTexture::createPBuffer");        

        // Query real width and height
        GLuint iWidth, iHeight;
        glXQueryDrawable(_pDpy, _hPBuffer, GLX_WIDTH, &iWidth);
        glXQueryDrawable(_pDpy, _hPBuffer, GLX_HEIGHT, &iHeight);

        LogManager::getSingleton().logMessage(
             LML_NORMAL,
                "GLXPBuffer::PBuffer created -- Real dimensions "+
                StringConverter::toString(iWidth)+"x"+StringConverter::toString(iHeight)+
                ", number of bits is "+
                StringConverter::toString(bits)+
                ", floating point is "+
                StringConverter::toString(isFloat)
        );
        mWidth = iWidth;  
        mHeight = iHeight;
    }

    GLXPBuffer::~GLXPBuffer()
    {
        // Destroy and unregister context
        delete mContext;
        // Destroy GL context
        glXDestroyContext(_pDpy, _hGLContext);
        _hGLContext = 0;
        glXDestroyPbuffer(_pDpy, _hPBuffer);
        _hPBuffer = 0;
        LogManager::getSingleton().logMessage(
             LML_NORMAL,
                "GLXPBuffer::PBuffer destroyed");
    }
    
    GLXPBuffer::RTFType GLXPBuffer::detectRTFType()
    {
        RTFType floatBuffer = RTF_NONE;
        /// Query supported float buffer extensions
        /// Choose the best one
        std::stringstream ext;
        std::string instr;
        ext << glXQueryExtensionsString(_pDpy, DefaultScreen(_pDpy)) << " " << glXGetClientString(_pDpy, GLX_EXTENSIONS);
        while(ext >> instr)
        {
            if(instr == "GLX_NV_float_buffer" && floatBuffer<RTF_NV)
                floatBuffer = RTF_NV;
            if(instr == "GLX_ATI_pixel_format_float" && floatBuffer<RTF_ATI)
                floatBuffer = RTF_ATI;
            if(instr == "GLX_ARB_fbconfig_float" && floatBuffer<RTF_ARB)
                floatBuffer = RTF_ARB;
        }
        return floatBuffer;
    }
  
}

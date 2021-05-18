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

#include "OgreSDLWindow.h"
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreImageCodec.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   include <windows.h>
#   include <wingdi.h>
#   include <GL/gl.h>
#   define GL_GLEXT_PROTOTYPES
#   include "glprocs.h"
#   include <GL/glu.h>
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#   include <GL/gl.h>
#   include <GL/glu.h>
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#   include <OpenGL/gl.h>
#   define GL_EXT_texture_env_combine 1
#   include <OpenGL/glext.h>
#   include <OpenGL/glu.h>
#endif

namespace Ogre {

    SDLWindow::SDLWindow() :
        mScreen(NULL), mActive(false), mClosed(false)
    {
    }

    SDLWindow::~SDLWindow()
    {
        // according to http://www.libsdl.org/cgi/docwiki.cgi/SDL_5fSetVideoMode
        // never free the surface returned from SDL_SetVideoMode
        /*if (mScreen != NULL)
            SDL_FreeSurface(mScreen);*/

    }

	void SDLWindow::create(const String& name, unsigned int width, unsigned int height,
	            bool fullScreen, const NameValuePairList *miscParams)
    {
		int colourDepth = 32;
		String title = name;
		if(miscParams)
		{
			// Parse miscellenous parameters
			NameValuePairList::const_iterator opt;
			// Bit depth
			opt = miscParams->find("colourDepth");
			if(opt != miscParams->end()) //check for FSAA parameter, if not ignore it...
				colourDepth = StringConverter::parseUnsignedInt(opt->second);
			// Full screen antialiasing
			opt = miscParams->find("FSAA");
			if(opt != miscParams->end()) //check for FSAA parameter, if not ignore it...
			{
				size_t fsaa_x_samples = StringConverter::parseUnsignedInt(opt->second);
				if(fsaa_x_samples>1) {
					// If FSAA is enabled in the parameters, enable the MULTISAMPLEBUFFERS
					// and set the number of samples before the render window is created.
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
					SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,fsaa_x_samples);
				}
			}
			// Window title
			opt = miscParams->find("title");
			if(opt != miscParams->end()) //check for FSAA parameter, if not ignore it...
				title = opt->second;
		}   
	
        LogManager::getSingleton().logMessage("SDLWindow::create", LML_TRIVIAL);
        SDL_Surface* screen;
        int flags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE;
		
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
        // request good stencil size if 32-bit colour
        if (colourDepth == 32)
        {
            SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8);
        }
		
        if (fullScreen)
            flags |= SDL_FULLSCREEN;

        LogManager::getSingleton().logMessage("Create window", LML_TRIVIAL);
        screen = SDL_SetVideoMode(width, height, colourDepth, flags);
        if (!screen)
        {
            LogManager::getSingleton().logMessage(LML_CRITICAL, 
                String("Could not make screen: ") + SDL_GetError());
            exit(1);
        }
        LogManager::getSingleton().logMessage("screen is valid", LML_TRIVIAL);
        mScreen = screen;

        mName = name;

        mWidth = width;
        mHeight = height;

        mActive = true;

        if (!fullScreen)
            SDL_WM_SetCaption(title.c_str(), 0);

        glXGetVideoSyncSGI = (int (*)(unsigned int *))SDL_GL_GetProcAddress("glXGetVideoSyncSGI");
        glXWaitVideoSyncSGI = (int (*)(int, int, unsigned int *))SDL_GL_GetProcAddress("glXWaitVideoSyncSGI");
    }

    void SDLWindow::destroy(void)
    {
        // according to http://www.libsdl.org/cgi/docwiki.cgi/SDL_5fSetVideoMode
        // never free the surface returned from SDL_SetVideoMode
        //SDL_FreeSurface(mScreen);
        mScreen = NULL;
        mActive = false;

        Root::getSingleton().getRenderSystem()->detachRenderTarget( this->getName() );
    }

    bool SDLWindow::isActive() const
    {
        return mActive;
    }

    bool SDLWindow::isClosed() const
    {
        return mClosed;
    }

    void SDLWindow::reposition(int left, int top)
    {
        // XXX FIXME
    }

    void SDLWindow::resize(unsigned int width, unsigned int height)
    {
        SDL_Surface* screen;
        int flags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE;
		
        LogManager::getSingleton().logMessage("Updating window", LML_TRIVIAL);
        screen = SDL_SetVideoMode(width, height, mScreen->format->BitsPerPixel, flags);
        if (!screen)
        {
            LogManager::getSingleton().logMessage(LML_CRITICAL, 
                String("Could not make screen: ") + SDL_GetError());
            exit(1);
        }
        LogManager::getSingleton().logMessage("screen is valid", LML_TRIVIAL);
        mScreen = screen;


        mWidth = width;
        mHeight = height;

        for (ViewportList::iterator it = mViewportList.begin();
             it != mViewportList.end(); ++it)
        {
            (*it).second->_updateDimensions();
        }
    }

    void SDLWindow::swapBuffers(bool waitForVSync)
    {
        if ( waitForVSync && glXGetVideoSyncSGI && glXWaitVideoSyncSGI )
        {
            unsigned int retraceCount;
            glXGetVideoSyncSGI( &retraceCount );
            glXWaitVideoSyncSGI( 2, ( retraceCount + 1 ) & 1, &retraceCount);
        }

        SDL_GL_SwapBuffers();
        // XXX More?
    }

	void SDLWindow::writeContentsToFile(const String& filename)
	{
		ImageCodec::ImageData* imgData = new ImageCodec::ImageData;
		imgData->width = mWidth;
		imgData->height = mHeight;
		imgData->format = PF_BYTE_RGB;

		// Allocate buffer 
		uchar* pBuffer = new uchar[mWidth * mHeight * 3];

		// Read pixels
		// I love GL: it does all the locking & colour conversion for us
		glReadPixels(0,0, mWidth-1, mHeight-1, GL_RGB, GL_UNSIGNED_BYTE, pBuffer);

		// Wrap buffer in a memory stream
        DataStreamPtr stream(new MemoryDataStream(pBuffer, mWidth * mHeight * 3, false));

		// Need to flip the read data over in Y though
		Image img;
		img.loadRawData(stream, mWidth, mHeight, imgData->format );
		img.flipAroundX();

        MemoryDataStreamPtr streamFlipped(new MemoryDataStream(img.getData(), stream->size(), false));

		// Get codec 
		size_t pos = filename.find_last_of(".");
		String extension;
		if( pos == String::npos )
			OGRE_EXCEPT(
			Exception::ERR_INVALIDPARAMS, 
			"Unable to determine image type for '" + filename + "' - invalid extension.",
			"SDLWindow::writeContentsToFile" );

		while( pos != filename.length() - 1 )
			extension += filename[++pos];

		// Get the codec
		Codec * pCodec = Codec::getCodec(extension);

		// Write out
		Codec::CodecDataPtr codecDataPtr(imgData);
		pCodec->codeToFile(streamFlipped, filename, codecDataPtr);

		delete [] pBuffer;


	}
}

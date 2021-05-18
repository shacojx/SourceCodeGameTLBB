/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 The OGRE Team
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

#include "OgreGLXWindow.h"
#include "OgreRoot.h"
#include "OgreGLRenderSystem.h"
#include "OgreImageCodec.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreGLXUtils.h"

#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include <climits>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#ifndef NO_XRANDR
#include <X11/extensions/Xrandr.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>


namespace Ogre
{

//-------------------------------------------------------------------------------------------------//
GLXWindow::GLXWindow(Display *display) :
	mDisplay(display), mWindow(0), mGlxContext(0), mClosed(false), mVisible(true), 
	mFullScreen(false), mOldMode(-1), mContext(0) 
{
	mActive = false;
}

//-------------------------------------------------------------------------------------------------//
GLXWindow::~GLXWindow() 
{
	if(mGlxContext)
		glXDestroyContext(mDisplay, mGlxContext);
	
	if(mWindow)
		XDestroyWindow(mDisplay, mWindow);

#ifndef NO_XRANDR
	if(mFullScreen) 
	{
		// Restore original video mode.
		Window rootWindow = DefaultRootWindow(mDisplay);
		XRRScreenConfiguration *config;

		// Get current screen info
		config = XRRGetScreenInfo(mDisplay, rootWindow);
		if(config) 
		{
			Rotation current_rotation;
			XRRConfigCurrentConfiguration (config, &current_rotation);
			//std::cerr << "Restore mode " << mOldMode << std::endl;
			LogManager::getSingleton().logMessage("GLXWindow::~GLXWindow -- Leaving full screen mode");
			XRRSetScreenConfig(mDisplay, config, rootWindow, mOldMode, current_rotation, CurrentTime);
			XRRFreeScreenConfigInfo(config);
		} 
		else 
		{
			LogManager::getSingleton().logMessage("GLXWindow::~GLXWindow -- Could not switch from full screen mode: XRRGetScreenInfo failed");
		}
	}
#endif
}

//-------------------------------------------------------------------------------------------------//
void GLXWindow::create(const String& name, unsigned int width, unsigned int height,
	            bool fullScreen, const NameValuePairList *miscParams)
{
	LogManager::getSingleton().logMessage("GLXWindow::create");

	String title = name;
	size_t fsaa_samples = 0;

	// We will attempt to create new window on default screen op display 0
	// unless external window handle passed below
	int screen = DefaultScreen(mDisplay);
	int depth = DisplayPlanes(mDisplay, screen);
	Window rootWindow = RootWindow(mDisplay,screen);
	Window parentWindow = rootWindow;

	// Make sure the window is centered if no left and top in parameters
	size_t left = (int)DisplayWidth(mDisplay, screen)/2 - width/2;
	size_t top = (int)DisplayHeight(mDisplay, screen)/2 - height/2;
	
	// Maybe user already created the window and passed its visualinfo in miscParams
	XVisualInfo *	extVisualHandler = NULL;

        // Unless parentWindowHandle is given in miscParams we're top-level
	mTopLevel = true;
   
	LogManager::getSingleton().logMessage("Parsing miscParams");
	if(miscParams)
	{
		// Parse miscellenous parameters
		NameValuePairList::const_iterator opt;
		// Full screen anti aliasing
		opt = miscParams->find("FSAA");
		if(opt != miscParams->end()) //check for FSAA parameter, if not ignore it...
			fsaa_samples = StringConverter::parseUnsignedInt(opt->second);
		// left (x)
		opt = miscParams->find("left");
		if(opt != miscParams->end())
			left = StringConverter::parseUnsignedInt(opt->second);
		// top (y)
		opt = miscParams->find("top");
		if(opt != miscParams->end())
			top = StringConverter::parseUnsignedInt(opt->second);
		// Window title
		opt = miscParams->find("title");
		if(opt != miscParams->end()) //check for FSAA parameter, if not ignore it...
			title = opt->second;
		opt = miscParams->find("parentWindowHandle");
		if(opt != miscParams->end()) {  // embedding OGRE
			std::vector<String> tokens = StringUtil::split(opt->second, " :");
			String new_display = tokens[0];
			String new_screen = tokens[1];
			String wid = tokens[2];

			// Now set things to their correct values
			// This must be the ugliest line of code I have ever written :P
			mDisplay = reinterpret_cast<Display*>(StringConverter::parseUnsignedLong(new_display));
			screen = StringConverter::parseUnsignedInt(new_screen);
			parentWindow = StringConverter::parseUnsignedLong(wid);

			depth = DisplayPlanes(mDisplay, screen);
			rootWindow = RootWindow(mDisplay, screen);

			left = top = 0;
			fullScreen = false; // Can't be full screen if embedded in an app!
			mTopLevel = false;  // Can't be top-level if embedded
		}
		
		opt = miscParams->find("externalWindowHandle");
		if(opt != miscParams->end()) // embedding OGRE in already created window
		{
			std::vector<String> tokens = StringUtil::split(opt->second, " :");
			String new_display = tokens[0];
			String new_screen = tokens[1];
			String wid = tokens[2];
			
			mDisplay = reinterpret_cast<Display*>(StringConverter::parseUnsignedLong(new_display)); 
			screen = StringConverter::parseUnsignedInt(new_screen); 
			mWindow = StringConverter::parseUnsignedLong(wid);
			
			if(tokens.size() > 3) // external visual was already setup
			{
				extVisualHandler = reinterpret_cast<XVisualInfo*>(StringConverter::parseUnsignedLong(tokens[3]));
			}
			
			depth = DisplayPlanes(mDisplay, screen);
			rootWindow = RootWindow(mDisplay, screen);
			
			left = top = 0;
			fullScreen = false; // Can't be full screen if embedded in an app!
			mTopLevel = false;  // Can't be top-level if embedded         
		}

	}

	// Check for full screen mode if FSAA was asked for
	if(!fullScreen && fsaa_samples>0)
	{
		LogManager::getSingleton().logMessage("GLXWindow::create -- FSAA only supported in fullscreen mode");
		fsaa_samples = 0;
	}
	// Disable FSAA for now -- it doesn't work on NVIDIA
	fsaa_samples = 0;

#ifndef NO_XRANDR
	// Attempt mode switch for fullscreen -- only if RANDR extension is there
	int dummy;
	if(fullScreen && ! XQueryExtension(mDisplay, "RANDR", &dummy, &dummy, &dummy)) 
	{
		LogManager::getSingleton().logMessage("GLXWindow::create -- Could not switch to full screen mode: No XRANDR extension found");
	} 
	else if(fullScreen) 
	{
		// Use Xrandr extension to switch video modes. This is much better than
		// XVidMode as you can't scroll away from the full-screen applications.
		XRRScreenConfiguration *config;
		XRRScreenSize *sizes;
		Rotation current_rotation;
		int nsizes;

		// Get current screen info
		config = XRRGetScreenInfo(mDisplay, rootWindow);
		// Get available sizes
		if(config)
			sizes = XRRConfigSizes (config, &nsizes);

		if(config && nsizes > 0) {
			// Get current size and rotation
			mOldMode = XRRConfigCurrentConfiguration (config, &current_rotation);
			// Find smallest matching mode
			int mode = -1;
			int mode_width = INT_MAX;
			int mode_height = INT_MAX;
			for(size_t i=0; i<nsizes; i++) {
				if(sizes[i].width >= width && sizes[i].height >= height &&
				                sizes[i].width < mode_width && sizes[i].height < mode_height) {
					mode = i;
					mode_width = sizes[i].width;
					mode_height = sizes[i].height;
				}
			}
			if(mode >= 0) {
				// Finally, set the screen configuration
				LogManager::getSingleton().logMessage("GLXWindow::create -- Entering full screen mode");
				XRRSetScreenConfig(mDisplay, config, rootWindow, mode, current_rotation, CurrentTime);
			} else {
				LogManager::getSingleton().logMessage("GLXWindow::create -- Could not switch to full screen mode: No conforming mode was found");
			}
			// Free configuration data
			XRRFreeScreenConfigInfo(config);
		} else {
			LogManager::getSingleton().logMessage("GLXWindow::create -- Could not switch to full screen mode: XRRGetScreenInfo failed");
		}
	}
#endif

	XVisualInfo* visualInfo = NULL;
	if(extVisualHandler == NULL) // user didn't create visual ( and window ) himself 
	{
		// Apply some magic algorithm to get the best visual
		int best_visual = GLXUtils::findBestVisual(mDisplay, screen, fsaa_samples);
		if(best_visual == -1)
		{
			best_visual = GLXUtils::findBestVisual(mDisplay, screen);
			LogManager::getSingleton().logMessage("GLXWindow::create -- Requested FSAA of "+
					StringConverter::toString(fsaa_samples)+" was not acquirable, defaulting to first suitable visual");
		}
		LogManager::getSingleton().logMessage("GLXWindow::create -- Best visual is "+StringConverter::toString(best_visual));

		// Get information about this so-called-best visual
		XVisualInfo templ;
		int nmatch;
		templ.visualid = best_visual;
		visualInfo = XGetVisualInfo(mDisplay, VisualIDMask, &templ, &nmatch);
		if(visualInfo==0 || nmatch==0) {
			OGRE_EXCEPT(999, "GLXWindow: error choosing visual", "GLXWindow::create");
		}

		XSetWindowAttributes attr;
		unsigned long mask;
		attr.background_pixel = 0;
		attr.border_pixel = 0;
		attr.colormap = XCreateColormap(mDisplay,rootWindow,visualInfo->visual,AllocNone);
		attr.event_mask = StructureNotifyMask | VisibilityChangeMask;
		if(fullScreen) {
			mask = CWBackPixel | CWColormap | CWOverrideRedirect | CWSaveUnder | CWBackingStore | CWEventMask;
			attr.override_redirect = True;
			attr.backing_store = NotUseful;
			attr.save_under = False;
			// Fullscreen windows are always in the top left origin
			left = top = 0;
		} else
			mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;
	
		// Create window on server
		mWindow = XCreateWindow(mDisplay,parentWindow,left,top,width,height,0,visualInfo->depth,InputOutput,visualInfo->visual,mask,&attr);
		if(!mWindow) {
			OGRE_EXCEPT(999, "GLXWindow: XCreateWindow failed", "GLXWindow::create");
		}
	
		// Make sure the window is in normal state
		XWMHints *wm_hints;
		if ((wm_hints = XAllocWMHints()) != NULL) {
			wm_hints->initial_state = NormalState;
			wm_hints->input = True;
			wm_hints->flags = StateHint | InputHint;
	
			// Check if we can give it an icon
			if(depth == 24 || depth == 32) {
				// Woot! The right bit depth, we can load an icon
				if(GLXUtils::LoadIcon(mDisplay, rootWindow, "GLX_icon.png", &wm_hints->icon_pixmap, &wm_hints->icon_mask))
					wm_hints->flags |= IconPixmapHint | IconMaskHint;
			}
		}
	
		// Set size and location hints
		XSizeHints *size_hints;
		if ((size_hints = XAllocSizeHints()) != NULL) {
			// Otherwise some window managers ignore our position request
			size_hints->flags = USPosition;
		}
	
		// Make text property from title
		XTextProperty titleprop;
		char *lst = (char*)title.c_str();
		XStringListToTextProperty((char **)&lst, 1, &titleprop);
	
		XSetWMProperties(mDisplay, mWindow, &titleprop, NULL, NULL, 0, size_hints, wm_hints, NULL);
	
		// We don't like memory leaks. Free the clientside storage, but not the
		// pixmaps as they're still being used by the server.
		XFree(titleprop.value);
		XFree(wm_hints);
		XFree(size_hints);
	
		// Acquire atom to recognize window close events
		mAtomDeleteWindow = XInternAtom(mDisplay,"WM_DELETE_WINDOW",False);
		XSetWMProtocols(mDisplay,mWindow,&mAtomDeleteWindow,1);
	
		// Map window unto screen and focus it.
		XMapWindow(mDisplay,mWindow);
	
		// Make sure the server is up to date and focus the window
		XFlush(mDisplay);
	}
	else
	{
		LogManager::getSingleton().logMessage("GLXWindow::create -- using external window handle");
		visualInfo = extVisualHandler;
	}

	GLRenderSystem *rs = static_cast<GLRenderSystem*>(Root::getSingleton().getRenderSystem());
	GLXContext* mainContext = static_cast<GLXContext*>( rs->_getMainContext() );
	if ( mainContext == 0 )
	{
		// Finally, create a GL context
		// we want to share it with main
		mGlxContext = glXCreateContext(mDisplay,visualInfo,NULL,True);
	}
		else
			mGlxContext = glXCreateContext(mDisplay,visualInfo,mainContext->mCtx,True);
	
	if(!mGlxContext)
		OGRE_EXCEPT(999, "glXCreateContext failed", "GLXWindow::create");

	// Free visual info
	if (extVisualHandler == NULL)
		XFree(visualInfo);

	mName = name;
	mWidth = width;
	mHeight = height;
	mFullScreen = fullScreen;

    // Create OGRE GL context
    mContext = new GLXContext(mDisplay, mWindow, mGlxContext);
}

//-------------------------------------------------------------------------------------------------//
void GLXWindow::destroy(void)
{
	// Unregister and destroy OGRE GLContext
	delete mContext;

	// Destroy GL context
	if(mGlxContext)
		glXDestroyContext(mDisplay, mGlxContext);

	if(mWindow)
		XDestroyWindow(mDisplay, mWindow);

	mContext = 0;
	mWindow = 0;
	mGlxContext = 0;
	mActive = false;

	Root::getSingleton().getRenderSystem()->detachRenderTarget( this->getName() );
}

//-------------------------------------------------------------------------------------------------//
bool GLXWindow::isActive() const
{
	return mActive;
}

//-------------------------------------------------------------------------------------------------//
bool GLXWindow::isClosed() const
{
	return mClosed;
}

//-------------------------------------------------------------------------------------------------//
bool GLXWindow::isVisible() const
{
	return mVisible;
}

//-------------------------------------------------------------------------------------------------//
void GLXWindow::reposition(int left, int top)
{
	XMoveWindow(mDisplay,mWindow,left,top);
}

//-------------------------------------------------------------------------------------------------//
void GLXWindow::resize(unsigned int width, unsigned int height)
{
	if (!mTopLevel)
		resized(width, height); /// Embedded
	else
		XResizeWindow(mDisplay,mWindow,width,height); /// Ogre handles window
}

//-------------------------------------------------------------------------------------------------//
void GLXWindow::swapBuffers(bool waitForVSync)
{
	glXSwapBuffers(mDisplay,mWindow);
}

//-------------------------------------------------------------------------------------------------//
void GLXWindow::injectXEvent(const XEvent &event)
{
	switch(event.type) 
	{
	case ClientMessage:
		if(event.xclient.display != mDisplay || event.xclient.window != mWindow)
			break;

		if(event.xclient.format == 32 && event.xclient.data.l[0] == (long)mAtomDeleteWindow)  
		{
			//Window Closed (via X button)
			mClosed = true;
			mActive = false;

			Root::getSingleton().getRenderSystem()->detachRenderTarget( this->getName() );
		}
		break;
	case ConfigureNotify:
		if(event.xconfigure.display != mDisplay || event.xconfigure.window != mWindow)
			break;

		resized(event.xconfigure.width,	event.xconfigure.height);
		break;
	case MapNotify:
		if(event.xconfigure.display != mDisplay || event.xconfigure.window != mWindow)
			break;

		// Window was mapped to the screen
		mActive = true;
		break;
	case UnmapNotify:
		if(event.xconfigure.display != mDisplay || event.xconfigure.window != mWindow)
			break;

		// Window was unmapped from the screen (user switched
		// to another workspace, for example)
		mActive = false;
		break;
	case VisibilityNotify:
		//Visibility status changed
		switch(event.xvisibility.state)
		{
		case VisibilityUnobscured:
			mActive = mVisible = true;
			break;
		case VisibilityPartiallyObscured:
			mActive = false;
			mVisible = true;
			break;
		case VisibilityFullyObscured:
			mActive = mVisible = false;
			break;
		}
	}
}

//-------------------------------------------------------------------------------------------------//
void GLXWindow::resized(size_t width, size_t height)
{
	// Check if the window size really changed
	if(mWidth == width && mHeight == height)
		return;

	mWidth = width;
	mHeight = height;

	for (ViewportList::iterator it = mViewportList.begin();	it != mViewportList.end(); ++it)
		(*it).second->_updateDimensions();
}

//-------------------------------------------------------------------------------------------------//
void GLXWindow::getCustomAttribute( const String& name, void* pData )
{
	if( name == "GLCONTEXT" ) 
	{
		*static_cast<GLXContext**>(pData) = mContext;
		return;
	} 
	else if( name == "GLXWINDOW" ) 
	{
		*static_cast<Window*>(pData) = mWindow;
		return;
	} 
	else if( name == "GLXDISPLAY" ) 
	{
		*static_cast<Display**>(pData) = mDisplay;
		return;
	}
}

//-------------------------------------------------------------------------------------------------//
void GLXWindow::writeContentsToFile(const String& filename)
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
	img.loadRawData(stream, mWidth, mHeight, PF_BYTE_RGB );
	img.flipAroundX();

	MemoryDataStreamPtr streamFlipped(new MemoryDataStream(img.getData(), stream->size(), false));

	// Get codec
	size_t pos = filename.find_last_of(".");
	String extension;
	if( pos == String::npos )
		OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, "Unable to determine image type for '" 
			+ filename + "' - invalid extension.", "SDLWindow::writeContentsToFile" );

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


#ifndef OGRE_GLXGLSupport_H
#define OGRE_GLXGLSupport_H

#include "OgreGLSupport.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/xf86vmode.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

namespace Ogre {

class GLXGLSupport : public GLSupport {
public:
	GLXGLSupport();
	~GLXGLSupport();

	/**
	* Add any special config values to the system.
	* Must have a "Full Screen" value that is a bool and a "Video Mode" value
	* that is a string in the form of wxh
	*/
	void addConfig(void);
	/**
	* Make sure all the extra options are valid
	*/
	String validateConfig(void);

	/// @copydoc GLSupport::createWindow
	RenderWindow* createWindow(bool autoCreateWindow, GLRenderSystem* renderSystem, const String& windowTitle);
	
	/// @copydoc RenderSystem::createRenderWindow
	virtual RenderWindow* newWindow(const String &name, unsigned int width, unsigned int height, 
		bool fullScreen, const NameValuePairList *miscParams = 0);

	
	/**
	* Start anything special
	*/
	void start();
	/**
	* Stop anything special
	*/
	void stop();

	/**
	* Get the address of a function
	*/
	void* getProcAddress(const String& procname);
 
    virtual GLPBuffer *createPBuffer(PixelComponentType format, size_t width, size_t height);
private:
	// X display
	Display *mDisplay;
}
; // class GLXGLSupport

}
; // namespace Ogre

#endif // OGRE_GLXGLSupport_H

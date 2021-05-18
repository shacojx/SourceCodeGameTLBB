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
#ifndef __GLXUtils_H__
#define __GLXUtils_H__

#include "OgrePrerequisites.h"
#include <X11/Xlib.h>
#include <GL/glx.h>

namespace Ogre {
   /**
     * Class that acquires and stores properties of a frame buffer configuration
     */
    class FBConfigData 
    {
    public:
        FBConfigData();
        FBConfigData(Display *dpy, GLXFBConfig config);
        String toString() const;
        
        int configID;
        int visualID;
        int bufferSize;
        int level;
        int doubleBuffer;
        int stereo;
        int auxBuffers;
        int renderType;
        int redSize;
        int greenSize;
        int blueSize;
        int alphaSize;
        int depthSize;
        int stencilSize;
        int accumRedSize;
        int accumGreenSize;        
        int accumBlueSize;
        int accumAlphaSize;        
        int drawableType;
        int caveat;
        int maxPBufferWidth;
        int maxPBufferHeight;
        int maxPBufferPixels;        
    };
    class GLXUtils {
    public:
        /**
         * Loads an icon from an Ogre resource into the X Server. This currently only
         * works for 24 and 32 bit displays. The image must be findable by the Ogre
         * resource system, and of format PF_A8R8G8B8.
         *
         * @param mDisplay,rootWindow   X resources to use
         * @param name           Name of image to load
         * @param pix         Receiver for the output pixmap
         * @param mask           Receiver for the output mask (alpha bitmap)
         * @returns true on success
         */        
        static bool LoadIcon(Display *mDisplay, Window rootWindow, const std::string &name, Pixmap *pix, Pixmap *mask);
        /*
         * Examine all visuals to find the so-called best one.
         * We prefer deepest RGBA buffer with depth, stencil and accum
         * that has no caveats. This will only choose formats with a multisample
		 * that equals multisample
		 * @returns -1 in case of failure, otherwise a valid visual ID
         * @author Brian Paul (from the glxinfo source)
         */
        static int findBestVisual(Display *dpy, int scrnum, int multisample = -1);
        /**
         * Find best FBConfig given a list required and a list of desired properties
         */
        static GLXFBConfig findBestMatch(Display *dpy, int scrnum, const int *attribs, const int *ideal);
        
    };
   
};

#endif

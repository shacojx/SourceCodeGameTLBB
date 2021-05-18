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
#ifndef __PlatformManager_H__
#define __PlatformManager_H__

#include "OgrePrerequisites.h"

#include "OgreSingleton.h"

namespace Ogre {
    typedef void (*DLL_CREATECONFIGDIALOG)(ConfigDialog** ppDlg);
    typedef void (*DLL_CREATEERRORDIALOG)(ErrorDialog** ppDlg);
    typedef void (*DLL_CREATEINPUTREADER)(InputReader** ppReader);
	typedef void (*DLL_CREATETIMER)(Timer** ppTimer);

    typedef void (*DLL_DESTROYCONFIGDIALOG)(ConfigDialog* ppDlg);
    typedef void (*DLL_DESTROYERRORDIALOG)(ErrorDialog* ppDlg);
    typedef void (*DLL_DESTROYINPUTREADER)(InputReader* ppReader);
    typedef void (*DLL_DESTROYTIMER)(Timer* ppTimer);

	typedef void (*DLL_MESSAGEPUMP)(RenderWindow* rw);

    /** Class which manages the platform settings Ogre runs on.
        @remarks
            Because Ogre is designed to be platform-independent, it
            dynamically loads a library containing all the platform-specific
            elements like dialogs etc. 
        @par
            This class manages that load and provides a simple interface to
            the platform.
    */
    class _OgreExport PlatformManager : public Singleton<PlatformManager>
    {
    protected:
        DLL_CREATECONFIGDIALOG mpfCreateConfigDialog;
        DLL_CREATEERRORDIALOG mpfCreateErrorDialog;
        DLL_CREATEINPUTREADER mpfCreateInputReader;
		DLL_CREATETIMER mpfCreateTimer;
		
		DLL_DESTROYCONFIGDIALOG mpfDestroyConfigDialog;
        DLL_DESTROYERRORDIALOG mpfDestroyErrorDialog;
        DLL_DESTROYINPUTREADER mpfDestroyInputReader;
        DLL_DESTROYTIMER mpfDestroyTimer;

		DLL_MESSAGEPUMP mpfMessagePump;
		
    public:
        /** Default constructor.
        */
        PlatformManager();

        /** Gets a new instance of a platform-specific config dialog.
            @remarks
                The instance returned from this method will be a
                platform-specific subclass of ConfigDialog, and must be
                destroyed by the caller when required.
        */
        ConfigDialog* createConfigDialog();

        /** Destroys an instance of a platform-specific config dialog.
            @remarks
                Required since deletion of objects must be performed on the
                correct heap.
        */
        void destroyConfigDialog(ConfigDialog* dlg);

        /** Gets a new instance of a platform-specific config dialog.
            @remarks
                The instance returned from this method will be a
                platform-specific subclass of ErrorDialog, and must be
                destroyed by the caller when required.
        */
        ErrorDialog* createErrorDialog();

        /** Destroys an instance of a platform-specific error dialog.
            @remarks
                Required since deletion of objects must be performed on the
                correct heap.
        */
        void destroyErrorDialog(ErrorDialog* dlg);

        /** Gets a new instance of a platform-specific input reader.
            @remarks
                The instance returned from this method will be a
                platform-specific subclass of InputReader, and must be
                destroyed by the caller when required.
        */
        InputReader* createInputReader();

        /** Destroys an instance of a platform-specific input reader.
            @remarks
                Required since deletion of objects must be performed on the
                correct heap.
        */
        void destroyInputReader(InputReader* reader);
		
		/** Creates a new Timer instance
		*/
		Timer* createTimer();

        /** Destroys an instance of a timer. */
        void destroyTimer(Timer* timer);

		/**
		@remarks
			Allows platform to provide Platform specific Event
			updating/dispatching per frame (ie. Win32 Message Pump) as called
			from Root::startRendering . If you are not using Root::startRendering,
			you can call this function yourself, or run your own event pump
		*/
		void messagePump(RenderWindow* rw);

        /** Override standard Singleton retrieval.
        @remarks
        Why do we do this? Well, it's because the Singleton
        implementation is in a .h file, which means it gets compiled
        into anybody who includes it. This is needed for the
        Singleton template to work, but we actually only want it
        compiled into the implementation of the class based on the
        Singleton, not all of them. If we don't change this, we get
        link errors when trying to use the Singleton-based class from
        an outside dll.
        @par
        This method just delegates to the template version anyway,
        but the implementation stays in this single compilation unit,
        preventing link errors.
        */
        static PlatformManager& getSingleton(void);
        /** Override standard Singleton retrieval.
        @remarks
        Why do we do this? Well, it's because the Singleton
        implementation is in a .h file, which means it gets compiled
        into anybody who includes it. This is needed for the
        Singleton template to work, but we actually only want it
        compiled into the implementation of the class based on the
        Singleton, not all of them. If we don't change this, we get
        link errors when trying to use the Singleton-based class from
        an outside dll.
        @par
        This method just delegates to the template version anyway,
        but the implementation stays in this single compilation unit,
        preventing link errors.
        */
        static PlatformManager* getSingletonPtr(void);


    };


}

#endif

/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.stevestreeting.com/ogre/

Copyright (c) 2000-2005 The OGRE Teameeting
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/gpl.html.
-----------------------------------------------------------------------------
*/

#ifndef __GLGpuProgram_H__
#define __GLGpuProgram_H__

#include "OgreGLPrerequisites.h"
#include "OgreGpuProgram.h"

namespace Ogre {

    /** Generalised low-level GL program, can be applied to multiple types (eg ARB and NV) */
    class GLGpuProgram : public GpuProgram
    {
    public:
        GLGpuProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
            const String& group, bool isManual = false, ManualResourceLoader* loader = 0);
        virtual ~GLGpuProgram();

        /// Execute the binding functions for this program
        virtual void bindProgram(void) {}
        /// Execute the binding functions for this program
        virtual void unbindProgram(void) {}
        /** Overridden from GpuProgram, do nothing */
        void loadFromSource(void) {}

        /// Execute the param binding functions for this program
        virtual void bindProgramParameters(GpuProgramParametersSharedPtr params) {}
		/// Bind just the pass iteration parameters
		virtual void bindProgramPassIterationParameters(GpuProgramParametersSharedPtr params) {}


        /// Get the assigned GL program id
        const GLuint getProgramID(void) const
        { return mProgramID; }

    protected:
        /// @copydoc Resource::unloadImpl
        void unloadImpl(void) {}

        GLuint mProgramID;
        GLenum mProgramType;
    };

    /** Specialisation of the GL low-level program for ARB programs. */
    class GLArbGpuProgram : public GLGpuProgram
    {
    public:
        GLArbGpuProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
            const String& group, bool isManual = false, ManualResourceLoader* loader = 0);
        virtual ~GLArbGpuProgram();

        /// @copydoc GpuProgram::setType
        void setType(GpuProgramType t);


        /// Execute the binding functions for this program
        void bindProgram(void);
        /// Execute the unbinding functions for this program
        void unbindProgram(void);
        /// Execute the param binding functions for this program
        void bindProgramParameters(GpuProgramParametersSharedPtr params);
		/// Bind just the pass iteration parameters
		void bindProgramPassIterationParameters(GpuProgramParametersSharedPtr params);

        /// Get the GL type for the program
        const GLuint getProgramType(void) const
        { return mProgramType; }

    protected:
        void loadFromSource(void);
        /// @copydoc Resource::unloadImpl
        void unloadImpl(void);

    };



}; // namespace Ogre

#endif // __GLGpuProgram_H__

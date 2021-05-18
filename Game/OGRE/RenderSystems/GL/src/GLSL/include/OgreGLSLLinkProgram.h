/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#ifndef __GLSLLinkProgram_H__
#define __GLSLLinkProgram_H__

#include "OgreGLPrerequisites.h"
#include "OgreGpuProgram.h"

namespace Ogre {

	/** C++ encapsulation of GLSL Program Object

	*/

	class GLSLLinkProgram
	{
	private:
		/// structure used to keep track of named uniforms in the linked program object
		struct UniformReference
		{
			String mName;
			GLenum mType;
			GLint  mLocation;
			bool isReal;
			GLsizei mElementCount;
            GLint mArraySize;
		};

        typedef std::vector<UniformReference> UniformReferenceList;
		typedef UniformReferenceList::iterator UniformReferenceIterator;
		/// container of uniform references that are active in the program object
		UniformReferenceList mUniformReferences;

		/// flag to indicate that uniform references have already been built
		bool		mUniformRefsBuilt;
		/// GL handle for the program object
		GLhandleARB mGLHandle;
		/// flag indicating that the program object has been successfully linked
		GLint		mLinked;
        /// flag indicating skeletal animation is being performed
        bool mSkeletalAnimation;

		/// build uniform references from active named uniforms
		void buildUniformReferences(void);

	public:
		/// constructor should only be used by GLSLLinkProgramManager
		GLSLLinkProgram(void);
		~GLSLLinkProgram(void);

		/** Makes a program object active by making sure it is linked and then putting it in use.

		*/
		void activate(void);
		/** updates program object uniforms using data from GpuProgramParamters.
		normally called by GLSLGpuProgram::bindParameters() just before rendering occurs.
		*/
		void updateUniforms(GpuProgramParametersSharedPtr params);
		/** updates program object uniforms using data from pass iteration GpuProgramParamters.
		normally called by GLSLGpuProgram::bindMultiPassParameters() just before multi pass rendering occurs.
		*/
		void updatePassIterationUniforms(GpuProgramParametersSharedPtr params);
		/// get the GL Handle for the program object
		GLhandleARB getGLHandle(void) const { return mGLHandle; }
        /** Sets whether the linked program includes the required instructions
        to perform skeletal animation. 
        @remarks
        If this is set to true, OGRE will not blend the geometry according to 
        skeletal animation, it will expect the vertex program to do it.
        */
        void setSkeletalAnimationIncluded(bool included) 
        { mSkeletalAnimation = included; }

        /** Returns whether the linked program includes the required instructions
            to perform skeletal animation. 
        @remarks
            If this returns true, OGRE will not blend the geometry according to 
            skeletal animation, it will expect the vertex program to do it.
        */
        bool isSkeletalAnimationIncluded(void) const { return mSkeletalAnimation; }

	};

}

#endif // __GLSLLinkProgram_H__

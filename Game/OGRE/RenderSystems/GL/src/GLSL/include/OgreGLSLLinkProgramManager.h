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
#ifndef __GLSLLinkProgramManager_H__
#define __GLSLLinkProgramManager_H__

#include "OgreGLPrerequisites.h"
#include "OgreSingleton.h"

#include "OgreGLSLExtSupport.h"

namespace Ogre {

	/** Ogre assumes that there are seperate vertex and fragment programs to deal with but
		GLSL has one program object that represents the active vertex and fragment shader objects
		during a rendering state.  GLSL Vertex and fragment 
		shader objects are compiled seperately and then attached to a program object and then the
		program object is linked.  Since Ogre can only handle one vertex program and one fragment
		program being active in a pass, the GLSL Link Program Manager does the same.  The GLSL Link
		program manager acts as a state machine and activates a program object based on the active
		vertex and fragment program.  Previously created program objects are stored along with a unique
		key in a hash_map for quick retrieval the next time the program object is required.

	*/

	class GLSLLinkProgramManager : public Singleton<GLSLLinkProgramManager>
	{

	private:
	
		typedef HashMap<GLuint, GLSLLinkProgram*> LinkProgramMap;
		typedef LinkProgramMap::iterator LinkProgramIterator;

		/// container holding previously created program objects 
		LinkProgramMap LinkPrograms; 

		/// active objects defining the active rendering gpu state
		GLSLGpuProgram* mActiveVertexGpuProgram;
		GLSLGpuProgram* mActiveFragmentGpuProgram;
		GLSLLinkProgram* mActiveLinkProgram;

	public:

		GLSLLinkProgramManager(void);

		~GLSLLinkProgramManager(void);

		/**
			Get the program object that links the two active shader objects together
			if a program object was not already created and linked a new one is created and linked
		*/
		GLSLLinkProgram* getActiveLinkProgram(void);

		/** Set the active fragment shader for the next rendering state.
			The active program object will be cleared.
			Normally called from the GLSLGpuProgram::bindProgram and unbindProgram methods
		*/
		void setActiveFragmentShader(GLSLGpuProgram* fragmentGpuProgram);
		/** Set the active vertex shader for the next rendering state.
			The active program object will be cleared.
			Normally called from the GLSLGpuProgram::bindProgram and unbindProgram methods
		*/
		void setActiveVertexShader(GLSLGpuProgram* vertexGpuProgram);

		static GLSLLinkProgramManager& getSingleton(void);
        static GLSLLinkProgramManager* getSingletonPtr(void);

	};

}

#endif // __GLSLLinkProgramManager_H__

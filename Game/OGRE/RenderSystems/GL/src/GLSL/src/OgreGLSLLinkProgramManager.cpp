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

#include "OgreGLSLLinkProgramManager.h"
#include "OgreGLSLLinkProgram.h"
#include "OgreGLSLGpuProgram.h"
#include "OgreGLSLProgram.h"

namespace Ogre {

	//-----------------------------------------------------------------------
	template<> GLSLLinkProgramManager* Singleton<GLSLLinkProgramManager>::ms_Singleton = 0;

	//-----------------------------------------------------------------------
    GLSLLinkProgramManager* GLSLLinkProgramManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }

	//-----------------------------------------------------------------------
    GLSLLinkProgramManager& GLSLLinkProgramManager::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }

	//-----------------------------------------------------------------------
	GLSLLinkProgramManager::GLSLLinkProgramManager(void) : mActiveVertexGpuProgram(NULL),
		mActiveFragmentGpuProgram(NULL), mActiveLinkProgram(NULL)
	{

	}

	//-----------------------------------------------------------------------
	GLSLLinkProgramManager::~GLSLLinkProgramManager(void)
	{
		// iterate through map container and delete link programs
		for (LinkProgramIterator currentProgram = LinkPrograms.begin();
			currentProgram != LinkPrograms.end(); ++currentProgram)
		{
			delete currentProgram->second;
		}

	}

	//-----------------------------------------------------------------------
	GLSLLinkProgram* GLSLLinkProgramManager::getActiveLinkProgram(void)
	{
		// if there is an active link program then return it
		if (mActiveLinkProgram)
			return mActiveLinkProgram;

		// no active link program so find one or make a new one
		// is there an active key?
		GLuint activeKey = 0;

		if (mActiveVertexGpuProgram)
		{
			activeKey = mActiveVertexGpuProgram->getProgramID() << 16;
		}

		if (mActiveFragmentGpuProgram)
		{
			activeKey += mActiveFragmentGpuProgram->getProgramID();
		}

		// only return a link program object if a vertex or fragment program exist
		if (activeKey > 0)
		{
			// find the key in the hash map
			LinkProgramIterator programFound = LinkPrograms.find(activeKey);
			// program object not found for key so need to create it
			if (programFound == LinkPrograms.end())
			{
				mActiveLinkProgram = new GLSLLinkProgram();
				LinkPrograms[activeKey] = mActiveLinkProgram;
				// tell shaders to attach themselves to the LinkProgram
				// let the shaders do the attaching since they may have several children to attach
				if (mActiveVertexGpuProgram)
				{
					mActiveVertexGpuProgram->getGLSLProgram()->attachToProgramObject( mActiveLinkProgram->getGLHandle() );
                    mActiveLinkProgram->setSkeletalAnimationIncluded(mActiveVertexGpuProgram->isSkeletalAnimationIncluded());
				}

				if (mActiveFragmentGpuProgram)
				{
					mActiveFragmentGpuProgram->getGLSLProgram()->attachToProgramObject( mActiveLinkProgram->getGLHandle() );
				}


			}
			else
			{
				// found a link program in map container so make it active
				mActiveLinkProgram = programFound->second;
			}

		}
		// make the program object active
		if (mActiveLinkProgram) mActiveLinkProgram->activate();

		return mActiveLinkProgram;

	}

	//-----------------------------------------------------------------------
	void GLSLLinkProgramManager::setActiveFragmentShader(GLSLGpuProgram* fragmentGpuProgram)
	{
		if (fragmentGpuProgram != mActiveFragmentGpuProgram)
		{
			mActiveFragmentGpuProgram = fragmentGpuProgram;
			// ActiveLinkProgram is no longer valid
			mActiveLinkProgram = NULL;
			// change back to fixed pipeline
			glUseProgramObjectARB(0);
		}
	}

	//-----------------------------------------------------------------------
	void GLSLLinkProgramManager::setActiveVertexShader(GLSLGpuProgram* vertexGpuProgram)
	{
		if (vertexGpuProgram != mActiveVertexGpuProgram)
		{
			mActiveVertexGpuProgram = vertexGpuProgram;
			// ActiveLinkProgram is no longer valid
			mActiveLinkProgram = NULL;
			// change back to fixed pipeline
			glUseProgramObjectARB(0);
		}
	}

}

/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.stevestreeting.com/ogre/

Copyright (c) 2000-2005 The OGRE Team
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


#ifndef __GLSLProgramFactory_H__
#define __GLSLProgramFactory_H__

#include "OgreHighLevelGpuProgramManager.h"
#include "OgreGLSLExtSupport.h"

namespace Ogre
{
    /** Factory class for GLSL programs. */
    class GLSLProgramFactory : public HighLevelGpuProgramFactory
    {
    protected:
    public:
        GLSLProgramFactory(void);
        ~GLSLProgramFactory(void);
		/// Get the name of the language this factory creates programs for
		const String& getLanguage(void) const;
		/// create an instance of GLSLProgram
        HighLevelGpuProgram* create(ResourceManager* creator, 
            const String& name, ResourceHandle handle,
            const String& group, bool isManual, ManualResourceLoader* loader);
		void destroy(HighLevelGpuProgram* prog);

	private:
		GLSLLinkProgramManager* mLinkProgramManager;

    };
}

#endif // __GLSLProgramFactory_H__

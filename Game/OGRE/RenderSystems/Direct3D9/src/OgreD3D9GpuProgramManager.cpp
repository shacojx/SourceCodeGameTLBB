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
#include "OgreD3D9GpuProgramManager.h"
#include "OgreD3D9GpuProgram.h"
#include "OgreException.h"

namespace Ogre {
    //-----------------------------------------------------------------------------
    D3D9GpuProgramManager::D3D9GpuProgramManager(LPDIRECT3DDEVICE9 device)
        :GpuProgramManager(), mpDevice(device)
    {
        // Superclass sets up members 

        // Register with resource group manager
        ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);

    }
    //-----------------------------------------------------------------------------
    D3D9GpuProgramManager::~D3D9GpuProgramManager()
    {
        // Unregister with resource group manager
        ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);

    }
    //-----------------------------------------------------------------------------
    GpuProgramParametersSharedPtr D3D9GpuProgramManager::createParameters(void)
    {
        return GpuProgramParametersSharedPtr(new GpuProgramParameters());
    }
    //-----------------------------------------------------------------------------
    Resource* D3D9GpuProgramManager::createImpl(const String& name, ResourceHandle handle, 
        const String& group, bool isManual, ManualResourceLoader* loader,
        const NameValuePairList* params)
    {
        NameValuePairList::const_iterator paramIt;

        if (!params || (paramIt = params->find("type")) == params->end())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                "You must supply a 'type' parameter",
                "D3D9GpuProgramManager::createImpl");
        }

        if (paramIt->second == "vertex_program")
        {
            return new D3D9GpuVertexProgram(this, name, handle, group, 
                isManual, loader, mpDevice);
        }
        else
        {
            return new D3D9GpuFragmentProgram(this, name, handle, group, 
                isManual, loader, mpDevice);
        }
    }
    //-----------------------------------------------------------------------------
    Resource* D3D9GpuProgramManager::createImpl(const String& name, ResourceHandle handle, 
        const String& group, bool isManual, ManualResourceLoader* loader,
        GpuProgramType gptype, const String& syntaxCode)
    {
        if (gptype == GPT_VERTEX_PROGRAM)
        {
            return new D3D9GpuVertexProgram(this, name, handle, group, 
                isManual, loader, mpDevice);
        }
        else
        {
            return new D3D9GpuFragmentProgram(this, name, handle, group, 
                isManual, loader, mpDevice);
        }
    }
}

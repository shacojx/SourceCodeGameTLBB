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
#include "OgreBspResourceManager.h"
#include "OgreBspLevel.h"
#include "OgreQuake3ShaderManager.h"


namespace Ogre {

    //-----------------------------------------------------------------------
    template<> BspResourceManager* Singleton<BspResourceManager>::ms_Singleton = 0;
    BspResourceManager* BspResourceManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    BspResourceManager& BspResourceManager::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }
    //-----------------------------------------------------------------------

    //-----------------------------------------------------------------------
    BspResourceManager::BspResourceManager()
    {
        mResourceType = "BspLevel";
        // Also create related shader manager (singleton managed)
        mShaderMgr = new Quake3ShaderManager();

        ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
    }
    //-----------------------------------------------------------------------
    BspResourceManager::~BspResourceManager()
    {
        delete mShaderMgr;
        ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
    }
    //-----------------------------------------------------------------------
    ResourcePtr BspResourceManager::load(const String& name, 
        const String& group, bool isManual, 
        ManualResourceLoader* loader, const NameValuePairList* loadParams)
    {
        // Only 1 BSP level allowed loaded at once
        removeAll();

        return ResourceManager::load(name, group, isManual, loader, loadParams);

    }
    //-----------------------------------------------------------------------
    ResourcePtr BspResourceManager::load(DataStreamPtr& stream, 
		const String& group)
    {
        // Only 1 BSP level allowed loaded at once
        removeAll();

		ResourcePtr ret = create("bsplevel", group, true, 0);
		BspLevelPtr bspLevel = ret;
		bspLevel->load(stream);
		
        return ret;

    }
    //-----------------------------------------------------------------------
    Resource* BspResourceManager::createImpl(const String& name, ResourceHandle handle, 
        const String& group, bool isManual, ManualResourceLoader* loader, 
        const NameValuePairList* createParams)
    {
        return new BspLevel(this, name, handle, group, isManual, loader);
    }


}

/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

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
#include "OgreStableHeaders.h"
#include "OgreSimpleRenderable.h"
#include "OgreException.h"
#include "OgreSceneNode.h"

#include "OgreMaterialManager.h"

namespace Ogre {

    uint SimpleRenderable::ms_uGenNameCount = 0;

    SimpleRenderable::SimpleRenderable()
    {
        m_matWorldTransform = Matrix4::IDENTITY;

        m_strMatName = "BaseWhite"; 
        m_pMaterial = MaterialManager::getSingleton().getByName("BaseWhite");

        m_pParentSceneManager = NULL;

        mParentNode = NULL;
        m_pCamera = NULL;

        // Generate name
		StringUtil::StrStreamType name;
		name << _TO_CHAR("SimpleRenderable") << ms_uGenNameCount++;
		mName = name.str();
    }

    void SimpleRenderable::setMaterial( const String& matName )
    {
        m_strMatName = matName;
        m_pMaterial = MaterialManager::getSingleton().getByName(m_strMatName);
		if (m_pMaterial.isNull())
			OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Could not find material " + m_strMatName,
				"SimpleRenderable::setMaterial" );
    
        // Won't load twice anyway
        m_pMaterial->load();
    }

    const MaterialPtr& SimpleRenderable::getMaterial(void) const
    {
        return m_pMaterial;
    }

    void SimpleRenderable::getRenderOperation(RenderOperation& op)
    {
        op = mRenderOp;
    }

    void SimpleRenderable::setRenderOperation( const RenderOperation& rend )
    {
        mRenderOp = rend;
    }

    void SimpleRenderable::setWorldTransform( const Matrix4& xform )
    {
        m_matWorldTransform = xform;
    }

    void SimpleRenderable::getWorldTransforms( Matrix4* xform ) const
    {
        *xform = m_matWorldTransform * mParentNode->_getFullTransform();
    }
    //-----------------------------------------------------------------------
    const Quaternion& SimpleRenderable::getWorldOrientation(void) const
    {
        return mParentNode->_getDerivedOrientation();
    }
    //-----------------------------------------------------------------------
    const Vector3& SimpleRenderable::getWorldPosition(void) const
    {
        return mParentNode->_getDerivedPosition();
    }

    void SimpleRenderable::_notifyCurrentCamera(Camera* cam)
    {
		MovableObject::_notifyCurrentCamera(cam);

        m_pCamera = cam;
    }

    void SimpleRenderable::setBoundingBox( const AxisAlignedBox& box )
    {
        mBox = box;
    }

    const AxisAlignedBox& SimpleRenderable::getBoundingBox(void) const
    {
        return mBox;
    }

    void SimpleRenderable::_updateRenderQueue(RenderQueue* queue)
    {
        queue->addRenderable( this, mRenderQueueID, OGRE_RENDERABLE_DEFAULT_PRIORITY); 
    }

    SimpleRenderable::~SimpleRenderable()
    {
    }
    //-----------------------------------------------------------------------
    const String& SimpleRenderable::getMovableType(void) const
    {
        static String movType = "SimpleRenderable";
        return movType;
    }
    //-----------------------------------------------------------------------
    const LightList& SimpleRenderable::getLights(void) const
    {
        // Use movable query lights
        return queryLights();
    }

}

/********************************************************************
filename:   WXMeshElement.cpp

purpose:    a mesh effect element.
*********************************************************************/

#include "WXMeshElement.h"
#include "WXEffect.h"
#include "Core/WXSystem.h"

// ogre header
//#include <OgreParticleSystem.h>
//#include <OgreParticleSystemManager.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreSubEntity.h>
#include <OgreSubMesh.h>

namespace WX	{

	MeshElement::CmdMeshName MeshElement::msMeshNameCmd;
	MeshElement::CmdMaterialName MeshElement::msMaterialNameCmd;

	MeshElement::MeshElement(const String &type, System *system) :
	EffectElement(type, system),
	mEntity(NULL),
	mMeshName(""), mMaterialName("none")
	{
		if ( initDefaults(mElementType) )
		{
			Ogre::ParamDictionary* dict = getParamDictionary();

        // Custom params      
			dict->addParameter(Ogre::ParameterDef("MeshName", 
            "the name of mesh.",
			Ogre::PT_STRING),&msMeshNameCmd);  

			dict->addParameter(Ogre::ParameterDef("Material", 
			"the name of material.",
			Ogre::PT_STRING),&msMaterialNameCmd);  
		}
	}
	//---------------------------------------------------------------------
	MeshElement::~MeshElement()
	{
		destroyRenderInstance();
	}
	//---------------------------------------------------------------------
	bool MeshElement::createRenderInstance(void)
	{
        return _createMesh();
	}
	//---------------------------------------------------------------------
	void MeshElement::destroyRenderInstance(void)
	{	
		_destroyMesh();  

		EffectElement::destroyRenderInstance();
	}
	//---------------------------------------------------------------------
	void MeshElement::destroyMe(void)
	{
		EffectElement::destroyMe();
	}
	//---------------------------------------------------------------------
	bool MeshElement::_createMesh(void)
	{
		assert ( NULL == mEntity );
		//assert ( false == mMeshName.empty() );

		if ( false == mMeshName.empty() )
		{
			if (mBasicNode)
			{
				assert (mEffectParent);

				mEntity = mSystem->getSceneManager()->createEntity(
					mBasicNode->getName(), mMeshName );
				assert (mEntity);
				mEntity->setMaterialName(mMaterialName);
                mEntity->setCastShadows(false);

                mEntity->setVisible(mVisible);

				mBasicNode->attachObject(mEntity);
				mBasicNode->setPosition(mRelPos);
                mBasicNode->setOrientation(mRelOrientation);

				return true;
			}
		}		

		return false;
	}
	//---------------------------------------------------------------------
	void MeshElement::_destroyMesh(void)
	{
		if (mEntity)
		{
			assert(mBasicNode);
			Ogre::SceneManager* creator = mBasicNode->getCreator();
			assert(creator);       

			mBasicNode->detachObject(mEntity->getName());
#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
			creator->destroyMovableObject(mEntity);
#else
			creator->removeEntity(mEntity->getName());
#endif

			mEntity = NULL;
		}
	}
	//---------------------------------------------------------------------
	void MeshElement::setMeshName(const String &name)
	{
		mMeshName = name;

		_destroyMesh();

		mIsRenderInstanceCreated = false;
	}
	//---------------------------------------------------------------------
	void MeshElement::setMaterialName(const String &name)
	{
		mMaterialName = name;

		if (mEntity)
		{
			// if this is "none",we will use the origin material of the mesh
			if (mMaterialName == "none")
			{

				if ( false == mMeshName.empty() )
				{
					Ogre::MeshPtr currentMesh = Ogre::MeshManager::getSingleton().getByName(mMeshName);

					for ( unsigned short i=0; i<currentMesh->getNumSubMeshes(); ++i )
					{
						mEntity->getSubEntity(i)->setMaterialName( currentMesh->getSubMesh(i)->getMaterialName() );
					}
				}
			}		
			else
				mEntity->setMaterialName(mMaterialName);
		}		
	}
    //-----------------------------------------------------------------------
    void MeshElement::setVisibleFlag(uint32 flags)
    {
        if (mEntity)
            mEntity->setVisibilityFlags(flags);
    }
	///////////////////////////////////////////
	String MeshElement::CmdMeshName::doGet(const void* target) const
    {
        return static_cast<const MeshElement*>(target)->getMeshName();
    }
    void MeshElement::CmdMeshName::doSet(void* target, const String& val)
    {
        static_cast<MeshElement *>(target)->setMeshName(val);
    }    
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void MeshElement::CmdMeshName::doCopy(void* target, const void* source)
    {
        static_cast<MeshElement *>(target)->setMeshName(
            static_cast<const MeshElement*>(source)->getMeshName() );
    }
#endif

	String MeshElement::CmdMaterialName::doGet(const void* target) const
	{
		return static_cast<const MeshElement*>(target)->getMaterialName();
	}
	void MeshElement::CmdMaterialName::doSet(void* target, const String& val)
	{
		static_cast<MeshElement *>(target)->setMaterialName(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void MeshElement::CmdMaterialName::doCopy(void* target, const void* source)
    {
        static_cast<MeshElement *>(target)->setMaterialName(
            static_cast<const MeshElement*>(source)->getMaterialName() );
    }
#endif
}
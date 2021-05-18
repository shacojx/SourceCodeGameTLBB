/********************************************************************
filename:   FairyProjectorElement.cpp

purpose:    a terrain projector effect element.
*********************************************************************/

#include "WXProjectorElement.h"
#include "Core/WXProjector.h"
#include "Core/WXSystem.h"

// ogre header
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreParticleEmitter.h>
#include <OgreSceneManager.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreTextureUnitState.h>

namespace WX	{

	ProjectorElement::CmdMaterialName       ProjectorElement::msMaterialNameCmd;
	ProjectorElement::CmdProjectionSize     ProjectorElement::msProjectionSizeCmd;

	ProjectorElement::CmdScaleAdjust		ProjectorElement::msScaleCmd[MAX_STAGES];
	ProjectorElement::CmdScaleTimeAdjust	ProjectorElement::msScaleTimeCmd[MAX_STAGES];
	ProjectorElement::CmdScaleChangeTime    ProjectorElement::msScaleChangeTime;

	ProjectorElement::CmdColourAdjust		ProjectorElement::msColourCmd[MAX_STAGES];
	ProjectorElement::CmdColourTimeAdjust	ProjectorElement::msColourTimeCmd[MAX_STAGES];
	ProjectorElement::CmdColourChangeTime   ProjectorElement::msColourChangeTime;
    ProjectorElement::CmdShowFrustum        ProjectorElement::msShowFrustumCmd;

    ProjectorElement::CmdNearClipDistance   ProjectorElement::msNearClipDistanceCmd;
    ProjectorElement::CmdFarClipDistance    ProjectorElement::msFarClipDistanceCmd;

	ProjectorElement::ProjectorElement(const String &type, System *system) :
	EffectElement(type, system),
	mProjector(NULL),
	mMaterialName(""),
	mProjectionSize(200.0f),
	mTextureUnitUScale(1.0f),
	mTextureUnitVScale(1.0f),
	mProjectorColour(Ogre::ColourValue::White),
	mScaleChangeTime(0.0f),
	mColourChangeTime(0.0f),
    mShowFrustum(false),
    mNearClipDistance(100.0f),
    mFarClipDistance(1000.0f)
	{
		for (int i=0;i<MAX_STAGES;i++)
		{
			mScaleAdj[i]	= Ogre::Vector3(1.0f, 1.0f, 1.0f);
			mScaleTimeAdj[i]		= 1.0f;

			mColourAdj[i]	= Ogre::ColourValue::White;
			mColourTimeAdj[i]		= 1.0f;
		}

		if ( initDefaults(mElementType) )
		{
			Ogre::ParamDictionary* dict = getParamDictionary();

			dict->addParameter(Ogre::ParameterDef("Material", 
			  "the name of material of the projector.",
				Ogre::PT_STRING),&msMaterialNameCmd);      

			dict->addParameter(Ogre::ParameterDef("ProjectionSize", 
				"the size of projected terrain or object.",
				Ogre::PT_REAL),&msProjectionSizeCmd);  

			for (int i=0;i<MAX_STAGES;i++)
			{
				msScaleCmd[i].mIndex	= i;
				msScaleTimeCmd[i].mIndex		= i;

				Ogre::StringUtil::StrStreamType stage;
				stage << i;
				String	scale_title	= String("Scale") + stage.str();
				String	time_title		= String("ScaleTime") + stage.str();
				String	scale_descr	= String("Stage ") + stage.str() + String(" Scale.");
				String	time_descr		= String("Stage ") + stage.str() + String(" ScaleTime.");

				dict->addParameter(Ogre::ParameterDef(scale_title, scale_descr, Ogre::PT_VECTOR3), &msScaleCmd[i]);
				dict->addParameter(Ogre::ParameterDef(time_title,   time_descr,   Ogre::PT_REAL),		 &msScaleTimeCmd[i]);
			}

			dict->addParameter(Ogre::ParameterDef("ScaleChangeTime", 
				"the size of projected terrain or object.",
				Ogre::PT_REAL),&msScaleChangeTime);  

			for (int i=0;i<MAX_STAGES;i++)
			{
				msColourCmd[i].mIndex	= i;
				msColourTimeCmd[i].mIndex		= i;

				Ogre::StringUtil::StrStreamType stage;
				stage << i;
				String	colour_title	= String("Colour") + stage.str();
				String	time_title		= String("ColourTime") + stage.str();
				String	colour_descr	= String("Stage ") + stage.str() + String(" Colour.");
				String	time_descr		= String("Stage ") + stage.str() + String(" ColourTime.");

				dict->addParameter(Ogre::ParameterDef(colour_title, colour_descr, Ogre::PT_COLOURVALUE), &msColourCmd[i]);
				dict->addParameter(Ogre::ParameterDef(time_title,   time_descr,   Ogre::PT_REAL),		 &msColourTimeCmd[i]);
			}

			dict->addParameter(Ogre::ParameterDef("ColourChangeTime", 
				"the size of projected terrain or object.",
				Ogre::PT_REAL),&msColourChangeTime); 

            dict->addParameter(Ogre::ParameterDef("ShowFrustum", 
                "show the shape of the frustum that used in this projector.",
                Ogre::PT_BOOL),&msShowFrustumCmd);

            dict->addParameter(Ogre::ParameterDef("NearClipDistance", 
                "set the near clip distance of the projector.",
                Ogre::PT_REAL),&msNearClipDistanceCmd);

            dict->addParameter(Ogre::ParameterDef("FarClipDistance", 
                "set the far clip distance of the projector.",
                Ogre::PT_REAL),&msFarClipDistanceCmd);
        }
	}
	//---------------------------------------------------------------------
	ProjectorElement::~ProjectorElement()
	{
		destroyRenderInstance();
	}	
	//---------------------------------------------------------------------
	bool ProjectorElement::createRenderInstance(void)
	{
		return _createProjector();
	}
	//---------------------------------------------------------------------
	void ProjectorElement::destroyRenderInstance(void)
	{
		_destroyProjector();

		EffectElement::destroyRenderInstance();
	}
	//---------------------------------------------------------------------
	void ProjectorElement::updateRenderInstance( Real time, TransformInfo &info )
	{
		mCurrentLifeTime += time;

		_updateProjectorMaterial();
	}
	//---------------------------------------------------------------------
	void ProjectorElement::_updateProjectorMaterial(void)
	{
		if (mProjector)
		{
			_updateProjectorScale();
			_updateProjectorColour();

			Ogre::MaterialPtr matPtr = mProjector->_getPatchMaterial();

			if (false == matPtr.isNull())
			{
				Ogre::Material::TechniqueIterator ti = matPtr->getTechniqueIterator();
				while (ti.hasMoreElements())
				{
					Ogre::Technique* technique = ti.getNext();
					Ogre::Technique::PassIterator pi = technique->getPassIterator();
					while (pi.hasMoreElements())
					{
						Ogre::Pass* pass = pi.getNext();

						// set the colour of projector material
						pass->setSelfIllumination(mProjectorColour);

						Ogre::Pass::TextureUnitStateIterator tusi = pass->getTextureUnitStateIterator();
						while (tusi.hasMoreElements())
						{
							Ogre::TextureUnitState* tus = tusi.getNext();

							// set the scale value of the projector material
							tus->setTextureVScale(mTextureUnitVScale);
							tus->setTextureUScale(mTextureUnitUScale);
						}
					}
				}	
			}
		}
	}    
	//---------------------------------------------------------------------
	bool ProjectorElement::_createProjector(void)
	{
		assert( mProjector == NULL );

		Ogre::SceneNode* parent = mBasicNode;

		if (parent)
		{
			mProjector = static_cast<Projector*>
				( mSystem->getSceneManager()->createMovableObject(parent->getName(), ProjectorFactory::FACTORY_TYPE_NAME) );
			mProjector->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
			mProjector->setFOVy( _getFovFromProjectionSize() );
			mProjector->setTerrain( mSystem->getTerrain() );
			mProjector->setAspectRatio(1);
			mProjector->setShowFrustum(mShowFrustum);
			mProjector->setEnableClipPlanes(false);
			mProjector->setPatchMaterial(mMaterialName);
            mProjector->setCastShadows(false);

            mProjector->setNearClipDistance(mNearClipDistance);
            mProjector->setFarClipDistance(mFarClipDistance);

            mProjector->setVisible(mVisible);

			parent->attachObject(mProjector);
			parent->setPosition(Ogre::Vector3(0, 200, 0));
			parent->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y, parent->TS_WORLD);

			return true;
		}

		return false;
	}
	//---------------------------------------------------------------------
	void ProjectorElement::_destroyProjector(void)
	{
		if (mProjector)
		{
			Ogre::SceneNode* parent = mProjector->getParentSceneNode();
			assert(parent);
			Ogre::SceneManager* creator = parent->getCreator();
			assert(creator);

			parent->detachObject(mProjector->getName());

			mSystem->getSceneManager()->destroyMovableObject(mProjector);

			mProjector = NULL;
		} 
	}
	//---------------------------------------------------------------------
	void ProjectorElement::_updateProjectorScale(void)
	{
		if (mCurrentLifeTime > 0.0f && mScaleChangeTime > 0.0f)
		{
			Real			projector_time	= fmod( mCurrentLifeTime / mScaleChangeTime, 1.0f );

			if (projector_time <= mScaleTimeAdj[0])
			{
				_setTextureUnitScale(mScaleAdj[0].x, mScaleAdj[0].y);
			} 
			else if (projector_time >= mScaleTimeAdj[MAX_STAGES - 1])
			{
				_setTextureUnitScale(mScaleAdj[MAX_STAGES-1].x, mScaleAdj[MAX_STAGES-1].y);
			}
			else
			{
				for (int i=0;i<MAX_STAGES-1;i++)
				{
					if (projector_time >= mScaleTimeAdj[i] && projector_time < mScaleTimeAdj[i + 1])
					{
						projector_time -= mScaleTimeAdj[i];
						projector_time /= (mScaleTimeAdj[i+1]-mScaleTimeAdj[i]);

						_setTextureUnitScale(
							(mScaleAdj[i+1].x * projector_time) + (mScaleAdj[i].x * (1.0f - projector_time)),
							(mScaleAdj[i+1].y * projector_time) + (mScaleAdj[i].y * (1.0f - projector_time))
							);
						break;
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------
	void ProjectorElement::_updateProjectorColour(void)
	{
		if (mCurrentLifeTime > 0.0f && mColourChangeTime > 0.0f)
		{
			Real			projector_time	= fmod( mCurrentLifeTime / mColourChangeTime, 1.0f );

			if (projector_time <= mColourTimeAdj[0])
			{
				mProjectorColour = mColourAdj[0];
			} 
			else if (projector_time >= mColourTimeAdj[MAX_STAGES - 1])
			{
				mProjectorColour = mColourAdj[MAX_STAGES - 1];
			}
			else
			{
				for (int i=0;i<MAX_STAGES-1;i++)
				{
					if (projector_time >= mColourTimeAdj[i] && projector_time < mColourTimeAdj[i + 1])
					{
						projector_time -= mColourTimeAdj[i];
						projector_time /= (mColourTimeAdj[i+1]-mColourTimeAdj[i]);

						mProjectorColour.r = ((mColourAdj[i+1].r * projector_time) + (mColourAdj[i].r * (1.0f - projector_time)));
						mProjectorColour.g = ((mColourAdj[i+1].g * projector_time) + (mColourAdj[i].g * (1.0f - projector_time)));
						mProjectorColour.b = ((mColourAdj[i+1].b * projector_time) + (mColourAdj[i].b * (1.0f - projector_time)));
						mProjectorColour.a = ((mColourAdj[i+1].a * projector_time) + (mColourAdj[i].a * (1.0f - projector_time)));
						break;
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------
	void ProjectorElement::_setTextureUnitScale(float uscale, float vscale)
	{
		mTextureUnitUScale = uscale;
		mTextureUnitVScale = vscale;
	}
	//---------------------------------------------------------------------
	void ProjectorElement::setMaterialName(const String &name)
	{
		mMaterialName = name;

		if (mProjector)
		{
			mProjector->setPatchMaterial(mMaterialName);
		}
	}
	//---------------------------------------------------------------------
	Ogre::Radian ProjectorElement::_getFovFromProjectionSize(void)
	{
		/**
		mProjectionSize来计算出所需的fov
                  /|
				/  |
			  /    | mProjectionSize / 2
            /      |
          /a       | 
		/----------- D


		a为fov/2，D为near distance，由图可知，tan( fov / 2 ) * D = mProjectionSize / 2，
		D为100.0f（默认数值），所以fov = atan( mProjectionSize / ( 2 * D ) ) * 2
		*/

        Real nearClip = mProjector->getNearClipDistance();

        if (nearClip > 0.0f)
        {
            return Ogre::Math::ATan( mProjectionSize / (nearClip * 2) ) * 2.0f;
        }
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "the near clip distance of projector must be larger than zero.",
                "ProjectorElement::_getFovFromProjectionSize");
        }
	}
	//---------------------------------------------------------------------
	void ProjectorElement::setProjectionSize(float size)
	{
		mProjectionSize = size;

		if (mProjector)
		{
			mProjector->setFOVy( _getFovFromProjectionSize() );
		}
	}
	//-----------------------------------------------------------------------
	void ProjectorElement::setScaleAdjust(size_t index, ProjectorScaleValue& scale)
	{
		mScaleAdj[index] = scale;
	}
	//-----------------------------------------------------------------------
	ProjectorScaleValue ProjectorElement::getScaleAdjust(size_t index) const
	{
		return mScaleAdj[index];
	}
	//-----------------------------------------------------------------------
	void ProjectorElement::setScaleTimeAdjust(size_t index, Real time)
	{
		mScaleTimeAdj[index] = time;
	}
	//-----------------------------------------------------------------------
	Real ProjectorElement::getScaleTimeAdjust(size_t index) const
	{
		return mScaleTimeAdj[index];
	}
	//-----------------------------------------------------------------------
	void ProjectorElement::setColourAdjust(size_t index, Ogre::ColourValue colour)
	{
		mColourAdj[index] = colour;
	}
	//-----------------------------------------------------------------------
	Ogre::ColourValue ProjectorElement::getColourAdjust(size_t index) const
	{
		return mColourAdj[index];
	}
	//-----------------------------------------------------------------------
	void ProjectorElement::setColourTimeAdjust(size_t index, Real time)
	{
		mColourTimeAdj[index] = time;
	}
	//-----------------------------------------------------------------------
	Real ProjectorElement::getColourTimeAdjust(size_t index) const
	{
		return mColourTimeAdj[index];
	}	
	//-----------------------------------------------------------------------
	void ProjectorElement::setScaleChangeTime(Real time)
	{
		mScaleChangeTime = time;
	}
	//-----------------------------------------------------------------------
	void ProjectorElement::setColourChangeTime(Real time)
	{
		mColourChangeTime = time;
	}
    //-----------------------------------------------------------------------
    void ProjectorElement::setShowFrustum(bool show)
    {
        mShowFrustum = show;

        if (mProjector)
            mProjector->setShowFrustum(mShowFrustum);
    }
    //-----------------------------------------------------------------------
    void ProjectorElement::setVisibleFlag(uint32 flags)
    {
        if (mProjector)
            mProjector->setVisibilityFlags(flags);
    }
    //-----------------------------------------------------------------------
    void ProjectorElement::setNearClipDistance(Real dis)
    {
        mNearClipDistance = dis;

        if (mProjector)
            mProjector->setNearClipDistance(dis);
    }
    //-----------------------------------------------------------------------
    void ProjectorElement::setFarClipDistance(Real dis)
    {
        mFarClipDistance = dis;

        if (mProjector)
            mProjector->setFarClipDistance(dis);
    }
	///////////////////////////////////////////
	String ProjectorElement::CmdMaterialName::doGet(const void* target) const
	{
		return static_cast<const ProjectorElement*>(target)->getMaterialName();
	}
	void ProjectorElement::CmdMaterialName::doSet(void* target, const String& val)
	{
		static_cast<ProjectorElement *>(target)->setMaterialName(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdMaterialName::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setMaterialName(
            static_cast<const ProjectorElement*>(source)->getMaterialName() );
    }
#endif
	//---------------------------------------------------------------------
	String ProjectorElement::CmdProjectionSize::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString( 
			static_cast<const ProjectorElement*>(target)->getProjectionSize() );
	}
	void ProjectorElement::CmdProjectionSize::doSet(void* target, const String& val)
	{
		static_cast<ProjectorElement *>(target)->setProjectionSize( Ogre::StringConverter::parseReal(val) );
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdProjectionSize::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setProjectionSize(
            static_cast<const ProjectorElement*>(source)->getProjectionSize() );
    }
#endif
	//-----------------------------------------------------------------------
	String ProjectorElement::CmdScaleAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const ProjectorElement*>(target)->getScaleAdjust(mIndex) );
	}
	void ProjectorElement::CmdScaleAdjust::doSet(void* target, const String& val)
	{
		static_cast<ProjectorElement*>(target)->setScaleAdjust(mIndex,
			Ogre::StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdScaleAdjust::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setScaleAdjust(mIndex,
            static_cast<const ProjectorElement*>(source)->getScaleAdjust(mIndex) );
    }
#endif
	//-----------------------------------------------------------------------
	String ProjectorElement::CmdScaleTimeAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const ProjectorElement*>(target)->getScaleTimeAdjust(mIndex) );
	}
	void ProjectorElement::CmdScaleTimeAdjust::doSet(void* target, const String& val)
	{
		static_cast<ProjectorElement*>(target)->setScaleTimeAdjust(mIndex,
			Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdScaleTimeAdjust::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setScaleTimeAdjust(mIndex,
            static_cast<const ProjectorElement*>(source)->getScaleTimeAdjust(mIndex) );
    }
#endif
	//-----------------------------------------------------------------------
	String ProjectorElement::CmdColourAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const ProjectorElement*>(target)->getColourAdjust(mIndex) );
	}
	void ProjectorElement::CmdColourAdjust::doSet(void* target, const String& val)
	{
		static_cast<ProjectorElement*>(target)->setColourAdjust(mIndex,
			Ogre::StringConverter::parseColourValue(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdColourAdjust::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setColourAdjust(mIndex,
            static_cast<const ProjectorElement*>(source)->getColourAdjust(mIndex) );
    }
#endif
	//-----------------------------------------------------------------------
	String ProjectorElement::CmdColourTimeAdjust::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const ProjectorElement*>(target)->getColourTimeAdjust(mIndex) );
	}
	void ProjectorElement::CmdColourTimeAdjust::doSet(void* target, const String& val)
	{
		static_cast<ProjectorElement*>(target)->setColourTimeAdjust(mIndex,
			Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdColourTimeAdjust::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setColourTimeAdjust(mIndex,
            static_cast<const ProjectorElement*>(source)->getColourTimeAdjust(mIndex) );
    }
#endif
	//-----------------------------------------------------------------------
	String ProjectorElement::CmdScaleChangeTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const ProjectorElement*>(target)->getScaleChangeTime() );
	}
	void ProjectorElement::CmdScaleChangeTime::doSet(void* target, const String& val)
	{
		static_cast<ProjectorElement*>(target)->setScaleChangeTime(
			Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdScaleChangeTime::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setScaleChangeTime(
            static_cast<const ProjectorElement*>(source)->getScaleChangeTime() );
    }
#endif
	//-----------------------------------------------------------------------
	String ProjectorElement::CmdColourChangeTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const ProjectorElement*>(target)->getColourChangeTime() );
	}
	void ProjectorElement::CmdColourChangeTime::doSet(void* target, const String& val)
	{
		static_cast<ProjectorElement*>(target)->setColourChangeTime(
			Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdColourChangeTime::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setColourChangeTime(
            static_cast<const ProjectorElement*>(source)->getColourChangeTime() );
    }
#endif
    //-----------------------------------------------------------------------
    String ProjectorElement::CmdShowFrustum::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const ProjectorElement*>(target)->getShowFrustum() );
    }
    void ProjectorElement::CmdShowFrustum::doSet(void* target, const String& val)
    {
        static_cast<ProjectorElement*>(target)->setShowFrustum(
            Ogre::StringConverter::parseBool(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdShowFrustum::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setShowFrustum(
            static_cast<const ProjectorElement*>(source)->getShowFrustum() );
    }
#endif
    //-----------------------------------------------------------------------
    String ProjectorElement::CmdNearClipDistance::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const ProjectorElement*>(target)->getNearClipDistance() );
    }
    void ProjectorElement::CmdNearClipDistance::doSet(void* target, const String& val)
    {
        static_cast<ProjectorElement*>(target)->setNearClipDistance(
            Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdNearClipDistance::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setNearClipDistance(
            static_cast<const ProjectorElement*>(source)->getNearClipDistance() );
    }
#endif
    //-----------------------------------------------------------------------
    String ProjectorElement::CmdFarClipDistance::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const ProjectorElement*>(target)->getFarClipDistance() );
    }
    void ProjectorElement::CmdFarClipDistance::doSet(void* target, const String& val)
    {
        static_cast<ProjectorElement*>(target)->setFarClipDistance(
            Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ProjectorElement::CmdFarClipDistance::doCopy(void* target, const void* source)
    {
        static_cast<ProjectorElement *>(target)->setFarClipDistance(
            static_cast<const ProjectorElement*>(source)->getFarClipDistance() );
    }
#endif
}
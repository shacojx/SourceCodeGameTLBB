/********************************************************************
filename:   WXBeamElement.cpp

purpose:    a billboard set effect element,this can use in weapon effect
*********************************************************************/

#include "WXBeamElement.h"
#include "Core/WXSystem.h"
#include "OgreExt/OgreBillboardChain.h"

// ogre header
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace WX	{

	BeamElement::CmdNoiseXMin BeamElement::msNoiseXMinCmd;
	BeamElement::CmdNoiseXMax BeamElement::msNoiseXMaxCmd;
	BeamElement::CmdNoiseYMin BeamElement::msNoiseYMinCmd;
	BeamElement::CmdNoiseYMax BeamElement::msNoiseYMaxCmd;
	BeamElement::CmdNoiseZMin BeamElement::msNoiseZMinCmd;
	BeamElement::CmdNoiseZMax BeamElement::msNoiseZMaxCmd;
	BeamElement::CmdNumElements BeamElement::msNumElementsCmd;
	BeamElement::CmdWidth BeamElement::msWidthCmd;
	BeamElement::CmdColour BeamElement::msColourCmd;
	BeamElement::CmdColourRangeStart BeamElement::msColourRangeStartCmd;
	BeamElement::CmdColourRangeEnd BeamElement::msColourRangeEndCmd;
	BeamElement::CmdMaterial BeamElement::msMaterialCmd;
	BeamElement::CmdFrequency BeamElement::msFrequencyCmd;

	//////////////////////////////////////////////////////////////////////////
	BeamElement::BeamElement(const String &type, System *system) :
	EffectElement(type, system),
	mNumBillboardElements(10),
	mNoiseXMin(0.0f),
	mNoiseXMax(0.0f),
	mNoiseYMin(0.0f),
	mNoiseYMax(0.0f),
	mNoiseZMin(0.0f),
	mNoiseZMax(0.0f),
    mDestPos(Ogre::Vector3::UNIT_Y), // 初始位置
	mWidth(10.0f),
	mBillboardChain(NULL),
	mMaterialName("BaseWhiteNoLighting"),
	mFrequency(20.0f),
	mFrequencyTime(0.0f)
	{
		if ( initDefaults(mElementType) )
		{
			Ogre::ParamDictionary* dict = getParamDictionary();

			// Custom params      
			dict->addParameter(Ogre::ParameterDef("NoiseXMin", 
				"min noise of x axis.",
				Ogre::PT_REAL),&msNoiseXMinCmd);

			dict->addParameter(Ogre::ParameterDef("NoiseXMax", 
				"max noise of x axis.",
				Ogre::PT_REAL),&msNoiseXMaxCmd);

			dict->addParameter(Ogre::ParameterDef("NoiseYMin", 
				"min noise of y axis.",
				Ogre::PT_REAL),&msNoiseYMinCmd);

			dict->addParameter(Ogre::ParameterDef("NoiseYMax", 
				"max noise of y axis.",
				Ogre::PT_REAL),&msNoiseYMaxCmd);

			dict->addParameter(Ogre::ParameterDef("NoiseZMin", 
				"min noise of z axis.",
				Ogre::PT_REAL),&msNoiseZMinCmd);

			dict->addParameter(Ogre::ParameterDef("NoiseZMax", 
				"max noise of z axis.",
				Ogre::PT_REAL),&msNoiseZMaxCmd);

			dict->addParameter(Ogre::ParameterDef("NumElements", 
				"the num of billboard chain elements.",
				Ogre::PT_INT),&msNumElementsCmd);

			dict->addParameter(Ogre::ParameterDef("Width", 
				"width of billboard.",
				Ogre::PT_REAL),&msWidthCmd);

			dict->addParameter(Ogre::ParameterDef("Colour", 
				"The colour of emitted particles.", Ogre::PT_COLOURVALUE),
				&msColourCmd);

			dict->addParameter(Ogre::ParameterDef("ColourRangeStart", 
				"The start of a range of colours to be assigned to emitted particles.", Ogre::PT_COLOURVALUE),
				&msColourRangeStartCmd);

			dict->addParameter(Ogre::ParameterDef("ColourRangeEnd", 
				"The end of a range of colours to be assigned to emitted particles.", Ogre::PT_COLOURVALUE),
				&msColourRangeEndCmd);

			dict->addParameter(Ogre::ParameterDef("Material", 
				"name of beam material.",
				Ogre::PT_STRING),&msMaterialCmd);

			dict->addParameter(Ogre::ParameterDef("Frequency", 
				"frequency of billboard.",
				Ogre::PT_REAL),&msFrequencyCmd);
		}

		mColourRangeStart = mColourRangeEnd = Ogre::ColourValue::White;
	}
	//---------------------------------------------------------------------
	BeamElement::~BeamElement()
	{
		destroyRenderInstance();
	}	
	//---------------------------------------------------------------------
	bool BeamElement::createRenderInstance(void)
	{
		return _createBillboardChains();
	}
	//---------------------------------------------------------------------
	void BeamElement::destroyRenderInstance(void)
	{
		_destroyBillboardChains();

		EffectElement::destroyRenderInstance();
	}
	//---------------------------------------------------------------------
	void BeamElement::updateRenderInstance( Real time, TransformInfo &info )
	{
		if ( mBasicNode )
		{
			_updateBillboardChains(time);
		}
	}
	//---------------------------------------------------------------------
	bool BeamElement::_createBillboardChains(void)
	{
		Ogre::SceneNode* parent = mBasicNode;

		if (parent)
		{			
			mBillboardChain = new Ogre::EffectBillboardChain(mNumBillboardElements);
			mBillboardChain->setNbChainElements(mNumBillboardElements);
			mBillboardChain->setMaterial(mMaterialName);

			mBillboardChain->updateBoundingBox();

            mBillboardChain->setCastShadows(false);

			// Add it to the scene
			parent->attachObject(mBillboardChain);
		
			return true;
		}

		return false;
	}
	//---------------------------------------------------------------------
	void BeamElement::_destroyBillboardChains(void)
	{		
		if (mBillboardChain)
		{
			mBasicNode->detachObject(mBillboardChain);

			delete mBillboardChain;
			mBillboardChain = NULL;
		}		
	}
	//---------------------------------------------------------------------
	void BeamElement::_updateBillboardChains( Real time )
	{
        mCurrentFrequencyTime += time;

		if (mCurrentFrequencyTime > mFrequencyTime || mFrequency == 0.0f)
		{
            // 如果目标位置为初始位置，就不进行更新，防止第一帧出现的位置不正确
            if (mDestPos == Ogre::Vector3::UNIT_Y)
            {
                return;
            }

			Ogre::Vector3 originPos = mBasicNode->_getDerivedPosition();

            // 如果初始点还没初始化（在LogicModel::addEffect中会把这个特效的位置设成一个很低的地方），也return
            if ( originPos.y < -9000 )
            {
                return;
            }

			if (mBillboardChain)
			{
				Ogre::ColourValue colour;
				genColour(colour);

				for (int j = 0; j < mNumBillboardElements; ++j)
				{
					if (j == 0)
					{
						mBillboardChain->setChainElement(j, 
							Ogre::EffectBillboardChainElement(
							Ogre::Vector3::ZERO,
							mWidth,
							0.0f,
							colour)
							);
					}
					else if ( j == (mNumBillboardElements-1) )
					{
						mBillboardChain->setChainElement(j, 
							Ogre::EffectBillboardChainElement(
							mDestPos - originPos,
							mWidth,
							1.0f,
							colour)
							);
					}
					else
					{
						float interpolateValue = (float)j / (float)mNumBillboardElements;
						Ogre::Vector3 pos = (mDestPos - originPos) * interpolateValue;
						pos.x += Ogre::Math::RangeRandom(mNoiseXMin, mNoiseXMax);
						pos.y += Ogre::Math::RangeRandom(mNoiseYMin, mNoiseYMax);
						pos.z += Ogre::Math::RangeRandom(mNoiseZMin, mNoiseZMax);

						mBillboardChain->setChainElement(j, 
							Ogre::EffectBillboardChainElement(
							pos,
							mWidth,
							interpolateValue,
							colour)
							);
					}					
				}

				mBillboardChain->updateBoundingBox();

                // 数据已准备好，可以进行渲染了
                mBillboardChain->isInitData(true);
			}

			mCurrentFrequencyTime = 0.0f;
		}		
	}
	//---------------------------------------------------------------------
	void BeamElement::updateExtraTransformInfos( const TransformInfos &infos )
	{
		mDestPos = infos[0].mPosition;	

	//	_updateBillboardChains();
	}
	//---------------------------------------------------------------------
	void BeamElement::setNumBillboardElements( int num )
	{
		mNumBillboardElements = num;
	
		if (mBillboardChain)
		{
			mBillboardChain->setNbChainElements(mNumBillboardElements);
		}
	}
	//-----------------------------------------------------------------------
	void BeamElement::setColour(const Ogre::ColourValue& colour)
	{
		mColourRangeStart = mColourRangeEnd = colour;
	}
	//-----------------------------------------------------------------------
	void BeamElement::setColour(const Ogre::ColourValue& colourStart, const Ogre::ColourValue& colourEnd)
	{
		mColourRangeStart = colourStart;
		mColourRangeEnd = colourEnd;
	}
	//-----------------------------------------------------------------------
	const Ogre::ColourValue& BeamElement::getColour(void) const
	{
		return mColourRangeStart;
	}
	//-----------------------------------------------------------------------
	const Ogre::ColourValue& BeamElement::getColourRangeStart(void) const
	{
		return mColourRangeStart;
	}
	//-----------------------------------------------------------------------
	const Ogre::ColourValue& BeamElement::getColourRangeEnd(void) const
	{
		return mColourRangeEnd;
	}
	//-----------------------------------------------------------------------
	void BeamElement::setColourRangeStart(const Ogre::ColourValue& val)
	{
		mColourRangeStart = val;
	}
	//-----------------------------------------------------------------------
	void BeamElement::setColourRangeEnd(const Ogre::ColourValue& val )
	{
		mColourRangeEnd = val;
	}
	//-----------------------------------------------------------------------
	void BeamElement::genColour(Ogre::ColourValue& destColour)
	{
		if (mColourRangeStart != mColourRangeEnd)
		{
			// Randomise
			//Real t = Math::UnitRandom();
			destColour.r = mColourRangeStart.r + (Ogre::Math::UnitRandom() * (mColourRangeEnd.r - mColourRangeStart.r));
			destColour.g = mColourRangeStart.g + (Ogre::Math::UnitRandom() * (mColourRangeEnd.g - mColourRangeStart.g));
			destColour.b = mColourRangeStart.b + (Ogre::Math::UnitRandom() * (mColourRangeEnd.b - mColourRangeStart.b));
			destColour.a = mColourRangeStart.a + (Ogre::Math::UnitRandom() * (mColourRangeEnd.a - mColourRangeStart.a));
		}
		else
		{
			destColour = mColourRangeStart;
		}
	}
	//-----------------------------------------------------------------------
	void BeamElement::setMaterialName(const String &matName)
	{
		mMaterialName = matName;

		if (mBillboardChain)
			mBillboardChain->setMaterial(mMaterialName);
	}
	//-----------------------------------------------------------------------
	void BeamElement::setFrequency(float frequency)
	{
		mFrequency = frequency;

		if (mFrequency > 0.0f)
		{
			mFrequencyTime = 1.0 / mFrequency;
		}
		else
		{
			mFrequencyTime = 0.0f;
			mCurrentFrequencyTime = 0.0f;
		}
	}
    //-----------------------------------------------------------------------
    void BeamElement::sleep(void)
    {
        if (mBillboardChain)
        {
            mBillboardChain->isInitData(false);
        }

        EffectElement::sleep();		
    }
    //-----------------------------------------------------------------------
    void BeamElement::setVisibleFlag(uint32 flags)
    {
        if (mBillboardChain)
            mBillboardChain->setVisibilityFlags(flags);
    }
	//////////////////////////////////////////////////////////////////////////
	String BeamElement::CmdNoiseXMin::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement *>(target)->mNoiseXMin );
	}
	void BeamElement::CmdNoiseXMin::doSet(void* target, const String& val)
	{
		static_cast<BeamElement *>(target)->mNoiseXMin = 
			Ogre::StringConverter::parseReal(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdNoiseXMin::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->mNoiseXMin = 
            static_cast<const BeamElement *>(source)->mNoiseXMin;
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdNoiseXMax::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement *>(target)->mNoiseXMax );
	}
	void BeamElement::CmdNoiseXMax::doSet(void* target, const String& val)
	{
		static_cast<BeamElement *>(target)->mNoiseXMax = 
			Ogre::StringConverter::parseReal(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdNoiseXMax::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->mNoiseXMax = 
            static_cast<const BeamElement *>(source)->mNoiseXMax;
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdNoiseYMin::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement *>(target)->mNoiseYMin );
	}
	void BeamElement::CmdNoiseYMin::doSet(void* target, const String& val)
	{
		static_cast<BeamElement *>(target)->mNoiseYMin = 
			Ogre::StringConverter::parseReal(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdNoiseYMin::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->mNoiseYMin = 
            static_cast<const BeamElement *>(source)->mNoiseYMin;
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdNoiseYMax::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement *>(target)->mNoiseYMax );
	}
	void BeamElement::CmdNoiseYMax::doSet(void* target, const String& val)
	{
		static_cast<BeamElement *>(target)->mNoiseYMax = 
			Ogre::StringConverter::parseReal(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdNoiseYMax::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->mNoiseYMax = 
            static_cast<const BeamElement *>(source)->mNoiseYMax;
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdNoiseZMin::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement *>(target)->mNoiseZMin );
	}
	void BeamElement::CmdNoiseZMin::doSet(void* target, const String& val)
	{
		static_cast<BeamElement *>(target)->mNoiseZMin = 
			Ogre::StringConverter::parseReal(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdNoiseZMin::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->mNoiseZMin = 
            static_cast<const BeamElement *>(source)->mNoiseZMin;
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdNoiseZMax::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement *>(target)->mNoiseZMax );
	}
	void BeamElement::CmdNoiseZMax::doSet(void* target, const String& val)
	{
		static_cast<BeamElement *>(target)->mNoiseZMax = 
			Ogre::StringConverter::parseReal(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdNoiseZMax::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->mNoiseZMax = 
            static_cast<const BeamElement *>(source)->mNoiseZMax;
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdNumElements::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement *>(target)->getNumBillboardElements() );
	}
	void BeamElement::CmdNumElements::doSet(void* target, const String& val)
	{
		static_cast<BeamElement *>(target)->setNumBillboardElements( Ogre::StringConverter::parseInt(val) );
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdNumElements::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->setNumBillboardElements(
            static_cast<const BeamElement *>(source)->getNumBillboardElements() );
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdWidth::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement *>(target)->mWidth );
	}
	void BeamElement::CmdWidth::doSet(void* target, const String& val)
	{
		static_cast<BeamElement *>(target)->mWidth = Ogre::StringConverter::parseReal(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdWidth::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->mWidth = 
            static_cast<const BeamElement *>(source)->mWidth;
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdColour::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement*>(target)->getColour() );
	}
	void BeamElement::CmdColour::doSet(void* target, const String& val)
	{
		static_cast<BeamElement*>(target)->setColour(Ogre::StringConverter::parseColourValue(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdColour::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->setColour(
            static_cast<const BeamElement *>(source)->getColour() );
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdColourRangeStart::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement*>(target)->getColourRangeStart() );
	}
	void BeamElement::CmdColourRangeStart::doSet(void* target, const String& val)
	{
		static_cast<BeamElement*>(target)->setColourRangeStart(Ogre::StringConverter::parseColourValue(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdColourRangeStart::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->setColourRangeStart(
            static_cast<const BeamElement *>(source)->getColourRangeStart() );
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdColourRangeEnd::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement*>(target)->getColourRangeEnd() );
	}
	void BeamElement::CmdColourRangeEnd::doSet(void* target, const String& val)
	{
		static_cast<BeamElement*>(target)->setColourRangeEnd(Ogre::StringConverter::parseColourValue(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdColourRangeEnd::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->setColourRangeEnd( 
            static_cast<const BeamElement *>(source)->getColourRangeEnd() );
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdMaterial::doGet(const void* target) const
	{
		return static_cast<const BeamElement *>(target)->getMaterialName();
	}
	void BeamElement::CmdMaterial::doSet(void* target, const String& val)
	{
		static_cast<BeamElement *>(target)->setMaterialName(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdMaterial::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->setMaterialName( 
            static_cast<const BeamElement *>(source)->getMaterialName() );
    }
#endif
	//-----------------------------------------------------------------------
	String BeamElement::CmdFrequency::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BeamElement *>(target)->getFrequency() );
	}
	void BeamElement::CmdFrequency::doSet(void* target, const String& val)
	{
		static_cast<BeamElement *>(target)->setFrequency( Ogre::StringConverter::parseReal(val) );
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BeamElement::CmdFrequency::doCopy(void* target, const void* source)
    {
        static_cast<BeamElement *>(target)->setFrequency( 
            static_cast<const BeamElement *>(source)->getFrequency() );
    }
#endif
}
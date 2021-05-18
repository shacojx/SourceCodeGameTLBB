/********************************************************************
filename:   WXBillboardSetElement.cpp

purpose:    a billboard set effect element,this can use in weapon effect
*********************************************************************/

#include "WXBillboardSetElement.h"
#include "Core/WXSystem.h"

// ogre header
#include <OgreSceneManager.h>
#include <OgreBillboard.h>
#include <OgreBillboardSet.h>
#include <OgreStringConverter.h>
#include <OgreMath.h>
#include <OgreLogManager.h>

namespace WX	{

	BillboardSetElement::CmdBillboardColour BillboardSetElement::msBillboardColourCmd;
    BillboardSetElement::CmdBillboardScale BillboardSetElement::msBillboardScaleCmd[MAX_STAGES];
    BillboardSetElement::CmdScaleIntervalTime BillboardSetElement::msScaleIntervalTimeCmd;
    BillboardSetElement::CmdBillboardAlpha BillboardSetElement::msBillboardAlphaCmd;
	BillboardSetElement::CmdBillboardCount BillboardSetElement::msBillboardCountCmd;
	BillboardSetElement::CmdBillboardMaterial BillboardSetElement::msBillboardMaterialCmd;
	BillboardSetElement::CmdBillboardInterval BillboardSetElement::msBillboardIntervalCmd;
	BillboardSetElement::CmdBillboardHeight BillboardSetElement::msBillboardHeightCmd;
	BillboardSetElement::CmdBillboardWidth BillboardSetElement::msBillboardWidthCmd;
    BillboardSetElement::CmdStacks BillboardSetElement::msStacksCmd;
    BillboardSetElement::CmdSlices BillboardSetElement::msSlicesCmd;
    BillboardSetElement::CmdTexIntervalTime BillboardSetElement::msTexIntervalTimeCmd;

	//////////////////////////////////////////////////////////////////////////
	BillboardSetElement::BillboardSetElement(const String &type, System *system) :
	EffectElement(type, system),
	mBillboardSet(NULL),
    mBillboardColour(Ogre::ColourValue::White),
    mScaleIntervalTime(0.1f),
    mSizeChanged(false),
	mBillboardCount(5),
	mBillboardMaterial("BaseWhiteNoLighting"),
	mBillboardInterval(10.0f),
	mBillboardWidth(100.0f),
	mBillboardHeight(100.0f),
    mAlphaValue(1.0f),
    mColourChanged(false),
    mStacks(1),
    mSlices(1),
    mTexIntervalTime(0.1f),
    mCurrentTexIntervalTime(0.0f)
	{
        for (int i=0; i<MAX_STAGES; ++i)
        {
            mBillboardScale[i] = Ogre::Vector3(1.0f, 1.0f, 1.0f);
        }

		if ( initDefaults(mElementType) )
		{
			Ogre::ParamDictionary* dict = getParamDictionary();

			// Custom params      

            dict->addParameter(Ogre::ParameterDef("BillboardColour", 
                "BillboardColour",
                Ogre::PT_COLOURVALUE), &msBillboardColourCmd);

            dict->addParameter(Ogre::ParameterDef("BillboardAlpha", 
                "Billboard Alpha Value of first point and second point.",
                Ogre::PT_REAL),&msBillboardAlphaCmd);

			dict->addParameter(Ogre::ParameterDef("BillboardCount", 
				"the life time of the vertex.",
				Ogre::PT_INT),&msBillboardCountCmd);

			dict->addParameter(Ogre::ParameterDef("Material", 
				"alpha factor of head point.",
				Ogre::PT_STRING),&msBillboardMaterialCmd);

			dict->addParameter(Ogre::ParameterDef("BillboardInterval", 
				"alpha factor of tail point.",
				Ogre::PT_REAL),&msBillboardIntervalCmd);

			dict->addParameter(Ogre::ParameterDef("BillboardHeight", 
				"alpha factor of tail point.",
				Ogre::PT_REAL),&msBillboardHeightCmd);

			dict->addParameter(Ogre::ParameterDef("BillboardWidth", 
				"alpha factor of tail point.",
				Ogre::PT_REAL),&msBillboardWidthCmd); 

            for (int i=0;i<MAX_STAGES;i++)
            {
                msBillboardScaleCmd[i].mIndex	= i;

                Ogre::StringUtil::StrStreamType stage;
                stage << i;
                String	colour_title	= String("BillboardScale") + stage.str();
                String	colour_descr	= String("Stage ") + stage.str() +
                    String(" BillboardScale of first point and second point.");

                dict->addParameter(Ogre::ParameterDef(colour_title, colour_descr,
                    Ogre::PT_VECTOR3), &msBillboardScaleCmd[i]);
            }

            dict->addParameter(Ogre::ParameterDef("ScaleIntervalTime", 
                "the interval time of changing scale.",
                Ogre::PT_REAL),&msScaleIntervalTimeCmd);   

            dict->addParameter(Ogre::ParameterDef("stacks", 
                "the stacks of texture coordinates.",
                Ogre::PT_UNSIGNED_LONG),
                &msStacksCmd);

            dict->addParameter(Ogre::ParameterDef("slices",
                "the slices of texture coordinates.",
                Ogre::PT_UNSIGNED_LONG),
                &msSlicesCmd);

            dict->addParameter(Ogre::ParameterDef("TexIntervalTime", 
                "the interval time of changing texture index.",
                Ogre::PT_REAL),&msTexIntervalTimeCmd);   

		}    
	}
	//---------------------------------------------------------------------
	BillboardSetElement::~BillboardSetElement()
	{
		destroyRenderInstance();
	}	
	//---------------------------------------------------------------------
	bool BillboardSetElement::createRenderInstance(void)
	{
		return _createBillboardSet();
	}
	//---------------------------------------------------------------------
	void BillboardSetElement::destroyRenderInstance(void)
	{
		_destroyBillboardSet();

		EffectElement::destroyRenderInstance();
	}
	//---------------------------------------------------------------------
	bool BillboardSetElement::_createBillboardSet(void)
	{
		Ogre::SceneNode* parent = mBasicNode;

		if (parent)
		{
			mBillboardSet = mSystem->getSceneManager()->createBillboardSet( parent->getName() );
			mBillboardSet->setMaterialName(mBillboardMaterial);

            if ( (mStacks > 1) || (mSlices > 1) )
                mBillboardSet->setTextureStacksAndSlices(mStacks, mSlices);

            mBillboardSet->setCastShadows(false);

            mBillboardSet->setVisible(mVisible);

			parent->attachObject(mBillboardSet);

			for ( int i=0; i<mBillboardCount; ++i )
			{
				Ogre::Billboard *billboard = mBillboardSet->createBillboard(0, i*mBillboardInterval, 0, mBillboardColour * mAlphaValue);
				assert (billboard);
				billboard->setDimensions(mBillboardWidth, mBillboardHeight);
			}

            _resetBillboardStartTexIndex();
            _resetBillboardStartScaleTime();

			return true;
		}

		return false;
	}
	//---------------------------------------------------------------------
	void BillboardSetElement::_destroyBillboardSet(void)
	{		
		if (mBillboardSet)
		{
			Ogre::SceneNode* parent = mBillboardSet->getParentSceneNode();
			assert(parent);
			Ogre::SceneManager* creator = parent->getCreator();
			assert(creator);

			parent->detachObject(mBillboardSet->getName());

			creator->destroyBillboardSet(mBillboardSet);
		}
	}
    //---------------------------------------------------------------------
    void BillboardSetElement::_resetBillboardStartTexIndex(void)
    {
        assert (mBillboardSet);

        for ( int i=0; i<mBillboardCount; ++i )
        {
            uint16 texIndex = Ogre::Math::RangeRandom(0, mStacks * mSlices - 1);

            mBillboardSet->getBillboard(i)->setTexcoordIndex(texIndex);
        }
    }
    //---------------------------------------------------------------------
    void BillboardSetElement::updateRenderInstance( Real time, TransformInfo &info )
    {
        if (mBillboardSet)
        {
            bool updateTexIndex = false;

            if (mSlices > 1 || mStacks > 1)
            {
                mCurrentTexIntervalTime += time;

                if (mCurrentTexIntervalTime >= mTexIntervalTime)
                {
                    updateTexIndex = true;
                    mCurrentTexIntervalTime = 0.0f;
                }            
            }

            for ( int i = 0; i < mBillboardSet->getNumBillboards(); ++i )
            {
                Ogre::Billboard* billboard = mBillboardSet->getBillboard(i);

                if (updateTexIndex)
                {
                    uint16 texIndex = billboard->getTexcoordIndex();
                    ++texIndex;

                    if (texIndex == mStacks * mSlices)
                        texIndex = 0;

                    billboard->setTexcoordIndex(texIndex);
                }
                
                if (mColourChanged)
                    billboard->setColour(mBillboardColour * mAlphaValue);

                if (mSizeChanged)
                {
                    _CurrentScaleInfo& info = mCurrentScaleInfos[i];
                    info.mCurrentScaleIntervalTime += time;

                    float colourPersent = fmod( info.mCurrentScaleIntervalTime, mScaleIntervalTime );

                    if (info.mCurrentScaleIntervalTime > mScaleIntervalTime)
                    {
                        info.mCurrentScaleIntervalTime  = colourPersent;

                        ++(info.mCurrentIndex);

                        if (info.mCurrentIndex == MAX_STAGES)
                            info.mCurrentIndex = 0;
                    }

                    Ogre::Vector3 resultColour;

                    Real rate = colourPersent / mScaleIntervalTime;

                    int incIndex = info.mCurrentIndex + 1;
                    if (incIndex == MAX_STAGES)
                        incIndex = 0;

                    resultColour = mBillboardScale[info.mCurrentIndex] * (1-rate) + mBillboardScale[incIndex] * rate;

                    billboard->setDimensions(mBillboardWidth * resultColour.x,
                        mBillboardHeight * resultColour.y);
                }
            }

            if (mColourChanged)
                mColourChanged = false;
        }
    }
    //---------------------------------------------------------------------
    void BillboardSetElement::setBillboardColour(const Ogre::ColourValue &colour)
    {
        if (mBillboardColour != colour)
        {
            mBillboardColour = colour;
            mColourChanged = true;
        }
    }
	//---------------------------------------------------------------------
    void BillboardSetElement::setBillboardScale(size_t index, const BillboardSetElement::ScaleValue &scale)
	{
        if (mBillboardScale[index] != scale)
        {
            mBillboardScale[index] = scale;
            mSizeChanged = true;
        }		
	}
	//---------------------------------------------------------------------
	void BillboardSetElement::setBillboardCount(int count)
	{
		mBillboardCount = count;

		if (mBillboardSet)
		{
			mBillboardSet->clear();

			for ( int i=0; i<mBillboardCount; ++i )
			{
				Ogre::Billboard *billboard = mBillboardSet->createBillboard(0, i*mBillboardInterval, 0, mBillboardColour * mAlphaValue);
				assert (billboard);
				billboard->setDimensions(mBillboardWidth, mBillboardHeight);
			}

            _resetBillboardStartTexIndex();
            _resetBillboardStartScaleTime();
		}
	}
	//---------------------------------------------------------------------
	void BillboardSetElement::setBillboardMaterial(const String &material)
	{
		mBillboardMaterial = material;

		if (mBillboardSet)
		{
			mBillboardSet->setMaterialName(mBillboardMaterial);
		}
	}
	//---------------------------------------------------------------------
	void BillboardSetElement::setBillboardInterval(Real interval)
	{
		mBillboardInterval = interval;

		if (mBillboardSet)
		{
			for ( int i = 0; i < mBillboardSet->getNumBillboards(); ++i )
			{
				mBillboardSet->getBillboard(i)->setPosition(0, i*mBillboardInterval, 0);
			}

            _resetBillboardStartScaleTime();
		}
	}
	//---------------------------------------------------------------------
	void BillboardSetElement::setBillboardWidth(Real width)
	{
		mBillboardWidth = width;

		if (mBillboardSet)
		{
			for ( int i = 0; i < mBillboardSet->getNumBillboards(); ++i )
			{
				mBillboardSet->getBillboard(i)->setDimensions(mBillboardWidth, mBillboardHeight);
			}
		}
	}
	//---------------------------------------------------------------------
	void BillboardSetElement::setBillboardHeight(Real height)
	{
		mBillboardHeight = height;

		if (mBillboardSet)
		{
			for ( int i = 0; i < mBillboardSet->getNumBillboards(); ++i )
			{
				mBillboardSet->getBillboard(i)->setDimensions(mBillboardWidth, mBillboardHeight);
			}
		}
	}
    //---------------------------------------------------------------------
    void BillboardSetElement::setStacks(ulong stacks)
    {
        if (stacks < 1)
            mStacks = 1;
        else
            mStacks = stacks;

        if (mBillboardSet)
        {
            mBillboardSet->setTextureStacksAndSlices((uchar)mStacks, (uchar)mSlices);
            _resetBillboardStartTexIndex();
        }
    }
    //-----------------------------------------------------------------------
    void BillboardSetElement::setSlices(ulong slices)
    {
        if (slices < 1)
            mSlices = 1;
        else
            mSlices = slices;

        if (mBillboardSet)
        {
            mBillboardSet->setTextureStacksAndSlices((uchar)mStacks, (uchar)mSlices);
            _resetBillboardStartTexIndex();
        }
    }
    //-----------------------------------------------------------------------
    void BillboardSetElement::_resetBillboardStartScaleTime(void)
    {
        mCurrentScaleInfos.resize(mBillboardCount);

        for (int i=0; i<mBillboardCount; ++i)
        {
            mCurrentScaleInfos[i].mCurrentScaleIntervalTime = Ogre::Math::RangeRandom(0.0f, mScaleIntervalTime);
            mCurrentScaleInfos[i].mCurrentIndex = (int)(Ogre::Math::RangeRandom(0,MAX_STAGES));
        }
    }
    //-----------------------------------------------------------------------
    void BillboardSetElement::setVisibleFlag(uint32 flags)
    {
        if (mBillboardSet)
            mBillboardSet->setVisibilityFlags(flags);
    }
	//////////////////////////////////////////////////////////////////////////
	String BillboardSetElement::CmdBillboardColour::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BillboardSetElement *>(target)->getBillboardColour() );
	}
	void BillboardSetElement::CmdBillboardColour::doSet(void* target, const String& val)
	{
		static_cast<BillboardSetElement *>(target)->setBillboardColour(
			Ogre::StringConverter::parseColourValue(val) );
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdBillboardColour::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setBillboardColour(
            static_cast<const BillboardSetElement *>(source)->getBillboardColour() );
    }
#endif
    //-----------------------------------------------------------------------
    String BillboardSetElement::CmdBillboardScale::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const BillboardSetElement *>(target)->getBillboardScale(mIndex) );
    }
    void BillboardSetElement::CmdBillboardScale::doSet(void* target, const String& val)
    {
        static_cast<BillboardSetElement *>(target)->setBillboardScale(mIndex,
            Ogre::StringConverter::parseVector3(val) );
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdBillboardScale::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setBillboardScale(mIndex,
            static_cast<const BillboardSetElement *>(source)->getBillboardScale(mIndex) );
    }
#endif
    //-----------------------------------------------------------------------
    String BillboardSetElement::CmdScaleIntervalTime::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const BillboardSetElement *>(target)->getScaleIntervalTime() );
    }
    void BillboardSetElement::CmdScaleIntervalTime::doSet(void* target, const String& val)
    {
        static_cast<BillboardSetElement *>(target)->setScaleIntervalTime(
            Ogre::StringConverter::parseReal(val) );
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdScaleIntervalTime::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setScaleIntervalTime(
            static_cast<const BillboardSetElement *>(source)->getScaleIntervalTime() );
    }
#endif
    //-----------------------------------------------------------------------
    String BillboardSetElement::CmdBillboardAlpha::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const BillboardSetElement *>(target)->getBillboardAlpha() );
    }
    void BillboardSetElement::CmdBillboardAlpha::doSet(void* target, const String& val)
    {
        static_cast<BillboardSetElement *>(target)->setBillboardAlpha(
            Ogre::StringConverter::parseReal(val) );
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdBillboardAlpha::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setBillboardAlpha(
            static_cast<const BillboardSetElement *>(source)->getBillboardAlpha() );
    }
#endif
    //-----------------------------------------------------------------------
	String BillboardSetElement::CmdBillboardCount::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BillboardSetElement *>(target)->getBillboardCount() );
	}
	void BillboardSetElement::CmdBillboardCount::doSet(void* target, const String& val)
	{
		static_cast<BillboardSetElement *>(target)->setBillboardCount( 
			Ogre::StringConverter::parseInt(val) );
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdBillboardCount::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setBillboardCount(
            static_cast<const BillboardSetElement *>(source)->getBillboardCount() );
    }
#endif
    //-----------------------------------------------------------------------
	String BillboardSetElement::CmdBillboardMaterial::doGet(const void* target) const
	{
		return static_cast<const BillboardSetElement *>(target)->getBillboardMaterial();
	}
	void BillboardSetElement::CmdBillboardMaterial::doSet(void* target, const String& val)
	{
		static_cast<BillboardSetElement *>(target)->setBillboardMaterial(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdBillboardMaterial::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setBillboardMaterial(
            static_cast<const BillboardSetElement *>(source)->getBillboardMaterial() );
    }
#endif
    //-----------------------------------------------------------------------
	String BillboardSetElement::CmdBillboardInterval::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BillboardSetElement *>(target)->getBillboardInterval() );
	}
	void BillboardSetElement::CmdBillboardInterval::doSet(void* target, const String& val)
	{
		static_cast<BillboardSetElement *>(target)->setBillboardInterval(
			Ogre::StringConverter::parseReal(val) );
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdBillboardInterval::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setBillboardInterval(
            static_cast<const BillboardSetElement *>(source)->getBillboardInterval() );
    }
#endif
    //-----------------------------------------------------------------------
	String BillboardSetElement::CmdBillboardHeight::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BillboardSetElement *>(target)->getBillboardHeight() );
	}
	void BillboardSetElement::CmdBillboardHeight::doSet(void* target, const String& val)
	{
		static_cast<BillboardSetElement *>(target)->setBillboardHeight(
			Ogre::StringConverter::parseReal(val) );
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdBillboardHeight::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setBillboardHeight(
            static_cast<const BillboardSetElement *>(source)->getBillboardHeight() );
    }
#endif
    //-----------------------------------------------------------------------
	String BillboardSetElement::CmdBillboardWidth::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BillboardSetElement *>(target)->getBillboardWidth() );
	}
	void BillboardSetElement::CmdBillboardWidth::doSet(void* target, const String& val)
	{
		static_cast<BillboardSetElement *>(target)->setBillboardWidth(
			Ogre::StringConverter::parseReal(val) );
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdBillboardWidth::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setBillboardWidth(
            static_cast<const BillboardSetElement *>(source)->getBillboardWidth() );
    }
#endif
    //-----------------------------------------------------------------------
    String BillboardSetElement::CmdStacks::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const BillboardSetElement*>(target)->getStacks() );
    }
    void BillboardSetElement::CmdStacks::doSet(void* target, const String& val)
    {
        static_cast<BillboardSetElement*>(target)->setStacks(
            Ogre::StringConverter::parseUnsignedLong(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdStacks::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setStacks(
            static_cast<const BillboardSetElement *>(source)->getStacks() );
    }
#endif
    //-----------------------------------------------------------------------
    String BillboardSetElement::CmdSlices::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const BillboardSetElement*>(target)->getSlices() );
    }
    void BillboardSetElement::CmdSlices::doSet(void* target, const String& val)
    {
        static_cast<BillboardSetElement*>(target)->setSlices(
            Ogre::StringConverter::parseUnsignedLong(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdSlices::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setSlices(
            static_cast<const BillboardSetElement *>(source)->getSlices() );
    }
#endif
    //-----------------------------------------------------------------------
    String BillboardSetElement::CmdTexIntervalTime::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const BillboardSetElement *>(target)->getTexIntervalTime() );
    }
    void BillboardSetElement::CmdTexIntervalTime::doSet(void* target, const String& val)
    {
        static_cast<BillboardSetElement *>(target)->setTexIntervalTime(
            Ogre::StringConverter::parseReal(val) );
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void BillboardSetElement::CmdTexIntervalTime::doCopy(void* target, const void* source)
    {
        static_cast<BillboardSetElement *>(target)->setTexIntervalTime(
            static_cast<const BillboardSetElement *>(source)->getTexIntervalTime() );
    }
#endif
}
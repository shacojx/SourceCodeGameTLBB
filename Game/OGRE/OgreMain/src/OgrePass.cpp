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

#include "OgrePass.h"
#include "OgreTechnique.h"
#include "OgreException.h"
#include "OgreGpuProgramUsage.h"
#include "OgreTextureUnitState.h"
#include "OgreStringConverter.h"

namespace Ogre {

    //-----------------------------------------------------------------------------
	Pass::PassSet Pass::msDirtyHashList;
    Pass::PassSet Pass::msPassGraveyard;
    //-----------------------------------------------------------------------------
	Pass::Pass(Technique* parent, unsigned short index)
        : mParent(parent), mIndex(index), mPassIterationCount(0)
    {
        // Default to white ambient & diffuse, no specular / emissive
	    mAmbient = mDiffuse = ColourValue::White;
	    mSpecular = mEmissive = ColourValue::Black;
	    mShininess = 0;
        mPointSize = 1.0f;
		mPointMinSize = 0.0f;
		mPointMaxSize = 0.0f;
		mPointSpritesEnabled = false;
		mPointAttenuationEnabled = false;
		mPointAttenuationCoeffs[0] = 1.0f;
		mPointAttenuationCoeffs[1] = mPointAttenuationCoeffs[2] = 0.0f;
        mTracking = TVC_NONE;
        mHash = 0;

        // By default, don't override the scene's fog settings
        mFogOverride = false;
        mFogMode = FOG_NONE;
        mFogColour = ColourValue::White;
        mFogStart = 0.0;
        mFogEnd = 1.0;
        mFogDensity = 0.001;

	    // Default blending (overwrite)
	    mSourceBlendFactor = SBF_ONE;
	    mDestBlendFactor = SBF_ZERO;

	    mDepthCheck = true;
	    mDepthWrite = true;
        mColourWrite = true;
	    mDepthFunc = CMPF_LESS_EQUAL;
        mDepthBias = 0;
		mAlphaRejectFunc = CMPF_ALWAYS_PASS;
		mAlphaRejectVal = 0;
	    mCullMode = CULL_CLOCKWISE;
	    mManualCullMode = MANUAL_CULL_BACK;
	    mLightingEnabled = true;
        mMaxSimultaneousLights = OGRE_MAX_SIMULTANEOUS_LIGHTS;
		mIteratePerLight = false;
        mRunOnlyForOneLightType = true;
        mOnlyLightType = Light::LT_POINT;
	    mShadeOptions = SO_GOURAUD;
		mPolygonMode = PM_SOLID;

		mVertexProgramUsage = NULL;
        mShadowCasterVertexProgramUsage = NULL;
        mShadowReceiverVertexProgramUsage = NULL;
		mShadowReceiverFragmentProgramUsage = NULL;
		mFragmentProgramUsage = NULL;

        mQueuedForDeletion = false;

        // default name to index
        mName = StringConverter::toString(mIndex);

        _dirtyHash();
   }

    //-----------------------------------------------------------------------------
	Pass::Pass(Technique *parent, unsigned short index, const Pass& oth)
        :mParent(parent), mIndex(index), mQueuedForDeletion(false), mPassIterationCount(0)
    {
        *this = oth;
        mParent = parent;
        mIndex = index;
        mQueuedForDeletion = false;
        _dirtyHash();
    }
    //-----------------------------------------------------------------------------
    Pass::~Pass()
    {

    }
    //-----------------------------------------------------------------------------
    Pass& Pass::operator=(const Pass& oth)
    {
        mName = oth.mName;
        mHash = oth.mHash;
	    mAmbient = oth.mAmbient;
        mDiffuse = oth.mDiffuse;
	    mSpecular = oth.mSpecular;
        mEmissive = oth.mEmissive;
	    mShininess = oth.mShininess;
        mTracking = oth.mTracking;

        // Copy fog parameters
        mFogOverride = oth.mFogOverride;
        mFogMode = oth.mFogMode;
        mFogColour = oth.mFogColour;
        mFogStart = oth.mFogStart;
        mFogEnd = oth.mFogEnd;
        mFogDensity = oth.mFogDensity;

	    // Default blending (overwrite)
	    mSourceBlendFactor = oth.mSourceBlendFactor;
	    mDestBlendFactor = oth.mDestBlendFactor;

	    mDepthCheck = oth.mDepthCheck;
	    mDepthWrite = oth.mDepthWrite;
		mAlphaRejectFunc = oth.mAlphaRejectFunc;
		mAlphaRejectVal = oth.mAlphaRejectVal;
        mColourWrite = oth.mColourWrite;
	    mDepthFunc = oth.mDepthFunc;
        mDepthBias = oth.mDepthBias;
	    mCullMode = oth.mCullMode;
	    mManualCullMode = oth.mManualCullMode;
	    mLightingEnabled = oth.mLightingEnabled;
        mMaxSimultaneousLights = oth.mMaxSimultaneousLights;
		mIteratePerLight = oth.mIteratePerLight;
        mRunOnlyForOneLightType = oth.mRunOnlyForOneLightType;
        mOnlyLightType = oth.mOnlyLightType;
	    mShadeOptions = oth.mShadeOptions;
		mPolygonMode = oth.mPolygonMode;
        mPassIterationCount = oth.mPassIterationCount;
		mPointSize = oth.mPointSize;
		mPointMinSize = oth.mPointMinSize;
		mPointMaxSize = oth.mPointMaxSize;
		mPointSpritesEnabled = oth.mPointSpritesEnabled;
		mPointAttenuationEnabled = oth.mPointAttenuationEnabled;
		memcpy(mPointAttenuationCoeffs, oth.mPointAttenuationCoeffs, sizeof(Real)*3);


		if (oth.mVertexProgramUsage)
		{
			mVertexProgramUsage = new GpuProgramUsage(*(oth.mVertexProgramUsage));
		}
		else
		{
		    mVertexProgramUsage = NULL;
		}
        if (oth.mShadowCasterVertexProgramUsage)
        {
            mShadowCasterVertexProgramUsage = new GpuProgramUsage(*(oth.mShadowCasterVertexProgramUsage));
        }
        else
        {
            mShadowCasterVertexProgramUsage = NULL;
        }
        if (oth.mShadowReceiverVertexProgramUsage)
        {
            mShadowReceiverVertexProgramUsage = new GpuProgramUsage(*(oth.mShadowReceiverVertexProgramUsage));
        }
        else
        {
            mShadowReceiverVertexProgramUsage = NULL;
        }
		if (oth.mFragmentProgramUsage)
		{
		    mFragmentProgramUsage = new GpuProgramUsage(*(oth.mFragmentProgramUsage));
        }
        else
        {
		    mFragmentProgramUsage = NULL;
        }
		if (oth.mShadowReceiverFragmentProgramUsage)
		{
			mShadowReceiverFragmentProgramUsage = new GpuProgramUsage(*(oth.mShadowReceiverFragmentProgramUsage));
		}
		else
		{
			mShadowReceiverFragmentProgramUsage = NULL;
		}

		TextureUnitStates::const_iterator i, iend;

        // Clear texture units but doesn't notify need recompilation in the case
        // we are cloning, The parent material will take care of this.
        iend = mTextureUnitStates.end();
        for (i = mTextureUnitStates.begin(); i != iend; ++i)
        {
            delete *i;
        }

        mTextureUnitStates.clear();

		// Copy texture units
		iend = oth.mTextureUnitStates.end();
		for (i = oth.mTextureUnitStates.begin(); i != iend; ++i)
		{
			TextureUnitState* t = new TextureUnitState(this, *(*i));
			mTextureUnitStates.push_back(t);
		}

        _dirtyHash();

		return *this;
    }
    //-----------------------------------------------------------------------
    void Pass::setName(const String& name)
    {
        mName = name;
    }
    //-----------------------------------------------------------------------
    void Pass::setPointSize(Real ps)
    {
	    mPointSize = ps;
    }
    //-----------------------------------------------------------------------
	void Pass::setPointSpritesEnabled(bool enabled)
	{
		mPointSpritesEnabled = enabled;
	}
    //-----------------------------------------------------------------------
	bool Pass::getPointSpritesEnabled(void) const
	{
		return mPointSpritesEnabled;
	}
    //-----------------------------------------------------------------------
	void Pass::setPointAttenuation(bool enabled,
		Real constant, Real linear, Real quadratic)
	{
		mPointAttenuationEnabled = enabled;
		mPointAttenuationCoeffs[0] = constant;
		mPointAttenuationCoeffs[1] = linear;
		mPointAttenuationCoeffs[2] = quadratic;
	}
    //-----------------------------------------------------------------------
	bool Pass::isPointAttenuationEnabled(void) const
	{
		return mPointAttenuationEnabled;
	}
    //-----------------------------------------------------------------------
	Real Pass::getPointAttenuationConstant(void) const
	{
		return mPointAttenuationCoeffs[0];
	}
    //-----------------------------------------------------------------------
	Real Pass::getPointAttenuationLinear(void) const
	{
		return mPointAttenuationCoeffs[1];
	}
    //-----------------------------------------------------------------------
	Real Pass::getPointAttenuationQuadratic(void) const
	{
		return mPointAttenuationCoeffs[2];
	}
    //-----------------------------------------------------------------------
	void Pass::setPointMinSize(Real min)
	{
		mPointMinSize = min;
	}
    //-----------------------------------------------------------------------
	Real Pass::getPointMinSize(void) const
	{
		return mPointMinSize;
	}
    //-----------------------------------------------------------------------
	void Pass::setPointMaxSize(Real max)
	{
		mPointMaxSize = max;
	}
    //-----------------------------------------------------------------------
	Real Pass::getPointMaxSize(void) const
	{
		return mPointMaxSize;
	}
    //-----------------------------------------------------------------------
    void Pass::setAmbient(Real red, Real green, Real blue)
    {
	    mAmbient.r = red;
	    mAmbient.g = green;
	    mAmbient.b = blue;

    }
    //-----------------------------------------------------------------------
    void Pass::setAmbient(const ColourValue& ambient)
    {
	    mAmbient = ambient;
    }
    //-----------------------------------------------------------------------
    void Pass::setDiffuse(Real red, Real green, Real blue, Real alpha)
    {
	    mDiffuse.r = red;
	    mDiffuse.g = green;
	    mDiffuse.b = blue;
		mDiffuse.a = alpha;
    }
    //-----------------------------------------------------------------------
    void Pass::setDiffuse(const ColourValue& diffuse)
    {
	    mDiffuse = diffuse;
    }
    //-----------------------------------------------------------------------
    void Pass::setSpecular(Real red, Real green, Real blue, Real alpha)
    {
	    mSpecular.r = red;
	    mSpecular.g = green;
	    mSpecular.b = blue;
		mSpecular.a = alpha;
    }
    //-----------------------------------------------------------------------
    void Pass::setSpecular(const ColourValue& specular)
    {
	    mSpecular = specular;
    }
    //-----------------------------------------------------------------------
    void Pass::setShininess(Real val)
    {
	    mShininess = val;
    }
    //-----------------------------------------------------------------------
    void Pass::setSelfIllumination(Real red, Real green, Real blue)
    {
	    mEmissive.r = red;
	    mEmissive.g = green;
	    mEmissive.b = blue;

    }
    //-----------------------------------------------------------------------
    void Pass::setSelfIllumination(const ColourValue& selfIllum)
    {
	    mEmissive = selfIllum;
    }
    //-----------------------------------------------------------------------
    void Pass::setVertexColourTracking(TrackVertexColourType tracking)
    {
        mTracking = tracking;
    }
    //-----------------------------------------------------------------------
    Real Pass::getPointSize(void) const
    {
	    return mPointSize;
    }
    //-----------------------------------------------------------------------
    const ColourValue& Pass::getAmbient(void) const
    {
	    return mAmbient;
    }
    //-----------------------------------------------------------------------
    const ColourValue& Pass::getDiffuse(void) const
    {
	    return mDiffuse;
    }
    //-----------------------------------------------------------------------
    const ColourValue& Pass::getSpecular(void) const
    {
	    return mSpecular;
    }
    //-----------------------------------------------------------------------
    const ColourValue& Pass::getSelfIllumination(void) const
    {
	    return mEmissive;
    }
    //-----------------------------------------------------------------------
    Real Pass::getShininess(void) const
    {
	    return mShininess;
    }
    //-----------------------------------------------------------------------
    TrackVertexColourType Pass::getVertexColourTracking(void) const
    {
        return mTracking;
    }
    //-----------------------------------------------------------------------
    TextureUnitState* Pass::createTextureUnitState(void)
    {
        TextureUnitState *t = new TextureUnitState(this);
        addTextureUnitState(t);
	    return t;
    }
    //-----------------------------------------------------------------------
    TextureUnitState* Pass::createTextureUnitState(
        const String& textureName, unsigned short texCoordSet)
    {
        TextureUnitState *t = new TextureUnitState(this);
	    t->setTextureName(textureName);
	    t->setTextureCoordSet(texCoordSet);
        addTextureUnitState(t);
	    return t;
    }
    //-----------------------------------------------------------------------
	void Pass::addTextureUnitState(TextureUnitState* state)
	{
        assert(state && "state is 0 in Pass::addTextureUnitState()");
        if (state)
        {
            // only attach TUS to pass if TUS does not belong to another pass
            if ((state->getParent() == 0) || (state->getParent() == this))
            {
		        mTextureUnitStates.push_back(state);
				// Notify state
				state->_notifyParent(this);
                // if texture unit state name is empty then give it a default name based on its index
                if (state->getName().empty())
                {
                    // its the last entry in the container so its index is size - 1
                    size_t idx = mTextureUnitStates.size() - 1;
                    state->setName( StringConverter::toString(idx) );
                    /** since the name was never set and a default one has been made, clear the alias name
                     so that when the texture unit name is set by the user, the alias name will be set to
                     that name
                    */
                    state->setTextureNameAlias(StringUtil::BLANK);
                }
                // Needs recompilation
                mParent->_notifyNeedsRecompile();
                _dirtyHash();
            }
            else
            {
			    OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "TextureUnitState already attached to another pass",
				    "Pass:addTextureUnitState");

            }
        }
	}
    //-----------------------------------------------------------------------
    TextureUnitState* Pass::getTextureUnitState(unsigned short index)
    {
        assert (index < mTextureUnitStates.size() && "Index out of bounds");
	    return mTextureUnitStates[index];
    }
    //-----------------------------------------------------------------------------
    TextureUnitState* Pass::getTextureUnitState(const String& name)
    {
        TextureUnitStates::iterator i    = mTextureUnitStates.begin();
        TextureUnitStates::iterator iend = mTextureUnitStates.end();
        TextureUnitState* foundTUS = 0;

        // iterate through TUS Container to find a match
        while (i != iend)
        {
            if ( (*i)->getName() == name )
            {
                foundTUS = (*i);
                break;
            }

            ++i;
        }

        return foundTUS;
    }
	//-----------------------------------------------------------------------
	const TextureUnitState* Pass::getTextureUnitState(unsigned short index) const
	{
		assert (index < mTextureUnitStates.size() && "Index out of bounds");
		return mTextureUnitStates[index];
	}
	//-----------------------------------------------------------------------------
	const TextureUnitState* Pass::getTextureUnitState(const String& name) const
	{
		TextureUnitStates::const_iterator i    = mTextureUnitStates.begin();
		TextureUnitStates::const_iterator iend = mTextureUnitStates.end();
		const TextureUnitState* foundTUS = 0;

		// iterate through TUS Container to find a match
		while (i != iend)
		{
			if ( (*i)->getName() == name )
			{
				foundTUS = (*i);
				break;
			}

			++i;
		}

		return foundTUS;
	}

    //-----------------------------------------------------------------------
    unsigned short Pass::getTextureUnitStateIndex(const TextureUnitState* state)
    {
        assert(state && "state is 0 in Pass::addTextureUnitState()");
        unsigned short idx = 0;

        // only find index for state attached to this pass
        if (state->getParent() == this)
        {
            // iterate through TUS container and find matching pointer to state
            TextureUnitStates::iterator i    = mTextureUnitStates.begin();
            TextureUnitStates::iterator iend = mTextureUnitStates.end();
            while (i != iend)
            {
                if ( (*i) == state )
                {
                    // calculate index
                    idx = static_cast<unsigned short>(i - mTextureUnitStates.begin());
                    break;
                }

                ++i;
            }
        }
        else
        {
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "TextureUnitState is not attached to this pass",
				"Pass:getTextureUnitStateIndex");
        }

        return idx;
    }

    //-----------------------------------------------------------------------
    Pass::TextureUnitStateIterator
        Pass::getTextureUnitStateIterator(void)
    {
        return TextureUnitStateIterator(mTextureUnitStates.begin(), mTextureUnitStates.end());
    }
	//-----------------------------------------------------------------------
	Pass::ConstTextureUnitStateIterator
		Pass::getTextureUnitStateIterator(void) const
	{
		return ConstTextureUnitStateIterator(mTextureUnitStates.begin(), mTextureUnitStates.end());
	}
    //-----------------------------------------------------------------------
    void Pass::removeTextureUnitState(unsigned short index)
    {
        assert (index < mTextureUnitStates.size() && "Index out of bounds");

        TextureUnitStates::iterator i = mTextureUnitStates.begin() + index;
        delete *i;
	    mTextureUnitStates.erase(i);
        if (!mQueuedForDeletion)
        {
            // Needs recompilation
            mParent->_notifyNeedsRecompile();
        }
        _dirtyHash();
    }
    //-----------------------------------------------------------------------
    void Pass::removeAllTextureUnitStates(void)
    {
        TextureUnitStates::iterator i, iend;
        iend = mTextureUnitStates.end();
        for (i = mTextureUnitStates.begin(); i != iend; ++i)
        {
            delete *i;
        }
        mTextureUnitStates.clear();
        if (!mQueuedForDeletion)
        {
            // Needs recompilation
            mParent->_notifyNeedsRecompile();
        }
        _dirtyHash();
    }
    //-----------------------------------------------------------------------
    void Pass::setSceneBlending(SceneBlendType sbt)
    {
	    // Turn predefined type into blending factors
	    switch (sbt)
	    {
	    case SBT_TRANSPARENT_ALPHA:
		    setSceneBlending(SBF_SOURCE_ALPHA, SBF_ONE_MINUS_SOURCE_ALPHA);
		    break;
	    case SBT_TRANSPARENT_COLOUR:
		    setSceneBlending(SBF_SOURCE_COLOUR, SBF_ONE_MINUS_SOURCE_COLOUR);
		    break;
		case SBT_MODULATE:
			setSceneBlending(SBF_DEST_COLOUR, SBF_ZERO);
			break;
	    case SBT_ADD:
		    setSceneBlending(SBF_ONE, SBF_ONE);
		    break;
        case SBT_REPLACE:
            setSceneBlending(SBF_ONE, SBF_ZERO);
            break;
	    // TODO: more
	    }

    }
    //-----------------------------------------------------------------------
    void Pass::setSceneBlending(SceneBlendFactor sourceFactor, SceneBlendFactor destFactor)
    {
	    mSourceBlendFactor = sourceFactor;
	    mDestBlendFactor = destFactor;
    }
    //-----------------------------------------------------------------------
    SceneBlendFactor Pass::getSourceBlendFactor(void) const
    {
	    return mSourceBlendFactor;
    }
    //-----------------------------------------------------------------------
    SceneBlendFactor Pass::getDestBlendFactor(void) const
    {
	    return mDestBlendFactor;
    }
    //-----------------------------------------------------------------------
    bool Pass::isTransparent(void) const
    {
		// Transparent if any of the destination colour is taken into account
		if (mDestBlendFactor == SBF_ZERO &&
			mSourceBlendFactor != SBF_DEST_COLOUR &&
			mSourceBlendFactor != SBF_ONE_MINUS_DEST_COLOUR &&
			mSourceBlendFactor != SBF_DEST_ALPHA &&
			mSourceBlendFactor != SBF_ONE_MINUS_DEST_ALPHA)
		{
		    return false;
		}
	    else
		{
		    return true;
		}
    }
    //-----------------------------------------------------------------------
    void Pass::setDepthCheckEnabled(bool enabled)
    {
	    mDepthCheck = enabled;
    }
    //-----------------------------------------------------------------------
    bool Pass::getDepthCheckEnabled(void) const
    {
	    return mDepthCheck;
    }
    //-----------------------------------------------------------------------
    void Pass::setDepthWriteEnabled(bool enabled)
    {
	    mDepthWrite = enabled;
    }
    //-----------------------------------------------------------------------
    bool Pass::getDepthWriteEnabled(void) const
    {
	    return mDepthWrite;
    }
    //-----------------------------------------------------------------------
    void Pass::setDepthFunction( CompareFunction func)
    {
	    mDepthFunc = func;
    }
    //-----------------------------------------------------------------------
    CompareFunction Pass::getDepthFunction(void) const
    {
	    return mDepthFunc;
    }
	//-----------------------------------------------------------------------
	void Pass::setAlphaRejectSettings(CompareFunction func, unsigned char value)
	{
		mAlphaRejectFunc = func;
		mAlphaRejectVal = value;
	}
	//-----------------------------------------------------------------------
	void Pass::setAlphaRejectFunction(CompareFunction func)
	{
		mAlphaRejectFunc = func;
	}
	//-----------------------------------------------------------------------
	void Pass::setAlphaRejectValue(unsigned char val)
	{
		mAlphaRejectVal = val;
	}
    //-----------------------------------------------------------------------
	void Pass::setColourWriteEnabled(bool enabled)
	{
		mColourWrite = enabled;
	}
    //-----------------------------------------------------------------------
	bool Pass::getColourWriteEnabled(void) const
	{
		return mColourWrite;
	}
    //-----------------------------------------------------------------------
    void Pass::setCullingMode( CullingMode mode)
    {
	    mCullMode = mode;
    }
    //-----------------------------------------------------------------------
    CullingMode Pass::getCullingMode(void) const
    {
	    return mCullMode;
    }
    //-----------------------------------------------------------------------
    void Pass::setLightingEnabled(bool enabled)
    {
	    mLightingEnabled = enabled;
    }
    //-----------------------------------------------------------------------
    bool Pass::getLightingEnabled(void) const
    {
	    return mLightingEnabled;
    }
    //-----------------------------------------------------------------------
    void Pass::setMaxSimultaneousLights(unsigned short maxLights)
    {
        mMaxSimultaneousLights = maxLights;
    }
    //-----------------------------------------------------------------------
    unsigned short Pass::getMaxSimultaneousLights(void) const
    {
        return mMaxSimultaneousLights;
    }
    //-----------------------------------------------------------------------
    void Pass::setIteratePerLight(bool enabled,
            bool onlyForOneLightType, Light::LightTypes lightType)
    {
        mIteratePerLight = enabled;
        mRunOnlyForOneLightType = onlyForOneLightType;
        mOnlyLightType = lightType;
    }
    //-----------------------------------------------------------------------
    void Pass::setShadingMode(ShadeOptions mode)
    {
	    mShadeOptions = mode;
    }
    //-----------------------------------------------------------------------
    ShadeOptions Pass::getShadingMode(void) const
    {
	    return mShadeOptions;
    }
	//-----------------------------------------------------------------------
	void Pass::setPolygonMode(PolygonMode mode)
	{
		mPolygonMode = mode;
	}
	//-----------------------------------------------------------------------
	PolygonMode Pass::getPolygonMode(void) const
	{
		return mPolygonMode;
	}
    //-----------------------------------------------------------------------
    void Pass::setManualCullingMode(ManualCullingMode mode)
    {
	    mManualCullMode = mode;
    }
    //-----------------------------------------------------------------------
    ManualCullingMode Pass::getManualCullingMode(void) const
    {
	    return mManualCullMode;
    }
    //-----------------------------------------------------------------------
    void Pass::setFog(bool overrideScene, FogMode mode, const ColourValue& colour, Real density, Real start, Real end)
    {
	    mFogOverride = overrideScene;
	    if (overrideScene)
	    {
		    mFogMode = mode;
		    mFogColour = colour;
		    mFogStart = start;
		    mFogEnd = end;
		    mFogDensity = density;
	    }
    }
    //-----------------------------------------------------------------------
    bool Pass::getFogOverride(void) const
    {
	    return mFogOverride;
    }
    //-----------------------------------------------------------------------
    FogMode Pass::getFogMode(void) const
    {
	    return mFogMode;
    }
    //-----------------------------------------------------------------------
    const ColourValue& Pass::getFogColour(void) const
    {
	    return mFogColour;
    }
    //-----------------------------------------------------------------------
    Real Pass::getFogStart(void) const
    {
	    return mFogStart;
    }
    //-----------------------------------------------------------------------
    Real Pass::getFogEnd(void) const
    {
	    return mFogEnd;
    }
    //-----------------------------------------------------------------------
    Real Pass::getFogDensity(void) const
    {
	    return mFogDensity;
    }
    //-----------------------------------------------------------------------
    void Pass::setDepthBias(ushort bias)
    {
        assert(bias <= 16 && "Depth bias must be between 0 and 16");
        mDepthBias = bias;
    }
    //-----------------------------------------------------------------------
    ushort Pass::getDepthBias(void) const
    {
        return mDepthBias;
    }
    //-----------------------------------------------------------------------
	Pass* Pass::_split(unsigned short numUnits)
	{
		if (mVertexProgramUsage || mFragmentProgramUsage)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Programmable passes cannot be "
				"automatically split, define a fallback technique instead.",
				"Pass:_split");
		}

		if (mTextureUnitStates.size() > numUnits)
		{
			size_t start = mTextureUnitStates.size() - numUnits;

			Pass* newPass = mParent->createPass();

			TextureUnitStates::iterator istart, i, iend;
			iend = mTextureUnitStates.end();
			i = istart = mTextureUnitStates.begin() + start;
			// Set the new pass to fallback using scene blend
			newPass->setSceneBlending(
				(*i)->getColourBlendFallbackSrc(), (*i)->getColourBlendFallbackDest());
			// Fixup the texture unit 0   of new pass   blending method   to replace
			// all colour and alpha   with texture without adjustment, because we
			// assume it's detail texture.
			(*i)->setColourOperationEx(LBX_SOURCE1,   LBS_TEXTURE, LBS_CURRENT);
			(*i)->setAlphaOperation(LBX_SOURCE1, LBS_TEXTURE, LBS_CURRENT);

			// Add all the other texture unit states
			for (; i != iend; ++i)
			{
				// detach from parent first
				(*i)->_notifyParent(0);
				newPass->addTextureUnitState(*i);
			}
			// Now remove texture units from this Pass, we don't need to delete since they've
			// been transferred
			mTextureUnitStates.erase(istart, iend);
			_dirtyHash();
			return newPass;
		}
		return NULL;
	}
	//-----------------------------------------------------------------------------
	void Pass::_notifyIndex(unsigned short index)
	{
		if (mIndex != index)
		{
			mIndex = index;
			_dirtyHash();
		}
	}
    //-----------------------------------------------------------------------
	void Pass::_load(void)
	{
		// We assume the Technique only calls this when the material is being
		// loaded

		// Load each TextureUnitState
		TextureUnitStates::iterator i, iend;
		iend = mTextureUnitStates.end();
		for (i = mTextureUnitStates.begin(); i != iend; ++i)
		{
			(*i)->_load();
		}

		// Load programs
		if (mVertexProgramUsage)
		{
			// Load vertex program
            mVertexProgramUsage->_load();
        }
        if (mShadowCasterVertexProgramUsage)
        {
            // Load vertex program
            mShadowCasterVertexProgramUsage->_load();
        }
        if (mShadowReceiverVertexProgramUsage)
        {
            // Load vertex program
            mShadowReceiverVertexProgramUsage->_load();
        }

        if (mFragmentProgramUsage)
        {
			// Load fragment program
            mFragmentProgramUsage->_load();
		}
		if (mShadowReceiverFragmentProgramUsage)
		{
			// Load Fragment program
			mShadowReceiverFragmentProgramUsage->_load();
		}

	}
    //-----------------------------------------------------------------------
	void Pass::_unload(void)
	{
		// Unload each TextureUnitState
		TextureUnitStates::iterator i, iend;
		iend = mTextureUnitStates.end();
		for (i = mTextureUnitStates.begin(); i != iend; ++i)
		{
			(*i)->_unload();
		}

		// Unload programs
		if (mVertexProgramUsage)
		{
			// TODO
		}
        if (mFragmentProgramUsage)
        {
            // TODO
        }
	}
    //-----------------------------------------------------------------------
	void Pass::setVertexProgram(const String& name, bool resetParams)
	{
        // Turn off vertex program if name blank
        if (name.empty())
        {
            if (mVertexProgramUsage) delete mVertexProgramUsage;
            mVertexProgramUsage = NULL;
        }
        else
        {
            if (!mVertexProgramUsage)
            {
                mVertexProgramUsage = new GpuProgramUsage(GPT_VERTEX_PROGRAM);
            }
		    mVertexProgramUsage->setProgramName(name, resetParams);
        }
        // Needs recompilation
        mParent->_notifyNeedsRecompile();
	}
    //-----------------------------------------------------------------------
	void Pass::setVertexProgramParameters(GpuProgramParametersSharedPtr params)
	{
		if (!mVertexProgramUsage)
        {
            OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS,
                "This pass does not have a vertex program assigned!",
                "Pass::setVertexProgramParameters");
        }
		mVertexProgramUsage->setParameters(params);
	}
    //-----------------------------------------------------------------------
	void Pass::setFragmentProgram(const String& name, bool resetParams)
	{
        // Turn off fragment program if name blank
        if (name.empty())
        {
            if (mFragmentProgramUsage) delete mFragmentProgramUsage;
            mFragmentProgramUsage = NULL;
        }
        else
        {
            if (!mFragmentProgramUsage)
            {
                mFragmentProgramUsage = new GpuProgramUsage(GPT_FRAGMENT_PROGRAM);
            }
		    mFragmentProgramUsage->setProgramName(name, resetParams);
        }
        // Needs recompilation
        mParent->_notifyNeedsRecompile();
	}
    //-----------------------------------------------------------------------
	void Pass::setFragmentProgramParameters(GpuProgramParametersSharedPtr params)
	{
		if (!mFragmentProgramUsage)
        {
            OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS,
                "This pass does not have a fragment program assigned!",
                "Pass::setFragmentProgramParameters");
        }
		mFragmentProgramUsage->setParameters(params);
	}
	//-----------------------------------------------------------------------
	const String& Pass::getVertexProgramName(void) const
	{
        if (!mVertexProgramUsage)
            return StringUtil::BLANK;
        else
		    return mVertexProgramUsage->getProgramName();
	}
	//-----------------------------------------------------------------------
	GpuProgramParametersSharedPtr Pass::getVertexProgramParameters(void) const
	{
		if (!mVertexProgramUsage)
        {
            OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS,
                "This pass does not have a vertex program assigned!",
                "Pass::getVertexProgramParameters");
        }
		return mVertexProgramUsage->getParameters();
	}
	//-----------------------------------------------------------------------
	const GpuProgramPtr& Pass::getVertexProgram(void) const
	{
		return mVertexProgramUsage->getProgram();
	}
	//-----------------------------------------------------------------------
	const String& Pass::getFragmentProgramName(void) const
	{
        if (!mFragmentProgramUsage)
            return StringUtil::BLANK;
        else
    		return mFragmentProgramUsage->getProgramName();
	}
	//-----------------------------------------------------------------------
	GpuProgramParametersSharedPtr Pass::getFragmentProgramParameters(void) const
	{
		return mFragmentProgramUsage->getParameters();
	}
	//-----------------------------------------------------------------------
	const GpuProgramPtr& Pass::getFragmentProgram(void) const
	{
		return mFragmentProgramUsage->getProgram();
	}
	//-----------------------------------------------------------------------
    bool Pass::isLoaded(void) const
    {
        return mParent->isLoaded();
    }
	//-----------------------------------------------------------------------
    void Pass::_recalculateHash(void)
    {
        /* Hash format is 32-bit, divided as follows (high to low bits)
           bits   purpose
            4     Pass index (i.e. max 16 passes!)
           14     Hashed texture name from unit 0
           14     Hashed texture name from unit 1

           Note that at the moment we don't sort on the 3rd texture unit plus
           on the assumption that these are less frequently used; sorting on
           the first 2 gives us the most benefit for now.
       */
        _StringHash H;
        mHash = (mIndex << 28);
        size_t c = getNumTextureUnitStates();

        if (c && !mTextureUnitStates[0]->isBlank())
            mHash += (H(mTextureUnitStates[0]->getTextureName()) % (1 << 14)) << 14;
        if (c > 1 && !mTextureUnitStates[1]->isBlank())
            mHash += (H(mTextureUnitStates[1]->getTextureName()) % (1 << 14));
    }
    //-----------------------------------------------------------------------
	void Pass::_dirtyHash(void)
	{
		// Mark this hash as for follow up
		msDirtyHashList.insert(this);
	}
    //-----------------------------------------------------------------------
    void Pass::_notifyNeedsRecompile(void)
    {
        mParent->_notifyNeedsRecompile();
    }
    //-----------------------------------------------------------------------
    void Pass::setTextureFiltering(TextureFilterOptions filterType)
    {
        TextureUnitStates::iterator i, iend;
        iend = mTextureUnitStates.end();
        for (i = mTextureUnitStates.begin(); i != iend; ++i)
        {
            (*i)->setTextureFiltering(filterType);
        }
    }
    // --------------------------------------------------------------------
    void Pass::setTextureAnisotropy(unsigned int maxAniso)
    {
        TextureUnitStates::iterator i, iend;
        iend = mTextureUnitStates.end();
        for (i = mTextureUnitStates.begin(); i != iend; ++i)
        {
            (*i)->setTextureAnisotropy(maxAniso);
        }
    }
    //-----------------------------------------------------------------------
    void Pass::_updateAutoParamsNoLights(const AutoParamDataSource& source) const
    {
        if (hasVertexProgram())
        {
            // Update vertex program auto params
            mVertexProgramUsage->getParameters()->_updateAutoParamsNoLights(source);
        }

        if (hasFragmentProgram())
        {
            // Update fragment program auto params
            mFragmentProgramUsage->getParameters()->_updateAutoParamsNoLights(source);
        }
    }
    //-----------------------------------------------------------------------
    void Pass::_updateAutoParamsLightsOnly(const AutoParamDataSource& source) const
    {
        if (hasVertexProgram())
        {
            // Update vertex program auto params
            mVertexProgramUsage->getParameters()->_updateAutoParamsLightsOnly(source);
        }

        if (hasFragmentProgram())
        {
            // Update fragment program auto params
            mFragmentProgramUsage->getParameters()->_updateAutoParamsLightsOnly(source);
        }
    }
    //-----------------------------------------------------------------------
    void Pass::processPendingPassUpdates(void)
    {
        // Delete items in the graveyard
        PassSet::iterator i, iend;
        iend = msPassGraveyard.end();
        for (i = msPassGraveyard.begin(); i != iend; ++i)
        {
            delete *i;
        }
        msPassGraveyard.clear();

        // The dirty ones will have been removed from the groups above using the old hash now
        iend = msDirtyHashList.end();
        for (i = msDirtyHashList.begin(); i != iend; ++i)
        {
            Pass* p = *i;
            p->_recalculateHash();
        }
        // Clear the dirty list
        msDirtyHashList.clear();
    }
    //-----------------------------------------------------------------------
    void Pass::queueForDeletion(void)
    {
        mQueuedForDeletion = true;

        removeAllTextureUnitStates();
        if (mVertexProgramUsage)
        {
            delete mVertexProgramUsage;
            mVertexProgramUsage = 0;
        }
        if (mShadowCasterVertexProgramUsage)
        {
            delete mShadowCasterVertexProgramUsage;
            mShadowCasterVertexProgramUsage = 0;
        }
        if (mShadowReceiverVertexProgramUsage)
        {
            delete mShadowReceiverVertexProgramUsage;
            mShadowReceiverVertexProgramUsage = 0;
        }
        if (mFragmentProgramUsage)
        {
            delete mFragmentProgramUsage;
            mFragmentProgramUsage = 0;
        }
		if (mShadowReceiverFragmentProgramUsage)
		{
			delete mShadowReceiverFragmentProgramUsage;
			mShadowReceiverFragmentProgramUsage = 0;
		}
        // remove from dirty list, if there
        msDirtyHashList.erase(this);

        msPassGraveyard.insert(this);
    }
    //-----------------------------------------------------------------------
    bool Pass::isAmbientOnly(void) const
    {
        // treat as ambient if lighting is off, or colour write is off,
        // or all non-ambient (& emissive) colours are black
        // NB a vertex program could override this, but passes using vertex
        // programs are expected to indicate they are ambient only by
        // setting the state so it matches one of the conditions above, even
        // though this state is not used in rendering.
        return (!mLightingEnabled || !mColourWrite ||
            (mDiffuse == ColourValue::Black &&
             mSpecular == ColourValue::Black));
    }
    //-----------------------------------------------------------------------
    void Pass::setShadowCasterVertexProgram(const String& name)
    {
        // Turn off vertex program if name blank
        if (name.empty())
        {
            if (mShadowCasterVertexProgramUsage) delete mShadowCasterVertexProgramUsage;
            mShadowCasterVertexProgramUsage = NULL;
        }
        else
        {
            if (!mShadowCasterVertexProgramUsage)
            {
                mShadowCasterVertexProgramUsage = new GpuProgramUsage(GPT_VERTEX_PROGRAM);
            }
            mShadowCasterVertexProgramUsage->setProgramName(name);
        }
        // Needs recompilation
        mParent->_notifyNeedsRecompile();
    }
    //-----------------------------------------------------------------------
    void Pass::setShadowCasterVertexProgramParameters(GpuProgramParametersSharedPtr params)
    {
        if (!mShadowCasterVertexProgramUsage)
        {
            OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS,
                "This pass does not have a shadow caster vertex program assigned!",
                "Pass::setShadowCasterVertexProgramParameters");
        }
        mShadowCasterVertexProgramUsage->setParameters(params);
    }
    //-----------------------------------------------------------------------
    const String& Pass::getShadowCasterVertexProgramName(void) const
    {
        if (!mShadowCasterVertexProgramUsage)
            return StringUtil::BLANK;
        else
            return mShadowCasterVertexProgramUsage->getProgramName();
    }
    //-----------------------------------------------------------------------
    GpuProgramParametersSharedPtr Pass::getShadowCasterVertexProgramParameters(void) const
    {
        if (!mShadowCasterVertexProgramUsage)
        {
            OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS,
                "This pass does not have a shadow caster vertex program assigned!",
                "Pass::getShadowCasterVertexProgramParameters");
        }
        return mShadowCasterVertexProgramUsage->getParameters();
    }
    //-----------------------------------------------------------------------
    const GpuProgramPtr& Pass::getShadowCasterVertexProgram(void) const
    {
        return mShadowCasterVertexProgramUsage->getProgram();
    }
    //-----------------------------------------------------------------------
    void Pass::setShadowReceiverVertexProgram(const String& name)
    {
        // Turn off vertex program if name blank
        if (name.empty())
        {
            if (mShadowReceiverVertexProgramUsage) delete mShadowReceiverVertexProgramUsage;
            mShadowReceiverVertexProgramUsage = NULL;
        }
        else
        {
            if (!mShadowReceiverVertexProgramUsage)
            {
                mShadowReceiverVertexProgramUsage = new GpuProgramUsage(GPT_VERTEX_PROGRAM);
            }
            mShadowReceiverVertexProgramUsage->setProgramName(name);
        }
        // Needs recompilation
        mParent->_notifyNeedsRecompile();
    }
    //-----------------------------------------------------------------------
    void Pass::setShadowReceiverVertexProgramParameters(GpuProgramParametersSharedPtr params)
    {
        if (!mShadowReceiverVertexProgramUsage)
        {
            OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS,
                "This pass does not have a shadow receiver vertex program assigned!",
                "Pass::setShadowReceiverVertexProgramParameters");
        }
        mShadowReceiverVertexProgramUsage->setParameters(params);
    }
    //-----------------------------------------------------------------------
    const String& Pass::getShadowReceiverVertexProgramName(void) const
    {
        if (!mShadowReceiverVertexProgramUsage)
            return StringUtil::BLANK;
        else
            return mShadowReceiverVertexProgramUsage->getProgramName();
    }
    //-----------------------------------------------------------------------
    GpuProgramParametersSharedPtr Pass::getShadowReceiverVertexProgramParameters(void) const
    {
        if (!mShadowReceiverVertexProgramUsage)
        {
            OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS,
                "This pass does not have a shadow receiver vertex program assigned!",
                "Pass::getShadowReceiverVertexProgramParameters");
        }
        return mShadowReceiverVertexProgramUsage->getParameters();
    }
    //-----------------------------------------------------------------------
    const GpuProgramPtr& Pass::getShadowReceiverVertexProgram(void) const
    {
        return mShadowReceiverVertexProgramUsage->getProgram();
    }
	//-----------------------------------------------------------------------
	void Pass::setShadowReceiverFragmentProgram(const String& name)
	{
		// Turn off Fragment program if name blank
		if (name.empty())
		{
			if (mShadowReceiverFragmentProgramUsage) delete mShadowReceiverFragmentProgramUsage;
			mShadowReceiverFragmentProgramUsage = NULL;
		}
		else
		{
			if (!mShadowReceiverFragmentProgramUsage)
			{
				mShadowReceiverFragmentProgramUsage = new GpuProgramUsage(GPT_FRAGMENT_PROGRAM);
			}
			mShadowReceiverFragmentProgramUsage->setProgramName(name);
		}
		// Needs recompilation
		mParent->_notifyNeedsRecompile();
	}
	//-----------------------------------------------------------------------
	void Pass::setShadowReceiverFragmentProgramParameters(GpuProgramParametersSharedPtr params)
	{
		if (!mShadowReceiverFragmentProgramUsage)
		{
			OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS,
				"This pass does not have a shadow receiver fragment program assigned!",
				"Pass::setShadowReceiverFragmentProgramParameters");
		}
		mShadowReceiverFragmentProgramUsage->setParameters(params);
	}
	//-----------------------------------------------------------------------
	const String& Pass::getShadowReceiverFragmentProgramName(void) const
	{
		if (!mShadowReceiverFragmentProgramUsage)
			return StringUtil::BLANK;
		else
			return mShadowReceiverFragmentProgramUsage->getProgramName();
	}
	//-----------------------------------------------------------------------
	GpuProgramParametersSharedPtr Pass::getShadowReceiverFragmentProgramParameters(void) const
	{
		if (!mShadowReceiverFragmentProgramUsage)
		{
			OGRE_EXCEPT (Exception::ERR_INVALIDPARAMS,
				"This pass does not have a shadow receiver fragment program assigned!",
				"Pass::getShadowReceiverFragmentProgramParameters");
		}
		return mShadowReceiverFragmentProgramUsage->getParameters();
	}
	//-----------------------------------------------------------------------
	const GpuProgramPtr& Pass::getShadowReceiverFragmentProgram(void) const
	{
		return mShadowReceiverFragmentProgramUsage->getProgram();
	}
    //-----------------------------------------------------------------------
	const String& Pass::getResourceGroup(void) const
	{
		return mParent->getResourceGroup();
	}

    //-----------------------------------------------------------------------
    bool Pass::applyTextureAliases(const AliasTextureNamePairList& aliasList, const bool apply) const
    {
        // iterate through each texture unit state and apply the texture alias if it applies
        TextureUnitStates::const_iterator i, iend;
        iend = mTextureUnitStates.end();
        bool testResult = false;

        for (i = mTextureUnitStates.begin(); i != iend; ++i)
        {
            if ((*i)->applyTextureAliases(aliasList, apply))
                testResult = true;
        }

        return testResult;

    }


}

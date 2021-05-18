/********************************************************************
filename:   WXAnimationRibbon.cpp

purpose:    the ribbon that attach to the bone or a locator.
*********************************************************************/

#include "WXAnimationRibbon.h"
#include "WXEffectManager.h"

#include "Core/WXSystem.h"

#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include <OgreRenderSystem.h>
#include <OgreRoot.h>
#include <OgreHardwareBufferManager.h>

namespace WX	{

	AnimationRibbon::CmdAttachTime	AnimationRibbon::msAttachTimeCmd;
	AnimationRibbon::CmdAttachPoint		AnimationRibbon::msAttachPointCmd;
	AnimationRibbon::CmdOffsetPos	 AnimationRibbon::msOffsetPosCmd;
	AnimationRibbon::CmdOffsetRotation	AnimationRibbon::msOffsetRotationCmd;

	AnimationRibbon::CmdPointCount AnimationRibbon::msPointCountCmd;
	AnimationRibbon::CmdStartColour AnimationRibbon::msStartColour;
	AnimationRibbon::CmdEndColour AnimationRibbon::msEndColour;
	AnimationRibbon::CmdStartAlpha AnimationRibbon::msStartAlpha;
	AnimationRibbon::CmdEndAlpha AnimationRibbon::msEndAlpha;
    AnimationRibbon::CmdFadeInTime AnimationRibbon::msFadeInTimeCmd;
    AnimationRibbon::CmdFadeOutTime AnimationRibbon::msFadeOutTimeCmd;
	AnimationRibbon::CmdMaterial AnimationRibbon::msMaterial;
	AnimationRibbon::CmdRibbonWidth AnimationRibbon::msRibbonWidthCmd;
	AnimationRibbon::CmdLifeTime AnimationRibbon::msLifeTimeCmd;
	AnimationRibbon::CmdUseInter AnimationRibbon::msUseInterCmd;
	AnimationRibbon::CmdInterTimes AnimationRibbon::msInterTimesCmd;
    AnimationRibbon::CmdVertexLifeTime AnimationRibbon::msVertexLifeTimeCmd;
	//////////////////////////////////////////////////////////////////////////

	AnimationRibbon::AnimationRibbon( System *system, unsigned short pointCount, float lifeTime, float ribbonWidth,
		const Ogre::ColourValue &startColour, const Ogre::ColourValue &endColour,
		float startAlpha, float endAlpha, float vertexLifeTime ) :
		mPointCount(pointCount), 
        mLifeTime(lifeTime), 
        mRibbonWidth(ribbonWidth),
		mStartColour(startColour), 
        mEndColour(endColour),
		mStartAlphaValue(startAlpha), 
        mEndAlphaValue(endAlpha), 
        mVertexLifeTime(vertexLifeTime),
        mCreateInstance(false), 
        mCurrentLifeTime(0.0f),
		mAttachTime(0.0f), 
        mAttachPoint(""), 
        mBasicNode(NULL), 
        mSystem(system),
		mOffsetPos(Ogre::Vector3::ZERO), 
        mOffsetRotation(Ogre::Quaternion::IDENTITY),
		mInterpolationTimes(2), 
        mUseInterpolation(false), 
        mBuffersCreated(false),
        mFadeInTime(0.0f), 
        mFadeOutTime(1.0f), 
        mCurrentStartAlphaValue(1.0f), 
        mCurrentEndAlphaValue(1.0f)
	{
		mPoints.clear();

		if (mUseInterpolation)
			mCurrentVertexCount = mPointCount * mInterpolationTimes;
		else
			mCurrentVertexCount = mPointCount;

		if (createParamDictionary("AnimationRibbon"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();

			dict->addParameter(Ogre::ParameterDef("AttachTime", 
				"the time that showing the ribbon.",
				Ogre::PT_REAL),&msAttachTimeCmd);	  

			dict->addParameter(Ogre::ParameterDef("AttachPoint", 
				"the attached bone or locator.",
				Ogre::PT_STRING),&msAttachPointCmd);	  

			dict->addParameter(Ogre::ParameterDef("OffsetPos", 
				"offset position to the attach point.",
				Ogre::PT_VECTOR3),&msOffsetPosCmd);	  

			dict->addParameter(Ogre::ParameterDef("OffsetRotation", 
				"offset orientation to the attach point.",
				Ogre::PT_QUATERNION),&msOffsetRotationCmd);	  

			dict->addParameter(Ogre::ParameterDef("PointCount", 
				"the number of points.",
				Ogre::PT_UNSIGNED_INT),&msPointCountCmd);

			dict->addParameter(Ogre::ParameterDef("LifeTime", 
				"the life time of ribbon vertex.",
				Ogre::PT_REAL),&msLifeTimeCmd);

			dict->addParameter(Ogre::ParameterDef("RibbonWidth", 
				"the width of ribbon.",
				Ogre::PT_REAL),&msRibbonWidthCmd);

			dict->addParameter(Ogre::ParameterDef("StartAlphaValue", 
				"the head point alpha value.",
				Ogre::PT_REAL),&msStartAlpha);

			dict->addParameter(Ogre::ParameterDef("EndAlphaValue", 
				"the tail point of alpha value.",
				Ogre::PT_REAL),&msEndAlpha);

			dict->addParameter(Ogre::ParameterDef("StartColour", 
				"the colour of head point.",
				Ogre::PT_COLOURVALUE),&msStartColour);

			dict->addParameter(Ogre::ParameterDef("EndColour", 
				"the colour of tail point.",
				Ogre::PT_COLOURVALUE),&msEndColour);

            dict->addParameter(Ogre::ParameterDef("FadeInTime", 
                "the alpha fade in time.",
                Ogre::PT_REAL),&msFadeInTimeCmd);

            dict->addParameter(Ogre::ParameterDef("FadeOutTime", 
                "the alpha fade out time.",
                Ogre::PT_REAL),&msFadeOutTimeCmd);

			dict->addParameter(Ogre::ParameterDef("Material", 
				"name of ribbon material.",
				Ogre::PT_STRING),&msMaterial);	 

			dict->addParameter(Ogre::ParameterDef("InterTimes", 
				"the time that showing the ribbon.",
				Ogre::PT_INT),&msInterTimesCmd);	  

			dict->addParameter(Ogre::ParameterDef("UseInter", 
				"the time that showing the ribbon.",
				Ogre::PT_BOOL),&msUseInterCmd);	 

            dict->addParameter(Ogre::ParameterDef("VertexLifeTime", 
                "the life time of vertex.",
                Ogre::PT_REAL),&msVertexLifeTimeCmd);
		}
	}
    //-----------------------------------------------------------------------
	AnimationRibbon::~AnimationRibbon()
	{
		destroyRenderInstance();
	}
    //-----------------------------------------------------------------------
	bool AnimationRibbon::createRenderInstance(void)
	{	
		if (mCreateInstance)
		{
            // 如果已经创建，就马上返回
			mBasicNode->setVisible(true);

			mCurrentLifeTime = 0.0f;

			//mPoints.clear();

			return true;
		}
		else
		{
			// create scene node
			Ogre::SceneNode *baseNode = mSystem->getBaseSceneNode();

			mBasicNode = baseNode->createChildSceneNode();

			Ogre::SceneNode* parent = mBasicNode;

			if (parent)
			{
				mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
				mRenderOp.useIndexes = false;

				if (mUseInterpolation)
					_createBuffer( mPointCount * mInterpolationTimes );
				else
					_createBuffer( mPointCount );

				parent->attachObject(this);

				setMaterial(m_strMatName);
                
                setCastShadows(false);

				mCreateInstance = true;

				return true;
			}

			return false;
		}		
	}
    //-----------------------------------------------------------------------
	void AnimationRibbon::destroyRenderInstance(void)
	{
		_destroyBuffer();

		// delete scene node
		if (mBasicNode)
		{
			mBasicNode->destroy();
			mBasicNode = NULL;
		}	

		mCreateInstance = false;
	}
    //-----------------------------------------------------------------------
	void AnimationRibbon::updateRenderInstance( float time, TransformInfo &transform )
	{
		if (mCreateInstance)
		{
            // 如果ribbon生命期不为默认值(0.0f)，就需要计算当前的生命期
            if ( false == Ogre::Math::RealEqual(0.0f, mLifeTime) )
            {
                mCurrentLifeTime += time;

                if (mCurrentLifeTime >= mLifeTime)
                {
                    mBasicNode->setVisible(false);
                    mPoints.clear();

                    return;
                }
            }

            // 如果vertex生命期不为默认值(0.0f)，就需要计算当前的生命期
            if ( false == Ogre::Math::RealEqual(0.0f, mVertexLifeTime) )
            {
                // 给所有的点添加life time，如果这个点的生命期到了，就从列表中删除
                for (Points::iterator i = mPoints.begin(); i != mPoints.end();)
                {
                    Vertex& vertex = *i;

                    vertex.lifeTime += time;

                    if (vertex.lifeTime >= mVertexLifeTime)
                    {
                        i = mPoints.erase(i);
                    }
                    else
                        ++i;
                }

                // 保证点的个数是2的倍数
                if (mPoints.size() % 2 != 0)
                    mPoints.pop_back();
            }

			Ogre::Vector3 firstPos = transform.mPosition;

			Ogre::Vector3 dir = transform.mRotation.xAxis();
			dir.normalise();
			Ogre::Vector3 secondPos = firstPos + 
				dir * mRibbonWidth;

			mPoints.push_front( Vertex(secondPos) );
			mPoints.push_front( Vertex( firstPos ) );

            //EffectLevel effectLevel = EffectManager::getSingleton().getEffectLevel();

            //size_t newPointCount = mPointCount;

            //if (effectLevel == EL_MID)
            //{
            //    mUseInterpolation = false;

            //    newPointCount = (size_t)(newPointCount * 0.5f);

            //    // 保证点的个数是2的倍数
            //    if (newPointCount % 2 != 0)
            //        --newPointCount;

            //    if (newPointCount < 4)
            //        newPointCount = 4;
            //}
            //else if (effectLevel == EL_LOW)
            //{
            //    mUseInterpolation = false;

            //    newPointCount = (size_t)(newPointCount * 0.1f);

            //    // 保证点的个数是2的倍数
            //    if (newPointCount % 2 != 0)
            //        --newPointCount;

            //    if (newPointCount < 4)
            //        newPointCount = 4;
            //}

            while ( mPoints.size() > mPointCount )
            {
                mPoints.pop_back();
                mPoints.pop_back();
            }

			mFirstPosSpline.clear();
			mSecondPosSpline.clear();

			std::list<Vertex>::iterator it = mPoints.begin();

			if (mUseInterpolation)
			{
				while ( it != mPoints.end() )
				{
					mSecondPosSpline.addPoint(it->pos);

					++it;

					mFirstPosSpline.addPoint(it->pos);

					++it;
				}
			}

			// change the vertex buffer,if necessary
			if (!mBuffersCreated)
			{
				if (mUseInterpolation)
					_createBuffer( mPointCount * mInterpolationTimes );
				else
					_createBuffer( mPointCount );
			}

			// set the vertex count that will be rendered
			if (mUseInterpolation)
				mRenderOp.vertexData->vertexCount = mPoints.size() * mInterpolationTimes;
			else
				mRenderOp.vertexData->vertexCount = mPoints.size();

			Ogre::RenderSystem *pSys = Ogre::Root::getSingleton().getRenderSystem();

			float *lockPtr = static_cast<float *>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

			float count = 0.0f;

			float maxSqLen = -1.0f;

			Ogre::Vector3 min(Ogre::Math::POS_INFINITY, Ogre::Math::POS_INFINITY, Ogre::Math::POS_INFINITY);
			Ogre::Vector3 max(Ogre::Math::NEG_INFINITY, Ogre::Math::NEG_INFINITY, Ogre::Math::NEG_INFINITY);

            // 如果alpha值有一个不为0,才需要计算当前的alpha值
            if ( false == Ogre::Math::RealEqual(1.0f, mStartAlphaValue) ||
                false == Ogre::Math::RealEqual(1.0f, mStartAlphaValue) )
            {
                // 由于现在lifetime都保留在各个点中了，所以不进行alpha 的插值
                float timeRatio = mCurrentLifeTime / mLifeTime;

                // 根据当前的生命时间来决定透明度
                if (timeRatio < mFadeInTime)
                {
                    // 如果在淡入时间内，就根据比例设置透明度
                    float ratio = mCurrentLifeTime / (mFadeInTime * mLifeTime);
                    mCurrentStartAlphaValue *= ratio;
                    mCurrentEndAlphaValue *= ratio;
                }
                else if(timeRatio > mFadeOutTime)
                {
                    // 否则，如果在淡出时间内，就根据比例设置透明度
                    float ratio = (mLifeTime - mCurrentLifeTime) / (mFadeOutTime * mLifeTime);
                    mCurrentStartAlphaValue *= ratio;
                    mCurrentEndAlphaValue *= ratio;
                }
                else
                {
                    // 在淡入与淡出时间中，就用原透明度
                    mCurrentStartAlphaValue = mStartAlphaValue;
                    mCurrentEndAlphaValue = mEndAlphaValue;
                }
            }
            else
            {
                mCurrentStartAlphaValue = 1.0f;
                mCurrentEndAlphaValue = 1.0f;
            }
            
			if (mUseInterpolation)
			{
				// if we use the simple spline to interpolate the ribbon
				// we will render more vertexs
				int numLoop = mPoints.size() / 2 * mInterpolationTimes; 
				for (int i=0; i<numLoop; ++i)
				{
					float delta = 1.0f;
					if (numLoop > 0.0f)
						delta = count/(numLoop-1);

					float endOpacity = (1.0 - delta) * mCurrentEndAlphaValue;
					float startOpacity = (1.0 - delta) * mCurrentStartAlphaValue;

					// compute the bounding box
					const Ogre::Vector3& pos = mSecondPosSpline.interpolate( (float)i / (float)(numLoop-1) );

					min.makeFloor(pos);
					max.makeCeil(pos);

					maxSqLen = std::max(maxSqLen, pos.squaredLength());

					// point1
					*lockPtr++ = pos.x;
					*lockPtr++ = pos.y;
					*lockPtr++ = pos.z;

					Ogre::RGBA *pCol;
					pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(lockPtr));

					pSys->convertColourValue( Ogre::ColourValue(mEndColour.r * endOpacity, mEndColour.g * endOpacity,
						mEndColour.b * endOpacity, 1.0f), pCol++ ) ;

					// Update lock pointer
					lockPtr = static_cast<float*>(static_cast<void*>(pCol));

					*lockPtr++ = delta;
					*lockPtr++ = 1.0;

					const Ogre::Vector3& pos1 = mFirstPosSpline.interpolate( (float)i / (float)(numLoop-1) );
					min.makeFloor(pos1);
					max.makeCeil(pos1);

					maxSqLen = std::max(maxSqLen, pos1.squaredLength());

					// point2
					*lockPtr++ = pos1.x;
					*lockPtr++ = pos1.y;
					*lockPtr++ = pos1.z;

					pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(lockPtr));

					pSys->convertColourValue( Ogre::ColourValue(mStartColour.r * startOpacity, mStartColour.g * startOpacity,
						mStartColour.b * startOpacity, 1.0f), pCol++ ) ;

					// Update lock pointer
					lockPtr = static_cast<float*>(static_cast<void*>(pCol));

					*lockPtr++ = delta;
					*lockPtr++ = 0.0;

					count+=1.0f;
				}

			}
			else
			{
				float pointCount = (float)(mPoints.size());

				while ( it != mPoints.end() )
				{
					float delta = count/(pointCount-2);
					float endOpacity = (1.0 - delta) * mCurrentEndAlphaValue;
					float startOpacity = (1.0 - delta) * mCurrentStartAlphaValue;

					// compute the bounding box
					const Ogre::Vector3& pos = it->pos;

					min.makeFloor(pos);
					max.makeCeil(pos);

					maxSqLen = std::max(maxSqLen, pos.squaredLength());

					// point1
					*lockPtr++ = it->pos.x;
					*lockPtr++ = it->pos.y;
					*lockPtr++ = it->pos.z;

					Ogre::RGBA *pCol;
					pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(lockPtr));

					pSys->convertColourValue( Ogre::ColourValue(mEndColour.r * endOpacity, mEndColour.g * endOpacity,
						mEndColour.b * endOpacity, 1.0f), pCol++ ) ;

					// Update lock pointer
					lockPtr = static_cast<float*>(static_cast<void*>(pCol));

					*lockPtr++ = delta;
					*lockPtr++ = 1.0;
						++it;

					const Ogre::Vector3& pos1 = it->pos;
					min.makeFloor(pos1);
					max.makeCeil(pos1);

					maxSqLen = std::max(maxSqLen, pos1.squaredLength());

					// point2
					*lockPtr++ = it->pos.x;
					*lockPtr++ = it->pos.y;
					*lockPtr++ = it->pos.z;

					pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(lockPtr));

					pSys->convertColourValue( Ogre::ColourValue(mStartColour.r * startOpacity, mStartColour.g * startOpacity,
						mStartColour.b * startOpacity, 1.0f), pCol++ ) ;

					// Update lock pointer
					lockPtr = static_cast<float*>(static_cast<void*>(pCol));

					*lockPtr++ = delta;
					*lockPtr++ = 0.0;

							++it;

					count+=2.0f;
				}
			}
			
			vbuf->unlock();

			mBox.setExtents(min, max);
			mBoundingRadius = Ogre::Math::Sqrt(maxSqLen);
			getParentNode()->needUpdate();
		}		
	} 
    //-----------------------------------------------------------------------
	float AnimationRibbon::getBoundingRadius(void) const
	{
		// TODO: Implementation
		return mBoundingRadius;
	}
    //-----------------------------------------------------------------------
	float AnimationRibbon::getSquaredViewDepth(const Ogre::Camera* cam) const
	{
		// TODO: Implementation
		Ogre::Vector3 min, max, mid, dist;

		min = mBox.getMinimum();
		max = mBox.getMaximum();
		mid = ((min - max) * 0.5) + min;
		dist = cam->getDerivedPosition() - mid;

		return dist.squaredLength();
		//return 0.0f;
	}	
    //-----------------------------------------------------------------------
	void AnimationRibbon::_createBuffer( size_t vertexCount )
	{
		if ( vertexCount == 0 )
			return;
		
		assert (mRenderOp.vertexData == NULL);
        assert (vertexCount % 2 == 0);

		mRenderOp.vertexData = new Ogre::VertexData;

		mRenderOp.vertexData->vertexCount = vertexCount;
		mRenderOp.vertexData->vertexStart = 0;
		// declaration
		Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

		size_t offset = 0;

		decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);
		decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);

		// create the vertex buffer
		vbuf = 
			Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(0),
			mRenderOp.vertexData->vertexCount,
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, vbuf);

		mBuffersCreated = true;
	}
    //-----------------------------------------------------------------------
	void AnimationRibbon::_destroyBuffer(void)
	{
		if (mRenderOp.vertexData)
		{
			delete mRenderOp.vertexData;
			mRenderOp.vertexData = NULL;
		}

		mBuffersCreated = false;
	}
    //-----------------------------------------------------------------------
	void AnimationRibbon::setUseInterpolation(bool use)
	{
		mUseInterpolation = use;

		_destroyBuffer();
	}
    //-----------------------------------------------------------------------
	void AnimationRibbon::setInterpolationTimes(int times)
	{
		mInterpolationTimes = times;

		_destroyBuffer();
	}
    //-----------------------------------------------------------------------
	void AnimationRibbon::setPointCount( unsigned short count )
	{
        // 保证点的个数是2的倍数
        if (count % 2 != 0)
            --count;

		// we need 4 point to draw a rectangle in OT_TRIANGLE_STRIP mode
		if (count < 4)
			mPointCount = 4;
		else
			mPointCount = count;

		_destroyBuffer();
	}
	//-----------------------------------------------------------------------
	void AnimationRibbon::setMaterialName(const String &matName)
	{
		m_strMatName = matName;

		if (mCreateInstance)
			setMaterial(m_strMatName);
	}
    //-----------------------------------------------------------------------
    void AnimationRibbon::setFadeInTime(float time)
    {
        if((time < 0.0f) || (time > 1.0f))
            mFadeInTime = 0.0f;
        else
            mFadeInTime = time;
    }
    //-----------------------------------------------------------------------
    void AnimationRibbon::setFadeOutTime(float time)
    {
        if((time < 0.0f) || (time > 1.0f))
            mFadeOutTime = 0.0f;
        else
            mFadeOutTime = time;
    }
    //-----------------------------------------------------------------------
    void AnimationRibbon::setVisibleFlag(uint32 flags)
    {
        setVisibilityFlags(flags);
    }
    //-----------------------------------------------------------------------
    void AnimationRibbon::copyParameters(AnimationRibbon& newInfo) const
    {        
        newInfo.mPointCount = mPointCount;
        newInfo.mStartColour = mStartColour;
        newInfo.mEndColour = mEndColour;
        newInfo.mStartAlphaValue = mStartAlphaValue;
        newInfo.mEndAlphaValue = mEndAlphaValue;
        newInfo.mFadeInTime = mFadeInTime;
        newInfo.mFadeOutTime = mFadeOutTime;
        newInfo.m_strMatName = m_strMatName;
        newInfo.mRibbonWidth = mRibbonWidth;
        newInfo.mLifeTime = mLifeTime;
        newInfo.mAttachTime = mAttachTime;
        newInfo.mAttachPoint = mAttachPoint;
        newInfo.mOffsetPos = mOffsetPos;
        newInfo.mOffsetRotation = mOffsetRotation;
        newInfo.mUseInterpolation = mUseInterpolation;
        newInfo.mInterpolationTimes = mInterpolationTimes;
        newInfo.mVertexLifeTime = mVertexLifeTime;
    }
	//////////////////////////////////////////////////////////////////////////
	String AnimationRibbon::CmdPointCount::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getPointCount() );
	}
	void AnimationRibbon::CmdPointCount::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setPointCount(Ogre::StringConverter::parseUnsignedInt(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdPointCount::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setPointCount(
            static_cast<const AnimationRibbon *>(source)->getPointCount() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdRibbonWidth::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getRibbonWidth() );
	}
	void AnimationRibbon::CmdRibbonWidth::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setRibbonWidth(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdRibbonWidth::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setRibbonWidth(
            static_cast<const AnimationRibbon *>(source)->getRibbonWidth() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdStartColour::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getStartColour() );
	}
	void AnimationRibbon::CmdStartColour::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setStartColour(Ogre::StringConverter::parseColourValue(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdStartColour::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setStartColour(
            static_cast<const AnimationRibbon *>(source)->getStartColour() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdEndColour::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getEndColour() );
	}
	void AnimationRibbon::CmdEndColour::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setEndColour(Ogre::StringConverter::parseColourValue(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdEndColour::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setEndColour(
            static_cast<const AnimationRibbon *>(source)->getEndColour() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdStartAlpha::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getStartAlpha() );
	}
	void AnimationRibbon::CmdStartAlpha::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setStartAlpha(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdStartAlpha::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setStartAlpha(
            static_cast<const AnimationRibbon *>(source)->getStartAlpha() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdEndAlpha::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getEndAlpha() );
	}
	void AnimationRibbon::CmdEndAlpha::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setEndAlpha(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdEndAlpha::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setEndAlpha(
            static_cast<const AnimationRibbon *>(source)->getEndAlpha() );
    }
#endif
    //-----------------------------------------------------------------------
    String AnimationRibbon::CmdFadeInTime::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const AnimationRibbon *>(target)->getFadeInTime() );
    }
    void AnimationRibbon::CmdFadeInTime::doSet(void* target, const String& val)
    {
        static_cast<AnimationRibbon *>(target)->setFadeInTime(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdFadeInTime::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setFadeInTime(
            static_cast<const AnimationRibbon *>(source)->getFadeInTime() );
    }
#endif
    //-----------------------------------------------------------------------
    String AnimationRibbon::CmdFadeOutTime::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const AnimationRibbon *>(target)->getFadeOutTime() );
    }
    void AnimationRibbon::CmdFadeOutTime::doSet(void* target, const String& val)
    {
        static_cast<AnimationRibbon *>(target)->setFadeOutTime(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdFadeOutTime::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setFadeOutTime(
            static_cast<const AnimationRibbon *>(source)->getFadeOutTime() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdMaterial::doGet(const void* target) const
	{
		return static_cast<const AnimationRibbon *>(target)->getMaterialName();
	}
	void AnimationRibbon::CmdMaterial::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setMaterialName(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdMaterial::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setMaterialName(
            static_cast<const AnimationRibbon *>(source)->getMaterialName() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdLifeTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getLifeTime() );
	}
	void AnimationRibbon::CmdLifeTime::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setLifeTime(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdLifeTime::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setLifeTime(
            static_cast<const AnimationRibbon *>(source)->getLifeTime() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdAttachTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getAttachTime() );
	}
	void AnimationRibbon::CmdAttachTime::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdAttachTime::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setAttachTime(
            static_cast<const AnimationRibbon *>(source)->getAttachTime() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdAttachPoint::doGet(const void* target) const
	{
		return static_cast<const AnimationRibbon *>(target)->getAttachPoint();
	}
	void AnimationRibbon::CmdAttachPoint::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setAttachPoint(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdAttachPoint::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setAttachPoint(
            static_cast<const AnimationRibbon *>(source)->getAttachPoint() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdOffsetPos::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getOffsetPos() );
	}
	void AnimationRibbon::CmdOffsetPos::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setOffsetPos(Ogre::StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdOffsetPos::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setOffsetPos(
            static_cast<const AnimationRibbon *>(source)->getOffsetPos() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdOffsetRotation::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getOffsetRotation() );
	}
	void AnimationRibbon::CmdOffsetRotation::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setOffsetRotation(Ogre::StringConverter::parseQuaternion(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdOffsetRotation::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setOffsetRotation(
            static_cast<const AnimationRibbon *>(source)->getOffsetRotation() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdUseInter::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getUseInterpolation() );
	}
	void AnimationRibbon::CmdUseInter::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setUseInterpolation( Ogre::StringConverter::parseBool(val) );
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdUseInter::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setUseInterpolation(
            static_cast<const AnimationRibbon *>(source)->getUseInterpolation() );
    }
#endif
    //-----------------------------------------------------------------------
	String AnimationRibbon::CmdInterTimes::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationRibbon *>(target)->getInterpolationTimes() );
	}
	void AnimationRibbon::CmdInterTimes::doSet(void* target, const String& val)
	{
		static_cast<AnimationRibbon *>(target)->setInterpolationTimes( Ogre::StringConverter::parseInt(val) );
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdInterTimes::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setInterpolationTimes(
            static_cast<const AnimationRibbon *>(source)->getInterpolationTimes() );
    }
#endif
    //-----------------------------------------------------------------------
    String AnimationRibbon::CmdVertexLifeTime::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const AnimationRibbon *>(target)->getVertexLifeTime() );
    }
    void AnimationRibbon::CmdVertexLifeTime::doSet(void* target, const String& val)
    {
        static_cast<AnimationRibbon *>(target)->setVertexLifeTime(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationRibbon::CmdVertexLifeTime::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        static_cast<AnimationRibbon *>(target)->setVertexLifeTime(
            static_cast<const AnimationRibbon *>(source)->getVertexLifeTime() );
    }
#endif
}
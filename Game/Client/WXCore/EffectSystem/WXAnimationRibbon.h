/********************************************************************
filename:   WXAnimationRibbon.h

purpose:    the ribbon that attach to the bone or a locator.
*********************************************************************/

#ifndef _FAIRYANIMATIONRIBBON_H_
#define _FAIRYANIMATIONRIBBON_H_

// ogre header
#include <OgreVector3.h>
#include <OgreColourValue.h>
#include <OgreSimpleRenderable.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreStringInterface.h>
#include <OgreStringVector.h>
#include <OgreQuaternion.h>
#include <OgreSimpleSpline.h>

#include "WXEffect.h"

namespace Ogre	{
	class SceneNode;
}

namespace WX	{

	class System;

	class AnimationRibbon :  public Ogre::StringInterface, public Ogre::SimpleRenderable
	{
		struct Vertex
		{
			Ogre::Vector3 pos;
			Ogre::RGBA color;
			float tex[2];
            float lifeTime;

			Vertex( const Ogre::Vector3 &p ) : pos(p), lifeTime(0.0f)
			{
			}
		};

	public:

		class CmdPointCount : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdLifeTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdStartColour : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdEndColour : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdStartAlpha : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdEndAlpha : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

        class CmdFadeInTime : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };

        class CmdFadeOutTime : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };

		class CmdMaterial : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdRibbonWidth : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdAttachTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdAttachPoint : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdOffsetPos : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdOffsetRotation : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdUseInter : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdInterTimes : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

        class CmdVertexLifeTime : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };
	
		/////////////////////////////////////////////////////////////
		AnimationRibbon( System *system, unsigned short pointCount = 30, float lifeTime = 1.0f, float ribbonWidth = 100.0f,
			const Ogre::ColourValue &startColour = Ogre::ColourValue::White,
            const Ogre::ColourValue &endColour = Ogre::ColourValue::White,
			float startAlpha = 1.0f, float endAlpha = 1.0f, float vertexLifeTime = 0.0f );

		~AnimationRibbon();

		bool createRenderInstance(void);
		void destroyRenderInstance(void);
		void updateRenderInstance( float time, TransformInfo &transform );

		//////////////////////////////////////////////////////////////////////////
		/// Implementation of Ogre::SimpleRenderable
		virtual float getBoundingRadius(void) const;
		virtual float getSquaredViewDepth(const Ogre::Camera* cam) const;

		void setPointCount( unsigned short count );

		unsigned short getPointCount(void) const
		{
			return mPointCount;
		}

		void setRibbonWidth( float width )
		{
			mRibbonWidth = width;
		}

		float getRibbonWidth(void) const
		{
			return mRibbonWidth;
		}

		void setStartColour( const Ogre::ColourValue &value )
		{
			mStartColour = value;
		}

		const Ogre::ColourValue & getStartColour(void) const
		{
			return mStartColour;
		}

		void setEndColour( const Ogre::ColourValue &value )
		{
			mEndColour = value;
		}

		const Ogre::ColourValue& getEndColour(void) const
		{
			return mEndColour;
		}

		void setStartAlpha( const float value )
		{
			mStartAlphaValue = value;
		}

		const float getStartAlpha(void) const
		{
			return mStartAlphaValue;
		}

		void setEndAlpha( const float value )
		{
			mEndAlphaValue = value;
		}

		const float getEndAlpha(void) const
		{
			return mEndAlphaValue;
		}

		const String& getMaterialName(void) const
		{
			return m_strMatName;
		}	

		float getLifeTime(void) const
		{
			return mLifeTime;
		}

		void setLifeTime(float time)
		{
			mLifeTime = time;
		}

		void setAttachTime(float time)
		{
			mAttachTime = time;
		}
		float getAttachTime(void) const
		{
			return mAttachTime;
		}

		void setAttachPoint(const String &point)
		{
			mAttachPoint = point;
		}
		const String& getAttachPoint(void) const
		{
			return mAttachPoint;
		}

		void setOffsetPos(const Ogre::Vector3 &pos)
		{
			mOffsetPos = pos;
		}
		const Ogre::Vector3& getOffsetPos(void) const
		{
			return mOffsetPos;
		}

		void setOffsetRotation(const Ogre::Quaternion &rotation)
		{
			mOffsetRotation = rotation;
		}
		const Ogre::Quaternion& getOffsetRotation(void) const
		{
			return mOffsetRotation;
		}

		void setUseInterpolation(bool use);

		bool getUseInterpolation(void) const
		{
			return mUseInterpolation;
		}

		void setInterpolationTimes(int times);

		int getInterpolationTimes(void) const
		{
			return mInterpolationTimes;
		}

		void setMaterialName(const String &matName);	

        void setFadeInTime(float time);
        float getFadeInTime(void) const
        {
            return mFadeInTime;
        }

        void setFadeOutTime(float time);
        float getFadeOutTime(void) const
        {
            return mFadeOutTime;
        }

        void setVertexLifeTime(float time)
        {
            mVertexLifeTime = time;
        }
        float getVertexLifeTime(void) const
        {
            return mVertexLifeTime;
        }

        /// 设置visible flag
        virtual void setVisibleFlag(uint32 flags);

        /** 拷贝所有特效参数变量
        @remarks 由于原来用Ogre::StringInterface::copyParametersTo这个函数
        来进行参数的拷贝速度太慢（因为里面有一些字符串的操作），所以
        用一个函数来复制所有的参数变量，每加一个参数，都要在这个函数
        中添加相对项
        */
        void copyParameters(AnimationRibbon& newInfo) const;

	protected:

		void _createBuffer( size_t vertexCount );
		void _destroyBuffer(void);

	protected:

		Ogre::HardwareVertexBufferSharedPtr vbuf;

        typedef std::list<Vertex> Points; 
		Points mPoints;

		unsigned short mPointCount;
		float mLifeTime;
        float mVertexLifeTime;
		// colour
		Ogre::ColourValue mStartColour;
		Ogre::ColourValue mEndColour;

		// alpha value
		float mStartAlphaValue;
		float mEndAlphaValue;

		float mRibbonWidth;

		/// Bounding radius
		float mBoundingRadius;

		Ogre::SceneNode *mBasicNode;

		bool mCreateInstance;

		float mCurrentLifeTime;

		System *mSystem;

		float mAttachTime;
		String mAttachPoint;

		Ogre::Vector3 mOffsetPos;
		Ogre::Quaternion mOffsetRotation;

		Ogre::SimpleSpline mFirstPosSpline;
		Ogre::SimpleSpline mSecondPosSpline;

		bool mUseInterpolation;
		int mInterpolationTimes;

		size_t mCurrentVertexCount;
		bool mBuffersCreated;

        /** 淡入时间
            光的start alpha和end alpha会在淡入时间内由零递增，在到达淡入时间时，alpha值也
            达到所设定的start alpha和end alpha的值
        */
        float mFadeInTime;
        
        /** 淡出时间
        光的start alpha和end alpha会在淡出时间内递减，在到达生命时间时，alpha值也
        减为0
        */
        float mFadeOutTime;

        /// 保存中间变化的alpha值
        float mCurrentStartAlphaValue;
        float mCurrentEndAlphaValue;
		////////////////////////////////////////////////////////////////
		static CmdPointCount msPointCountCmd;
		static CmdStartColour msStartColour;
		static CmdEndColour msEndColour;
		static CmdStartAlpha msStartAlpha;
		static CmdEndAlpha msEndAlpha;
        static CmdFadeInTime msFadeInTimeCmd;
        static CmdFadeOutTime msFadeOutTimeCmd;
		static CmdMaterial msMaterial;
		static CmdRibbonWidth msRibbonWidthCmd;
		static CmdLifeTime msLifeTimeCmd;
		static CmdAttachTime msAttachTimeCmd;
		static CmdAttachPoint msAttachPointCmd;
		static CmdOffsetPos msOffsetPosCmd;
		static CmdOffsetRotation msOffsetRotationCmd;
		static CmdUseInter msUseInterCmd;
		static CmdInterTimes msInterTimesCmd;
        static CmdVertexLifeTime msVertexLifeTimeCmd;
	};
}

#endif
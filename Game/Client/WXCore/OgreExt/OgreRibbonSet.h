/********************************************************************
filename:   OgreRibbonSet.h

purpose:    这个类用于产生一条或多条ribbon chain
*********************************************************************/
#ifndef __RibbonSet_H__
#define __RibbonSet_H__

#include <OgreBillboardChain.h>
#include <OgreParticle.h>

namespace Ogre
{
    class RibbonSet : public BillboardChain
    {
    public:

        RibbonSet(const String& name, size_t maxElements = 20, size_t numberOfChains = 1, 
            bool useTextureCoords = true, bool useColours = true);

        void _notifyResized(void);
        void setDefaultSize(Real width, Real length);

        /** @copydoc BillboardChain::setNumberOfChains */
        void setNumberOfChains(size_t numChains);
        /** @copydoc BillboardChain::setMaxChainElements */
        void setMaxChainElements(size_t maxElements);

        /** 更新指定的ribbon
        @param index ribbon的index（保存在RibbonParticleData中）
        @param pos ribbon头部当前的位置（有可能是世界坐标，也有可能是局部坐标）
        @param worldSpace 标识pos参数是否是世界坐标
        @param ribbon当前的颜色值
        @param elemWidth ribbon当前的宽度
        */
        void updateRibbon(size_t index, const Vector3& pos, bool worldSpace, const ColourValue& colour, Real elemWidth);

        /** 重置指定的ribbon
        @param index ribbon的index（保存在RibbonParticleData中）       
        */
        void resetRibbon(size_t index);

        void setHeadAlpha(Real alpha);
        Real getHeadAlpha(void) const
        {
            return mRibbonHeadAlpha;
        }

        void setTailAlpha(Real alpha);
        Real getTailAlpha(void) const
        {
            return mRibbonTailAlpha;
        }

        void setHeadWidthScale(Real scale);
        Real getHeadWidthScale(void) const
        {
            return mRibbonHeadWidthScale;
        }

        void setTailWidthScale(Real scale);
        Real getTailWidthScale(void) const
        {
            return mRibbonTailWidthScale;
        }

    protected:

        /// 默认的宽度值（相同于particle system的particle_width和particle_height）
        Real mDefaultElemWidth;
        Real mDefaultElemLength;

        /// 当前ribbon的宽度是否经过了affector影响
        bool mAllDefaultSize;

        /// 每个element的平方高度
        Real mSquaredElemLength;

        Real mRibbonHeadAlpha;
        Real mRibbonTailAlpha;
        bool mNeedChangeColour;

        Real mRibbonHeadWidthScale;
        Real mRibbonTailWidthScale;
        bool mNeedChangeWidth;
    };

    /// visual data，用于保存这个particle所对应的ribbon的index
    class RibbonParticleData : public ParticleVisualData
    {
    public:
        RibbonParticleData(size_t chainIndex)
            : mRibbonIndex(chainIndex)
        {
        }

        size_t getRibbonIndex(void)
        {
            return mRibbonIndex;
        }

    protected:

        size_t mRibbonIndex;
    };
}

#endif // __RibbonSet_H__
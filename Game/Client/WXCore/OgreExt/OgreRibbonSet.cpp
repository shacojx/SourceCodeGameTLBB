/********************************************************************
filename:   OgreRibbonSet.cpp

purpose:    这个类用于产生一条或多条ribbon chain
*********************************************************************/
#include "OgreRibbonSet.h"

#include <OgreSceneNode.h>

namespace Ogre
{
    RibbonSet::RibbonSet(const String& name, size_t maxElements, 
        size_t numberOfChains, bool useTextureCoords, bool useColours)
        : BillboardChain(name, maxElements, 0, useTextureCoords, useColours, true)
        , mSquaredElemLength(0.0f)
        , mDefaultElemWidth(0.0f)
        , mDefaultElemLength(0.0f)
        , mAllDefaultSize(true)
        , mRibbonHeadAlpha(1.0f)
        , mRibbonTailAlpha(1.0f)
        , mNeedChangeColour(false)
        , mRibbonHeadWidthScale(1.0f)
        , mRibbonTailWidthScale(1.0f)
        , mNeedChangeWidth(false)
    {
        // use U as varying texture coord, so we can use 1D textures to 'smear'
        setTextureCoordDirection(TCD_U);
    }
    //-----------------------------------------------------------------------
    void RibbonSet::_notifyResized(void)
    {
        mAllDefaultSize = false;
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setNumberOfChains(size_t numChains)
    {
        BillboardChain::setNumberOfChains(numChains);
    }
    //-----------------------------------------------------------------------
    void RibbonSet::updateRibbon(size_t index, const Vector3& pos, bool worldSpace, const ColourValue& colour, Real elemWidth)
    {
        Real realWidth;

        // 如果当前用的是粒子系统的所定的粒子大小
        if (mAllDefaultSize)
            realWidth = mDefaultElemWidth;
        // 如果当前用的是粒子系统影响器的所定的粒子大小
        else
            realWidth = elemWidth;

        Vector3 newPos = pos;
        if (mParentNode && worldSpace)
        {
            // Transform position to ourself space
            newPos = mParentNode->_getDerivedOrientation().UnitInverse() *
                (newPos - mParentNode->_getDerivedPosition()) / mParentNode->_getDerivedScale();
        }

        // Node has changed somehow, we're only interested in the derived position
        ChainSegment& seg = mChainSegmentList[index];

        if (seg.head == SEGMENT_EMPTY)
        {
            Element e(newPos, realWidth, 0, colour);
            addChainElement(index, e);
            addChainElement(index, e);
        }

        Element& headElem = mChainElementList[seg.start + seg.head];
        size_t nextElemIdx = seg.head + 1;
        // wrap
        if (nextElemIdx == mMaxElementsPerChain)
            nextElemIdx = 0;
        Element& nextElem = mChainElementList[seg.start + nextElemIdx];

        // Vary the head elem, but bake new version if that exceeds element len
        Vector3 diff = newPos - nextElem.position;
        Real sqlen = diff.squaredLength();
        if (sqlen >= mSquaredElemLength)
        {
            // Move existing head to mDefaultElemLength
            Vector3 scaledDiff = diff * (mDefaultElemLength / Math::Sqrt(sqlen));
            headElem.position = nextElem.position + scaledDiff;
            // Add a new element to be the new head
            Element newElem(newPos, realWidth, 0.0f, colour);
            addChainElement(index, newElem);
            // alter diff to represent new head size
            diff = newPos - newElem.position;

        }
        else
        {
            // Extend existing head
            headElem.position = newPos;
        }

        // Is this segment full?
        if ((seg.tail + 1) % mMaxElementsPerChain == seg.head)
        {
            // If so, shrink tail gradually to match head extension
            Element& tailElem = mChainElementList[seg.start + seg.tail];
            size_t preTailIdx;
            if (seg.tail == 0)
                preTailIdx = mMaxElementsPerChain - 1;
            else
                preTailIdx = seg.tail - 1;
            Element& preTailElem = mChainElementList[seg.start + preTailIdx];

            // Measure tail diff from pretail to tail
            Vector3 taildiff = tailElem.position - preTailElem.position;
            Real taillen = taildiff.length();
            if (taillen > 1e-06)
            {
                Real tailsize = mDefaultElemLength - diff.length();
                taildiff *= tailsize / taillen;
                tailElem.position = preTailElem.position + taildiff;
            }

        }

        // 计算出当前chain中的实际element个数
        size_t elementCount;

        // 根据addElement中的实现，如果head大于tail，说明chain已经是满的了，如果head小于tail，说明chain未满
        if (seg.head > seg.tail)
            elementCount = getMaxChainElements();
        else if (seg.head < seg.tail)
            elementCount = seg.tail - seg.head + 1;

        assert (seg.head != seg.tail);

        // 取出这个chain中第一个和最后一个元素
        const Element& firstElem = mChainElementList[seg.start + seg.tail];
        const Element& lastElem = mChainElementList[seg.start + seg.head];

        // 计算它们之间的平方距离
        Real totalLength = (lastElem.position - firstElem.position).squaredLength();

        // 如果距离大于0，才进行计算（在每个particle第一次进入这个函数时，它们的距离是0）
        if (false == Math::RealEqual(0.0f, totalLength))
        {
            // 遍历当前chain中的所以实际元素
            for (size_t i=0; i<elementCount; ++i)
            {
                Element e = getChainElement(index, i);

                // 计算当前元素与第一个元素之间的平方距离，求出这个元素在chain中的实际位置，这里先求出两个平方距离的商，再开方
                // 比直接求出两个实际length再求商能节省一次开方操作
                Real ratio = 1.0f - Math::Sqrt( (e.position - firstElem.position).squaredLength() / totalLength );
                e.texCoord = ratio;

                if (mNeedChangeColour)
                    e.colour = colour * (mRibbonHeadAlpha + (mRibbonTailAlpha - mRibbonHeadAlpha) * ratio);

                if (mNeedChangeWidth)
                    e.width = realWidth * (mRibbonHeadWidthScale + (mRibbonTailWidthScale - mRibbonHeadWidthScale) * ratio);

                updateChainElement(index,i,e);
            }
        }

        mBoundsDirty = true;

        if (mParentNode)
        {
            mParentNode->needUpdate();
        }
    }
    //-----------------------------------------------------------------------
    void RibbonSet::resetRibbon(size_t index)
    {
        clearChain(index);

        // we removed an entry so indexes need updating
        mIndexContentDirty = true;
        mBoundsDirty = true;
        // tell parent node to update bounds
        if (mParentNode)
            mParentNode->needUpdate();
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setHeadAlpha(Real alpha)
    {
        mRibbonHeadAlpha = alpha;

        // 如果有一个数值不为1，就需要改变颜色
        if ( false == Math::RealEqual(1.0f, mRibbonHeadAlpha) || 
            false == Math::RealEqual(1.0f, mRibbonTailAlpha) )
            mNeedChangeColour = true;            
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setTailAlpha(Real alpha)
    {
        mRibbonTailAlpha = alpha;

        // 如果有一个数值不为1，就需要改变颜色
        if ( false == Math::RealEqual(1.0f, mRibbonHeadAlpha) || 
            false == Math::RealEqual(1.0f, mRibbonTailAlpha) )
            mNeedChangeColour = true;
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setHeadWidthScale(Real scale)
    {
        mRibbonHeadWidthScale = scale;

        // 如果有一个数值不为1，就需要改变颜色
        if ( false == Math::RealEqual(1.0f, mRibbonHeadWidthScale) || 
            false == Math::RealEqual(1.0f, mRibbonTailWidthScale) )
            mNeedChangeWidth = true;
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setTailWidthScale(Real scale)
    {
        mRibbonTailWidthScale = scale;

        // 如果有一个数值不为1，就需要改变颜色
        if ( false == Math::RealEqual(1.0f, mRibbonHeadWidthScale) || 
            false == Math::RealEqual(1.0f, mRibbonTailWidthScale) )
            mNeedChangeWidth = true;
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setDefaultSize(Real width, Real length)
    {
        mDefaultElemWidth = width;
        mDefaultElemLength = length;

        mSquaredElemLength = mDefaultElemLength * mDefaultElemLength;
    }
    //-----------------------------------------------------------------------
    void RibbonSet::setMaxChainElements(size_t maxElements)
    {
        BillboardChain::setMaxChainElements(maxElements);

        mBuffersNeedRecreating = mIndexContentDirty = true;
    }
}
/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

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
#include "OgreRibbonTrail.h"
#include "OgreMath.h"
#include "OgreException.h"
#include "OgreSceneNode.h"
#include "OgreStringConverter.h"

namespace Ogre
{
    namespace
    {
        /** Controller value for pass frame time to RibbonTrail
        */
        class _OgrePrivate TimeControllerValue : public ControllerValue<Real>
        {
        protected:
            RibbonTrail* mTrail;
        public:
            TimeControllerValue(RibbonTrail* r) { mTrail = r; }

            Real getValue(void) const { return 0; }// not a source 
            void setValue(Real value) { mTrail->_timeUpdate(value); }
        };
    }
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	RibbonTrail::RibbonTrail(const String& name, size_t maxElements, 
		size_t numberOfChains, bool useTextureCoords, bool useColours)
		:BillboardChain(name, maxElements, 0, useTextureCoords, useColours, true),
		mFadeController(0)
	{
		setTrailLength(100);
		setNumberOfChains(numberOfChains);
		mTimeControllerValue = ControllerValueRealPtr(new TimeControllerValue(this));

		// use V as varying texture coord, so we can use 1D textures to 'smear'
		setTextureCoordDirection(TCD_V);


	}
	//-----------------------------------------------------------------------
	RibbonTrail::~RibbonTrail()
	{
		// Detach listeners
		for (NodeList::iterator i = mNodeList.begin(); i != mNodeList.end(); ++i)
		{
			(*i)->setListener(0);
		}

        if (mFadeController)
        {
			// destroy controller
			ControllerManager::getSingleton().destroyController(mFadeController);
        }

	}
	//-----------------------------------------------------------------------
	void RibbonTrail::addNode(Node* n)
	{
		if (mNodeList.size() == mChainCount)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				mName + " cannot monitor any more nodes, chain count exceeded",
				"RibbonTrail::addNode");
		}
		if (n->getListener())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				mName + " cannot monitor node " + n->getName() + " since it already has a listener.",
				"RibbonTrail::addNode");
		}

        // initialise the chain
        resetTrail(mNodeList.size(), n);

		mNodeList.push_back(n);
		n->setListener(this);

	}
	//-----------------------------------------------------------------------
	void RibbonTrail::removeNode(Node* n)
	{
		NodeList::iterator i = std::find(mNodeList.begin(), mNodeList.end(), n);
		if (i != mNodeList.end())
		{
			n->setListener(0);
			mNodeList.erase(i);
		}
	}
	//-----------------------------------------------------------------------
	RibbonTrail::NodeIterator 
	RibbonTrail::getNodeIterator(void) const
	{
		return NodeIterator(mNodeList.begin(), mNodeList.end());
	}
	//-----------------------------------------------------------------------
	void RibbonTrail::setTrailLength(Real len)
	{
		mTrailLength = len;
		mElemLength = mTrailLength / mMaxElementsPerChain;
		mSquaredElemLength = mElemLength * mElemLength;
	}
	//-----------------------------------------------------------------------
	void RibbonTrail::setMaxChainElements(size_t maxElements)
	{
		BillboardChain::setMaxChainElements(maxElements);
		mElemLength = mTrailLength / mMaxElementsPerChain;
		mSquaredElemLength = mElemLength * mElemLength;

        resetAllTrails();
	}
	//-----------------------------------------------------------------------
	void RibbonTrail::setNumberOfChains(size_t numChains)
	{
        if (numChains < mNodeList.size())
        {
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Can't shrink the number of chains less than number of tracking nodes",
                "RibbonTrail::setNumberOfChains");
        }

		BillboardChain::setNumberOfChains(numChains);

		mInitialColour.resize(numChains, ColourValue::White);
        mDeltaColour.resize(numChains, ColourValue::ZERO);
		mInitialWidth.resize(numChains, 10);
		mDeltaWidth.resize(numChains, 0);

        resetAllTrails();
	}
	//-----------------------------------------------------------------------
    void RibbonTrail::clearChain(size_t chainIndex)
    {
        BillboardChain::clearChain(chainIndex);

        // Reset if we are tracking for this chain
        if (chainIndex < mNodeList.size())
        {
            resetTrail(chainIndex, mNodeList[chainIndex]);
        }
    }
	//-----------------------------------------------------------------------
	void RibbonTrail::setInitialColour(size_t chainIndex, const ColourValue& col)
	{
		setInitialColour(chainIndex, col.r, col.g, col.b, col.a);
	}
	//-----------------------------------------------------------------------
	void RibbonTrail::setInitialColour(size_t chainIndex, Real r, Real g, Real b, Real a)
	{
		if (chainIndex >= mChainCount)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"chainIndex out of bounds", "RibbonTrail::setInitialColour");
		}
		mInitialColour[chainIndex].r = r;
		mInitialColour[chainIndex].g = g;
		mInitialColour[chainIndex].b = b;
		mInitialColour[chainIndex].a = a;
	}
	//-----------------------------------------------------------------------
	const ColourValue& RibbonTrail::getInitialColour(size_t chainIndex) const
	{
		if (chainIndex >= mChainCount)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"chainIndex out of bounds", "RibbonTrail::getInitialColour");
		}
		return mInitialColour[chainIndex];
	}
	//-----------------------------------------------------------------------
	void RibbonTrail::setInitialWidth(size_t chainIndex, Real width)
	{
		if (chainIndex >= mChainCount)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"chainIndex out of bounds", "RibbonTrail::setInitialWidth");
		}
		mInitialWidth[chainIndex] = width;
	}
	//-----------------------------------------------------------------------
	Real RibbonTrail::getInitialWidth(size_t chainIndex) const
	{
		if (chainIndex >= mChainCount)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"chainIndex out of bounds", "RibbonTrail::getInitialWidth");
		}
		return mInitialWidth[chainIndex];
	}
	//-----------------------------------------------------------------------
	void RibbonTrail::setColourChange(size_t chainIndex, const ColourValue& valuePerSecond)
	{
		setColourChange(chainIndex, 
			valuePerSecond.r, valuePerSecond.g, valuePerSecond.b, valuePerSecond.a);
	}
	//-----------------------------------------------------------------------
	void RibbonTrail::setColourChange(size_t chainIndex, Real r, Real g, Real b, Real a)
	{
		if (chainIndex >= mChainCount)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"chainIndex out of bounds", "RibbonTrail::setColourChange");
		}
		mDeltaColour[chainIndex].r = r;
		mDeltaColour[chainIndex].g = g;
		mDeltaColour[chainIndex].b = b;
		mDeltaColour[chainIndex].a = a;

		manageController();

	}
	//-----------------------------------------------------------------------
	const ColourValue& RibbonTrail::getColourChange(size_t chainIndex) const
	{
		if (chainIndex >= mChainCount)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"chainIndex out of bounds", "RibbonTrail::getColourChange");
		}
		return mDeltaColour[chainIndex];
	}
	//-----------------------------------------------------------------------
	void RibbonTrail::setWidthChange(size_t chainIndex, Real widthDeltaPerSecond)
	{
		if (chainIndex >= mChainCount)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"chainIndex out of bounds", "RibbonTrail::setWidthChange");
		}
		mDeltaWidth[chainIndex] = widthDeltaPerSecond;
		manageController();
	}
	//-----------------------------------------------------------------------
	Real RibbonTrail::getWidthChange(size_t chainIndex) const
	{
		if (chainIndex >= mChainCount)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"chainIndex out of bounds", "RibbonTrail::getWidthChange");
		}
		return mDeltaWidth[chainIndex];

	}
	//-----------------------------------------------------------------------
	void RibbonTrail::manageController(void)
	{
		bool needController = false;
		for (size_t i = 0; i < mChainCount; ++i)
		{
			if (mDeltaWidth[i] != 0 || mDeltaColour[i] != ColourValue::ZERO)
			{
				needController = true;
				break;
			}
		}
		if (!mFadeController && needController)
		{
			// Set up fading via frame time controller
			ControllerManager& mgr = ControllerManager::getSingleton();
			mFadeController = mgr.createFrameTimePassthroughController(mTimeControllerValue);
		}
		else if (mFadeController && !needController)
		{
			// destroy controller
			ControllerManager::getSingleton().destroyController(mFadeController);
			mFadeController = 0;
		}

	}
	//-----------------------------------------------------------------------
	void RibbonTrail::nodeUpdated(const Node* node)
	{
		
		for (size_t idx = 0; idx < mNodeList.size(); ++idx)
		{
			if (mNodeList[idx] == node)
			{
				updateTrail(idx, node);
				break;
			}
		}
	}
	//-----------------------------------------------------------------------
	void RibbonTrail::nodeDestroyed(const Node* node)
	{
		removeNode(const_cast<Node*>(node));

	}
	//-----------------------------------------------------------------------
	void RibbonTrail::updateTrail(size_t index, const Node* node)
	{
		// Node has changed somehow, we're only interested in the derived position
		ChainSegment& seg = mChainSegmentList[index];
		Element& headElem = mChainElementList[seg.start + seg.head];
		size_t nextElemIdx = seg.head + 1;
		// wrap
		if (nextElemIdx == mMaxElementsPerChain)
			nextElemIdx = 0;
		Element& nextElem = mChainElementList[seg.start + nextElemIdx];

		// Vary the head elem, but bake new version if that exceeds element len
        Vector3 newPos = node->_getDerivedPosition();
        if (mParentNode)
        {
            // Transform position to ourself space
            newPos = mParentNode->_getDerivedOrientation().UnitInverse() *
                (newPos - mParentNode->_getDerivedPosition()) / mParentNode->_getDerivedScale();
        }
		Vector3 diff = newPos - nextElem.position;
		Real sqlen = diff.squaredLength();
		if (sqlen >= mSquaredElemLength)
		{
			// Move existing head to mElemLength
			Vector3 scaledDiff = diff * (mElemLength / Math::Sqrt(sqlen));
			headElem.position = nextElem.position + scaledDiff;
			// Add a new element to be the new head
			Element newElem(newPos, mInitialWidth[index], 0.0f, mInitialColour[index]);
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
				Real tailsize = mElemLength - diff.length();
				taildiff *= tailsize / taillen;
				tailElem.position = preTailElem.position + taildiff;
			}

		}


		mBoundsDirty = true;
		// Need to dirty the parent node, but can't do it using needUpdate() here 
		// since we're in the middle of the scene graph update (node listener), 
		// so re-entrant calls don't work. Queue.
		if (mParentNode)
		{
			Node::queueNeedUpdate(getParentSceneNode());
		}

	}
	//-----------------------------------------------------------------------
	void RibbonTrail::_timeUpdate(Real time)
	{
		// Apply all segment effects
		for (size_t s = 0; s < mChainSegmentList.size(); ++s)
		{
			ChainSegment& seg = mChainSegmentList[s];
			if (seg.head != SEGMENT_EMPTY && seg.head != seg.tail)
			{
				
				for(size_t e = seg.head + 1;; ++e) // until break
				{
					e = e % mMaxElementsPerChain;

					Element& elem = mChainElementList[seg.start + e];
					elem.width = elem.width - (time * mDeltaWidth[s]);
					elem.width = std::max(Real(0.0f), elem.width);
					elem.colour = elem.colour - (mDeltaColour[s] * time);
					elem.colour.saturate();

					if (e == seg.tail)
						break;
					
				}
			}

		}

	}
    //-----------------------------------------------------------------------
    void RibbonTrail::resetTrail(size_t index, const Node* node)
    {
        assert(index < mChainCount);

        ChainSegment& seg = mChainSegmentList[index];
        // set up this segment
        seg.head = seg.tail = SEGMENT_EMPTY;
        // Create new element, v coord is always 0.0f
        Element e(node->_getDerivedPosition(),
            mInitialWidth[index], 0.0f, mInitialColour[index]);
        // Add the start position
        addChainElement(index, e);
        // Add another on the same spot, this will extend
        addChainElement(index, e);
    }
	//-----------------------------------------------------------------------
    void RibbonTrail::resetAllTrails(void)
    {
        for (size_t i = 0; i < mNodeList.size(); ++i)
        {
            resetTrail(i, mNodeList[i]);
        }
    }
	//-----------------------------------------------------------------------
	const String& RibbonTrail::getMovableType(void) const
	{
		return RibbonTrailFactory::FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	String RibbonTrailFactory::FACTORY_TYPE_NAME = "RibbonTrail";
	//-----------------------------------------------------------------------
	const String& RibbonTrailFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	MovableObject* RibbonTrailFactory::createInstanceImpl( const String& name,
		const NameValuePairList* params)
	{
		size_t maxElements = 20;
		size_t numberOfChains = 1;
		bool useTex = true;
		bool useCol = true;
		// optional params
		if (params != 0)
		{
			NameValuePairList::const_iterator ni = params->find("maxElements");
			if (ni != params->end())
			{
				maxElements = StringConverter::parseUnsignedLong(ni->second);
			}
			ni = params->find("numberOfChains");
			if (ni != params->end())
			{
				numberOfChains = StringConverter::parseUnsignedLong(ni->second);
			}
			ni = params->find("useTextureCoords");
			if (ni != params->end())
			{
				useTex = StringConverter::parseBool(ni->second);
			}
			ni = params->find("useVertexColours");
			if (ni != params->end())
			{
				useCol = StringConverter::parseBool(ni->second);
			}

		}

		return new RibbonTrail(name, maxElements, numberOfChains, useTex, useCol);

	}
	//-----------------------------------------------------------------------
	void RibbonTrailFactory::destroyInstance( MovableObject* obj)
	{
		delete obj;
	}




}


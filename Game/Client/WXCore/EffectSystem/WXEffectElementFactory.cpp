/********************************************************************
filename:   WXEffectElementFactory.cpp

purpose:    the element factory create the effect element
*********************************************************************/

// fairy header
#include "WXEffectElementFactory.h"
#include "WXEffectElement.h"
#include "WXParticleElement.h"
#include "WXMeshElement.h"
#include "WXBillboardSetElement.h"
#include "WXProjectorElement.h"
#include "WXBeamElement.h"
#include "WXPointLightElement.h"

namespace WX	{

	void EffectElementFactory::destroyElement( EffectElement *e )
	{
		assert (e);

		for ( Elements::iterator i = mElements.begin();
			i != mElements.end();	++i	)
		{
			if ((*i) == e)
			{
				delete e;
				e = NULL;
				mElements.erase(i);
			}
		}
	}

	///////////////////////////////////////////////////////////
	ParticleElementFactory::ParticleElementFactory()
	{}

	ParticleElementFactory::~ParticleElementFactory()
	{
	}

	EffectElement *ParticleElementFactory::createElement(System *system)
	{
		assert (system);
		EffectElement *element = new ParticleElement("Particle", system);
		assert (element);
		mElements.push_back(element);
		return element;
	}

	//////////////////////////////////////////////////////////
	MeshElementFactory::MeshElementFactory()
	{}

	MeshElementFactory::~MeshElementFactory()
	{
	}

	EffectElement *MeshElementFactory::createElement(System *system)
	{
		assert (system);
		EffectElement *element = new MeshElement("Mesh", system);
		assert (element);
		mElements.push_back(element);
		return element;
	}

	//////////////////////////////////////////////////////////
	BillboardSetElementFactory::BillboardSetElementFactory()
	{}

	BillboardSetElementFactory::~BillboardSetElementFactory()
	{
	}

	EffectElement *BillboardSetElementFactory::createElement(System *system)
	{
		assert (system);
		EffectElement *element = new BillboardSetElement("BillboardSet", system);
		assert (element);
		mElements.push_back(element);
		return element;
	}

	//////////////////////////////////////////////////////////
	ProjectorElementFactory::ProjectorElementFactory()
	{}

	ProjectorElementFactory::~ProjectorElementFactory()
	{
	}

	EffectElement *ProjectorElementFactory::createElement(System *system)
	{
		assert (system);
		EffectElement *element = new ProjectorElement("Projector", system);
		assert (element);
		mElements.push_back(element);
		return element;
	}

	//////////////////////////////////////////////////////////
	BeamElementFactory::BeamElementFactory()
	{}

	BeamElementFactory::~BeamElementFactory()
	{
	}

	EffectElement *BeamElementFactory::createElement(System *system)
	{
		assert (system);
		EffectElement *element = new BeamElement("Beam", system);
		assert (element);
		mElements.push_back(element);
		return element;
	}

	//////////////////////////////////////////////////////////
	PointLightElementFactory::PointLightElementFactory()
	{}

	PointLightElementFactory::~PointLightElementFactory()
	{
	}

	EffectElement *PointLightElementFactory::createElement(System *system)
	{
		assert (system);
		EffectElement *element = new PointLightElement("PointLight", system);
		assert (element);
		mElements.push_back(element);
		return element;
	}
}
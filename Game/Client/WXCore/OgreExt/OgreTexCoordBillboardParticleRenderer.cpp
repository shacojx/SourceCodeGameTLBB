/********************************************************************
filename:   OgreTexCoordBillboardParticleRenderer.cpp

purpose:    this particle renderer is used to renderer particle that want to 
change the texture coordinate at runtime. 
*********************************************************************/

#include "OgreTexCoordBillboardParticleRenderer.h"
#include "OgreParticle.h"
#include "OgreStringConverter.h"

namespace Ogre {

	String rendererTypeName = "texcoord_billboard";

	/** 由于在_updateRenderQueue中需要设置Billboard的宽与高，由于
		TexCoordBillboardParticleRenderer不是Billboard的友元，所以不能
		直接设置Billboard的mWidth和mHeight属性，而且不能调用Billboard的
		setDimensions，因为mParentSet为空，所以由Billboard派生了一个新
		的类型BillboardForTexcoordBillboardRenderer使我们能设置mWidth
		和mHeight属性
	*/
	struct BillboardForTexcoordBillboardRenderer : public Billboard
	{
	public:
		void _setDimensions(bool o, Real w, Real h)
		{
			if (mOwnDimensions = o)
			{
				mWidth = w;
				mHeight = h;
			}
		}
	};

	//-----------------------------------------------------------------------
	TexCoordBillboardParticleRenderer::CmdStacks TexCoordBillboardParticleRenderer::msStacksCmd;
	TexCoordBillboardParticleRenderer::CmdSlices TexCoordBillboardParticleRenderer::msSlicesCmd;
	TexCoordBillboardParticleRenderer::CmdRepeatTimes TexCoordBillboardParticleRenderer::msRepeatTimesCmd;

	//-----------------------------------------------------------------------
	TexCoordBillboardParticleRenderer::TexCoordBillboardParticleRenderer()
	{
		if (createParamDictionary("TexCoordBillboardParticleRenderer"))
		{
			ParamDictionary* dict = getParamDictionary();
			dict->addParameter(ParameterDef("billboard_type", 
				"The type of billboard to use. 'point' means a simulated spherical particle, " 
				"'oriented_common' means all particles in the set are oriented around common_direction, "
				"'oriented_self' means particles are oriented around their own direction, "
				"'perpendicular_common' means all particles are perpendicular to common_direction, "
				"and 'perpendicular_self' means particles are perpendicular to their own direction.",
				PT_STRING),
				&msBillboardTypeCmd);

            dict->addParameter(ParameterDef("billboard_origin", 
                "This setting controls the fine tuning of where a billboard appears in relation to it's position. "
                "Possible value are: 'top_left', 'top_center', 'top_right', 'center_left', 'center', 'center_right', "
                "'bottom_left', 'bottom_center' and 'bottom_right'. Default value is 'center'.",
                PT_STRING),
                &msBillboardOriginCmd);

            dict->addParameter(ParameterDef("billboard_rotation_type", 
                "This setting controls the billboard rotation type. "
                "'vertex' means rotate the billboard's vertices around their facing direction."
                "'texcoord' means rotate the billboard's texture coordinates. Default value is 'texcoord'.",
                PT_STRING),
                &msBillboardRotationTypeCmd);

			dict->addParameter(ParameterDef("common_direction", 
				"Only useful when billboard_type is oriented_common or perpendicular_common. "
				"When billboard_type is oriented_common, this parameter sets the common orientation for "
				"all particles in the set (e.g. raindrops may all be oriented downwards). "
				"When billboard_type is perpendicular_common, this parameter sets the perpendicular vector for "
				"all particles in the set (e.g. an aureola around the player and parallel to the ground).",
				PT_VECTOR3),
				&msCommonDirectionCmd);

			dict->addParameter(ParameterDef("common_up_vector",
				"Only useful when billboard_type is perpendicular_self or perpendicular_common. This "
				"parameter sets the common up-vector for all particles in the set (e.g. an aureola around "
				"the player and parallel to the ground).",
				PT_VECTOR3),
				&msCommonUpVectorCmd);

            dict->addParameter(ParameterDef("point_rendering",
                "Set whether or not particles will use point rendering "
                "rather than manually generated quads. This allows for faster "
                "rendering of point-oriented particles although introduces some "
                "limitations too such as requiring a common particle size."
                "Possible values are 'true' or 'false'.",
                PT_BOOL),
                &msPointRenderingCmd);
            dict->addParameter(ParameterDef("accurate_facing",
                "Set whether or not particles will be oriented to the camera "
                "based on the relative position to the camera rather than just "
                "the camera direction. This is more accurate but less optimal. "
                "Cannot be combined with point rendering.",
                PT_BOOL),
                &msAccurateFacingCmd);

            dict->addParameter(ParameterDef("speed_relatived_size_factor",
                "Sets the factor of the particle size relative to speed. If greater "
                "than zero, the particle size are relative to its speed as well as "
                "this factor. Otherwise, the particle rendering use its own size "
                "only. This parameters applies only to 'oriented_self' billboards. "
                "Default to zero as particle size unrelated to speed",
                PT_REAL),
                &msSpeedRelativedSizeFactorCmd);

			dict->addParameter(ParameterDef("stacks", 
				"the stacks of texture coordinates.",
				PT_UNSIGNED_LONG),
				&msStacksCmd);

			dict->addParameter(ParameterDef("slices",
				"the slices of texture coordinates.",
				PT_UNSIGNED_LONG),
				&msSlicesCmd);

			dict->addParameter(ParameterDef("repeat_times", 
				"the repeat times of the colour interpolate.",
				PT_REAL), &msRepeatTimesCmd);
		}

		mStacks = 1;
		mSlices = 1;

		mRepeatTimes = 1.0f;
	}
	//-----------------------------------------------------------------------
	const String& TexCoordBillboardParticleRenderer::getType(void) const
	{
		return rendererTypeName;
	}	
	//-----------------------------------------------------------------------
	void TexCoordBillboardParticleRenderer::_updateRenderQueue(RenderQueue* queue, 
		std::list<Particle*>& currentParticles, bool cullIndividually)
	{	
		mBillboardSet->setCullIndividually(cullIndividually);

		ulong numTexCoords = mSlices * mStacks;
	//	--numTexCoords;

		Real life_time = 0.0f;
		Real particle_time = 0.0f;
		Real repeatedParticleTime = 0.0f;
		uint16 texCoords = 0;

		// Update billboard set geometry
		mBillboardSet->beginBillboards();
		BillboardForTexcoordBillboardRenderer bb;

		for (std::list<Particle*>::iterator i = currentParticles.begin();
			i != currentParticles.end(); ++i)
		{
			Particle* p = *i;
			bb.mPosition = p->position;

            switch (mBillboardSet->getBillboardType())
            {
            case BBT_ORIENTED_SELF:
                if (mSpeedRelativedSizeFactor > 0)
                {
                    bb.mDirection = p->direction;
                    bb.mDirection *= mSpeedRelativedSizeFactor;
                    break;
                }
                // fall through

            case BBT_PERPENDICULAR_SELF:
                // Normalise direction vector
                bb.mDirection = p->direction;
                bb.mDirection.normalise();
                break;
            }

			bb.mColour = p->colour;
			bb.mRotation = p->rotation;

			life_time		= p->totalTimeToLive;
			particle_time	= 1.0f - (p->timeToLive / life_time); 

			// wrap the particle time
			repeatedParticleTime = fmod( particle_time * mRepeatTimes, 1.0f );

			texCoords = (uint16)(repeatedParticleTime * numTexCoords);

			if (texCoords == numTexCoords)
				--texCoords;

			bb.setTexcoordIndex(texCoords);

			bb._setDimensions(p->mOwnDimensions, p->mWidth, p->mHeight);

			mBillboardSet->injectBillboard(bb);
		}

		mBillboardSet->endBillboards();		

		// Update the queue
		mBillboardSet->_updateRenderQueue(queue);
	}
	//-----------------------------------------------------------------------
	void TexCoordBillboardParticleRenderer::setStacks(ulong stacks)
	{
		if (stacks < 1)
			mStacks = 1;
		else
			mStacks = stacks;

		mBillboardSet->setTextureStacksAndSlices((uchar)mStacks, (uchar)mSlices);
	}
	//-----------------------------------------------------------------------
	void TexCoordBillboardParticleRenderer::setSlices(ulong slices)
	{
		if (slices < 1)
			mSlices = 1;
		else
			mSlices = slices;

		mBillboardSet->setTextureStacksAndSlices((uchar)mStacks, (uchar)mSlices);
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	const String& TexCoordBillboardParticleRendererFactory::getType() const
	{
		return rendererTypeName;
	}
	//-----------------------------------------------------------------------
	ParticleSystemRenderer* TexCoordBillboardParticleRendererFactory::createInstance( 
		const String& name )
	{
		return new TexCoordBillboardParticleRenderer();
	}
	//-----------------------------------------------------------------------
	void TexCoordBillboardParticleRendererFactory::destroyInstance( 
		ParticleSystemRenderer* inst)
	{
		delete inst;
	}
	//-----------------------------------------------------------------------	
	//-----------------------------------------------------------------------
	String TexCoordBillboardParticleRenderer::CmdStacks::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const TexCoordBillboardParticleRenderer*>(target)->getStacks() );
	}
	void TexCoordBillboardParticleRenderer::CmdStacks::doSet(void* target, const String& val)
	{
		static_cast<TexCoordBillboardParticleRenderer*>(target)->setStacks(
			StringConverter::parseUnsignedLong(val));
	}
	//-----------------------------------------------------------------------
	String TexCoordBillboardParticleRenderer::CmdSlices::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const TexCoordBillboardParticleRenderer*>(target)->getSlices() );
	}
	void TexCoordBillboardParticleRenderer::CmdSlices::doSet(void* target, const String& val)
	{
		static_cast<TexCoordBillboardParticleRenderer*>(target)->setSlices(
			StringConverter::parseUnsignedLong(val));
	}
	//-----------------------------------------------------------------------
	String TexCoordBillboardParticleRenderer::CmdRepeatTimes::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const TexCoordBillboardParticleRenderer*>(target)->getRepeatTimes() );
	}
	void TexCoordBillboardParticleRenderer::CmdRepeatTimes::doSet(void* target, const String& val)
	{
		static_cast<TexCoordBillboardParticleRenderer*>(target)->setRepeatTimes(
			StringConverter::parseReal(val));
	}
}


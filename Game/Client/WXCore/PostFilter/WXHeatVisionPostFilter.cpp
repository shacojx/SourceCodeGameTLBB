#include "WXPostFilter.h"
#include "WXPostFilterFactory.h"

#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include <OgreCompositionTechnique.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>

#include <OgreGpuProgram.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreControllerManager.h>

namespace WX {

using namespace Ogre;

class HeatVisionPostFilter
    : public PostFilter
    , public CompositorInstance::Listener
{
private:
    class Factory : public PostFilterFactory
    {
    public:
        static const String& _theType(void)
        {
            static const String type = "HeatVision";
            return type;
        }

        const String& getType(void) const
        {
            return _theType();
        }

        PostFilter* createInstance(const String& name)
        {
            return new HeatVisionPostFilter(name, _theType());
        }

        void destroyInstance(PostFilter* postFilter)
        {
            delete postFilter;
        }

        void _init(void)
        {
            CompositorPtr compositor = CompositorManager::getSingleton().create(
                _theType(), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			CompositionTechnique *t;
            CompositionTechnique::TextureDefinition *td;
            CompositionTargetPass *tp;
            CompositionPass *pass;

            t = compositor->createTechnique();
            td = t->createTextureDefinition("scene");
            td->width.viewport = 1;
            td->height.viewport = 1;
            td->format = PF_X8R8G8B8;
            td = t->createTextureDefinition("heat");
            td->width.viewport = 0.5;
            td->height.viewport = 0.5;
            td->format = PF_X8R8G8B8;

            tp = t->createTargetPass();
            tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
            tp->setOutputName("scene");

            tp = t->createTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("heat");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/HeatVision/LightToHeat");
            pass->setInput(0, "scene");
            pass->setIdentifier(0xDEADBADE);

            tp = t->getOutputTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/HeatVision/Blur");
            pass->setInput(0, "heat");
        }
    };

public:
    static Factory* getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

    const String& getType(void) const
    {
        return Factory::_theType();
    }

public:
    HeatVisionPostFilter(const String& name, const String& compositor)
        : PostFilter(name, compositor)
        , mInc()
        , mStart()
        , mEnd()
        , mCurrent()
    {
    }

    ~HeatVisionPostFilter()
    {
        if (mCompositorInstance)
            mCompositorInstance->removeListener(this);
    }

    // from PostFilter
public:

    void _init(Viewport* vp)
    {
        PostFilter::_init(vp);
        if (mCompositorInstance)
            mCompositorInstance->addListener(this);
    }

    // from CompositorInstance::Listener
public:

    void notifyMaterialSetup(uint32 pass_id, MaterialPtr &mat)
    {
        if (pass_id == 0xDEADBADE)
        {
            mFragmentProgramParameters = mat->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
        }
    }

	void notifyMaterialRender(uint32 pass_id, MaterialPtr &mat)
    {
        if (pass_id == 0xDEADBADE)
        {
            // "random_fractions" parameter
            mFragmentProgramParameters->setNamedConstant("random_fractions",
                Vector4(Math::RangeRandom(0.0, 1.0), Math::RangeRandom(0, 1.0), 0, 0));

            // "depth_modulator" parameter
            mInc = ControllerManager::getSingleton().getFrameTimeSource()->getValue() / 2;

            if ((Math::RealEqual(mCurrent, mEnd, 0.001)))
            {
                // take a new value to reach
                mEnd = Math::RangeRandom(0.95, 1.0);
                mStart = mCurrent;
            }

            if (mCurrent > mEnd)
                mCurrent -= mInc;
            else
                mCurrent += mInc;

            mFragmentProgramParameters->setNamedConstant("depth_modulator", Vector4(mCurrent, 0, 0, 0));
        }
    }

private:
    Real mInc, mStart, mEnd, mCurrent;
    GpuProgramParametersSharedPtr mFragmentProgramParameters;
};  

//////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////

extern PostFilterFactory* getHeatVisionPostFilterFactory(void)
{
    return HeatVisionPostFilter::getFactory();
}

}

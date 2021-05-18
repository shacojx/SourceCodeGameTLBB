#include "WXPostFilter.h"
#include "WXPostFilterFactory.h"

#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include <OgreCompositionTechnique.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>

#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreGpuProgram.h>
#include <OgreStringConverter.h>

namespace WX {

using namespace Ogre;

class FloodlightingPostFilter
    : public PostFilter
    , public CompositorInstance::Listener
{
private:
    class Factory : public PostFilterFactory
    {
    public:
        static const String& _theType(void)
        {
            static const String type = "Floodlighting";
            return type;
        }

        const String& getType(void) const
        {
            return _theType();
        }

        PostFilter* createInstance(const String& name)
        {
            return new FloodlightingPostFilter(name, _theType());
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
            td = t->createTextureDefinition("blur0");
            td->width.viewport = 0.5;
            td->height.viewport = 0.5;
            td->format = PF_X8R8G8B8;
            td = t->createTextureDefinition("blur1");
            td->width.viewport = 0.5;
            td->height.viewport = 0.5;
            td->format = PF_X8R8G8B8;

            tp = t->createTargetPass();
            tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
            tp->setOutputName("scene");

            tp = t->createTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("blur0");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Floodlighting/DownSample");
            pass->setInput(0, "scene");

            tp = t->createTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("blur1");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Floodlighting/BlurHorizontal");
            pass->setInput(0, "blur0");

            tp = t->createTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            tp->setOutputName("blur1");
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Floodlighting/BlurVertical");
            pass->setInput(0, "blur0");

            tp = t->getOutputTargetPass();
			tp->setInputMode(CompositionTargetPass::IM_NONE);
            pass = tp->createPass();
            pass->setType(CompositionPass::PT_RENDERQUAD);
            pass->setMaterialName("PostFilters/Floodlighting/Blend");
            pass->setInput(0, "scene");
            pass->setInput(1, "blur1");
            pass->setIdentifier(0xDEADBADE);

            mBlurAmount = 0.2f;
            mShineAmount = 0.3f;
            // receive default parameters from material script
            if (!pass->getMaterial().isNull())
            {
                GpuProgramParametersSharedPtr parameters = pass->getMaterial()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
#if OGRE_VERSION >= 0x010300
                const GpuConstantDefinition* def;
                
                def = parameters->_findNamedConstantDefinition("blur_amount");
                if (def)
                    parameters->_readRawConstants(def->physicalIndex, 1, &mBlurAmount);
                def = parameters->_findNamedConstantDefinition("shine_amount");
                if (def)
                    parameters->_readRawConstants(def->physicalIndex, 1, &mShineAmount);
#else
                GpuProgramParameters::RealConstantEntry* entry;

                entry = parameters->getNamedRealConstantEntry("blur_amount");
                if (entry && entry->isSet)
                    mBlurAmount = entry->val[0];

                entry = parameters->getNamedRealConstantEntry("shine_amount");
                if (entry && entry->isSet)
                    mShineAmount = entry->val[0];
#endif
            }
        }

        Real mBlurAmount;
        Real mShineAmount;
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
    FloodlightingPostFilter(const String& name, const String& compositor)
        : PostFilter(name, compositor)
        , mBlurAmount(getFactory()->mBlurAmount)
        , mShineAmount(getFactory()->mShineAmount)
    {
    }

    ~FloodlightingPostFilter()
    {
        if (mCompositorInstance)
            mCompositorInstance->removeListener(this);
    }

    // from PostFilter
public:

    void setParameter(const String& name, const String& value)
    {
        if (name == "BlurAmount")
        {
            mBlurAmount = StringConverter::parseReal(value);
        }
        else if (name == "ShineAmount")
        {
            mShineAmount = StringConverter::parseReal(value);
        }
        else
        {
            PostFilter::setParameter(name, value);
        }
    }

    String getParameter(const String& name) const
    {
        if (name == "BlurAmount")
        {
            return StringConverter::toString(mBlurAmount);
        }
        else if (name == "ShineAmount")
        {
            return StringConverter::toString(mShineAmount);
        }
    
        return PostFilter::getParameter(name);
    }

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
            // update gpu program parameters
            mFragmentProgramParameters->setNamedConstant("blur_amount", mBlurAmount);
            mFragmentProgramParameters->setNamedConstant("shine_amount", mShineAmount);
        }
    }

private:
    Real mBlurAmount;
    Real mShineAmount;
    GpuProgramParametersSharedPtr mFragmentProgramParameters;
};

//////////////////////////////////////////////////////////////////////////
// Public functions
//////////////////////////////////////////////////////////////////////////

extern PostFilterFactory* getFloodlightingPostFilterFactory(void)
{
    return FloodlightingPostFilter::getFactory();
}

}

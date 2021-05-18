#include "ScorpioPrerequisites.h"

#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreTextureUnitState.h>
#include <OgreMaterialManager.h>
#include <OgreGpuProgram.h>
#include <OgreGpuProgramManager.h>
#include <OgreStringConverter.h>

namespace Ogre
{

    static const String& getSupportedPostfix(void)
    {
        static const String postfixList[] = { "_hlsl", "_glsl", "_cg" };

        for (size_t i = 0; i < sizeof(postfixList)/sizeof(*postfixList); ++i)
        {
            const String& postfix = postfixList[i];
            GpuProgramPtr vp =
                GpuProgramManager::getSingleton().getByName("Scorpio/HardwareAnimation/Skinning/OneWeight_NoTex_AmbientOnly" + postfix);
            if (!vp.isNull())
            {
                vp->load();
                if (vp->isSupported())
                {
                    return postfix;
                }
            }
        }

        return StringUtil::BLANK;
    }

    static MaterialPtr deriveHardwareSkinningMaterial(const MaterialPtr& origin,
        const String& prefix, const String& postfix, size_t maxSimultaneousLights)
    {
        String derivedMaterialName = origin->getName() +
            " : " + prefix + StringConverter::toString(maxSimultaneousLights) + "_";
        MaterialPtr derived = MaterialManager::getSingleton().getByName(derivedMaterialName);
        if (derived.isNull())
        {
            derived = origin->clone(derivedMaterialName);
            Technique* newTechnique = derived->createTechnique();
            Technique* technique = derived->getTechnique(0);
            *newTechnique = *technique;

            String baseName = prefix;

            Pass* pass = technique->getPass(0);
            if (pass->isAmbientOnly())
            {
                pass->setVertexProgram(baseName + "AmbientOnly" + postfix);
                pass->getVertexProgramParameters()->setNamedAutoConstant("ambient", GpuProgramParameters::ACT_DERIVED_SCENE_COLOUR);
                pass->setShadowCasterVertexProgram(baseName + "AmbientOnly" + postfix);
                pass->getShadowCasterVertexProgramParameters()->setNamedAutoConstant("ambient", GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
                pass->setShadowReceiverVertexProgram(baseName + "Unlit" + postfix);
            }
            else if (0 < maxSimultaneousLights && maxSimultaneousLights <= 3)
            {
                static const String lightsNameList[] = { "OneLight", "TwoLight", "ThreeLight" };
                pass->setVertexProgram(baseName + lightsNameList[maxSimultaneousLights-1] + postfix);
                pass->setShadowCasterVertexProgram(baseName + "AmbientOnly" + postfix);
                pass->getShadowCasterVertexProgramParameters()->setNamedAutoConstant("ambient", GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
                pass->setShadowReceiverVertexProgram(baseName + "Unlit" + postfix);
            }
            else if (pass->isTransparent())
            {
                pass->setVertexProgram(baseName + "ThreeLight" + postfix);
                pass->setShadowCasterVertexProgram(baseName + "AmbientOnly" + postfix);
                pass->getShadowCasterVertexProgramParameters()->setNamedAutoConstant("ambient", GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
                pass->setShadowReceiverVertexProgram(baseName + "Unlit" + postfix);
            }
            else
            {
                Pass* preDirectionalLightPass = technique->createPass();
                Pass* prePointLightPass = technique->createPass();
                Pass* preSpotLightPass = technique->createPass();
                Pass* decalPass = technique->createPass();

                // Setup decal pass
                *decalPass = *pass;
                decalPass->setLightingEnabled(false);
                decalPass->setSceneBlending(SBT_MODULATE);
                decalPass->setVertexProgram(baseName + "AmbientOnly" + postfix);
                decalPass->getVertexProgramParameters()->setNamedConstant("ambient", ColourValue::White);

                if (pass->getAlphaRejectFunction() == CMPF_ALWAYS_PASS)
                {
                    pass->removeAllTextureUnitStates();
                    baseName += "NoTex_";
                }
                *preDirectionalLightPass = *pass;
                *prePointLightPass = *pass;
                *preSpotLightPass = *pass;

                // Setup ambient pass
                pass->setLightingEnabled(false);
                pass->setVertexProgram(baseName + "AmbientOnly" + postfix);
                pass->getVertexProgramParameters()->setNamedAutoConstant("ambient", GpuProgramParameters::ACT_DERIVED_SCENE_COLOUR);
                pass->setShadowCasterVertexProgram(baseName + "AmbientOnly" + postfix);
                pass->getShadowCasterVertexProgramParameters()->setNamedAutoConstant("ambient", GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
                pass->setShadowReceiverVertexProgram(baseName + "Unlit" + postfix);

                preDirectionalLightPass->setSceneBlending(SBT_ADD);
                preDirectionalLightPass->setIteratePerLight(true, true, Light::LT_DIRECTIONAL);
                preDirectionalLightPass->setVertexProgram(baseName + "DirectionalLight" + postfix);

                prePointLightPass->setSceneBlending(SBT_ADD);
                prePointLightPass->setIteratePerLight(true, true, Light::LT_POINT);
                prePointLightPass->setVertexProgram(baseName + "PointLight" + postfix);

                preSpotLightPass->setSceneBlending(SBT_ADD);
                preSpotLightPass->setIteratePerLight(true, true, Light::LT_SPOTLIGHT);
                preSpotLightPass->setVertexProgram(baseName + "PointLight" + postfix);
            }
        }

        return derived;
    }

    bool _OgreExport convertEntityToHardwareSkinning(Entity* entity, size_t maxSimultaneousLights)
    {
        if (entity->isHardwareAnimationEnabled())
            return false;

        if (!entity->hasSkeleton() || entity->hasVertexAnimation())
            return false;

        static const String& postfix = getSupportedPostfix();
        if (postfix.empty())
            return false;

        // Derive hardware skinning materials, only modify origin material
        // when all sub-entities can dealing with hardware skinning.
        std::vector<MaterialPtr> derivedMaterials;
        for (unsigned short i = 0; i < entity->getNumSubEntities(); ++i)
        {
            SubEntity* se = entity->getSubEntity(i);
            SubMesh* sm = se->getSubMesh();

            MaterialPtr origin = MaterialManager::getSingleton().getByName(sm->getMaterialName());
            if (origin.isNull())
                return false;

            if (origin->getNumTechniques() != 1)
                return false;

            Technique* technique = origin->getTechnique(0);
            if (technique->getNumPasses() != 1)
                return false;

            Pass* pass = technique->getPass(0);
            if (pass->hasVertexProgram())
            {
                if (!pass->getVertexProgram()->isSkeletalAnimationIncluded())
                    return false;
                if (!pass->getVertexProgram()->isSupported())
                    return false;

                derivedMaterials.push_back(origin);
                continue;
            }

            if (pass->getNumTextureUnitStates() != 1)
                return false;

            const VertexData* vertexData;
            const Mesh::IndexMap* indexMap;
            if (sm->useSharedVertices)
            {
                vertexData = sm->parent->sharedVertexData;
                indexMap = &sm->parent->sharedBlendIndexToBoneIndexMap;
            }
            else
            {
                vertexData = sm->vertexData;
                indexMap = &sm->blendIndexToBoneIndexMap;
            }

            const VertexElement* blendWeights =
                vertexData->vertexDeclaration->findElementBySemantic(VES_BLEND_WEIGHTS);
            if (!blendWeights)
                return false;

            size_t numWeightsPerVertex = VertexElement::getTypeCount(blendWeights->getType());
            size_t numWorldMatrices = indexMap->size();
            if (numWorldMatrices > 25)
                return false;

            static const String weightNames[] = { "OneWeight", "TwoWeight", "ThreeWeight", "FourWeight" };
            const String& weightName = weightNames[numWeightsPerVertex-1];
            String prefix = "Scorpio/HardwareAnimation/Skinning/" + weightName + "_";

            MaterialPtr derived = deriveHardwareSkinningMaterial(origin, prefix, postfix, maxSimultaneousLights);
            derivedMaterials.push_back(derived);
        }

        // Change the materials
        for (unsigned short i = 0; i < entity->getNumSubEntities(); ++i)
        {
            SubEntity* se = entity->getSubEntity(i);
            se->setMaterialName(derivedMaterials[i]->getName());
        }

        return true;
    }

}

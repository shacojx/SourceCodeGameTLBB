#include "WXMaterialScriptSolver.h"
#include "WXResourceSolverHelper.h"

#include <OgreResourceGroupManager.h>
#include <OgreMaterialManager.h>
#include <OgreTextureManager.h>
#include <OgreTextureUnitState.h>
#include <OgrePass.h>
#include <OgreTechnique.h>
#include <OgreMaterial.h>
#include <OgreMaterialSerializer.h>

namespace WX {

    //////////////////////////////////////////////////////////////////////////

    class MaterialScriptSolver : public ResourceSolverBase
    {
    public:
        ResourceLocationSet modifiedScripts;

        MaterialScriptSolver(void)
            : ResourceSolverBase()
            , modifiedScripts()
        {
        }

        void addModifiedScripts(const ResourceLocationSet& scripts)
        {
            modifiedScripts.insert(scripts.begin(), scripts.end());
        }

        const String& getName(void) const
        {
            static const String name = "MaterialScript";
            return name;
        }

        bool commitPermanent(void)
        {
            Ogre::MaterialManager& mm = Ogre::MaterialManager::getSingleton();

            typedef std::map<ResourceLocation, String> ResourceLocationFilenameMap;
            typedef std::map<String, ResourceLocation> ResourceFilenameLocationMap;
            ResourceLocationFilenameMap resourceLocationFilenameMap;
            ResourceFilenameLocationMap resourceFilenameLocationMap;
            for (ResourceLocationSet::const_iterator it = modifiedScripts.begin(); it != modifiedScripts.end(); ++it)
            {
                const ResourceLocation& resourceLocation = *it;
                StringVectorPtr filenames = findResourceFilenames(resourceLocation.first, resourceLocation.second);
                if (filenames.isNull() || filenames->empty())
                {
                    // FIXME: The resource may be in a zip file
                }
                else if (filenames->size() == 1)
                {
                    const String& filename = *filenames->begin();
                    // The difference resource location may be reference to same file
                    if (resourceFilenameLocationMap.insert(ResourceFilenameLocationMap::value_type(filename, resourceLocation)).second)
                    {
                        resourceLocationFilenameMap.insert(ResourceLocationFilenameMap::value_type(resourceLocation, filename));
                    }
                }
                else
                {
                    // FIXME: Multi-group contains same qualified name, resource location describe in difference path
                }
            }

            typedef std::list<Ogre::MaterialPtr> MaterialList;
            typedef std::map<String, MaterialList> QueuedScripts;
            QueuedScripts queuedScripts;
            Ogre::MaterialManager::ResourceMapIterator mi = mm.getResourceIterator();
            while (mi.hasMoreElements())
            {
                Ogre::MaterialPtr material = mi.getNext();
                const String& groupName = material->getGroup();
                const String& scriptName = material->getOrigin();
                ResourceLocationFilenameMap::const_iterator it = resourceLocationFilenameMap.find(ResourceLocation(groupName, scriptName));
                if (it != resourceLocationFilenameMap.end())
                {
                    queuedScripts[it->second].push_back(material);
                }
            }

            Ogre::MaterialSerializer serializer;
            for (QueuedScripts::const_iterator it = queuedScripts.begin(); it != queuedScripts.end(); ++it)
            {
                for (MaterialList::const_iterator itMaterial = it->second.begin(); itMaterial != it->second.end(); ++itMaterial)
                {
                    serializer.queueForExport(*itMaterial, false, false);
                }
                serializer.exportQueued(it->first);
            }

            modifiedScripts.clear();
            return !queuedScripts.empty();
        }
    };

    //////////////////////////////////////////////////////////////////////////

    class UnsolvedMaterialTexture : public UnsolvedResourceSimple
    {
    public:
        MaterialScriptSolver* solver;
        ResourceLocationSet referenceScripts;

        UnsolvedMaterialTexture(MaterialScriptSolver* solver)
            : UnsolvedResourceSimple()
            , solver(solver)
            , referenceScripts()
        {
            assert(solver);
        }

        bool setResolvedValue(const String& value)
        {
            bool ok = UnsolvedResourceSimple::setResolvedValue(value);
            solver->addModifiedScripts(referenceScripts);
            return ok;
        }
    };

    //////////////////////////////////////////////////////////////////////////

    ResourceSolverPtr findUnsolvedMaterialTextures(const String& group)
    {
        Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
        Ogre::TextureManager& tm = Ogre::TextureManager::getSingleton();
        Ogre::MaterialManager& mm = Ogre::MaterialManager::getSingleton();

        MaterialScriptSolver* solver = new MaterialScriptSolver;
        ResourceSolverPtr holder(solver);

        typedef std::map<String, StringVectorPtr> PossibleMap;
        PossibleMap possibleMap;
        typedef std::map<String, UnsolvedMaterialTexture*> TextureNames;
        TextureNames textureNames;
        Ogre::MaterialManager::ResourceMapIterator mi = mm.getResourceIterator();
        while (mi.hasMoreElements())
        {
            Ogre::MaterialPtr material = mi.getNext();
            const String& groupName = material->getGroup();
            if (!group.empty() && group != groupName)
                continue;

            const String& materialName = material->getName();
            const String& scriptName = material->getOrigin();
            Ogre::Material::TechniqueIterator ti = material->getTechniqueIterator();
            while (ti.hasMoreElements())
            {
                Ogre::Technique* technique = ti.getNext();
                Ogre::Technique::PassIterator pi = technique->getPassIterator();
                while (pi.hasMoreElements())
                {
                    Ogre::Pass* pass = pi.getNext();
                    Ogre::Pass::TextureUnitStateIterator tusi = pass->getTextureUnitStateIterator();
                    while (tusi.hasMoreElements())
                    {
                        Ogre::TextureUnitState* tus = tusi.getNext();

                        // Skip texture unit that exists texture alias name
                        if (!tus->getTextureNameAlias().empty())
                            continue;

                        for (size_t frame = 0; frame < tus->getNumFrames(); ++frame)
                        {
                            const String& textureName = tus->getFrameTextureName(frame);

                            if (textureName.empty() || (*textureName.begin() == '<' && *textureName.rbegin() == '>'))
                            {
                                // The texture name is place holder
                                continue;
                            }

                            std::pair<TextureNames::iterator, bool> inserted =
                                textureNames.insert(TextureNames::value_type(textureName, nullptr));
                            if (!inserted.second)
                            {
                                // The texture name already checked
                                if (!inserted.first->second)
                                {
                                    // The texture exists
                                    continue;
                                }
                            }
                            else
                            {
                                // First time to check the texture
								Ogre::String grp = groupName;
                                bool found = !tm.getByName(textureName).isNull() || rgm.resourceExists(grp, textureName);
                                if (found)
                                {
                                    continue;
                                }

                                // Create new unsolved resource
                                UnsolvedMaterialTexture* item = new UnsolvedMaterialTexture(solver);
                                solver->unsolvedResources.push_back(UnsolvedResourcePtr(item));

                                // Setup origin name
                                item->origin = textureName;

                                // Setup description
                                Ogre::StringUtil::StrStreamType str;
                                if (scriptName.empty())
                                {
                                    str << "Reference by material '" << materialName
                                        << "' in group '" << groupName << "'";
                                }
                                else
                                {
                                    str << "Reference by material '" << materialName
                                        << "' within script file '" << scriptName
                                        << "' in group '" << groupName << "'";
                                }
                                item->description = str.str();

                                // Setup recommend values
                                String textureBasename, texturePath;
                                Ogre::StringUtil::splitFilename(textureName, textureBasename, texturePath);
                                String scriptBasename, scriptPath;
                                Ogre::StringUtil::splitFilename(scriptName, scriptBasename, scriptPath);
                                item->recommends = rgm.findResourceNames(groupName, textureBasename);
                                // Move the better match name to front
                                if (scriptPath != texturePath)
                                {
                                    // Find in script path with base name
                                    moveMatchToFront(*item->recommends, scriptPath + textureBasename);
                                }
                                if (!scriptPath.empty() && !texturePath.empty())
                                {
                                    // Find in script path with full qualified name
                                    moveMatchToFront(*item->recommends, scriptPath + textureName);
                                }

                                // Setup possible values
                                PossibleMap::const_iterator itPossible = possibleMap.find(groupName);
                                if (itPossible != possibleMap.end())
                                {
                                    item->possibles = itPossible->second;
                                }
                                else
                                {
                                    item->possibles = findImageResources(groupName);
                                    possibleMap.insert(PossibleMap::value_type(groupName, item->possibles));
                                }

                                inserted.first->second = item;
                            }

                            // HACK
                            inserted.first->second->references.push_back(const_cast<String*>(&textureName));
                            inserted.first->second->referenceScripts.insert(ResourceLocation(groupName, scriptName));
                        }
                    }
                }
            }
        }

        return holder;
    }
}

#include "WXUtils.h"
#include "WXResources.h"

#include <OgreMeshManager.h>
#include <OgreMaterialManager.h>
#include <OgreFrustum.h>
#include <OgreHardwareBufferManager.h>
#include <OgreSubMesh.h>
#include <OgreRoot.h>
#include <OgreEntity.h>
#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) < 0x010100
#include <OgreParticleSystemManager.h>
#endif
#include <OgreBone.h>
#include <OgreAnimation.h>
#include <OgreDynLib.h>
#include <OgreDynLibManager.h>

#include "../../Scorpio/include/Scorpio.h"

namespace WX {

using namespace Ogre;

MeshPtr
createSphere(const String& name,
             const Real radius, const int rings, const int segments,
             const bool normals,
             const int numTexCoordSets,
             const bool inwards)
{
    MeshPtr mesh = MeshManager::getSingleton().createManual(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    SubMesh *subMesh = mesh->createSubMesh();

    VertexData* vertexData = new VertexData();
    mesh->sharedVertexData = vertexData;

    // define the vertex format
    VertexDeclaration* decl = vertexData->vertexDeclaration;
    size_t offset = 0;
    // positions
    decl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
    offset += VertexElement::getTypeSize(VET_FLOAT3);
    // normals
    if (normals)
    {
        decl->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
        offset += VertexElement::getTypeSize(VET_FLOAT3);
    }
    for (int i = 0; i < numTexCoordSets; ++i)
    {
        // two dimensional texture coordinates
        decl->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, i);
        offset += VertexElement::getTypeSize(VET_FLOAT2);
    }

    // allocate the vertex buffer
    vertexData->vertexCount = (rings + 1) * (segments + 1);
    HardwareVertexBufferSharedPtr vbuf =
        HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(0),
            vertexData->vertexCount,
            HardwareBuffer::HBU_STATIC_WRITE_ONLY,
            false);
    VertexBufferBinding* binding = vertexData->vertexBufferBinding;
    binding->setBinding(0, vbuf);

    // allocate index buffer
    subMesh->indexData->indexCount = 6 * rings * (segments + 1);
    subMesh->indexData->indexBuffer =
        HardwareBufferManager::getSingleton().createIndexBuffer(
            HardwareIndexBuffer::IT_16BIT,
            subMesh->indexData->indexCount,
            HardwareBuffer::HBU_STATIC_WRITE_ONLY,
            false);
    HardwareIndexBufferSharedPtr ibuf = subMesh->indexData->indexBuffer;

    // Generate the group of rings for the sphere
    float* pFloat = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
    ushort* pIndices = static_cast<ushort*>(ibuf->lock(HardwareBuffer::HBL_DISCARD));

    Real ringStep = 1 / (Real) rings;
    Real segmentStep = 1 / (Real) segments;
    Real ringAngleStep = Math::PI * ringStep;
    Real segmentAngleStep = Math::TWO_PI * segmentStep;
    ushort vertexIndex = 0;

    for (int ring = 0; ring <= rings; ++ring)
    {
        Real r0 = radius * Math::Sin(ring * ringAngleStep);
        Real y0 = radius * Math::Cos(ring * ringAngleStep);

        // Generate the group of segments for the current ring
        for(int segment = 0; segment <= segments; ++segment)
        {
            Real x0 = r0 * Math::Sin(segment * segmentAngleStep);
            Real z0 = r0 * Math::Cos(segment * segmentAngleStep);

            // Add one vertex to the strip which makes up the sphere
            *pFloat++ = x0;
            *pFloat++ = y0;
            *pFloat++ = z0;

            if (normals)
            {
                Vector3 normal = Vector3(x0, y0, z0).normalisedCopy();
                if (inwards)
                    normal = -normal;
                *pFloat++ = normal.x;
                *pFloat++ = normal.y;
                *pFloat++ = normal.z;
            }

            for (int i = 0; i < numTexCoordSets; ++i)
            {
                *pFloat++ = segment * segmentStep;
                *pFloat++ = ring * ringStep;
            }

            if (ring != rings)
            {
                // each vertex (except the last) has six indicies pointing to it
                if (inwards)
                {
                    *pIndices++ = vertexIndex + segments + 1;
                    *pIndices++ = vertexIndex + segments;
                    *pIndices++ = vertexIndex;               
                    *pIndices++ = vertexIndex + segments + 1;
                    *pIndices++ = vertexIndex;
                    *pIndices++ = vertexIndex + 1;
                }
                else
                {
                    *pIndices++ = vertexIndex + segments + 1;
                    *pIndices++ = vertexIndex;               
                    *pIndices++ = vertexIndex + segments;
                    *pIndices++ = vertexIndex + segments + 1;
                    *pIndices++ = vertexIndex + 1;
                    *pIndices++ = vertexIndex;
                }
                ++vertexIndex;
            }
        }; // end for segment
    } // end for ring

    // Unlock
    ibuf->unlock();
    vbuf->unlock();

    // Generate face list
    subMesh->useSharedVertices = true;

    mesh->_setBoundingSphereRadius(radius);
    mesh->_setBounds(AxisAlignedBox(Vector3(-radius, -radius, -radius), Vector3(radius, radius, radius)), false);

    // Disable auto build edge list because the vertex and index buffer doesn't readable
    mesh->setAutoBuildEdgeLists(false);

    // Makes clear the mesh is loaded (avoids memory leakes)
    mesh->load();

    return mesh;
}

MeshPtr
createCommonSphere(const String& namePrefix,
                   const Real radius, const int rings, const int segments,
                   const bool normals,
                   const int numTexCoordSets,
                   const bool inwards)
{
    StringUtil::StrStreamType str;
    str << namePrefix << '_' << radius << '_' << rings << '*' << segments << '_' << (normals ? "N" : "") << 'T' << numTexCoordSets << '$' << (inwards ? 'I' : 'O');
    String name = str.str();

    ResourcePtr mesh = MeshManager::getSingleton().getByName(name);
    if (!mesh.isNull())
        return mesh;
    else
        return createSphere(name, radius, rings, segments, normals, numTexCoordSets, inwards);
}

MeshPtr
createCommonSphere(const Real radius, const int rings, const int segments,
                   const bool normals,
                   const int numTexCoordSets,
                   const bool inwards)
{
    static const String DefaultNamePrefix("Fairy/Sphere");

    return createCommonSphere(DefaultNamePrefix, radius, rings, segments, normals, numTexCoordSets, inwards);
}

MaterialPtr
createPureColourMaterial(const ColourValue& colour)
{
    RGBA rgba;
    Root::getSingleton().convertColourValue(colour, &rgba);

    StringUtil::StrStreamType str;
    str << "PureColour" << std::hex << std::setfill('0') << std::setw(8) << rgba;
    String name = str.str();

    ResourcePtr res = MaterialManager::getSingleton().getByName(name);
    if (!res.isNull())
        return res;

    MaterialPtr material = MaterialManager::getSingleton().create(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->setAmbient(0, 0, 0);
    material->setDiffuse(0, 0, 0, colour.a);
    material->setSelfIllumination(colour);
    material->setFog(true, FOG_NONE);
    if (colour.a != 1)
        material->setSceneBlending(SBT_TRANSPARENT_ALPHA);
    return material;
}

MaterialPtr
createColourMaterial(const ColourValue& colour)
{
    if (colour == ColourValue::White)
        return MaterialManager::getSingleton().getByName("BaseWhite");

    RGBA rgba;
    Root::getSingleton().convertColourValue(colour, &rgba);

    StringUtil::StrStreamType str;
    str << "Colour_" << std::hex << std::setfill('0') << std::setw(8) << rgba;
    String name = str.str();

    ResourcePtr res = MaterialManager::getSingleton().getByName(name);
    if (!res.isNull())
        return res;

    MaterialPtr material = MaterialManager::getSingleton().create(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->setAmbient(colour);
    material->setDiffuse(colour);
    if (colour.a != 1)
        material->setSceneBlending(SBT_TRANSPARENT_ALPHA);
    return material;
}

MaterialPtr
createColourMaterial(const ColourValue& colour, const ColourValue& specular, Real shininess)
{
    if (shininess == 0 || specular == ColourValue::Black)
        return createColourMaterial(colour);

    RGBA rgba, rgbaSpecular;
    Root::getSingleton().convertColourValue(colour, &rgba);
    Root::getSingleton().convertColourValue(specular, &rgbaSpecular);

    StringUtil::StrStreamType str;
    str << "Colour_" << std::hex << std::setfill('0')
        << std::setw(8) << rgba
        << std::setw(0) << '_'
        << std::setw(8) << rgbaSpecular
        << std::setw(0) << '_' << shininess;
    String name = str.str();

    ResourcePtr res = MaterialManager::getSingleton().getByName(name);
    if (!res.isNull())
        return res;

    MaterialPtr material = MaterialManager::getSingleton().create(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->setAmbient(colour);
    material->setDiffuse(colour);
    material->setSpecular(specular);
    material->setShininess(shininess);
    if (colour.a != 1)
        material->setSceneBlending(SBT_TRANSPARENT_ALPHA);
    return material;
}

//////////////////////////////////////////////////////////////////////////

MaterialPtr
createProjectiveMaterial(const String& materialTemplateName, const Frustum* frustum)
{
    if (materialTemplateName.empty())
    {
        OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
            "You must give a material template name for create projective material.",
            "createProjectiveMaterial");
    }

    // Build the material from template
    MaterialPtr material = MaterialManager::getSingleton().getByName(materialTemplateName);
    if (material.isNull())
    {
        OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
            "Can't found material with name " + materialTemplateName +
            "Have you forgotten to define it in a  .material script?",
            "createProjectiveMaterial");
    }

    material = material->clone(material->getName() + "/Projectived/" + frustum->getName());
    fixupProjectiveMaterial(material, frustum);

    return material;
}

void
fixupProjectiveMaterial(const MaterialPtr& material, const Frustum* frustum)
{
    // apply texture projective to each texture unit state
    Material::TechniqueIterator ti = material->getTechniqueIterator();
    while (ti.hasMoreElements())
    {
        Technique* technique = ti.getNext();
        Technique::PassIterator pi = technique->getPassIterator();
        while (pi.hasMoreElements())
        {
            Pass* pass = pi.getNext();
            Pass::TextureUnitStateIterator tusi = pass->getTextureUnitStateIterator();
            while (tusi.hasMoreElements())
            {
                TextureUnitState* tus = tusi.getNext();
                // Set projector only if texture addressing mode is clamp and doesn't
                // exists any texture effect that conflict with projective texturing
#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
                const TextureUnitState::UVWAddressingMode& am = tus->getTextureAddressingMode();
                if (am.u == tus->TAM_CLAMP || am.u == tus->TAM_BORDER &&
                    am.v == tus->TAM_CLAMP || am.v == tus->TAM_BORDER &&
                    am.w == tus->TAM_CLAMP || am.w == tus->TAM_BORDER)
#else
                if (tus->getTextureAddressingMode() == tus->TAM_CLAMP)
#endif
                {
                    typedef std::multimap<TextureUnitState::TextureEffectType, TextureUnitState::TextureEffect> EffectMap;
                    const EffectMap& effects = tus->getEffects();
                    if (effects.find(tus->ET_ENVIRONMENT_MAP) == effects.end())
                    {
                        tus->setProjectiveTexturing(true, frustum);
                    }
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

typedef std::list<MovableObject*> ObjectList;

template <class Iterator>
static inline void
addAttachedObjects(Iterator it, ObjectList& objects)
{
    while (it.hasMoreElements())
    {
        MovableObject* object = it.getNext();
        addAttachedObject(object, objects);
    }
}

static void
addAttachedObject(MovableObject* object, ObjectList& objects)
{
    objects.push_back(object);

    if (object->getMovableType() == "Entity")
    {
        Entity* entity = static_cast<Entity*>(object);
        addAttachedObjects(entity->getAttachedObjectIterator(), objects);
    }
}

void
findAllAttachedObjects(SceneNode* sn, ObjectList& objects)
{
    addAttachedObjects(sn->getAttachedObjectIterator(), objects);

    SceneNode::ChildNodeIterator itNodes = sn->getChildIterator();
    while (itNodes.hasMoreElements())
    {
        Node* node = itNodes.getNext();
        SceneNode* child = static_cast<SceneNode*>(node);
        findAllAttachedObjects(child, objects);
    }
}

void
destroyObject(SceneManager* creator, MovableObject* object)
{
    const String& type = object->getMovableType();
    const String& name = object->getName();

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
    if (type == "Camera")
    {
        creator->destroyCamera(name);
    }
    else if (type == "StaticGeometry")
    {
        creator->destroyStaticGeometry(name);
    }
    else
    {
        creator->destroyMovableObject(object);
    }
#else
    if (type == "Entity")
    {
        creator->destroyEntity(name);
    }
    else if (type == "Camera")
    {
        creator->destroyCamera(name);
    }
    else if (type == "Light")
    {
        creator->destroyLight(name);
    }
    else if (type == "BillboardSet")
    {
        creator->removeBillboardSet(name);
    }
    else if (type == "StaticGeometry")
    {
        creator->destroyStaticGeometry(name);
    }
    else if (type == "ParticleSystem")
    {
        ParticleSystemManager::getSingleton().destroySystem(name);
    }
    else
    {
        OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
            "Unknown object type " + type,
            "destroyObject");
    }
#endif
}

void
destroyObjects(SceneManager* creator, const ObjectList& objects)
{
    for (ObjectList::const_iterator it = objects.begin(); it != objects.end(); ++it)
    {
        destroyObject(creator, *it);
    }
}

void
cleanupSceneNode(SceneNode* sn)
{
    ObjectList objects;
    findAllAttachedObjects(sn, objects);
    sn->removeAndDestroyAllChildren();

    destroyObjects(sn->getCreator(), objects);
}

void
cleanupAndDestroySceneNode(SceneNode* sn)
{
    cleanupSceneNode(sn);
    sn->destroy();
}

//////////////////////////////////////////////////////////////////////////

AnimationState*
getAnimationStateSafely(AnimationStateSet* animationStateSet, const String& name)
{
    if (animationStateSet)
    {
#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
        if (animationStateSet->hasAnimationState(name))
            return animationStateSet->getAnimationState(name);
#else
        AnimationStateSet::iterator it = animationStateSet->find(name);
        if (it != animationStateSet->end())
            return &it->second;
#endif
    }

    return NULL;
}

AnimationState*
getAnimationStateSafely(Entity* entity, const String& name)
{
    assert(entity);
    return getAnimationStateSafely(entity->getAllAnimationStates(), name);
}

Animation*
getAnimationSafely(const Skeleton* skeleton, const String& name)
{
    return skeleton ? skeleton->_getAnimationImpl(name) : 0;
}

Animation*
getAnimationSafely(const SkeletonPtr& skeleton, const String& name)
{
    return getAnimationSafely(skeleton.get(), name);
}

Animation*
getAnimationSafely(const Mesh* mesh, const String& name)
{
    return mesh ? getAnimationSafely(mesh->getSkeleton(), name) : 0;
}

Animation*
getAnimationSafely(const MeshPtr& mesh, const String& name)
{
    return getAnimationSafely(mesh.get(), name);
}

//////////////////////////////////////////////////////////////////////////

static AxisAlignedBox
_getSkeletonBoundingBox(const Skeleton* _skeleton, bool deltaOnly)
{
    Skeleton* skeleton = const_cast<Skeleton*>(_skeleton);
    assert(skeleton);

    AxisAlignedBox boundingBox;

    // calculate bone initial positions
    std::map<Bone*, Vector3> boneInitialPositions;

    skeleton->reset();

    Skeleton::BoneIterator itBone = skeleton->getBoneIterator();
    while (itBone.hasMoreElements())
    {
        Bone* bone = itBone.getNext();
        const Vector3& pos = bone->_getDerivedPosition();
        if (deltaOnly)
        {
            boneInitialPositions[bone] = pos;
        }
        else
        {
            boundingBox.merge(pos);
        }
    }

    // create animation state set
    AnimationStateSet animationStateSet;
    skeleton->_initAnimationState(&animationStateSet);

    // for each animation
    AnimationStateIterator asi = animationStateSet.getAnimationStateIterator();
    while (asi.hasMoreElements())
    {
        AnimationState* as = asi.getNext();

        typedef std::set<Real> RealSet;
        typedef ConstVectorIterator<RealSet> ConstRealSetIterator;

        // collect key frame times
        RealSet keyFrameTimes;
        Animation* animation = skeleton->getAnimation(as->getAnimationName());
        Animation::NodeTrackIterator nti = animation->getNodeTrackIterator();
        while (nti.hasMoreElements())
        {
            AnimationTrack* nt = nti.getNext();
            for (ushort i = 0; i < nt->getNumKeyFrames(); ++i)
            {
                KeyFrame* tkf = nt->getKeyFrame(i);
                keyFrameTimes.insert(tkf->getTime());
            }
        }

        // collect bounds of the animation
        as->setEnabled(true);
        ConstRealSetIterator kfti(keyFrameTimes.begin(), keyFrameTimes.end());
        while (kfti.hasMoreElements())
        {
            // Applies the time position
            as->setTimePosition(kfti.getNext());
            skeleton->setAnimationState(animationStateSet);

            Skeleton::BoneIterator itBone = skeleton->getBoneIterator();
            while (itBone.hasMoreElements())
            {
                Bone* bone = itBone.getNext();
                const Vector3& pos = bone->_getDerivedPosition();
                if (deltaOnly)
                {
                    boundingBox.merge(pos - boneInitialPositions[bone]);
                }
                else
                {
                    boundingBox.merge(pos);
                }
            }
        }
        as->setEnabled(false);
    }

    // reset skeleton
    skeleton->reset();

    return boundingBox;
}

AxisAlignedBox
skinningTransformBoundingBox(const AxisAlignedBox& boundingBox, const Skeleton* skeleton)
{
    if (boundingBox.isNull())
        return boundingBox;

    AxisAlignedBox deltaBoundingBox = _getSkeletonBoundingBox(skeleton, true);
    if (deltaBoundingBox.isNull())
        return boundingBox;

    AxisAlignedBox newBoundingBox(boundingBox);
    newBoundingBox.merge(boundingBox.getMinimum() + deltaBoundingBox.getMinimum());
    newBoundingBox.merge(boundingBox.getMaximum() + deltaBoundingBox.getMaximum());
    return newBoundingBox;
}

AxisAlignedBox
skinningTransformBoundingBox(const AxisAlignedBox& boundingBox, const SkeletonPtr& skeleton)
{
    return skinningTransformBoundingBox(boundingBox, skeleton.get());
}

AxisAlignedBox
getSkeletonBoundingBox(const Skeleton* skeleton)
{
    return _getSkeletonBoundingBox(skeleton, false);
}

AxisAlignedBox
getSkeletonBoundingBox(const SkeletonPtr& skeleton)
{
    return getSkeletonBoundingBox(skeleton.get());
}

//////////////////////////////////////////////////////////////////////////

static class NullSkeletonMeshLoader : public ManualResourceLoader
{
public:
    void loadResource(Resource* resource)
    {
        Mesh* mesh = static_cast<Mesh*>(resource);
        mesh->setAutoBuildEdgeLists(false);
        mesh->setSkeletonName(mesh->getName());
        mesh->_setBounds(getSkeletonBoundingBox(mesh->getSkeleton()));
    }
} gsNullSkeletonMeshLoader;

MeshPtr
createNullMeshForSkeleton(const String& meshName, const SkeletonPtr& skeleton)
{
    MeshManager& mm = MeshManager::getSingleton();

    MeshPtr mesh = mm.getByName(meshName);
    if (mesh.isNull())
    {
        if (meshName == skeleton->getName())
        {
            mesh = mm.createManual(meshName, skeleton->getGroup(), &gsNullSkeletonMeshLoader);
        }
        else
        {
            mesh = mm.createManual(meshName, skeleton->getGroup());
            mesh->setAutoBuildEdgeLists(false);
            mesh->_notifySkeleton(const_cast<SkeletonPtr&>(skeleton));
            mesh->_setBounds(getSkeletonBoundingBox(skeleton));
        }

        mesh->load();
    }

    return mesh;
}

MeshPtr
createNullMeshForSkeleton(const SkeletonPtr& skeleton)
{
    return createNullMeshForSkeleton(skeleton->getName(), skeleton);
}

//////////////////////////////////////////////////////////////////////////

typedef bool ConvertEntityToHardwareSkinningFunc(Ogre::Entity* entity, size_t maxSimultaneousLights);
static bool convertEntityToHardwareSkinning_Init(Ogre::Entity* entity, size_t maxSimultaneousLights);
static bool convertEntityToHardwareSkinning_Noop(Ogre::Entity* entity, size_t maxSimultaneousLights);
static ConvertEntityToHardwareSkinningFunc* convertEntityToHardwareSkinningPtr = &convertEntityToHardwareSkinning_Init;

static bool convertEntityToHardwareSkinning_Noop(Ogre::Entity* entity, size_t maxSimultaneousLights)
{
    return false;
}

static bool convertEntityToHardwareSkinning_Init(Ogre::Entity* entity, size_t maxSimultaneousLights)
{
    //convertEntityToHardwareSkinningPtr = &convertEntityToHardwareSkinning_Noop;

    //Ogre::DynLib* dynLib =  Ogre::DynLibManager::getSingleton().load("Plugin_Scorpio");
    //if (dynLib)
    //{
    //    ConvertEntityToHardwareSkinningFunc* func =
    //        static_cast<ConvertEntityToHardwareSkinningFunc*>(
    //            dynLib->getSymbol("convertEntityToHardwareSkinning"));
    //    if (func)
    //    {
    //        convertEntityToHardwareSkinningPtr = func;
    //    }
    //}

    //return (*convertEntityToHardwareSkinningPtr)(entity, maxSimultaneousLights);

	return Ogre::convertEntityToHardwareSkinning(entity,maxSimultaneousLights);
}

extern bool
convertEntityToHardwareSkinning(Ogre::Entity* entity)
{
    return (*convertEntityToHardwareSkinningPtr)(entity, 3);
}

}

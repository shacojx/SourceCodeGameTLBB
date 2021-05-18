#include "WXModel.h"

#include "WXResources.h"
#include "WXUtils.h"
#include "WXFlags.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSkeletonManager.h>
#include <OgreSkeleton.h>
#include <OgreSkeletonInstance.h>
#include <OgreTagPoint.h>
#include <OgreAnimation.h>
#include <OgreAnimationState.h>
#include <OgreMeshManager.h>
#include <OgreMesh.h>
#include <OgreEntity.h>
#include <OgreControllerManager.h>
#include <OgreController.h>

#include "WXXMLHandler.h"
#include "WXXMLAttributes.h"

// HACK: Use expat parser now
#include "WXExpatParser.h"

namespace WX {

    //////////////////////////////////////////////////////////////////////////
    namespace
    {
        class ModelAnimationControllerValue : public Ogre::ControllerValue<Real>
        {
        protected:
            Model* mModel;

        public:
            ModelAnimationControllerValue(Model* model)
                : mModel(model)
            {
            }

            Real getValue(void) const
            {
                return 0;
            }

            void setValue(Real value)
            {
                mModel->addTime(value);
            }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    
    class ModelSerializer : public XMLHandler
    {
    protected:
        Model* mModel;
        String mModelResourceName;
        String mModelResourceGroup;
        Ogre::Entity* mMasterEntity;
        Ogre::MovableObject* mLastMovable;
        Ogre::Node* mLastAttachNode;

    public:
        ModelSerializer(Model* model, const String& resourceName, const String& resourceGroup);
        ~ModelSerializer();

        void startElement(const String& element, const XMLAttributes& attributes);
        void endElement(const String& element);
    };

    //////////////////////////////////////////////////////////////////////////
    
    Model::Model(Ogre::SceneNode* parent, const String& resourceName, const String& resourceGroup)
        : mCreator()
        , mParent(parent)
        , mUserObject()
        , mSceneNodes()
        , mMasterObjects()
        , mMovables()
        , mAnimationStates()
        , mBoundingBox()
        , mAnimationType(AT_FULL)
        , mCastShadows(false)
        , mController()
    {
        assert(parent);
        mCreator = parent->getCreator();
        create(resourceName, resourceGroup);
    }
    //-----------------------------------------------------------------------
    Model::~Model()
    {
        cleanup();
    }
    //-----------------------------------------------------------------------
    void Model::setUserObject(Ogre::UserDefinedObject* userObject)
    {
        mUserObject = userObject;
        for (ObjectList::const_iterator it = mMovables.begin(); it != mMovables.end(); ++it)
        {
            Ogre::MovableObject* movable = *it;
            movable->setUserObject(userObject);
        }
    }
    //-----------------------------------------------------------------------
    Ogre::UserDefinedObject* Model::getUserObject(void) const
    {
        return mUserObject;
    }
    //-----------------------------------------------------------------------
    const Ogre::AxisAlignedBox& Model::getBoundingBox(void) const
    {
        return mBoundingBox;
    }
    //-----------------------------------------------------------------------
    void Model::setCastShadows(bool castShadows)
    {
        if (mCastShadows != castShadows)
        {
            mCastShadows = castShadows;
            updateCastShadows();
        }
    }
    //-----------------------------------------------------------------------
    void Model::setAnimationType(AnimationType animationType)
    {
        if (mAnimationType != animationType)
        {
            mAnimationType = animationType;
            updateAnimationType();
        }
    }
    //-----------------------------------------------------------------------
    void Model::updateCastShadows(void)
    {
        std::for_each(mMovables.begin(), mMovables.end(),
            std::bind2nd(std::mem_fun(&Ogre::MovableObject::setCastShadows), mCastShadows));
    }
    //-----------------------------------------------------------------------
    void Model::updateAnimationType(void)
    {
        if (!mAnimationStates.empty())
        {
            switch (mAnimationType)
            {
            case AT_FULL:
                createController();
                enableAnimationStates();
                break;

            case AT_FROZE:
                destroyController();
                enableAnimationStates();
                break;

            case AT_DISABLE:
                destroyController();
                disableAnimationStates();
                break;
            }
        }
    }
    //-----------------------------------------------------------------------
    void Model::enableAnimationStates(void)
    {
        Real timePosition = Ogre::Math::UnitRandom();
        for (AnimationStateList::const_iterator it = mAnimationStates.begin();
            it != mAnimationStates.end(); ++it)
        {
            Ogre::AnimationState* as = *it;
            if (!as->getEnabled())
            {
                as->setTimePosition(timePosition * as->getLength());
                as->setEnabled(true);
            }
        }
    }
    //-----------------------------------------------------------------------
    void Model::disableAnimationStates(void)
    {
        for (AnimationStateList::const_iterator it = mAnimationStates.begin();
            it != mAnimationStates.end(); ++it)
        {
            Ogre::AnimationState* as = *it;
            if (as->getEnabled())
            {
                as->setEnabled(false);
            }
        }
    }
    //-----------------------------------------------------------------------
    void Model::createController(void)
    {
        if (!mController)
        {
            Ogre::ControllerManager& controllerManager = Ogre::ControllerManager::getSingleton();
            mController = controllerManager.createFrameTimePassthroughController(
                Ogre::ControllerValueRealPtr(new ModelAnimationControllerValue(this)));
        }
    }
    //-----------------------------------------------------------------------
    void Model::destroyController(void)
    {
        if (mController)
        {
            Ogre::ControllerManager& controllerManager = Ogre::ControllerManager::getSingleton();
            controllerManager.destroyController(mController);
            mController = NULL;
        }
    }
    //-----------------------------------------------------------------------
    void Model::addTime(AnimationStateList& animationStates, Real offset)
    {
        for (AnimationStateList::const_iterator it = animationStates.begin(); it != animationStates.end(); ++it)
        {
            Ogre::AnimationState* animationState = *it;
            animationState->addTime(offset);
        }
    }
    void Model::addTime(Real offset)
    {
        addTime(mAnimationStates, offset);
    }
    //-----------------------------------------------------------------------
    void Model::create(const String& resourceName, const String& resourceGroup)
    {
        // HACK: Use expat parser now
        ExpatParser expatParser(NULL);
        XMLParser* xmlParser = &expatParser;

        ModelSerializer serializer(this, resourceName, resourceGroup);
        xmlParser->parseXMLFile(serializer, resourceName, resourceGroup, Ogre::StringUtil::BLANK, Ogre::StringUtil::BLANK);

        updateCastShadows();
        updateAnimationType();
    }
    //-----------------------------------------------------------------------
    void Model::cleanup()
    {
        destroyController();

        destroyObjects(mCreator, mMovables);
        mMovables.clear();

        destroyObjects(mCreator, mMasterObjects);
        mMasterObjects.clear();

        std::for_each(mSceneNodes.begin(), mSceneNodes.end(), std::mem_fun(&Ogre::SceneNode::destroy));
        mSceneNodes.clear();

        mAnimationStates.clear();

        mBoundingBox.setNull();
    }

    //////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------
    ModelSerializer::ModelSerializer(Model* model, const String& resourceName, const String& resourceGroup)
        : mModel(model)
        , mModelResourceName(resourceName)
        , mModelResourceGroup(resourceGroup)
        , mMasterEntity()
        , mLastMovable()
        , mLastAttachNode()
    {
    }
    //-----------------------------------------------------------------------
    ModelSerializer::~ModelSerializer()
    {
    }
    //-----------------------------------------------------------------------
    void ModelSerializer::startElement(const String& element, const XMLAttributes& attributes)
    {
        if (element == "entity")
        {
            if (mLastMovable || mLastAttachNode)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Invalidate model resource " + mModelResourceName,
                    "Model::create");
            }

            // Determinate which node to attach to
            const String* boneName = 0;
            if (attributes.exists("node"))
            {
                if (!mMasterEntity || !mMasterEntity->hasSkeleton())
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                        "Invalidate model resource " + mModelResourceName,
                        "Model::create");
                }

                boneName = &attributes.getValue("node");
                mMasterEntity->getSkeleton()->getBone(*boneName);
            }

            // Determinate should be normalise normal
            bool normalise = false;
            if (attributes.exists("normalise"))
            {
                normalise = attributes.getValueAs<bool>("normalise");
            }

            // Determinate it's decal target
            bool receive_decals = false;
            if (attributes.exists("receive_decals"))
            {
                receive_decals = attributes.getValueAs<bool>("receive_decals");
            }

            // Get the mesh name
            const String& meshName = attributes.getValue("mesh");

            // Load the mesh
            Ogre::MeshPtr mesh = loadMesh(
                meshName, Ogre::StringUtil::BLANK,
                mModelResourceName, mModelResourceGroup);

            // Create the entity
            Ogre::Entity* entity = mModel->mCreator->createEntity(
                mModel->mParent->getName() + "/" + mesh->getName(),
                mesh->getName());
            mModel->mMovables.push_back(entity);

            entity->setUserObject(mModel->mUserObject);
            if (normalise)
            {
                entity->setNormaliseNormals(normalise);
            }
            if (receive_decals)
            {
                entity->addQueryFlags(OQF_DECAL_TARGET);
                // If attaching to master entity, should enable master entity query flags too,
                // otherwise will failed to query.
                if (mMasterEntity)
                {
                    mMasterEntity->addQueryFlags(OQF_DECAL_TARGET);
                }
            }

            if (boneName)
            {
                mLastAttachNode = mMasterEntity->attachObjectToBone(*boneName, entity);
            }
            else
            {
                mModel->mParent->attachObject(entity);
            }

            mLastMovable = entity;

            // Compute bounding box
            if (mesh->hasSkeleton())
            {
                // Merge skinned bounding box
                mModel->mBoundingBox.merge(skinningTransformBoundingBox(mesh->getBounds(), mesh->getSkeleton()));
            }
            else
            {
                // Merge normal bounding box
                mModel->mBoundingBox.merge(mesh->getBounds());
            }

            // Add predefined animations
            Ogre::AnimationState* animationState = getAnimationStateSafely(entity, AUTO_ANIMATION_NAME);
            if (animationState)
            {
                convertEntityToHardwareSkinning(entity);
                mModel->mAnimationStates.push_back(animationState);
            }
        }
        else if (element == "offset")
        {
            if (!mLastMovable)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Invalidate model resource " + mModelResourceName,
                    "Model::create");
            }

            if (!mLastAttachNode)
            {
                if (mLastMovable->getParentSceneNode() != mModel->mParent)
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                        "Invalidate model resource " + mModelResourceName,
                        "Model::create");
                }

                // Detach form parent and attach to new offset scene node
                Ogre::SceneNode* attachNode = mLastMovable->getParentSceneNode()->createChildSceneNode();
                mModel->mSceneNodes.push_back(attachNode);
                mLastMovable->getParentSceneNode()->detachObject(mLastMovable->getName());
                attachNode->attachObject(mLastMovable);
                mLastAttachNode = attachNode;
            }
        }
        else if (element == "translate")
        {
            if (!mLastAttachNode)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Invalidate model resource " + mModelResourceName,
                    "Model::create");
            }

            mLastAttachNode->setPosition(
                attributes.getValueAs<Ogre::Real>("x"),
                attributes.getValueAs<Ogre::Real>("y"),
                attributes.getValueAs<Ogre::Real>("z"));
            mLastAttachNode->setInitialState();
        }
        else if (element == "rotate")
        {
            if (!mLastAttachNode)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Invalidate model resource " + mModelResourceName,
                    "Model::create");
            }

            mLastAttachNode->setOrientation(
                attributes.getValueAs<Ogre::Real>("w"),
                attributes.getValueAs<Ogre::Real>("x"),
                attributes.getValueAs<Ogre::Real>("y"),
                attributes.getValueAs<Ogre::Real>("z"));
            mLastAttachNode->setInitialState();
        }
        else if (element == "scale")
        {
            if (!mLastAttachNode)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Invalidate model resource " + mModelResourceName,
                    "Model::create");
            }

            mLastAttachNode->setScale(
                attributes.getValueAs<Ogre::Real>("x"),
                attributes.getValueAs<Ogre::Real>("y"),
                attributes.getValueAs<Ogre::Real>("z"));
            mLastAttachNode->setInitialState();
        }
        else if (element == "frame")
        {
            if (mMasterEntity)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Invalidate model resource " + mModelResourceName,
                    "Model::create");
            }

            const String& frameName = attributes.getValue("name");

            // Load the skeleton
            Ogre::SkeletonPtr skeleton = loadCorrelativeResource(
                frameName, Ogre::StringUtil::BLANK,
                mModelResourceName, mModelResourceGroup,
                Ogre::SkeletonManager::getSingleton());

            if (skeleton.isNull())
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                    "Unable to load frame " + frameName +
                    " for Model " + mModelResourceName +
                    ". This Model will not be animated. " +
                    "You can ignore this message if you are using an offline tool.",
                    "Model::create");
            }
            else if (!skeleton->getNumBones())
            {
                // Null skeleton, don't know why this happen
                static int i;
                ++i;
            }
            else
            {
                // Construct fake mesh and entity
                Ogre::MeshPtr mesh = createNullMeshForSkeleton(skeleton);
                mMasterEntity = mModel->mCreator->createEntity(
                    mModel->mParent->getName() + "/" + mesh->getName(),
                    mesh->getName());
                mModel->mMasterObjects.push_back(mMasterEntity);

                mModel->mParent->attachObject(mMasterEntity);

                // Merge frame structure bounding box
                mModel->mBoundingBox.merge(mMasterEntity->getBoundingBox());

                // Add predefined animations
                Ogre::AnimationState* animationState = getAnimationStateSafely(mMasterEntity, AUTO_ANIMATION_NAME);
                if (animationState)
                {
                    mModel->mAnimationStates.push_back(animationState);
                }
            }
        }
        else if (element == "model")
        {
            // TODO:
        }
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Unexpected data was found while parsing the model file: '" + element + "' is unknown.",
                "Model::create");
        }
    }
    //-----------------------------------------------------------------------
    void ModelSerializer::endElement(const String& element)
    {
        if (element == "entity")
        {
            mLastMovable = NULL;
            mLastAttachNode = NULL;
        }
        else if (element == "offset")
        {
            assert(mLastMovable);
            assert(mLastAttachNode);
            mLastAttachNode = NULL;
        }
        else if (element == "translate")
        {
            assert(mLastAttachNode);
        }
        else if (element == "rotate")
        {
            assert(mLastAttachNode);
        }
        else if (element == "scale")
        {
            assert(mLastAttachNode);
        }
        else if (element == "frame")
        {
        }
        else if (element == "model")
        {
        }
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Unexpected data was found while parsing the model file: '" + element + "' is unknown.",
                "Model::create");
        }
    }
    //-----------------------------------------------------------------------
    bool Model::existsObject(Ogre::MovableObject* object) const
	{			
        return std::find(mMovables.begin(), mMovables.end(), object) != mMovables.end();
	}

}
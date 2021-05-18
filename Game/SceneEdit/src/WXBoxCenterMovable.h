#ifndef __BoxCenterMovable_H__
#define __BoxCenterMovable_H__

#include "WXSceneListener.h"

#include "Core/WXObject.h"

#include <OgreMovableObject.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>

namespace Ogre
{
	class RaySceneQuery;
	class SceneManager;
}

namespace WX {

	class BoxCenterMovable;
	class SceneManipulator;
	class BoxCenterManager : public SceneListener
	{
	private:
		typedef std::map<ObjectWeakPtr,BoxCenterMovable*> ObjBoxCenterMap;
		typedef std::map<Ogre::MovableObject*,ObjectWeakPtr> BoxCenterObjMap;
		ObjBoxCenterMap mObjBoxCenterMap;
		BoxCenterObjMap mBoxCenterObjMap;

		SceneManipulator* mSceneManipulator;
		Ogre::RaySceneQuery* mRaySceneQuery;
		Ogre::SceneManager* mSceneMgr;
	protected:
		void onSceneReset(void);
		void onRemoveObject(const ObjectPtr& object);
	public:
		BoxCenterManager(SceneManipulator* manip,Ogre::SceneManager* mSceneManager);
		~BoxCenterManager();

		void init();
		void addObjBox(const ObjectPtr& object,BoxCenterMovable* mv);
		void removeBox(BoxCenterMovable* mv);
		ObjectWeakPtr findHitObject(const Point& pt);
	};

    class BoxCenterMovable : public Ogre::MovableObject
    {
    protected:
        Ogre::AxisAlignedBox mBox;

        /// The scene manager for the current frame.
        Ogre::SceneManager *m_pParentSceneManager;

        /// The camera for the current frame.
        Ogre::Camera *m_pCamera;
        Ogre::Viewport* mViewport;

        /// Static member used to automatically generate names for SimpleRendaerable objects.
        static unsigned int ms_uGenNameCount;

        Ogre::Overlay* mObjectCenterPointOverlay;        
        Ogre::OverlayContainer* mObjectCenterPointRect;
        Ogre::Vector3 mCenterPosWC;
        Ogre::Vector2 mCenterPosSC;
        ObjectWeakPtr object;

    public:
        BoxCenterMovable();


        virtual void _notifyCurrentCamera(Ogre::Camera* cam);

        void setBoundingBox( const Ogre::AxisAlignedBox& box );
        virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;
        virtual Real getBoundingRadius(void) const;


        virtual void _updateRenderQueue(Ogre::RenderQueue* queue);

        virtual ~BoxCenterMovable();

        /** Overridden from MovableObject */
        virtual const Ogre::String& getMovableType(void) const;

        void init(const ObjectPtr& obj,Ogre::SceneManager* mgr,Ogre::Viewport* viewport,Ogre::Camera* camera);
        ObjectWeakPtr& getObject()
        {
            return object;
        }
    
    protected:
        void createGuiElements(void);
        void getBoundingBoxCenter();
    };
}

#endif


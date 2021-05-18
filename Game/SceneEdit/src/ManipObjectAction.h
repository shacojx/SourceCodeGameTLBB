#ifndef __MANIPOBJECTACTION_H__
#define __MANIPOBJECTACTION_H__

#include "Action.h"
#include "Core/WXTransform.h"
#include "Core/WXObject.h"
#include "HitInfo.h"

#include "WXBoxCenterMovable.h"

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

namespace Ogre
{
    class PlaneBoundedVolumeListSceneQuery;
}

namespace WX
{
    class AxisRenderable;
	class BoxCenterManager;
}

namespace WX
{
    class ManipObjectAction : public Action, protected SceneListener
    {    
    protected:
        Real mScaleFactor;
        Real mMinScale;
        Real mMaxScale;
        Real mMouseScaleScale;
        Real mMouseRotateScale;
        Ogre::String mCurrentMode;

        Ogre::RaySceneQuery* mRaySceneQuery;
        Ogre::PlaneBoundedVolumeListSceneQuery* mPlaneBoundedVolumeListSceneQuery;

        /// The overlay which contains our selecting rectangle display
        Ogre::Overlay* mSelectingOverlay;
        /// The window that displays the selecting rectangle
        Ogre::OverlayContainer* mSelectingRect;

        Ogre::SceneNode* mBaseSceneNode;

        typedef Ogre::WireBoundingBox Indicator;
        typedef std::list<Indicator*> IndicatorList;
        IndicatorList mIndicators;
        IndicatorList mFreeIndicators;

        HitInfo mHitInfo;
        bool mSelecting;

        struct ObjectInfo : Transform {
            Indicator* indicator;

            ObjectInfo()
                : indicator(NULL)
            {
            }
        };

        typedef std::map<ObjectWeakPtr, ObjectInfo> SelectedObjectMap;
        SelectedObjectMap mSelectedObjects;
        ObjectWeakPtr mHitObject;
        Indicator* mHitIndicator;
        Ogre::Vector3 mCenterPos;
        typedef std::set<Ogre::MovableObject*> MovableSet;
        MovableSet mvSet;
        AxisRenderable* lr;
        Ogre::SceneNode* mAxisNode;
		BoxCenterManager mBoxCenterMgr;
    public:
        typedef std::set<ObjectWeakPtr> ObjectPtrSet;
        ObjectPtrSet objSet;
    public:
        ManipObjectAction(SceneManipulator* sceneManipulator);
        ~ManipObjectAction();

        const String& getName(void) const;

        void setParameter(const String& name, const String& value);
        String getParameter(const String& name) const;
        void destroyAxis();
		void hideAxis();

        std::set<ObjectWeakPtr>& getObjSet();
        void findHitObjectByBoxCenter(const Point& pt);
        BoxCenterMovable* createBoxCenterMovable(const ObjectPtr& object);
        AxisRenderable* createAxis(Ogre::SceneManager* sceneMgr,Ogre::Camera* camera,Ogre::Viewport* Viewport);       
		void createBoxCenterManager();
		void clearBoxCenterManager();
		void addBoxCenterMovable(const Point& pt);


    protected:
        void _onActive(bool active);

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

        void onSceneReset(void);

        void onRemoveObject(const ObjectPtr& object);

        void onSelectObject(const ObjectPtr& object);
        void onDeselectObject(const ObjectPtr& object);
        void onDeselectAllObjects(void);

        void onObjectPropertyChanged(const ObjectPtr& object, const String& name);

		void dragObject(const Point& pt);
		void pickObject(const Point& pt);
		void dragObjectByMax(const Point& pt);
		void dragObjectByKey(const Point& pt);

    protected:
        void hide(void);

        ObjectPtr findHitObject(const Point& pt) const;
        ObjectWeakPtr findFirstHitObjectByBoxCenter(const Point& pt);
        Ogre::PlaneBoundedVolume buildSelectingRectVolume(const Ogre::Camera* camera,
            Real left, Real top, Real right, Real bottom) const;

        void createGuiElements(void);

        Indicator* allocateIndicator(void);
        void freeIndicator(Indicator* indicator);
        void updateIndicator(const ObjectPtr& object);
        static void setupIndicator(Indicator* indicator, const ObjectPtr& object);

        static void getObjectTransform(const ObjectPtr& object, Transform& transform);

        Ogre::Vector3 getCenterPos(bool old = false);
        void updateAxis();

    };

}

#endif // __ManipObjectAction_H__

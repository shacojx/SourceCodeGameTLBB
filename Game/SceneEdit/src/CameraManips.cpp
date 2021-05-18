#include "CameraManips.h"
#include "SceneManipulator.h"

#include "Core/TerrainData.h"

#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>

namespace WX {

//////////////////////////////////////////////////////////////////////////
    
CameraPan::CameraPan(SceneManipulator* sceneManipulator)
    : Manip(sceneManipulator)
{
}

const String& CameraPan::getName(void) const
{
    static const String name = "CameraPan";
    return name;
}

void CameraPan::_onDrag(const Point& pt)
{
    getCamera()->moveRelative(
        Ogre::Vector3(+mDragDelta.x * getSceneManipulator()->_getMoveFactor() / getViewport()->getActualWidth(),
                      -mDragDelta.y * getSceneManipulator()->_getMoveFactor() / getViewport()->getActualHeight(),
                       0));
    getSceneManipulator()->_fireCameraPosChanged();
}

//////////////////////////////////////////////////////////////////////////
    
CameraRotate::CameraRotate(SceneManipulator* sceneManipulator)
    : Manip(sceneManipulator)
{
}

const String& CameraRotate::getName(void) const
{
    static const String name = "CameraRotate";
    return name;
}

void CameraRotate::_onDrag(const Point& pt)
{
    getCamera()->yaw(Ogre::Degree(-mDragDelta.x * getSceneManipulator()->getRotateFactor() / getViewport()->getActualWidth()));

    // 只有在不使用真实摄像机的情况下才允许上下旋转摄像机
    if ( !getSceneManipulator()->getUseRealCameraAngle() )
        getCamera()->pitch(Ogre::Degree(-mDragDelta.y * getSceneManipulator()->getRotateFactor() / getViewport()->getActualHeight()));

    getSceneManipulator()->_fireCameraPosChanged();
}

//////////////////////////////////////////////////////////////////////////

void HintBasedCameraManip::_onBegin(const Point& pt)
{
    getSceneManipulator()->makeTerrainHitInfo(mTerrainHitInfo, pt);
}

//////////////////////////////////////////////////////////////////////////

CameraDragMove::CameraDragMove(SceneManipulator* sceneManipulator)
    : HintBasedCameraManip(sceneManipulator)
{
}

const String& CameraDragMove::getName(void) const
{
    static const String name = "CameraDragMove";
    return name;
}

void CameraDragMove::_onDrag(const Point& pt)
{
    if (!mTerrainHitInfo.hitted)
        return;

    Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(pt);
    if (!ray.getDirection().y)
        return;

    Real length = (mTerrainHitInfo.position.y - mTerrainHitInfo.hitPosition.y) / ray.getDirection().y;
    Ogre::Vector3 newPosition = mTerrainHitInfo.hitPosition + ray.getDirection() * length;

    if (getSceneManipulator()->getCameraHeightLocked())
    {
        newPosition.y +=
            getTerrainData()->getHeightAt(newPosition.x, newPosition.z) -
            getTerrainData()->getHeightAt(mTerrainHitInfo.position.x, mTerrainHitInfo.position.z);
    }

    getSceneManipulator()->setCameraPosition(newPosition);
}

//////////////////////////////////////////////////////////////////////////

CameraDragRotate::CameraDragRotate(SceneManipulator* sceneManipulator)
    : HintBasedCameraManip(sceneManipulator)
{
}

const String& CameraDragRotate::getName(void) const
{
    static const String name = "CameraDragRotate";
    return name;
}

void CameraDragRotate::_onDrag(const Point& pt)
{
    if (!mTerrainHitInfo.hitted)
        return;

    Ogre::Vector3 oldPosition = getCamera()->getDerivedPosition();
    Ogre::Quaternion oldOrientation = getCamera()->getDerivedOrientation();
    
    getCamera()->yaw(Ogre::Degree(-mDragDelta.x * getSceneManipulator()->getRotateFactor() / getViewport()->getActualWidth()));
    
    // 只有在不使用真实摄像机的情况下才允许上下旋转摄像机
    if ( !getSceneManipulator()->getUseRealCameraAngle() )
        getCamera()->pitch(Ogre::Degree(-mDragDelta.y * getSceneManipulator()->getRotateFactor() / getViewport()->getActualHeight()));

    Ogre::Quaternion newOrientation = getCamera()->getDerivedOrientation();
    Ogre::Vector3 newPosition = mTerrainHitInfo.hitPosition +
        newOrientation * oldOrientation.Inverse() * (oldPosition - mTerrainHitInfo.hitPosition);

    getSceneManipulator()->setCameraPosition(newPosition);
}

}

#ifndef __CameraManips_H__
#define __CameraManips_H__

#include "Manip.h"
#include "HitInfo.h"

#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreRay.h>

namespace WX {

    class CameraPan : public Manip
    {
    public:
        CameraPan(SceneManipulator* sceneManipulator);

        const String& getName(void) const;

    protected:
        void _onDrag(const Point& pt);
    };

    class CameraRotate : public Manip
    {
    public:
        CameraRotate(SceneManipulator* sceneManipulator);

        const String& getName(void) const;

    protected:
        void _onDrag(const Point& pt);
    };

    class HintBasedCameraManip : public Manip
    {
    protected:
        HitInfo mTerrainHitInfo;

    public:
        HintBasedCameraManip(SceneManipulator* sceneManipulator)
            : Manip(sceneManipulator)
        {
        }

    protected:
        void _onBegin(const Point& pt);
    };

    class CameraDragMove : public HintBasedCameraManip
    {
    public:
        CameraDragMove(SceneManipulator* sceneManipulator);

        const String& getName(void) const;

    protected:
        void _onDrag(const Point& pt);
    };

    class CameraDragRotate : public HintBasedCameraManip
    {
    public:
        CameraDragRotate(SceneManipulator* sceneManipulator);

        const String& getName(void) const;

    protected:
        void _onDrag(const Point& pt);
    };
}

#endif // __CameraManips_H__

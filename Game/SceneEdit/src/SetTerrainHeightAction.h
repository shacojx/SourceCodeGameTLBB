#ifndef __SetTerrainHeightAction_H__
#define __SetTerrainHeightAction_H__

#include "HeightAction.h"
#include "HitInfo.h"

namespace WX {

    /** 根据ui界面中设置的数值来设置地形的高度
    */
    class SetTerrainHeightAction : public HeightAction
    {
    public:
        SetTerrainHeightAction(SceneManipulator* sceneManipulator);
        ~SetTerrainHeightAction();

        const String& getName(void) const;

        void setParameter(const String& name, const String& value);
        String getParameter(const String& name) const;

    protected:

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

        void _doModify(const Point& pt);

    protected:

        int mTerrainHeight;
		int mTerrainHeightIncrement;

        JunctionSelection* mModifiedJunctions;
    };

}

#endif // __SetTerrainHeightAction_H__

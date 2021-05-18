#ifndef __TerrainLayerOneEraserAction_H__
#define __TerrainLayerOneEraserAction_H__

#include "PaintAction.h"

namespace WX {

    class GridSelection;

    /** 擦掉第一层纹理，原来是从action类派生的，后来为了利用PaintAction类的doFinish来进行redo，undo，就
        从PaintAction类派生了，这也是合理的，因为这项操作本质也是修改地形纹理
    */
    class TerrainLayerOneEraserAction : public PaintAction
    {
    public:
        TerrainLayerOneEraserAction(SceneManipulator* sceneManipulator);
        ~TerrainLayerOneEraserAction();

        const String& getName(void) const;

      //  void setParameter(const String& name, const String& value);
     //   String getParameter(const String& name) const;

    protected:
        GridSelection* mCurrentGrids;
        GridSelection* mHintModified;
        GridSelection* mModifiedGrids;

        void _buildHitIndicator(const Point& pt);
        void _doErase(GridSelection* modified);

        void _onActive(bool active);

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);   

    };

}

#endif // __TerrainLayerOneEraserAction_H__

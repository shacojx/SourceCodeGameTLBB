#ifndef __SimplePaintAction_H__
#define __SimplePaintAction_H__

#include "PaintAction.h"
#include "Selection.h"

namespace WX {

    class GridSelection;

    class SimplePaintAction : public PaintAction
    {
    public:
        SimplePaintAction(SceneManipulator* sceneManipulator);
        ~SimplePaintAction();

        const String& getName(void) const;

		void setParameter(const String& name, const String& value);
		String getParameter(const String& name) const;

    protected:
        GridSelection* mCurrentGrids;
        GridSelection* mHintModified;
        GridSelection* mModifiedGrids;

        void _buildHitIndicator(const Point& pt);
        void _doPaint(GridSelection* modified);

        void _onActive(bool active);

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

    protected:

        typedef std::vector< PaintPixmap > SimplePaintPixmapArray;

        SimplePaintPixmapArray mPixmapArray;

        /// 当前要贴的纹理的大小为64（标准大小）
        bool mbTexNormalSize;
        /// 是否使用随机方向
        bool mbUseRandomOrientation;

        /// 画刷的大小（普通画刷的纹理是64*64，如果大小超过64的纹理，那么这里就要通过setParameter把这两个
        /// 画刷的大小设进来）
        int mXBrushSize;
        int mZBrushSize;

        
    };

}

#endif // __SimplePaintAction_H__

#ifndef __BrushShapes_H__
#define __BrushShapes_H__

#include "BrushShape.h"

namespace WX {

    class Selection;
	class SceneManipulator;

    class BoxBrushShape : public BrushShape
    {
    public:
        virtual const String& getType(void) const;
        virtual void build(Selection& selection, Real xcentre, Real zcentre, int xsize, int zsize, SceneManipulator *manipulator);
    };

    class EllipseBrushShape : public BrushShape
    {
    public:
        virtual const String& getType(void) const;
        virtual void build(Selection& selection, Real xcentre, Real zcentre, int xsize, int zsize, SceneManipulator *manipulator);
    };
}

#endif // __BrushShapes_H__

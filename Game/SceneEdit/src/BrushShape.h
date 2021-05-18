#ifndef __BRUSHSHAPE_H__
#define __BRUSHSHAPE_H__

#include "Core/WXPrerequisites.h"

class SceneManipulator;

namespace WX {

    class Selection;

    class BrushShape
    {
    public:
        virtual ~BrushShape() {}

        virtual const String& getType(void) const = 0;

        virtual void build(Selection& selection, Real xcentre, Real zcentre, int xsize, int zsize, SceneManipulator *manipulator) = 0;
    };

}

#endif // __BrushShape_H__

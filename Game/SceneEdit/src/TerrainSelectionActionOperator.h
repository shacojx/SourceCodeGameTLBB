#ifndef __TERRAINSELECTIONACTIONOPERATOR_H__
#define __TERRAINSELECTIONACTIONOPERATOR_H__

#include "WXOperator.h"

namespace WX {

    class FlipTerrainDiagonalOperator;
    class ModifyTerrainHeightOperator;
    class ModifyTerrainPaintOperator;

    class SceneManipulator;

    /** 这个operator用于地形的拷贝，粘贴，集成了三个operator
    */
    class TerrainSelectionActionOperator : public Operator
    {
    public:

        TerrainSelectionActionOperator(SceneManipulator* sceneManipulator);
        ~TerrainSelectionActionOperator();

        bool empty(void) const;

        const String& getGroupName(void) const;

        String getDescription(void) const;

        String getHelp(void) const;

        void undo(void);

        void redo(void);

    public:

        FlipTerrainDiagonalOperator *mFlipDiagonalOperator;
        ModifyTerrainHeightOperator *mModifyHeightOperator;
        ModifyTerrainPaintOperator *mModifyPaintOperator;
    };
}

#endif // __TerrainSelectionActionOperator_H__
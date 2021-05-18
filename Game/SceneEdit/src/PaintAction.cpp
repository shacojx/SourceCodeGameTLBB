#include "PaintAction.h"
#include "WXModifyTerrainPaintOperator.h"
#include "WXOperatorManager.h"
#include "TerrainSelections.h"
#include "SceneManipulator.h"

#include "Core/TerrainData.h"

namespace WX {
    //-----------------------------------------------------------------------
    PaintAction::PaintAction(SceneManipulator* sceneManipulator)
        : Action(sceneManipulator)
		, mPixmapInfoValid(false), mPixmapTransparent(false), mOrientationType(0), mPixmapId(0) 
    {
        mPixmap = PaintPixmap();
    }
    //-----------------------------------------------------------------------
    void
        PaintAction::setPixmap(const String& textureName, Real left, Real top, Real right, Real bottom )
    {
        mPixmap.textureName = textureName;
        mPixmap.left = left;
        mPixmap.top = top;
        mPixmap.right = right;
        mPixmap.bottom = bottom;
        mPixmapInfoValid = false;
    }
    void 
        PaintAction::setPixmap(const PaintPixmap &pixmap)
    {
        mPixmap.textureName = pixmap.textureName;
        mPixmap.left = pixmap.left;
        mPixmap.top = pixmap.top;
        mPixmap.right = pixmap.right;
        mPixmap.bottom = pixmap.bottom;
        mPixmapInfoValid = false;
    }
    //-----------------------------------------------------------------------
    int
        PaintAction::getPixmapId(void) const
    {
        if (!mPixmapInfoValid)
            updatePixmapInfo();
        return mPixmapId;
    }
    //-----------------------------------------------------------------------
    bool
        PaintAction::getPixmapTransparent(void) const
    {
        if (!mPixmapInfoValid)
            updatePixmapInfo();
        return mPixmapTransparent;
    }
    //-----------------------------------------------------------------------
    void
        PaintAction::updatePixmapInfo(void) const
    {
        mPixmapId = getTerrainData()->_registerPixmap(
            mPixmap.textureName,
            mPixmap.left,
            mPixmap.top,
            mPixmap.right,
            mPixmap.bottom);

        mPixmapTransparent = false;
        if (!mPixmap.textureName.empty())
        {
            mPixmapTransparent = getSceneManipulator()->IsTransparentTexture(mPixmap.textureName);
        }

        mPixmapInfoValid = true;
    }
    //-----------------------------------------------------------------------
    void 
    PaintAction::doFinish(GridSelection* origin, bool canceled)
    {
        assert(origin);

        if (canceled)
        {
            origin->apply();
        }
        else
        {
            if (!origin->empty())
            {
                std::auto_ptr<ModifyTerrainPaintOperator> op(new ModifyTerrainPaintOperator(getSceneManipulator()));

                const GridSelection::GridMap& grids = origin->getGrids();
                GridSelection::GridMap::const_iterator it;
                for (it = grids.begin(); it != grids.end(); ++it)
                {
                    const GridSelection::Grid& grid = it->second;
                    const TerrainData::GridInfo &newGridInfo = getTerrainData()->getGridInfo(grid.x, grid.z);
                    const TerrainData::GridInfo &oldGridInfo = grid.info;
                    if (newGridInfo != oldGridInfo)
                    {
                        op->add(grid.x, grid.z, oldGridInfo, newGridInfo);
                    }
                }

                if (!op->empty())
                    getOperatorManager()->addOperator(op.release());
            }
        }
    }
}
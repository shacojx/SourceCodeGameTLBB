#include "HeightAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"
#include "WXModifyTerrainHeightOperator.h"
#include "WXOperatorManager.h"

namespace WX {

    //////////////////////////////////////////////////////////////////////////

    void HeightAction::_onActive(bool active)
    {
        if (!active)
        {
            getSceneManipulator()->getHitIndicator("JunctionPoints")->hide();
        }
    }
    //-----------------------------------------------------------------------
    void HeightAction::doFinish(JunctionSelection* origin, bool canceled)
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
                std::auto_ptr<ModifyTerrainHeightOperator> op(new ModifyTerrainHeightOperator(getSceneManipulator()));

                const JunctionSelection::JunctionMap& junctions = origin->getJunctions();
                JunctionSelection::JunctionMap::const_iterator it;
                for (it = junctions.begin(); it != junctions.end(); ++it)
                {
                    const JunctionSelection::Junction& junction = it->second;
                    Real newHeight = getTerrainData()->getHeight(junction.x, junction.z);
                    Real oldHeight = junction.height;
                    if (newHeight != oldHeight)
                    {
                        op->add(junction.x, junction.z, oldHeight, newHeight);
                    }
                }

                if (!op->empty())
                    getOperatorManager()->addOperator(op.release());
            }
        }
    }

}

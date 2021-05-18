#ifndef __FAIRYSCENESOLVER_H__
#define __FAIRYSCENESOLVER_H__

#include "WXResourceSolver.h"

namespace WX {
    class SceneInfo;

    ResourceSolverPtr findUnsolvedSceneItems(SceneInfo* sceneInfo);

}

#endif // 

#ifndef __FAIRYTERRAINSOLVER_H__
#define __FAIRYTERRAINSOLVER_H__

#include "WXResourceSolver.h"

namespace WX {
    class Terrain;

    ResourceSolverPtr findUnsolvedTerrainBrushes(Terrain* terrain);

}

#endif // 

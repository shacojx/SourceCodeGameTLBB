#ifndef __FAIRYMATERIALSCRIPTSOLVER_H__
#define __FAIRYMATERIALSCRIPTSOLVER_H__

#include "WXResourceSolver.h"

namespace WX {

    ResourceSolverPtr findUnsolvedMaterialTextures(const String& group = Ogre::StringUtil::BLANK);

}

#endif // 

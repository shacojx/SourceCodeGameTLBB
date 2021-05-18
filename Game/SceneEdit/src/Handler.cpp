#include "Handler.h"
#include "SceneManipulator.h"

namespace WX {

OperatorManager*    Handler::getOperatorManager(void) const { return getSceneManipulator()->getOperatorManager(); }

Ogre::Root*         Handler::getRoot(void) const          { return getSceneManipulator()->getRoot(); }
Ogre::RenderWindow* Handler::getRenderWindow(void) const  { return getSceneManipulator()->getRenderWindow(); }
Ogre::SceneManager* Handler::getSceneManager(void) const  { return getSceneManipulator()->getSceneManager(); }
Ogre::Camera*       Handler::getCamera(void) const        { return getSceneManipulator()->getCamera(); }
Ogre::Viewport*     Handler::getViewport(void) const      { return getSceneManipulator()->getViewport(); }

Ogre::SceneNode*    Handler::getIndicatorRootSceneNode(void) const  { return getSceneManipulator()->getIndicatorRootSceneNode(); }
Terrain*            Handler::getTerrain(void) const       { return getSceneManipulator()->getTerrain(); }
TerrainData*        Handler::getTerrainData(void) const   { return getSceneManipulator()->getTerrainData(); }
SceneInfo*          Handler::getSceneInfo(void) const     { return getSceneManipulator()->getSceneInfo(); }

}

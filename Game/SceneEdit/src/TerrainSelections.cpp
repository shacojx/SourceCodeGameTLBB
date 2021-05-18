#include "TerrainSelections.h"
#include "SceneManipulator.h"

#include "Core/Terrain.h"

namespace WX {

JunctionSelection::JunctionSelection(Terrain* terrain,SceneManipulator* sceneManipulator)
    : TerrainSelection(terrain,sceneManipulator)
{
}

JunctionSelection::~JunctionSelection()
{
}

const String&
JunctionSelection::getType(void) const
{
    static const String type = "JunctionSelection";
    return type;
}

bool
JunctionSelection::empty(void) const
{
    return mJunctions.empty();
}

void
JunctionSelection::reset(void)
{
    mJunctions.clear();
}

void
JunctionSelection::apply(void)
{
    std::vector<TerrainInfo> terrainInfo;
    for (JunctionMap::const_iterator it = mJunctions.begin(); it != mJunctions.end(); ++it)
    {
        const Junction& junction = it->second;

        float oldHeight = getTerrainData()->getHeight(junction.x,junction.z);

        TerrainInfo terrInfo;
        terrInfo.x = junction.x;
        terrInfo.z = junction.z;
        terrInfo.oldHeight = oldHeight;
        terrInfo.nowHeight = junction.height;
        terrainInfo.push_back(terrInfo);

        
        getTerrainData()->setHeight(junction.x, junction.z, junction.height);
    }
    
    getSceneManipulator()->_fireTerrainHeightChanged(terrainInfo);
    notifyModified();
}

void
JunctionSelection::notifyModified(void) const
{
    if (!mJunctions.empty())
    {
        int minx, maxx, minz, maxz;
        JunctionMap::const_iterator it = mJunctions.begin();
        minx = maxx = it->second.x;
        minz = maxz = it->second.z;
        while (++it != mJunctions.end())
        {
            int x = it->second.x;
            if (minx > x)
                minx = x;
            else if (maxx < x)
                maxx = x;
            int z = it->second.z;
            if (minz > z)
                minz = z;
            else if (maxz < z)
                maxz = z;
        }
        if (minx < 0) minx = 0;
        if (minz < 0) minz = 0;
        if (maxx > getTerrainData()->mXSize)
            maxx = getTerrainData()->mXSize;
        if (maxz > getTerrainData()->mZSize)
            maxz = getTerrainData()->mZSize;
        if (minx <= maxx && minz <= maxz)
        {
            getTerrain()->notifyHeightModified(minx, minz, maxx+1, maxz+1);
        }
    }
}

bool
JunctionSelection::add(int x, int z, Real weight)
{
    if (!getTerrainData()->isValidJunction(x, z))
        return false;

    size_t index = getTerrainData()->_getJunctionIndex(x, z);
    std::pair<JunctionMap::iterator, bool> inserted =
        mJunctions.insert(JunctionMap::value_type(index, Junction(x, z, getTerrainData()->getHeight(x, z), weight)));
    return inserted.second;
}

bool
JunctionSelection::remove(int x, int z)
{
    return getTerrainData()->isValidJunction(x, z) &&
            mJunctions.erase(getTerrainData()->_getJunctionIndex(x, z)) > 0;
}

bool
JunctionSelection::exist(int x, int z) const
{
    return getTerrainData()->isValidJunction(x, z) &&
            mJunctions.find(getTerrainData()->_getJunctionIndex(x, z)) != mJunctions.end();
}

bool
JunctionSelection::setValue(int x, int z, Real height)
{
    if (!getTerrainData()->isValidJunction(x, z))
        return false;

    JunctionMap::iterator it = mJunctions.find(getTerrainData()->_getJunctionIndex(x, z));
    if (it == mJunctions.end())
        return false;

    it->second.height = height;
    return true;
}

//////////////////////////////////////////////////////////////////////////

GridSelection::GridSelection(Terrain* terrain,SceneManipulator* sceneManipulator)
    : TerrainSelection(terrain,sceneManipulator)
{
}

GridSelection::~GridSelection()
{
}

const String&
GridSelection::getType(void) const
{
    static const String type = "GridSelection";
    return type;
}

bool
GridSelection::empty(void) const
{
    return mGrids.empty();
}

void
GridSelection::reset(void)
{
    mGrids.clear();
}

bool
GridSelection::add(int x, int z)
{
    if (!getTerrainData()->isValidGrid(x, z))
        return false;

    size_t index = getTerrainData()->_getGridIndex(x, z);
    std::pair<GridMap::iterator, bool> inserted =
        mGrids.insert(GridMap::value_type(index, Grid(x, z, getTerrainData()->mGridInfos[index])));
    return inserted.second;
}

bool
GridSelection::remove(int x, int z)
{
    return getTerrainData()->isValidGrid(x, z) &&
            mGrids.erase(getTerrainData()->_getGridIndex(x, z)) > 0;
}

bool
GridSelection::exist(int x, int z) const
{
    return getTerrainData()->isValidGrid(x, z) &&
            mGrids.find(getTerrainData()->_getGridIndex(x, z)) != mGrids.end();
}

bool
GridSelection::setValue(int x, int z, const TerrainData::GridInfo& info)
{
    if (!getTerrainData()->isValidGrid(x, z))
        return false;

    GridMap::iterator it = mGrids.find(getTerrainData()->_getGridIndex(x, z));
    if (it == mGrids.end())
        return false;

    it->second.info = info;
    return true;
}

void
GridSelection::apply(void)
{
    for (GridMap::const_iterator it = mGrids.begin(); it != mGrids.end(); ++it)
    {
        const Grid& grid = it->second;
        getTerrainData()->setGridInfo(grid.x, grid.z, grid.info);
    }
    notifyModified();
}

void
GridSelection::notifyModified(void) const
{
    if (!mGrids.empty())
    {
        int minx, maxx, minz, maxz;
        GridMap::const_iterator it = mGrids.begin();
        minx = maxx = it->second.x;
        minz = maxz = it->second.z;
        while (++it != mGrids.end())
        {
            int x = it->second.x;
            if (minx > x)
                minx = x;
            else if (maxx < x)
                maxx = x;
            int z = it->second.z;
            if (minz > z)
                minz = z;
            else if (maxz < z)
                maxz = z;
        }
        if (minx < 0) minx = 0;
        if (minz < 0) minz = 0;
        if (maxx > getTerrainData()->mXSize-1)
            maxx = getTerrainData()->mXSize-1;
        if (maxz > getTerrainData()->mZSize-1)
            maxz = getTerrainData()->mZSize-1;
        if (minx <= maxx && minz <= maxz)
        {
            getTerrain()->notifyGridInfoModified(minx, minz, maxx+1, maxz+1);
        }
    }
}

}

#ifndef __GRIDINFOFLAGMODIFYOPERATOR_H__
#define __GRIDINFOFLAGMODIFYOPERATOR_H__

#include "WXOperator.h"

namespace WX {

    class SceneManipulator;
    /** 对角翻转线的operator，用于undo redo
    */
    class GridInfoFlagModifyOperator : public Operator
    {
    public:

        struct Info
        {
            int x;
            int z;
            uint oldFlipFlag;
            uint newFlipFlag;
        };

        GridInfoFlagModifyOperator(SceneManipulator* sceneManipulator);
        ~GridInfoFlagModifyOperator();

        void add(int x, int z, uint oldFlipFlag, uint newFlipFlag);
        
        bool empty(void) const;
        
        const String& getGroupName(void) const;
        
        String getDescription(void) const;
        
        String getHelp(void) const;
        
        void undo(void);
        
        void redo(void);
        
    protected:

        void apply(uint Info::* flipFlag);
        
    protected:
        SceneManipulator* mSceneManipulator;        

        typedef std::vector<Info> InfoList;

        InfoList mInfos;
        int mMinX, mMaxX, mMinZ, mMaxZ;

        Ogre::String mIndicatorName;
        Ogre::String mGroupName;
    };

    //////////////////////////////////////////////////////////////////////////
    class FlipTerrainDiagonalOperator : public GridInfoFlagModifyOperator
    {
    public:
        FlipTerrainDiagonalOperator(SceneManipulator* sceneManipulator);
    };

    //////////////////////////////////////////////////////////////////////////
    class DisableDecalOperator : public GridInfoFlagModifyOperator
    {
    public:
        DisableDecalOperator(SceneManipulator* sceneManipulator);
    };
}

#endif // __GridInfoFlagModifyOperator_H__
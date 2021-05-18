#ifndef __FAIRYMODIFYTERRAINHEIGHTOPERATOR_H__
#define __FAIRYMODIFYTERRAINHEIGHTOPERATOR_H__

#include "WXOperator.h"

namespace WX
{
    class SceneManipulator;

    class ModifyTerrainHeightOperator : public Operator
    {
    public:
        ModifyTerrainHeightOperator(SceneManipulator* sceneManipulator);
        ~ModifyTerrainHeightOperator();

        void add(int x, int z, Real oldHeight, Real newHeight);
        bool empty(void) const;

        /** @copydoc Operator::getGroupName */
        const String& getGroupName(void) const;

        /** @copydoc Operator::getDescription */
        String getDescription(void) const;

        /** @copydoc Operator::getHelp */
        String getHelp(void) const;

        /** @copydoc Operator::undo */
        void undo(void);

        /** @copydoc Operator::redo */
        void redo(void);

    protected:
        SceneManipulator* mSceneManipulator;

        struct Info
        {
            int x;
            int z;
            Real oldHeight;
            Real newHeight;
        };

        typedef std::vector<Info> InfoList;

        InfoList mInfos;
        int mMinX, mMaxX, mMinZ, mMaxZ;

        void apply(Real Info::* height);
    };
}

#endif 

#ifndef __FAIRYRENAMEOBJECTOPERATOR_H__
#define __FAIRYRENAMEOBJECTOPERATOR_H__

#include "WXOperator.h"

namespace WX
{
    class SceneManipulator;

    class RenameObjectOperator : public Operator
    {
    public:
        RenameObjectOperator(SceneManipulator* sceneManipulator);
        ~RenameObjectOperator();

        void add(const String& oldName, const String& newName);

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

        typedef std::pair<String, String> NamePair;
        typedef std::list<NamePair> NamePairList;

        NamePairList mRenameList;
    };
}

#endif 

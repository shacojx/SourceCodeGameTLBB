#include "WXRenameObjectOperator.h"
#include "SceneManipulator.h"

#include "Core/WXSceneInfo.h"

namespace WX
{

    RenameObjectOperator::RenameObjectOperator(SceneManipulator* sceneManipulator)
        : mSceneManipulator(sceneManipulator)
        , mRenameList()
    {
    }
    //-----------------------------------------------------------------------
    RenameObjectOperator::~RenameObjectOperator()
    {
    }
    //-----------------------------------------------------------------------
    void RenameObjectOperator::add(const String& oldName, const String& newName)
    {
        assert(!oldName.empty());
        assert(!newName.empty());

        mRenameList.push_back(NamePair(oldName, newName));
    }
    //-----------------------------------------------------------------------
    const String& RenameObjectOperator::getGroupName(void) const
    {
        static const String groupName = "Objects";
        return groupName;
    }
    //-----------------------------------------------------------------------
    String RenameObjectOperator::getDescription(void) const
    {
        // TODO:
        return "";
    }
    //-----------------------------------------------------------------------
    String RenameObjectOperator::getHelp(void) const
    {
        // TODO
        return "";
    }
    //-----------------------------------------------------------------------
    void RenameObjectOperator::undo(void)
    {
        for (NamePairList::const_iterator i = mRenameList.begin(); i != mRenameList.end(); ++i)
        {
            // first: oldName, second: newName
            const ObjectPtr& object = mSceneManipulator->getSceneInfo()->findObjectByName(i->second);
            assert(object);
            mSceneManipulator->renameObject(object, i->first, NULL, true);
        }
    }
    //-----------------------------------------------------------------------
    void RenameObjectOperator::redo(void)
    {
        for (NamePairList::const_iterator i = mRenameList.begin(); i != mRenameList.end(); ++i)
        {
            // first: oldName, second: newName
            const ObjectPtr& object = mSceneManipulator->getSceneInfo()->findObjectByName(i->first);
            assert(object);
            mSceneManipulator->renameObject(object, i->second, NULL, true);
        }
    }

}

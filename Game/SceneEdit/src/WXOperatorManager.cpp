#include "WXOperatorManager.h"
#include "WXOperator.h"

namespace WX
{

    static const OperatorManager::OperatorList nullOperatorList;
    //-----------------------------------------------------------------------
    static void clearOperatorList(OperatorManager::OperatorList& operators)
    {
        OperatorManager::OperatorList::const_iterator it;
        for (it = operators.begin(); it != operators.end(); ++it)
        {
            delete *it;
        }
        operators.clear();
    }
    //-----------------------------------------------------------------------
    static void eraseOperatorByGroupName(OperatorManager::OperatorList& operators, const String& groupName)
    {
        OperatorManager::OperatorList::iterator it = operators.begin();
        while (it != operators.end())
        {
            if ((*it)->getGroupName() == groupName)
                operators.erase(it++);
            else
                ++it;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    class OperatorManager::Group
    {
    protected:
        OperatorList _undoList;
        OperatorList _redoList;

    public:
        Group(void);
        ~Group();

        void reset(void);

        void add(Operator* op);

        size_t undo(size_t count = 1);
        size_t redo(size_t count = 1);

        const OperatorList& getUndoOperators(void) const;
        const OperatorList& getRedoOperators(void) const;
    };
    //-----------------------------------------------------------------------
    OperatorManager::Group::Group(void)
        : _undoList()
        , _redoList()
    {
    }
    //-----------------------------------------------------------------------
    OperatorManager::Group::~Group()
    {
        reset();
    }
    //-----------------------------------------------------------------------
    void OperatorManager::Group::add(Operator* op)
    {
        assert(op);

        clearOperatorList(_redoList);
        _undoList.push_front(op);
    }
    //-----------------------------------------------------------------------
    void OperatorManager::Group::reset(void)
    {
        clearOperatorList(_redoList);
        clearOperatorList(_undoList);
    }
    //-----------------------------------------------------------------------
    size_t OperatorManager::Group::undo(size_t count)
    {
        size_t n = 0;

        while (n < count && !_undoList.empty())
        {
            Operator* op = _undoList.front();
            op->undo();
            _redoList.push_front(op);
            _undoList.pop_front();
            ++n;
        }

        return n;
    }
    //-----------------------------------------------------------------------
    size_t OperatorManager::Group::redo(size_t count)
    {
        size_t n = 0;

        while (n < count && !_redoList.empty())
        {
            Operator* op = _redoList.front();
            op->redo();
            _undoList.push_front(op);
            _redoList.pop_front();
            ++n;
        }

        return n;
    }
    //-----------------------------------------------------------------------
    const OperatorManager::OperatorList& OperatorManager::Group::getUndoOperators(void) const
    {
        return _undoList;
    }
    //-----------------------------------------------------------------------
    const OperatorManager::OperatorList& OperatorManager::Group::getRedoOperators(void) const
    {
        return _redoList;
    }
    //////////////////////////////////////////////////////////////////////////
    OperatorManager::OperatorManager(void)
        : mUndoList()
        , mRedoList()
        , mGroups()
        , mTimestamp(0)
    {
    }
    //-----------------------------------------------------------------------
    OperatorManager::~OperatorManager()
    {
        reset();
    }
    //-----------------------------------------------------------------------
    void OperatorManager::reset(void)
    {
        for (GroupMap::const_iterator it = mGroups.begin(); it != mGroups.end(); ++it)
        {
            delete it->second;
        }
        mGroups.clear();
        mUndoList.clear();
        mRedoList.clear();
    }
    //-----------------------------------------------------------------------
    OperatorManager::Group* OperatorManager::findGroup(const String& groupName)
    {
        GroupMap::const_iterator it = mGroups.find(groupName);
        if (it == mGroups.end())
            return NULL;
        return it->second;
    }
    //-----------------------------------------------------------------------
    const OperatorManager::Group* OperatorManager::findGroup(const String& groupName) const
    {
        return const_cast<OperatorManager*>(this)->findGroup(groupName);
    }
    //-----------------------------------------------------------------------
    void OperatorManager::addOperator(Operator* op)
    {
        assert(op);

        // Setup timestamp
        op->setTimestamp(++mTimestamp);

        const String& groupName = op->getGroupName();

        // Erase all operator that has same group name from global redo list
        OperatorList::iterator it = mRedoList.begin();
        while (it != mRedoList.end())
        {
            if ((*it)->getGroupName() == groupName)
                mRedoList.erase(it++);
            else
                ++it;
        }

        // Add to global undo list
        mUndoList.push_front(op);

        // Find or create the group by group name
        Group* group = findGroup(groupName);
        if (!group)
        {
            group = new Group;
            std::pair<GroupMap::iterator, bool> inserted =
                mGroups.insert(GroupMap::value_type(groupName, group));
            assert(inserted.second);
        }

        // Add to the group
        group->add(op);
    }
    //-----------------------------------------------------------------------
    void OperatorManager::applyOperator(Operator* op)
    {
        assert(op);

        op->redo();
        addOperator(op);
    }
    //-----------------------------------------------------------------------
    size_t OperatorManager::undo(size_t count)
    {
        size_t n = 0;

        while (n < count && !mUndoList.empty())
        {
            Operator* op = mUndoList.front();
            Group* group = findGroup(op->getGroupName());
            assert(group);
            group->undo();
            mRedoList.push_front(op);
            mUndoList.pop_front();
            ++n;
        }

        return n;
    }
    //-----------------------------------------------------------------------
    size_t OperatorManager::redo(size_t count)
    {
        size_t n = 0;

        while (n < count && !mRedoList.empty())
        {
            Operator* op = mRedoList.front();
            Group* group = findGroup(op->getGroupName());
            assert(group);
            group->redo();
            mUndoList.push_front(op);
            mRedoList.pop_front();
            ++n;
        }

        return n;
    }
    //-----------------------------------------------------------------------
    size_t OperatorManager::undo(const String& groupName, size_t count)
    {
        size_t n = 0;

        Group* group = findGroup(groupName);
        if (group)
        {
            n = group->undo(count);

            // Move all undo'ed operator from undo list to redo list
            size_t m = 0;
            OperatorList::iterator itUndo = mUndoList.begin();
            OperatorList::iterator itRedo = mRedoList.begin();
            while (m < n && itUndo != mUndoList.end())
            {
                Operator* op = *itUndo;
                if (op->getGroupName() == groupName)
                {
                    // Find the correct insert place, redo list is sort ascend by timestamp
                    size_t timestamp = op->getTimestamp();
                    while (itRedo != mRedoList.end() && timestamp > (*itRedo)->getTimestamp())
                    {
                        ++itRedo;
                    }
                    mRedoList.insert(itRedo, op);
                    mUndoList.erase(itUndo++);
                    ++m;
                }
                else
                {
                    ++itUndo;
                }
            }
            assert(m == n);
        }

        return n;
    }
    //-----------------------------------------------------------------------
    size_t OperatorManager::redo(const String& groupName, size_t count)
    {
        size_t n = 0;

        Group* group = findGroup(groupName);
        if (group)
        {
            n = group->redo(count);

            // Move all redo'ed operator from redo list to undo list
            size_t m = 0;
            OperatorList::iterator itRedo = mRedoList.begin();
            OperatorList::iterator itUndo = mUndoList.begin();
            while (m < n && itRedo != mRedoList.end())
            {
                Operator* op = *itRedo;
                if (op->getGroupName() == groupName)
                {
                    // Find the correct insert place, undo list is sort descend by timestamp
                    size_t timestamp = op->getTimestamp();
                    while (itUndo != mUndoList.end() && timestamp < (*itUndo)->getTimestamp())
                    {
                        ++itUndo;
                    }
                    mUndoList.insert(itUndo, op);
                    mRedoList.erase(itRedo++);
                    ++m;
                }
                else
                {
                    ++itRedo;
                }
            }
            assert(m == n);
        }

        return n;
    }
    //-----------------------------------------------------------------------
    OperatorManager::GroupNameList OperatorManager::getGroupNameList(void) const
    {
        GroupNameList groupNames;
        for (GroupMap::const_iterator it = mGroups.begin(); it != mGroups.end(); ++it)
        {
            groupNames.push_back(it->first);
        }
        return groupNames;
    }
    //-----------------------------------------------------------------------
    OperatorManager::Snapshot OperatorManager::makeSnapshot(void) const
    {
        Snapshot snapshot;
        for (GroupMap::const_iterator it = mGroups.begin(); it != mGroups.end(); ++it)
        {
            const Group* group = it->second;
            if (!group->getUndoOperators().empty())
                snapshot.insert(Snapshot::value_type(it->first, group->getUndoOperators().front()->getTimestamp()));
        }
        return snapshot;
    }
    //-----------------------------------------------------------------------
    bool OperatorManager::checkSnapshot(const Snapshot& snapshot) const
    {
        GroupMap::const_iterator itGroup = mGroups.begin();
        Snapshot::const_iterator itSnapshot = snapshot.begin();
        while (itGroup != mGroups.end())
        {
            const Group* group = itGroup->second;
            if (!group->getUndoOperators().empty())
            {
                if (itSnapshot == snapshot.end())
                    return false;
                if (itGroup->first != itSnapshot->first)
                    return false;
                if (group->getUndoOperators().front()->getTimestamp() != itSnapshot->second)
                    return false;
                ++itSnapshot;
            }
            ++itGroup;
        }
        return itSnapshot == snapshot.end();
    }
    //-----------------------------------------------------------------------
    const OperatorManager::OperatorList& OperatorManager::getUndoOperators(void) const
    {
        return mUndoList;
    }
    //-----------------------------------------------------------------------
    const OperatorManager::OperatorList& OperatorManager::getRedoOperators(void) const
    {
        return mRedoList;
    }
    //-----------------------------------------------------------------------
    const OperatorManager::OperatorList& OperatorManager::getUndoOperators(const String& groupName) const
    {
        const Group* group = findGroup(groupName);
        if (!group)
            return nullOperatorList;
        return group->getUndoOperators();
    }
    //-----------------------------------------------------------------------
    const OperatorManager::OperatorList& OperatorManager::getRedoOperators(const String& groupName) const
    {
        const Group* group = findGroup(groupName);
        if (!group)
            return nullOperatorList;
        return group->getRedoOperators();
    }

}

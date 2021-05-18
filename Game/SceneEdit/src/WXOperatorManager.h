#ifndef __FAIRYOPERATORMANAGER_H__
#define __FAIRYOPERATORMANAGER_H__

#include "Core/WXPrerequisites.h"

namespace WX {

    class Operator;

    class OperatorManager
    {
    public:
        typedef size_t Timestamp;
        typedef std::map<String, Timestamp> Snapshot;
        typedef std::list<String> GroupNameList;
        typedef std::list<Operator*> OperatorList;

    public:
        OperatorManager(void);
        ~OperatorManager();

        void reset(void);

        void addOperator(Operator* op);
        void applyOperator(Operator* op);

        size_t undo(size_t count = 1);
        size_t redo(size_t count = 1);
        size_t undo(const String& groupName, size_t count = 1);
        size_t redo(const String& groupName, size_t count = 1);

        GroupNameList getGroupNameList(void) const;

        Snapshot makeSnapshot(void) const;
        bool checkSnapshot(const Snapshot& snapshot) const;

        const OperatorList& getUndoOperators(void) const;
        const OperatorList& getRedoOperators(void) const;
        const OperatorList& getUndoOperators(const String& groupName) const;
        const OperatorList& getRedoOperators(const String& groupName) const;

    protected:
        class Group;
        typedef std::map<String, Group*> GroupMap;

        Group* findGroup(const String& groupName);
        const Group* findGroup(const String& groupName) const;

        GroupMap mGroups;
        OperatorList mUndoList;
        OperatorList mRedoList;
        Timestamp mTimestamp;
    };

}

#endif // 

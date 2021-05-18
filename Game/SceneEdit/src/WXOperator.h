#ifndef __FAIRYOPERATOR_H__
#define __FAIRYOPERATOR_H__

#include "Core/WXPrerequisites.h"

namespace WX {

    class Operator
    {
    public:
        typedef size_t Timestamp;

    protected:
        Timestamp _timestamp;

    public:
        Operator(void) : _timestamp(0) {}

        /** Virtual desctructor, see Effective C++ */
        virtual ~Operator() {}

        void setTimestamp(Timestamp timestamp) { _timestamp = timestamp; }
        Timestamp getTimestamp(void) const     { return _timestamp; }

        /** Retrieves a group name of this operator
        @remarks
            The OperatorManager will manager grouping operators by group name,
            so undo/redo can be perform independent with each operator group.
        @par
            The operators must use same group name when their may be influence
            by each other.
        @returns
            group name of this operator
        */
        virtual const String& getGroupName(void) const = 0;

        /** Retrieves a description of this operator
        @remarks
            The description should be display as a menu text.
        @returns
            description of this operator
        */
        virtual String getDescription(void) const = 0;

        /** Retrieves a help text of this operator
        @remarks
            The help text should be use as a detail description of this operator,
            when use selecting menu item of this operator, this help text will
            display in status line.
        @returns
            help text of this operator
        */
        virtual String getHelp(void) const = 0;

        /** Perform undo operate
        */
        virtual void undo(void) = 0;

        /** Perform redo operate
        */
        virtual void redo(void) = 0;
    };

}

#endif // 

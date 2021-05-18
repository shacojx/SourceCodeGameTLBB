#ifndef __EVENTAREAEDITOPERATOR_H__
#define __EVENTAREAEDITOPERATOR_H__
#pragma once
#include "WXOperator.h"
#include <OgreVector3.h>
namespace WX 
{
	class CEventAreaEditAction;

	class CEventAreaEditOperator: public Operator
	{
	public:
		CEventAreaEditOperator(void);
		~CEventAreaEditOperator(void);
	
	public:

		unsigned long m_ulTypeId;
		unsigned long m_ulInstanceId;

		Ogre::Vector3 m_vectorUnDoPos;
		Ogre::Vector3 m_vectorReDoPos;

		CEventAreaEditAction* m_pAction;

		// 设置区域编辑操作的指针.
		void SetAction(CEventAreaEditAction* pAction);

		// 设置区域ID
		void SetInstanceId(unsigned long ulInstanceId);

		// 设置点ID
		void SetTypeId(unsigned long ulTypeId);

		// 设置undo， redo 操作需要的位置.
		void SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos);

	};


	// 修改一个点的位置的redo， undo函数.
	//

	class CEventAreaEditDelOperator : public CEventAreaEditOperator
	{
	public:
		CEventAreaEditDelOperator(void);
		~CEventAreaEditDelOperator(void);

	public:
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
        virtual const String& getGroupName(void) const;

        /** Retrieves a description of this operator
        @remarks
            The description should be display as a menu text.
        @returns
            description of this operator
        */
        virtual String getDescription(void) const;

        /** Retrieves a help text of this operator
        @remarks
            The help text should be use as a detail description of this operator,
            when use selecting menu item of this operator, this help text will
            display in status line.
        @returns
            help text of this operator
        */
        virtual String getHelp(void) const;

        /** Perform undo operate
        */
        virtual void undo(void);

        /** Perform redo operate
        */
        virtual void redo(void);

	};



	/////////////////////////////////////////////////////////////////////////////////
	//
	// 添加一个点的redo， undo操作.
	//
	//
	class CEventAreaEditAddOperator : public CEventAreaEditOperator
	{
	public:
		CEventAreaEditAddOperator(void);
		~CEventAreaEditAddOperator(void);

	public:
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
        virtual const String& getGroupName(void) const;

        /** Retrieves a description of this operator
        @remarks
            The description should be display as a menu text.
        @returns
            description of this operator
        */
        virtual String getDescription(void) const;

        /** Retrieves a help text of this operator
        @remarks
            The help text should be use as a detail description of this operator,
            when use selecting menu item of this operator, this help text will
            display in status line.
        @returns
            help text of this operator
        */
        virtual String getHelp(void) const;

        /** Perform undo operate
        */
        virtual void undo(void);

        /** Perform redo operate
        */
        virtual void redo(void);

	};//
}

#endif //__EVENTAREAEDITOPERATOR_H__
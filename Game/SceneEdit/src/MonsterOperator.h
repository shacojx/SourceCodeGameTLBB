#ifndef __MONSTEROPERATOR_H__
#define __MONSTEROPERATOR_H__
#pragma once

#include "WXOperator.h"
#include <OgreVector3.h>

namespace WX 
{

	class CMonsterEditaction;
	class CMonsterOperator : public Operator
	{
	public:
		CMonsterOperator(void);
		~CMonsterOperator(void);

	public:

		// 怪物实例id
		unsigned long m_ulMonsterInstanceId;

		// 怪物外形点
        unsigned long m_ulMonsterId;

		// 怪物名字
		String m_strMonsterName;

		Ogre::Vector3 m_vectorUnDoPos;
		Ogre::Vector3 m_vectorReDoPos;

		CMonsterEditaction* m_pMonsterAction;

		// 设置区域编辑操作的指针.
		void SetMonsterAction(CMonsterEditaction* pMonsterAction);

		// 怪物实例id
		void SetMonsterInstanceId(unsigned long ulMonsterInstanceId);

		// 设置怪物外形id
		void SetMonsterId(unsigned long ulMonsterId);

		// 设置undo， redo 操作需要的位置.
		void SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos);

		// 怪物名字.
		void SetMonsterName(String& strName);


		virtual const String& getGroupName(void) const;
        virtual String getDescription(void) const;
	    virtual String getHelp(void) const;
        virtual void undo(void);
        virtual void redo(void);

	};



	/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// 添加一个新怪
	//
	class CMonsterEditAddNewMonsterOperator : public CMonsterOperator
	{
	public:
		CMonsterEditAddNewMonsterOperator(void);
		~CMonsterEditAddNewMonsterOperator(void);

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
	// 删除一个怪物的操作.
	//
	//
	class CMonsterEditDelMonsterOperator : public CMonsterOperator
	{
	public:
		CMonsterEditDelMonsterOperator(void);
		~CMonsterEditDelMonsterOperator(void);

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



















}// namespace WX 

#endif //__MONSTEROPERATOR_H__
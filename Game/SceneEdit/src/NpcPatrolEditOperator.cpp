#include "npcpatroleditoperator.h"
#include "NpcPatrolAction.h"

namespace WX 
{
	
	////////////////////////////////////////////////////////////////////////////////
	//
	// 基类 CRegionEditOperator 的实现函数
	//
	CNpcPatrolEditOperator::CNpcPatrolEditOperator(void)
	{
		m_pRegionAction = NULL;
	}

	CNpcPatrolEditOperator::~CNpcPatrolEditOperator(void)
	{
	}

	void CNpcPatrolEditOperator::SetRegionAction(CNpcPatrolAction* pRegionAction)
	{
		m_pRegionAction = pRegionAction;
	}

	// 设置区域ID
	void CNpcPatrolEditOperator::SetRegionId(unsigned long ulRegionId)
	{
		m_ulRegionId = ulRegionId;
	}

	// 设置点ID
	void CNpcPatrolEditOperator::SetPointId(unsigned long ulPointId)
	{
		m_ulPointId = ulPointId;
	}

	// 设置undo， redo 操作需要的位置.
	void CNpcPatrolEditOperator::SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos)
	{
		m_vectorUnDoPos = vectorUnDoPos;
		m_vectorReDoPos = vectorReDoPos;
	}



	/////////////////////////////////////////////////////////////////////////////////
	//
	// 修改一个点的redo， undo操作.
	//
	//
	CNpcPatrolEditModifyOperator::CNpcPatrolEditModifyOperator(void)
	{

	}

	CNpcPatrolEditModifyOperator::~CNpcPatrolEditModifyOperator(void)
	{

	}


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
	const String& CNpcPatrolEditModifyOperator::getGroupName(void) const
	{
		static String strGroupName = "RegionModify";
		return strGroupName;
	}

        /** Retrieves a description of this operator
        @remarks
            The description should be display as a menu text.
        @returns
            description of this operator
        */
     String CNpcPatrolEditModifyOperator::getDescription(void) const
	 {
		return String("RegionModify");
	 }

        /** Retrieves a help text of this operator
        @remarks
            The help text should be use as a detail description of this operator,
            when use selecting menu item of this operator, this help text will
            display in status line.
        @returns
            help text of this operator
        */
      String CNpcPatrolEditModifyOperator::getHelp(void) const
	  {
		return String("RegionModify");
	  }

        /** Perform undo operate
        */
      void CNpcPatrolEditModifyOperator::undo(void)
	  {
		 if(m_pRegionAction)
		 {
			 m_pRegionAction->ChangeCurEditPointPos(m_ulRegionId, m_ulPointId, m_vectorUnDoPos);
		 }
	  }

        /** Perform redo operate
        */
      void CNpcPatrolEditModifyOperator::redo(void)
	  {
		 if(m_pRegionAction)
		 {
			 m_pRegionAction->ChangeCurEditPointPos(m_ulRegionId, m_ulPointId, m_vectorReDoPos);
		 }
	  }







	/////////////////////////////////////////////////////////////////////////////////
	//
	// 添加一个点的redo， undo操作.
	//
	//


	  CNpcPatrolEditAddOperator::CNpcPatrolEditAddOperator(void)
	  {

	  }
		
	  CNpcPatrolEditAddOperator::~CNpcPatrolEditAddOperator(void)
	  {

	  }

	
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
	  const String& CNpcPatrolEditAddOperator::getGroupName(void) const
	  {
		  static String strGroupName = "AddNewPoint";

		  return strGroupName;
	  }

        /** Retrieves a description of this operator
        @remarks
            The description should be display as a menu text.
        @returns
            description of this operator
        */
	  String CNpcPatrolEditAddOperator::getDescription(void) const
	  {
		  return "AddNewPoint";
	  }

        /** Retrieves a help text of this operator
        @remarks
            The help text should be use as a detail description of this operator,
            when use selecting menu item of this operator, this help text will
            display in status line.
        @returns
            help text of this operator
        */
	  String CNpcPatrolEditAddOperator::getHelp(void) const
	  {
		 return "AddNewPoint";
	  }

        /** Perform undo operate
        */
		void CNpcPatrolEditAddOperator::undo(void)
		{
			if(m_pRegionAction)
			{
				m_pRegionAction->DelPoint(m_ulRegionId, m_ulPointId);
			}
		}

        /** Perform redo operate
        */
		void CNpcPatrolEditAddOperator::redo(void)
		{
			if(m_pRegionAction)
			{
				m_pRegionAction->AddNewPoint(m_ulRegionId, m_ulPointId, m_vectorReDoPos);
			}
		
		}




}
#include "MonsterOperator.h"
#include "MonsterEditAction.h"
////////////////////////////////////////////////////////////////////////////
//
// 怪物redo undo 基类操作.
//

namespace WX 
{
	CMonsterOperator::CMonsterOperator(void)
	:m_strMonsterName("")
	{
	}

	CMonsterOperator::~CMonsterOperator(void)
	{
	}


	// 怪物名字.
	void CMonsterOperator::SetMonsterName(String& strName)
	{
		m_strMonsterName = strName;
	}

	// 设置区域编辑操作的指针.
	void CMonsterOperator::SetMonsterAction(CMonsterEditaction* pMonsterAction)
	{
		m_pMonsterAction = pMonsterAction;
	}

	// 怪物实例id
	void CMonsterOperator::SetMonsterInstanceId(unsigned long ulMonsterInstanceId)
	{
		m_ulMonsterInstanceId = ulMonsterInstanceId;
	}


	// 设置怪物外形id
	void CMonsterOperator::SetMonsterId(unsigned long ulMonsterId)
	{
		m_ulMonsterId = ulMonsterId;
	}

	// 设置undo， redo 操作需要的位置.
	void CMonsterOperator::SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos)
	{
		m_vectorUnDoPos = vectorUnDoPos;
		m_vectorReDoPos = vectorReDoPos;
	}


	const String& CMonsterOperator::getGroupName(void) const
	{
		static String strGroupName = "BaseMonsterAdd";
		return strGroupName;
	}

	String CMonsterOperator::getDescription(void) const
	{
		return String("BaseMonsterAdd");
	}

	String CMonsterOperator::getHelp(void) const
	{
		return String("BaseMonsterAdd");
	}

	void CMonsterOperator::undo(void)
	{

	}

	void CMonsterOperator::redo(void)
	{

	}





	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 具体redo undo操作.
	//
	//


	// 添加一个新怪.
	CMonsterEditAddNewMonsterOperator::CMonsterEditAddNewMonsterOperator(void)
	{

	}

	CMonsterEditAddNewMonsterOperator::~CMonsterEditAddNewMonsterOperator(void)
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
	const String& CMonsterEditAddNewMonsterOperator::getGroupName(void) const
	{
		static String strGroupName = "MonsterAdd";
		return strGroupName;
	}

	/** Retrieves a description of this operator
	@remarks
		The description should be display as a menu text.
	@returns
		description of this operator
	*/
	String CMonsterEditAddNewMonsterOperator::getDescription(void) const
	{
	return String("MonsterAdd");
	}

	/** Retrieves a help text of this operator
	@remarks
		The help text should be use as a detail description of this operator,
		when use selecting menu item of this operator, this help text will
		display in status line.
	@returns
		help text of this operator
	*/
	String CMonsterEditAddNewMonsterOperator::getHelp(void) const
	{
	return String("MonsterAdd");
	}

	/** Perform undo operate
	*/
	void CMonsterEditAddNewMonsterOperator::undo(void)
	{
		if(m_pMonsterAction)
		{
			m_pMonsterAction->DelMonster(m_ulMonsterInstanceId);
		}
	}

	/** Perform redo operate
	*/
	void CMonsterEditAddNewMonsterOperator::redo(void)
	{
		if(m_pMonsterAction)
		{
			m_pMonsterAction->AddNewMonster(m_strMonsterName, m_vectorReDoPos, m_ulMonsterId);
			m_ulMonsterInstanceId = m_pMonsterAction->m_dwMonsterInstanceId - 1;
		}
	}







		/////////////////////////////////////////////////////////////////////////////////
		//
		// 添加一个点的redo， undo操作.
		//
		//


		CMonsterEditDelMonsterOperator::CMonsterEditDelMonsterOperator(void)
		{

		}
			
		CMonsterEditDelMonsterOperator::~CMonsterEditDelMonsterOperator(void)
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
		const String& CMonsterEditDelMonsterOperator::getGroupName(void) const
		{
			static String strGroupName = "MonsterDel";

			return strGroupName;
		}

		
		/** Retrieves a description of this operator
		@remarks
			The description should be display as a menu text.
		@returns
			description of this operator
		*/
		String CMonsterEditDelMonsterOperator::getDescription(void) const
		{
			return "MonsterDel";
		}

		/** Retrieves a help text of this operator
		@remarks
			The help text should be use as a detail description of this operator,
			when use selecting menu item of this operator, this help text will
			display in status line.
		@returns
			help text of this operator
		*/
		String CMonsterEditDelMonsterOperator::getHelp(void) const
		{
			return "MonsterDel";
		}

		/** Perform undo operate
		*/
		void CMonsterEditDelMonsterOperator::undo(void)
		{
			if(m_pMonsterAction)
			{
				m_pMonsterAction->AddNewMonster(m_vectorUnDoPos, m_ulMonsterId);
				m_ulMonsterInstanceId = m_pMonsterAction->m_dwMonsterInstanceId - 1;
			}//
		}

		/** Perform redo operate
		*/
		void CMonsterEditDelMonsterOperator::redo(void)
		{
			if(m_pMonsterAction)
			{
				m_pMonsterAction->DelMonster(m_ulMonsterInstanceId);
			}//
		
		}



}

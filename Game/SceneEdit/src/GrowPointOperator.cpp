#include "GrowPointOperator.h"

namespace WX 
{

	CGrowPointOperator::CGrowPointOperator(void)
	{
	}

	CGrowPointOperator::~CGrowPointOperator(void)
	{
	}


	const String& CGrowPointOperator::getGroupName(void) const
	{
	static String strGroupName = "GrowPoint";
	return strGroupName;
	}

	String CGrowPointOperator::getDescription(void) const
	{
	return String("GrowPoint");
	}


	String CGrowPointOperator::getHelp(void) const
	{
	return String("GrowPoint");
	}


	void CGrowPointOperator::undo(void)
	{

	}


	void CGrowPointOperator::redo(void)
	{

	}




		// 设置区域编辑操作的指针.
	void CGrowPointOperator::SetAction(CGrowPointEditAction* pAction)
	{
		
		m_pAction = pAction;

	}

		// 怪物实例id
	void CGrowPointOperator::SetInstanceId(unsigned long ulInstanceId)
	{
		
		m_ulInstanceId = ulInstanceId;
	
	}

		// 设置怪物外形id
	void CGrowPointOperator::SetTypeId(unsigned long ulTypeId)
	{
		
		m_ulTypeId = ulTypeId;

	}

		// 设置undo， redo 操作需要的位置.
	void CGrowPointOperator::SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos)
	{
		
		m_vectorUnDoPos = vectorUnDoPos;
		m_vectorReDoPos = vectorReDoPos;

	}

		// 怪物名字.
	void CGrowPointOperator::SetName(String& strName)
	{
		// 怪物名字
		 m_strName = strName;
	}





	/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// 添加一个新怪
	//
	CGrowPointAddOperator::CGrowPointAddOperator(void)
	{

	}
	CGrowPointAddOperator::~CGrowPointAddOperator(void)
	{

	}


	void CGrowPointAddOperator::undo(void)
	{
		if(m_pAction)
		{
			m_pAction->DelGrowPoint(m_ulInstanceId);
		}

	}
	void CGrowPointAddOperator::redo(void)
	{
		if(m_pAction)
		{
			
			m_ulInstanceId = m_pAction->AddNewGrowPoint(m_ulTypeId, m_vectorReDoPos);
			
		}

	}

	const String& CGrowPointAddOperator::getGroupName(void) const
	{
		static String strGroupName = "GrowPointAdd";
		return strGroupName;

	}
	String CGrowPointAddOperator::getDescription(void) const
	{
		return String("GrowPointAdd");
	}
	String CGrowPointAddOperator::getHelp(void) const
	{
		return String("GrowPointAdd");
	}



	/////////////////////////////////////////////////////////////////////////////////
	//
	// 删除一个怪物的操作.
	//
	//
	CGrowPointDelOperator::CGrowPointDelOperator(void)
	{

	}
	
	CGrowPointDelOperator::~CGrowPointDelOperator(void)
	{

	}

	
	void CGrowPointDelOperator::undo(void)
	{
		if(m_pAction)
		{
			m_ulInstanceId = m_pAction->AddNewGrowPoint(m_ulTypeId, m_vectorUnDoPos);
			m_vectorUnDoPos = m_vectorUnDoPos;
			m_vectorReDoPos = m_vectorUnDoPos;
		}

	}
	void CGrowPointDelOperator::redo(void)
	{
		if(m_pAction)
		{
			m_pAction->DelGrowPoint(m_ulInstanceId);
		}
	}


	const String& CGrowPointDelOperator::getGroupName(void) const
	{
		static String strGroupName = "GrowPointDel";
		return strGroupName;

	}

	String CGrowPointDelOperator::getDescription(void) const
	{
		return String("GrowPointDel");
	}

	String CGrowPointDelOperator::getHelp(void) const
	{
		return String("GrowPointDel");
	}

	

}

#include ".\stallinfoeditoperator.h"
#include ".\stallinfoaction.h"

namespace WX 
{
	CStallInfoEditOperator::CStallInfoEditOperator(void)
	{
		m_pAction = NULL;
	}

	CStallInfoEditOperator::~CStallInfoEditOperator(void)
	{

	}

	// 设置区域编辑操作的指针.
	void CStallInfoEditOperator::SetAction(CStallInfoAction* pAction)
	{
		m_pAction = pAction;
	}

	
	// 设置位置
	void CStallInfoEditOperator::SetPos(unsigned long dwposX, unsigned long dwposY, unsigned long dwEndPosX, unsigned long dwEndPosY)
	{
		m_dwStartPosX = dwposX;
		m_dwStartPosY = dwposY;
		m_dwEndPosX	  =	dwEndPosX;
		m_dwEndPosY	  = dwEndPosY;

	}

	// 设置位置信息
	void CStallInfoEditOperator::SetStallInfo(	
												unsigned char	m_bCanStall,	//是否可以摆摊
												unsigned char	m_iTradeTax,	//交易税
												unsigned long	m_dwPosTax,		//摊位费
												unsigned char	m_iExtraInfo	//附加信息
												)
	{
		m_bCanStall  = m_bCanStall;		//是否可以摆摊
		m_iTradeTax  = m_iTradeTax;		//交易税
		m_dwPosTax   = m_dwPosTax;		//摊位费
		m_iExtraInfo = m_iExtraInfo;	//附加信息
	}

	const String& CStallInfoEditOperator::getGroupName(void) const
	{
		static String strGroupName = "StallInfo";
		return strGroupName;
	}

	String CStallInfoEditOperator::getDescription(void) const
	{
		return String("StallInfo");
	}

	String CStallInfoEditOperator::getHelp(void) const
	{
		return String("StallInfo");
	}

	void CStallInfoEditOperator::undo(void)
	{

	}

	void CStallInfoEditOperator::redo(void)
	{

	}





	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 删除摊位
	//

	CStallInfoEditDelOperator::CStallInfoEditDelOperator(void)
	{
	}
	CStallInfoEditDelOperator::~CStallInfoEditDelOperator(void)
	{

	}

	const String& CStallInfoEditDelOperator::getGroupName(void) const
	{
		static String strGroupName = "StallInfoDel";
		return strGroupName;
	}

	String CStallInfoEditDelOperator::getDescription(void) const
	{
		return String("StallInfoDel");
	}

	String CStallInfoEditDelOperator::getHelp(void) const
	{
		return String("StallInfoDel");
	}

	void CStallInfoEditDelOperator::undo(void)
	{

	}

	void CStallInfoEditDelOperator::redo(void)
	{

	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 添加摊位
	//
	CStallInfoEditAddOperator::CStallInfoEditAddOperator(void)
	{

	}
	CStallInfoEditAddOperator::~CStallInfoEditAddOperator(void)
	{

	}
	const String& CStallInfoEditAddOperator::getGroupName(void) const
	{
		static String strGroupName = "StallInfoAdd";
		return strGroupName;
	}

	String CStallInfoEditAddOperator::getDescription(void) const
	{
		return String("StallInfoAdd");
	}

	String CStallInfoEditAddOperator::getHelp(void) const
	{
		return String("StallInfoAdd");
	}

	void CStallInfoEditAddOperator::undo(void)
	{

	}

	void CStallInfoEditAddOperator::redo(void)
	{

	}

}

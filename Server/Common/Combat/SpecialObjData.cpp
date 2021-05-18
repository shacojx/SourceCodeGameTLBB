#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：SpecialObjData.cpp
// 程序员：高骐
// 功能说明：特殊对象的数据表相关
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "SpecialObjData.h"
#include "GameUtil.h"
#include "TLBB_DBC.h"

namespace Combat_Module
{
	namespace Special_Obj_Module
	{
		//class SpecialObjData_T::Descriptor_T;
		BOOL SpecialObjData_T::Descriptor_T::InitFromDB(DBCFile* pDB, INT const nRow, INT& nColumn)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			m_szDesc = pDB->Search_Posistion(nRow, nColumn++)->pString;
			m_nValue = pDB->Search_Posistion(nRow, nColumn++)->iValue;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class SpecialObjData_T
		SpecialObjData_T::SpecialObjData_T() :
			m_bInited(FALSE),
			m_nDataID(INVALID_ID),
			m_szName(NULL),
			m_nLogicID(INVALID_ID),
			m_nTrapUsedFlags(0),
			m_nActiveTimes(0),
			m_nContinuance(0),
			m_nInterval(0),
			m_fTriggerRadius(0.0),
			m_fEffectRadius(0.0),
			m_nEffectedObjNumber(0),
			m_szDescription(NULL)
		{
		}
		BOOL SpecialObjData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 0;
			m_nDataID = pDB->Search_Posistion(nRow, column++)->iValue;
			m_szName = pDB->Search_Posistion(nRow, column++)->pString;
			++column; //Skip Tooltip
			m_nClass = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nLogicID = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nStealthLevel = pDB->Search_Posistion(nRow, column++)->iValue; 
			m_nTrapUsedFlags = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nActiveTimes = pDB->Search_Posistion(nRow, column++)->iValue;
			column = column + 10; //Skip client data area

			m_nContinuance = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nInterval = pDB->Search_Posistion(nRow, column++)->iValue;
			
			m_fTriggerRadius = pDB->Search_Posistion(nRow, column++)->fValue;
			m_fEffectRadius = pDB->Search_Posistion(nRow, column++)->fValue;
			m_nEffectedObjNumber = pDB->Search_Posistion(nRow, column++)->iValue;
			//Skip reserved fields
			column = column + 5;
			
			for(INT i=0;DESCRIPTOR_NUMBER>i;++i)
			{
				if(FALSE==m_aDescriptors[i].InitFromDB(pDB, nRow, column))
				{
					AssertEx(NULL, "[SpecialObjData_T::InitFromDB]:Load Descriptional Data Failed!");
				}
			}
			m_szDescription= pDB->Search_Posistion(nRow, column++)->pString;
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		
	}
}


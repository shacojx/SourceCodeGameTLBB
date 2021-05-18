#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：RelationshipChecker.cpp
// 功能说明：目标和技能使用者的关系检测逻辑
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file RelationshipChecker.cpp
*		\brief the implement of relationship checker between target and me
*/
#include "RelationshipChecker.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		//class RelationshipChecker_T
		BOOL RelationshipChecker_T::IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const 
		{
			__ENTER_FUNCTION
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class AlwaysLegal_T : public RelationshipChecker_T
		BOOL AlwaysLegal_T::IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const 
		{
			__ENTER_FUNCTION
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class FriendlyAndNeutrallyAreLegal_T : public RelationshipChecker_T
		BOOL FriendlyAndNeutrallyAreLegal_T::IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const 
		{
			__ENTER_FUNCTION
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class OnlyFriendlyIsLegal_T : public RelationshipChecker_T
		BOOL OnlyFriendlyIsLegal_T::IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const 
		{
			__ENTER_FUNCTION
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class OnlyNeutrallyIsLegal_T : public RelationshipChecker_T
		BOOL OnlyNeutrallyIsLegal_T::IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const 
		{
			__ENTER_FUNCTION
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class OnlyHostileIsLegal_T : public	RelationshipChecker_T
		BOOL OnlyHostileIsLegal_T::IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const 
		{
			__ENTER_FUNCTION
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class HostileAndNeutrallyAreLegal_T : public RelationshipChecker_T
		BOOL HostileAndNeutrallyAreLegal_T::IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const 
		{
			__ENTER_FUNCTION
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class AlwaysIllegal_T : public RelationshipChecker_T
		BOOL AlwaysIllegal_T::IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const 
		{
			__ENTER_FUNCTION
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	}
}

#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：TargetChecker.cpp
// 功能说明：目标是否合法的检测逻辑
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file TargetChecker.cpp
*		\brief the implement  of the target type checking logic
*/	

#include "TargetChecker.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		//class TargetChecker_T
		BOOL TargetChecker_T::IsLegal(Obj_Character const& rTarget) const 
		{
			__ENTER_FUNCTION
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class AllTypesAreLegal_T : public TargetChecker_T
		BOOL AllTypesAreLegal_T::IsLegal(Obj_Character const& rTarget) const 
		{
			__ENTER_FUNCTION
			if ( !IsCharacterObj( rTarget.GetObjType() ) )
			{
				return FALSE;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class NonPlayerCharactersAreLegal_T : public TargetChecker_T
		BOOL NonPlayerCharactersAreLegal_T::IsLegal(Obj_Character const& rTarget) const 
		{
			__ENTER_FUNCTION
			if ( !IsCharacterObj( rTarget.GetObjType() ) )
				return FALSE;
			Obj::ObjType eObjType = rTarget.GetObjType();
			switch ( eObjType )
			{
			case Obj::OBJ_TYPE_PET:
			case Obj::OBJ_TYPE_MONSTER:
				return TRUE;
				break;
			case Obj::OBJ_TYPE_HUMAN:
			default:
				return FALSE;
				break;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class OnlyPlayerCharacterIsLegal_T : public TargetChecker_T
		BOOL OnlyPlayerCharacterIsLegal_T::IsLegal(Obj_Character const& rTarget) const 
		{
			__ENTER_FUNCTION
			if ( !IsCharacterObj( rTarget.GetObjType() ) )
				return FALSE;
			Obj::ObjType eObjType = rTarget.GetObjType();
			switch ( eObjType )
			{
			case Obj::OBJ_TYPE_HUMAN:
				return TRUE;
				break;
			case Obj::OBJ_TYPE_PET:
			case Obj::OBJ_TYPE_MONSTER:
			default:
				return FALSE;
				break;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class OnlyPetIsLegal_T : public TargetChecker_T
		BOOL OnlyPetIsLegal_T::IsLegal(Obj_Character const& rTarget) const 
		{
			__ENTER_FUNCTION
			if ( !IsCharacterObj( rTarget.GetObjType() ) )
				return FALSE;
			Obj::ObjType eObjType = rTarget.GetObjType();
			switch ( eObjType )
			{
			case Obj::OBJ_TYPE_PET:
				return TRUE;
				break;
			case Obj::OBJ_TYPE_HUMAN:
			case Obj::OBJ_TYPE_MONSTER:
			default:
				return FALSE;
				break;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class NonTypeIsLegal_T : public	TargetChecker_T
		BOOL NonTypeIsLegal_T::IsLegal(Obj_Character const& rTarget) const 
		{
			__ENTER_FUNCTION
			if ( !IsCharacterObj( rTarget.GetObjType() ) )
				return FALSE;
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	}
}


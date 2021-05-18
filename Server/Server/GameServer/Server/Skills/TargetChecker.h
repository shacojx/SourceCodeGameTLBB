///////////////////////////////////////////////////////////////////////////////
// 文件名：TargetChecker.h
// 功能说明：目标是否合法的检测逻辑
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file TargetChecker.h
*		\brief the logic of target type checking
*/	

#ifndef	TARGETCHECKER_H
#define TARGETCHECKER_H
#include "Type.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		class TargetChecker_T
		{
			public:
				typedef enum
				{
					INVALID_CHECKER_ID = INVALID_ID,
					ALL_TYPE_ARE_LEGAL = 0,
					NPCS_ARE_LEGAL = 1,
					ONLY_PC_IS_LEGAL = 2,
					ONLY_PET_IS_LEGAL = 3,
					NON_TYPE_IS_LEGAL = 4,
					NUMBER_OF_CHECKERS
				} TargetTypeCheckerID_T;
				enum
				{
					ID = INVALID_CHECKER_ID,
				};
				TargetChecker_T(){};
				~TargetChecker_T() {};
				virtual BOOL IsLegal(Obj_Character const& rTarget) const ;
			protected:
			private:
		};
		class AllTypesAreLegal_T : public TargetChecker_T
		{
			public:
				enum
				{
					ID = TargetChecker_T::ALL_TYPE_ARE_LEGAL,
				};
				AllTypesAreLegal_T(){};
				~AllTypesAreLegal_T(){};
				virtual BOOL IsLegal(Obj_Character const& rTarget) const ;
			protected:
			private:
		};
		class NonPlayerCharactersAreLegal_T : public TargetChecker_T
		{
			public:
				enum
				{
					ID = TargetChecker_T::NPCS_ARE_LEGAL,
				};
				NonPlayerCharactersAreLegal_T(){};
				~NonPlayerCharactersAreLegal_T(){};
				virtual BOOL IsLegal(Obj_Character const& rTarget) const ;
			protected:
			private:
		};
		class OnlyPlayerCharacterIsLegal_T : public TargetChecker_T
		{
			public:
				enum
				{
					ID = TargetChecker_T::ONLY_PC_IS_LEGAL,
				};
				OnlyPlayerCharacterIsLegal_T() {};
				~OnlyPlayerCharacterIsLegal_T() {};
				virtual BOOL IsLegal(Obj_Character const& rTarget) const ;
			protected:
			private:
		};
		class OnlyPetIsLegal_T : public TargetChecker_T
		{
			public:
				enum
				{
					ID = TargetChecker_T::ONLY_PET_IS_LEGAL,
				};
				OnlyPetIsLegal_T() {};
				~OnlyPetIsLegal_T() {};
				virtual BOOL IsLegal(Obj_Character const& rTarget) const ;
			protected:
			private:
		};
		class NonTypeIsLegal_T : public	TargetChecker_T
		{
			public:
				enum
				{
					ID = TargetChecker_T::NON_TYPE_IS_LEGAL,
				};
				NonTypeIsLegal_T() {};
				~NonTypeIsLegal_T() {};
				virtual BOOL IsLegal(Obj_Character const& rTarget) const ;
			protected:
			private:
		};
	}
}
#endif //TARGETCHECKER_H


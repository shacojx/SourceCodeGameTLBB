///////////////////////////////////////////////////////////////////////////////
// 文件名：RelationshipChecker.h
// 功能说明：目标和技能使用者的关系检测逻辑
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file RelationshipChecker.h
*		\brief the logic of the relationship between target and self
*/	

#ifndef	RELATIONSHIPCHECKER_H
#define RELATIONSHIPCHECKER_H
#include "Type.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		class RelationshipChecker_T
		{
			public:
				typedef enum
				{
					INVALID_RELATIONSHIP_CHECKER_ID = INVALID_ID,
					ALWAYS_LEGAL = 0,
					FRIENDLY_AND_NEUTRALLY_ARE_LEGAL = 1,
					ONLY_FRIENDLY_IS_LEGAL = 2,
					ONLY_NEUTRALLY_IS_LEGAL = 3,
					ONLY_HOSTILE_IS_LEGAL = 4,
					HOSTILE_AND_NEUTRALLY_IS_LEGAL = 5,
					ALWAYS_ILLEGAL = 6,
					NUMBER_OF_CHECKERS
				} RelationshipCheckerID_T;
				enum
				{
					ID = INVALID_RELATIONSHIP_CHECKER_ID,
				};
				RelationshipChecker_T(){};
				~RelationshipChecker_T() {};
				virtual BOOL IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const ;
			protected:
			private:
				
		};
		class AlwaysLegal_T : public RelationshipChecker_T
		{
			public:
				enum
				{
					ID = RelationshipChecker_T::ALWAYS_LEGAL,
				};
				AlwaysLegal_T(){};
				~AlwaysLegal_T(){};
				virtual BOOL IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const ;
			protected:
			private:
		};
		class FriendlyAndNeutrallyAreLegal_T : public RelationshipChecker_T
		{
			public:
				enum
				{
					ID = RelationshipChecker_T::FRIENDLY_AND_NEUTRALLY_ARE_LEGAL,
				};
				FriendlyAndNeutrallyAreLegal_T(){};
				~FriendlyAndNeutrallyAreLegal_T(){};
				virtual BOOL IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const ;
			protected:
			private:
		};
		class OnlyFriendlyIsLegal_T : public RelationshipChecker_T
		{
			public:
				enum
				{
					ID = RelationshipChecker_T::ONLY_FRIENDLY_IS_LEGAL,
				};
				OnlyFriendlyIsLegal_T() {};
				~OnlyFriendlyIsLegal_T() {};
				virtual BOOL IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const ;
			protected:
			private:
		};
		class OnlyNeutrallyIsLegal_T : public RelationshipChecker_T
		{
			public:
				enum
				{
					ID = RelationshipChecker_T::ONLY_NEUTRALLY_IS_LEGAL,
				};
				OnlyNeutrallyIsLegal_T() {};
				~OnlyNeutrallyIsLegal_T() {};
				virtual BOOL IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const ;
			protected:
			private:
		};
		class OnlyHostileIsLegal_T : public	RelationshipChecker_T
		{
			public:
				enum
				{
					ID = RelationshipChecker_T::ONLY_HOSTILE_IS_LEGAL,
				};
				OnlyHostileIsLegal_T() {};
				~OnlyHostileIsLegal_T() {};
				virtual BOOL IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const ;
			protected:
			private:
		};
		class HostileAndNeutrallyAreLegal_T : public RelationshipChecker_T
		{
			public:
				enum
				{
					ID = RelationshipChecker_T::HOSTILE_AND_NEUTRALLY_IS_LEGAL,
				};
				HostileAndNeutrallyAreLegal_T() {};
				~HostileAndNeutrallyAreLegal_T() {};
				virtual BOOL IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const ;
			protected:
			private:
		};
		class AlwaysIllegal_T : public RelationshipChecker_T
		{
			public:
				enum
				{
					ID = RelationshipChecker_T::ALWAYS_ILLEGAL,
				};
				AlwaysIllegal_T() {};
				~AlwaysIllegal_T() {};
				virtual BOOL IsLegal(Obj_Character const& rTarget, Obj_Character const& rMe) const ;
			protected:
			private:
		};
	}
}
#endif //RELATIONSHIPCHECKER_H

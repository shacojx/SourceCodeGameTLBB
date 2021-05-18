#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：LogicManager.cpp
// 功能说明：通用的逻辑管理器
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "LogicManager.h"

namespace MenPai_Module
{
	//g_MenPaiLogicList
	MenPaiLogicManager_T g_MenPaiLogicList;
};
namespace Combat_Module
{
	namespace Impact_Module
	{
		//g_ImpactLogicList
		ImpactLogicManager_T g_ImpactLogicList;
	};
	namespace Special_Obj_Module
	{
		//g_SpecialObjLogicList;
		SpecialObjLogicManager_T g_SpecialObjLogicList;
	};
	namespace Skill_Module
	{
		//g_SkillLogicList
		SkillLogicManager_T g_SkillLogicList;

		/*
		//g_RelationshipCheckerList
		RelationshipCheckerManager_T g_RelationshipCheckerList;

		//g_TargetCheckerList
		TargetTypeCheckerManager_T g_TargetCheckerList;
		*/
	};
};

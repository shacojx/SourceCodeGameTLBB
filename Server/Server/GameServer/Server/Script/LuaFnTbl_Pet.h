//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Pet.h
// 模块	:	Script
// 功能	 :  属性相关函数
// 修改历史:
//-----------------------------------------------------------------------------

#ifndef __LUAFNTBL_PET_H__
#define __LUAFNTBL_PET_H__

#include "LuaFnMacro.h"
#include "Obj_Human.h"

#include "GCDetailAttrib_Pet.h"

extern INT CalcPetDomesticationMoney( INT nLevel, INT nHP, INT nHPMax, INT nHappiness );

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnPetStudySkill
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					完家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				skillId
	*					技能ID
	*     功能说明 :
	*				宠物学习技能
	*/
	INT LuaFnPetStudySkill(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t humanId = Lua_ValueToNumber(L,2);
		UINT petGUID_H = Lua_ValueToNumber(L,3);
		UINT petGUID_L = Lua_ValueToNumber(L,4);
		INT skillId = Lua_ValueToNumber(L,5);

		BEGINSCENEDEFINE("LuaFnPetStudySkill")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			PET_GUID_t guidPet;
			guidPet.m_uHighSection = petGUID_H;
			guidPet.m_uLowSection = petGUID_L;
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				INT nSkillIndex = -1;
				BOOL bResult = pHuman->Pet_Skill_Modify_Study(guidPet, skillId, &nSkillIndex);
				if ( bResult )
				{
					GCDetailAttrib_Pet msgDetailPet;
					msgDetailPet.SetGUID(guidPet);

					_OWN_SKILL skillOwn;
					skillOwn.m_nSkillID = pPetItem->GetSkill(nSkillIndex).m_nSkillID;
					msgDetailPet.SetSkill(nSkillIndex, &skillOwn);

					pHuman->GetPlayer()->SendPacket(&msgDetailPet);

					return 0;
				}
			}
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnPetStudySkill
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					完家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				skillId
	*					技能ID
	*     功能说明 :
	*				宠物学习门派技能
	*/
	INT LuaFnPetStudySkill_MenPai(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t humanId = Lua_ValueToNumber(L,2);
		UINT petGUID_H = Lua_ValueToNumber(L,3);
		UINT petGUID_L = Lua_ValueToNumber(L,4);
		INT skillId = Lua_ValueToNumber(L,5);

		BEGINSCENEDEFINE("LuaFnPetStudySkill_MenPai")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			PET_GUID_t guidPet;
			guidPet.m_uHighSection = petGUID_H;
			guidPet.m_uLowSection = petGUID_L;
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				BOOL bResult = pHuman->Pet_Skill_Modify_MenPai(guidPet, skillId);
				if ( bResult )
				{
					GCDetailAttrib_Pet msgDetailPet;
					msgDetailPet.SetGUID(guidPet);

					_OWN_SKILL skillOwn;
					skillOwn.m_nSkillID = pPetItem->GetSkill(PET_SKILL_INDEX_MENPAI).m_nSkillID;
					msgDetailPet.SetSkill(PET_SKILL_INDEX_MENPAI, &skillOwn);

					pHuman->GetPlayer()->SendPacket(&msgDetailPet);

					return 0;
				}
			}
		}
		ENDSCENEDEFINE
		

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnCalcPetDomesticationMoney
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					宠物ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				得到训养宠物所需费用
	*/
	INT LuaFnCalcPetDomesticationMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t humanId = Lua_ValueToNumber(L,2);
		UINT petGUID_H = Lua_ValueToNumber(L,3);
		UINT petGUID_L = Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnCalcPetDomesticationMoney")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			PET_GUID_t guidPet;
			guidPet.m_uHighSection = petGUID_H;
			guidPet.m_uLowSection = petGUID_L;
			if(!guidPet.IsNull())
			{
				ItemContainer *pPetContainer = pHuman->GetPetContain();
				Item *pPetItem = pHuman->GetPetItem(guidPet);
				if(pPetItem != NULL)
				{
					INT nMoney = CalcPetDomesticationMoney(pPetItem->GetLevel(), pHuman->GetPetHP(guidPet), pHuman->GetPetMaxHP(guidPet), pHuman->GetPetHappiness(guidPet));
					Lua_PushNumber(L,nMoney);
					return 1;
				}
			}
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnPetDomestication
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					宠物ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				训养宠物
	*/
	INT LuaFnPetDomestication(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t humanId = Lua_ValueToNumber(L,2);
		UINT petGUID_H = Lua_ValueToNumber(L,3);
		UINT petGUID_L = Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnPetDomestication")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			PET_GUID_t guidPet;
			guidPet.m_uHighSection = petGUID_H;
			guidPet.m_uLowSection = petGUID_L;
			if(!guidPet.IsNull())
			{
				ItemContainer *pPetContainer = pHuman->GetPetContain();
				Item *pPetItem = pHuman->GetPetItem(guidPet);
				if(pPetItem != NULL)
				{
					INT nMoney = CalcPetDomesticationMoney(pPetItem->GetLevel(), pHuman->GetPetHP(guidPet), pHuman->GetPetMaxHP(guidPet), pHuman->GetPetHappiness(guidPet));
					if((UINT)nMoney < pHuman->GetMoney())
					{
						INT petIndex = pHuman->GetPetIndexByGUID(guidPet);
						pHuman->SetMoney(pHuman->GetMoney() - (UINT)nMoney);
						
						MONEY_LOG_PARAM	MoneyLogParam;
						MoneyLogParam.CharGUID	=	pHuman->GetGUID();
						MoneyLogParam.OPType	=	MONEY_PET_COST;	
						MoneyLogParam.Count		=	nMoney;
						MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
						MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
						MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
						SaveMoneyLog(&MoneyLogParam);

						pHuman->SetPetHP(guidPet, pHuman->GetPetMaxHP(guidPet));
						pHuman->SetPetHappiness(guidPet, 100);

						GCDetailAttrib_Pet msgDetailPet;
						msgDetailPet.SetGUID(guidPet);
						msgDetailPet.SetHP(pHuman->GetPetHP(guidPet));
						msgDetailPet.SetHappiness(pHuman->GetPetHappiness(guidPet));

						pHuman->GetPlayer()->SendPacket(&msgDetailPet);

						Lua_PushNumber(L,1);
						return 1;
					}
				}
			}
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnGetPetHP
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				设置宠物HP
	*/
	INT LuaFnGetPetHP(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		UINT petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));

		BEGINHUMANDEFINE("LuaFnGetPetHP")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			INT nHP = pHuman->GetPetHP(guidPet);

			Lua_PushNumber(L,nHP);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnGetPetMaxHP
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				设置宠物HP最大值
	*/
	INT LuaFnGetPetMaxHP(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		UINT petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));

		BEGINHUMANDEFINE("LuaFnGetPetMaxHP")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			INT nHP = pHuman->GetPetMaxHP(guidPet);

			Lua_PushNumber(L,nHP);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnGetPetLife
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				设置宠物寿命
	*/
	INT LuaFnGetPetLife(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		UINT petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));

		BEGINHUMANDEFINE("LuaFnGetPetLife")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			INT nLife = pHuman->GetPetLife(guidPet);

			Lua_PushNumber(L,nLife);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnGetPetHappiness
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				设置宠物快乐度
	*/
	INT LuaFnGetPetHappiness(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		UINT petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));

		BEGINHUMANDEFINE("LuaFnGetPetHappiness")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			INT nHappiness = pHuman->GetPetHappiness(guidPet);

			Lua_PushNumber(L,nHappiness);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnSetPetHP
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				hp
	*					要设置的HP值	
	*     功能说明 :
	*				设置宠物HP
	*/
	INT LuaFnSetPetHP(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		UINT petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		INT hp			= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetPetHP")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			pHuman->SetPetHP(guidPet,hp);

			GCDetailAttrib_Pet msgDetailPet;
			msgDetailPet.SetGUID(guidPet);
			msgDetailPet.SetHP(pHuman->GetPetHP(guidPet));
			pHuman->GetPlayer()->SendPacket(&msgDetailPet);

			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnSetPetMaxHP
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				maxHP
	*					要设置的HP最大值	
	*     功能说明 :
	*				设置宠物HP最大值
	*/
	INT LuaFnSetPetMaxHP(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		UINT petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		INT maxHP		= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetPetMaxHP")

			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnSetPetLife
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				life
	*					要设置的寿命值	
	*     功能说明 :
	*				设置宠物寿命值	
	*/
	INT LuaFnSetPetLife(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		UINT petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		INT life		= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetPetLife")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			pHuman->SetPetLife(guidPet, life);

			GCDetailAttrib_Pet msgDetailPet;
			msgDetailPet.SetGUID(guidPet);
			msgDetailPet.SetLife(pHuman->GetPetLife(guidPet));
			pHuman->GetPlayer()->SendPacket(&msgDetailPet);

			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnSetPetHappiness
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				happiness
	*					要设置的快乐度
	*     功能说明 :
	*				设置宠物快乐度
	*/
	INT LuaFnSetPetHappiness(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		UINT petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		INT happiness	= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetPetHappiness")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			pHuman->SetPetHappiness(guidPet, happiness);

			GCDetailAttrib_Pet msgDetailPet;
			msgDetailPet.SetGUID(guidPet);
			msgDetailPet.SetHappiness(pHuman->GetPetHappiness(guidPet));
			pHuman->GetPlayer()->SendPacket(&msgDetailPet);

			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-11	16:31
	 *	函数名称：	LuaFnPetCanUseFood
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				petGUID_H
	 *					宠物GUID高位
	 *				petGUID_L
	 *					宠物GUID低位
	 *				nIndex
	 *					物品背包位置
	 *
	 *	功能说明：	判断物品是否适合宠物食用
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnPetCanUseFood(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId			= Lua_ValueToNumber(L, 1);
		ObjID_t selfId			= Lua_ValueToNumber(L, 2);
		UINT petGUID_H		= (UINT)(Lua_ValueToNumber(L, 3));
		UINT petGUID_L		= (UINT)(Lua_ValueToNumber(L, 4));
		INT nIndex			= Lua_ValueToNumber(L, 5);

		BEGINHUMANDEFINE("LuaFnPetCanUseFood")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			INT nPetLevel;
			INT nFoodLevel;

			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if( pPetItem == NULL )
			{
				Lua_PushNumber(L, FALSE);
				return 1;
			}
			else
			{
				nPetLevel = pPetItem->GetLevel();
			}

			Item *pFood = HumanItemLogic::GetItem(pHuman, nIndex);
			if( pFood == NULL )
			{
				Lua_PushNumber(L, FALSE);
				return 1;
			}
			else
			{
				nFoodLevel = pFood->GetItemLevel();
			}

			if( nFoodLevel > nPetLevel )
			{ // 宠物级别不够
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			PET_ATTR *pPetAttr = g_PetAttrTbl.GetAttr( pPetItem->GetDataID() );
			if( pPetAttr == NULL )
			{
				Assert(FALSE);
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			INT nRet;
			switch( pFood->GetItemType() )
			{ // 判断食物种类是否匹配，最不挑食的宠物食物类型是 1111
			case 2: // PET_FOOD_TYPE_MEAT
				nRet = (pPetAttr->m_FoodType / 1000) & 1;
				break;
			case 3: // PET_FOOD_TYPE_GRASS
				nRet = (pPetAttr->m_FoodType / 100) & 1;
				break;
			case 4: // PET_FOOD_TYPE_WORM
				nRet = (pPetAttr->m_FoodType / 10) & 1;
				break;
			case 5: // PET_FOOD_TYPE_PADDY
				nRet = pPetAttr->m_FoodType & 1;
				break;
			default:
				AssertEx(FALSE, "Invalid Pet Food.");
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			Lua_PushNumber(L, nRet);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-04-19
	*     函数名称 : LuaFnPetReturnToChild
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				宠物还童Lua接口
	*/
	INT LuaFnPetReturnToChild(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		UINT petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		
		BEGINHUMANDEFINE("LuaFnPetReturnToChild")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;

			Lua_PushNumber(L, pHuman->ReturnToChild(guidPet));
			return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-04-19
	*     函数名称 : LuaFnPetReturnToChild
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				宠物还童Lua接口
	*/
	INT LuaFnPetCanReturnToChild(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		UINT petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		
		BEGINHUMANDEFINE("LuaFnPetCanReturnToChild")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;

			Lua_PushNumber(L, pHuman->CanReturnToChild(guidPet));
			return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-05-12
	*     函数名称 : LuaFnGetPetGUID
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petIndex
	*					宠物索引
	*     功能说明 :
	*				取得宠物的GUID
	*/
	INT LuaFnGetPetGUID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		INT petIndex	= Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetPetGUID")

			PET_GUID_t guidPet = pHuman->GetPetGUIDByIndex(petIndex);

			Lua_PushNumber(L,guidPet.m_uHighSection);
			Lua_PushNumber(L,guidPet.m_uLowSection);
			return 2;

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
}

#endif // __LUAFNTBL_PET_H__

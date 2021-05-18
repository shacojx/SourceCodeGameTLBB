#include "StdAfx.h"
#include "GMInterface_Script_Tooltip.h"
#include "TDException.h"
#include "LuaPlus.h"
#include "TDActionSystem.h"
#include "..\action\GMActionSystem_Item.h"
#include "..\action\GMActionSystem_XinFa.h"
#include "..\action\GMActionSystem_Skill.h"
#include "..\object\item\Obj_Item_Medicine.h"
#include "..\DBC\GMDataBase.h"
#include "..\Procedure\GameProcedure.h"
#include "..\DataPool\GMDataPool.h"
//
//脚本函数沙箱，这些函数应该由脚本调用
//
namespace SCRIPT_SANDBOX
{

	SuperTooltips SuperTooltips::s_SupperTooltip;
	LuaPlus::LuaObject* SuperTooltips::s_pMetaTable = NULL;
	SuperTooltips::SuperTooltips()
	{
		m_pActionItem = NULL;
	}

	SuperTooltips::~SuperTooltips()
	{
	}

	//由游戏逻辑设置激活中的ActionItem
	VOID SuperTooltips::SetActionItem(CActionItem* pActionitem)
	{
		m_pActionItem = pActionitem;
		if(m_pActionItem)
		{
			m_nActionId = m_pActionItem->GetID();
		}
		
	}
		
	INT SuperTooltips::GetTitle(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetName());
		return 1;
	}


	INT SuperTooltips::GetIcon(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetIconName());
		return 1;
	}

	INT SuperTooltips::GetAttributeValue(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		//检查参数
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA: SuperTooltips GetAttributeValue Wrong Param1");
		}

		//通过ActionItem查询逻辑属性
		state->PushString(m_pActionItem->GetAttributeValue(args[2].GetString()).c_str());

		return 1;
	}

	INT	SuperTooltips::GetItemDesc(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		//得到详细说明.
		state->PushString(m_pActionItem->GetDesc());
		return 1;
	}
	INT SuperTooltips::GetItemTransferParam( LuaPlus::LuaState* state)
	{
		/*
		定点传送需要参数
		1、场景id 

		2、物品使用次数  
		3、x坐标
		4.z坐标
		
		*/
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if( pItem && (pItem->GetItemExtraInfoItem()) )
		{

			int sceneid=GETLOW(pItem->GetItemExtraInfoItem()->GetItemParamValue( 0, IPV_INT ));
			int nUseCount=GETHIGH(pItem->GetItemExtraInfoItem()->GetItemParamValue( 0, IPV_INT ));
			state->PushInteger( sceneid );
			state->PushInteger( nUseCount );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 4, IPV_INT ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 8, IPV_INT ) );
	
			return 4;
		}
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		
		return 4;

	}
	/*
	物品索引
	*/
	INT SuperTooltips::GetItemIndex( LuaPlus::LuaState* state)
	{
		
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if( pItem && (pItem->GetItemExtraInfoItem()) )
		{
			state->PushInteger( pItem->GetItemExtraInfoItem()->m_ItemIndex );
			return 1;
		}
		state->PushInteger( -1 );

		return 1;

	}

	INT	SuperTooltips::GetItemLevel(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemLevel());
		return 1;
	}

	//查询Tooltip的物品耐久
	INT	SuperTooltips::GetItemDur(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			state->PushInteger(-1);
			return 2;
		}

		state->PushInteger(m_pActionItem->GetItemDur());
		state->PushInteger(m_pActionItem->GetItemMaxDur());
		return 2;
	}

	//查询Tooltip的物品的修理次数
	INT	SuperTooltips::GetItemRepairCount(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemRepairCount());
	
		return 1;
	}

	//
	// 得到最大修理次数
	INT	SuperTooltips::GetEquipMaxRepair(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetItemMaxRepair());
		return 1;
	
	}

	//查询Tooltip的物品的绑定信息
	INT	SuperTooltips::GetItemBindInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemBindInfo());
	
		return 1;
	}

	//查询Tooltip的物品的卖给npc的价格
	INT	SuperTooltips::GetItemPrice(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		//state->PushInteger(m_pActionItem->GetItemPrice());

		INT iPrice = CDataPool::GetMe()->ItemBox_GetCurNpcBuyPrice(m_pActionItem->GetPosIndex());
		state->PushInteger(iPrice);

		return 1;
	}

	//查询Tooltip的物品的制造者
	INT	SuperTooltips::GetItemManufacturer(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetManufacturer());
	
		return 1;
	}

	//查询Tooltip的物品的白色属性
	INT	SuperTooltips::GetItemBaseWhiteAttrInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetBaseWhiteAttrInfo());
		return 1;
	}

	
	//查询Tooltip的物品的绿色属性
	INT	SuperTooltips::GetItemGreenAttrInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(pActionItem_Item->GetGreenAttrInfo());
		return 1;
	}

	//查询Tooltip的物品的蓝色属性
	INT	SuperTooltips::GetItemExtBlueAttrInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetExtBlueAttrInfo());
		return 1;
	}

	
	//查询Tooltip的物品的类型
	INT	SuperTooltips::GetItemType(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetType_String());
		return 1;
	}


	//查询Tooltip的装备的品质(白, 蓝, 绿)
	INT	SuperTooltips::GetItemEquipQuantity(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
	
		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetEquipQuantity());
		return 1;

	}

	//查询Tooltip的装备的类型属性
	INT	SuperTooltips::GetItemClassType(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetItemType());
		return 1;
	}

	//查询Tooltip的装备的宝石信息
	//
	//
	//   
	// 传出参数5个
	//   0[int] - 宝石个数
	//   1[str] - 宝石1的图标字符串 "" - 表示无图标
	//	 2[str] - 宝石2的图标字符串 "" - 表示无图标
	//	 3[str] - 宝石2的图标字符串 "" - 表示无图标
	//	 4[str] - 宝石附加属性的描述
	INT	SuperTooltips::GetGemInEquipInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			// 得到宝石的个数
			STRING strInfo = _T("");
			INT iCount = pActionItem_Item->GetGemInEquipCount();

			state->PushInteger(iCount);

			for(int i = 0; i < 3; i++)
			{
				strInfo = _T("");
				pActionItem_Item->GetGemIcon(i, strInfo);
				state->PushString(strInfo.c_str());
			}

			strInfo = _T("");
			pActionItem_Item->GetGemInEquipDesc(strInfo);
			state->PushString(strInfo.c_str());
		}
		else//
		{

			state->PushInteger(0);
			state->PushString("");
			state->PushString("");
			state->PushString("");
			state->PushString("");

		}

		return 5;
	}


	

	//----------------------------------------------------------------------------------------------------------------------
	// 心法函数
	//
	
	// 得到心法等级
	// 传出参数1个
	// 0[int] - 当前学会的心法等级.
	//INT	SuperTooltips::GetXinfaLevel(LuaPlus::LuaState* state)
	//{
	//	// 不利用保存的指针。要重新得到指针。2006－4－15
	//	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	//	if(!m_pActionItem)
	//	{
	//		state->PushInteger(0);
	//		return 1;
	//	}

	//	CActionItem_XinFa* pXinfaAction = (CActionItem_XinFa*)m_pActionItem;
	//	if(pXinfaAction)
	//	{
	//		state->PushInteger(pXinfaAction->GetXinfaLevel());
	//		return 1;
	//	}

	//	state->PushInteger(0);
	//	return 1;
	//	
	//}

	INT SuperTooltips::GetItem_IsProtected(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(false);
			return 1;
		}

		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if(!pItem)
		{
			state->PushInteger(false);
			return 1;
		}

		state->PushBoolean(pItem->GetByProtect()==TRUE);
		return 1;
	}

	//----------------------------------------------------------------------------------------------------------------------
	// 技能函数
	//

	// 得到技能等级
	// 传出参数1个
	// 0[int] - 当前学会的技能等级.
	INT	SuperTooltips::GetSkillLevel(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			state->PushInteger(pAction->GetSkillLevel());
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}


	//----------------------------------------------------------------------------------------------------------------------
	// 得到是否学会技能
	//

	// 得到是否学会技能
	// 传出参数1个
	// 0[int] - 是否学会 1 - 学会, 0 - 没学会
	INT	SuperTooltips::GetIsLearnedSkill(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			state->PushInteger(pAction->GetIsLearnedSkill());
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}

	//----------------------------------------------------------------------------------------------------------------------
	// 得到技能描述
	//

	// 得到技能描述
	// 传出参数1个
	// 0[str] - 得到技能描述
	INT	SuperTooltips::GetSkillDesc(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;

		if(pAction)
		{
			state->PushString(pAction->GetDesc());
			return 1;
		}

		state->PushString(_T(""));
		return 1;
	}

		
    //----------------------------------------------------------------------------------------------------------------------
	//
	// 宝石信息
	//

	// 得到宝石等级
	// 传出参数1个
	// 0[int] - 宝石等级
	INT	SuperTooltips::GetGemLevel(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushInteger(pActionItem_Item->GetGemLevel());
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}

	// 得到宝石属性
	// 传出参数1个
	// 0[str] - 宝石属性
	INT	SuperTooltips::GetGemArribInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushString(pActionItem_Item->GetGemArribInfo());
			return 1;
		}

		state->PushString("");
		return 1;
	}


	//----------------------------------------------------------------------------------------------------------------------
	//
	// 药品或宝石信息
	//

	// 得到药品或宝石品质
	// 传出参数1个
	// 0[int] - 药品或宝石品质
	INT	SuperTooltips::GetItemQuality(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushInteger(pActionItem_Item->GetItemQuality());
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}


	// 得到技能对应的心法等级
	//
	// 传出参数1个
	// 0[int] - 心法等级
	//INT	SuperTooltips::GetSkillXinfaLevel(LuaPlus::LuaState* state)
	//{
	//	// 不利用保存的指针。要重新得到指针。2006－4－15
	//	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	//	CActionItem_Skill* pAction = NULL;

	//	pAction = (CActionItem_Skill*)m_pActionItem;
	//	if(pAction)
	//	{
	//		state->PushInteger(pAction->GetSkillXinfaLevel());
	//		return 1;
	//	}

	//	state->PushInteger(0);
	//	return 1;

	//}

	// 得到物品在表中的类型
	// 传出参数1个
	// 0[int] - 表中的类型。
	INT	SuperTooltips::GetItemTableType(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushInteger(pActionItem_Item->GetMedicineItemTableType());
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}


	// 得到配方需要的技能名字。
	// 传出参数1个
	// 0[int] - 表中的类型。
	INT	SuperTooltips::GetPrescrSkillName(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			//CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)pActionItem_Item->GetItemImpl();
			//if(pItem)
			//{
			//	INT nIndex = pItem->m_theBaseDef->nScriptID - 700000;

			//	//打开宝石数据表
			//	DBC_DEFINEHANDLE(s_pItem_Compound, DBC_LIFEABILITY_ITEMCOMPOSE);
			//	_DBC_LIFEABILITY_ITEMCOMPOSE* pData = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_pItem_Compound->Search_Index_EQU(nIndex);

			//	if(pData)
			//	{
			//		state->PushString(pData->szName);
			//		return 1;
			//	}
			//	//const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
			//	//if(pPrescr)
			//	//{
			//	//	STRING strTemp = "#r#cFFCF00已学会";
			//	//	m_strExtraDesc += strTemp;
			//	//}//

			//}//

			LPCTSTR pNeedSkillName = pActionItem_Item->GetPrescrNeedSkillName();
			if(pNeedSkillName)
			{
				state->PushString(pNeedSkillName);
				return 1;
			}
		}//


		state->PushString("");
		return 1;
	}


	
	// 得到配方需要的技能等级。
	// 传出参数1个
	// 0[int] - 表中的等级。
	INT	SuperTooltips::GetPrescrNeedSkillLevel(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			int iNeedLevel = pActionItem_Item->GetPrescrNeedSkillLevel();
			state->PushInteger(iNeedLevel);
			return 1;
			
		}//


		state->PushInteger(-1);
		return 1;
	}


	// 得到配方需要技能的当前等级。
	// 传出参数1个
	// 0[int] - 配方需要的当前等级。
	INT	SuperTooltips::GetPrescrNeedSkillCurLevel(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			int iNeedCurLevel = pActionItem_Item->GetPrescrNeedSkillCurLevel();
				
			state->PushInteger(iNeedCurLevel);
			return 1;
			
		}//

		state->PushInteger(0);
		return 1;
	}

	// 向服务器请求item的详细信息
	// 
	// 
	INT	SuperTooltips::SendAskItemInfoMsg(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(m_pActionItem->GetType() != AOT_ITEM) return 0;

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			pActionItem_Item->SendAskItemInfoMsg();
			
		}//

		return 0;
	}


		
	// 得到蓝色装备是否鉴定
	// 
	// 
	INT	SuperTooltips::IsBlueEquipIdentify(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;
		//if(AOT_EQUIP == pActionItem_Item->GetType())
		if(pActionItem_Item)
		{
			INT nIdentify = pActionItem_Item->GetEquipIdentify();
			state->PushInteger(nIdentify);
		}
		else
		{
			
			state->PushInteger(0);
		}

		return 1;
	}


	
	//
	// npc商店是否打开。
	// 
	INT	SuperTooltips::IsNpcShopOpen(LuaPlus::LuaState* state)
	{

		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		return 1;
	}

	INT SuperTooltips::IsPresent(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		state->PushInteger(m_pActionItem ? 1 : 0);
		return 1;
	}

	//得到描述信息2006－4－28
	INT SuperTooltips::GetTypeDesc(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;
		if(pActionItem_Item)
		{
			state->PushString(pActionItem_Item->GetTypeDesc());
		}

		return 1;
	}

	//得到物品是否在背包中2006-5-10
	INT SuperTooltips::IsItemInPacket(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushString(0);
			return 1;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// 使用卷轴的背包索引位置
		CObject_Item* pItem = (CObject_Item*)pActionItem_Item->GetItemImpl();
		if(NULL == pItem)
		{
			state->PushInteger(0);
			return 1;
		}

		if(tObject_Item::IO_MYSELF_PACKET == pItem->GetTypeOwner())
		{
			state->PushInteger(1);
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}

	//
	// 得到是否显示价格 2006-5-18
	//
	INT SuperTooltips::IsShowPrice(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}


		state->PushInteger(m_pActionItem->IsViewToolTopsPrice());
		return 1;
	}

	//
	// 得到显示价格 2006-5-18
	//
	INT SuperTooltips::GetShowPrice(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}


		state->PushInteger(m_pActionItem->GetViewToolTopsPrice());
		return 1;
	}

	//
	// 得到物品档次等级描述 2006-5-18
	//
	INT SuperTooltips::GetItemLevelDesc(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。2006－4－15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemLevelDesc());
		return 1;
	}

	INT SuperTooltips::GetAnchorValue( LuaPlus::LuaState* state)
	{
		/*
		1、Param1 - SHORT(16) - Key，这位标识操作集合，也是Client修正Tooltips的依据，可扩展。
		当key==10时，修正Tooltips；否则显示原始信息。
		……
		2、Param2 - BYTE(8)   - 最大使用次数
		3、Param3 - BYTE(8)   - 剩余使用次数
		4、Param4 - SHORT(16) - 场景编号
		5、Param5 - SHORT(16) - X坐标
		6、Param6 - SHORT(16) - Y坐标
		*/
		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if( pItem && (pItem->GetItemExtraInfoItem()) )
		{

			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 0, IPV_SHORT ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 2, IPV_CHAR ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 3, IPV_CHAR ) );
			int nSceneID = pItem->GetItemExtraInfoItem()->GetItemParamValue( 4, IPV_SHORT );
			DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);
			const _DBC_SCENE_DEFINE* pSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_Index_EQU(nSceneID);
			if( pSceneDef )
				state->PushString( pSceneDef->szName );
			else
				state->PushString( "没有这个场景" );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 6, IPV_SHORT ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 8, IPV_SHORT ) );
			return 6;
		}
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		state->PushString( "" );
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		return 6;

	}

	INT SuperTooltips::IsItemSuit(  LuaPlus::LuaState* state )
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger( 0 );
			return 1;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// 使用卷轴的背包索引位置
		CObject_Item_Equip* pItem = (CObject_Item_Equip*)pActionItem_Item->GetItemImpl();
		if( pItem)
		{

			int nID = -1;
			switch( pItem->GetEquipQuantity() )
			{
			case CObject_Item_Equip::GREEN_EQUIP:
			case CObject_Item_Equip::PURPLE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsGreen->nSetID;
				break;
			case CObject_Item_Equip::BLUE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsBlue->nSetID;
				break;
			case CObject_Item_Equip::WHITE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsWhite->nSetID;
				break;
			default:
				break;
			}

			if( nID == -1 )
			{
				state->PushInteger( 1 );
			}
			else
			{
				state->PushInteger( 0 );
			}
		}
		else
			state->PushInteger( 0 );
		return 1;
	}

	// 得到套装的数量
	INT SuperTooltips::GetItemSuitNumber( LuaPlus::LuaState* state )
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger( 0 );
			return 1;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// 使用卷轴的背包索引位置
		CObject_Item_Equip* pItem = (CObject_Item_Equip*)pActionItem_Item->GetItemImpl();
		if( pItem )
		{

			int nID = -1;
			int nNumber = 0;
			switch( pItem->GetEquipQuantity() )
			{
			case CObject_Item_Equip::GREEN_EQUIP:
			case CObject_Item_Equip::PURPLE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsGreen->nSetID;
				nNumber = pItem->GetEquipBaseDefine().pDefineAsGreen->nSetNum;
				break;
			case CObject_Item_Equip::BLUE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsBlue->nSetID;
				nNumber = pItem->GetEquipBaseDefine().pDefineAsBlue->nSetNum;
				break;
			case CObject_Item_Equip::WHITE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsWhite->nSetID;
				nNumber = pItem->GetEquipBaseDefine().pDefineAsWhite->nSetNum;
				break;
			default:
				break;
			}

			if( nID == -1 ) // 如果没有套装就为0
			{
				nNumber = 0;
			}
			state->PushInteger( nNumber );
		}
		else
			state->PushInteger( 0 );

		return 1;
	}

	// 判断自己是不是有第几件套装
	INT SuperTooltips::IsItemExist( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger( 0 );
			state->PushString( "" );
			return 2;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// 使用卷轴的背包索引位置
		CObject_Item_Equip* pItem = (CObject_Item_Equip*)pActionItem_Item->GetItemImpl();
		if( pItem)
		{

			int nID = -1;
			int nNumber = 0;
			int nType = pItem->GetEquipQuantity();
			switch( nType )
			{
			case CObject_Item_Equip::PURPLE_EQUIP:
			case CObject_Item_Equip::GREEN_EQUIP:
				{
					nID = pItem->GetEquipBaseDefine().pDefineAsGreen->nSetID;
					nNumber = pItem->GetEquipBaseDefine().pDefineAsGreen->nSetNum;
					DBC_DEFINEHANDLE(s_pItem_GreenEquip, DBC_ITEM_EQUIP_GREEN);
					int nIndex = 0;
					for(INT i=0; i<(INT)s_pItem_GreenEquip->GetRecordsNum(); i++)
					{
						const _DBC_ITEM_EQUIP_GREEN* pGreenEquip = (const _DBC_ITEM_EQUIP_GREEN*)s_pItem_GreenEquip->Search_LineNum_EQU((UINT)i);

						if( pGreenEquip && pGreenEquip->nSetID == nID )
						{
							if( args[ 2 ].GetInteger() == nIndex ) // 如果是需要找的那个
							{
								// 判断身上有没有这个装备
								for( int iEquit = 0; iEquit < HEQUIP_NUMBER; iEquit ++ )
								{
									// 如果id相等，就说明已经有了
									tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP(iEquit) );
									if( pItem )
									{
										if( pItem->GetIdTable() == pGreenEquip->nID )
										{

											state->PushInteger( 1 );
											state->PushString( pGreenEquip->szName );
											return 2;
										}
									}
								}
								state->PushInteger( 0 );
								state->PushString( pGreenEquip->szName );
								return 2;
							}
							nIndex ++;

						}
					}
					break;
				}
			case CObject_Item_Equip::BLUE_EQUIP:
				{
					nID = pItem->GetEquipBaseDefine().pDefineAsBlue->nSetID;
					nNumber = pItem->GetEquipBaseDefine().pDefineAsBlue->nSetNum;
					DBC_DEFINEHANDLE(s_pItem_BlueEquip, DBC_ITEM_EQUIP_BLUE);
					int nIndex = 0;
					for(INT i=0; i<(INT)s_pItem_BlueEquip->GetRecordsNum(); i++)
					{
						const _DBC_ITEM_EQUIP_BLUE* pGreenEquip = (const _DBC_ITEM_EQUIP_BLUE*)s_pItem_BlueEquip->Search_LineNum_EQU((UINT)i);

						if( pGreenEquip && pGreenEquip->nSetID == nID )
						{
							if( args[ 2 ].GetInteger() == nIndex ) // 如果是需要找的那个
							{
								// 判断身上有没有这个装备
								for( int iEquit = 0; iEquit < HEQUIP_NUMBER; iEquit ++ )
								{
									// 如果id相等，就说明已经有了
									tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP(iEquit) );
									if( pItem )
									{
										if( pItem->GetIdTable() == pGreenEquip->nID )
										{
											// 有
											state->PushInteger( 1 );
											state->PushString( pGreenEquip->szName );
											return 2;
										}
									}
								}
								state->PushInteger( 0 );
								state->PushString( pGreenEquip->szName );
								return 2;
							}
							nIndex ++;
						}
					}
					break;
				}
				break;
			case CObject_Item_Equip::WHITE_EQUIP:
				{
					nID = pItem->GetEquipBaseDefine().pDefineAsWhite->nSetID;
					nNumber = pItem->GetEquipBaseDefine().pDefineAsWhite->nSetNum;
					DBC_DEFINEHANDLE(s_pItem_GreenEquip, DBC_ITEM_EQUIP_WHITE);
					int nIndex = 0;
					for(INT i=0; i<(INT)s_pItem_GreenEquip->GetRecordsNum(); i++)
					{
						const _DBC_ITEM_EQUIP_WHITE* pGreenEquip = (const _DBC_ITEM_EQUIP_WHITE*)s_pItem_GreenEquip->Search_LineNum_EQU((UINT)i);

						if( pGreenEquip && pGreenEquip->nSetID == nID )
						{
							if( args[ 2 ].GetInteger() == nIndex ) // 如果是需要找的那个
							{
								// 判断身上有没有这个装备
								for( int iEquit = 0; iEquit < HEQUIP_NUMBER; iEquit ++ )
								{
									// 如果id相等，就说明已经有了
									tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP(iEquit) );
									if( pItem )
									{
										if( pItem->GetIdTable() == pGreenEquip->nID )
										{

											state->PushInteger( 1 );
											state->PushString( pGreenEquip->szName );
											return 2;
										}
									}
								}
								state->PushInteger( 0 );
								state->PushString( pGreenEquip->szName );
								return 2;
							}
							nIndex ++;

						}
					}
					break;
				}
			default:
				break;
			}
		}
		state->PushInteger( 0 );
		state->PushString( "" );
		return 2;
	}

	// 得到套装的属性
	INT SuperTooltips::GetSuitAtt( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushString( "" );
			return 1;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// 使用卷轴的背包索引位置
		CObject_Item_Equip* pItem = (CObject_Item_Equip*)pActionItem_Item->GetItemImpl();
		if( pItem)
		{
			state->PushString( pItem->GetSuitAttrInfo() );
		}
		else
			state->PushString( "" );
		return 1;
	}

	INT SuperTooltips::GetTwoMoneyValue(LuaPlus::LuaState* state)
	{
		/*
		银票物品号 TICKET_ITEM_INDEX
		1、Param1 - SHORT(16) - 银票当前的价值
		2、Param2 - SHORT(16) - 银票最大的价值

		跑商货物物品号范围 
		*/

		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if( pItem && (pItem->GetItemExtraInfoItem()) && pItem->GetIdTable() == TICKET_ITEM_INDEX)	//银票
		{

			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( TICKET_MERCHANDISE_ITEM_PARAM_BUY_PRICE_START, TICKET_MERCHANDISE_ITEM_PARAM_BUY_PRICE_TYPE ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( TICKET_MERCHANDISE_ITEM_PARAM_SCENE_START, TICKET_MERCHANDISE_ITEM_PARAM_SCENE_TYPE ) );
			return 2;
		}

		if( pItem && (pItem->GetItemExtraInfoItem()) && ITEM_IDX_IS_TICKET_MERCHANDISE(pItem->GetIdTable()))		//跑商货物
		{

			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( TICKET_MERCHANDISE_ITEM_PARAM_BUY_PRICE_START, TICKET_MERCHANDISE_ITEM_PARAM_BUY_PRICE_TYPE ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( TICKET_MERCHANDISE_ITEM_PARAM_SALE_PRICE_START, TICKET_MERCHANDISE_ITEM_PARAM_SALE_PRICE_TYPE ) );
			return 2;
		}
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		return 2;
	}

	INT SuperTooltips::GetSysColour(LuaPlus::LuaState* state)
	{
		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if(pItem)
		{
			state->PushString(pItem->GetSysColour());
			return 1;
		}
		state->PushString("");
		return 1;
	}

}
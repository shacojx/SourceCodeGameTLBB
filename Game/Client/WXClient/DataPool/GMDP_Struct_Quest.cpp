/****************************************\
*										*
* 			   数据池数据结构			*
*					-任务				*
*										*
*			  修改记录
\****************************************/

#include "stdafx.h"

#include "GMDP_Struct_Quest.h"
#include "../Object/Item/Obj_Item.h"
#include "GMUIDataPool.h"
#include "LuaPlus.h"
#include "FileDataMgr.h"
#include "../Script/GMScriptSystem.h"
#include "..\DBC\GMDataBase.h"
#include "tddbc_struct.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Action\GMActionSystem.h"

_MISSION_INFO* misInfo = NULL;
MisInfo m_misInfo;

void parseMissionName(CHAR** pDest,CHAR* pSrc,INT j)
{
//	strcpy(*pDest,tokens[j]);
	CHAR* pCur = strchr(pSrc,'"');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	CHAR* pEnd = pCur + 1;
	pEnd = strrchr(pCur,'"');
	
	if(pEnd-pCur-1 <= 0) return;
	strncat(*pDest,pCur+1,pEnd-pCur-1);
	//填充
	misInfo->m_misName = *pDest;
}
void parseMissionBriefTarget(CHAR** pDest,CHAR* pSrc,INT j)
{
//	strcpy(*pDest,tokens[j]);
	CHAR* pCur = strchr(pSrc,'"');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	CHAR* pEnd = pCur + 1;
	pEnd = strrchr(pCur,'"');
	
	if(pEnd-pCur-1 <= 0) return;
	strncat(*pDest,pCur+1,pEnd-pCur-1);
	//填充
	misInfo->m_misDesc = *pDest;
}
void parseMissionState(CHAR** pDest,CHAR* pSrc,INT j)
{
//	strcpy(*pDest,tokens[j]);
	CHAR* pCur = strchr(pSrc,'"');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	CHAR* pEnd = pCur + 1;
	pEnd = strrchr(pCur,'"');
	
	if(pEnd-pCur-1 <= 0) return;
	strncat(*pDest,pCur+1,pEnd-pCur-1);
	//填充
	misInfo->m_misState = *pDest;
}
void parseMissionBillName(CHAR** pDest,CHAR* pSrc,INT j)
{
//	strcpy(*pDest,tokens[j]);
	CHAR* pCur = strchr(pSrc,'"');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	CHAR* pEnd = pCur + 1;
	pEnd = strrchr(pCur,'"');
	
	if(pEnd-pCur-1 <= 0) return;
	strncat(*pDest,pCur+1,pEnd-pCur-1);
	//填充
	misInfo->m_misBillName = *pDest;
}

void parseMissionKind(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	pCur++;

	sprintf(*pDest,"%d",atoi(pCur));

	misInfo->m_nKind = atoi(pCur);

}
//没写好，以后用到再改。
void parseMissionCustom(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	strcpy(*pDest,tokens[j]);
	CHAR* pNow = *pDest + strlen(tokens[j]);
	
	CHAR szStr[128];
	static CHAR szTemp[MAX_PATH];
	int count = 0;
	int digit = 1;
	int str_pos = 0;

	CHAR* customName = 0;
	int customCount = 0;
	while(pCur++)
	{
		if(*pCur == '{')
		{
			digit = 1;
			count++;
		}
		else if(*pCur == '}')
		{
			digit = 1;
			count--;
			if(count == 0)
				break;
		}
		
		if(digit == 1 && *pCur == '"')
		{
			digit = 2;
			CHAR* pEnd = pCur + 1;
			pEnd = strchr(pCur+1,'"');
	
//			strncat(szStr,pCur+1,pEnd-pCur-1);
			memset(szStr,0,sizeof(szStr));
			strncpy(szStr,pCur+1,pEnd-pCur-1);
//			_snprintf(szTemp,"%s",szStr);
			//填充
			customName = szStr;
//			customName = szTemp;

			*pNow++=*pCur;
		}
/*
		if(digit == 1 && isdigit(*pCur))
		{	
			digit = 2;
			customName = pCur;

			*pNow++=*pCur;			
		}
*/		else if(digit == 3 && isdigit(*pCur))
		{
			digit = 4;
			customCount = atoi(pCur);

			_MISSION_INFO::QuestCustom newCustom;
			newCustom.szCustomString = customName;
			newCustom.nCount = customCount;

			misInfo->m_vecQuestCustom.push_back(newCustom);

			*pNow++=*pCur;
			
		}
		else if(digit && (*pCur == ','))
		{
			digit = 3;
			*pNow++=',';			
		}
	}
}

//g_DemandKill={{id=66,num=1},{id=56,num=1}}
void parseDemandKill(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	strcpy(*pDest,tokens[j]);
	CHAR* pNow = *pDest + strlen(tokens[j]);
	
	int count = 0;
	int digit = 1;

	int npcId = 0;
	int npcCount = 0;
	while(pCur++)
	{
		if(*pCur == '{')
		{
			digit = 1;
			count++;
		}
		else if(*pCur == '}')
		{
			digit = 1;
			count--;
			if(count == 0)
				break;
		}

		if(digit == 1 && isdigit(*pCur))
		{	
			digit = 2;
			npcId = atoi(pCur);

			*pNow++=*pCur;			
		}
		else if(digit == 3 && isdigit(*pCur))
		{
			digit = 4;
			npcCount = atoi(pCur);

			DBC_DEFINEHANDLE(s_pCreatureDBC, DBC_CREATURE_ATT);
	
			//从DBC中查找定义
			const _DBC_CREATURE_ATT* pMissionNpc = (const _DBC_CREATURE_ATT*)s_pCreatureDBC->Search_Index_EQU(npcId);
			_MISSION_INFO::QuestDemandKill newDemandKill;
			newDemandKill.pDemandKill = new SMissionDemandKill;
			newDemandKill.pDemandKill->m_uNPCID = npcId;
			newDemandKill.pDemandKill->m_yCount = npcCount;
			if(!pMissionNpc)
				newDemandKill.szNPCName = "???";
			else
				newDemandKill.szNPCName = (STRING)pMissionNpc->pName;
			misInfo->m_vecQuestDemandKill.push_back(newDemandKill);

			*pNow++=*pCur;
			
		}
		else if(digit && (*pCur == ','))
		{
			digit = 3;
			*pNow++=',';			
		}
	}
}
//g_DemandItem={{id=20004008,num=1},{id=20004009,num=1}}
void parseDemandItem(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	strcpy(*pDest,tokens[j]);
	CHAR* pNow = *pDest + strlen(tokens[j]);
	
	int count = 0;
	int digit = 1;

	int itemId = 0;
	int itemCount = 0;
	while(pCur++)
	{
		if(*pCur == '{')
		{
			digit = 1;
			count++;
		}
		else if(*pCur == '}')
		{
			digit = 1;
			count--;
			if(count == 0)
				break;
		}

		if(digit == 1 && isdigit(*pCur))
		{	
			digit = 2;
			itemId = atoi(pCur);

			*pNow++=*pCur;			
		}
		else if(digit == 3 && isdigit(*pCur))
		{
			digit = 4;
			itemCount = atoi(pCur);

			tObject_Item* pItem = CObject_Item::NewItem(itemId);
			if(!pItem)
			{
				TDThrow("非法的奖励物品ID:%d", itemId);
			}

			_MISSION_INFO::QuestDemandItem newDemandItem;
			newDemandItem.pDemandItem = new SMissionBonusItem;
			newDemandItem.pDemandItem->m_uItemID = itemId;
			newDemandItem.pDemandItem->m_yCount = itemCount;
			pItem->SetTypeOwner(tObject_Item::IO_QUESTVIRTUALITEM);
			newDemandItem.pItemImpl = pItem;
			misInfo->m_vecQuestDemandItem.push_back(newDemandItem);

			*pNow++=*pCur;
			
		}
		else if(digit && (*pCur == ','))
		{
			digit = 3;
			*pNow++=',';			
		}
	}
}
//g_ItemBonus={{id=30002001,num=5},{id=10412001,num=5}}
void parseItemBonus(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	strcpy(*pDest,tokens[j]);
	CHAR* pNow = *pDest + strlen(tokens[j]);

	int count = 0;
	int digit = 1;

	int itemId = 0;
	int itemCount = 0;

	while(pCur++)
	{
		if(*pCur == '{')
		{
			digit = 1;
			count++;
		}
		else if(*pCur == '}')
		{	
			count--;
			if(count == 0)
				break;
		}

		if(digit == 1 && isdigit(*pCur))
		{	
			itemId = atoi(pCur);

			digit = 2;
			*pNow++=*pCur;			
		}
		else if(digit == 3 && isdigit(*pCur))
		{
			digit = 4;
			itemCount = atoi(pCur);
			
			SMissionBonus* rewardItem = new SMissionBonus;
			rewardItem->m_ItemBonus.m_uItemID = itemId;
			rewardItem->m_ItemBonus.m_yCount = itemCount;
			rewardItem->m_nType = MISSION_BONUS_TYPE_ITEM;

			_MISSION_INFO::QuestRewardItem newRewardItem;

			tObject_Item* pItem = CObject_Item::NewItem(rewardItem->m_ItemBonus.m_uItemID);
			if(!pItem)
			{
				TDThrow("非法的奖励物品ID:%d", rewardItem->m_ItemBonus.m_uItemID);
			}
//			CActionSystem::GetMe()->UpdateAction_FromItem(pItem);
			newRewardItem.pItemData = rewardItem;
			newRewardItem.bSelected = FALSE;
			pItem->SetTypeOwner(tObject_Item::IO_QUESTVIRTUALITEM);
			newRewardItem.pItemImpl = pItem;
			misInfo->m_vecQuestRewardItem.push_back(newRewardItem);
			
			*pNow++=*pCur;
		}
		else if(digit && (*pCur == ','))
		{
			*pNow++=',';			
			digit = 3;
		}
	}
}
void parseRandItemBonus(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	strcpy(*pDest,tokens[j]);
	CHAR* pNow = *pDest + strlen(tokens[j]);

	int count = 0;
	int digit = 1;

	int itemId = 0;
	int itemCount = 0;

	while(pCur++)
	{
		if(*pCur == '{')
		{
			digit = 1;
			count++;
		}
		else if(*pCur == '}')
		{	
			count--;
			if(count == 0)
				break;
		}

		if(digit == 1 && isdigit(*pCur))
		{	
			itemId = atoi(pCur);

			digit = 2;
			*pNow++=*pCur;			
		}
		else if(digit == 3 && isdigit(*pCur))
		{
			digit = 4;
			itemCount = atoi(pCur);
			
			SMissionBonus* rewardItem = new SMissionBonus;
			rewardItem->m_ItemBonus.m_uItemID = itemId;
			rewardItem->m_ItemBonus.m_yCount = itemCount;
			rewardItem->m_nType = MISSION_BONUS_TYPE_ITEM_RAND;
			
			

			_MISSION_INFO::QuestRewardItem newRewardItem;

//只显示为“问号”型物品
//			tObject_Item* pItem = CObject_Item::NewItem(rewardItem->m_ItemBonus.m_uItemID);
//			CActionSystem::GetMe()->UpdateAction_FromItem(pItem);
			tObject_Item* pItem = CObject_Item::NewItem(20005020);//只显示为“问号”型物品

			newRewardItem.pItemData = rewardItem;
			newRewardItem.bSelected = FALSE;
			pItem->SetTypeOwner(tObject_Item::IO_QUESTVIRTUALITEM);
			newRewardItem.pItemImpl = pItem;

			misInfo->m_vecQuestRewardItem.push_back(newRewardItem);
			
			*pNow++=*pCur;
		}
		else if(digit && (*pCur == ','))
		{
			*pNow++=',';			
			digit = 3;
		}
	}
}

void parseRadioItemBonus(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	strcpy(*pDest,tokens[j]);
	CHAR* pNow = *pDest + strlen(tokens[j]);

	int count = 0;
	int digit = 1;

	int itemId = 0;
	int itemCount = 0;

	while(pCur++)
	{
		if(*pCur == '{')
		{
			digit = 1;
			count++;
		}
		else if(*pCur == '}')
		{
			digit = 1;
			count--;
			if(count == 0)
				break;
		}

		if(digit == 1 && isdigit(*pCur))
		{	
			itemId = atoi(pCur);

			digit = 2;
			*pNow++=*pCur;			
		}
		else if(digit == 3 && isdigit(*pCur))
		{
			digit = 4;
			itemCount = atoi(pCur);
			
			//以后考虑内存池
			SMissionBonus* rewardItem = new SMissionBonus;
			rewardItem->m_ItemBonus.m_uItemID = itemId;
			rewardItem->m_ItemBonus.m_yCount = itemCount;
			rewardItem->m_nType =  MISSION_BONUS_TYPE_ITEM_RADIO;

			tObject_Item* pItem = CObject_Item::NewItem(itemId);
			if(!pItem)
			{
				TDThrow("非法的奖励物品ID:%d", itemId);
			}

			_MISSION_INFO::QuestRewardItem newRewardItem;
//			CActionSystem::GetMe()->UpdateAction_FromItem(pItem);
			newRewardItem.pItemData = rewardItem;
			pItem->SetTypeOwner(tObject_Item::IO_QUESTVIRTUALITEM);
			newRewardItem.pItemImpl = pItem;
			newRewardItem.bSelected = FALSE;	
			misInfo->m_vecQuestRewardItem.push_back(newRewardItem);

			*pNow++=*pCur;			
		}
		else if(digit && (*pCur == ','))
		{
			*pNow++=',';
			digit = 3;
		}

	}
}

//g_MoneyBonus=905
void parseMoneyBonus(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	while(!isdigit(*pCur))
		pCur++;

	sprintf(*pDest,"%d",atoi(pCur));

	SMissionBonus* rewardItem = new SMissionBonus;
	rewardItem->m_uMoney = atoi(pCur);
	rewardItem->m_nType =  MISSION_BONUS_TYPE_MONEY;

	_MISSION_INFO::QuestRewardItem newRewardItem;

	newRewardItem.pItemData = rewardItem;
	newRewardItem.bSelected = FALSE;

	misInfo->m_vecQuestRewardItem.push_back(newRewardItem);
}

void parseMissionOKFail(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	pCur++;

	sprintf(*pDest,"%d",atoi(pCur));
	
	misInfo->m_nOKFail = 1;
}
void parseMissionLimitTime(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	while(!isdigit(*pCur))
		pCur++;

	sprintf(*pDest,"%d",atoi(pCur));
	
	misInfo->m_nLimitTime = 1;
	INT xx = atoi(pCur);
}

void parseMissionRound(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	pCur++;

	sprintf(*pDest,"%d",atoi(pCur));
	
	misInfo->m_nRound = atoi(pCur);
}

void parseMissionBill(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	pCur++;

	sprintf(*pDest,"%d",atoi(pCur));
	
	misInfo->m_nBill = 1;
}

void parseExpBonus(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	while(!isdigit(*pCur))
		pCur++;

	sprintf(*pDest,"%d",atoi(pCur));

	SMissionBonus* rewardItem = new SMissionBonus;
	rewardItem->m_uExp = atoi(pCur);
	rewardItem->m_nType =  MISSION_BONUS_TYPE_EXP;

	_MISSION_INFO::QuestRewardItem newRewardItem;

	newRewardItem.pItemData = rewardItem;
	newRewardItem.bSelected = FALSE;

	misInfo->m_vecQuestRewardItem.push_back(newRewardItem);
}

void parseMissionLevel(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	while(!isdigit(*pCur))
		pCur++;

	sprintf(*pDest,"%d",atoi(pCur));
	
	misInfo->m_nLevel = atoi(pCur);
	
}

void parseMissionElite(CHAR** pDest,CHAR* pSrc,INT j)
{
	CHAR* pCur = strchr(pSrc,'=');
	if(!pCur)
		TDThrow("策划写错了这个脚本:%s", pSrc);
	while(!isdigit(*pCur))
		pCur++;

	sprintf(*pDest,"%d",atoi(pCur));
	
	misInfo->m_nElite = atoi(pCur);

}
void RemoveMissionInfo(INT nIndex)
{
	const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
	INT scriptId = OwnMission->m_idScript;

	misInfo = &(GetMissionInfo(scriptId));

	for(UINT i=0;i<misInfo->m_vecQuestDemandItem.size();i++)
	{
		if(misInfo->m_vecQuestDemandItem[i].pDemandItem)
			delete misInfo->m_vecQuestDemandItem[i].pDemandItem;
		if(misInfo->m_vecQuestDemandItem[i].pItemImpl)
			CObject_Item::DestroyItem((CObject_Item*)(misInfo->m_vecQuestDemandItem[i].pItemImpl));	
	}
	for(UINT i=0;i<misInfo->m_vecQuestRewardItem.size();i++)
	{
		if(misInfo->m_vecQuestRewardItem[i].pItemData)
			delete misInfo->m_vecQuestRewardItem[i].pItemData;
	}
	misInfo->m_misName = "";
	misInfo->m_misDesc = "";

	MisInfo::iterator it = m_misInfo.find(scriptId);
	if (it != m_misInfo.end())
	{
		m_misInfo.erase(it);
	}
}

_MISSION_INFO& GetMissionInfo( UINT scriptId )
{
	MisInfo::iterator it = m_misInfo.find(scriptId);
	if (it == m_misInfo.end())
	{
		//添加新项
		_MISSION_INFO misInfo;
		misInfo.m_bFill = FALSE;
		misInfo.m_misBillName= "";
		misInfo.m_misDesc="";
		misInfo.m_misName="";
		misInfo.m_misState="";
		misInfo.m_nBill=-1;
		misInfo.m_nElite=-1;
		misInfo.m_nKind=-1;
		misInfo.m_nLevel=-1;
		misInfo.m_nLimitTime=-1;
		misInfo.m_nOKFail=-1;
		misInfo.m_nRound=-1;

		m_misInfo.insert(MisInfo::value_type(scriptId,misInfo));
	}

	return m_misInfo[scriptId];
}
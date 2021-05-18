#include "stdafx.h"

#include "HumanDB.h"
#include "ServerManager.h"
#include "SMUManager.h"
#include "ShareMemManager.h"
#include "DB_Struct.h"

HumanDB::HumanDB( )
{

__ENTER_FUNCTION
	
	BOOL bEnableShareMem = g_pServerManager->GetCurrentServerInfo()->m_EnableShareMem;

	if(bEnableShareMem)
	{
		HumanSMU* pSMU =	g_HumanSMUPool.NewObj();
		Assert(pSMU);
		m_HumanSMU		=	pSMU;
		m_AttrRegSM		=	new HUMAN_DB_ATTR_REG;

		INT Stat  =	 m_HumanSMU->GetUseStatus(SM_S_WRITE);
		if(Stat!=SM_USE_FREE)
			AssertEx(FALSE,"ShareMemory Ã»ÓÐ´æÅÌÍê±Ï!");
	}
	else
	{
		m_HumanSMU		=	0;
		m_AttrRegSM		=	0;
	}
	
	m_AttrReg		=	new HUMAN_DB_ATTR_REG;
	m_AttrFlag		=	new HUMAN_DB_ATTR_FLAG;
	m_dbHuman		=	new _HUMAN_DB_LOAD ;
	m_dbEquip		=	new _EQUIP_DB_LOAD ;
	m_dbBag			=	new _BAG_DB_LOAD ;
	m_dbBank		=	new _BANK_DB_LOAD ;
	m_dbSkill		=	new _SKILL_DB_LOAD ;
	m_dbCooldown	=	new _COOLDOWN_DB_LOAD_FOR_HUMAN;
	m_dbXinFa		=	new _XINFA_DB_LOAD ;
	m_dbImpact		=	new _IMPACT_DB_LOAD ;
	m_dbAbility		=	new _ABILITY_DB_LOAD ;
	m_dbMission		=	new _MISSION_DB_LOAD ;
	m_dbSetting		=	new _SETTING_DB_LOAD ;
	m_dbPetList		=	new _PET_DB_LIST_LOAD ;
	m_dbRelation	=	new _RELATION_DB_LOAD ;
	m_dbPrivateInfo	=	new _PRIVATE_INFO_DB_LOAD;
	
	_RegisterDBAttributes();
	
	if(bEnableShareMem)
	{
		_RegisterSMAttributes();
	}

__LEAVE_FUNCTION
}

HumanDB::~HumanDB( )
{
__ENTER_FUNCTION
	
	SAFE_DELETE(m_AttrRegSM);
	SAFE_DELETE(m_AttrReg);
	SAFE_DELETE(m_AttrFlag);
	SAFE_DELETE(m_dbHuman) ;
	SAFE_DELETE(m_dbEquip) ;
	SAFE_DELETE(m_dbBag) ;
	SAFE_DELETE(m_dbBank) ;
	SAFE_DELETE(m_dbSkill) ;
	SAFE_DELETE(m_dbCooldown) ;
	SAFE_DELETE(m_dbXinFa) ;
	SAFE_DELETE(m_dbImpact) ;
	SAFE_DELETE(m_dbAbility) ;
	SAFE_DELETE(m_dbMission) ;
	SAFE_DELETE(m_dbSetting) ;
	SAFE_DELETE(m_dbPetList) ;
	SAFE_DELETE(m_dbRelation) ;
	SAFE_DELETE(m_dbPrivateInfo);


__LEAVE_FUNCTION
}

VOID HumanDB::CleanUp( )
{
__ENTER_FUNCTION

	BOOL bEnableShareMem = g_pServerManager->GetCurrentServerInfo()->m_EnableShareMem;

	if(!bEnableShareMem)
	{
		m_AttrReg->CleanUp();
		m_AttrFlag->CleanUp();
		m_dbHuman->CleanUp( ) ;
		m_dbEquip->CleanUp( ) ;
		m_dbBag->CleanUp( ) ;
		m_dbBank->CleanUp( ) ;
		m_dbSkill->CleanUp( ) ;
		m_dbCooldown->CleanUp();
		m_dbXinFa->CleanUp( ) ;
		m_dbImpact->CleanUp( ) ;
		m_dbAbility->CleanUp( ) ;
		m_dbMission->CleanUp( ) ;
		m_dbSetting->CleanUp( ) ;
		m_dbPetList->CleanUp( ) ;
		m_dbRelation->CleanUp( ) ;
		m_dbPrivateInfo->CleanUp();
	}

__LEAVE_FUNCTION
}


VOID HumanDB::ValidateShareMem(BOOL bForceAll,BOOL& bUpdateAttr)
{
	 INT iAttr;

	__ENTER_FUNCTION
	
	if(m_HumanSMU)
	{
		Lock();
		
		for(iAttr = CATTR_DB_HP;iAttr< CATTR_DB_NUMBER;iAttr++)
		{
			if((IsNoDirectAttr((CHAR_ATTR_DB)iAttr)&& (bUpdateAttr != FALSE)) ||bForceAll)
			{
				memcpy(GetSMEnumAttr((CHAR_ATTR_DB)iAttr),
					GetDBEnumAttr((CHAR_ATTR_DB)iAttr),
					GetDBEnumAttrSize((CHAR_ATTR_DB)iAttr));
				SetDBEnumAttrDirty((CHAR_ATTR_DB)iAttr,FALSE);

			}
			else if(GetDBEnumAttrDirty((CHAR_ATTR_DB)iAttr)||bForceAll)
			{
				memcpy(GetSMEnumAttr((CHAR_ATTR_DB)iAttr),
						GetDBEnumAttr((CHAR_ATTR_DB)iAttr),
						GetDBEnumAttrSize((CHAR_ATTR_DB)iAttr));
				SetDBEnumAttrDirty((CHAR_ATTR_DB)iAttr,FALSE);
			}
		}

		UnLock();

		bUpdateAttr = FALSE;
	}



	__LEAVE_FUNCTION
}
BOOL	HumanDB::IsNoDirectAttr(CHAR_ATTR_DB attrType)
{
	__ENTER_FUNCTION
	
	switch(attrType) 
	{
	case CATTR_DB_COOLDOWN:
	case CATTR_DB_IMPACT:
	case CATTR_DB_RELATION:
	case CATTR_DB_PRIVATEINFO:
	case CATTR_DB_AT_POSITION:
		return TRUE;
		break;
	
	default:
		return FALSE;
	}

	return FALSE;

	__LEAVE_FUNCTION
	
	return FALSE;
}

VOID HumanDB::ValidatePlayerID(PlayerID_t PID)
{
	if(m_HumanSMU)
	{
		m_HumanSMU->SetPlayerID(SM_S_WRITE,PID);
	}
}
VOID HumanDB::RandSet( )
{
__ENTER_FUNCTION

	INT iRand = 10000+rand() ;
	m_dbHuman->m_HP = iRand ;
	sprintf( m_dbHuman->m_Name, "²âÊÔ%d", iRand ) ;
	m_dbHuman->m_Name[sizeof(m_dbHuman->m_Name)-1]='\0';


__LEAVE_FUNCTION
}

BOOL HumanDB::InitHumanDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HumanDB::InitEquipDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HumanDB::InitBagDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HumanDB::InitBankDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HumanDB::InitSkillDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HumanDB::InitCooldownDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}


VOID	HumanDB::_RegisterDBAttributes()
{

#define REG_DB_ATTR(type,var)						_RegDBAttr(type,&(var),sizeof(var));
#define REG_DB_ATTR_SIZE(type,varPtr,size)			_RegDBAttr(type,varPtr,size);



__ENTER_FUNCTION

	REG_DB_ATTR(CATTR_DB_HP,				m_dbHuman->m_HP);
	REG_DB_ATTR(CATTR_DB_MP,				m_dbHuman->m_MP);
	REG_DB_ATTR(CATTR_DB_STRIKEPOINT,		m_dbHuman->m_StrikePoint);
	REG_DB_ATTR(CATTR_DB_RMB,				m_dbHuman->m_RMBMoney);
	REG_DB_ATTR(CATTR_DB_BANK_RMB,			m_dbHuman->m_BankRMB);
	REG_DB_ATTR(CATTR_DB_VIGOR_REGENE_RATE,	m_dbHuman->m_VigorRegeneRate);
	REG_DB_ATTR(CATTR_DB_ENERGY,			m_dbHuman->m_DoubleExpTime_Num);
	REG_DB_ATTR(CATTR_DB_MAX_ENERGY,		m_dbHuman->m_GmRight);
	REG_DB_ATTR(CATTR_DB_ENERGY_REGENE_RATE,m_dbHuman->m_EnergyRegeneRate);
	REG_DB_ATTR(CATTR_DB_RAGE,				m_dbHuman->m_Rage);
	REG_DB_ATTR(CATTR_DB_LEVEL,				m_dbHuman->m_Level);
	REG_DB_ATTR(CATTR_DB_PKVALUE,			m_dbHuman->m_nPKValue);
	REG_DB_ATTR(CATTR_DB_CURRENTPET,		m_dbHuman->m_guidCurrentPet);
	REG_DB_ATTR(CATTR_DB_EXP,				m_dbHuman->m_Exp);
	REG_DB_ATTR(CATTR_DB_AT_POSITION,		m_dbHuman->m_Position);
	REG_DB_ATTR(CATTR_DB_BK_POSITION,		m_dbHuman->m_BakPosition);
	REG_DB_ATTR(CATTR_DB_AT_SCENE,			m_dbHuman->m_StartScene);
	REG_DB_ATTR(CATTR_DB_BK_SCENE,			m_dbHuman->m_BakScene);
	REG_DB_ATTR(CATTR_DB_STR,				m_dbHuman->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR]);
	REG_DB_ATTR(CATTR_DB_SPR,				m_dbHuman->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_SPR]);
	REG_DB_ATTR(CATTR_DB_CON,				m_dbHuman->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON]);
	REG_DB_ATTR(CATTR_DB_INT,				m_dbHuman->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT]);
	REG_DB_ATTR(CATTR_DB_DEX,				m_dbHuman->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX]);
	REG_DB_ATTR(CATTR_DB_REMAINPOINT,		m_dbHuman->m_Level1Points);
	REG_DB_ATTR(CATTR_DB_CAMP,				m_dbHuman->m_CampData);
	REG_DB_ATTR(CATTR_DB_MENPAI,			m_dbHuman->m_MenPai);
	REG_DB_ATTR(CATTR_DB_GUILDID,			m_dbHuman->m_GuildID);
	REG_DB_ATTR(CATTR_DB_TEAMID,			m_dbHuman->m_TeamID);
	REG_DB_ATTR(CATTR_DB_GUID,				m_dbHuman->m_GUID);
	REG_DB_ATTR(CATTR_DB_PORTRAITID,		m_dbHuman->m_PortraitID);
	REG_DB_ATTR_SIZE(CATTR_DB_NAME,			m_dbHuman->m_Name,sizeof(CHAR)*MAX_CHARACTER_NAME);
	REG_DB_ATTR(CATTR_DB_SEX,				m_dbHuman->m_Sex);
	REG_DB_ATTR(CATTR_DB_CREATETIME,		m_dbHuman->m_CreateDate);
	REG_DB_ATTR_SIZE(CATTR_DB_TITLE,		m_dbHuman->m_Title,sizeof(CHAR)*MAX_CHARACTER_TITLE);
	REG_DB_ATTR_SIZE(CATTR_DB_PASSWORD,		m_dbHuman->m_Passwd,sizeof(CHAR)*MAX_PWD);
	REG_DB_ATTR(CATTR_DB_PWDDELTIME,		m_dbHuman->m_uPwdDelTime);
	REG_DB_ATTR(CATTR_DB_HAIR_COLOR,		m_dbHuman->m_HairColor);
	REG_DB_ATTR(CATTR_DB_FACE_COLOR,		m_dbHuman->m_FaceColor);
	REG_DB_ATTR(CATTR_DB_HAIR_MODEL,		m_dbHuman->m_HairModel);
	REG_DB_ATTR(CATTR_DB_FACE_MODEL,		m_dbHuman->m_FaceModel);
	REG_DB_ATTR(CATTR_DB_ONLINETIME,		m_dbHuman->m_OnlineTime);
	REG_DB_ATTR(CATTR_DB_LASTLOGINTIME,		m_dbHuman->m_LastLoginTime);
	REG_DB_ATTR(CATTR_DB_LASTLOGOUTTIME,	m_dbHuman->m_LastLogoutTime);
	REG_DB_ATTR(CATTR_DB_DBVERSION,			m_dbHuman->m_DBVersion);
	REG_DB_ATTR(CATTR_DB_MONEY,				m_dbHuman->m_Money);
	REG_DB_ATTR(CATTR_DB_BANK_MONEY,		m_dbBank->m_Money);
	REG_DB_ATTR(CATTR_DB_BANK_OFFSIZE,		m_dbBank->m_CurEndIndex);
	int iShopIndex = 0;
	for(int iDBShop=CATTR_DB_PLAYER_SHOP_BEGIN;iDBShop<=CATTR_DB_PLAYER_SHOP_END;iDBShop++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBShop,m_dbHuman->m_ShopGuid[iShopIndex]);
		iShopIndex++;
	}

	int iFavoriteShopIndex = 0;
	for(int iDBFavoriteShop=CATTR_DB_PLAYER_SHOP_FAVORITE_BEGIN;iDBFavoriteShop<=CATTR_DB_PLAYER_SHOP_FAVORITE_END;iDBFavoriteShop++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBFavoriteShop,m_dbHuman->m_ShopGuid[iFavoriteShopIndex]);
		iFavoriteShopIndex++;
	}

	int iBagIndex = 0;
	for(int iDBBag=CATTR_DB_BAG_BEGIN;iDBBag<=CATTR_DB_BAG_BEGIN+MAX_BAG_SIZE;iDBBag++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBBag,m_dbBag->m_pItem[iBagIndex]);
		iBagIndex++;
	}
	int iEquipIndex = 0;
	for(int iDBEquip = CATTR_DB_EQUIP_BEGIN;iDBEquip<=CATTR_DB_EQUIP_END;iDBEquip++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBEquip,m_dbEquip->m_pItem[iEquipIndex]);
		iEquipIndex++;
	}
	int iBankIndex = 0;
	for(int iDBBank = CATTR_DB_BANK_BEGIN;iDBBank<=CATTR_DB_BANK_END;iDBBank++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBBank,m_dbBank->m_pItem[iBankIndex]);
		iBankIndex++;
	}
	
	REG_DB_ATTR(CATTR_DB_SKILL,*m_dbSkill);
	REG_DB_ATTR(CATTR_DB_COOLDOWN,*m_dbCooldown);	
	REG_DB_ATTR(CATTR_DB_XINFA,*m_dbXinFa);
	REG_DB_ATTR(CATTR_DB_IMPACT,*m_dbImpact);
	REG_DB_ATTR(CATTR_DB_ABILITY,*m_dbAbility);	
	REG_DB_ATTR(CATTR_DB_MISSION,*m_dbMission);
	REG_DB_ATTR(CATTR_DB_SETTING,*m_dbSetting);
	REG_DB_ATTR(CATTR_DB_PET,*m_dbPetList);
	REG_DB_ATTR(CATTR_DB_RELATION,*m_dbRelation);
	REG_DB_ATTR(CATTR_DB_PRIVATEINFO,*m_dbPrivateInfo);
	REG_DB_ATTR(CATTR_DB_TITLEINFO,m_dbHuman->m_Titles);
	REG_DB_ATTR_SIZE(CATTR_DB_RESERVE,m_dbHuman->m_Reserve,sizeof(INT)*MAX_RESERVE);

__LEAVE_FUNCTION

}

VOID	HumanDB::_RegisterSMAttributes()
{

#define REG_SM_ATTR(type,var)					_RegSMAttr(type,&(var),sizeof(var));
#define REG_SM_ATTR_SIZE(type,varPtr,size)		_RegSMAttr(type,varPtr,size);

	__ENTER_FUNCTION
		
	Lock();
	REG_SM_ATTR(CATTR_DB_HP,				m_HumanSMU->m_HumanSM.m_HP);
	REG_SM_ATTR(CATTR_DB_MP,				m_HumanSMU->m_HumanSM.m_MP);
	REG_SM_ATTR(CATTR_DB_STRIKEPOINT,		m_HumanSMU->m_HumanSM.m_StrikePoint);
	REG_SM_ATTR(CATTR_DB_RMB,				m_HumanSMU->m_HumanSM.m_RMBMoney);
	REG_SM_ATTR(CATTR_DB_BANK_RMB,			m_HumanSMU->m_HumanSM.m_BankRMB);
	REG_SM_ATTR(CATTR_DB_VIGOR_REGENE_RATE,	m_HumanSMU->m_HumanSM.m_VigorRegeneRate);
	REG_SM_ATTR(CATTR_DB_ENERGY,			m_HumanSMU->m_HumanSM.m_DoubleExpTime_Num);
	REG_SM_ATTR(CATTR_DB_MAX_ENERGY,		m_HumanSMU->m_HumanSM.m_GmRight);
	REG_SM_ATTR(CATTR_DB_ENERGY_REGENE_RATE,m_HumanSMU->m_HumanSM.m_EnergyRegeneRate);
	REG_SM_ATTR(CATTR_DB_RAGE,				m_HumanSMU->m_HumanSM.m_Rage);
	REG_SM_ATTR(CATTR_DB_LEVEL,				m_HumanSMU->m_HumanSM.m_Level);
	REG_SM_ATTR(CATTR_DB_PKVALUE,			m_HumanSMU->m_HumanSM.m_nPKValue);
	REG_SM_ATTR(CATTR_DB_CURRENTPET,		m_HumanSMU->m_HumanSM.m_guidCurrentPet);
	REG_SM_ATTR(CATTR_DB_EXP,				m_HumanSMU->m_HumanSM.m_Exp);
	REG_SM_ATTR(CATTR_DB_AT_POSITION,		m_HumanSMU->m_HumanSM.m_Position);
	REG_SM_ATTR(CATTR_DB_BK_POSITION,		m_HumanSMU->m_HumanSM.m_BakPosition);
	REG_SM_ATTR(CATTR_DB_AT_SCENE,			m_HumanSMU->m_HumanSM.m_StartScene);
	REG_SM_ATTR(CATTR_DB_BK_SCENE,			m_HumanSMU->m_HumanSM.m_BakScene);
	REG_SM_ATTR(CATTR_DB_STR,				m_HumanSMU->m_HumanSM.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR]);
	REG_SM_ATTR(CATTR_DB_SPR,				m_HumanSMU->m_HumanSM.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_SPR]);
	REG_SM_ATTR(CATTR_DB_CON,				m_HumanSMU->m_HumanSM.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON]);
	REG_SM_ATTR(CATTR_DB_INT,				m_HumanSMU->m_HumanSM.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT]);
	REG_SM_ATTR(CATTR_DB_DEX,				m_HumanSMU->m_HumanSM.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX]);
	REG_SM_ATTR(CATTR_DB_REMAINPOINT,		m_HumanSMU->m_HumanSM.m_Level1Points);
	REG_SM_ATTR(CATTR_DB_CAMP,				m_HumanSMU->m_HumanSM.m_CampData);
	REG_SM_ATTR(CATTR_DB_MENPAI,			m_HumanSMU->m_HumanSM.m_MenPai);
	REG_SM_ATTR(CATTR_DB_GUILDID,			m_HumanSMU->m_HumanSM.m_GuildID);
	REG_SM_ATTR(CATTR_DB_TEAMID,			m_HumanSMU->m_HumanSM.m_TeamID);
	REG_SM_ATTR(CATTR_DB_GUID,				m_HumanSMU->m_HumanSM.m_GUID);
	REG_SM_ATTR(CATTR_DB_PORTRAITID,		m_HumanSMU->m_HumanSM.m_PortraitID);
	REG_SM_ATTR_SIZE(CATTR_DB_NAME,			m_HumanSMU->m_HumanSM.m_Name,sizeof(CHAR)*MAX_CHARACTER_NAME);
	REG_SM_ATTR(CATTR_DB_SEX,				m_HumanSMU->m_HumanSM.m_Sex);
	REG_SM_ATTR(CATTR_DB_CREATETIME,		m_HumanSMU->m_HumanSM.m_CreateDate);
	REG_SM_ATTR_SIZE(CATTR_DB_TITLE,		m_HumanSMU->m_HumanSM.m_Title,sizeof(CHAR)*MAX_CHARACTER_TITLE);
	REG_SM_ATTR_SIZE(CATTR_DB_PASSWORD,		m_HumanSMU->m_HumanSM.m_Passwd,sizeof(CHAR)*MAX_PWD);
	REG_SM_ATTR(CATTR_DB_PWDDELTIME,		m_HumanSMU->m_HumanSM.m_uPwdDelTime);
	REG_SM_ATTR(CATTR_DB_HAIR_COLOR,		m_HumanSMU->m_HumanSM.m_HairColor);
	REG_SM_ATTR(CATTR_DB_FACE_COLOR,		m_HumanSMU->m_HumanSM.m_FaceColor);
	REG_SM_ATTR(CATTR_DB_HAIR_MODEL,		m_HumanSMU->m_HumanSM.m_HairModel);
	REG_SM_ATTR(CATTR_DB_FACE_MODEL,		m_HumanSMU->m_HumanSM.m_FaceModel);
	REG_SM_ATTR(CATTR_DB_ONLINETIME,		m_HumanSMU->m_HumanSM.m_OnlineTime);
	REG_SM_ATTR(CATTR_DB_LASTLOGINTIME,		m_HumanSMU->m_HumanSM.m_LastLoginTime);
	REG_SM_ATTR(CATTR_DB_LASTLOGOUTTIME,	m_HumanSMU->m_HumanSM.m_LastLogoutTime);
	REG_SM_ATTR(CATTR_DB_DBVERSION,			m_HumanSMU->m_HumanSM.m_DBVersion);
	REG_SM_ATTR(CATTR_DB_MONEY,				m_HumanSMU->m_HumanSM.m_Money);
	REG_SM_ATTR(CATTR_DB_BANK_MONEY,		m_HumanSMU->m_BankSM.m_Money);
	REG_SM_ATTR(CATTR_DB_BANK_OFFSIZE,		m_HumanSMU->m_BankSM.m_CurEndIndex);
	int iShopIndex = 0;
	for(int iDBShop=CATTR_DB_PLAYER_SHOP_BEGIN;iDBShop<=CATTR_DB_PLAYER_SHOP_END;iDBShop++)
	{
		REG_SM_ATTR((CHAR_ATTR_DB)iDBShop,m_dbHuman->m_ShopGuid[iShopIndex]);
		iShopIndex++;
	}
	int iFavoriteShopIndex = 0;
	for(int iDBFavoriteShop=CATTR_DB_PLAYER_SHOP_FAVORITE_BEGIN;iDBFavoriteShop<=CATTR_DB_PLAYER_SHOP_FAVORITE_END;iDBFavoriteShop++)
	{
		REG_SM_ATTR((CHAR_ATTR_DB)iDBFavoriteShop,m_dbHuman->m_ShopGuid[iFavoriteShopIndex]);
		iFavoriteShopIndex++;
	}
	int iBagIndex = 0;
	for(int iDBBag=CATTR_DB_BAG_BEGIN;iDBBag<=CATTR_DB_BAG_BEGIN+MAX_BAG_SIZE;iDBBag++)
	{
		REG_SM_ATTR((CHAR_ATTR_DB)iDBBag,m_HumanSMU->m_BagSM.m_pItem[iBagIndex]);
		iBagIndex++;
	}
	int iEquipIndex = 0;
	for(int iDBEquip = CATTR_DB_EQUIP_BEGIN;iDBEquip<=CATTR_DB_EQUIP_END;iDBEquip++)
	{
		REG_SM_ATTR((CHAR_ATTR_DB)iDBEquip,m_HumanSMU->m_EquipSM.m_pItem[iEquipIndex]);
		iEquipIndex++;
	}
	int iBankIndex = 0;
	for(int iDBBank = CATTR_DB_BANK_BEGIN;iDBBank<=CATTR_DB_BANK_END;iDBBank++)
	{
		REG_SM_ATTR((CHAR_ATTR_DB)iDBBank,m_HumanSMU->m_BankSM.m_pItem[iBankIndex]);
		iBankIndex++;
	}
	REG_SM_ATTR(CATTR_DB_SKILL,		m_HumanSMU->m_SkillSM);
	REG_SM_ATTR(CATTR_DB_COOLDOWN,	m_HumanSMU->m_CooldownSM);	
	REG_SM_ATTR(CATTR_DB_XINFA,		m_HumanSMU->m_XinFaSM);
	REG_SM_ATTR(CATTR_DB_IMPACT,	m_HumanSMU->m_ImpactSM);
	REG_SM_ATTR(CATTR_DB_ABILITY,	m_HumanSMU->m_AbilitySM);	
	REG_SM_ATTR(CATTR_DB_MISSION,	m_HumanSMU->m_MissionSM);
	REG_SM_ATTR(CATTR_DB_SETTING,	m_HumanSMU->m_SettingSM);
	REG_SM_ATTR(CATTR_DB_PET,		m_HumanSMU->m_PetListSM);
	REG_SM_ATTR(CATTR_DB_RELATION,  m_HumanSMU->m_RelationSM);
	REG_SM_ATTR(CATTR_DB_PRIVATEINFO,m_HumanSMU->m_PrivateInfoSM);
	REG_SM_ATTR(CATTR_DB_TITLEINFO,m_HumanSMU->m_HumanSM.m_Titles);
	REG_SM_ATTR_SIZE(CATTR_DB_RESERVE,m_HumanSMU->m_HumanSM.m_Reserve,sizeof(INT)*MAX_RESERVE);

	UnLock();
	__LEAVE_FUNCTION
}


VOID*	HumanDB::GetDBEnumAttr(CHAR_ATTR_DB eOffSet)
{
	__ENTER_FUNCTION

	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(m_AttrReg);

	return m_AttrReg->m_AttrPtr[eOffSet];

	__LEAVE_FUNCTION

	return NULL;
}

VOID*	HumanDB::GetSMEnumAttr(CHAR_ATTR_DB eOffSet)
{
	__ENTER_FUNCTION

	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(m_AttrRegSM);

	return m_AttrRegSM->m_AttrPtr[eOffSet];

	__LEAVE_FUNCTION

	return NULL;
}

INT		HumanDB::GetDBEnumAttrSize(CHAR_ATTR_DB eOffSet)
{
	__ENTER_FUNCTION

	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(m_AttrFlag);

	return m_AttrReg->m_AttrSize[eOffSet];

	__LEAVE_FUNCTION

	return 0;
}

VOID	HumanDB::SetDBEnumAttrDirty(CHAR_ATTR_DB eOffSet,BOOL bFlag)
{
	__ENTER_FUNCTION

	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(m_AttrFlag);

	m_AttrFlag->SetBitFlag(eOffSet,bFlag);
		
	__LEAVE_FUNCTION
}

BOOL	HumanDB::GetDBEnumAttrDirty(CHAR_ATTR_DB eOffSet)
{
	__ENTER_FUNCTION

	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(m_AttrFlag);
	
	
	return m_AttrFlag->IsUpdated(eOffSet);

	__LEAVE_FUNCTION

	return FALSE;
}

VOID	HumanDB::SetFreeOwnFlag(INT Flag)
{
	__ENTER_FUNCTION
		
	if(m_HumanSMU)
	{
		m_HumanSMU->SetUseStatus(Flag,SM_S_WRITE);
	}

	__LEAVE_FUNCTION
}

INT	HumanDB::GetFreeOwnFlag()
{
	__ENTER_FUNCTION
		
	if(m_HumanSMU)
	{
		return m_HumanSMU->GetUseStatus(SM_S_WRITE);
	}
	
	return -1;

	__LEAVE_FUNCTION

	return -1;
}
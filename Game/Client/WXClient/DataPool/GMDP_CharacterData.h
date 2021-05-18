/****************************************\
*										*
* 			   数据池数据结构接口		*
*					-角色数据			*
*										*
\****************************************/

#pragma once

#ifndef	__CHARACTERDATA_H__
#define __CHARACTERDATA_H__

/******************************************************
角色逻辑数据
******************************************************/
#include "GMDP_Struct_CharacterData.h"

struct _CAMP_DATA;
class CObject_Character;
class CCharacterData
{
	//-------------------------------------------
	//公用接口
public:
	INT					Get_RaceID(VOID) const { return m_pData->m_nRaceID; }			
	VOID				Set_RaceID(INT nRaceID);

	INT					Get_PortraitID(VOID) const { return m_pData->m_nPortraitID; }			
	VOID				Set_PortraitID(INT nPortraitID);

	LPCTSTR				Get_Name(VOID) const { return m_pData->m_strName.c_str(); }		
	VOID				Set_Name(LPCTSTR szName);

	LPCTSTR				Get_CurTitle(VOID) const { return m_pData->m_strTitle.c_str(); }											//1
	VOID				Set_CurTitle(LPCTSTR szTitle);

	BYTE				Get_CurTitleType(VOID) const { return m_pData->m_TitleType; }											//1
	VOID				Set_CurTitleType(BYTE bTitleType);

	FLOAT				Get_HPPercent(VOID) const { return m_pData->m_fHPPercent; }		
	VOID				Set_HPPercent(FLOAT fPercent);
	BOOL				IsDie(VOID)const{ return (Get_HPPercent() <= 0.f); }

	FLOAT				Get_MPPercent(VOID) const { return m_pData->m_fMPPercent; }		
	VOID				Set_MPPercent(FLOAT fPercent);

	FLOAT				Get_MoveSpeed(VOID) const { return m_pData->m_fMoveSpeed; }		
	VOID				Set_MoveSpeed(FLOAT fSpeed);

	const _CAMP_DATA	*Get_CampData(VOID) const { return &(m_pData->m_CampData); }			
	VOID				Set_CampData(const _CAMP_DATA *pCampData);

	ObjID_t				Get_OwnerID(VOID) const { return m_pData->m_nOwnerID; }
	VOID				Set_OwnerID(ObjID_t nOwnerID);

	INT					Get_AIType(VOID) const { return m_pData->m_nAIType; }
	VOID				Set_AIType(INT nAIType);

	GUID_t				Get_OccupantGUID(VOID) const { return m_pData->m_OccupantGUID; }
	VOID				Set_OccupantGUID(GUID_t guid);

	INT					Get_Relative(VOID) const { return m_pData->m_nRelative; }											
	VOID				Set_Relative(INT nRelative);

	INT					Get_ModelID(VOID) const { return m_pData->m_nModelID; }
	VOID				Set_ModelID(INT nModelID);

	INT					Get_MountID(VOID) const { return m_pData->m_nMountID; }
	VOID				Set_MountID(INT nMountID);

	INT					Get_Level(VOID) const { return m_pData->m_nLevel; }				
	VOID				Set_Level(INT nLevel);

	//BOOL				Get_FightState(VOID) const { return m_pData->m_bFightState; }	
	//VOID				Set_FightState(BOOL bFightState);

	INT					Get_StealthLevel(VOID) const { return m_pData->m_nStealthLevel; }				
	VOID				Set_StealthLevel(INT nStealthLevel);

	BOOL				IsSit(VOID) const { return m_pData->m_bSit; }	
	VOID				Set_Sit(BOOL bSit);

	//---------------------------------
	INT					Get_MenPai(VOID) const;											
	VOID				Set_MenPai(INT nMenPai);

	INT					Get_Guild(VOID) const;											
	VOID				Set_Guild(INT nGuild);

	INT					Get_HairMesh(VOID) const;										
	VOID				Set_HairMesh(INT nHairMesh);

	UINT				Get_HairColor(VOID) const;										
	VOID				Set_HairColor(UINT uHairColor);

	INT					Get_FaceMesh(VOID) const;										
	VOID				Set_FaceMesh(INT nFaceMesh);

	INT					Get_EquipVer(VOID) const;										
	VOID				Set_EquipVer(INT nEquipVer);

	INT					Get_Equip(HUMAN_EQUIP point) const;								
	VOID				Set_Equip(HUMAN_EQUIP point, INT nID); 

	INT					Get_EquipGem(HUMAN_EQUIP point) const;								
	VOID				Set_EquipGem(HUMAN_EQUIP point, INT nID); 

	BOOL				Get_HaveTeamFlag(VOID) const;
	VOID				Set_HaveTeamFlag(BOOL flag);

	BOOL				Get_TeamLeaderFlag(VOID) const;
	VOID				Set_TeamLeaderFlag(BOOL flag);

	BOOL				Get_TeamFullFlag(VOID) const;
	VOID				Set_TeamFullFlag(BOOL flag);

	BOOL				Get_TeamFollowFlag(VOID) const;
	VOID				Set_TeamFollowFlag(BOOL flag);

	INT					Get_TeamFollowListCount(VOID) const;
	VOID				Set_TeamFollowListCount(INT count);

	GUID_t				Get_TeamFollowedMember(INT index) const;
	VOID				Set_TeamFollowedMember(INT index, GUID_t guid);

	_TITLE_				Get_Title(INT index) const;										
	VOID				Set_Title(INT index, _TITLE_ title);

	INT					Get_TitleNum() const;
	VOID				Set_TitleNum(INT num );

	INT					Get_CurTitleIndex() const;
	VOID				Set_CurTitleIndex(INT index );

	VOID				Clear_AllTitles();												
	//---------------------------------
	INT					Get_HP(VOID) const;												
	VOID				Set_HP(INT nHP);

	INT					Get_MP(VOID) const;												
	VOID				Set_MP(INT nMP);

	INT					Get_Exp(VOID) const;											
	VOID				Set_Exp(INT nExp);

	INT					Get_MaxExp(VOID) const;											
	//VOID				Set_MaxExp(INT nMaxExp); //通过查表获得

	INT					Get_Money(VOID) const;											
	VOID				Set_Money(INT nMoney);

	PET_GUID_t			Get_CurrentPetGUID(VOID) const;
	VOID				Set_CurrentPetGUID(PET_GUID_t guid);

	INT					Get_Rage(VOID)const;
	VOID				Set_Rage(INT nRage);

	INT					Get_StrikePoint(VOID)const;
	VOID				Set_StrikePoint(INT nStrikePoint);

	INT					Get_RMB(VOID)const;
	VOID				Set_RMB(INT nRMB);

	//INT					Get_MaxVigor(VOID)const;
	//VOID				Set_MaxVigor(INT nVigor);

	INT					Get_DoubleExpTime_Num(VOID)const;
	VOID				Set_DoubleExpTime_Num(INT nDoubleExpTime_Num);

	//INT					Get_MaxEnergy(VOID)const;
	//VOID				Set_MaxEnergy(INT nEnergy);

	//INT					Get_GoodBadValue(VOID)const;
	//VOID				Set_GoodBadValue(INT nValue);

	//----
	INT					Get_STR(VOID) const;											
	VOID				Set_STR(INT nSTR);

	INT					Get_SPR(VOID) const;											
	VOID				Set_SPR(INT nSPR);

	INT					Get_CON(VOID) const;											
	VOID				Set_CON(INT nCON);

	INT					Get_INT(VOID) const;											
	VOID				Set_INT(INT nINT);

	INT					Get_DEX(VOID) const;											
	VOID				Set_DEX(INT nDEX);

	INT					Get_PointRemain(VOID) const;									
	VOID				Set_PointRemain(INT nPoint);

	//----
	INT					Get_AttPhysics(VOID) const;										
	VOID				Set_AttPhysics(INT nAttPhysics);

	INT					Get_AttMagic(VOID) const;										
	VOID				Set_AttMagic(INT nAttMagic);

	INT					Get_DefPhysics(VOID) const;										
	VOID				Set_DefPhysics(INT nDefPhysics);

	INT					Get_DefMagic(VOID) const;										
	VOID				Set_DefMagic(INT nDefMagic);

	//----
	INT					Get_MaxHP(VOID) const;											
	VOID				Set_MaxHP(INT nMaxHP);

	INT					Get_MaxMP(VOID) const;											
	VOID				Set_MaxMP(INT nMaxMP);

	INT					Get_HPRespeed(VOID) const;										
	VOID				Set_HPRespeed(INT nHPRespeed);

	INT					Get_MPRespeed(VOID) const;										
	VOID				Set_MPRespeed(INT nMPRespeed);

	INT					Get_Hit(VOID) const;											
	VOID				Set_Hit(INT nHit);

	INT					Get_Miss(VOID) const;											
	VOID				Set_Miss(INT nMiss);

	INT					Get_CritRate(VOID) const;										
	VOID				Set_CritRate(INT nCritRate);

	INT					Get_AttackSpeed(VOID) const;									
	VOID				Set_AttackSpeed(INT fSpeed);

	INT					Get_AttCold(VOID) const;										
	VOID				Set_AttCold(INT nAttCold);

	INT					Get_DefCold(VOID) const;										
	VOID				Set_DefCold(INT nDefCold);

	INT					Get_AttFire(VOID) const;										
	VOID				Set_AttFire(INT nAttFire);

	INT					Get_DefFire(VOID) const;										
	VOID				Set_DefFire(INT nDefFire);

	INT					Get_AttLight(VOID) const;										
	VOID				Set_AttLight(INT nAttLight);

	INT					Get_DefLight(VOID) const;										
	VOID				Set_DefLight(INT nDefLight);

	INT					Get_AttPosion(VOID) const;										
	VOID				Set_AttPosion(INT nAttPosion);

	INT					Get_DefPosion(VOID) const;										
	VOID				Set_DefPosion(INT nDefPosion);

	BOOL				IsLimitMove(VOID) const;
	VOID				SetLimitMove(BOOL bSet);

	//BOOL				IsLimitUseSkill(VOID) const;
	//VOID				SetLimitUseSkill(BOOL bSet);

	//BOOL				IsLimitHandle(VOID) const;
	//VOID				SetLimitHandle(BOOL bSet);

	BOOL				IsHaveCanActionFlag1( VOID )const;
	VOID				SetCanActionFlag1( BOOL bSet );

	BOOL				IsHaveCanActionFlag2( VOID )const;
	VOID				SetCanActionFlag2( BOOL bSet );


	// 设置二级保护密码 2006-3-27
	BOOL				Get_IsMinorPwdSetup(  BOOL bOpenDlg = TRUE ) const;
	VOID				Set_SetMinorPwdSetup( INT nSet );

	// 是否验证过二级保护密码 2006-3-27
	BOOL				Get_IsMinorPwdUnlocked( BOOL bOpenDlg = TRUE ) const;
	VOID				Set_SetMinorPwdUnlocked( INT nSet );

	//---------------------------------
	const SKILLCLASS_MAP&		Get_SkillClass(VOID) const;
	const SCLIENT_SKILLCLASS*	Get_SkillClass(INT nID) const;
	VOID						Set_SkillClass(INT nID, INT nLevel);


	const SSKILL_MAP&			Get_Skill(VOID) const;
	const SCLIENT_SKILL*		Get_Skill(INT nID) const;
	VOID						Set_Skill(INT nID, BYTE nLevel, BOOL bLean);
	VOID						Skill_CleanAll(void);
	//VOID						Update_SkillByXinFaID( INT nXinFaID );

	const SLIFEABILITY_MAP&		Get_LifeAbility(VOID) const;
	const SCLIENT_LIFEABILITY*	Get_LifeAbility(INT nID) const;
	VOID						Set_LifeAbility(INT nID, INT nLevel, INT nExp);

	const SPRESCR_MAP&			Get_Prescr(VOID) const;
	const SCLIENT_PRESCR*		Get_Prescr(INT nID) const;
	VOID						Set_Prescr(INT nID, BOOL bLean);

	BOOL						Get_IsInStall(VOID) const;										
	VOID						Set_IsInStall(BOOL bIsInStall);

	LPCTSTR						Get_StallName(VOID) const;		
	VOID						Set_StallName(LPCTSTR szName);

	const FLOAT					Get_SpeedRate(VOID) const { return m_fSpeedRate; }


public:
	CCharacterData(CObject_Character* pCharObj);
	virtual ~CCharacterData();

protected:
	VOID				_Init_AsCharacter(VOID);
	VOID				_Init_AsNPC(VOID);
	VOID				_Init_AsPlayerOther(VOID);
	VOID				_Init_AsPlayerMySelf(VOID);

	VOID				_On_MenPaiChanged(VOID);

protected:
	//角色逻辑指针
	CObject_Character*	m_pObjRef;
	//角色数据结构
	SDATA_CHARACTER*	m_pData;
	//改变角色频率相关
	FLOAT				m_fSpeedRate;
};

#endif	// __CHARACTERDATA_H__

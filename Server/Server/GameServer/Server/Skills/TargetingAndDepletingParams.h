///////////////////////////////////////////////////////////////////////////////
// 文件名：TargetingAndDepletingParams.h
// 功能说明：角色身上的目标和消耗临时数据的集合
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TARGETINGANDDEPLETINGPARAMS_H
#define TARGETINGANDDEPLETINGPARAMS_H

#include "Type.h"
#include "DataRecords.h"
#include "GameDefine2.h"
#include "OResultDef.h"
#include "GameStruct.h"
#include "DB_Struct.h"

namespace Combat_Module
{
	class TargetingAndDepletingParams_T
	{
		public:
			enum
			{
				MAX_PARAM_NUMBER = 8,
			};
		public:
			//Activating information
			SkillID_t	GetActivatedSkill(VOID) const {return m_nActivatedSkill;};
			VOID		SetActivatedSkill(SkillID_t nSkill) {m_nActivatedSkill=nSkill;};
			BYTE		GetSkillLevel(VOID) const {return m_nSkillLevel;};
			VOID		SetSkillLevel(BYTE nLevel) {m_nSkillLevel=nLevel;};
			ScriptID_t	GetActivatedScript(VOID) const {return m_nActivatedScript;};
			VOID		SetActivatedScript(ScriptID_t nScript) {m_nActivatedScript = nScript;};
			//Target informations
			ObjID_t		GetTargetObj(VOID) const {return m_nTargetObj;};
			VOID		SetTargetObj(ObjID_t nObj) {m_nTargetObj=nObj;};
			GUID_t		GetTargetGuid(VOID) const {return m_guidTargetPartner;};
			VOID		SetTargetGuid(GUID_t nGuid) {m_guidTargetPartner=nGuid;};
			WORLD_POS const& GetTargetPosition(VOID) const {return m_posTargetPosition;};
			VOID		SetTargetPosition(WORLD_POS const& rPos) {m_posTargetPosition = rPos;};
			FLOAT		GetTargetDirection(VOID) const {return m_fTargetDirection;};
			VOID		SetTargetDirection(FLOAT fDir) {m_fTargetDirection = fDir;};
			INT			GetTargetBagSlot(VOID) const {return m_nTargetBagSlot;};
			VOID		SetTargetBagSlot(INT nSlot) {m_nTargetBagSlot = nSlot;};
			PET_GUID_t const& GetTargetPetGuid(VOID) const {return m_guidTargetPet;};
			VOID		SetTargetPetGuid(PET_GUID_t const& rGuid) {m_guidTargetPet = rGuid;}
			//Flags
			BOOL		GetIgnoreConditionCheckFlag(VOID) const {return m_bIgnoreConditionCheck;};
			VOID		SetIgnoreConditionCheckFlag(BOOL bFlag) {m_bIgnoreConditionCheck = bFlag;};
			//Deplete informations
			INT			GetDepletedRage(VOID) const {return m_nDepletedRage;};
			VOID		SetDepletedRage(INT nDepleted) {m_nDepletedRage = nDepleted;};
			INT			GetDepletedStrikePoints(VOID) const {return m_nDepletedStrikePoints;};
			VOID		SetDepletedStrikePoints(INT nDepleted) {m_nDepletedStrikePoints = nDepleted;};
			_ITEM_GUID const& GetDepletedItemGuid(VOID) const {return m_guidDepletedItem;};
			VOID		SetDepletedItemGuid(_ITEM_GUID const& rGuid) {m_guidDepletedItem = rGuid;};
			INT			GetBagIndexOfDepletedItem(VOID) const {return m_nBagIndexOfDepletedItem;};
			VOID		SetBagIndexOfDepletedItem(INT nIndex) {m_nBagIndexOfDepletedItem = nIndex;};
			INT			GetItemIndexOfDepletedItem(VOID) const {return m_nItemIndexOfDepletedItem;};
			VOID		SetItemIndexOfDepletedItem(INT nIndex) {m_nItemIndexOfDepletedItem = nIndex;};
			//Other usful informations
			Time_t		GetDelayTime(VOID) const {return m_nDelayTime;};
			VOID		SetDelayTime(Time_t nTime) {m_nDelayTime = nTime;};
			INT			GetTargetCount(VOID) const {return m_nTargetCount;};
			VOID		SetTargetCount(INT nCount) {m_nTargetCount = nCount;};
			INT			GetCorrelativeXinFaLevel(VOID) const {return m_nCorrelativeXinFaLevel;};
			VOID		SetCorrelativeXinFaLevel(INT nLevel) {m_nCorrelativeXinFaLevel = nLevel;};
			//Error informations
			ID_t		GetErrCode(VOID) const {return m_nErrCode;};
			VOID		SetErrCode(ID_t nErr) {m_nErrCode = nErr;};
			INT			GetErrParam(VOID) const {return m_nErrParam;};
			VOID		SetErrParam(INT nParam) {m_nErrParam = nParam;};
			//Dynamic params
			inline BOOL	IsDynamicParamIndexLegal(INT nIndex) const
			{
				if(0<=nIndex && MAX_PARAM_NUMBER>nIndex)
				{
					return TRUE;
				}
				return FALSE;
			}
			inline INT	GetDynamicParamByIndex(INT nIndex) const
			{
				if(TRUE==IsDynamicParamIndexLegal(nIndex))
				{
					return m_aDynamicParams[nIndex];
				}
				return INVALID_ID;
			}
			inline VOID	SetDynamicParamByIndex(INT nIndex, INT nValue)
			{
				if(TRUE==IsDynamicParamIndexLegal(nIndex))
				{
					m_aDynamicParams[nIndex] = nValue;
				}
			}
		public:
			TargetingAndDepletingParams_T(VOID);
			~TargetingAndDepletingParams_T(VOID){};
			TargetingAndDepletingParams_T& operator=(TargetingAndDepletingParams_T const& rhs);
			VOID Reset(VOID);
		protected:
		private:
			//Activating information
			SkillID_t	m_nActivatedSkill;
			BYTE		m_nSkillLevel;
			ScriptID_t	m_nActivatedScript;
			//Target informations
			ObjID_t		m_nTargetObj;
			GUID_t		m_guidTargetPartner;
			WORLD_POS	m_posTargetPosition;
			FLOAT		m_fTargetDirection;
			INT			m_nTargetBagSlot;
			PET_GUID_t	m_guidTargetPet;
			//Flags
			BOOL		m_bIgnoreConditionCheck;
			//Deplete informations
			INT			m_nDepletedRage;
			INT			m_nDepletedStrikePoints;
			_ITEM_GUID 	m_guidDepletedItem;
			INT			m_nBagIndexOfDepletedItem;
			INT			m_nItemIndexOfDepletedItem;
			//Other usful informations
			Time_t		m_nDelayTime;
			INT			m_nTargetCount;
			INT			m_nCorrelativeXinFaLevel;
			//Dynamic params
			INT 		m_aDynamicParams[MAX_PARAM_NUMBER];
			//Error informations
			ID_t		m_nErrCode;
			INT			m_nErrParam;
	};
};

#endif //TARGETINGANDDEPLETINGPARAMS_H

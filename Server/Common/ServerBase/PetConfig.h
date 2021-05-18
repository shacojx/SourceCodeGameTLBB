#ifndef _PET_CONFIG_H_
#define _PET_CONFIG_H_

#define GENGU_NUM (5)

struct SPetConfig
{
	enum {
		VARIANCEPET_LEVEL_NUM = 8,
	};

	INT		 m_VariancePetRate;		//生成变异宠概率
	INT		 m_BabyPetRate;			//生成宝宝概率

	//野生宠等级生成概率（四个值）
	INT		 m_WilenessPetRate_TakeLevel;	// 携带等级概率
	INT		 m_WilenessPetRate_Delta1;		// 携带等级+,-1概率	
	INT		 m_WilenessPetRate_Delta2;		// 携带等级+,-2概率	
	INT		 m_WilenessPetRate_Delta3;		// 携带等级+,-3概率	

	//成长率生成概率（五个值）
	INT		 m_GrowRate0;
	INT		 m_GrowRate1;
	INT		 m_GrowRate2;
	INT		 m_GrowRate3;
	INT		 m_GrowRate4;

	// 宠物变异等级几率(10w)
	INT		m_aRateOfLevelVariancePet[VARIANCEPET_LEVEL_NUM];

	//根骨相关
	struct SGenGu
	{
		INT		m_Begin;
		INT		m_End;
		INT		m_Rate;
	};
	SGenGu	m_vGenGu[GENGU_NUM];

	//资质变动范围（如±5％）
	INT		 m_IntelligenceRange;

	//移动速度
	INT		 m_MoveSpeed;

	//攻击速度
	INT		 m_AttackSpeed;

	// AI相关数据
	INT		m_PetAI0_MagicRate;
	INT		m_PetAI1_MagicRate;
	INT		m_PetAI2_MagicRate;
	INT		m_PetAI3_MagicRate;

	// 初始HP、物攻、魔攻、物防、魔防、命中、闪避、会心（八个）
	INT		m_BaseHP;
	INT		m_BasePhyAttack;
	INT		m_BaseMgcAttack;
	INT		m_BasePhyDefence;
	INT		m_BaseMgcDefence;
	INT		m_BaseHit;
	INT		m_BaseMiss;
	INT		m_BaseCritical;

	// 体质对HP，力量对物攻，灵气对魔攻，体质对物防，定力对魔防，敏捷对闪避，敏捷对会
	// 心,敏捷对命中的影响系数
	FLOAT		m_Con_HP_Pram;
	FLOAT		m_Str_PhyAttack_Pram;
	FLOAT		m_Spr_MgcAttack_Pram;
	FLOAT		m_Con_PhyDefence_Pram;
	FLOAT		m_Int_MgcDefence_Pram;
	FLOAT		m_Dex_Miss_Pram;
	FLOAT		m_Dex_Critical_Pram;
	FLOAT		m_Dex_Hit_Pram;

	// 等级对HP、物攻、魔攻、物防、魔防、闪避、会心、命中的影响系数
	FLOAT		m_Level_HP_Pram;
	FLOAT		m_Level_PhyAttack_Pram;
	FLOAT		m_Level_MgcAttack_Pram;
	FLOAT		m_Level_PhyDefence_Pram;
	FLOAT		m_Level_MgcDefence_Pram;
	FLOAT		m_Level_Miss_Pram;
	FLOAT		m_Level_Critical_Pram;
	FLOAT		m_Level_Hit_Pram;

};

#endif
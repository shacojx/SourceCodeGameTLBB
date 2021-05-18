//TDDBC_Struct.h
//基本数据库结构


#pragma once

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								主角相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//主角种族							(CHAR_01)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_RACE			(101)		//主角种族(男，女)
struct _DBC_CHAR_RACE
{
	INT		nID;
	INT		nGender;
	INT		nIsPlayer;
	INT		nModelID;
	INT		nDefHeadGeo;
	INT		nDefHairGeo;
	INT		nDefBody;
	INT		nDefArm;
	INT		nDefFoot;
	INT		nIdleInterval;
};

/////////////////////////////////////////////////////////////////////
//主角头发模型						(CHAR_02)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_HAIR_GEO		(102)		//主角头发模型
struct _DBC_CHAR_HAIR_GEO
{
	INT		nID;
	INT		nRace;
	LPCSTR	pMeshFile;
	LPCSTR	pShowName;
};

/////////////////////////////////////////////////////////////////////
//主角脸部模型						(CHAR_04)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_HEAD_GEO		(104)		//主角脸部模型
struct _DBC_CHAR_HEAD_GEO
{
	INT		nID;
	INT		nRace;
	LPCSTR	pMeshFile;
	LPCSTR	pShowName;
};

/////////////////////////////////////////////////////////////////////
//主角动作组						(CHAR_06)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_ACTION_SET	(106)
#define MAX_WEAPON_TYPE_NUMBER		(9)
struct _DBC_CHARACTER_ACTION_SET
{
	INT		nID;
	LPCSTR	pWeapon_Set[MAX_WEAPON_TYPE_NUMBER];
	BOOL	bHideWeapon;
	INT		nAppointedWeaponID;		// 指定的武器ID
	LPCSTR	pszDesc;
};

/////////////////////////////////////////////////////////////////////
//主角特效							(CHAR_07)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_EFFECT		(107)
struct _DBC_CHARACTER_EFFECT
{
	INT		nID;
	INT		nEffect1;
	INT		nEffect2;
	INT		nSoundID;
	LPCSTR	pLocator;
	LPCSTR	pWeaponLocator;
};

/////////////////////////////////////////////////////////////////////
//主角升级经验值						(CHAR_08)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_EXPLEVEL		(108)
struct _DBC_CHARACTER_EXPLEVEL
{
	INT		nID;
	INT		nEffectNeed;
};

/////////////////////////////////////////////////////////////////////
//外形ID对应模型名称					(CHAR_09)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_MODEL			(109)
#define MAX_MOUNT_NUMBER			(20)
struct _DBC_CHARACTER_MODEL
{
	INT		m_nID;
	LPCSTR	m_pszModelName;
	FLOAT	m_fFuseTime;				// 动作熔合时间
	LPCSTR	m_pszActionSetName_None;
	LPCSTR	m_apszActionSetName_Mount[MAX_MOUNT_NUMBER];
};

/////////////////////////////////////////////////////////////////////
//主角头像
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_FACE		(110)		//主角头像
struct _DBC_CHAR_FACE
{
	INT		nID;
	INT		nRace;
	LPCSTR	pImageSetName;
};

/////////////////////////////////////////////////////////////////////
//坐骑表
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_MOUNT		(111)
struct _DBC_CHARACTER_MOUNT
{
	INT		m_nID;
	INT		m_nModelID;
	FLOAT	m_fAddHeight;
	INT		m_nPassLevel;
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								生物相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//生物定义总表						(CREATURE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_CREATURE_ATT			(201)
struct _DBC_CREATURE_ATT
{
	INT		nID;
	LPCSTR	pName;
	INT		nCannotBeAttack;
	INT		nLevel;
	INT		NOTCARE1[2];
	INT		nIdleInterval;
	INT		NOTCARE2[6];
	INT		nCanHandle;				// 是否可以交互
	INT		nMonsterBossFlag;
	INT		nAttackRate_P;
	INT		nDefence_P;
	INT		nAttackRate_M;
	INT		nDefence_M;
	INT		nMaxHP;
	INT		nMaxMP;
	INT		nRestoreHP;
	INT		nRestoreMP;
	INT		nHit;
	INT		nMiss;
	INT		nCritrate;
	INT		nSpeed;
	INT		nMonsterWalkSpeed;
	INT		nAttackSpeed;
	INT		nAttackCold;
	INT		nDefenceCold;
	INT		nAttackFire;
	INT		nDefenceFire;
	INT		nAttackLight;
	INT		nDefenceLight;
	INT		nAttackPoison;
	INT		nDefencePoison;
	INT		nMonsterBossSnapImmID;
	INT		nMonsterBossDurationImmID;
	INT		nModelID;				// 外型ID 以前是LPCSTR	szModeleFile;
//	INT		nReserve;				// 已经废除 LPCSTR	szActionSetFile;		// 动作文件名称 
	INT		nIsDisplayerName;		// 是否显示头顶信息板
	INT		nIsCanSwerve;			// 是否随玩家点选转向
	FLOAT	fBoardHeight;			// 头顶名字板高度
	FLOAT	fProjTexRange;			// 选中环大小
	FLOAT	fShadowRange;			// 是否显示阴影，阴影大小
	LPCSTR	szIconName;				// 头像图标
};

/////////////////////////////////////////////////////////////////////
//宠物附加属性						(DBC_PET_EX_ATT)
/////////////////////////////////////////////////////////////////////
#define DBC_PET_EX_ATT				(202)
struct _DBC_PET_EX_ATT
{
	INT		m_Type;						// 宠物编号
	LPTSTR	m_Name;						// 名称
	INT		m_TakeLevel;				// 可携带等级
	INT		m_Camp;						// 阵营
	INT		m_Reserve1;					// 保留
	INT		m_Reserve2;					// 保留
	INT		m_FoodType;					// 食物类
	INT		m_SkillLearnNum;			// 所能学的技能数
	INT		m_PositiveSkill;			// 主动技能
	INT		m_PassiveSkill1;			// 被动技能1
	INT		m_PassiveSkill2;			// 被动技能2
	INT		m_PassiveSkill3;			// 被动技能3
	INT		m_PassiveSkill4;			// 被动技能4
	INT		m_Life;						// 标准寿命
	INT		m_StrPerception;			// 标准力量资质
	INT		m_ConPerception;			// 标准体质资质
	INT		m_DexPerception;			// 标准灵气资质
	INT		m_SprPerception;			// 标准身法资质
	INT		m_IntPerception;			// 标准定力资质
	INT		m_GrowRate0;				// 成长率1
	INT		m_GrowRate1;				// 成长率2
	INT		m_GrowRate2;				// 成长率3
	INT		m_GrowRate3;				// 成长率4
	INT		m_GrowRate4;				// 成长率5
	INT		m_CowardiceRate;			// 胆小几率
	INT		m_WarinessRate;				// 谨慎几率	
	INT		m_LoyalismRate;				// 忠诚几率
	INT		m_CanninessRate;			// 精明几率
	INT		m_ValourRate;				// 勇猛几率
};

/////////////////////////////////////////////////////////////////////
//生物声音定义表					(CREATURE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_CREATURE_SOUND			(203)
struct _DBC_CREATURE_SOUND
{
	INT		nID;
	INT		nAttack;
	INT		nUnderAttack;
	INT		nDeath;
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								物品相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//装备类物品显示定义				(ITEM_01)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_VISUAL_CHAR		(301)
struct _DBC_ITEM_VISUAL_CHAR
{
	#define CHAR_RACE_NUM	(2)

	INT		nID;
	LPCSTR	pVisualEntity[2*CHAR_RACE_NUM];
	LPCSTR	pEffectName[30];//增加30列各种宝石的特效
};

/////////////////////////////////////////////////////////////////////
//挂接类物品显示定义				(ITEM_02)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_VISUAL_LOCATOR		(302)
struct _DBC_ITEM_VISUAL_LOCATOR
{
	INT		nID;
	LPCSTR	pObjFile_Right;
	LPCSTR	pMatName_Right;
	LPCSTR	pObjParam1_Right;
	LPCSTR	pObjParam2_Right;
	LPCSTR	pObjFile_Left;
	LPCSTR	pMatName_Left;
	LPCSTR	pObjParam1_Left;
	LPCSTR	pObjParam2_Left;
	LPCSTR	pEffectName[30];//增加30列各种宝石的特效

};

/////////////////////////////////////////////////////////////////////
//物品定义表_白装					(ITEM_03)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_EQUIP_WHITE		(303)
struct _DBC_ITEM_EQUIP_WHITE
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nIndex;
	INT		nEquipPoint;
	INT		nVisualID;
	INT		nRule;
	INT		nSetID;
	INT		nSetNum;
	INT		nJob;
	LPCSTR	szName;
	INT		nLevelRequire;
	LPCSTR	szDesc;
	INT		nBasePrice;
	INT		nBMaxDur;
	INT		nBMaxRepair;
	INT		nBMaxGem;
	//...NOT CARE
	INT		NOTCARE[66];
	LPCTSTR szIcon;
	LPCTSTR szTypeDesc;		// 物品的类型描述2006－4－28
};

/////////////////////////////////////////////////////////////////////
//物品定义表_绿装					(ITEM_04)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_EQUIP_GREEN		(304)
struct _DBC_ITEM_EQUIP_GREEN
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nIndex;
	INT		nEquipPoint;
	INT		nVisualID;
	INT		nRule;
	INT		nSetID;
	INT		nSetNum;
	INT		nJob;
	LPCSTR	szName;
	INT		nLevelRequire;
	LPCSTR	szDesc;
	INT		nBasePrice;
	INT		nBMaxDur;
	INT		nBMaxRepair;
	INT		nBMaxGem;
	//...NOT CARE
	INT		NOTCARE[66];
	LPCTSTR szIcon;
	LPCTSTR szTypeDesc;		// 物品的类型描述2006－4－28
};


/////////////////////////////////////////////////////////////////////
//物品定义表_蓝装					(ITEM_05)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_EQUIP_BLUE		(305)
struct _DBC_ITEM_EQUIP_BLUE
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nIndex;
	INT		nEquipPoint;
	INT		nVisualID;
	INT		nRule;
	INT		nSetID;
	INT		nSetNum;
	INT		nJob;
	LPCSTR	szName;
	INT		nLevelRequire;
	LPCSTR	szDesc;
	INT		nBasePrice;
	INT		nBMaxDur;
	INT		nBMaxRepair;
	INT		nBMaxGem;
	//...NOT CARE
	INT		NOTCARE[66];
	LPCTSTR szIcon;
	LPCTSTR szTypeDesc;		// 物品的类型描述2006－4－28
};

/////////////////////////////////////////////////////////////////////
//物品定义表_药瓶				(ITEM_06)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_MEDIC			(306)
struct _DBC_ITEM_MEDIC
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nIndex;
	LPCSTR	szIcon;
	LPCSTR	szName;
	LPCSTR	szDesc;
	INT		nLevelRequire;
	INT		nSalePrice;	//基本价格
	INT		nRule;		//适应规则
	INT		nPileCount; //叠放数量
	INT		nScriptID;	//脚本编号
	INT		nSkillID;	//技能编号
	INT		NOTCARE[3];	//需求技能等级
	INT		nMaxHold;	//最大持有数量
	INT		nTargetType;	// 针对对象类型 ENUM_ITEM_TARGET_TYPE
	LPCTSTR szTypeDesc;		// 物品的类型描述2006－4－28

	//... NOT CARE
};

/////////////////////////////////////////////////////////////////////
//物品定义表_宝石				(ITEM_07)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_GEM			(307)
struct _DBC_ITEM_GEM
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nGemIndex;
	LPCTSTR	szIcon;
	INT		nRule;
	LPCTSTR	szName;
	LPCTSTR szDesc;
	INT		nPrice;		// 宝石的价格
	INT		nAttr[60];	// 宝石的附加属性
	LPCTSTR	szColor;	// 附加属性的颜色
	LPCTSTR szTypeDesc; // 类型描述
	INT		nEffectIndex;//特效索引
	//... NOT CARE
};

/////////////////////////////////////////////////////////////////////
//物品定义表_地图				(ITEM_08)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_STOREMAP			(308)
struct _DBC_ITEM_STOREMAP
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nIndex;
	LPCSTR	szIcon;
	LPCSTR	szName;
	LPCSTR	szDesc;
	INT		nLevelRequire;
	INT		nRule;
	//.......................
	//... NOT CARE
};

/////////////////////////////////////////////////////////////////////
//套装组合后附加属性			(ITEM_15)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_SETATT			(315)
struct _DBC_ITEM_SETATT
{
	INT		nID;
	INT		nAtt[60];
};


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								声音相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//声音文件定义						(SOUND_01)
/////////////////////////////////////////////////////////////////////
#define DBC_SOUND_INFO				(401)
struct _DBC_SOUND_INFO
{
	INT		nID;
	LPCSTR	pSoundFile;
};


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								特效相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//特效定义							(EFFECT_01)
/////////////////////////////////////////////////////////////////////
#define DBC_EFFECT_DEFINE			(501)
struct _DBC_EFFECT_DEFINE
{
	INT		nID;
	LPCSTR	pEffectType;
	LPCSTR	pParam1;
	LPCSTR	pParam2;
	LPCSTR	pParam3;
	LPCSTR	pParam4;
	LPCSTR	pParam5;
	LPCSTR	pParam6;
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								技能相关								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
/////////////////////////////////////////////////////////////////////
//心法定义表						(SKILL_01)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_XINFA				(601)
struct _DBC_SKILL_XINFA
{
	INT		nID;
	INT		nMenpaiID;
	LPCSTR	pszName;
	LPCSTR	pszDesc;
	LPCSTR	pszIconName;
};

/////////////////////////////////////////////////////////////////////
//附加效果							(SKILL_02)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_ADDIMPACT			(602)
struct _DBC_SKILL_ADDIMPACT
{
	INT		nID;
	LPCSTR	pDesc;
	INT		nParamNum;
	INT		nAddAttributeNum;
	LPCSTR	pAddAttribute1;
	LPCSTR	pAddAttribute2;
	LPCSTR	pAddAttribute3;
	LPCSTR	pAddAttribute4;
	INT		nSpliceID;
	LPCSTR	pIcon;
};
//
///////////////////////////////////////////////////////////////////////
////子弹效果定义						(SKILL_03)
///////////////////////////////////////////////////////////////////////
//#define DBC_SKILL_BULLET			(603)
//struct _DBC_SKILL_BULLET
//{
//	INT		nID;							//子弹类型
//	INT		nType;							/*
//											|	0 - 瞬间到达
//											|	1 - 移动到达
//											*/
//	INT		nEffect;						//特效ID
//	FLOAT	fSpeed;							//移动速度	(m/s)
//	INT		nHitEffect;						//击中特效
//	INT		nHitEffectLiveTime;				//击中特效维持时间
//};

/////////////////////////////////////////////////////////////////////
//BUFF附加效果						(BUFF_IMPACT)
/////////////////////////////////////////////////////////////////////
#define DBC_BUFF_IMPACT					(604)
//_DBC_BUFF_IMPACT

/////////////////////////////////////////////////////////////////////
//DIRECTLY附加效果						(DIRECTLY_IMPACT)
/////////////////////////////////////////////////////////////////////
#define DBC_DIRECTLY_IMPACT				(605)
//_DBC_DIRECTLY_IMPACT

/////////////////////////////////////////////////////////////////////
//子弹						(BULLET)
/////////////////////////////////////////////////////////////////////
#define DBC_BULLET_DATA					(606)
//_DBC_BULLET_DATA

/////////////////////////////////////////////////////////////////////
//技能						(SKILL)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_DATA					(607)
//_DBC_SKILL_DATA

/////////////////////////////////////////////////////////////////////
//技能						(SKILL_DEPLETE)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_DEPLETE					(608)
struct _DBC_SKILL_DEPLETE
{
	INT			m_nID;
	INT			m_nHP;
	INT			m_nMP;
	INT			m_nSP;
	INT			m_nStrikePoint;
	INT			m_nItemID;
};

/////////////////////////////////////////////////////////////////////
//任务						(MISSION)
/////////////////////////////////////////////////////////////////////
#define DBC_MISSION_LIST				(610)
struct _DBC_MISSION_LIST
{
	INT				nScriptID;
	INT				nMissionID;
	INT				nReward;
	INT				nPunish;
	INT				nDialog;
	INT				nPromulgatorScene;
	INT				nPromulgatorID;
	INT				nSubmitorScene;
	INT				nSubmitorID;
	INT				nDestScene;
	INT				nDestX;
	INT				nDestZ;
};

#define DBC_MISSION_DATA				(611)
struct _DBC_MISSION_DATA
{
	INT		nMissionID;
	INT		nMissionClass;
	INT		nMissionSubClass;
	INT		nMissionType;
	INT		nMissionLevel;
	INT		nLevel;
	INT		nMaxLeve;
	INT		nCamp;
	INT		nIdentity; //no use
	INT		nItem1ID;
	INT		nItem1Num;
	INT		nItem2ID;
	INT		nItem2Num;
	INT		nItem3ID;
	INT		nItem3Num;
	INT		nItem4ID;
	INT		nItem4Num;
	INT		nItem5ID;
	INT		nItem5Num;
	INT		nItem6ID;
	INT		nItem6Num;
	INT		nItem7ID;
	INT		nItem7Num;
	INT		nMonster1ID;
	INT		nMonster1Num;
	INT		nMonster2ID;
	INT		nMonster2Num;
	INT		nMonster3ID;
	INT		nMonster3Num;
	INT		nMonster4ID;
	INT		nMonster4Num;
	INT		nMoney1;
	INT		nMoney2;
	INT		nMenPai;
	INT		nGuildLevel; //no use
	INT		nMissionKind;
	INT		nTotalTimes;
	INT		nDayTimes;
	INT		nTimeDist;
	INT		nMaxCycle;
	INT		nPositionTag;
	INT		nPreMission;
	INT		nAftMission;
};

#define DBC_MISSION_REWARD				(612)
struct _DBC_MISSION_REWARD
{
	INT			nRewardID;
	INT			nMoney;
	INT			nItem1ID;
	INT			nItem1Num;
	INT			nItem2ID;
	INT			nItem2Num;
	INT			nItem3ID;
	INT			nItem3Num;
	INT			nItem4ID;
	INT			nItem4Num;
	INT			nItem5ID;
	INT			nItem5Num;
	INT			nItem6ID;
	INT			nItem6Num;
	INT			nExp;
	INT			nLevel;
	INT			nGuildExp1;
	INT			nGuildExp2;
	INT			nGuildExp3;
	INT			nBuff1ID;
	INT			nBuff2ID;
	INT			nBuff3ID;
};

#define DBC_MISSION_PUNISH				(613)
struct _DBC_MISSION_PUNISH
{
	INT		nPunishID;
	INT		nMoney;
	INT		nItem1ID;
	INT		nItem1Num;
	INT		nItem2ID;
	INT		nItem2Num;
	INT		nItem3ID;
	INT		nItem3Num;
	INT		nExp;
	INT		nLevel;
	INT		nMissionTag;
};

#define DBC_MISSION_DIALOG				(614)
struct _DBC_MISSION_DIALOG
{
	INT			nDialogID;
	LPCSTR		szMissionName;		//任务名称
	LPCSTR		szMissionDesc;	//任务描述
	LPCSTR		szMissionTarget;	//任务目标
	LPCSTR		szMissionContinue;	//继续任务
	LPCSTR		szMissionAbandon;	//放弃任务
	LPCSTR		szMissionSuccess;	//完成任务
};

/////////////////////////////////////////////////////////////////////
//法术OBJ数据						(SPECIAL_OBJ_DATA)
/////////////////////////////////////////////////////////////////////
#define DBC_SPECIAL_OBJ_DATA			(630)
struct _DBC_SPECIAL_OBJ_DATA
{
	INT			m_nID;					//ID
	LPCSTR		m_lpszName;				//名称
	LPCSTR		m_lpszToolTips;			//ToolTips
	INT			m_nType;				//类别（服务器专用）
	INT			m_nLogicID;				//逻辑ID（服务器专用）
	INT			m_nStealthLevel;		//隐形级别（服务器专用，陷阱专用）
	INT			m_nTrapFlags;			//陷阱标记位集合（服务器专用，陷阱专用）
	INT			m_nActiveTimes;			//可以激发的次数（服务器专用）
	LPCSTR		m_lpszEffect_Normal;	//生存期持续特效
	LPCSTR		m_lpszSound_Normal;		//生存期持续音效
	LPCSTR		m_lpszEffect_Active;	//激发特效
	LPCSTR		m_lpszSound_Active;		//激发音效
	LPCSTR		m_lpszEffect_Die;		//死亡特效
	LPCSTR		m_lpszSound_Die;		//死亡音效
	INT			m_nReserve1;			//预留
	INT			m_nReserve2;			//预留
	INT			m_nReserve3;			//预留
	INT			m_nBulletID;			//子弹ID
	INT			m_nDuration;			//持续时间（服务器专用）
	INT			m_nInterval;			//激活或触发时间间隔（服务器专用）
	FLOAT		m_nTriggerRadius;		//触发半径（服务器专用，陷阱专用）
	FLOAT		m_nEffectRadius;		//影响半径（服务器专用，陷阱专用）
	INT			m_nEffectUnitNum;		//影响个体数目（服务器专用，陷阱专用）
	INT			m_nReserve4;			//预留
	INT			m_nReserve5;			//预留
	INT			m_nReserve6;			//预留
	INT			m_nReserve7;			//预留
	INT			m_nReserve8;			//预留
	LPCSTR		m_lpszParamDesc0;		//参数说明（服务器专用）
	INT			m_nParam0;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc1;		//参数说明（服务器专用）
	INT			m_nParam1;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc2;		//参数说明（服务器专用）
	INT			m_nParam2;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc3;		//参数说明（服务器专用）
	INT			m_nParam3;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc4;		//参数说明（服务器专用）
	INT			m_nParam4;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc5;		//参数说明（服务器专用）
	INT			m_nParam5;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc6;		//参数说明（服务器专用）
	INT			m_nParam6;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc7;		//参数说明（服务器专用）
	INT			m_nParam7;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc8;		//参数说明（服务器专用）
	INT			m_nParam8;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc9;		//参数说明（服务器专用）
	INT			m_nParam9;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc10;		//参数说明（服务器专用）
	INT			m_nParam10;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc11;		//参数说明（服务器专用）
	INT			m_nParam11;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc12;		//参数说明（服务器专用）
	INT			m_nParam12;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc13;		//参数说明（服务器专用）
	INT			m_nParam13;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc14;		//参数说明（服务器专用）
	INT			m_nParam14;				//参数值（服务器专用）
	LPCSTR		m_lpszParamDesc15;		//参数说明（服务器专用）
	INT			m_nParam15;				//参数值（服务器专用）
	LPCSTR		m_lpszInfo;				//详细说明(内部使用)
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								场景相关相关							//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//场景定义							(SCENE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_SCENE_DEFINE			(701)
struct _DBC_SCENE_DEFINE
{
	static const int SCENE_SERVER_ID_COLUMN = 1;

	INT		nLocalID;
	INT		nServerID;			//	 <--- ServerID is here!
	INT		nCityLevel;
	LPCSTR	szName;
	INT		nXSize;
	INT		nZSize;
	LPCSTR	szWXObjectName;
	LPCSTR	szRegionFile;
	LPCSTR  szCollisionfile;	// 建筑物行走面文件。
	LPCSTR	szMiniMap;
	INT		nBackSound;
	LPCSTR  szSceneMap;

	INT		nWroldMapPosX;
	INT		nWroldMapPosY;				// 场景图标所在得位置
	INT		nNameWroldMapPosX;
	INT		nNameWroldMapPosY;			// 场景名字所在得位置
	LPCSTR	szSceneType;				// 场景得图标等级
	LPCSTR	szCityNameNormalImageSet;		// 场景正常名字所用得图片资源
	LPCSTR	szCityNameNormalImage;		// 场景正常名字所用得图片资源
	LPCSTR	szCityNameHoverImageSet;		// 场景高亮名字所用得图片资源
	LPCSTR	szCityNameHoverImage;		// 场景高亮名字所用得图片资源

};

/////////////////////////////////////////////////////////////////////
//场景建筑物定义					(SCENE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_BUILDING_DEINFE			(702)
struct _DBC_BUILDING_DEFINE
{
	static const int MAX_LEVEL = 5;

	INT		nID;				//资源id
	LPCSTR	szResType;			//资源类型
	LPCSTR	szLevel[MAX_LEVEL];	//等级1-5资源
};

/////////////////////////////////////////////////////////////////////
//城市建筑物定义					(SCENE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_CITY_BUILDING			(703)
struct _DBC_CITY_BUILDING
{
	INT		nID;					//id
	INT		nCityID;				//城市id(DBC_CITY_DEINFE)
	INT		nBuildingID;			//建筑物id(DBC_BUILDING_DEINFE)
	INT		nInitLevel;				//初始等级
	LPCSTR	szGfxPosition;			//位置(gfx)
	LPCSTR	szGfxOrientation;		//旋转四元数(gfx)
};

/////////////////////////////////////////////////////////////////////
//场景传送点定义							(SCENE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_SCENE_POS_DEFINE			(917)
struct _DBC_SCENE_POS_DEFINE
{
	INT		nID;				// id
	LPCSTR	szSceneName;		// 场景的名字
	INT		nSceneID;			// 场景的id
	INT		nXPos;				// 传送点的位置
	INT		nZPos;				// 传送点的位置
	LPCSTR	szAimName;			// 传送到哪个场景
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								UI相关									//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
/////////////////////////////////////////////////////////////////////
//UI组定义						(UI_01)
/////////////////////////////////////////////////////////////////////
#define DBC_UI_LAYOUTDEFINE		(801)
struct _DBC_UI_LAYOUTDEFINE
{
	INT		nID;
	LPCSTR	szName;
	LPCSTR	szLayout;
	LPCSTR	szScript;
	INT		nDemise;
};


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								生活技能相关							//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
/////////////////////////////////////////////////////////////////////
//资源生长点							(LIFE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_GROWPOINT		(901)
struct _DBC_LIFEABILITY_GROWPOINT
{
	INT		nID;
	LPCSTR	szName;
	LPCSTR	szMeshFile;
	INT		nLifeAbility;
	LPCSTR	szTips;
	INT		nTimeNeed;
	LPCSTR	szNotcare[4];
	INT		nAnimOpen;
};

/////////////////////////////////////////////////////////////////////
//生活技能								(LIFE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_DEFINE	(902)
struct _DBC_LIFEABILITY_DEFINE
{
	INT		nID;
	LPCSTR	szName;
	INT		nLevelNeed;
	INT		nLevelMax;
	INT		nTimeOperation;
	INT		nToolNeed;
	FLOAT	fPlatformDist;
	INT		nPlatformID;
	INT		nAnimOperation;
	LPCSTR	szIconName;
	INT		nItemVisualLocator; //动作挂接
	LPCSTR	szExplain;			//详细解释
	INT		nPopup;				//是否弹出下级界面
	LPCSTR	szProgressbarName;	//进度条名称
};

/////////////////////////////////////////////////////////////////////
//生活技能								(LIFE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_ITEMCOMPOSE		(903)
struct _DBC_LIFEABILITY_ITEMCOMPOSE
{
	INT		nID;
	LPCSTR	szName;
	INT		nResultID;
	INT		nResultNum;
	INT		nLifeAbility;
	INT		nLifeAbility_Level;
	INT		nStuff1ID;
	INT		nStuff1num;
	INT		nStuff2ID;
	INT		nStuff2num;
	INT		nStuff3ID;
	INT		nStuff3num;
	INT		nStuff4ID;
	INT		nStuff4num;
	INT		nStuff5ID;
	INT		nStuff5num;
	INT		nAttribute_ID1;
	INT		nAttribute_Level1;
	INT		nAttribute_ID2;
	INT		nAttribute_Level2;
	INT		nCoolDown_Time;
	INT		nFacility;				// 需要的工具	
	INT		nCoolDoon_Troop;		// 冷却组
	INT		nProficiencyRequire;	// 熟练度需求
	INT		nProficiencyIncrease;	// 熟练度增加
	INT		NOTCARE;				// 不关心的1列
	INT		nProficiencyTime;		// 配方时间
	INT		nScriptID;				// 脚本ID
	INT		nFailItem;				// 失败后的替代品
	LPCSTR	szExplain;				// 详细描述


	// 功能分组

	//NOT CARE...
};

/////////////////////////////////////////////////////////////////////
//生活技能								(LIFE_04)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_PLATFORM		(904)
struct _DBC_LIFEABILITY_PLATFORM
{
	INT		nID;
	INT		nType;
	LPCSTR	szName;
	LPCSTR	szMeshFile;
	INT		nClickType;
};
/////////////////////////////////////////////////////////////////////
// 表情和颜色转换字符
/////////////////////////////////////////////////////////////////////
#define DBC_CODE_CONVERT			( 905 )
struct _DBC_CODE_CONVERT
{
	INT		nID;		// ID
	LPCSTR  szName;		// 介绍
	LPCSTR  szCode;		// 字符串形式的十六进制数字
};

/////////////////////////////////////////////////////////////////////
//心法技能								(XINFA_02)
/////////////////////////////////////////////////////////////////////
#define DBC_XINFA_XINFADEFINE	(902)
struct _DBC_XINFA_XINFADEFINE
{
	INT		nID;			//心法ID
	INT		nIDMenpai;		//门派ID
	LPCSTR	szName;			//心法名称
	LPCSTR	szTips;			//心法描述
	LPCSTR	szIconName;		//图标名称
};
/////////////////////////////////////////////////////////////////////
//心法升级花费						(XINFA_02)
/////////////////////////////////////////////////////////////////////
struct Spend
{
	INT		dwSpendMoney;			// 需要花费的金钱数
	INT		dwSpendExperience;		// 需要花费的经验值
};
#define DBC_XINFA_STUDYSPEND	(907)
struct _DBC_XINFA_STUDYSPEND
{
	INT		nStudyLevel;			// 要学习的等级
	Spend	StudySpend[54];
};

/////////////////////////////////////////////////////////////////////
//称号列表						(TITLE)
/////////////////////////////////////////////////////////////////////
#define DBC_TITLE_NAME			(908)
struct _DBC_TITLE_NAME
{
	INT		nTitleIndex;			// 称号的索引
	UINT	nReserve;				// 类型保留
	LPCSTR	szTitle;				// 称号的名字
	LPCSTR	szTitleDesc;			// 称号的描述
};
/////////////////////////////////////////////////////////////////////
//阵营数据	
/////////////////////////////////////////////////////////////////////
#define DBC_CAMP_DATA	(909)

/////////////////////////////////////////////////////////////////////
//技能	skillData_v1.txt(读取表)					
/////////////////////////////////////////////////////////////////////
#define DBC_SKILLDATA_V1_DEPLETE					(910)
struct _DBC_SKILLDATA_V1_DATA_PAIR
{
	INT		nData;
	LPCSTR  szDesc;
	
};
struct _DBC_SKILLDATA_V1_DEPLETE
{

	INT		nId;				//  ID	  
	INT		nCeHuaId;			//	策划专用ID	
	LPCSTR	szEffectDesc;		//	效果的简要逻辑说明
	INT		nNeedLevel;			//	
	INT		nNeedMoney;			//
	INT		nSkillLogicid;		//	技能逻辑ID	
	INT		nCooldownTime;		//	冷却时间	
	INT		nJuqiTime;			//	聚气时间(ms)	
	INT		nDirectTime;		//	引导时间(ms)	
	LPCSTR  szCondition1Desc;	//	条件或消耗参数说明
	INT		nCondition11;		//	条件或消耗参数值	
	INT	    nCondition12;		//	条件或消耗参数值	
	LPCSTR  szCondition2Desc;	//	条件或消耗参数说明
	INT		nCondition21;		//	条件或消耗参数值	
	INT		nCondition22;		//	条件或消耗参数值	
	LPCSTR  szCondition3Desc;	//	条件或消耗参数说明
	INT		nCondition31;		//	条件或消耗参数值	
	INT		nCondition32;		//	条件或消耗参数值	
	LPCSTR	szCondition4Desc;	//	条件或消耗参数说明
	INT		nCondition41;		//	条件或消耗参数值	
	INT		nCondition42;		//	条件或消耗参数值	
	LPCSTR  szCondition5Desc;	//	条件或消耗参数说明
	INT		nCondition51;		//	条件或消耗参数值	
	INT		nCondition52;		//	条件或消耗参数值	
	LPCSTR  szCondition6Desc;	//	参数说明
	    
	// 参数对
	_DBC_SKILLDATA_V1_DATA_PAIR	paraPair[12];

	LPCSTR Skill_Desc_Interface;

};

/////////////////////////////////////////////////////////////////////
//称号列表						(MissionKind)
/////////////////////////////////////////////////////////////////////
#define DBC_MISSION_KIND			(911)
struct _DBC_MISSION_KIND
{
	INT		nIndex;					// 任务种类的索引
	LPCSTR	szKindName;				// 任务种类的名字
};

/////////////////////////////////////////////////////////////////////
// 表情和颜色转换字符
/////////////////////////////////////////////////////////////////////
#define DBC_STRING_DICT			( 912 )
struct _DBC_STRING_DICT
{
	INT		nID;			// ID
	LPCSTR  szKey;			// 关键字
	LPCSTR  szString;		// 字符串
};


/////////////////////////////////////////////////////////////////////
// 环境音效列表
/////////////////////////////////////////////////////////////////////
#define DBC_ENV_SOUND			( 913 )
struct _DBC_ENV_SOUND
{
	INT		nID;			// ID
	UINT	nSoundID;		// 音效ID
	UINT	nXPos;			// 声源的X坐标
	UINT	nZPos;			// 声源的Z坐标
	UINT	nDistance;		// 能听到声音的最大距离
};

/////////////////////////////////////////////////////////////////////
// 物品规则表
/////////////////////////////////////////////////////////////////////
#define DBC_ITEM_RULE			( 914 )
struct _DBC_ITEM_RULE
{
	INT		nID;			// ID
	INT		bCanDrop;		// 是否可丢弃
	INT		bCanOverLay;	// 是否可重叠
	INT		bCanQuick;		// 是否可放入快捷栏
	INT		bCanSale;		// 是否可以出售给NPC商店
	INT		bCanExchange;	// 是否可以交易
	INT		bCanUse;		// 是否可以使用
	INT		bPickBind;		// 是否是拾取邦定
	INT		bEquipBind;		// 是否是装备邦定
	INT		bUnique;		// 是否是唯一
};

/////////////////////////////////////////////////////////////////////
// 过滤词词汇表
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_FILTER			( 915 )
struct _DBC_TALK_FILTER
{
	INT		nID;			// ID
	LPCSTR	szString;		// 过滤词汇（即：不能说的词汇）
};

/////////////////////////////////////////////////////////////////////
// 等级金钱对应表
/////////////////////////////////////////////////////////////////////
#define DBC_LV_MAXMONEY			( 916 )
struct _DBC_LV_MAXMONEY
{
	INT		nID;			//ID
	INT		nLv;			//等级
	INT		nMaxMoney;		//最大金钱
};

/////////////////////////////////////////////////////////////////////
// 聊天动作命令表
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_ACT			( 918 )
struct _DBC_TALK_ACT
{
	INT		nID;			//ID
	LPCSTR	szCmd;			//命令
	LPCSTR	szNobody;		//没有目标时的字串
	LPCSTR	szMyself;		//目标自己时的字串
	LPCSTR	szOther;		//目标是其他玩家时的字串
	LPCSTR	szAct;			//收到聊天信息时需要做的动作命令串

	LPCSTR	szIconName;		//图标
	LPCSTR	szToolTip;		//提示信息
};

/////////////////////////////////////////////////////////////////////
// 聊天限制配置表
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_CONFIG			( 919 )
struct _DBC_TALK_CONFIG
{
	INT		nID;			//ID 依据ENUM_CHAT_TYPE
	LPCSTR	szChannelName;	//频道名称
	LPCSTR	szChannelHeader;	//频道前导字符
	INT		nTimeSlice;		//时间间隔（单位：秒）
	INT		nNeedType1;		//消耗类型1 依据CHAT_NEED_TYPE
	INT		nNeedValue1;	//消耗数值1
	INT		nNeedType2;		//消耗类型2 依据CHAT_NEED_TYPE
	INT		nNeedValue2;	//消耗数值2
	INT		nNeedType3;		//消耗类型3 依据CHAT_NEED_TYPE
	INT		nNeedValue3;	//消耗数值3
	INT		nNeedLv;		//等级限制，这里标称的是最低多少级才能在此频道发言
};

/////////////////////////////////////////////////////////////////////
// 发型改变消耗表
/////////////////////////////////////////////////////////////////////
#define DBC_HAIR_STYLE			( 920 )
struct _DBC_HAIR_STYLE
{
	INT		nID;			//ID索引
	INT		nRaceID;		//性别区分
	INT		nItemTableID;	//消耗物品ID
	INT		nItemCount;		//消耗物品数量
};

/////////////////////////////////////////////////////////////////////
// 怪物头顶泡泡表
/////////////////////////////////////////////////////////////////////
#define DBC_MONSTER_PAOPAO		( 921 )
struct _DBC_MONSTER_PAOPAO
{
	INT		nID;			//ID索引
	LPCSTR	szPaoPaoTxt;	//怪物要说的头顶文字
};


#define DBC_CAMP_AND_STAND		(922)	//阵营


/////////////////////////////////////////////////////////////////////
//字体信息颜色表
/////////////////////////////////////////////////////////////////////
#define DBC_SYSTEM_COLOR		(923)
struct _DBC_SYSTEM_COLOR
{
	const static INT NAME_PLAYER_MYSELF		 = 1;	//名字_主角
	const static INT NAME_PLAYER_OTHER		 = 2;	//名字_其他玩家
	const static INT NAME_NPC_PEACE			 = 3;	//名字_和平NPC
	const static INT NAME_NPC_ATTACK_ACTIVE	 = 4;	//名字_主动攻击NPC
	const static INT NAME_NPC_ATTACK_PASS	 = 5;	//名字_不主动攻击NPC
	const static INT NAME_NPC_CANNT_ATTACK	 = 6;	//不可攻击的NPC
	const static INT NAME_PET				 = 7;	//宠物名字
	const static INT NANE_SELF_PET			 = 8;	//自己的宠物

	const static INT NANE_ISACK_NOTRETORT_PLAYER	 = 10;	//可以攻击_不还击的玩家
	const static INT NANE_ISACK_ISTRETORT_PLAYER	 = 11;	//可以攻击_会还击的玩家
	const static INT NANE_NOTACK_ISTRETORT_PLAYER	 = 12;	//不可攻击_会还击的玩家
	const static INT NANE_NOTACK_NOTRETORT_PLAYER	 = 13;	//不可攻击_不还击的玩家
	const static INT NANE_ISACK_NOTRETORT_MONSTER	 = 14;	//可攻击、不还击、不主动
	const static INT NANE_ISACK_ISRETORT_MONSTER	 = 15;	//可攻击、会还击、不主动
	const static INT NANE_ISACK_ISACK_MONSTER		 = 16;	//可攻击、会还击、会主动
	const static INT NANE_NOTACK_ISACK_MONSTER		 = 17;	//不可攻击、会还击、会主动
	const static INT NANE_NOTACK_NOTRETORT_MONSTER	 = 18;	//不可攻击、不会还击、不会主动

	const static INT TITLE_NORMAL			 = 100;	//称号_普通
	const static INT TITLE_BANGPAI			 = 101;	//称号_帮派职称
	const static INT TITLE_MOOD				 = 102;	//称号_玩家自定义心情
	const static INT TITLE_PLAYER_SHOP		 = 103;	//称号_玩家商店

	const static INT UI_DEBUGLISTBOX		 = 1000; //debuglistbox的颜色

	INT		m_nID;
	LPCSTR  strDesc;
	LPCSTR	strFontColor;
	LPCSTR	strExtenColor;
};

/////////////////////////////////////////////////////////////////////
// 完全匹配过滤表，用在创建帮会、起名字之类的地方，避免使用系统词组
/////////////////////////////////////////////////////////////////////
#define DBC_FULLCMP_FILTER		( 924 )
struct _DBC_FULLCMP_FILTER
{
	INT		nID;			//ID索引
	LPCSTR	szFilterType;	//用在什么地方，例如：all,guild,team,.....表示用在所有地方，仅帮会，仅组队等等。
	LPCSTR	szFullTxt;		//完全屏蔽词
};

/////////////////////////////////////////////////////////////////////
// 宠物升级需要的经验表
/////////////////////////////////////////////////////////////////////
#define DBC_PET_LEVELUP			( 925 )
struct _DBC_PET_LEVELUP
{
	INT		nID;			//ID索引
	INT		nExp;			//所需要经验
};


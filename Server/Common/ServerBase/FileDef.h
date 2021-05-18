#ifndef __FILEDEF_H__
#define __FILEDEF_H__

// 下面是 INI 部分

#define FILE_CONFIG_INFO					"./Config/ConfigInfo.ini"
#define FILE_LOGIN_INFO						"./Config/LoginInfo.ini"
#define	FILE_WORLD_INFO						"./Config/WorldInfo.ini"
#define FILE_SHARE_MEM_INFO					"./Config/ShareMemInfo.ini"
#define FILE_MACHINE_INFO					"./Config/MachineInfo.ini"
#define FILE_SERVER_INFO					"./Config/ServerInfo.ini"
#define FILE_SCENE_INFO						"./Config/SceneInfo.ini"
#define FILE_COPY_SCENE						"./Config/CopyScene.ini"

#define FILE_BASE_VALUE						"./Config/BaseValueTable.ini"

#define FILE_ITEM_SERIAL					"./Config/ItemSerial.ini"
#define FILE_MONSTER_AI						"../Public/Config/MonsterAITable.ini"
#define FILE_PET_CONFIG						"./Config/PetConfigTable.ini"
#define FILE_GUILD_MEMBER_ACCESS			"./Config/GuildMemberAccess.ini"
#define FILE_IPRANGE						"./Config/IPRange.ini"
#define FILE_GMLIST							"./Config/GMList.ini"

// 下面是 TXT 部分

#define FILE_PLAYER_EXP_LEVEL				"../Public/Config/PlayerExpLevel.txt"
#define FILE_NPC_CHAT						"./Config/NPCChat.txt"
#define FILE_NPCPAOPAOINDEXTBL				"./Config/NPCPaoPaoIndexTable.txt"
#define FILE_MONSTER_ATTREX					"../Public/Config/MonsterAttrExTable.txt"
#define FILE_PET_ATTR						"../Public/Config/PetAttrTable.txt"
#define FILE_PET_ATTRPOINTDISTRIBUTE		"./Config/PetAttrPointDistribute.txt"
#define FILE_PET_LEVELUP					"../Public/Config/PetLevelUpTable.txt"
#define FILE_PET_SKILLDISTRIBUTE			"./Config/PetSkillDistribute.txt"
#define FILE_PET_SKILLINDEXTBL				"./Config/PetSkillIndexTable.txt"
#define FILE_PET_STUDYSKILLTBL				"./Config/PetStudySkllTable.txt"
#define FILE_PET_APPERCEIVESKILLTBL			"./Config/PetApperceiveSkillTable.txt"
#define FILE_PET_DOMESTICATION_MONEY		"./Config/PetDomesticationMoney.txt"
#define	FILE_PET_AISTRATEGYTBL				"./Config/PetAIStrategyTbl.txt"
#define FILE_PET_TRAINCHARGE				"./Config/PetTrainCharge.txt"
#define FILE_EXP_ATTENUATION				"./Config/ExpAttenuation.txt"
#define FILE_GROWPOINT						"../Public/Config/GrowPoint.txt"
#define FILE_PLANTS							"./Config/Plants.txt"
#define FILE_ATTR_LEVEL_UP					"./Config/AttrLevelUpTable.txt"

#define FILE_XINFA_STUDY_SPEND				"../Public/Config/XinFaStudySpend_V1.txt"
#define FILE_XINFA							"../Public/Config/XinFa_V1.txt"
#define FILE_SKILL_TEMPLATE					"../Public/Config/SkillTemplate_V1.txt"
#define FILE_SKILL_DATA						"../Public/Config/SkillData_V1.txt"
#define FILE_SPECIAL_OBJ_DATA				"../Public/Config/SpecialObjData.txt"
#define FILE_IMPACT_SEDATA					"../Public/Config/ImpactSEData_V1.txt"
#define FILE_STANDARD_IMPACT_DATA			"./Config/StandardImpact.txt"
#define FILE_CAMP_AND_STAND					"../Public/Config/CampAndStand.txt"
#define FILE_ID_COLLECTION					"../Public/Config/IDCollections.txt"

#define FILE_ABILITY						"../Public/Config/Ability.txt"
#define FILE_HAIRSTYLE						"../Public/Config/CharHairGeo.txt"
#define FILE_PRESCRIPTION					"../Public/Config/ItemCompound.txt"
#define	FILE_ABILITY_EXP					"../Public/Config/AbilityExpTable.txt"

#define File_WHITE_EQUIP_BASE				"../Public/Config/WhiteEquipBase.txt"
#define FILE_GREEN_EQUIP_BASE				"../Public/Config/GreenEquipBase.txt"
#define FILE_BLUE_EQUIP_BASE				"../Public/Config/BlueEquipBase.txt"
#define FILE_ITEM_QUALITY					"./Config/ItemQuality.txt"
#define FILE_ITEM_VALUE						"./Config/ItemValue.txt"
#define FILE_GEM_INFO						"../Public/Config/GemInfo.txt"
#define FILE_COMMON_ITEM					"../Public/Config/CommonItem.txt"
#define FILE_ITEM_RULE						"../Public/Config/ItemRule.txt"
#define FILE_STORE_MAP_ITEM					"../Public/Config/StoreMapItem.txt"
#define FILE_DROP_BOX_CONTENT				"./Config/DropBoxContent.txt"
#define FILE_MONSTER_DROP_BOXS				"./Config/MonsterDropBoxs.txt"
#define FILE_DROP_ATTENUATION				"./Config/DropAttenuation.txt"
#define FILE_EQUIP_SET_ATTR					"../Public/Config/EquipSetAttr.txt"

#define FILE_SHOP							"../Public/Config/ShopTable.txt"
#define FILE_QUESTION						"./Config/Questions.txt"

#define FILE_DIE_PENALTY					"./Config/DiePenalty.txt"
#define FILE_PLAYER_LEVEL_MONEY				"../Public/Config/LvMaxMoney.txt"
#define FILE_CITY_SCENE						"../Public/Config/CityInfo.txt"
#define FILE_CHAT_CONSUME					"../Public/Config/ChatConfig.txt"

#define FILE_CITY_BUILDING					"../Public/Config/CityBuilding.txt"

//普通任务数据表
#define FILE_MISSION_DATA					"../Public/Config/MissionData.txt"
#define FILE_MISSION_REWARD					"../Public/Config/MissionReward.txt"
#define FILE_MISSION_PUNISH					"../Public/Config/MissionPunish.txt"
#define FILE_MISSION_DIALOG					"../Public/Config/MissionDialog.txt"
#define FILE_MISSION_LIST					"../Public/Config/MissionList.txt"

// 其他

#define FILE_AI_SCRIPT						"../Public/Data/AIScript.dat"
#define FILE_SCRIPT							"../Public/Data/Script.dat"

#define PATH_AI_SCRIPT						"../Public/Data/AIScript/"
#define PATH_SCENE							"../Public/Scene/"

#define GET_FULL_PATH(fullpath, filepath, filename)	do { \
		sprintf(fullpath, "%s%s", (filepath), (filename)); \
	} while (0)

#define GET_AI_SCRIPT_FULL_PATH(fullpath, filename) GET_FULL_PATH(fullpath, PATH_AI_SCRIPT, (filename))
#define GET_SCENE_FULL_PATH(fullpath, filename) GET_FULL_PATH(fullpath, PATH_SCENE, (filename))
#endif // __FILEDEF_H__
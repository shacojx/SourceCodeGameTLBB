#ifndef _SQL_TEMPLATE_H_
#define _SQL_TEMPLATE_H_
#include "Type.h"


#define CHAR_TABLE		"t_char"
#define ITEM_TABLE		"t_iteminfo"
#define SKILL_TABLE		"t_skill"
#define XINFA_TABLE		"t_xinfa"
#define ABILITY_TABLE	"t_ability"
#define TASK_TABLE		"t_mission"
#define RELATION_TABLE	"t_relation"
#define IMPACT_TABLE	"t_impact"
#define PET_TABLE		"t_pet"
#define ISERIAL_TABLE	"t_itemkey"
#define MAIL_TABLE		"t_mail"
#define GUILD_TABLE		"t_guild"
#define SHOP_TABLE		"WX_Shop"



extern CHAR* LoadCharList;

extern CHAR* LoadCharEquipList;

extern CHAR* LoadCharFullData;

extern CHAR* LoadCharItemList;
extern CHAR* DeleteCharItemList;
extern CHAR* NewCharItem;


extern CHAR* LoadCharSkillList;
extern CHAR* DeleteCharSkillList;
extern CHAR* NewCharSkill;

extern CHAR* LoadCharXinFaList;
extern CHAR* DeleteCharXinFaList;
extern CHAR* NewCharXinFa;

extern CHAR* LoadCharAbilityList;
extern CHAR* LoadCharPresList;
extern CHAR* DeleteCharAbilityList;
extern CHAR* NewCharAbility;
extern CHAR* UpdateCharPresList;


extern CHAR* LoadCharTaskDataAndFlag;
extern CHAR* LoadCharTaskList;
extern CHAR* DeleteCharTaskList;
extern CHAR* NewCharTask;
extern CHAR* UpdateCharTaskData;
extern CHAR* UpdateCharTaskFlag;


extern CHAR* LoadCharRelationList;
extern CHAR* LoadCharRelationData;
extern CHAR* DeleteCharRelationList;
extern CHAR* NewCharRelation;
extern CHAR* UpdateCharRelationData;


extern CHAR* LoadCharImpactList;
extern CHAR* DeleteCharImpactList;
extern CHAR* NewCharImpact;

extern CHAR* LoadCharPetList;
extern CHAR* DeleteCharPetList;
extern CHAR* NewCharPet;

extern CHAR* LoadCharPrivateInfo;
extern CHAR* UpdateCharPrivateInfo;

extern CHAR* LoadCharTitleInfo;
extern CHAR* UpdateCharTitleInfo;

extern CHAR* LoadCharCoolDownInfo;
extern CHAR* UpdateCharCoolDownInfo;

extern CHAR* CreateNewChar;
extern CHAR* FetchNewGuid;

extern CHAR* DeleteBaseChar;
extern CHAR* UpdateCharFullData;


extern CHAR* LoadServerItemSerial;
extern CHAR* UpdateServerItemSerial;

extern CHAR* LoadWorldMailInfo;
extern CHAR* SaveWorldMailInfo;
extern CHAR* DeleteWorldMailInfo;

extern CHAR* LoadWorldGuildInfo;
extern CHAR* SaveWorldGuildInfo;
extern CHAR* DeleteWorldGuildInfo;

extern CHAR* LoadWebShopInfo;
extern CHAR* SaveWebShopInfo;

#endif
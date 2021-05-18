
#ifndef _HUMAN_PET_CONTAINER_H_
#define _HUMAN_PET_CONTAINER_H_
#include "ItemContainer.h"
#include "DB_Struct.h"

class HumanDB;
struct _PET_DB_LOAD;

struct _HUMAN_PETCONTAINER_INIT: 
	public _ITEMCONTAINER_INIT
{
	HumanDB*				m_pHumanDB;
	ITEM_CONTAINER_TYPE		m_ICTType;
};

class Obj_Human;

class HumanPetContainer:public ItemContainer
{
	HumanDB*				m_pHumanDB;
	ITEM_CONTAINER_TYPE		m_ICTType;
public:
	virtual		BOOL		Init( const _ITEMCONTAINER_INIT* pInit ) ;//初始化容器
protected :
	//设置物品的属性
	virtual		BOOL		SetItem( const INT nIndex, const Item* pItem );
	//删除物品
	virtual		BOOL		EraseItem(UINT uIndex);
	//设置物品属性
	virtual	BOOL			SetItemValue(INT nIndex,const _PET_DB_LOAD* pPet);


	virtual BOOL  			SetPetGUID(INT nIndex,PET_GUID_t GUID) ;				//设置宠物GUID
	virtual BOOL  			SetSpouseGUID(INT nIndex,PET_GUID_t GUID) ;			//设置宠物配偶GUID
	virtual BOOL			SetDataID(INT nIndex,INT ID);							//设置宠物模型		
	virtual BOOL 			SetName(INT nIndex, const CHAR* pName);						//设置名字
	virtual BOOL   			SetNick(INT nIndex, const CHAR* pNick);						//设置昵称
	virtual BOOL  			SetLevel(INT nIndex,INT level);							//设置等级
	virtual BOOL  			SetTakeLevel(INT nIndex,INT takeLevel);					//设置携带等级
	virtual BOOL  			SetAttackType(INT nIndex,INT attackType);				//设置进攻类型（物/法）
	virtual BOOL  			SetAIType(INT nIndex,INT AIType);						//设置AI类型
	virtual BOOL  			SetCampData(INT nIndex,const _CAMP_DATA* pCamp);		//设置阵营
	virtual BOOL  			SetHP(INT nIndex,INT hp);								//设置生命值
	virtual BOOL  			SetLife(INT nIndex,INT Life);							//设置当前寿命
	virtual BOOL  			SetPetType(INT nIndex,BYTE PetType);					//宝宝，变异，野生
	virtual BOOL  			SetGeneration(INT nIndex,BYTE Gen);						//几代宠
	virtual BOOL  			SetHappiness(INT nIndex,BYTE byHappiness);						//快乐度		
	virtual BOOL  			SetStrPer(INT nIndex,INT strper);						//力量资质	
	virtual BOOL  			SetConPer(INT nIndex,INT conper);						//体力资质	
	virtual BOOL   			SetDexPer(INT nIndex,INT dexper);						//身法资质
	virtual BOOL  			SetSprPer(INT nIndex,INT sprper);						//灵气资质
	virtual BOOL   			SetIntPer(INT nIndex,INT intper);						//定力资质
	virtual BOOL			SetGenGu(INT nIndex,INT gengu);							//根骨
	virtual BOOL  			SetGrowRate(INT nIndex,FLOAT rate);						//成长率
	virtual BOOL  			SetRemainPoint(INT nIndex,INT rPoint);					//一级属性剩余点数
	virtual BOOL  			SetExp(INT nIndex,INT exp) ;							//经验值
	virtual BOOL  			SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value);//基础一级战斗属性（不包括技能和装备增加的部分）
	virtual BOOL  			SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill);	//宠物技能
 
private:
	BOOL					SetDBDirty();
};



#endif
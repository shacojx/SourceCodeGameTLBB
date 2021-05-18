/////////////////////////////////////////////////////////////////////////////////
//文件名：ItemContainer.h
//功能描述：物品容器的封装类，通过此类来读取及设置多个物品的相关信息
//修改记录：2005-11-2创建
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__

#include "Type.h"
#include "Item.h"

#define MAX_ITEMCONTAINER_SIZE 128

struct _ITEMCONTAINER_INIT
{
	INT						m_nContainerSize ;//容器大小
	
};



class ItemContainer
{
friend class ItemOperator ;
	//ItemContainerType
	
public :
	ItemContainer( ) ;
	virtual ~ItemContainer( ) ;

	/*
	 *	ITEM 使用方法
	 */

	//通过物品的GUID取得此物品在容器中的位置
	INT					GetIndexByGUID( const _ITEM_GUID* pGuid ) ;
	//通过宠物的GUID取得此物品在容器中的位置
	INT					GetIndexByGUID( const PET_GUID_t* pGuid ) ;
	//通过物品的类型取得此物品在容器中的位置
	INT					GetIndexByType( const UINT uType ) ;
	//通过物品的类型取得此物品在容器中的位置
	INT					GetIndexByType( const UINT uType ,UINT emptyCount) ;

	//通过容器的位置取得物品
	Item*				GetItem( const INT nIndex ) ;

	

	
	//取得一个没满的物品位置,如果INVALID_INDEX 则表示没找到
	INT					GetNoFullItemIndex( UINT uItemIndex);
	//容器编号到Bag编号
	virtual		UINT	ConIndex2BagIndex(UINT uConIndex){return uConIndex;}
	//背包编号到容器编号
	virtual		UINT	BagIndex2ConIndex(UINT uBagIndex){return uBagIndex;}
	//测试一个nIndex是不是在这个Container中
	virtual		BOOL	IsInContainer(UINT uBagIndex);

	
public:
	/*
	 *	PET 和 ITEM 公用方法
	 */

	virtual BOOL		Init( const _ITEMCONTAINER_INIT* pInit ) ;//初始化容器

	VOID				CleanUp( ) ;//清除容器内的所有物品数据
	//计算容器空格个数
	virtual		UINT	CalcItemSpace();	
	//交易中计算容器空格个数
	//交易时的物品被交易锁锁住，计算空间时忽略被交易锁锁住的物品，这样物品就可以交换了。
	UINT				CalcExchangeItemSpace();	
	//取得一个空的物品位置, 如果返回INVALID_INDEX表明满了
	INT					GetEmptyItemIndex( ) ;
	//取得容器大小
	INT					GetContainerSize( ){ return m_nContainerSize ; }

	BOOL				SetItemPWLock(INT nIndex,BOOL bLock);

public:
	/*
	 *	PET 使用方法
	 */

protected :
	//将物品设置到对应容器位置
	virtual BOOL		SetItem( const INT nIndex, const Item* pItem );

	//改变Container 的大小
	BOOL				ChangeSize(const INT nSize);

	//设置物品重叠数量
	virtual VOID		SetItemLayCount(INT nIndex, INT nCount) ;
	//减少物品重叠数量
	virtual BOOL		DecItemLayCount(INT nIndex, INT nCount=1) ;
	//增加物品重叠数量
	virtual BOOL		IncItemLayCount(INT nIndex, INT nCount=1);
	//删除物品
	virtual	BOOL		EraseItem(UINT uIndex);
	//设置物品耐久度
	virtual	BOOL		SetItemDur(INT nIndex, INT nDur);
	//设置物品损伤点
	virtual	BOOL		SetItemDamagePoint(INT nIndex, INT nPoint);
	//设置物品耐久度
	virtual	BOOL		SetItemMaxDur(INT nIndex, INT nDur);
	//设置物品属性
	virtual	BOOL		SetItemValue(INT nIndex,_ITEM*	pItem);
	//设置物品帮定
	virtual	BOOL		SetItemBind(INT nIndex);
	//设置物品鉴定信息
	virtual BOOL		SetItemIdent(INT nIndex);
	//设置物品修理次数
	virtual BOOL		SetItemFailTimes(INT nIndex, INT nTimes);

	//增加物品属性
	virtual BOOL		AddItemAttr(INT nIndex,_ITEM_ATTR iA);
	//删除物品属性
	virtual	BOOL		DelItemAttr(INT nIndex,_ITEM_ATTR iA);
	//删除宝石信息
	virtual BOOL		DelGemInfo(INT nIndex,INT GemIndex);
	//添加宝石信息
	virtual	BOOL		AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType);
	//设置物品GUID
	virtual BOOL		SetItemGuid(INT nIndex,_ITEM_GUID* pGUID);
	//设置物品GUID
	virtual BOOL		SetItemGuid(INT nIndex,PET_GUID_t* pGUID);
	//创造者
	virtual	BOOL		SetItemCreator(INT nIndex,const CHAR* CreatorName);
	//物品参数
	virtual BOOL		SetItemParam(INT nIndex,UINT start,INT type,INT value);
	
	//设置PET 属性
	//设置物品属性
	virtual	BOOL		SetItemValue(INT nIndex,const _PET_DB_LOAD*	pPet);



	virtual BOOL  		SetPetGUID(INT nIndex,PET_GUID_t GUID) ;				//设置宠物GUID
	virtual BOOL  		SetSpouseGUID(INT nIndex,PET_GUID_t GUID) ;				//设置宠物配偶GUID
	virtual BOOL		SetDataID(INT nIndex,INT ID);							//设置宠物模型		
	virtual BOOL 		SetName(INT nIndex,const CHAR* pName);					//设置名字
	virtual BOOL   		SetNick(INT nIndex,const CHAR* pNick);					//设置昵称
	virtual BOOL  		SetLevel(INT nIndex,INT level);							//设置等级
	virtual BOOL  		SetTakeLevel(INT nIndex,INT takeLevel);					//设置携带等级
	virtual BOOL  		SetAttackType(INT nIndex,INT attackType);				//设置进攻类型（物/法）
	virtual BOOL  		SetAIType(INT nIndex,INT AIType);						//设置AI类型
	virtual BOOL  		SetCampData(INT nIndex,const _CAMP_DATA* pCamp);		//设置阵营
	virtual BOOL  		SetHP(INT nIndex,INT hp);								//设置生命值
	virtual BOOL  		SetLife(INT nIndex,INT Life);							//设置当前寿命
	virtual BOOL  		SetPetType(INT nIndex,BYTE PetType);					//宝宝，变异，野生
	virtual BOOL  		SetGeneration(INT nIndex,BYTE Gen);						//几代宠
	virtual BOOL  		SetHappiness(INT nIndex,BYTE byHappiness);				//快乐度		
	virtual BOOL  		SetStrPer(INT nIndex,INT strper);						//力量资质	
	virtual BOOL  		SetConPer(INT nIndex,INT conper);						//体力资质	
	virtual BOOL   		SetDexPer(INT nIndex,INT dexper);						//身法资质
	virtual BOOL  		SetSprPer(INT nIndex,INT sprper);						//灵气资质
	virtual BOOL   		SetIntPer(INT nIndex,INT intper);						//定力资质
	virtual BOOL		SetGenGu(INT nIndex,INT gengu);							//根骨
	virtual BOOL  		SetGrowRate(INT nIndex,FLOAT rate);						//成长率
	virtual BOOL  		SetRemainPoint(INT nIndex,INT rPoint);					//一级属性剩余点数
	virtual BOOL  		SetExp(INT nIndex,INT exp) ;							//经验值
	virtual BOOL  		SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value);//基础一级战斗属性（不包括技能和装备增加的部分）
	virtual BOOL  		SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill);	//宠物技能
	
	INT					m_nContainerSize ;					//容器大小
private :
	
	
	Item				m_aItem[MAX_ITEMCONTAINER_SIZE] ;	//容器内物品空间
	
};

#endif

/////////////////////////////////////////////////////////////////////////////////
//文件名：ItemOperator.h
//功能描述：物品及物品容器的操作封装类，通过此类来处理两个容器间物品的转移，销毁
//			生成、以及设置物品数据等功能。
//修改记录：2005-11-2创建
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __ITEMOPERATOR_H__
#define __ITEMOPERATOR_H__

#include "Type.h"
#include "ItemContainer.h"
#include "DB_Struct.h"


enum ITEM_OPERATOR_ERROR//此枚举结构内的数据必须都为负数
{
	ITEMOE_SUCCESS				=  0,	//成功
	ITEMOE_UNKNOW				= -1 ,	//未知的异常错误
	ITEMOE_DESTOPERATOR_HASITEM = -2 ,  //容器的目标位置已经有物品了
	ITEMOE_SOUROPERATOR_LOCK	= -3 ,	//源容器中的物品被上锁了
	ITEMOE_DESTOPERATOR_LOCK	= -4 ,	//目标容器中的物品被上锁了
	ITEMOE_DESTOPERATOR_FULL	= -5 ,	//目标容器已经满了
	ITEMOE_SOUROPERATOR_EMPTY	= -6 ,	//源容器里的物品是空的
	ITEMOE_DESTOPERATOR_EMPTY   = -7 ,	//目标容器的物品是空的
	ITEMOE_CREATEITEM_FAIL		= -8,	//物品产生失败
	ITEMOE_CREATEGUID_FAIL		= -9,	//物品GUID创建失败
	ITEMOE_DIFF_ITEM_DATA		= -10	//不同类型物品数据不能操作

};


struct _ITEMOPERATOR_INIT
{

};

struct ITEM_LOG_PARAM;

struct ItemBoxContaner;

class ItemOperator
{
public :
	ItemOperator( ) ;
	~ItemOperator( ) ;

	BOOL				Init( _ITEMOPERATOR_INIT* pInit ) ;//初始化物品操作系统

	VOID				CleanUp( ) ;//清除

public :
	//////////////////////////////
	//在两个物品容器间的操作接口//
	//////////////////////////////



	/*
	 *	在容器pDestContainer中创建一件物品,物品类型根据uItemIndex,uQuality 来规定
	 *  uQuality 
	 *  如果已经有此类物品则进行叠加,否则物品的位置放在空位置上
	 *  
	 *	uConIndex 最后物品放置的位置
	 *
	 *	此操作产生一个具有唯一(GUID) 的新物品
	 */

	BOOL				CreateItem(ITEM_LOG_PARAM*	pLogParam,
								   UINT uItemIndex,
								   UINT	uQuality,
								   ItemContainer*	pDestContainer,
								   BOOL&			bNewEmptyPos,
								   UINT&			uConIndex,			
								   BYTE				nCoinType,
								   INT nDestIndex = INVALID_INDEX);



	


	/*
	 *	物品掉落中初始化ItemBox 的一种方式
	 *	将pSourceContainer 中 nSourceIndex 位置的物品
	 *  copy 到 pDestContainer 中 nDestIndex 位置
	 *	如果nDestIndex 未指定则寻找一个空格,或者未满的格
	 */

	INT					CopyItem(ItemBoxContaner*	pSourceContainer,
								 INT				nSourceIndex,
								 BOOL&				bNewPos,
								 ItemContainer*		pDestContainer,
								 INT	nDestIndex	=INVALID_INDEX);
								

	


	//将pSourContainer容器内的nSourIndex位置的物品转移到pDestContainer容器内的
	//nDestIndex位置
	//如果nDestIndex的值为INVALID_INDEX，则将物品转移到pDestContainer容器内的随
	//机找到的一个空位置上
	//返回值：执行成功，则返回物品在pDestContainer容器内的位置
	//		  执行失败，则返回ITEM_OPERATOR_ERROR中描述的值
	//注意：此操作不会生成新的物品数据
	INT					MoveItem( ItemContainer* pSourContainer,
							      INT			 nSourIndex ,
								  BOOL&			 bNewPos,
								  ItemContainer* pDestContainer,
								  INT			 nDestIndex=INVALID_INDEX ) ;

	//将pSourContainer容器内的nSourIndex位置的物品转移到pSourContainer容器内的
	//nDestIndex位置
	//返回值：执行成功，则返回物品在pSourContainer容器内的位置
	//		  执行失败，则返回ITEM_OPERATOR_ERROR中描述的值
	//注意：此操作不会生成新的物品数据
	//不应该写Log 容器内的移动
	INT					MoveItem( ItemContainer* pSourContainer,
								  INT nSourIndex,
								  INT nDestIndex);



	
	
	

	//拆分物品操作
	//注意:此操作可能产生新的GUID
	INT					SplitItem(ITEM_LOG_PARAM* pLogParam,
								  ItemContainer* pSourContainer,	//原容器
								  UCHAR uSourIndex,					//原容器中位置
								  UCHAR uCount,						//分离个数
								  ItemContainer* pDestContainer,	//目标容器
                                  INT nDestIndex =INVALID_INDEX );  //目标位置

	//合并物品操作
	//注意:此操作不产生新的GUID
	INT					SpliceItem(	ItemContainer* pSourContainer,	//原容器
									UCHAR uSourIndex,				//原容器中位置
									ItemContainer* pDestContainer,	//目标容器
									INT nDestIndex  );				//目标位置

	
	//合并物品操作
	//注意:此操作不产生新的GUID
	INT					MoveSpliceItem(ItemContainer* pSourContainer,	//原容器
									UCHAR uSourIndex,					//原容器中位置
									ItemContainer* pDestContainer,		//目标容器
									INT nDestIndex);					//目标位置




	


public :
	////////////////
	//操作物品接口//
	////////////////

	
	//设置物品重叠数量
	VOID	SetItemLayCount(ItemContainer* pContainer, INT nIndex, INT nCount) ;
	//减少物品重叠数量
	BOOL	DecItemLayCount(ItemContainer* pContainer,INT nIndex, INT nCount=1) ;
	//增加物品重叠数量
	BOOL	IncItemLayCount(ItemContainer* pContainer,INT nIndex, INT nCount=1);
	//设置物品耐久度
	BOOL	SetItemDur(ItemContainer*	pContainer,INT nIndex, INT nDur);
	//设置物品鉴定属性
	BOOL	SetItemIdent(ItemContainer*	pContainer,INT nIndex);
	//设置物品损伤度
    BOOL	SetItemDamagePoint(ItemContainer*	pContainer,INT nIndex, INT nPoint);
	//设置物品最大耐久度
	BOOL	SetItemMaxDur(ItemContainer*	pContainer,INT nIndex, INT nDur);
	//设置物品绑定属性
	BOOL	SetItemBind(ItemContainer*	pContainer,INT nIndex);
	//设置物品修理次数
	BOOL	SetItemFailTimes(ItemContainer*	pContainer,INT nIndex, INT nTimes);
	//设置物品是否受密码保护
	BOOL	SetItemPWLock(ItemContainer*	pContainer,INT nIndex, BOOL bLock);
	//设置物品制造者信息
	BOOL	SetItemCreator(ItemContainer* pContainer,INT nIndex,const CHAR* CreatorName);
	//设置物品参数
	BOOL	SetItemParam(ItemContainer* pContainer,INT nIndex,UINT start,INT type,INT value);

	//增加物品属性
	BOOL			AddItemAttr(ItemContainer*	pContainer,INT nIndex,_ITEM_ATTR	iA);
	//删除物品属性
	BOOL			DelItemAttr(ItemContainer*	pContainer,INT nIndex,_ITEM_ATTR	iA);
	//删除宝石信息
	BOOL			DelGemInfo(ItemContainer* pContainer,INT nIndex,INT GemIndex);
	//添加宝石信息
	BOOL			AddGemInfo(ItemContainer* pContainer,INT nIndex,INT& GemIndex,UINT GemType);

	////////////////
	//宠物操作接口//
	////////////////

	BOOL						SetItemValue(ItemContainer*	pContainer,INT nIndex,const _PET_DB_LOAD*	pPet);
	
	BOOL  						SetPetGUID(ItemContainer*	pContainer,INT nIndex,PET_GUID_t GUID) ;				//设置宠物GUID
	BOOL  						SetSpouseGUID(ItemContainer*	pContainer,INT nIndex,PET_GUID_t GUID) ;										//设置宠物配偶GUID
	BOOL						SetDataID(ItemContainer*	pContainer,INT nIndex,INT ID);							//设置宠物模型		
	BOOL 						SetName(ItemContainer*	pContainer,INT nIndex,const CHAR* pName);						//设置名字
	BOOL   						SetNick(ItemContainer*	pContainer,INT nIndex,const CHAR* pNick);						//设置昵称
	BOOL  						SetLevel(ItemContainer*	pContainer,INT nIndex,INT level);						//设置等级
	BOOL  						SetTakeLevel(ItemContainer*	pContainer,INT nIndex,INT takeLevel);				//设置携带等级
	BOOL  						SetAttackType(ItemContainer*	pContainer,INT nIndex,INT attackType);				//设置进攻类型（物/法）
	BOOL  						SetAIType(ItemContainer*	pContainer,INT nIndex,INT AIType);						//设置AI类型
	BOOL  						SetCampData(ItemContainer*	pContainer,INT nIndex,const _CAMP_DATA* pCamp);		//设置阵营
	BOOL  						SetHP(ItemContainer*	pContainer,INT nIndex,INT hp);								//设置生命值
	BOOL  						SetLife(ItemContainer*	pContainer,INT nIndex,INT Life);							//设置当前寿命
	BOOL  						SetPetType(ItemContainer*	pContainer,INT nIndex,BYTE PetType);					//宝宝，变异，野生
	BOOL  						SetGeneration(ItemContainer*	pContainer,INT nIndex,BYTE Gen);					//几代宠
	BOOL  						SetHappiness(ItemContainer*	pContainer,INT nIndex,BYTE byHappiness);						//快乐度		
	BOOL  						SetStrPer(ItemContainer*	pContainer,INT nIndex,INT strper);						//力量资质	
	BOOL  						SetConPer(ItemContainer*	pContainer,INT nIndex,INT conper);						//体力资质	
	BOOL   						SetDexPer(ItemContainer*	pContainer,INT nIndex,INT dexper);						//身法资质
	BOOL  						SetSprPer(ItemContainer*	pContainer,INT nIndex,INT sprper);						//灵气资质
	BOOL   						SetIntPer(ItemContainer*	pContainer,INT nIndex,INT intper);						//定力资质
	BOOL						SetGenGu(ItemContainer*	pContainer,INT nIndex,INT gengu);						//根骨
	BOOL  						SetGrowRate(ItemContainer*	pContainer,INT nIndex,FLOAT rate);					//成长率
	BOOL  						SetRemainPoint(ItemContainer*	pContainer,INT nIndex,INT rPoint);					//一级属性剩余点数
	BOOL  						SetExp(ItemContainer*	pContainer,INT nIndex,INT exp) ;						//经验值
	BOOL  						SetLvl1Attr(ItemContainer*	pContainer,INT nIndex,CHAR_ATTR_LEVEL1 type,INT value);//基础一级战斗属性（不包括技能和装备增加的部分）
	BOOL  						SetSkill(ItemContainer*	pContainer,INT nIndex,UINT SkillIndex,_PET_SKILL skill);	 //宠物技能

public:
	/*
	 *	ITEM 和 PET 公共操作接口
	 */
	//锁定容器pContainer内nIndex位置的物品
	VOID	LockItem(ItemContainer* pContainer, INT nIndex ) ;
	//解锁容器pContainer内nIndex位置的物品
	VOID	UnlockItem(ItemContainer* pContainer, INT nIndex ) ;
	

	//在容器pDestContainer的nDestIndex位置上生成一件物品，此物品与pTemplateItem具有
	//相同的逻辑数据及属性，但GUID不同。
	//如果nDestIndex的值为INVALID_INDEX，则将物品生成到pDestContainer容器内随机找到
	//的一个空位置上
	//返回值：执行成功，则返回物品在pDestContainer容器内的位置
	//		  执行失败，则返回ITEM_OPERATOR_ERROR中描述的值
	//
	//注意：此操作会生成一个具有唯一标识（GUID）的新物品
	//
	INT					CreateItem( ITEM_LOG_PARAM*	pLogParam,
									Item* pTemplateItem ,
									ItemContainer* pDestContainer,
									INT nDestIndex=INVALID_INDEX ) ;

	//删除物品操作
	//删除 pDestContainer 中与uIndex位置的物品
	//注意:此操作会销毁目标容器的uIndex 位置的物品
	BOOL				EraseItem(	ItemContainer* pDestContainer, 
									UCHAR uIndex );
	

	//将pSourContainer容器内的nSourIndex位置的物品与pDestContainer容器内的
	//nDestIndex位置的物品进行交换操作
	//返回值：执行成功，则返回物品在pDestContainer容器内的位置
	//		  执行失败，则返回ITEM_OPERATOR_ERROR中描述的值
	//注意：此操作不会生成新的物品数据
	INT					ExchangeItem( ItemContainer* pSourContainer,
									  INT nSourIndex ,
									  ItemContainer* pDestContainer,
									  INT nDestIndex ) ;
	

	//将pSourContainer容器内的nSourIndex位置的物品复制到pDestContainer容器内的
	//nDestIndex位置
	//如果nDestIndex的值为INVALID_INDEX，则将物品复制到pDestContainer容器内的随
	//机找到的一个空位置上
	//返回值：执行成功，则返回物品在pDestContainer容器内的位置
	//		  执行失败，则返回ITEM_OPERATOR_ERROR中描述的值
	//
	//注意：此操作会复制出一份物品,新旧物品有相同的GUID
	//
	INT					CopyItem( ItemContainer* pSourContainer,
								  INT			 nSourIndex ,
								  ItemContainer* pDestContainer,
								  INT nDestIndex = INVALID_INDEX ) ;


	//将pSourContainer容器内的nSourIndex位置的物品转移到pDestContainer容器内的
	//nDestIndex位置
	//如果nDestIndex的值为INVALID_INDEX，则将物品转移到pDestContainer容器内的随
	//机找到的一个空位置上
	//返回值：执行成功，则返回物品在pDestContainer容器内的位置
	//		  执行失败，则返回ITEM_OPERATOR_ERROR中描述的值
	//注意：此操作不会生成新的物品数据
	INT					MoveItem( ItemContainer* pSourContainer,
								  INT nSourIndex ,
								  ItemContainer* pDestContainer,
								  INT nDestIndex=INVALID_INDEX ) ;


public:

	/*
	 *	PET 专用的操作接口
	 */
	
	

protected :
	//内部操作函数

	//设置物品的GUID
	VOID			SetItemGUID( ItemContainer* pContainer, INT nIndex, _ITEM_GUID* pGuid ) ;
	VOID			SetItemGUID( ItemContainer* pContainer, INT nIndex, PET_GUID_t* pGuid ) ;
	
	
	BOOL			GenItemGUID(_ITEM_GUID& ig) ;//创造一个新的GUID
	BOOL			GenItemGUID(PET_GUID_t& ig) ;//创造一个新的GUID


	
};

extern ItemOperator	g_ItemOperator;





#endif

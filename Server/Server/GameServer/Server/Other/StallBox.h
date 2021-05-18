#ifndef _STALL_BOX_H_
#define _STALL_BOX_H_

#include "Type.h"
#include "ItemContainer.h"
#include "BBS.h"
class Obj_Human;
class StallInfoManager;
struct _ITEM;
/*
服务器端摊位盒结构.
*/
class ServerStallBox
{
public:
	enum STALL_STATUS
	{
		STALL_CLOSE,
		STALL_READY,
		STALL_OPEN,
	};
public:
	ServerStallBox()
	{
		m_pMySelf = NULL;
	}
	~ServerStallBox()
	{
	}

public:
	/*
	初始化自己摊位盒
	*/
	VOID Init(Obj_Human* pHuman);

	/*
	清空
	*/
	VOID CleanUp();

	/*
	摊主自己,一旦初始化完不会再改
	*/
	Obj_Human*	GetMySelf(){return m_pMySelf;}

	/*
	结所有锁
	*/
	VOID	UnLockAllItem();

	/*
	位置释放
	*/
	VOID	UnLockMapPos();
public:

	BYTE	GetStallStatus(){ return m_StallStatus; }
	VOID	SetStallStatus(BYTE StallStatus){m_StallStatus = StallStatus;}

	BOOL	GetStallIsOpen(){ return m_IsOpen; }
	VOID	SetStallIsOpen(BOOL IsOpen){m_IsOpen = IsOpen;}

	UINT	GetPosTax(){ return m_PosTax; }
	VOID	SetPosTax(UINT PosTax){ m_PosTax = PosTax; }

	UINT	GetTradeTax(){ return m_TradeTax; }
	VOID	SetTradeTax(UINT TradeTax){ m_TradeTax = TradeTax; }

	CHAR*	GetStallName(){ return m_szStallName; }
	VOID	SetStallName(const CHAR *pszTitle, BYTE nSize );	

	UINT	GetFirstPage(){return m_nFirstPage;}
	VOID	SetFirstPage(UINT nFirstPage){m_nFirstPage = nFirstPage;}

	//物品信息处理
	UINT	GetSerialByIndex(UINT index)
	{
		if(index>=0 && index<STALL_BOX_SIZE)
			return m_ItemSerial[index];
		else
			return 0;
	}
	VOID	SetSerialByIndex(UINT index, UINT nSerial)
	{
		if(index>=0 && index<STALL_BOX_SIZE)
			m_ItemSerial[index] = nSerial;
	}
	VOID	IncSerialByIndex(UINT index)
	{
		if(index>=0 && index<STALL_BOX_SIZE)
			m_ItemSerial[index] += m_SerialIncStep;
	}

	VOID	SetSerialIncStep(UINT SerialIncStep) {m_SerialIncStep = SerialIncStep;}
	UINT	GetSerialIncStep() {return m_SerialIncStep;}

	UINT	GetPriceByIndex(UINT index)
	{
		if(index>=0 && index<STALL_BOX_SIZE)
			return m_ItemPrice[index];
		else
			return 0;
	}
	VOID	SetPriceByIndex(UINT index, UINT nPrice)
	{
		if(index>=0 && index<STALL_BOX_SIZE)
			m_ItemPrice[index] = nPrice;
	}

	//宠物信息处理
	UINT	GetPetSerialByIndex(UINT index)
	{
		if(index>=0 && index<STALL_PET_BOX_SIZE)
			return m_PetSerial[index];
		else
			return 0;
	}

	VOID	SetPetSerialByIndex(UINT index, UINT nSerial)
	{
		if(index>=0 && index<STALL_PET_BOX_SIZE)
			m_PetSerial[index] = nSerial;
	}

	VOID	IncPetSerialByIndex(UINT index)
	{
		if(index>=0 && index<STALL_PET_BOX_SIZE)
			m_PetSerial[index] += m_SerialIncStep;
	}

	UINT	GetPetPriceByIndex(UINT index)
	{
		if(index>=0 && index<STALL_PET_BOX_SIZE)
			return m_PetPrice[index];
		else
			return 0;
	}

	VOID	SetPetPriceByIndex(UINT index, UINT nPrice)
	{
		if(index>=0 && index<STALL_PET_BOX_SIZE)
			m_PetPrice[index] = nPrice;
	}

	VOID	SetStallPos(UINT Pos_x, UINT Pos_z)
	{
		m_PosX	=	Pos_x;	
		m_PosZ	=	Pos_z;	
	}

	ItemContainer*	GetContainer() {return &m_Container;}

	ItemContainer*	GetPetContainer() {return &m_PetContainer;}

	ServerBBS*		GetBBS()	{return &m_StallBBS;}
private:
	Obj_Human*		m_pMySelf;				//指向自己的指针，用这个可以找到自己身上的东西
	BOOL			m_IsOpen;				//摊位是否已经打开
	BYTE			m_StallStatus;			//当前摊位是否在被打开
	UINT			m_PosTax;				//摊位费
	UINT			m_TradeTax;				//每次交易税收
	CHAR			m_szStallName[MAX_STALL_NAME];			//标示摊位名
	UINT			m_nFirstPage;				//起始页
	UINT			m_ItemSerial[STALL_BOX_SIZE];//每个商品的序列号
	UINT			m_ItemPrice[STALL_BOX_SIZE];//每个商品的价格

	ItemContainer	m_Container;					//CONTAINER
	_ITEM			m_ItemList[STALL_BOX_SIZE];		//物品的真实数据

	UINT			m_PetSerial[STALL_PET_BOX_SIZE];//每个宠物的序列号
	UINT			m_PetPrice[STALL_PET_BOX_SIZE];	//每个宠物的价格

	ItemContainer	m_PetContainer;						//摊位盒中的宠物容器
	_PET_DB_LOAD	m_PetItemList[STALL_PET_BOX_SIZE];	//宠物的真实数据

	UINT			m_SerialIncStep;		//序列号递增步长		
	ServerBBS		m_StallBBS;				//摊位留言板

	StallInfoManager* m_pStallInfoMgr;			//摊位信息管理器
	UINT			m_PosX;					//摊位的位置记录下来,供CleanUp使用
	UINT			m_PosZ;					//摊位的位置记录下来,供CleanUp使用

};

#endif
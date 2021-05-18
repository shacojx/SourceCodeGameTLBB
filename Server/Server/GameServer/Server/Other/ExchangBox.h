/*
服务器段交易盒
*/

#ifndef _EXCHANGE_BOX_H_
#define _EXCHANGE_BOX_H_

#include "Type.h"
#include "DB_Struct.h"
#include "HumanPetContainer.h"

class Obj_Human;

/*
验证当前人物的交易状态是否已经上锁.
*/
#define	EXCHANGE_CERTIFY_ISLOCK(pHuman)\
		if(pHuman->m_ExchangBox.m_IsLocked == TRUE)\
		{\
			GCExchangeError Msg;\
			Msg.SetID(EXCHANGE_MSG::ERR_ALREADY_LOCKED);\
			pHuman->GetPlayer()->SendPacket(&Msg);\
			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 已锁定交易窗口，操作无效 ", pHuman->GetName() ) ;\
			return PACKET_EXE_CONTINUE;\
		}\

/*
验证交易双方是否掉线合法
*/
#define EXCHANGE_CERTIFY_EACH_OTHER(pHuman)\
		ObjID_t	TargetID = pHuman->m_ExchangBox.m_ObjID;\
		Obj_Human* pTargetHuman = pScene->GetHumanManager()->GetHuman( TargetID );\
		if( pTargetHuman == NULL )\
		{\
			GCExchangeError Msg;\
			Msg.SetID(EXCHANGE_MSG::ERR_DROP);\
			pHuman->GetPlayer()->SendPacket(&Msg);\
			pHuman->m_ExchangBox.CleanUp();\
			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 已掉线，操作无效 ", pHuman->GetName() ) ;\
			return PACKET_EXE_CONTINUE;\
		}\
		ObjID_t	SourceID = pTargetHuman->m_ExchangBox.m_ObjID;\
		if( SourceID != pHuman->GetID())\
		{\
			GCExchangeError Msg;\
			Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);\
			pHuman->GetPlayer()->SendPacket(&Msg);\
			pHuman->m_ExchangBox.CleanUp();\
			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 在与其他人交易，操作无效 ", pTargetHuman->GetName() ) ;\
			return PACKET_EXE_CONTINUE;\
		}\

/*
验证当前人物的交易状态是否正确.
*/
#define	EXCHANGE_CERTIFY_STATUS(pHuman, status)\
		if(pHuman->m_ExchangBox.m_Status != ServerExchangeBox::status)\
		{\
			GCExchangeError Msg;\
			Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);\
			pHuman->GetPlayer()->SendPacket(&Msg);\
			ObjID_t	DestID = pHuman->m_ExchangBox.m_ObjID;\
			Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( DestID );\
			pHuman->m_ExchangBox.CleanUp();\
			pDestHuman->m_ExchangBox.CleanUp();\
			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 交易状态出错 status = %d ", pHuman->GetName(), pHuman->m_ExchangBox.m_Status );\
			return PACKET_EXE_CONTINUE;\
		}\

/*
服务器端交易盒结构.
*/
class ServerExchangeBox
{
public:
	enum ExchangeStatus
	{
		EXCHANGE_NONE = 0,										//没有交易发生
		EXCHANGE_SYNCH_DATA,									//自己处于同步数据阶段。
		EXCHANGE_WAIT_FOR_CONFIRM,								//自己处于等待自己最后确认阶段。
		EXCHANGE_CONFIRM_READY,									//自己处于等待对方确认阶段
	};	
public:
	ServerExchangeBox()
	{
		m_pMySelf = NULL;
	}
	VOID Init(Obj_Human* pHuman);
	VOID CleanUp();
	VOID UnLockAllItem();
public:
	ExchangeStatus		m_Status;								//交易状态
	ObjID_t				m_ObjID;								//当前交易对象
	Obj_Human* 			m_pMySelf;								//自己
	BOOL				m_IsLocked;								//标示界面上lock选项是否钩上
	BOOL				m_CanConform;							//标示是否显示确定按钮
	UINT				m_Money;								//标示交易的金钱
	ItemContainer		m_Container;							//交易盒中的物品
	_ITEM				m_ItemList[EXCHANGE_BOX_SIZE];			//物品的真实数据
	ItemContainer		m_PetContainer;							//交易盒中的宠物容器
	_PET_DB_LOAD		m_PetItemList[EXCHANGE_PET_BOX_SIZE];	//宠物的真实数据
	
};
#endif
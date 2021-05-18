#include "stdafx.h"


#include "CGBankMoney.h"
#include "GCBankMoney.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGBankMoneyHandler::Execute( CGBankMoney* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	BYTE	IsSave = pPacket->GetSaveType();
	INT		Amount = pPacket->GetAmount();
	INT		AmountRMB = pPacket->GetAmountRMB();

	INT CurMoney = pHuman->__GetBankMoney();
	INT CurRMB = pHuman->GetBankRMB();

	if(IsSave & CGBankMoney::SAVE_MONEY)
	{
		if( Amount>(INT)(pHuman->GetMoney()) )
		{
			Amount = pHuman->GetMoney();
		}
		CurMoney += Amount;
		//将来要判一下是否大于银行能存的最大上限
		if(CurMoney>MAX_BANK_MONEY)
			CurMoney = MAX_BANK_MONEY;

		pHuman->__SetBankMoney(CurMoney);
		pHuman->SetMoney(pHuman->GetMoney() - Amount);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_BANK_SAVE;
		MoneyLogParam.Count		=	Amount;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

	}
	else if(IsSave & CGBankMoney::PUTOUT_MONEY)
	{
		if(CurMoney - Amount < 0)
			Amount = CurMoney;

		pHuman->__SetBankMoney(CurMoney - Amount);
		pHuman->SetMoney(pHuman->GetMoney() + Amount);
		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_BANK_WITHDRAW;
		MoneyLogParam.Count		=	Amount;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

	}
	else if(IsSave & CGBankMoney::UPDATE_MONEY)
	{
		Amount = CurMoney;
	}

	if( IsSave & CGBankMoney::SAVE_RMB )
	{
		if( AmountRMB > pHuman->GetRMB() )
		{
			AmountRMB = pHuman->GetRMB();
		}

		CurRMB += AmountRMB;
		if(CurRMB>MAX_BANK_RMB)
			CurRMB = MAX_BANK_RMB;

		pHuman->SetBankRMB(CurRMB);
		pHuman->SetRMB( pHuman->GetRMB() - AmountRMB );

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_BANK_SAVERMB;
		MoneyLogParam.Count		=	Amount;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);
	}
	else if( IsSave & CGBankMoney::PUTOUT_RMB )
	{
		if(CurRMB - AmountRMB < 0)
			AmountRMB = CurRMB;

		pHuman->SetBankRMB(CurRMB - AmountRMB);
		pHuman->SetRMB(pHuman->GetRMB() + AmountRMB);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_BANK_WITHDRAWRMB;
		MoneyLogParam.Count		=	Amount;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);
	}
	else if( IsSave & CGBankMoney::UPDATA_RMB )
	{
		AmountRMB = CurRMB;
	}

	GCBankMoney Msg;
	Msg.SetSaveType(IsSave);
	Msg.SetAmount(Amount);
	Msg.SetAmountRMB(AmountRMB);
	pPlayer = pHuman->GetPlayer();
	pPlayer->SendPacket( &Msg ) ;
	g_pLog->FastSaveLog( LOG_FILE_1, "CGBankMoneyHandler: Issave=%d, Amount=%d, AmountRMB=%d", (INT)IsSave, Amount, AmountRMB ) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

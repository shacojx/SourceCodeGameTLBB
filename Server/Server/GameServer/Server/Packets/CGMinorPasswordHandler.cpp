#include "stdafx.h"

#include "CGMinorPasswd.h"

#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GCMinorPasswd.h"
#include "TimeManager.h"

using namespace Packets;


uint CGMinorPasswdHandler::Execute( CGMinorPasswd* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	CG_MINORPASSWD* pPasswdPacket;
	pPasswdPacket = pPacket->GetMinorPassword();

	GCMinorPasswd Msg;
	Msg.GetMinorPassword()->CleanUp();
	INT nErr = 0;

	switch( pPasswdPacket->m_Type )
	{
	case MREQT_PASSWDSETUP:					// 询问二级密码是否已经设置
		{
			Msg.GetMinorPassword()->m_Type = MRETT_PASSWDSETUP;
			RETURN_MINORPASSWD_SETUP* pSend = &(Msg.GetMinorPassword()->m_ReturnSetup);
			pSend->CleanUp();
			pSend->SetSetupFlag( pHuman->__IsPasswordSetup() );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X get the minor password setup info.",
				pHuman->GetGUID() );
		}
		break;
	case MREQT_DELETEPASSWDTIME:			// 询问是否处于强制解除阶段
		{
			UINT uRemainTime;

			uRemainTime = pHuman->__GetPasswordDeleteRemainTime();
			if ( uRemainTime == UINT_MAX )
			{
				return PACKET_EXE_CONTINUE;
			}

			Msg.GetMinorPassword()->m_Type = MRETT_DELETEPASSWDTIME;
			RETURN_DELETE_MINORPASSWD_TIME* pSend = &(Msg.GetMinorPassword()->m_ReturnDeleteTime);
			pSend->CleanUp();
			pSend->SetDeleteTime( uRemainTime );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X get the minor password delete time.",
				pHuman->GetGUID() );
		}
		break;
	case MREQT_SETPASSWD:					// 设置二级密码
		{
			if( pHuman->__IsPasswordSetup() )
			{
				Msg.GetMinorPassword()->m_Type = MRETT_ERR_SETPASSWDFAIL;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X set the minor password duplicated.",
					pHuman->GetGUID() );
			}
			else if( pHuman->__SetPassword( pPasswdPacket->m_OnePasswd.GetPasswd() ) )
			{
				Msg.GetMinorPassword()->m_Type = MRETT_SETPASSWDSUCC;
			}
			else
			{
				Msg.GetMinorPassword()->m_Type = MRETT_ERR_SETPASSWDFAIL;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X set the minor password failed.",
					pHuman->GetGUID() );
			}
		}
		break;
	case MREQT_MODIFYPASSWD:				// 修改二级密码
		{
			REQUEST_MINORPASSWD_MODIFY* pRecv = &(pPasswdPacket->m_ModifyPasswd);

			if( pHuman->ComparePasswd( pRecv->GetOldPasswd() )
			 && pHuman->__SetPassword( pRecv->GetNewPasswd() )
			 )
			{
				Msg.GetMinorPassword()->m_Type = MRETT_MODIFYPASSWDSUCC;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X change the minor password.",
					pHuman->GetGUID() );
			}
			else
			{
				Msg.GetMinorPassword()->m_Type = MRETT_ERR_MODIFYPASSWDFAIL;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X change the minor password failed.",
					pHuman->GetGUID() );
			}
		}
		break;
	case MREQT_UNLOCKPASSWD:				// 二级密码解锁
		{
			if( pHuman->ComparePasswd( pPasswdPacket->m_OnePasswd.GetPasswd() ) )
			{
				pHuman->__UnlockPassword();
				Msg.GetMinorPassword()->m_Type = MRETT_UNLOCKPASSWDSUCC;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X unlock the minor password.",
					pHuman->GetGUID() );

				if( pHuman->__GetPasswordDeleteTime() > 0 )
				{							// 解锁成功的话，强制密码解除失效
					pHuman->__SetPasswordDeleteTime( 0 );
					GCMinorPasswd Msg2;
					Msg2.GetMinorPassword()->m_Type = MRETT_DELETEPASSWDCANCEL;
					pGamePlayer->SendPacket( &Msg2 );

					g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X cancel the minor password deleting.",
						pHuman->GetGUID() );
				}
			}
			else
			{
				pHuman->__LockPassword();	// 只要解锁失败，重新上锁，不管以前是否已经解锁
				Msg.GetMinorPassword()->m_Type = MRETT_ERR_UNLOCKPASSWDFAIL;

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X unlock the minor password failed.",
					pHuman->GetGUID() );
			}
		}
		break;
	case MREQT_DELETEPASSWD:				// 强制解除二级密码
		{
			if( pHuman->__GetPasswordDeleteTime() > 0
			 && pHuman->__IsValidToDeletePassword()
			)
			{								// 如果是 15 天后点击强制解除，则解除成功
				pHuman->__DelPassword();
				Msg.GetMinorPassword()->m_Type = MRETT_DELETEPASSWDSUCC;

				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X delete the minor password success.",
					pHuman->GetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			else if( pHuman->__GetPasswordDeleteTime() == 0 )
			{								// 设置强制解除时间
				pHuman->__SetPasswordDeleteTime( (UINT)g_pTimeManager->GetANSITime() );
			}

			// 提示剩余时间
			Msg.GetMinorPassword()->m_Type = MRETT_DELETEPASSWDTIME;
			RETURN_DELETE_MINORPASSWD_TIME* pSend = &(Msg.GetMinorPassword()->m_ReturnDeleteTime);
			pSend->CleanUp();
			pSend->SetDeleteTime( pHuman->__GetPasswordDeleteRemainTime() );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGMinorPasswdHandler: GUID=%X wants to delete the minor password.",
				pHuman->GetGUID() );
		}
		break;
	default:
		return PACKET_EXE_CONTINUE;
	}

	pGamePlayer->SendPacket( &Msg );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

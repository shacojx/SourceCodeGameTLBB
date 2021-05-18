#include "StdAfx.h"
#include "GCMinorPasswd.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharSel.h"
#include "..\..\procedure\GamePro_CharCreate.h"

uint GCMinorPasswdHandler::Execute( GCMinorPasswd* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GC_MINORPASSWD* pRecv;
	pRecv = pPacket->GetMinorPassword();

	switch( pRecv->m_Type )
	{
	case MRETT_PASSWDSETUP:
		{
			
			if(1 == pRecv->m_ReturnSetup.m_uFlag)
			{// 已经设置过的密码
				
				// 设置密码成功
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(TRUE);
				// 设置未解锁密码状态。。
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
			}
			else
			{// 没有设置过的密码
				
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(INVALID_ID);
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(INVALID_ID);
			}
			break;
		}
	case MRETT_DELETEPASSWDTIME:
		{

			TCHAR bufInfo[512];
			_stprintf(bufInfo, "你已经申请了强制解除密码,请在%d小时后再次点击强制解除密码以确认", (pRecv->m_ReturnDeleteTime.m_uTime / (1000 * 60 *60)));
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, bufInfo);	
			break;
		}
	case MRETT_SETPASSWDSUCC:
		{
			// 设置密码成功
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(TRUE);

			// 设置未解锁密码状态。。
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "设置二级密码成功");	
			break;
		}
	case MRETT_MODIFYPASSWDSUCC:
		{
			// 修改密码成功
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "修改密码成功");	
			break;
		}
	case MRETT_UNLOCKPASSWDSUCC:
		{
			// 解锁密码成功
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(TRUE);
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "解锁密码成功");
			break;
		}
	case MRETT_DELETEPASSWDCANCEL:
		{
			break;
		}
	case MRETT_DELETEPASSWDSUCC:
		{
			// 强制接触密码成功。
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "强制解除密码成功");	
			break;
		}
	case MRETT_ERR_SETPASSWDFAIL:
		{
			// 设置二级密码失败
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "设置二级密码失败");	

			break;
		}
	case MRETT_ERR_MODIFYPASSWDFAIL:
		{// 修改密码失败
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "修改密码失败");	
			break;
		}
	case MRETT_ERR_UNLOCKPASSWDFAIL:
		{// 解锁失败。
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "解锁密码失败");	
			break;
		}
	case MRETT_ERR_DELETEPASSWDFAIL:
		{
			// 强制接触密码失败。
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "强制解除密码失败");	
			break;
		}
	default :
		Assert(FALSE);
		return 0;
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

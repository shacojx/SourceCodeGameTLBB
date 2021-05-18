#include "StdAfx.h"

#include "GCRetSetting.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Interface\GMInterface_Script_SystemSetup.h"


uint GCRetSettingHandler::Execute( GCRetSetting* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	_OWN_SETTING* pSetting = pPacket->GetSetting();

	SCRIPT_SANDBOX::SystemSetup::GetMe()->Set_GameData(pSetting[SETTING_TYPE_GAME].m_SettingData);
	
	for(INT i=SETTING_TYPE_K0;i < SETTING_TYPE_K19+1; i++)
	{
		CActionSystem::GetMe()->MainMenuBar_SetID(i-SETTING_TYPE_K0,pSetting[i].m_SettingType,pSetting[i].m_SettingData);
	}

	SCRIPT_SANDBOX::Talk::s_Talk.LoadTabSetting(pPacket);
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

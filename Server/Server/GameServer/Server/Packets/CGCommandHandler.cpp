#include "stdafx.h"


#include "CGCommand.h"
#include "Log.h"
#include "SceneManager.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Obj_Pet.h"
#include "AI_Human.h"
#include "Obj_Monster.h"
#include "Config.h"
#include "Server.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GCDetailXinFaList.h"
#include "GCNotifyEquip.h"
#include "CGPlayerDieResult.h"
#include "GCNotifyChangeScene.h"
#include "CGChat.h"
#include "GCChat.h"
#include "CGAskDetailAbilityInfo.h"
#include "CGUseAbility.h"
#include "CGUseGem.h"
#include "CGGemCompound.h"
#include "GWLevelUp.h"
#include "GWTeamInvite.h"
#include "GWTeamRetInvite.h"
#include "GWTeamLeaderRetInvite.h"
#include "GWTeamLeave.h"
#include "GWTeamDismiss.h"
#include "GWTeamKick.h"
#include "GWTeamApply.h"
#include "GWTeamRetApply.h"
#include "GWTeamAppoint.h"
#include "GWChannelCreate.h"
#include "GWChannelInvite.h"
#include "GWChannelDismiss.h"
#include "GWChannelKick.h"
#include "HumanItemLogic.h"
#include "GCItemInfo.h"
#include "PetManager.h"
#include "GameTable.h"
#include "GWMail.h"
#include "GWAskMail.h"
#include "GWGuild.h"
#include "CGRelation.h"
#include "GCAbilityLevel.h"
#include "GWCommand.h"
#include "GUIDManager.h"
#include "PlayerPool.h"
#include "GCOperateResult.h"
#include "ItemOperator.h"
#include "AI_Pet.h"
#include "Obj_Pet.h"
#include "GCDetailAttrib_Pet.h"
#include "LogDefine.h"
#include "GCPrescription.h"
#include "Impact_Core.h"
#include "GCLevelUpResult.h"
#include "GCLevelUp.h"
#include "CGFinger.h"
#include "GCAbilityExp.h"
#include "GCNotifyGoodBad.h"
#include "GWChat.h"
#include "GWSystemMsg.h"
using namespace Combat_Module::Impact_Module;


int g_Command_CommandDeny=FALSE ;//控制参数，禁止使用GM指令


#define COMMAND_BUFFER_SIZE 256
struct COMMAND_PARAM
{
	INT			Param0 ;
	INT			Param1 ;
	INT			Param2 ;
	INT			Param3 ;
	FLOAT		fParam0 ;
	FLOAT		fParam1 ;
	FLOAT		fParam2 ;
	FLOAT		fParam3 ;
	CHAR		m_aBuffer[COMMAND_BUFFER_SIZE] ;
	CHAR		m_aBufferEx[COMMAND_BUFFER_SIZE] ;

	COMMAND_PARAM( )
	{
		Param0 = 0 ;
		Param1 = 0 ;
		Param2 = 0 ;
		Param3 = 0 ;
		fParam0 = 0.0f ;
		fParam1 = 0.0f ;
		fParam2 = 0.0f ;
		fParam3 = 0.0f ;
		memset( m_aBuffer, 0, COMMAND_BUFFER_SIZE ) ;
		memset( m_aBufferEx, 0, COMMAND_BUFFER_SIZE ) ;
	};
};

BOOL Calc_GmRight( GM_EXECUTE_LEVEL gm_level, COMMAND_TYPE cmd )
{
	switch(cmd) {
	case COMMAND_TYPE_MODIFYMENPAI :
	case COMMAND_TYPE_CREATESCENE  :
	case COMMAND_TYPE_CLOSESCENE  :
	case COMMAND_TYPE_DEBUG_RELOADCONFIG  :
	case COMMAND_TYPE_DEBUG_SHUTDOWN  :
	case COMMAND_TYPE_TEAM_INVITE  :
	case COMMAND_TYPE_TEAM_RETINVITE  :
	case COMMAND_TYPE_TEAM_LEADERRETINVITE  :
	case COMMAND_TYPE_TEAM_APPLY  :
	case COMMAND_TYPE_TEAM_RETAPPLY  :
	case COMMAND_TYPE_TEAM_LEAVE  :
	case COMMAND_TYPE_TEAM_KICK   :
	case COMMAND_TYPE_TEAM_DISMISS   :
	case COMMAND_TYPE_TEAM_APPOINT   :
	case COMMAND_TYPE_MAKETEAMFOLLOW  :
	case COMMAND_TYPE_ABILITY_DETAIL :
	case COMMAND_TYPE_USE_ABILITY:
	case COMMAND_TYPE_COMBOUND_GEM :
	case COMMAND_TYPE_USE_GEM :
	case COMMAND_TYPE_CHANNEL_CREATE :
	case COMMAND_TYPE_CHANNEL_INVITE :
	case COMMAND_TYPE_CHANNEL_DISMISS  :
	case COMMAND_TYPE_CHANNEL_KICK  :
	case COMMAND_TYPE_CREATEGUILD :
	case COMMAND_TYPE_JOINGUILD :
	case COMMAND_TYPE_GUILDRECRUIT :
	case COMMAND_TYPE_GUILDEXPEL :
	case COMMAND_TYPE_LEAVEGUILD :
	case COMMAND_TYPE_GUILDDISMISS :
	case COMMAND_TYPE_ASKGUILDLIST :
	case COMMAND_TYPE_SETMOVEMODE   :
	case COMMAND_TYPE_ASKRELATIONLIST   :
	case COMMAND_TYPE_ADDFRIEND :
	case COMMAND_TYPE_SETFRIENDPOINT :
	case COMMAND_TYPE_ADDBLACKNAME:
	case COMMAND_TYPE_DELFRIEND :
	case COMMAND_TYPE_DELBLACK :
	case COMMAND_TYPE_TRANSITION :
	case COMMAND_TYPE_SETCAMPID :
	case COMMAND_TYPE_SAVE  :
	case COMMAND_TYPE_USEITEM :
	case COMMAND_TYPE_FORGETRECIPE :
	case COMMAND_TYPE_SEND_IMPACT_TO_UNIT :
	case COMMAND_TYPE_RELOAD_COMBAT_TABLES :
	case COMMAND_TYPE_USE_SKILL :
	case COMMAND_TYPE_SAVEPACKETINFO :
		{
			if(gm_level <GM_EXECUTE_ALL)
			{
				return FALSE;
			}
			
		}
		break;
	case COMMAND_TYPE_MODIFYXINFA  :
	case COMMAND_TYPE_LEVELUPALLXINFA  :
	case COMMAND_TYPE_SET_PK_VALUE  :
	case COMMAND_TYPE_SET_MODEL_ID  :
	case COMMAND_TYPE_SET_EQUIP_DUR  :
	case COMMAND_TYPE_ENABLEGOD   :
	case COMMAND_TYPE_DISABLEGOD   :
	case COMMAND_TYPE_CREATEITEM   :
	case COMMAND_TYPE_CREATEPET   :
	case COMMAND_TYPE_DELETEPET   :
	case COMMAND_TYPE_CAPTUREPET   :
	case COMMAND_TYPE_CALLUPPET   :
	case COMMAND_TYPE_RECALLPET   :
	case COMMAND_TYPE_FREEPETTONATURE   :
	case COMMAND_TYPE_SETPETHAPPINESS   :
	case COMMAND_TYPE_RETURNTOCHILD   :
	case COMMAND_TYPE_SETPETHP :
	case COMMAND_TYPE_SETPETLIFE  :
	case COMMAND_TYPE_PETLEVELUP  :
	case COMMAND_TYPE_SETPETSKILL  :
	case COMMAND_TYPE_USEPETSKILL  :
	case COMMAND_TYPE_PETPULLULATE :
	case COMMAND_TYPE_ADD_SKILL  :
	case COMMAND_TYPE_REMOVE_SKILL  :
	case COMMAND_TYPE_MODIFYMONEY :
	case COMMAND_TYPE_LEVELUP  :
	case COMMAND_TYPE_ABILITYUP :
	case COMMAND_TYPE_LEVELDOWN  :
	case COMMAND_TYPE_ADDEXP  :
	case COMMAND_TYPE_SETHAIRCOLOR  :
	case COMMAND_TYPE_SETHAIRMODEL  :
	case COMMAND_TYPE_CREATEGUARD :
	case COMMAND_TYPE_CREATETRAP  :
	case COMMAND_TYPE_SETABILITYEXP :
	case COMMAND_TYPE_SERMBMONEY  :
	case COMMAND_TYPE_DOUBLEEXP  :
	case COMMAND_TYPE_SETGMRIGHT:
		{
			if(gm_level <GM_EXECUTE_ADMIN)
			{
				return FALSE;
			}

		}
		break;
	case COMMAND_TYPE_SET_SCENE_TYPE :
	case COMMAND_TYPE_SETAITYPE  :
	case COMMAND_TYPE_LOADMONSTER :
	case COMMAND_TYPE_TELEPORT   :
	case COMMAND_TYPE_GOTO   :
	case COMMAND_TYPE_RELIVE   :
	case COMMAND_TYPE_IAMGOD   :
	case COMMAND_TYPE_IAMDOG   :
	case COMMAND_TYPE_FULLRECOVER   :
	case COMMAND_TYPE_SKILL_RELIVE  :
	case COMMAND_TYPE_SETDAMAGE  :
	case COMMAND_TYPE_KILLOBJ :
	case COMMAND_TYPE_CREATEMONSTER  :
	case COMMAND_TYPE_DELETEMONSTER  :
	case COMMAND_TYPE_SENDMAIL  :
	case COMMAND_TYPE_RECVMAIL  :
	case COMMAND_TYPE_CATCHPLAYER  :
	case COMMAND_TYPE_PLAYERINFO :
	case COMMAND_TYPE_WHO :
	case COMMAND_TYPE_ALLWHO  :
	case COMMAND_TYPE_GETOBJINFO :
	case COMMAND_TYPE_FINGERGUID  :
	case COMMAND_TYPE_FINGERNAME  :
	case COMMAND_TYPE_ADVANCEDFINGER  :
		{
			if(gm_level <GM_EXECUTE_GMADMIN)
			{
				return FALSE;
			}

		}
		break;
	case COMMAND_TYPE_SYSCHAT   :
		{
			if(gm_level <GM_EXECUTE_GM)
			{
				return FALSE;
			}
		}
		break;
	case COMMAND_TYPE_GMTEST:
	case COMMAND_TYPE_SHOWGUID:
		{
			return TRUE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

COMMAND_TYPE Calc_Command( CHAR* pCommand, INT iSize, COMMAND_PARAM* pOutParam )
{
	CHAR szCommand[MAX_COMMAND_SIZE] ;
	memset( szCommand, 0, MAX_COMMAND_SIZE ) ;

	if( iSize>=MAX_COMMAND_SIZE || iSize<0 )
		return COMMAND_TYPE_NONE ;

	INT iPos ;
	for( iPos=0; iPos<iSize; iPos++ )
	{
		if( pCommand[iPos]==' ' || pCommand[iPos]==0 || pCommand[iPos]=='\t' )
			break ;
		szCommand[iPos] = pCommand[iPos] ;
	}
	CHAR* pCom =  szCommand ;


	if( tstricmp(pCom,"none")==0 )
	{
		return COMMAND_TYPE_NONE ;
	}
	else if( tstricmp(pCom,"teleport")==0 || tstricmp(pCom,"tp")==0 )
	{//teleport obj=?? loc=???,??? scene=??
	
		CHAR* pParam = pCommand+iPos ;

		CHAR* pObj = strchr( pParam, '=' ) ;
		if( pObj==NULL ) return COMMAND_TYPE_NONE ;

		pObj ++ ;
		pOutParam->Param0 = atoi( pObj ) ;

		CHAR* pX = strchr( pObj, '=' ) ;
		if( pX==NULL ) return COMMAND_TYPE_NONE ;

		pX ++ ;
		pOutParam->fParam0 = (FLOAT)(atof(pX)) ;

		CHAR* pZ = strchr( pX, ',' ) ;
		if( pZ==NULL ) return COMMAND_TYPE_NONE ;

		pZ ++ ;
		pOutParam->fParam1 = (FLOAT)(atof(pZ)) ;

		pOutParam->Param1 = INVALID_ID ;
		CHAR* pScene = strchr( pZ, '=' ) ;
		if( pScene==NULL ) return COMMAND_TYPE_TELEPORT ;

		pScene ++ ;
		pOutParam->Param1 = atoi(pScene) ;


		return COMMAND_TYPE_TELEPORT ;
	}
	else if( tstricmp( pCom, "modifymenpai")==0 )
	{//modifymenpai menpai=??
		CHAR* pParam = pCommand+iPos ;

		CHAR* pMenpai = strchr( pParam, '=' ) ;
		if( pMenpai==NULL ) return COMMAND_TYPE_NONE ;

		pMenpai ++ ;
		pOutParam->Param0 = atoi( pMenpai ) ;

		return COMMAND_TYPE_MODIFYMENPAI ;
	}
	else if( tstricmp( pCom, "modifyxinfa")==0 )
	{//modifyxinfa xinfa=??,grade=??
		CHAR* pParam = pCommand+iPos ;

		CHAR* pXinfa = strchr( pParam, '=' ) ;
		if( pXinfa==NULL ) return COMMAND_TYPE_NONE ;

		pXinfa ++ ;
		pOutParam->Param0 = atoi( pXinfa ) ;

		CHAR* pGrade = strchr( pXinfa, '=' ) ;
		if( pGrade==NULL ) return COMMAND_TYPE_NONE ;

		pGrade ++ ;
		pOutParam->Param1 = atoi(pGrade) ;

		return COMMAND_TYPE_MODIFYXINFA ;
	}
	else if( tstricmp( pCom, "levelupallxinfa")==0 )
	{//levelupallxinxa grade=??
		CHAR* pParam = pCommand+iPos ;

		CHAR* pGrade = strchr( pParam, '=' ) ;
		if( pGrade==NULL ) return COMMAND_TYPE_NONE ;
		
		pGrade ++ ;
		pOutParam->Param0 = atoi(pGrade) ;

		return COMMAND_TYPE_LEVELUPALLXINFA;
	}

	else if( tstricmp( pCom, "createitem")==0 )
	{//createitem itemtype=??,quality=??
		CHAR* pParam = pCommand+iPos ;

		CHAR* pGUID_Type = strchr( pParam, '=' ) ;
		if( pGUID_Type==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Type ++ ;
		pOutParam->Param0 = atoi( pGUID_Type ) ;

		CHAR* pQuality = strchr( pGUID_Type, '=' ) ;
		if( pQuality==NULL ) return COMMAND_TYPE_NONE ;

		pQuality ++ ;
		pOutParam->Param1= atoi( pQuality ) ;

		return COMMAND_TYPE_CREATEITEM ;
	}
	else if(tstricmp( pCom, "useitem")==0 )
	{//useitem bagitempos=??,targetitempos=??
		CHAR* pParam = pCommand+iPos ;

		CHAR* pBagItemPos = strchr( pParam, '=' ) ;
		if( pBagItemPos==NULL ) return COMMAND_TYPE_NONE ;

		pBagItemPos ++ ;
		pOutParam->Param0 = atoi( pBagItemPos ) ;

		CHAR* pTargetItemPos = strchr( pBagItemPos, '=' ) ;
		if( pTargetItemPos==NULL ) return COMMAND_TYPE_NONE ;

		pTargetItemPos ++ ;
		pOutParam->Param1= atoi( pTargetItemPos ) ;

		return COMMAND_TYPE_USEITEM ;	
	}
	else if( tstricmp( pCom, "relive")==0 )
	{//relive
		return COMMAND_TYPE_RELIVE ;
	}
	else if( tstricmp( pCom, "chat")==0 )
	{//chat ?????????
		CHAR* pParam = pCommand+iPos ;

		strncpy( pOutParam->m_aBuffer, pParam+1, COMMAND_BUFFER_SIZE-1 ) ;

		return COMMAND_TYPE_CHAT ;
	}
	else if ( tstricmp( pCom, "enablegod" )==0 )
	{//enablegod
		return COMMAND_TYPE_ENABLEGOD;
	}
	else if ( tstricmp( pCom, "disablegod" )==0 )
	{//disablegod
		return COMMAND_TYPE_DISABLEGOD;
	}
	else if( tstricmp( pCom, "iamgod")==0 )
	{//iamgod
		return COMMAND_TYPE_IAMGOD ;
	}
	else if( tstricmp( pCom, "iamdog")==0 )
	{//iamdog
		return COMMAND_TYPE_IAMDOG ;
	}
	else if( tstricmp( pCom, "debug_reloadconfig")==0 )
	{//debug_reloadconfig
		return COMMAND_TYPE_DEBUG_RELOADCONFIG ;
	}
	else if( tstricmp( pCom, "syschat")==0 )
	{//syschat ????????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pSour = strchr( pParam, '=' ) ;
		if( pSour==NULL ) return COMMAND_TYPE_NONE ;
		pSour++;
		strncpy( pOutParam->m_aBuffer, pSour, COMMAND_BUFFER_SIZE-1 ) ;
		return COMMAND_TYPE_SYSCHAT ;
	}
	else if( tstricmp( pCom, "debug_shutdown")==0 )
	{//debug_shutdown
		return COMMAND_TYPE_DEBUG_SHUTDOWN ;
	}
	else if( tstricmp( pCom, "team_nnvite")==0 )
	{//team_nnvite sguid=????, dguid=????

		CHAR* pParam = pCommand+iPos ;

		CHAR* pGUID_Sour = strchr( pParam, '=' ) ;
		if( pGUID_Sour==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Sour ++ ;
		pOutParam->Param0 = atoi( pGUID_Sour ) ;

		CHAR* pName_Dest = strchr( pGUID_Sour, '=' ) ;
		if( pName_Dest==NULL ) return COMMAND_TYPE_NONE ;

		pName_Dest ++ ;
		strncpy(pOutParam->m_aBuffer, pName_Dest, MAX_CHARACTER_NAME-1);


		return COMMAND_TYPE_TEAM_INVITE ;
	}
	else if( tstricmp( pCom, "team_retinvite")==0 )
	{//team_retinvite ret=?, guid=????

		CHAR* pParam = pCommand+iPos ;

		CHAR* pRet = strchr( pParam, '=' ) ;
		if( pRet==NULL ) return COMMAND_TYPE_NONE ;

		pRet ++ ;
		pOutParam->Param0 = atoi( pRet ) ;

		CHAR* pGUID = strchr( pRet, '=' ) ;
		if( pGUID==NULL ) return COMMAND_TYPE_NONE ;

		pGUID ++ ;
		pOutParam->Param1 = atoi( pGUID ) ;

		return COMMAND_TYPE_TEAM_RETINVITE ;
	}
	else if( tstricmp( pCom, "team_leaderretinvite")==0 )
	{//team_leaderretinvite ret=?, sguid=????, dguid=????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pRet = strchr( pParam, '=' ) ;
		if( pRet==NULL ) return COMMAND_TYPE_NONE ;

		pRet ++ ;
		pOutParam->Param0 = atoi( pRet ) ;

		CHAR* pGUID_Sour = strchr( pRet, '=' ) ;
		if( pGUID_Sour==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Sour ++ ;
		pOutParam->Param1 = atoi( pGUID_Sour ) ;

		CHAR* pGUID_Dest = strchr( pGUID_Sour, '=' ) ;
		if( pGUID_Dest==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Dest ++ ;
		pOutParam->Param2 = atoi( pGUID_Dest ) ;


		return COMMAND_TYPE_TEAM_LEADERRETINVITE ;
	}
	else if( tstricmp( pCom, "team_apply")==0 )
	{//team_apply sguid=????, dguid=????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pGUID_Sour = strchr( pParam, '=' ) ;
		if( pGUID_Sour==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Sour ++ ;
		pOutParam->Param0 = atoi( pGUID_Sour ) ;

		CHAR* pName_Dest = strchr( pGUID_Sour, '=' ) ;
		if( pName_Dest==NULL ) return COMMAND_TYPE_NONE ;

		pName_Dest ++ ;
		strncpy(pOutParam->m_aBuffer, pName_Dest, COMMAND_BUFFER_SIZE) ;


		return COMMAND_TYPE_TEAM_APPLY ;
	}
	else if( tstricmp( pCom, "team_retapply")==0 )
	{//team_retapply ret=?, sguid=????, dguid=????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pRet = strchr( pParam, '=' ) ;
		if( pRet==NULL ) return COMMAND_TYPE_NONE ;

		pRet ++ ;
		pOutParam->Param0 = atoi( pRet ) ;

		CHAR* pGUID_Sour = strchr( pRet, '=' ) ;
		if( pGUID_Sour==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Sour ++ ;
		pOutParam->Param1 = atoi( pGUID_Sour ) ;

		CHAR* pGUID_Dest = strchr( pGUID_Sour, '=' ) ;
		if( pGUID_Dest==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Dest ++ ;
		pOutParam->Param2 = atoi( pGUID_Dest ) ;

		return COMMAND_TYPE_TEAM_RETAPPLY ;
	}
	else if( tstricmp( pCom, "team_leave")==0 )
	{//team_leave guid=????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pGUID = strchr( pParam, '=' ) ;
		if( pGUID==NULL ) return COMMAND_TYPE_NONE ;

		pGUID ++ ;
		pOutParam->Param0 = atoi( pGUID ) ;

		return COMMAND_TYPE_TEAM_LEAVE ;
	}
	else if( tstricmp( pCom, "team_kick")==0 )
	{//team_kick sguid=????, dguid=????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pGUID_Sour = strchr( pParam, '=' ) ;
		if( pGUID_Sour==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Sour ++ ;
		pOutParam->Param0 = atoi( pGUID_Sour ) ;

		CHAR* pGUID_Dest = strchr( pGUID_Sour, '=' ) ;
		if( pGUID_Dest==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Dest ++ ;
		pOutParam->Param1 = atoi( pGUID_Dest ) ;


		return COMMAND_TYPE_TEAM_KICK ;
	}
	else if( tstricmp( pCom, "team_dismiss")==0 )
	{//team_dismiss guid=????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pGUID = strchr( pParam, '=' ) ;
		if( pGUID==NULL ) return COMMAND_TYPE_NONE ;

		pGUID ++ ;
		pOutParam->Param0 = atoi( pGUID ) ;

		return COMMAND_TYPE_TEAM_DISMISS ;
	}
	else if( tstricmp( pCom, "team_appoint")==0 )
	{//team_appoint sguid=????, dguid=????

		CHAR* pParam = pCommand+iPos ;

		CHAR* pGUID_Sour = strchr( pParam, '=' ) ;
		if( pGUID_Sour==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Sour ++ ;
		pOutParam->Param0 = atoi( pGUID_Sour ) ;

		CHAR* pGUID_Dest = strchr( pGUID_Sour, '=' ) ;
		if( pGUID_Dest==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Dest ++ ;
		pOutParam->Param1 = atoi( pGUID_Dest ) ;


		return COMMAND_TYPE_TEAM_APPOINT ;
	}
	else if( tstricmp(pCom, "ability_detail") == 0 || tstricmp( pCom, "ability" ) == 0 )
	{// ability objid=????
		CHAR* pParam = pCommand+iPos;
		CHAR* pObjID = strchr( pParam, '=' );
		if( pObjID==NULL ) return COMMAND_TYPE_NONE;

		pObjID++;
		pOutParam->Param0 = atoi( pObjID ) ;

		return COMMAND_TYPE_ABILITY_DETAIL;
	}
	else if( tstricmp( pCom, "use_ability" ) == 0 )
	{// use_ability ability_id=?? prescription_id=???? obj_id=????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pAbilityID = strchr( pParam, '=' ) ;
		if( pAbilityID==NULL ) return COMMAND_TYPE_NONE ;

		pAbilityID++ ;
		pOutParam->Param0 = atoi( pAbilityID ) ;

		CHAR* pPrescrID = strchr( pAbilityID, '=' ) ;
		if( pPrescrID==NULL ) return COMMAND_TYPE_NONE ;

		pPrescrID++ ;
		pOutParam->Param1 = atoi( pPrescrID ) ;

		CHAR* pObjID = strchr( pPrescrID, '=' );
		if( pObjID == NULL ) return COMMAND_TYPE_NONE;

		pObjID++;
		pOutParam->Param2 = atoi( pObjID );

		return COMMAND_TYPE_USE_ABILITY;
	}
	else if( tstricmp( pCom, "combound_gem" ) == 0 )
	{ // combound_gem gempos1=?? gempos2 = ??
		CHAR* pParam = pCommand+iPos;
		CHAR* pGemPos = strchr( pParam, '=' );
		if( pGemPos == NULL ) return COMMAND_TYPE_NONE;

		pGemPos++;
		pOutParam->Param0 = atoi( pGemPos );

		pGemPos = strchr( pGemPos, '=' );
		if( pGemPos == NULL ) return COMMAND_TYPE_NONE;

		pGemPos++;
		pOutParam->Param1 = atoi( pGemPos );

		return COMMAND_TYPE_COMBOUND_GEM;
	}
	else if( tstricmp( pCom, "use_gem" ) == 0 )
	{ // use_gem pos1=?? pos2 = ??
		CHAR* pParam = pCommand+iPos;
		CHAR* pGemPos = strchr( pParam, '=' );
		if( pGemPos == NULL ) return COMMAND_TYPE_NONE;

		pGemPos++;
		pOutParam->Param0 = atoi( pGemPos );

		pGemPos = strchr( pGemPos, '=' );
		if( pGemPos == NULL ) return COMMAND_TYPE_NONE;

		pGemPos++;
		pOutParam->Param1 = atoi( pGemPos );

		return COMMAND_TYPE_USE_GEM;
	}
	else if( tstricmp( pCom, "channel_create" ) == 0 )
	{//channel_create guid=????
		CHAR* pParam = pCommand+iPos;
		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID==NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = atoi( pGUID ) ;

		return COMMAND_TYPE_CHANNEL_CREATE ;
	}
	else if( tstricmp( pCom, "channel_invite" ) == 0 )
	{//channel_invite sguid=????, dguid=????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pGUID_Sour = strchr( pParam, '=' ) ;
		if( pGUID_Sour==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Sour ++ ;
		pOutParam->Param0 = atoi( pGUID_Sour ) ;

		CHAR* pGUID_Dest = strchr( pGUID_Sour, '=' ) ;
		if( pGUID_Dest==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Dest ++ ;
		pOutParam->Param1 = atoi( pGUID_Dest ) ;

		return COMMAND_TYPE_CHANNEL_INVITE ;
	}
	else if( tstricmp( pCom, "channel_dismiss" ) == 0 )
	{//channel_dismiss guid=????
		CHAR* pParam = pCommand+iPos;
		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID==NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = atoi( pGUID ) ;

		return COMMAND_TYPE_CHANNEL_DISMISS ;
	}
	else if( tstricmp( pCom, "channel_kick" ) == 0 )
	{//channel_kick sguid=????, dguid=????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pGUID_Sour = strchr( pParam, '=' ) ;
		if( pGUID_Sour==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Sour ++ ;
		pOutParam->Param0 = atoi( pGUID_Sour ) ;

		CHAR* pGUID_Dest = strchr( pGUID_Sour, '=' ) ;
		if( pGUID_Dest==NULL ) return COMMAND_TYPE_NONE ;

		pGUID_Dest ++ ;
		pOutParam->Param1 = atoi( pGUID_Dest ) ;

		return COMMAND_TYPE_CHANNEL_KICK ;
	}
	else if( tstricmp( pCom, "goto" ) == 0 )
	{//goto loc=??,?? scene=??
		CHAR* pParam = pCommand+iPos ;

		CHAR* pX = strchr( pParam, '=' ) ;
		if( pX==NULL ) return COMMAND_TYPE_NONE ;

		pX ++ ;
		pOutParam->fParam0 = (FLOAT)(atof(pX)) ;

		CHAR* pZ = strchr( pX, ',' ) ;
		if( pZ==NULL ) return COMMAND_TYPE_NONE ;

		pZ ++ ;
		pOutParam->fParam1 = (FLOAT)(atof(pZ)) ;

		pOutParam->Param0 = INVALID_ID ;
		CHAR* pScene = strchr( pZ, '=' ) ;
		if( pScene==NULL ) return COMMAND_TYPE_GOTO ;

		pScene ++ ;
		pOutParam->Param0 = atoi(pScene) ;

		return COMMAND_TYPE_GOTO ;
	}
	else if( tstricmp(pCom,"fullrecover")==0 || tstricmp(pCom,"full")==0 )
	{// fullrecover(full)
		return COMMAND_TYPE_FULLRECOVER ;
	}
	else if ( tstricmp(pCom, "createpet") == 0 ) 
	{// createpet petid=???
		CHAR* pParam = pCommand+iPos;

		CHAR* pDataID = strchr( pParam, '=' ) ;
		if( pDataID==NULL ) return COMMAND_TYPE_NONE ;

		pDataID++;
		pOutParam->Param0 = atoi( pDataID ) ;

		return COMMAND_TYPE_CREATEPET;
	}
	else if ( tstricmp(pCom, "deletepet") == 0 )
	{// deletepet petid=???
		CHAR* pParam = pCommand+iPos;

		CHAR* pDataID = strchr( pParam, '=' ) ;
		if( pDataID==NULL ) return COMMAND_TYPE_NONE ;

		pDataID++;
		pOutParam->Param0 = atoi( pDataID ) ;

		return COMMAND_TYPE_DELETEPET;
	}
	else if ( tstricmp(pCom, "capturepet") == 0 )
	{// capturepet petid=???
		CHAR* pParam = pCommand+iPos;

		CHAR* pDataID = strchr( pParam, '=' );
		if ( pDataID == NULL ) return COMMAND_TYPE_NONE;

		pDataID++;
		pOutParam->Param0 = atoi( pDataID );

        return COMMAND_TYPE_CAPTUREPET;
	}
	else if ( tstricmp(pCom, "calluppet") == 0 )
	{// calluppet petindex=???
		CHAR* pParam = pCommand+iPos;

		CHAR* pDataID = strchr( pParam, '=' );
		if ( pDataID == NULL ) return COMMAND_TYPE_NONE;

		pDataID++;
		pOutParam->Param0 = atoi( pDataID );

		return COMMAND_TYPE_CALLUPPET;
	}
	else if ( tstricmp(pCom, "recallpet") == 0 )
	{// recallpet
		return COMMAND_TYPE_RECALLPET;
	}
	else if ( tstricmp(pCom, "freepettonature") == 0 )
	{// freepettonature =petindex
		CHAR* pParam = pCommand+iPos;
		CHAR* pszIndex = strchr( pParam, '=' );
		if( pszIndex == NULL ) return COMMAND_TYPE_NONE;

		pszIndex++;
		pOutParam->Param0 = (INT)(atoi(pszIndex));

		return COMMAND_TYPE_FREEPETTONATURE;
	}
	else if ( tstricmp(pCom, "returntochild") == 0 )
	{// returntochild =petindex
		CHAR* pParam = pCommand+iPos;
		CHAR* pszIndex = strchr( pParam, '=' );
		if( pszIndex == NULL ) return COMMAND_TYPE_NONE;

		pszIndex++;
		pOutParam->Param0 = (INT)(atoi(pszIndex));

		return COMMAND_TYPE_RETURNTOCHILD;
	}
	else if ( tstricmp(pCom, "setmovemode") == 0 )
	{// setmovemode modeid=???
		CHAR* pParam = pCommand+iPos;

		CHAR* pDataID = strchr( pParam, '=' );
		if ( pDataID == NULL ) return COMMAND_TYPE_NONE;

		pDataID++;
		pOutParam->Param0 = atoi( pDataID );

		return COMMAND_TYPE_SETMOVEMODE;
	}
	else if ( tstricmp( pCom, "skillrelive" ) == 0 )
	{// skillrelive
		return COMMAND_TYPE_SKILL_RELIVE;
	}
	else if( tstricmp(pCom, "createscene") == 0 )
	{//createscene
		return COMMAND_TYPE_CREATESCENE ;
	}
	else if( tstricmp(pCom, "closescene") == 0 )
	{//closescene scene=??
		CHAR* pParam = pCommand+iPos;

		CHAR* pSceneID = strchr( pParam, '=' );
		if ( pSceneID == NULL ) return COMMAND_TYPE_NONE;

		pSceneID++;
		pOutParam->Param0 = atoi( pSceneID );

		return COMMAND_TYPE_CLOSESCENE ;
	}
	else if( tstricmp(pCom, "setscenetype") == 0 )
	{//setscenetype scene=??
		CHAR* pParam = pCommand+iPos;

		CHAR* pSceneType = strchr( pParam, '=' );
		if ( pSceneType == NULL ) return COMMAND_TYPE_NONE;

		pSceneType++;
		pOutParam->Param0 = atoi( pSceneType );

		return COMMAND_TYPE_SET_SCENE_TYPE ;
	}
	else if( tstricmp(pCom, "setpkvalue") == 0 )
	{
		CHAR* pParam = pCommand+iPos;

		CHAR* pszText = strchr( pParam, '=' );
		if ( pszText == NULL ) return COMMAND_TYPE_NONE;

		pszText++;
		pOutParam->Param0 = atoi( pszText );

		return COMMAND_TYPE_SET_PK_VALUE ;
	}
	else if( tstricmp(pCom, "setmodelid") == 0 )
	{
		CHAR* pParam = pCommand+iPos;

		CHAR* pszText = strchr( pParam, '=' );
		if ( pszText == NULL ) return COMMAND_TYPE_NONE;

		pszText++;
		pOutParam->Param0 = atoi( pszText );

		return COMMAND_TYPE_SET_MODEL_ID ;
	}
	else if( tstricmp(pCom, "setequipdur") == 0 )
	{
		CHAR* pParam = pCommand+iPos ;

		CHAR* pIndex = strchr( pParam, '=' ) ;
		if( pIndex==NULL ) return COMMAND_TYPE_NONE ;

		pIndex ++ ;
		pOutParam->Param1 = (INT)(atoi(pIndex)) ;

		CHAR* pLeft = strchr( pIndex, ',' ) ;
		if( pLeft==NULL ) return COMMAND_TYPE_NONE ;

		pLeft ++ ;
		pOutParam->Param2 = (INT)(atoi(pLeft)) ;

		return COMMAND_TYPE_SET_EQUIP_DUR ;
	}
	else if( tstricmp(pCom, "setdamage") == 0 )
	{//setdamage obj=?? damage=????
		CHAR* pParam = pCommand+iPos ;

		CHAR* pObjID = strchr( pParam, '=' ) ;
		if( pObjID==NULL ) return COMMAND_TYPE_NONE ;

		pObjID ++ ;
		pOutParam->Param0 = (INT)(atoi(pObjID)) ;

		CHAR* pDamage = strchr( pObjID, '=' ) ;
		if( pDamage==NULL ) return COMMAND_TYPE_NONE ;

		pDamage ++ ;
		pOutParam->Param1 = (INT)(atoi(pDamage)) ;

		return COMMAND_TYPE_SETDAMAGE ;
	}
	else if( tstricmp(pCom, "killobj") == 0 )
	{//killobj obj=??
		CHAR* pParam = pCommand+iPos ;

		CHAR* pObjID = strchr( pParam, '=' ) ;
		if( pObjID==NULL ) return COMMAND_TYPE_NONE ;

		pObjID ++ ;
		pOutParam->Param0 = (INT)(atoi(pObjID)) ;

		return COMMAND_TYPE_KILLOBJ ;
	}
	else if( tstricmp(pCom, "createmonster") == 0 )
	{//createmonster type=?? pos=??,?? baseai=?? scriptai=?? script=??
		CHAR* pParam = pCommand+iPos ;

		CHAR* pType = strchr( pParam, '=' ) ;
		if( pType==NULL ) return COMMAND_TYPE_NONE ;

		pType ++ ;
		pOutParam->Param0 = (INT)(atoi(pType)) ;

		CHAR* pPos_X = strchr( pType, '=' ) ;
		if( pPos_X==NULL ) return COMMAND_TYPE_NONE ;

		pPos_X ++ ;
		pOutParam->fParam0 = (FLOAT)(atof(pPos_X)) ;

		CHAR* pPos_Z = strchr( pPos_X, ',' ) ;
		if( pPos_Z==NULL ) return COMMAND_TYPE_NONE ;

		pPos_Z ++ ;
		pOutParam->fParam1 = (FLOAT)(atof(pPos_Z)) ;

		CHAR* pBaseAI = strchr( pPos_Z, '=' ) ;
		if( pBaseAI==NULL ) return COMMAND_TYPE_NONE ;

		pBaseAI ++ ;
		pOutParam->Param1 = (INT)(atoi(pBaseAI)) ;

		CHAR* pScriptAI = strchr( pBaseAI, '=' ) ;
		if( pScriptAI==NULL ) return COMMAND_TYPE_NONE ;

		pScriptAI ++ ;
		pOutParam->Param2 = (INT)(atoi(pScriptAI)) ;

		CHAR* pScriptID = strchr( pScriptAI, '=' ) ;
		if( pScriptID==NULL ) return COMMAND_TYPE_NONE ;

		pScriptID ++ ;
		pOutParam->Param3 = (INT)(atoi(pScriptID)) ;

		CHAR* pCampID = strchr( pScriptID, '=' ) ;
		if( pCampID==NULL ) return COMMAND_TYPE_NONE ;

		pCampID ++ ;
		strncpy( pOutParam->m_aBuffer, pCampID, 10 );

		return COMMAND_TYPE_CREATEMONSTER ;
	}
	else if( tstricmp(pCom, "deletemonster") == 0 )
	{//deletemonster objid=???
		CHAR* pParam = pCommand+iPos ;

		CHAR* pObjID = strchr( pParam, '=' ) ;
		if( pObjID==NULL ) return COMMAND_TYPE_NONE ;

		pObjID ++ ;
		pOutParam->Param0 = (INT)(atoi(pObjID)) ;


		return COMMAND_TYPE_DELETEMONSTER ;
	}
	else if (tstricmp(pCom, "setaitype") == 0)
	{//setaitype objid=??? aitype=???
		CHAR* pParam = pCommand+iPos ;

		CHAR* pObjID = strchr( pParam, '=' ) ;
		if( pObjID==NULL ) return COMMAND_TYPE_NONE ;

		pObjID ++ ;
		pOutParam->Param0 = (INT)(atoi(pObjID)) ;

		CHAR* pAIType = strchr( pObjID, '=' ) ;
		if( pAIType==NULL ) return COMMAND_TYPE_NONE ;

		pAIType ++ ;
		pOutParam->Param1 = (INT)(atoi(pAIType)) ;

		return COMMAND_TYPE_SETAITYPE ;

	}
	else if( tstricmp(pCom, "modifymoney") == 0 )
	{//modifymoney gold=???
		CHAR* pParam = pCommand+iPos ;

		CHAR* pMoney = strchr( pParam, '=' ) ;
		if( pMoney==NULL ) return COMMAND_TYPE_NONE ;

		pMoney ++ ;
		pOutParam->Param0 = (INT)(atoi(pMoney)) ;

		return COMMAND_TYPE_MODIFYMONEY ;
	}
	else if ( tstricmp(pCom, "teamfollow") == 0 )
	{//teamfollow
		return COMMAND_TYPE_MAKETEAMFOLLOW;
	}
	else if( tstricmp(pCom, "sendmail")==0 )
	{//sendmail destguid=??? contex=???
		CHAR* pParam = pCommand+iPos ;

		CHAR* pDestGUID = strchr( pParam, '=' ) ;
		if( pDestGUID==NULL ) return COMMAND_TYPE_NONE ;

		pDestGUID ++ ;
		CHAR* pDestNameLast = strchr( pDestGUID, ' ' ) ;
		if( pDestNameLast==NULL ) return COMMAND_TYPE_NONE ;

		INT nLen = (INT)(pDestNameLast-pDestGUID) ;
		memcpy( pOutParam->m_aBuffer, pDestGUID, nLen ) ;

		CHAR* pContex = strchr( pDestGUID, '=' ) ;
		if( pContex==NULL ) return COMMAND_TYPE_NONE ;

		pContex ++ ;
		strncpy( pOutParam->m_aBufferEx, pContex, COMMAND_BUFFER_SIZE-1 ) ;

		return COMMAND_TYPE_SENDMAIL ;
	}
	else if( tstricmp(pCom, "recvmail")==0 )
	{//recvmail
		return COMMAND_TYPE_RECVMAIL ;
	}
	else if( tstricmp(pCom, "createguild") == 0 )
	{ // createguild =guildname
		CHAR* pParam = pCommand+iPos;
		CHAR* pName = strchr( pParam, '=' );
		if( pName == NULL ) return COMMAND_TYPE_NONE;

		pName++;
		strncpy(pOutParam->m_aBuffer, pName, sizeof(pOutParam->m_aBuffer) - 1);
		return COMMAND_TYPE_CREATEGUILD;	// 创建帮会
	}
	else if( tstricmp(pCom, "joinguild") == 0 )
	{ // joinguild =guildid
		CHAR* pParam = pCommand+iPos;
		CHAR* pGuildID = strchr( pParam, '=' );
		if( pGuildID == NULL ) return COMMAND_TYPE_NONE;

		pGuildID++;
		pOutParam->Param0 = (INT)(atoi(pGuildID));
		return COMMAND_TYPE_JOINGUILD;	// 加入帮会
	}
	else if( tstricmp(pCom, "guildrecruit") == 0 )
	{ // guildrecruit =guid
		CHAR* pParam = pCommand+iPos;
		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = (INT)(atoi(pGUID));
		return COMMAND_TYPE_GUILDRECRUIT;	// 帮会收人
	}
	else if( tstricmp(pCom, "guildexpel") == 0 )
	{ // guildexpel =guild
		CHAR* pParam = pCommand+iPos;
		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = (INT)(atoi(pGUID));
		return COMMAND_TYPE_GUILDEXPEL;	// 帮会踢人
	}
	else if( tstricmp(pCom, "leaveguild") == 0 )
	{
		return COMMAND_TYPE_LEAVEGUILD;	// 离开帮会
	}
	else if( tstricmp(pCom, "dismissguild") == 0 )
	{
		return COMMAND_TYPE_GUILDDISMISS;	// 解散帮会
	}
	else if( tstricmp(pCom, "guildlist") == 0 )
	{ // guildlist =startindex
		CHAR* pParam = pCommand+iPos;
		CHAR* pIndex = strchr( pParam, '=' );
		if( pIndex == NULL )
		{
			pOutParam->Param0 = 0;
		}
		else
		{
			pIndex++;
			pOutParam->Param0 = (INT)(atoi(pIndex));
		}

		return COMMAND_TYPE_ASKGUILDLIST;	// 帮会列表
	}
	else if( tstricmp(pCom, "relation") == 0 )
	{ // relation
		return COMMAND_TYPE_ASKRELATIONLIST;	// 关系列表
	}
	else if( tstricmp(pCom, "addfriend") == 0 )
	{ // addfriend =friendguid =friendgroup =friendname
		CHAR* pParam = pCommand+iPos;

		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = (INT)(atoi(pGUID));

		CHAR* pGroup = strchr( pGUID, '=' );
		if( pGroup == NULL ) return COMMAND_TYPE_NONE;

		pGroup++;
		pOutParam->Param1 = (INT)(atoi(pGroup));

		CHAR* pName = strchr( pGroup, '=' );
		if( pName == NULL ) return COMMAND_TYPE_NONE;

		pName++;
		strncpy(pOutParam->m_aBuffer, pName, sizeof(pOutParam->m_aBuffer) - 1);

		return COMMAND_TYPE_ADDFRIEND;		// 加入一个好友
	}
	else if( tstricmp(pCom, "setfriendpoint") == 0 || tstricmp(pCom, "setfp") == 0 )
	{ // setfp =friendguid =friendpoint
		CHAR* pParam = pCommand+iPos;

		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = (INT)(atoi(pGUID));

		CHAR* pFriendPoint = strchr( pGUID, '=' );
		if( pFriendPoint == NULL ) return COMMAND_TYPE_NONE;

		pFriendPoint++;
		pOutParam->Param1 = (INT)(atoi(pFriendPoint));

		return COMMAND_TYPE_SETFRIENDPOINT;
	}
	else if( tstricmp(pCom, "addblackname") == 0 || tstricmp(pCom, "addblack") == 0 )
	{ // addblack =blackguid =blackname
		CHAR* pParam = pCommand+iPos;

		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = (INT)(atoi(pGUID));

		CHAR* pName = strchr( pGUID, '=' );
		if( pName == NULL ) return COMMAND_TYPE_NONE;

		pName++;
		strncpy(pOutParam->m_aBuffer, pName, sizeof(pOutParam->m_aBuffer) - 1);
		return COMMAND_TYPE_ADDBLACKNAME;	// 加入黑名单
	}
	else if( tstricmp(pCom, "delfriend") == 0 )
	{ // delfriend =friendguid =passwd
		CHAR* pParam = pCommand+iPos;
		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = (INT)(atoi(pGUID));

		CHAR* pPasswd = strchr( pGUID, '=' );
		if( pPasswd != NULL )
		{
			pPasswd++;
			strncpy(pOutParam->m_aBuffer, pPasswd, sizeof(pOutParam->m_aBuffer) - 1);
		}
		else
		{
			memset((void*)pOutParam->m_aBuffer, 0, sizeof(pOutParam->m_aBuffer));
		}

		return COMMAND_TYPE_DELFRIEND;		// 删除一个好友
	}
	else if( tstricmp(pCom, "delblack") == 0 )
	{ // delblack =blackguid
		CHAR* pParam = pCommand+iPos;
		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = (INT)(atoi(pGUID));

		return COMMAND_TYPE_DELBLACK;	// 从黑名单里删除一个玩家
	}
	else if( tstricmp(pCom, "transition") == 0 )
	{ // transition =guid =type =group
		CHAR* pParam = pCommand+iPos;
		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = (INT)(atoi(pGUID));

		CHAR* pType = strchr( pGUID, '=');
		if( pType == NULL ) return COMMAND_TYPE_NONE;

		pType++;
		pOutParam->Param1 = (INT)(atoi(pType));

		CHAR* pGroup = strchr( pType, '=' );
		if( pGroup == NULL ) return COMMAND_TYPE_NONE;

		pGroup++;
		pOutParam->Param2 = (INT)(atoi(pGroup));

		return COMMAND_TYPE_TRANSITION;	// 组间转移
	}
	else if( tstricmp(pCom, "setcamp") == 0 )
	{ // setcamp =objid =campid
		CHAR* pParam = pCommand+iPos;
		CHAR* pObjID = strchr( pParam, '=' );
		if( pObjID == NULL ) return COMMAND_TYPE_NONE;

		pObjID++;
		pOutParam->Param0 = (INT)(atoi(pObjID));

		CHAR* pCampID = strchr( pObjID, '=');
		if( pCampID == NULL ) return COMMAND_TYPE_NONE;

		pCampID++;
		pOutParam->Param1 = (INT)(atoi(pCampID));
		return COMMAND_TYPE_SETCAMPID;
	}
	else if( tstricmp(pCom, "save") == 0 )//保存用户档案
	{// save
		return COMMAND_TYPE_SAVE ;
	}
	else if( tstricmp(pCom, "getobjinfo") == 0 )
	{// getobjinfo objid=???
		CHAR* pParam = pCommand+iPos;
		CHAR* pObjID = strchr( pParam, '=' );
		if( pObjID == NULL ) return COMMAND_TYPE_NONE;

		pObjID++;
		pOutParam->Param0 = (INT)(atoi(pObjID));

		return COMMAND_TYPE_GETOBJINFO ;
	}
	else if( tstricmp(pCom, "levelup") == 0 )
	{ // levelup =level
		CHAR* pParam = pCommand+iPos;
		CHAR* pLevel = strchr( pParam, '=' );
		if( pLevel == NULL )
		{
			pOutParam->Param0 = 0;
		}
		else
		{
			pLevel++;
			pOutParam->Param0 = (INT)(atoi(pLevel));
		}

		return COMMAND_TYPE_LEVELUP;				// 升级
	}
	else if( tstricmp(pCom, "abilityup") == 0 )
	{ // abilityup =abilityid =level
		CHAR* pParam = pCommand+iPos;
		CHAR* pID = strchr( pParam, '=' );
		if( pID == NULL ) return COMMAND_TYPE_NONE;

		pID++;
		pOutParam->Param0 = (INT)(atoi(pID));

		CHAR* pLevel = strchr( pID, '=' );
		if( pLevel == NULL )
		{
			pOutParam->Param1 = 0;
		}
		else
		{
			pLevel++;
			pOutParam->Param1 = (INT)(atoi(pLevel));
		}

		return COMMAND_TYPE_ABILITYUP;				// 生活技能升级
	}
	else if( tstricmp(pCom, "who") == 0 )
	{// who
		return COMMAND_TYPE_WHO ;
	}
	else if( tstricmp(pCom, "allwho") == 0 )
	{// allwho
		return COMMAND_TYPE_ALLWHO ;
	}
	else if( tstricmp(pCom, "catchplayer") == 0 )
	{// catchplayer name=???? 或者
	 // catchplayer guid:????
		CHAR* pParam = pCommand+iPos;

		pOutParam->Param0 = 0 ;
		CHAR* pName = strchr( pParam, '=' );
		if( pName )
		{
			pOutParam->Param0 = 0 ;//名字查询
			pName++;

			strncpy( pOutParam->m_aBuffer, pName, MAX_CHARACTER_NAME-1 ) ;

			return COMMAND_TYPE_CATCHPLAYER ;
		}


		CHAR* pGUID = strchr( pParam, ':' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE ;

		CHAR* pStop ;
		pGUID ++ ;
		pOutParam->Param0 = 1 ;//GUID查询
		pOutParam->Param1 = (INT)strtoul(pGUID,&pStop,16) ;


		return 	COMMAND_TYPE_CATCHPLAYER ;
	}
	else if( tstricmp(pCom, "playerinfo") == 0 )
	{// playerinfo name=???? 或者
	 //	playerinfo guid:????
		CHAR* pParam = pCommand+iPos;

		pOutParam->Param0 = 0 ;
		CHAR* pName = strchr( pParam, '=' );
		if( pName )
		{
			pOutParam->Param0 = 0 ;//名字查询
			pName++;

			strncpy( pOutParam->m_aBuffer, pName, MAX_CHARACTER_NAME-1 ) ;

			return COMMAND_TYPE_PLAYERINFO ;
		}


		CHAR* pGUID = strchr( pParam, ':' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE ;

		CHAR* pStop ;
		pGUID ++ ;
		pOutParam->Param0 = 1 ;//GUID查询
		pOutParam->Param1 = (INT)strtoul(pGUID,&pStop,16) ;

		return COMMAND_TYPE_PLAYERINFO ;
	}
	else if ( tstricmp( pCom, "setpethappiness" ) == 0 )
	{
		CHAR* pParam = pCommand+iPos;
		CHAR* pszIndex = strchr( pParam, '=' );
		if( pszIndex == NULL ) return COMMAND_TYPE_NONE;

		pszIndex++;
		pOutParam->Param0 = (INT)(atoi(pszIndex));

		CHAR* pszHappiness = strchr( pszIndex, '=');
		if( pszHappiness == NULL ) return COMMAND_TYPE_NONE;

		pszHappiness++;
		pOutParam->Param1 = (INT)(atoi(pszHappiness));
		return COMMAND_TYPE_SETPETHAPPINESS;
	}
	else if ( tstricmp( pCom, "setpethp" ) == 0 )
	{
		CHAR* pParam = pCommand+iPos;
		CHAR* pszIndex = strchr( pParam, '=' );
		if( pszIndex == NULL ) return COMMAND_TYPE_NONE;

		pszIndex++;
		pOutParam->Param0 = (INT)(atoi(pszIndex));

		CHAR* pszHp = strchr( pszIndex, '=');
		if( pszHp == NULL ) return COMMAND_TYPE_NONE;

		pszHp++;
		pOutParam->Param1 = (INT)(atoi(pszHp));
		return COMMAND_TYPE_SETPETHP;
	}
	else if ( tstricmp( pCom, "setpetlife" ) == 0 )
	{
		CHAR* pParam = pCommand+iPos;
		CHAR* pszIndex = strchr( pParam, '=' );
		if( pszIndex == NULL ) return COMMAND_TYPE_NONE;

		pszIndex++;
		pOutParam->Param0 = (INT)(atoi(pszIndex));

		CHAR* pszLife = strchr( pszIndex, '=');
		if( pszLife == NULL ) return COMMAND_TYPE_NONE;

		pszLife++;
		pOutParam->Param1 = (INT)(atoi(pszLife));
		return COMMAND_TYPE_SETPETLIFE;
	}
	else if ( tstricmp( pCom, "petlevelup" ) == 0 )
	{
		CHAR* pParam = pCommand+iPos;
		CHAR* pszLevel = strchr( pParam, '=' );
		if( pszLevel == NULL )
		{
			pOutParam->Param0 = 1;
		}
		else
		{
			pszLevel++;
			pOutParam->Param0 = (INT)(atoi(pszLevel));
		}
		return COMMAND_TYPE_PETLEVELUP;
	}
	else if ( tstricmp( pCom, "setpetskill" ) == 0 )
	{
		CHAR* pParam = pCommand+iPos;
		CHAR* pszIndex = strchr( pParam, '=' );
		if( pszIndex == NULL ) return COMMAND_TYPE_NONE;

		pszIndex++;
		pOutParam->Param0 = (INT)(atoi(pszIndex));

		CHAR* pszSkillIndex = strchr( pszIndex, '=');
		if( pszSkillIndex == NULL ) return COMMAND_TYPE_NONE;

		pszSkillIndex++;
		pOutParam->Param1 = (INT)(atoi(pszSkillIndex));

		CHAR* pszSkill = strchr( pszSkillIndex, '=');
		if( pszSkill == NULL ) return COMMAND_TYPE_NONE;

		pszSkill++;
		pOutParam->Param2 = (INT)(atoi(pszSkill));
		return COMMAND_TYPE_SETPETSKILL;
	}
	else if ( tstricmp( pCom, "usepetskill" ) == 0 )
	{// usepetskill skillid=??? objid=???
		CHAR* pParam = pCommand+iPos;
		CHAR* pszIndex = strchr( pParam, '=' );
		if( pszIndex == NULL ) return COMMAND_TYPE_NONE;

		pszIndex++;
		pOutParam->Param0 = (INT)(atoi(pszIndex));

		CHAR* pszSkillIndex = strchr( pszIndex, '=');
		if( pszSkillIndex == NULL ) return COMMAND_TYPE_NONE;

		pszSkillIndex++;
		pOutParam->Param1 = (INT)(atoi(pszSkillIndex));

		return COMMAND_TYPE_USEPETSKILL;
	}
	else if ( tstricmp( pCom, "leveldown" ) == 0 )
	{// LevelDown =levelwillbecut
		CHAR* pParam = pCommand+iPos;

		CHAR* pDownLevel = strchr( pParam, '=' );
		if( pDownLevel == NULL )
		{
			pOutParam->Param0 = 1;
		}
		else
		{
			pDownLevel++;
			pOutParam->Param0 = (INT)(atoi(pDownLevel));
		}

		return COMMAND_TYPE_LEVELDOWN;
	}
	else if ( tstricmp( pCom, "addexp" ) == 0 )
	{// AddExp =Expwillbeadded
		CHAR* pParam = pCommand+iPos;

		CHAR* pExp = strchr( pParam, '=' );
		if( pExp == NULL ) return COMMAND_TYPE_NONE;

		pExp++;
		pOutParam->Param0 = (INT)(atoi(pExp));

		return COMMAND_TYPE_ADDEXP;
	}
	else if ( tstricmp( pCom, "doubleexp" ) == 0 )
	{// AddExp =Expwillbeadded
		CHAR* pParam = pCommand+iPos;

		CHAR* pExp = strchr( pParam, '=' );
		if( pExp == NULL ) return COMMAND_TYPE_NONE;

		pExp++;
		pOutParam->Param0 = (INT)(atoi(pExp));

		CHAR* pExpNum = strchr( pExp, '=' );
		if( pExpNum == NULL ) return COMMAND_TYPE_NONE;

		pExpNum++;
		pOutParam->Param1 = (INT)(atoi(pExpNum));

		return COMMAND_TYPE_DOUBLEEXP;
	}
	else if ( tstricmp( pCom, "CityCreate" ) == 0 )
	{// 建立城市 =入口场景号
		CHAR* pParam = pCommand+iPos;

		CHAR* pSceneID = strchr( pParam, '=' );
		if( pSceneID == NULL ) return COMMAND_TYPE_NONE;

		pSceneID++;
		pOutParam->Param0 = (INT)(atoi(pSceneID));

		return COMMAND_TYPE_CREATECITY;
	}
	else if ( tstricmp( pCom, "CityDelete" ) == 0 )
	{// 建立城市 =入口场景号
		CHAR* pParam = pCommand+iPos;

		CHAR* pPortSceneID = strchr( pParam, '=' );
		if( pPortSceneID == NULL ) return COMMAND_TYPE_NONE;

		pPortSceneID++;
		pOutParam->Param0 = (INT)(atoi(pPortSceneID));

		CHAR* pPortSceneIndex = strchr( pPortSceneID, '=' );
		if( pPortSceneIndex == NULL ) return COMMAND_TYPE_NONE;

		pPortSceneIndex++;
		pOutParam->Param1 = (INT)(atoi(pPortSceneIndex));

		return COMMAND_TYPE_DELETECITY;
	}
	else if ( tstricmp( pCom, "loadmonster" ) == 0 )
	{// loadmonster filename
		CHAR* pParam = pCommand+iPos ;

		strncpy( pOutParam->m_aBuffer, pParam+1, COMMAND_BUFFER_SIZE-1 ) ;

		return COMMAND_TYPE_LOADMONSTER;
	}
	else if ( tstricmp( pCom, "sethaircolor" ) == 0 )
	{
		CHAR* pParam = pCommand+iPos;

		CHAR* pszA = strchr( pParam, '=' );
		if( pszA == NULL ) return COMMAND_TYPE_NONE;
		pszA++;
		pOutParam->Param0 = (INT)(atoi(pszA));

		CHAR* pszR = strchr( pszA, '=' );
		if( pszR == NULL ) return COMMAND_TYPE_NONE;
		pszR++;
		pOutParam->Param1 = (INT)(atoi(pszR));

		CHAR* pszG = strchr( pszR, '=' );
		if( pszG == NULL ) return COMMAND_TYPE_NONE;
		pszG++;
		pOutParam->Param2 = (INT)(atoi(pszG));

		CHAR* pszB = strchr( pszG, '=' );
		if( pszB == NULL ) return COMMAND_TYPE_NONE;
		pszB++;
		pOutParam->Param3 = (INT)(atoi(pszB));

		return COMMAND_TYPE_SETHAIRCOLOR;
	}
	else if ( tstricmp( pCom, "sethairmodel" ) == 0 )
	{
		CHAR* pParam = pCommand+iPos;

		CHAR* pszModel = strchr( pParam, '=' );
		if( pszModel == NULL ) return COMMAND_TYPE_NONE;
		pszModel++;
		pOutParam->Param0 = (INT)(atoi(pszModel));

		return COMMAND_TYPE_SETHAIRMODEL;
	}
	else if ( tstricmp( pCom, "createguard" ) == 0 )
	{// createhelper modelid=???
		CHAR* pParam = pCommand+iPos;

		CHAR* pszModel = strchr( pParam, '=' );
		if( pszModel == NULL ) return COMMAND_TYPE_NONE;
		pszModel++;
		pOutParam->Param0 = (INT)(atoi(pszModel));

		return COMMAND_TYPE_CREATEGUARD;
	}
	else if (tstricmp(pCom, "createtrap") == 0)
	{
		CHAR* pParam = pCommand+iPos;

		CHAR* pszTrapID = strchr( pParam, '=' );
		if( pszTrapID == NULL ) return COMMAND_TYPE_NONE;
		pszTrapID++;
		pOutParam->Param0 = (INT)(atoi(pszTrapID));

		return COMMAND_TYPE_CREATETRAP;
	}
	else if (tstricmp(pCom, "forgetrecipe") == 0)
	{
		CHAR* pParam = pCommand+iPos;

		CHAR* pRecipeID = strchr( pParam, '=' );
		if( pRecipeID == NULL ) return COMMAND_TYPE_NONE;

		pRecipeID++;
		pOutParam->Param0 = (INT)(atoi(pRecipeID));

		return COMMAND_TYPE_FORGETRECIPE;
	}
	else if (tstricmp(pCom, "SendImpactToUnit") == 0)
	{
		CHAR* pParam = pCommand+iPos;
		CHAR* pObjID = strchr( pParam, '=' );
		if( pObjID == NULL ) return COMMAND_TYPE_NONE;

		pObjID++;
		pOutParam->Param0 = (INT)(atoi(pObjID));

		CHAR* pImpactDataIndex = strchr( pObjID, '=' );
		if( pImpactDataIndex == NULL ) return COMMAND_TYPE_NONE;

		pImpactDataIndex++;
		pOutParam->Param1 = (INT)(atoi(pImpactDataIndex));

		return COMMAND_TYPE_SEND_IMPACT_TO_UNIT;
	}
	else if (tstricmp(pCom, "ReloadCombatTables") == 0)
	{
		return COMMAND_TYPE_RELOAD_COMBAT_TABLES;
	}
	else if (tstricmp(pCom, "UseSkill") == 0)
	{
		CHAR* pParam = pCommand+iPos;

		CHAR* pszMagicName = strchr( pParam, '=' );
		if( pszMagicName == NULL ) return COMMAND_TYPE_NONE;

		pszMagicName++;
		strncpy(pOutParam->m_aBuffer, pszMagicName, sizeof(pOutParam->m_aBuffer));
		INT i = 0;
		INT nLen = (INT)(strlen(pOutParam->m_aBuffer));
		while(i <= nLen)
		{
			if(pOutParam->m_aBuffer[i] == ' ')
			{
				pOutParam->m_aBuffer[i] = 0;
				break;
			}
			i++;
		}

		pOutParam->Param0	= INVALID_ID;
		pOutParam->Param1	= INVALID_GUID;
		pOutParam->fParam0	= -1.f;
		pOutParam->fParam1	= -1.f;
		pOutParam->fParam2	= -1.f;

		CHAR* pszTargetID = strchr( pszMagicName, '=' );
		if( pszTargetID == NULL ) return COMMAND_TYPE_USE_SKILL;
		pszTargetID++;
		pOutParam->Param0 = (INT)(atoi(pszTargetID));

		CHAR* pszTargetPosX = strchr( pszTargetID, '=' );
		if( pszTargetPosX == NULL ) return COMMAND_TYPE_USE_SKILL;
		pszTargetPosX++;
		pOutParam->fParam0 = (FLOAT)(atof(pszTargetPosX));

		CHAR* pszTargetPosZ = strchr( pszTargetPosX, ',' );
		if( pszTargetPosZ == NULL ) return COMMAND_TYPE_USE_SKILL;
		pszTargetPosZ++;
		pOutParam->fParam1 = (FLOAT)(atof(pszTargetPosZ));

		CHAR* pszDir = strchr( pszTargetPosZ, '=' );
		if( pszDir == NULL ) return COMMAND_TYPE_USE_SKILL;
		pszDir++;
		pOutParam->fParam2 = (FLOAT)(atof(pszDir));

		CHAR* pszTargetGUID = strchr( pszDir, '=' );
		if( pszTargetGUID == NULL ) return COMMAND_TYPE_USE_SKILL;
		pszTargetGUID++;
		pOutParam->Param1 = (INT)(atoi(pszTargetGUID));

		return COMMAND_TYPE_USE_SKILL;
	}
	else if (tstricmp(pCom, "savepacketinfo") == 0)
	{
		return COMMAND_TYPE_SAVEPACKETINFO ;
	}
	//else if (tstricmp(pCom, "SetVigor") == 0)
	//{
	//	CHAR* pParam = pCommand+iPos;
	//	CHAR* pValue = strchr( pParam, '=' );
	//	if( pValue == NULL ) return COMMAND_TYPE_NONE;

	//	pValue++;
	//	pOutParam->Param0 = (INT)(atoi(pValue));

	//	return COMMAND_TYPE_SETVIGOR;
	//}
	//else if (tstricmp(pCom, "SetEnergy") == 0)
	//{
	//	CHAR* pParam = pCommand+iPos;
	//	CHAR* pValue = strchr( pParam, '=' );
	//	if( pValue == NULL ) return COMMAND_TYPE_NONE;

	//	pValue++;
	//	pOutParam->Param0 = (INT)(atoi(pValue));

	//	return COMMAND_TYPE_SETENERGY;
	//}
	else if (tstricmp(pCom, "PetPullulate") == 0)
	{
		CHAR* pParam = pCommand+iPos;
		CHAR* pszPetIndex = strchr( pParam, '=' );
		if( pszPetIndex == NULL ) return COMMAND_TYPE_NONE;

		pszPetIndex++;
		pOutParam->Param0 = (INT)(atoi(pszPetIndex));

		return COMMAND_TYPE_PETPULLULATE;
	}
	else if( tstricmp( pCom, "addskill")==0 )
	{//addskill =skilll_id
		CHAR* pParam = pCommand+iPos ;

		CHAR* pSkill = strchr( pParam, '=' ) ;
		if( pSkill==NULL ) return COMMAND_TYPE_NONE ;

		pSkill ++ ;
		pOutParam->Param0 = atoi( pSkill ) ;

		CHAR* pLevel = strchr( pSkill, '=' );
		if( pLevel == NULL ) 
			pOutParam->Param1 = 1;
		else
		{
			pLevel++;
			pOutParam->Param1 = atoi(pLevel);
		}

		return COMMAND_TYPE_ADD_SKILL ;
	}
	else if( tstricmp( pCom, "removeskill")==0 )
	{//removeskill =skilll_id
		CHAR* pParam = pCommand+iPos ;

		CHAR* pSkill = strchr( pParam, '=' ) ;
		if( pSkill==NULL ) return COMMAND_TYPE_NONE ;

		pSkill ++ ;
		pOutParam->Param0 = atoi( pSkill ) ;

		return COMMAND_TYPE_REMOVE_SKILL ;
	}
	else if (tstricmp(pCom, "setgmright") == 0)
	{ // fingerguid =guid =onlineflag
		CHAR* pParam = pCommand+iPos;

		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = atoi(pGUID);

		CHAR* pOnlineFlag = strchr( pGUID, '=' );
		if( pOnlineFlag == NULL ) return COMMAND_TYPE_NONE;

		pOnlineFlag++;
		pOutParam->Param1 = atoi(pOnlineFlag);

		return COMMAND_TYPE_SETGMRIGHT;
	}
	else if (tstricmp(pCom, "gmtest") == 0)
	{ 
		return COMMAND_TYPE_GMTEST;
	}
	else if (tstricmp(pCom, "guid") == 0)
	{ 
		return COMMAND_TYPE_SHOWGUID;
	}
	else if (tstricmp(pCom, "fingerguid") == 0)
	{ 
		CHAR* pParam = pCommand+iPos;

		CHAR* pGUID = strchr( pParam, '=' );
		if( pGUID == NULL ) return COMMAND_TYPE_NONE;

		pGUID++;
		pOutParam->Param0 = atoi(pGUID);

		CHAR* pOnlineFlag = strchr( pGUID, '=' );
		if( pOnlineFlag == NULL ) return COMMAND_TYPE_NONE;

		pOnlineFlag++;
		pOutParam->Param1 = atoi(pOnlineFlag);

		return COMMAND_TYPE_FINGERGUID;
	}
	else if (tstricmp(pCom, "fingername") == 0)
	{ // fingername opt=OnlineFlag,PreciseFlag,Position name=name
		CHAR* pParam = pCommand+iPos;

		CHAR* pOption = strchr( pParam, '=' );
		if( pOption == NULL ) return COMMAND_TYPE_NONE;

		CHAR* pOnlineFlag = pOption + 1;
		pOutParam->Param0 = atoi(pOnlineFlag);

		CHAR* pPreciseFlag = strchr( pOnlineFlag, ',' );
		if( pPreciseFlag == NULL ) return COMMAND_TYPE_NONE;

		pPreciseFlag++;
		pOutParam->Param1 = atoi(pPreciseFlag);

		CHAR* pPosition = strchr( pPreciseFlag, ',' );
		if( pPosition == NULL ) return COMMAND_TYPE_NONE;

		pPosition++;
		pOutParam->Param2 = atoi(pPosition);

		CHAR* pName = strchr( pPosition, '=' );
		if( pPosition == NULL ) return COMMAND_TYPE_NONE;

		pName++;
		strncpy( pOutParam->m_aBuffer, pName, MAX_CHARACTER_NAME-1 );

		return COMMAND_TYPE_FINGERNAME;
	}
	else if (tstricmp(pCom, "advancedfinger") == 0)
	{ // advancedfinger =pos =menpai =guildid =sex =fromlevel,tolevel
		CHAR* pParam = pCommand+iPos;

		CHAR* pPosition = strchr( pParam, '=' );
		if( pPosition == NULL ) return COMMAND_TYPE_NONE;

		pPosition++;
		pOutParam->Param0 = atoi(pPosition);

		CHAR* pMenpai = strchr( pPosition, '=' );
		if( pMenpai == NULL ) return COMMAND_TYPE_NONE;

		pMenpai++;
		pOutParam->Param1 = atoi(pMenpai);

		CHAR* pGuildID = strchr( pMenpai, '=' );
		if( pGuildID == NULL ) return COMMAND_TYPE_NONE;

		pGuildID++;
		pOutParam->Param2 = atoi(pGuildID);

		CHAR* pSex = strchr( pGuildID, '=' );
		if( pSex == NULL ) return COMMAND_TYPE_NONE;

		pSex++;
		pOutParam->Param3 = atoi(pSex);

		CHAR* pLevelFrom = strchr( pSex, '=' );
		if( pLevelFrom == NULL ) return COMMAND_TYPE_NONE;

		pLevelFrom++;
		pOutParam->fParam0 = (FLOAT)atoi(pLevelFrom);

		CHAR* pLevelTo = strchr( pLevelFrom, ',' );
		if( pLevelTo == NULL ) return COMMAND_TYPE_NONE;

		pLevelTo++;
		pOutParam->fParam1 = (FLOAT)atoi(pLevelTo);

		return COMMAND_TYPE_ADVANCEDFINGER;
	}
	else if( tstricmp(pCom, "setabilityexp") == 0 )
	{ // setabilityexp =abilityid =value
		CHAR* pParam = pCommand+iPos;
		CHAR* pID = strchr( pParam, '=' );
		if( pID == NULL ) return COMMAND_TYPE_NONE;

		pID++;
		pOutParam->Param0 = (INT)(atoi(pID));

		CHAR* pValue = strchr( pID, '=' );
		if( pValue == NULL )
		{
			pOutParam->Param1 = 0;
		}
		else
		{
			pValue++;
			pOutParam->Param1 = (INT)(atoi(pValue));
		}

		return COMMAND_TYPE_SETABILITYEXP;				// 生活技能升级
	}
	else if( tstricmp(pCom, "setrmb") == 0 )
	{	// setgoodbad =value
		CHAR* pParam = pCommand+iPos;
		CHAR* pValue = strchr( pParam, '=' );
		if( pValue == NULL ) return COMMAND_TYPE_NONE;

		pValue++;
		pOutParam->Param0 = (INT)(atoi(pValue));

		return COMMAND_TYPE_SERMBMONEY;
	}
	else
	{
	}
	return COMMAND_TYPE_NONE ;
};




static BOOL s_bEnableGod = TRUE;
uint CGCommandHandler::Execute( CGCommand* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	GM_EXECUTE_LEVEL gm_level;
	gm_level = (GM_EXECUTE_LEVEL)pHuman->GetGmRight();
	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	if( !CommandSafeCheck( pPacket->GetCommand(), pPacket->GetCommandSize() ) ) return PACKET_EXE_CONTINUE ;

	COMMAND_PARAM Out ;
	COMMAND_TYPE Command = Calc_Command( pPacket->GetCommand(), pPacket->GetCommandSize(), &Out ) ;
	
	BOOL GMCommand=TRUE ;
	if( !g_Command_CommandDeny )
	{
		GMCommand=FALSE ;
		if( !g_pServerManager->IsEnableShareMem() )
		{
			GMCommand=TRUE ;
		}
		else
		{
			for( INT i=0; i<g_GMTbl.m_uCount; i++ )
			{
				if( g_GMTbl.m_aGUID[i]==pGamePlayer->m_HumanGUID )
				{
					GMCommand = TRUE ;
					break ;
				}
			}
			GMCommand = TRUE; //__??
		}
	}
	if(	g_Command_CommandDeny || GMCommand==FALSE )//禁止使用GM指令
	{
		if( Command!=COMMAND_TYPE_NONE )
		{
			CHAR szContex[256] ;
			sprintf( szContex, "无法使用GM指令！当前模式不支持！" ) ;
			GCChat Msg ;
			Msg.SetChatType( CHAT_TYPE_SYSTEM ) ;
			Msg.SetContex( szContex ) ;
			Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
			Msg.SetSourCamp( INVALID_CAMP );

			pHuman->GetPlayer()->SendPacket( &Msg ) ;
		}

		g_pLog->FastSaveLog( LOG_FILE_1, "CGCommand::GUID=%X, Obj=%d, \"%s\" Deny! ", 
			pHuman->GetGUID(), pHuman->GetID(), pPacket->GetCommand() ) ;

		return PACKET_EXE_CONTINUE ;
	}

	if(!Calc_GmRight(gm_level,Command))
	{
		return PACKET_EXE_CONTINUE ;
	}

	switch( Command )
	{
	case COMMAND_TYPE_NONE:
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Not Valid Command!" ) ;
		}
		break ;
	case COMMAND_TYPE_TELEPORT:
		{
			ObjID_t ObjID = (ObjID_t)(Out.Param0) ;
			Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;
			if( pObj == NULL )
				break ;
			if( !IsCharacterObj( pObj->GetObjType() ) )
				break ;

			Obj_Character* pCharacter = (Obj_Character*)pObj ;
			WORLD_POS New ;
			New.m_fX = Out.fParam0 ;
			New.m_fZ = Out.fParam1 ;

			if( Out.Param1!=INVALID_ID )
			{//切换场景
			 //向此玩家发送可以转移场景消息
				if( pCharacter->GetObjType() != Obj::OBJ_TYPE_HUMAN )
					break ;

				if( pScene->SceneID()==Out.Param1 )
					break ;

				Obj_Human* pTargetHuman = (Obj_Human*)pCharacter ;

				pTargetHuman->ChangeScene( pScene->SceneID(), Out.Param1, New, 9 ) ;

				//GCNotifyChangeScene Msg ;
				//Msg.setCurrentSceneID( pScene->SceneID() ) ;
				//Msg.setTargetSceneID( Out.Param1 ) ;
				//Msg.setTargetPos( &New ) ;
				//Msg.setTargetDir( 9 ) ;
				//	
				//pGamePlayer->SendPacket( &Msg ) ;

			}
			else
			{//同场景内转移

				WORLD_POS Old = *pObj->getWorldPos() ;

				pScene->GetMap()->VerifyPos( &New ) ;
				if( !pScene->GetMap()->IsCanGo( New,pCharacter->GetDriverLevel() ) )
					break ;

				((Obj_Human*)(pCharacter))->GetHumanAI()->PushCommand_Idle( ) ;

				pCharacter->Teleport( &New ) ;

			}
		}
		break ;
	case COMMAND_TYPE_MODIFYMENPAI:
		{
			INT Menpai = Out.Param0 ;
			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;

			SkillLogParam.OPType	=	SKILL_OP_GM_MODIFY;

			pHuman->ChangeMenpai(&SkillLogParam,Menpai);
			pHuman->ChangeMenpaiPoints();
		}
		break ;
	case COMMAND_TYPE_MODIFYXINFA:
		{
			//BOOL bModify = FALSE ;
			//pHuman->Skill_SetXinFaLevel(Out.Param0, Out.Param1);
			////通知
			//GCSkillClass	Msg;
			//Msg.setObjID( pHuman->GetID() );
			//Msg.setXinFaList(pHuman->Skill_GetXinFaList().m_Count, pHuman->Skill_GetXinFaList().m_aXinFa);
			//pGamePlayer->SendPacket( &Msg ) ;

			//XINFA_LOG_PARAM XinFaLogParam;
			//XinFaLogParam.CharGUID	=	pHuman->GetGUID();
			//XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
			//XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			//XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			//XinFaLogParam.OPType	=	XINFA_OP_GM_MODIFY;
			//XinFaLogParam.Count		=	pHuman->Skill_GetXinFaList().m_Count;
			//XinFaLogParam.XinFaID	=	Out.Param0;
			//XinFaLogParam.Level		=	Out.Param1;

			//SaveXinFaLog(&XinFaLogParam);
			
		}
		break ;
	case COMMAND_TYPE_LEVELUPALLXINFA:
		{
			//BOOL bModify = FALSE ;
			//pHuman->Skill_AllXinFaLevel(Out.Param0);
			////通知客户端
			//GCSkillClass	Msg;
			//Msg.setObjID( pHuman->GetID() );
			////Msg.setXinFa( (WORD)(pHuman->GetDB()->m_dbXinFa->m_Count), pHuman->GetDB()->m_dbXinFa->m_aXinFa );
			//Msg.setXinFaList(pHuman->Skill_GetXinFaList().m_Count, pHuman->Skill_GetXinFaList().m_aXinFa);
			//pGamePlayer->SendPacket( &Msg ) ;
			//
			//XINFA_LOG_PARAM XinFaLogParam;
			//XinFaLogParam.CharGUID	=	pHuman->GetGUID();
			//XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
			//XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			//XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			//XinFaLogParam.OPType	=	XINFA_OP_GM_MODIFY;
			//XinFaLogParam.Count		=	pHuman->Skill_GetXinFaList().m_Count;
			//XinFaLogParam.Level		=	Out.Param0;

			//SaveXinFaLog(&XinFaLogParam);

		}
		break ;
	case COMMAND_TYPE_CREATEITEM:
		{
		
			
			_ITEM item;
			uint BagIndex;
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType		= ITEM_CREATE_GM;
			ItemLogParam.CharGUID	= pHuman->GetGUID();
			ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
			ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
			ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;


			HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,Out.Param0,BagIndex,Out.Param1);
			if(BagIndex==INVALID_INDEX) //创建失败
			{
				return PACKET_EXE_CONTINUE;
			}
			SaveItemLog(&ItemLogParam);
			Item*	pDestItem = HumanItemLogic::GetItem(pHuman,BagIndex);
			Assert(pDestItem);

			//通知
			GCNotifyEquip Msg ;
			Msg.SetBagIndex( BagIndex ) ;

			pDestItem->SaveValueTo(Msg.GetItem());
			pGamePlayer->SendPacket( &Msg ) ;
		}
		break ;
	
	case COMMAND_TYPE_RELIVE:
		{
			if( pHuman->IsAlive() )
				break ;

			Obj_Human::_RELIVE_INFO infoRelive;
			infoRelive.m_Pos			= *(pHuman->getWorldPos());
			infoRelive.m_SceneID		= (pHuman->getScene() != NULL)?(pHuman->getScene()->SceneID()):(-1);
			infoRelive.m_bHPUsePercent	= FALSE;
			infoRelive.m_nHP			= 1;
			infoRelive.m_bMPUsePercent	= FALSE;
			infoRelive.m_nMP			= 0;
			infoRelive.m_bSPUsePercent	= FALSE;
			infoRelive.m_nSP			= 0;
			pHuman->SetReliveInfo( TRUE, &infoRelive );
			pHuman->MarkAliveFlag();
			((AI_Human*)(pHuman->GetAIObj()))->PushCommand_DieResult( DIE_RESULT_CODE_RELIVE ) ;
		}
		break ;
	case COMMAND_TYPE_CHAT:
		{
			//GCChat Msg ;
			//INT Size = (INT)strlen(Out.m_aBuffer) ;
			//Msg.SetChatType( CHAT_TYPE_NORMAL ) ;
			//Msg.SetContexSize( Size ) ;
			//Msg.SetContex( Out.m_aBuffer ) ;

			//pScene->BroadCast_Scene( &Msg ) ;
		}
		break ;
	case COMMAND_TYPE_ENABLEGOD:
		{
			s_bEnableGod = TRUE;
		}
		break;
	case COMMAND_TYPE_DISABLEGOD:
		{
			s_bEnableGod = FALSE;
		}
		break;
	case COMMAND_TYPE_IAMGOD:
		{
			if ( s_bEnableGod )
			{
				pHuman->SetGod( TRUE ) ;
			}
		}
		break ;
	case COMMAND_TYPE_IAMDOG:
		{
			pHuman->SetGod( FALSE ) ;
		}
		break ;
	case COMMAND_TYPE_DEBUG_RELOADCONFIG:
		{
			g_TableInit.InitGMTable() ;
		}
		break ;
	case COMMAND_TYPE_SYSCHAT:
		{
	
			BYTE nChatLen = (BYTE)strlen(Out.m_aBuffer);
			if( nChatLen >= MAX_CHAT_SIZE ) nChatLen = MAX_CHAT_SIZE-1;

			GWSystemMsg* pPacket = (GWSystemMsg*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_SYSTEMMSG)) ;
			pPacket->SetGUID( pGamePlayer->m_HumanGUID );
			pPacket->SetMessageType( GWSystemMsg::SYSTEMMSG_TYPE_GM );
			pPacket->SetContexSize( nChatLen );
			pPacket->SetContex(Out.m_aBuffer) ;
			g_pServerManager->SendPacket( pPacket, INVALID_ID) ;


		}
		break ;
	case COMMAND_TYPE_DEBUG_SHUTDOWN:
		{
			g_Server.Stop( ) ;
		}
		break ;
	case COMMAND_TYPE_TEAM_INVITE:
		{
			GWTeamInvite* pMsg = (GWTeamInvite*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMINVITE)) ;
			pMsg->SetSourGUID(  (GUID_t)(Out.Param0) ) ;
			pMsg->SetDestName( (CHAR*)(Out.m_aBuffer) ) ;
			pMsg->SetGUID( pGamePlayer->m_HumanGUID ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_TEAM_RETINVITE:
		{
			GWTeamRetInvite* pMsg = (GWTeamRetInvite*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMRETINVITE)) ;
			pMsg->SetReturn( (BOOL)(Out.Param0) ) ;
			pMsg->SetSourGUID( (GUID_t)(Out.Param1) ) ;
			pMsg->SetDestGUID( pGamePlayer->m_HumanGUID ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_TEAM_LEADERRETINVITE:
		{
			GWTeamLeaderRetInvite* pMsg = (GWTeamLeaderRetInvite*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMLEADERRETINVITE)) ;
			pMsg->SetReturn( (BOOL)(Out.Param0) ) ;
			pMsg->SetSourGUID( (GUID_t)(Out.Param1) ) ;
			pMsg->SetDestGUID( (GUID_t)(Out.Param2) ) ;
			pMsg->SetLeaderGUID( pGamePlayer->m_HumanGUID ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_TEAM_APPLY:
		{
			GWTeamApply* pMsg = (GWTeamApply*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMAPPLY)) ;
			pMsg->SetSourGUID( (GUID_t)(Out.Param0) ) ;
			pMsg->SetDestName( Out.m_aBuffer ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_TEAM_RETAPPLY:
		{
			GWTeamRetApply* pMsg = (GWTeamRetApply*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMRETAPPLY)) ;
			pMsg->SetReturn( (BOOL)(Out.Param0) ) ;
			pMsg->SetSourGUID( (GUID_t)(Out.Param1) ) ;
			pMsg->SetLeaderGUID( pGamePlayer->m_HumanGUID ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_TEAM_LEAVE:
		{
			GWTeamLeave* pMsg = (GWTeamLeave*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMLEAVE)) ;
			pMsg->SetGUID( (GUID_t)(Out.Param0) ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_TEAM_KICK:
		{
			GWTeamKick* pMsg = (GWTeamKick*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMKICK)) ;
			pMsg->SetSourGUID( (GUID_t)(Out.Param0) ) ;
			pMsg->SetDestGUID( (GUID_t)(Out.Param1) ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_TEAM_DISMISS:
		{
			GWTeamDismiss* pMsg = (GWTeamDismiss*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMDISMISS)) ;
			pMsg->SetGUID( (GUID_t)(Out.Param0) ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_TEAM_APPOINT:
		{
			GWTeamAppoint* pMsg = (GWTeamAppoint*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMAPPOINT)) ;
			pMsg->SetSourGUID( (GUID_t)(Out.Param0) ) ;
			pMsg->SetDestGUID( (GUID_t)(Out.Param1) ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_ABILITY_DETAIL:
		{
			CGAskDetailAbilityInfo* pMsg = (CGAskDetailAbilityInfo*)(g_pPacketFactoryManager->CreatePacket(PACKET_CG_ASKDETAILABILITYINFO));
			pMsg->setTargetID( (ObjID_t)(Out.Param0) );
			CGAskDetailAbilityInfoHandler::Execute(pMsg, pHuman->GetPlayer());
			g_pPacketFactoryManager->RemovePacket( pMsg ) ;
		}
		break;
	case COMMAND_TYPE_USE_ABILITY:
		{
			CGUseAbility* pMsg = (CGUseAbility*)(g_pPacketFactoryManager->CreatePacket(PACKET_CG_USEABILITY));
			pMsg->SetAbilityID( (AbilityID_t)(Out.Param0) );
			pMsg->SetPrescriptionID( (PrescriptionID_t)(Out.Param1) );
			pMsg->SetPlatformGUID( (ObjID_t)(Out.Param2) );
			CGUseAbilityHandler::Execute( pMsg, pHuman->GetPlayer() );
			g_pPacketFactoryManager->RemovePacket( pMsg ) ;
		}
		break;
	case COMMAND_TYPE_COMBOUND_GEM:
		{
			CGGemCompound* pMsg = (CGGemCompound*)(g_pPacketFactoryManager->CreatePacket(PACKET_CG_GEM_COMPOUND));
			pMsg->SetGemBagIndex1( (BYTE)Out.Param0 );
			pMsg->SetGemBagIndex2( (BYTE)Out.Param1 );
			CGGemCompoundHandler::Execute( pMsg, pHuman->GetPlayer() );
			g_pPacketFactoryManager->RemovePacket( pMsg ) ;
		}
		break;
	case COMMAND_TYPE_USE_GEM:
		{
			CGUseGem* pMsg = (CGUseGem*)(g_pPacketFactoryManager->CreatePacket(PACKET_CG_USEGEM));
			pMsg->SetGemBagIndex( (BYTE)Out.Param0 );
			pMsg->SetEquipBagIndex( (BYTE)Out.Param1 );
			CGUseGemHandler::Execute( pMsg, pHuman->GetPlayer() );
			g_pPacketFactoryManager->RemovePacket( pMsg ) ;
		}
		break;
	case COMMAND_TYPE_CHANNEL_CREATE://自建聊天频道-创建
		{
			GWChannelCreate* pMsg = (GWChannelCreate*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHANNELCREATE)) ;
			pMsg->SetGUID( (GUID_t)(Out.Param0) ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_CHANNEL_INVITE://自建聊天频道-邀请
		{
			GWChannelInvite* pMsg = (GWChannelInvite*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHANNELINVITE)) ;
			pMsg->SetSourGUID( (GUID_t)(Out.Param0) ) ;//邀请人
			pMsg->SetDestGUID( (GUID_t)(Out.Param1) ) ;//被邀请人

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_CHANNEL_DISMISS://自建聊天频道-解散
		{
			GWChannelDismiss* pMsg = (GWChannelDismiss*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHANNELDISMISS)) ;
			pMsg->SetGUID( (GUID_t)(Out.Param0) ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_CHANNEL_KICK://自建聊天频道-踢除成员
		{
			GWChannelKick* pMsg = (GWChannelKick*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHANNELKICK)) ;
			pMsg->SetSourGUID( (GUID_t)(Out.Param0) ) ;//踢人者
			pMsg->SetDestGUID( (GUID_t)(Out.Param1) ) ;//被踢者

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break;
	case COMMAND_TYPE_GOTO:
		{
			Obj* pObj = pHuman ;
			if( pObj == NULL )
				break ;
			if( !IsCharacterObj( pObj->GetObjType() ) )
				break ;

			Obj_Character* pCharacter = (Obj_Character*)pObj ;
			WORLD_POS New ;
			New.m_fX = Out.fParam0 ;
			New.m_fZ = Out.fParam1 ;

			if( Out.Param0!=INVALID_ID )
			{//切换场景
			 //向此玩家发送可以转移场景消息
				if( pCharacter->GetObjType() != Obj::OBJ_TYPE_HUMAN )
					break ;

				if( pScene->SceneID()==Out.Param0 )
					break ;

				Obj_Human* pTargetHuman = (Obj_Human*)pCharacter ;

				pTargetHuman->ChangeScene( pScene->SceneID(), Out.Param0, New, 9 ) ;

				//GCNotifyChangeScene Msg ;
				//Msg.setCurrentSceneID( pScene->SceneID() ) ;
				//Msg.setTargetSceneID( Out.Param0 ) ;
				//Msg.setTargetPos( &New ) ;
				//Msg.setTargetDir( 9 ) ;
				//	
				//pGamePlayer->SendPacket( &Msg ) ;

			}
			else
			{//同场景内转移

				WORLD_POS Old = *pObj->getWorldPos() ;

				pScene->GetMap()->VerifyPos( &New ) ;
				if( !pScene->GetMap()->IsCanGo( New, pCharacter->GetDriverLevel() ) )
					break ;

				((Obj_Human*)(pCharacter))->GetHumanAI()->PushCommand_Idle( ) ;

				////通知周围玩家，此Obj要被移走了
				//GCDelObject MsgLeave ;
				//MsgLeave.setObjID( pCharacter->GetID() ) ;
				//pScene->BroadCast( &MsgLeave, pCharacter ) ;

				pCharacter->Teleport( &New ) ;

				////通知新位置的周围玩家，此Obj进来了
				//if( pCharacter->GetObjType()==Obj::OBJ_TYPE_HUMAN )
				//{
				//	Obj_Human* pDestHuman = (Obj_Human*)pCharacter ;

				//	GCNewPlayer MsgEnter ;
				//	MsgEnter.setObjID( pDestHuman->GetID() ) ;
				//	MsgEnter.setWorldPos( *(pDestHuman->getWorldPos()) ) ;
				//	MsgEnter.setEquipVer( pDestHuman->GetEquipVer() ) ;

				//	pScene->BroadCast( &MsgEnter, pDestHuman, TRUE ) ;

				//	ZoneID_t zid = pScene->CalcZoneID( pDestHuman->getWorldPos() ) ;
				//	pScene->ScanEnv( pDestHuman, zid ) ;

				//}
			}
		}
		break ;
	case COMMAND_TYPE_FULLRECOVER :
		{
			if(pHuman->IsDie())
			{
				Obj_Human::_RELIVE_INFO infoRelive;
				infoRelive.m_Pos			= *(pHuman->getWorldPos());
				infoRelive.m_SceneID		= (pHuman->getScene() != NULL)?(pHuman->getScene()->SceneID()):(-1);
				infoRelive.m_bHPUsePercent	= FALSE;
				infoRelive.m_nHP			= 1;
				infoRelive.m_bMPUsePercent	= FALSE;
				infoRelive.m_nMP			= 0;
				infoRelive.m_bSPUsePercent	= FALSE;
				infoRelive.m_nSP			= 0;
				pHuman->SetReliveInfo( TRUE, &infoRelive );
				pHuman->MarkAliveFlag();
				((AI_Human*)(pHuman->GetAIObj()))->PushCommand_DieResult( DIE_RESULT_CODE_RELIVE ) ;
			}
			INT iMaxHP = pHuman->GetMaxHP() ;
			pHuman->HealthIncrement(iMaxHP);
			INT iMaxMP = pHuman->GetMaxMP() ;
			pHuman->ManaIncrement(iMaxMP);
			int iMaxRage = pHuman->GetMaxRage();
			pHuman->RageIncrement(iMaxRage);
			int iMaxStrikePoint = pHuman->GetMaxStrikePoint();
			pHuman->StrikePointIncrement(iMaxStrikePoint);
		}
		break ;
	case COMMAND_TYPE_CREATEPET:
		{
			INT idModel = Out.Param0 ;
			WORLD_POS posHuman = *(pHuman->getWorldPos());
			posHuman.m_fX -= 2.f;
			posHuman.m_fZ -= 2.f;
			pHuman->getScene()->GetMap()->VerifyPos(&posHuman);
			pHuman->getScene()->GetMap()->IsCanGo(posHuman,pHuman->GetDriverLevel());
			ObjID_t idPet;
			pHuman->getScene()->GetPetManager()->CreatePet( idModel, &posHuman, idPet );

		}
		break;
	case COMMAND_TYPE_DELETEPET:
		{
			INT idPet = Out.Param0 ;
			Obj_Pet *pPet = (Obj_Pet *)(pHuman->getScene()->GetObjManager()->GetObj(idPet));
			if(pPet->GetObjType() == Obj::OBJ_TYPE_PET)
			{
				Obj_Human *pCreator = (Obj_Human*)(pPet->GetCreator());
				if(pCreator != NULL && pCreator->GetObjType() == Obj::OBJ_TYPE_HUMAN)
				{
					pCreator->ReCallPet();
				}
				else
				{
					pHuman->getScene()->GetPetManager()->RemovePet( idPet );
				}
			}
		}
		break;
	case COMMAND_TYPE_CAPTUREPET:
		{
			INT idPet = Out.Param0;
			PET_LOG_PARAM	PetLogParam;
			pHuman->CapturePet(&PetLogParam,idPet );
			PetLogParam.CharGUID	=	pHuman->GetGUID();
			PetLogParam.OPType		=	PET_OP_GM_CREATE;
			SavePetLog(&PetLogParam);
		}
		break;
	case COMMAND_TYPE_CALLUPPET:
		{
			pHuman->ReCallPet();

			INT PetIndex = Out.Param0 ;
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(PetIndex);
			pHuman->SetGUIDOfCallUpPet( guidPet );
			ORESULT oResult = pHuman->CallUpPet();
			if(OR_FAILED(oResult))
			{
				pHuman->SendOperateResultMsg(oResult);
			}
		
		}
		break;
	case COMMAND_TYPE_RECALLPET:
		{
			pHuman->ReCallPet();
		}
		break;
	case COMMAND_TYPE_FREEPETTONATURE:
		{
			INT PetIndex = Out.Param0 ;
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(PetIndex);
			PET_LOG_PARAM	PetLogParam;
			pHuman->FreePetToNature(&PetLogParam,guidPet);
			PetLogParam.CharGUID	=	pHuman->GetGUID();
			PetLogParam.OPType		=	PET_OP_GM_CREATE;
			PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
			PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SavePetLog(&PetLogParam);

		}
		break;
	case COMMAND_TYPE_RETURNTOCHILD:
		{
			INT PetIndex = Out.Param0 ;
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(PetIndex);
			pHuman->ReturnToChild(guidPet);
		}
		break;
	case COMMAND_TYPE_SETMOVEMODE:
		{
			Obj_Character::ENUM_MOVE_MODE eMoveMode = (Obj_Character::ENUM_MOVE_MODE)(Out.Param0);
			pHuman->SetMoveMode(eMoveMode);
		}
		break;
	case COMMAND_TYPE_SKILL_RELIVE:
		{
			Obj_Human::_RELIVE_INFO infoRelive;
			infoRelive.m_Pos			= *(pHuman->getWorldPos());
			infoRelive.m_SceneID		= (pHuman->getScene() != NULL)?(pHuman->getScene()->SceneID()):(-1);
			infoRelive.m_bHPUsePercent	= FALSE;
			infoRelive.m_nHP			= 1;
			infoRelive.m_bMPUsePercent	= FALSE;
			infoRelive.m_nMP			= 0;
			infoRelive.m_bSPUsePercent	= FALSE;
			infoRelive.m_nSP			= 0;
			pHuman->SetReliveInfo( TRUE, &infoRelive );
		}
		break;
	case COMMAND_TYPE_CREATESCENE:
		{
			pScene->SetSceneLoad_Area( "dali_area.ini" ) ;
			pScene->SetSceneLoad_GrowPointData( "dali_growpoint.txt" ) ;
			pScene->SetSceneLoad_GrowPointSetup( "dali_growpointsetup.txt" ) ;
			pScene->SetSceneLoad_Map( "dali.nav" ) ;
			pScene->SetSceneLoad_Monster( "dali_monster.ini" ) ;
			pScene->SetSceneLoad_Pet( "dali_pet.ini" ) ;
			pScene->SetSceneLoad_Platform( NULL ) ;
			pScene->SetSceneLoad_PatrolPointData( "dali_patrolpoint.ini" ) ;

			COPYSCENE_SELECT SelectScene ;
			SelectScene.m_Load = pScene->m_SceneLoad ;
			SelectScene.m_SceneDataType = 0 ;
			SelectScene.m_CopyData.m_SourSceneID = pScene->SceneID() ;
			SelectScene.m_CopyData.m_TeamLeader = pGamePlayer->m_HumanGUID ;
			SelectScene.m_CopyData.m_NoUserCloseTime=12000 ;
			SelectScene.m_CopyData.m_SceneTimer=1000 ;

			g_pCopySceneManager->SelectScene( &SelectScene ) ;
		}
		break ;
	case COMMAND_TYPE_CLOSESCENE:
		{
			Scene* pNowScene = g_pSceneManager->GetScene( Out.Param0 ) ;
			if( pNowScene && pNowScene->GetSceneType()!=SCENE_TYPE_GAMELOGIC )
			{
				pNowScene->CloseScene() ;
			}
		}
		break ;
	case COMMAND_TYPE_SET_SCENE_TYPE:
		{
			Scene *pScene = pHuman->getScene();
			if ( pScene != NULL )
			{
				pScene->SetSceneType( Out.Param0 );
			}
		}
		break ;
	case COMMAND_TYPE_SET_PK_VALUE:
		{
			pHuman->SetPKValue( Out.Param0 );
		}
		break ;
	case COMMAND_TYPE_SET_MODEL_ID:
		{
			pHuman->SetModelIDRefix( Out.Param0 );
		}
		break ;
	case COMMAND_TYPE_SET_EQUIP_DUR:
		{
			HumanItemLogic::SetBagItemDur(pHuman, Out.Param1, Out.Param2);

			Item* pItem = HumanItemLogic::GetItem(pHuman, Out.Param1);

			GCItemInfo  Msg;
			Msg.setIsNull(FALSE);
			Msg.setID(Out.Param1);
			pItem->SaveValueTo(Msg.getItem());
			pHuman->GetPlayer()->SendPacket(&Msg);
		}
		break ;
	case COMMAND_TYPE_SETDAMAGE:
		{
			ObjID_t ObjID = (ObjID_t)(Out.Param0) ;
			Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;
			if( pObj == NULL )
				break ;
			if( !IsCharacterObj( pObj->GetObjType() ) )
				break ;

			Obj_Character* pCharacter = (Obj_Character*)pObj ;
			
			//pCharacter->SetDamage( Out.Param1, pHuman ) ;
			pCharacter->HealthIncrement(-Out.Param1, pHuman);
		}
		break ;
	case COMMAND_TYPE_KILLOBJ:
		{
			ObjID_t ObjID = (ObjID_t)(Out.Param0) ;
			Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;
			if( pObj == NULL )
				break ;
			if( !IsCharacterObj( pObj->GetObjType() ) )
				break ;

			Obj_Character* pCharacter = (Obj_Character*)pObj ;
			pCharacter->GM_killObj();
		}
		break;
	case COMMAND_TYPE_CREATEMONSTER:
		{
			SceneID_t		SceneID			=	pScene->SceneID() ;
			INT				MonsterType		=	Out.Param0;	
			WORLD_POS pos;
			pos.m_fX						=	Out.fParam0;
			pos.m_fZ						=	Out.fParam1;
			INT				iAIType			=	Out.Param1;	
			INT				iAIScript		=	Out.Param2;	
			INT				ScriptID		=	Out.Param3;

			_OBJ_MONSTER_INIT	init;

			MONSTER_EXT_ATTR extAttr  = *(g_MonsterAttrExTbl.GetExtAttr(MonsterType));

			init.m_uDataID			=	MonsterType ;
			init.m_Pos.m_fX			=	pos.m_fX ;
			init.m_Pos.m_fZ			=	pos.m_fZ ;
			init.m_RespawnTime		=	-1;
			init.m_BaseAI			=	iAIType ;
			init.m_ExtAIScript		=   iAIScript;
			init.m_idScript			=	ScriptID;
			//init.m_PositionRange	=	0;
			//init.m_Level			=	extAttr.m_Level;
			//init.m_BaseExp			=	extAttr.m_BaseExp;
			init.m_nCamp				=	atoi(Out.m_aBuffer);
			//init.m_FriendValue		=	extAttr.m_FriendValue;
			//init.m_PositionRange	=	10;


			pScene->CreateTempMonster((const _OBJ_MONSTER_INIT*)&init);
		}
		break ;
	case COMMAND_TYPE_DELETEMONSTER:
		{
			ObjID_t ObjID = (ObjID_t)(Out.Param0) ;
			Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;
			if( pObj == NULL )
				break ;
			if( pObj->GetObjType()!=Obj::OBJ_TYPE_MONSTER )
				break ;

			Obj_Monster* pMonster = (Obj_Monster*)pObj ;
			
			pScene->DeleteTempMonster(pMonster) ;
		}
		break ;
	case COMMAND_TYPE_SETAITYPE:
		{
			ObjID_t ObjID = (ObjID_t)(Out.Param0) ;
			Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;
			if( pObj == NULL )
				break ;
			if( pObj->GetObjType()!=Obj::OBJ_TYPE_MONSTER )
				break ;

			Obj_Monster* pMonster = (Obj_Monster*)pObj ;
			
			INT nAIType = (Out.Param1) ;
			pMonster->SetAIType(nAIType);
		}
		break;
	case COMMAND_TYPE_MODIFYMONEY:
		{
			pHuman->SetMoney( Out.Param0 ) ;
			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	MONEY_BANK_WITHDRAW;
			MoneyLogParam.Count		=	Out.Param0;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);
		}
		break ;
	case COMMAND_TYPE_MAKETEAMFOLLOW:
		{
/*			if ( pHuman->__GetTeamFollowFlag() )
			{
				pHuman->__SetTeamFollowFlag( FALSE );
				pHuman->__ClearFollowedMembers();
			}
			else
			{
				pHuman->__SetTeamFollowFlag( TRUE );
				pHuman->__AddFollowedMember( pHuman );
			}
*/		}
		break;
	case COMMAND_TYPE_SENDMAIL:
		{
			MAIL mail ;
			mail.m_GUID = pHuman->GetGUID();
			mail.m_SourSize = (BYTE)strlen(pHuman->GetName()) ;
			strncpy( mail.m_szSourName, pHuman->GetName(), MAX_CHARACTER_NAME-1) ;
			mail.m_DestSize = (BYTE)strlen(Out.m_aBuffer) ;
			strncpy( mail.m_szDestName, Out.m_aBuffer, MAX_CHARACTER_NAME-1) ;
			mail.m_ContexSize = (BYTE)strlen(Out.m_aBufferEx) ;
			strncpy( mail.m_szContex, Out.m_aBufferEx, MAX_MAIL_CONTEX-1 ) ;
			mail.m_Camp = pHuman->GetCampData()->m_nCampID;
			

			GWMail* pMsg = (GWMail*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_MAIL)) ;
			pMsg->SetMail( &mail ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

		}
		break ;
	case COMMAND_TYPE_RECVMAIL:
		{
			GWAskMail* pMsg = (GWAskMail*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKMAIL)) ;
			pMsg->SetGUID( pHuman->GetGUID() ) ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		break ;
	case COMMAND_TYPE_CREATEGUILD:
		{
			GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);

			pMsg->SetGUID( pHuman->GetGUID() );
			_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
			pGuildPacket->m_uPacketType = GUILD_PACKET_GW_CREATE;

			GUILD_CGW_CREATE* pPacketCreate;
			pPacketCreate = (GUILD_CGW_CREATE*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_CREATE));

			pPacketCreate->m_NameSize = (UCHAR)strlen(Out.m_aBuffer);
			strncpy(pPacketCreate->m_szGuildName, Out.m_aBuffer, sizeof(pPacketCreate->m_szGuildName) - 1);

			pPacketCreate->m_Camp = pHuman->GetCampData()->m_nCampID;

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		}
		break;
	case COMMAND_TYPE_JOINGUILD:
		{
			GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);

			pMsg->SetGUID( pHuman->GetGUID() );
			_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
			pGuildPacket->m_uPacketType = GUILD_PACKET_GW_JOIN;

			GUILD_CGW_JOIN* pPacketJoin = (GUILD_CGW_JOIN*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_JOIN));

			pPacketJoin->m_GuildGUID = Out.Param0;
			pPacketJoin->m_Camp = pHuman->GetCampData()->m_nCampID;

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		}
		break;
	case COMMAND_TYPE_GUILDRECRUIT:
		{
			GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);

			pMsg->SetGUID( pHuman->GetGUID() );
			_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
			pGuildPacket->m_uPacketType = GUILD_PACKET_GW_RECRUIT;

			GUILD_CGW_RECRUIT* pPacketRecruit = (GUILD_CGW_RECRUIT*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_RECRUIT));

			pPacketRecruit->m_ProposerGUID = Out.Param0;

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		}
		break;
	case COMMAND_TYPE_GUILDEXPEL:
		{
			GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);

			pMsg->SetGUID( pHuman->GetGUID() );
			_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
			pGuildPacket->m_uPacketType = GUILD_PACKET_GW_EXPEL;

			GUILD_CGW_EXPEL* pPacketExpel = (GUILD_CGW_EXPEL*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_EXPEL));

			pPacketExpel->m_GuildUserGUID = Out.Param0;

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		}
		break;
	case COMMAND_TYPE_LEAVEGUILD:
		{
			GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);

			pMsg->SetGUID( pHuman->GetGUID() );
			_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
			pGuildPacket->m_uPacketType = GUILD_PACKET_GW_LEAVE;
			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		}
		break;
	case COMMAND_TYPE_GUILDDISMISS:
		{
			GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);

			pMsg->SetGUID( pHuman->GetGUID() );
			_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
			pGuildPacket->m_uPacketType = GUILD_PACKET_GW_DISMISS;
			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		}
		break;
	case COMMAND_TYPE_ASKGUILDLIST:
		{
			GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);

			pMsg->SetGUID( pHuman->GetGUID() );
			_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
			pGuildPacket->m_uPacketType = GUILD_PACKET_GW_ASKLIST;

			GUILD_CGW_ASKLIST* pPacketAskList = (GUILD_CGW_ASKLIST*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_ASKLIST));

			pPacketAskList->m_uStart = Out.Param0;
			pPacketAskList->m_Camp = pHuman->GetCampData()->m_nCampID;

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		}
		break;
	case COMMAND_TYPE_ASKRELATIONLIST:		// 关系列表
		{
			CGRelation* pMsg = (CGRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_CG_RELATION);

			pMsg->GetRelation()->m_Type = REQ_RELATIONLIST;
			pScene->SendPacket( pMsg, pGamePlayer->PlayerID() );
		}
		break;
	case COMMAND_TYPE_ADDFRIEND:			// 加入一个好友
		{
			CGRelation* pMsg = (CGRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_CG_RELATION);

			pMsg->GetRelation()->m_Type = REQ_ADDFRIEND;

			REQUEST_ADD_RELATION_WITH_GROUP* pFriend = &(pMsg->GetRelation()->m_AddRelationWithGroup);
			pFriend->SetTargetGUID( Out.Param0 );
			pFriend->SetTargetName( Out.m_aBuffer );
			pFriend->SetGroup( Out.Param1 );

			pScene->SendPacket( pMsg, pGamePlayer->PlayerID() );
		}
		break;
	case COMMAND_TYPE_SETFRIENDPOINT:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			GUID_t guid = Out.Param0;
			INT nPoint = Out.Param1;

			nPoint -= pMyRelation->GetFriendPoint( guid );
			if( nPoint > 0 )
			{
				// 增加友好度
				pMyRelation->IncFriendPoint( guid, nPoint );
			}
			else if( nPoint < 0 )
			{
				// 减少友好度
				pMyRelation->DecFriendPoint( guid, -nPoint );
			}
		}
		break;
	case COMMAND_TYPE_ADDBLACKNAME:			// 加入黑名单
		{
			CGRelation* pMsg = (CGRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_CG_RELATION);

			pMsg->GetRelation()->m_Type = REQ_ADDTOBLACKLIST;

			REQUEST_ADD_RELATION* pBlackName = &(pMsg->GetRelation()->m_AddRelation);
			pBlackName->SetTargetGUID( Out.Param0 );
			pBlackName->SetTargetName( Out.m_aBuffer );

			pScene->SendPacket( pMsg, pGamePlayer->PlayerID() );
		}
		break;
	case COMMAND_TYPE_DELFRIEND:			// 删除一个好友
		{
			CGRelation* pMsg = (CGRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_CG_RELATION);

			pMsg->GetRelation()->m_Type = REQ_DELFRIEND;

			pMsg->GetRelation()->m_DelFriend.SetTargetGUID( Out.Param0 );

			pScene->SendPacket( pMsg, pGamePlayer->PlayerID() );
		}
		break;
	case COMMAND_TYPE_DELBLACK:				// 从黑名单里删除一个玩家
		{
			CGRelation* pMsg = (CGRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_CG_RELATION);

			pMsg->GetRelation()->m_Type = REQ_DELFROMBLACKLIST;

			pMsg->GetRelation()->m_RelationGUID.SetTargetGUID( Out.Param0 );

			pScene->SendPacket( pMsg, pGamePlayer->PlayerID() );
		}
		break;
	case COMMAND_TYPE_TRANSITION:			// 组间转移
		{
			CGRelation* pMsg = (CGRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_CG_RELATION);

			pMsg->GetRelation()->m_Type = REQ_TRANSITION;

			RELATION_GUID_UCHAR_UCHAR* pTrans = &(pMsg->GetRelation()->m_RelationGUIDUCHARUCHAR);
			pTrans->SetTargetGUID( Out.Param0 );
			pTrans->SetRelationType( Out.Param1 );
			pTrans->SetGroup( Out.Param2 );

			pScene->SendPacket( pMsg, pGamePlayer->PlayerID() );
		}
		break;
	case COMMAND_TYPE_SETCAMPID:
		{
			Obj_Character* pChar;
			Obj* pObj = pScene->GetObjManager()->GetObj( Out.Param0 );

			if( !pObj )
			{
				Assert( FALSE && "No Such Object in the Scene!(You can ingore this assert.)");
				return PACKET_EXE_CONTINUE;
			}

			if( pObj->GetObjType() != pObj->OBJ_TYPE_HUMAN
			 && pObj->GetObjType() != pObj->OBJ_TYPE_MONSTER
			 && pObj->GetObjType() != pObj->OBJ_TYPE_PET
			 )
			{
				Assert( FALSE && "No Such Character in the Scene!(You can ingore this assert.)");
				return PACKET_EXE_CONTINUE;
			}

			pChar = (Obj_Character*)pObj;
			Assert(pChar);

			pChar->SetBaseCampID( Out.Param1 );
		}
		break;
	case COMMAND_TYPE_SAVE:
		{
			pGamePlayer->Save( ) ;
		}
		break ;
	case COMMAND_TYPE_GETOBJINFO:
		{
			Obj* pObj = pScene->GetObjManager()->GetObj( Out.Param0 );
			if( !pObj )
			{
				return PACKET_EXE_CONTINUE;
			}

			GCChat Msg ;
			Msg.SetChatType(CHAT_TYPE_NORMAL) ;
			Msg.SetSourNameSize(0) ;
			Msg.SetSourID(pHuman->GetID()) ;
			Msg.SetSourCamp( INVALID_CAMP );

			switch( pObj->GetObjType() )
			{
			case Obj::OBJ_TYPE_HUMAN:
			case Obj::OBJ_TYPE_MONSTER:
			case Obj::OBJ_TYPE_PET:
			case Obj::OBJ_TYPE_ITEM_BOX:
			case Obj::OBJ_TYPE_PLATFORM:
			case Obj::OBJ_TYPE_SPECIAL:
			default :
				{
					CHAR szContex[256] ;
					sprintf( szContex, "Pos=(%f,%f)", pObj->getWorldPos()->m_fX, pObj->getWorldPos()->m_fZ ) ;
					Msg.SetContex(szContex) ;
					Msg.SetContexSize((BYTE)strlen(szContex)) ;
				}
				break ;
			};

			pGamePlayer->SendPacket( &Msg ) ;
		}
		break ;
	case COMMAND_TYPE_LEVELUP:				// 升级
		{
			INT nMyLevel, nLevel;

			nMyLevel = pHuman->GetLevel();

			if( Out.Param0 == 0 )
			{
				nLevel = nMyLevel + 1;
			}
			else
			{
				nLevel = Out.Param0;
			}

			for( INT i=nMyLevel; i<nLevel; ++i)
			{
				pHuman->Levelup();
			}

			GCLevelUpResult ResultMsg;
			ResultMsg.SetRemainExp(pHuman->GetExp());
			ResultMsg.SetLevelUpResult(LEVELUP_SUCCESS);

			pGamePlayer->SendPacket(&ResultMsg);

			GCLevelUp otherMsg;
			otherMsg.setObjId(pHuman->GetID());
			otherMsg.setLevel(pHuman->GetLevel());

			pScene->BroadCast(&otherMsg, pHuman, TRUE);

			GWLevelUp* pWorldMsg = (GWLevelUp*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_LEVEUP));
			pWorldMsg->SetGUID( pHuman->GetGUID() );
			pWorldMsg->SetLevel( pHuman->GetLevel() );

			g_pServerManager->SendPacket( pWorldMsg, INVALID_ID );
		}
		break;
	case COMMAND_TYPE_ABILITYUP:				// 生活技能升级
		{
			INT nMyLevel, nLevel;
			AbilityID_t aid;

			aid = Out.Param0;
			nMyLevel = pHuman->__GetAbilityLevel(aid);

			if( Out.Param1 == 0 )
			{
				nLevel = nMyLevel + 1;
			}
			else
			{
				nLevel = Out.Param1;
			}

			pHuman->__SetAbilityLevel(aid, nLevel);
			GCAbilityLevel Msg;
			Msg.setAbilityId(aid);
			Msg.setLevel(nLevel);
			pGamePlayer->SendPacket( &Msg );


			ABILITY_LOG_PARAM	AbilityLogParam;

			AbilityLogParam.CharGUID	=	pHuman->GetGUID();
			AbilityLogParam.AbilityID	=	aid;
			AbilityLogParam.Level		=	nLevel;
			AbilityLogParam.OPType		=	ABILITY_OP_GM_MODIFY;
			AbilityLogParam.SceneID		=	pHuman->getScene()->SceneID();
			AbilityLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			AbilityLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveAbilityLog(&AbilityLogParam);

		}
		break;
	case COMMAND_TYPE_WHO:
		{
			CHAR szSourName[] = "信息获取" ;
			CHAR szContex[256] ;
			
			sprintf( szContex, "PlayerCount: %d/%d", 
				g_pSceneManager->GetScenePlayerCount(pScene->SceneID()),
				g_pSceneManager->GetTotalPlayerCount() ) ;

			GCChat Msg ;
			Msg.SetChatType(CHAT_TYPE_TEAM) ;
			Msg.SetWorldChatID(0) ;
			Msg.SetSourName(szSourName) ;
			Msg.SetSourNameSize( (BYTE)strlen(szSourName) ) ;
			Msg.SetContex( szContex ) ;
			Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
			Msg.SetSourCamp( INVALID_CAMP );

			pGamePlayer->SendPacket( &Msg ) ;
		}
		break ;
	case COMMAND_TYPE_ALLWHO:
		{
			GWCommand* pMsg = (GWCommand*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_COMMAND);
			if( pMsg==NULL )
				break ;

			pMsg->SetPlayerID( pGamePlayer->PlayerID() ) ;
			pMsg->GetServerCommand()->m_Command = COMMAND_TYPE_ALLWHO ;

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		}
		break ;
	case COMMAND_TYPE_CATCHPLAYER:
		{
			GWCommand* pMsg = (GWCommand*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_COMMAND);
			if( pMsg==NULL )
				break ;

			pMsg->SetPlayerID( pGamePlayer->PlayerID() ) ;
			pMsg->GetServerCommand()->m_Command = COMMAND_TYPE_CATCHPLAYER ;
			pMsg->GetServerCommand()->m_Param0 = Out.Param0 ;
			if( Out.Param0==0 )
			{
				strncpy( pMsg->GetServerCommand()->m_szName, Out.m_aBuffer, MAX_CHARACTER_NAME-1 ) ;
			}
			else if( Out.Param0==1 )
			{
				pMsg->GetServerCommand()->m_Param1 = Out.Param1 ;
			}
			else
			{
				Assert(FALSE) ;
			}
			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		}
		break ;
	case COMMAND_TYPE_PLAYERINFO:
		{
			CHAR szSourName[] = "信息获取" ;
			CHAR szContex[256] ;

			if( Out.Param0 == 0 )
			{
				BOOL bFind = FALSE ;
				PlayerID_t* aPlayers = pScene->GetScenePlayerManager()->GetPlayers() ;
				for( INT i=0; i<pScene->GetScenePlayerManager()->GetCount(); i++ )
				{
					GamePlayer* pSearchPlayer = g_pPlayerPool->GetPlayer(aPlayers[i]) ;
					if( pSearchPlayer==NULL )
					{
						Assert( FALSE ) ;
						break ;
					}
					Obj_Human* pSearchHuman = pSearchPlayer->GetHuman() ;
					if( strcmp( pSearchHuman->GetName(), Out.m_aBuffer)==0 )
					{
						sprintf( szContex, "%X: Pos(%d,%d) Menpai:%d Level:%d",	
							pSearchPlayer->m_HumanGUID,
							(INT)pSearchHuman->getWorldPos()->m_fX,
							(INT)pSearchHuman->getWorldPos()->m_fZ,
							pSearchHuman->GetMenPai(),
							pSearchHuman->GetLevel() ) ;

						GCChat Msg ;
						Msg.SetChatType(CHAT_TYPE_TEAM) ;
						Msg.SetWorldChatID(0) ;
						Msg.SetSourName(szSourName) ;
						Msg.SetSourNameSize( (BYTE)strlen(szSourName) ) ;
						Msg.SetContex( szContex ) ;
						Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
						Msg.SetSourCamp( INVALID_CAMP );

						pGamePlayer->SendPacket( &Msg ) ;

						bFind = TRUE ;
						break ;
					}
				}//end for

				if( bFind==FALSE )
				{
					sprintf( szContex, "Cannot find name=%s player", Out.m_aBuffer ) ;

					GCChat Msg ;
					Msg.SetChatType(CHAT_TYPE_TEAM) ;
					Msg.SetWorldChatID(0) ;
					Msg.SetSourName(szSourName) ;
					Msg.SetSourNameSize( (BYTE)strlen(szSourName) ) ;
					Msg.SetContex( szContex ) ;
					Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
					Msg.SetSourCamp( INVALID_CAMP );

					pGamePlayer->SendPacket( &Msg ) ;
				}
			}
			else if( Out.Param0 == 1 )
			{//根据GUID查询
				Obj_Human* pSearchHuman = (Obj_Human*)g_pGUIDManager->Get((GUID_t)Out.Param1) ;
				if( pSearchHuman==NULL )
				{
					sprintf( szContex, "Cannot find GUID=%X player", Out.Param1 ) ;

					GCChat Msg ;
					Msg.SetChatType(CHAT_TYPE_TEAM) ;
					Msg.SetWorldChatID(0) ;
					Msg.SetSourName(szSourName) ;
					Msg.SetSourNameSize( (BYTE)strlen(szSourName) ) ;
					Msg.SetContex( szContex ) ;
					Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
					Msg.SetSourCamp( INVALID_CAMP );

					pGamePlayer->SendPacket( &Msg ) ;
					break ;
				}

				sprintf( szContex, "%s: Pos(%d,%d) Menpai:%d Level:%d",	
					pSearchHuman->GetName(),
					(INT)pSearchHuman->getWorldPos()->m_fX,
					(INT)pSearchHuman->getWorldPos()->m_fZ,
					pSearchHuman->GetMenPai(),
					pSearchHuman->GetLevel() ) ;

				GCChat Msg ;
				Msg.SetChatType(CHAT_TYPE_TEAM) ;
				Msg.SetWorldChatID(0) ;
				Msg.SetSourName(szSourName) ;
				Msg.SetSourNameSize( (BYTE)strlen(szSourName) ) ;
				Msg.SetContex( szContex ) ;
				Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
				Msg.SetSourCamp( INVALID_CAMP );

				pGamePlayer->SendPacket( &Msg ) ;
			}
			else
			{
				Assert(FALSE) ;
			}
		}
		break ;
	case COMMAND_TYPE_SETPETHAPPINESS:
		{
			INT nPetIndex;
			BYTE yHappiness;
			nPetIndex	= Out.Param0;
			yHappiness	= (BYTE)(Out.Param1);
			g_ItemOperator.SetHappiness(pHuman->GetPetContain(), nPetIndex, yHappiness);

			Item *pPetItem = pHuman->GetPetItem(pHuman->GetPetGUIDByValidIndex(nPetIndex));
			if(pPetItem != NULL)
			{
				GCDetailAttrib_Pet msgDetail;
				msgDetail.SetGUID(pPetItem->GetPetGUID());
				msgDetail.SetHappiness(pPetItem->GetHappiness());

				pHuman->GetPlayer()->SendPacket(&msgDetail);
			}
		}
		break;
	case COMMAND_TYPE_SETPETHP:
		{
			INT nPetIndex;
			INT nHP;
			nPetIndex	= Out.Param0;
			nHP	= Out.Param1;
			g_ItemOperator.SetHP(pHuman->GetPetContain(), nPetIndex, nHP);

			Item *pPetItem = pHuman->GetPetItem(pHuman->GetPetGUIDByValidIndex(nPetIndex));
			if(pPetItem != NULL)
			{
				GCDetailAttrib_Pet msgDetail;
				msgDetail.SetGUID(pPetItem->GetPetGUID());
				msgDetail.SetHP(pPetItem->GetHP());

				pHuman->GetPlayer()->SendPacket(&msgDetail);
			}
		}
		break;
	case COMMAND_TYPE_SETPETLIFE:
		{
			INT nPetIndex;
			INT nLife;
			nPetIndex	= Out.Param0;
			nLife		= Out.Param1;
			g_ItemOperator.SetLife(pHuman->GetPetContain(), nPetIndex, nLife);

			Item *pPetItem = pHuman->GetPetItem(pHuman->GetPetGUIDByValidIndex(nPetIndex));
			if(pPetItem != NULL)
			{
				GCDetailAttrib_Pet msgDetail;
				msgDetail.SetGUID(pPetItem->GetPetGUID());
				msgDetail.SetLife(pPetItem->GetLife());

				pHuman->GetPlayer()->SendPacket(&msgDetail);
			}
		}
		break;
	case COMMAND_TYPE_PETLEVELUP:
		{
			INT nLevel;
			nLevel		= Out.Param0;
			Obj_Pet *pPet = pHuman->GetPet();
			if(pPet != NULL)
			{
				while(nLevel > 0)
				{
					pPet->LevelUp();
					nLevel--;
				}
			}
		}
		break;
	case COMMAND_TYPE_SETPETSKILL:
		{
			INT nPetIndex, nSkillIndex, nSkillID;
			nPetIndex	= Out.Param0;
			nSkillIndex	= Out.Param1;
			nSkillID	= Out.Param2;
			_PET_SKILL skillPet;
			skillPet.m_nSkillID	= nSkillID;
			g_ItemOperator.SetSkill(pHuman->GetPetContain(), nPetIndex, nSkillIndex, skillPet);

			Item *pPetItem = pHuman->GetPetItem(pHuman->GetPetGUIDByValidIndex(nPetIndex));
			if(pPetItem != NULL)
			{
				GCDetailAttrib_Pet msgDetail;
				msgDetail.SetGUID(pPetItem->GetPetGUID());
				_OWN_SKILL skillPet;
				skillPet.m_nSkillID		= pPetItem->GetSkill(nSkillIndex).m_nSkillID;
				msgDetail.SetSkill(nSkillIndex, &skillPet);

				pHuman->GetPlayer()->SendPacket(&msgDetail);
			}
		}
		break;
	case COMMAND_TYPE_USEPETSKILL:
		{
			INT nSkillID  = Out.Param0;
			INT nObjID	  = Out.Param1; 

			Obj* pObj = pScene->GetObjManager()->GetObj(nObjID);
			Obj_Pet* pPet = pHuman->GetPet();
			if (pPet)
			{
				AI_Pet* pAIPet = (AI_Pet*)(pPet->GetAIObj());
				pAIPet->PushCommand_UseSkill(nSkillID, pObj->GetID(),
					pObj->getWorldPos()->m_fX, pObj->getWorldPos()->m_fZ, -1.f, INVALID_GUID);
			}
		}
		break;
	case COMMAND_TYPE_LEVELDOWN:
		{
			INT nLevelBeCut = Out.Param0;
			INT nLevel;

			nLevel = pHuman->GetLevel() - nLevelBeCut;
			if( nLevel < 1 )
			{
				nLevel = 1;
			}

			pHuman->SetLevel( nLevel );
		}
		break;
	case COMMAND_TYPE_ADDEXP:
		{

			INT nExpBeAdded = Out.Param0;
			pHuman->SetAlterExp( nExpBeAdded );
		}
		break;

	case COMMAND_TYPE_DOUBLEEXP:
		{
			INT nExpBeAdded = Out.Param0;
			int nExpDoubleNum=Out.Param1;
			int nExpTime=nExpBeAdded*1000;
			if(pHuman->m_ExpTimer.IsSetTimer())
			{

				//如果经验倍数小于已经设置过的就不会重新设置
				if(nExpDoubleNum < GETLOW(pHuman->GetDoubleExpTime_Num()))
				{
			//		Lua_PushNumber( L, 0 );
				//	return 1;
					break;

				}
				//如果经验倍数相等就增加时间
				if(nExpDoubleNum == GETLOW(pHuman->GetDoubleExpTime_Num()))
				{
					//pHuman->SetEnergy((nExpTime+GETHIGH(pHuman->GetEnergy()))<<16+nExpDoubleNum);
					pHuman->m_ExpTimer.SetTermTime(pHuman->m_ExpTimer.GetTermTime()+nExpTime);
				}
				//如果经验倍数大于已经设置过的就更新双倍经验
				if(nExpDoubleNum > GETLOW(pHuman->GetDoubleExpTime_Num()))
				{
					pHuman->m_ExpTimer.CleanUp();
					pHuman->m_ExpTimer.BeginTimer(nExpTime,g_pTimeManager->CurrentTime());
					pHuman->SetDoubleExpTime_Num(((nExpTime/1000)<<16)+nExpDoubleNum);
				}

			}
			else
			{
				//设置双倍经验;
				pHuman->m_ExpTimer.BeginTimer(nExpTime,g_pTimeManager->CurrentTime());
				pHuman->SetDoubleExpTime_Num(((nExpTime/1000)<<16)+nExpDoubleNum);
				GCChat msgChat;
				CHAR szChat[128]={0};
				sprintf(szChat,"你已经设置了打怪获取%d倍经验状态",nExpDoubleNum);
				INT Size = (INT)strlen(szChat) ;
				msgChat.SetChatType( CHAT_TYPE_SELF) ;
				msgChat.SetContexSize( Size ) ;
				msgChat.SetContex( szChat ) ;
				msgChat.SetSourNameSize( (BYTE)strlen(pHuman->GetName())) ;
				msgChat.SetSourName( pHuman->GetName()) ;
				msgChat.SetSourID(pHuman->GetID()) ;
				msgChat.SetSourCamp( INVALID_CAMP );
				pHuman->GetPlayer()->SendPacket(&msgChat);


			}

			//pHuman->SetAlterExp( nExpBeAdded );
		}
		break;
	case COMMAND_TYPE_CREATECITY:
		{
			SceneID_t  nSceneID = Out.Param0;

			pHuman->CreateCity( nSceneID );
		}
		break;
	case COMMAND_TYPE_DELETECITY:
		{
			pHuman->DeleteCity( Out.Param0, Out.Param1 );
		}
		break;
	case COMMAND_TYPE_LOADMONSTER:
		{
			pScene->ExecuteMonsterIni( Out.m_aBuffer ) ;
		}
		break ;
	case COMMAND_TYPE_SETHAIRCOLOR:
		{
			BYTE yA, yR, yG, yB;
			yA	= Out.Param0;
			yR	= Out.Param1;
			yG	= Out.Param2;
			yB	= Out.Param3;
			UINT uColor = MAKE_COLOR(yR, yG, yB, yA);
			pHuman->__SetHairColor(uColor);
		}
		break ;
	case COMMAND_TYPE_SETHAIRMODEL:
		{
			INT nModel	= Out.Param0;
			pHuman->__SetHairModel(nModel);
		}
		break ;
	case COMMAND_TYPE_CREATEGUARD:
		{
			INT nDataID = Out.Param0;
			pHuman->CreateGuard(nDataID);
		}
		break;
	case COMMAND_TYPE_CREATETRAP:
		{
			INT nTrapID = Out.Param0;
			Obj* pObj = pHuman->Skill_CreateObjSpecial(*(pHuman->getWorldPos()), nTrapID);
			if(NULL!=pObj)
			{
				pObj->SetActiveFlag(TRUE);
			}
		}
		break;
	case COMMAND_TYPE_USEITEM:
		{
			INT BagItemPos = Out.Param0;
			INT TargetItemPos = Out.Param1;
			pHuman->UseIdentScroll(BagItemPos,TargetItemPos);
		}
		break;
	case COMMAND_TYPE_FORGETRECIPE:
		{
			INT nRecipeID = Out.Param0;

			pHuman->__SetPrescrLearnedFlag(nRecipeID, FALSE);

			GCPrescription Msg;
			Msg.setPrescription(nRecipeID);
			Msg.setLearnOrAbandon(FALSE);

			pHuman->GetPlayer()->SendPacket( &Msg );
		}
		break;
	case COMMAND_TYPE_SEND_IMPACT_TO_UNIT:
		{
			Obj* pObj = pScene->GetObjManager()->GetObj( Out.Param0 );
			if(NULL==pObj)
			{
				Assert( FALSE && "No Such Object in the Scene!(You can ingore this assert.)");
				return PACKET_EXE_CONTINUE;
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				Assert( FALSE && "No Such Character in the Scene!(You can ingore this assert.)");
				return PACKET_EXE_CONTINUE;
			}
			Obj_Character& rChar = *(Obj_Character*)pObj;

			g_ImpactCore.SendImpactToUnit(rChar, Out.Param1, pHuman->GetID(), 0);
		}
		break;
	case COMMAND_TYPE_RELOAD_COMBAT_TABLES:
		{
			g_TableInit.ReloadCombatTables();
		}
		break;
	case COMMAND_TYPE_USE_SKILL:
		{
			INT nSkillID = INVALID_ID;

			const _SKILL_LIST *pSkillList = &(pHuman->Skill_GetSkillList());
			const _OWN_SKILL *pSkill;
			const SkillTemplateData_T *pSkillTemplateData;
			INT i;
			for(i = 0; i < pSkillList->m_Count; i++)
			{
				pSkill = &(pSkillList->m_aSkill[i]);
				pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(pSkill->m_nSkillID);
				if(pSkillTemplateData != NULL)
				{
					if(0 == strcmp(pSkillTemplateData->GetName(), Out.m_aBuffer))
					{
						nSkillID = pSkill->m_nSkillID;
						break;
					}
				}
			}
			if(nSkillID != INVALID_ID)
			{
				((AI_Human*)(pHuman->GetAIObj()))->PushCommand_UseSkill(nSkillID, 1, Out.Param0, Out.fParam0, Out.fParam1, Out.fParam2, Out.Param1);
			}
		}
		break ;
	case COMMAND_TYPE_SAVEPACKETINFO:
		{
			for( UINT i=0; i<PACKET_MAX; i++ )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "Packet: %d-%d", i, g_pPacketFactoryManager->m_pPacketAllocCount[i] ) ;
			}
		}
		break ;
	//case COMMAND_TYPE_SETVIGOR:
	//	{
	//		pHuman->SetVigor(Out.Param0);
	//	}
	//	break ;
	//case COMMAND_TYPE_SETENERGY:
	//	{
	//		pHuman->SetEnergy(Out.Param0);
	//	}
	//	break ;
	case COMMAND_TYPE_PETPULLULATE:
		{
			INT nPetIndex = Out.Param0;
			PET_GUID_t guidPet = pHuman->GetPetGUIDByIndex(nPetIndex);
			Item *pPetItem = NULL;
			if(!guidPet.IsNull())
			{
				pPetItem = pHuman->GetPetItem(guidPet);
			}

			CHAR szChat[128];

			if(pPetItem != NULL)
			{
				FLOAT fGrowRate = pPetItem->GetGrowRate();

				sprintf(szChat, "#R成长率：%f", fGrowRate);
			}
			else
			{
				sprintf(szChat, "#R未找到指定宠物");
			}

			GCChat msgChat;

			INT Size = (INT)strlen(szChat) ;
			msgChat.SetChatType( CHAT_TYPE_SELF) ;
			msgChat.SetContexSize( Size ) ;
			msgChat.SetContex( szChat ) ;
			msgChat.SetSourNameSize( (BYTE)strlen(pHuman->GetName())) ;
			msgChat.SetSourName( pHuman->GetName()) ;
			msgChat.SetSourID(pHuman->GetID()) ;
			msgChat.SetSourCamp( INVALID_CAMP );
			pHuman->GetPlayer()->SendPacket(&msgChat);
		}
		break ;
	case COMMAND_TYPE_ADD_SKILL:
		{
			BOOL bModify = FALSE ;
			pHuman->Skill_ModifySkill(Out.Param0, Out.Param1, 1);

			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SkillLogParam.OPType	=	SKILL_OP_GM_MODIFY;
			SkillLogParam.Count		=	pHuman->Skill_GetSkillList().m_Count;
			SkillLogParam.SkillID	=	Out.Param0;
			SkillLogParam.Level		=	1;

			SaveSkillLog(&SkillLogParam);
		}
		break ;
	case COMMAND_TYPE_REMOVE_SKILL:
		{
			BOOL bModify = FALSE ;
			pHuman->Skill_ModifySkill(Out.Param0, 1, -1);

			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SkillLogParam.OPType	=	SKILL_OP_GM_MODIFY;
			SkillLogParam.Count		=	pHuman->Skill_GetSkillList().m_Count;
			SkillLogParam.SkillID	=	Out.Param0;
			SkillLogParam.Level		=	0;

			SaveSkillLog(&SkillLogParam);
		}
		break ;
	case COMMAND_TYPE_SETGMRIGHT:
		{
			
			GUID_t guid = Out.Param0;
			INT nGmRightFlag = Out.Param1;
			if(nGmRightFlag >= (INT)GM_EXECUTE_ALL)
			{
				nGmRightFlag=(INT)GM_EXECUTE_ALL;

			}
			if(gm_level == GM_EXECUTE_ADMIN)
			{
				if(nGmRightFlag >= (INT)GM_EXECUTE_ADMIN)
				{
					break;
				}

			}
			Obj_Human* pSearchHuman = (Obj_Human*)g_pGUIDManager->Get(guid) ;
			if( pSearchHuman==NULL )
			{
				break;
			}
			
			pSearchHuman->SetGmRight(nGmRightFlag);
			

		}
		break;
	case COMMAND_TYPE_GMTEST:
		{
			pHuman->SetGmRight(4);
		}
    	break;
	case COMMAND_TYPE_SHOWGUID:
		{
			GCChat msgChat;
			CHAR szChat[MAX_CHAT_SIZE]={0};
			sprintf(szChat,"GUID:%d",pHuman->GetGUID());
			INT Size = (INT)strlen(szChat) ;
			msgChat.SetChatType( CHAT_TYPE_SELF) ;
			msgChat.SetContexSize( Size ) ;
			msgChat.SetContex( szChat ) ;
			msgChat.SetSourNameSize( (BYTE)strlen(pHuman->GetName())) ;
			msgChat.SetSourName( pHuman->GetName()) ;
			msgChat.SetSourID(pHuman->GetID()) ;
			msgChat.SetSourCamp( INVALID_CAMP );
			pHuman->GetPlayer()->SendPacket(&msgChat);

		}
		break;
	case COMMAND_TYPE_FINGERGUID:			// 用 GUID 找玩家信息
		{
			GUID_t guid = Out.Param0;
			INT nOnlineFlag = Out.Param1;

			CGFinger* pMsg = (CGFinger*)g_pPacketFactoryManager->CreatePacket(PACKET_CG_FINGER);

			CG_FINGER* pFinger = pMsg->GetFinger();
			pFinger->CleanUp();
			pFinger->m_Type = FREQ_GUID;
			pFinger->m_FingerByGUID.CleanUp();
			pFinger->m_FingerByGUID.SetTargetGUID( guid );
			pFinger->m_FingerByGUID.SetOnlineFlag( nOnlineFlag );
			pScene->SendPacket( pMsg, pGamePlayer->PlayerID() );

			g_pLog->FastSaveLog( LOG_FILE_1, "COMMAND_TYPE_FINGERGUID: GUID:%u, OnlineFlag=%d", guid, nOnlineFlag );
		}
		break;
	case COMMAND_TYPE_FINGERNAME:			// 用名字找玩家信息
		{
			INT nOnlineFlag = Out.Param0;
			INT nPreciseFlag = Out.Param1;
			INT nPosition = Out.Param2;
			CHAR szName[MAX_CHARACTER_NAME];

			strncpy( szName, Out.m_aBuffer, MAX_CHARACTER_NAME-1 );

			CGFinger* pMsg = (CGFinger*)g_pPacketFactoryManager->CreatePacket(PACKET_CG_FINGER);

			CG_FINGER* pFinger = pMsg->GetFinger();
			pFinger->CleanUp();
			pFinger->m_Type = FREQ_NAME;
			pFinger->m_FingerByName.CleanUp();
			pFinger->m_FingerByName.SetTargetName( szName );
			pFinger->m_FingerByName.SetPreciseFlag( nPreciseFlag );
			pFinger->m_FingerByName.SetOnlineFlag( nOnlineFlag );
			pFinger->m_FingerByName.SetPosition( nPosition );
			pScene->SendPacket( pMsg, pGamePlayer->PlayerID() );

			g_pLog->FastSaveLog( LOG_FILE_1, "COMMAND_TYPE_FINGERNAME: Name:%s, OnlineFlag=%d, PreciseFlag=%d, Position=%d",
				szName, nOnlineFlag, nPreciseFlag, nPosition );
		}
		break;
	case COMMAND_TYPE_ADVANCEDFINGER:		// 高级查找
		{
			INT nPosition = Out.Param0;
			INT nMenpai = Out.Param1;
			INT nGuildID = Out.Param2;
			INT nSex = Out.Param3;
			INT nLevelFrom = (INT)Out.fParam0;
			INT nLevelTo = (INT)Out.fParam1;

			CGFinger* pMsg = (CGFinger*)g_pPacketFactoryManager->CreatePacket(PACKET_CG_FINGER);

			CG_FINGER* pFinger = pMsg->GetFinger();
			pFinger->CleanUp();
			pFinger->m_Type = FREQ_ADVANCED;
			pFinger->m_AdvancedFinger.CleanUp();
			pFinger->m_AdvancedFinger.SetPosition( nPosition );

			if( nMenpai != INVALID_ID )
			{
				pFinger->m_AdvancedFinger.SetMenPai( nMenpai );
			}

			if( nGuildID != INVALID_ID )
			{
				pFinger->m_AdvancedFinger.SetGuildID( nGuildID );
			}

			if( nSex != INVALID_ID )
			{
				pFinger->m_AdvancedFinger.SetSex( nSex );
			}

			if( nLevelFrom != INVALID_ID )
			{
				pFinger->m_AdvancedFinger.SetBottomLevel( nLevelFrom );
			}

			if( nLevelTo != INVALID_ID )
			{
				pFinger->m_AdvancedFinger.SetTopLevel( nLevelTo );
			}

			pScene->SendPacket( pMsg, pGamePlayer->PlayerID() );

			g_pLog->FastSaveLog( LOG_FILE_1, "COMMAND_TYPE_FINGERNAME: Position=%d,Menpai=%d,GuildID=%d,Sex=%d,Level=%d~%d",
				nPosition, nMenpai, nGuildID, nSex, nLevelFrom, nLevelTo );
		}
		break;
	case COMMAND_TYPE_SETABILITYEXP:				// 生活技能升级
		{
			AbilityID_t aid;

			aid = Out.Param0;

			pHuman->__SetAbilityExp(aid, Out.Param1*100);
			GCAbilityExp Msg;
			Msg.setAbilityId(aid);
			Msg.setExp(pHuman->__GetAbilityExp(aid));
			pGamePlayer->SendPacket( &Msg );
		}
		break;
	case COMMAND_TYPE_SERMBMONEY:
		{
			pHuman->SetRMB( pHuman->GetRMB()+Out.Param0 );
			GCNotifyRMBMoney Msg;
			Msg.SetValue( Out.Param0 );
			pGamePlayer->SendPacket( &Msg );
		}
		break;
	default :
		{
		}
		break ;
	};

	g_pLog->FastSaveLog( LOG_FILE_1, "CGCommand::GUID=%X, Obj=%d, \"%s\" OK! ", 
		pHuman->GetGUID(), pHuman->GetID(), pPacket->GetCommand() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

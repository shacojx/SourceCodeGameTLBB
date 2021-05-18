#include "stdafx.h"


#include "AI_Monster.h"
#include "Obj_Character.h"
#include "Obj_Monster.h"
#include "Obj_Pet.h"
#include "Scene.h"
#include "Log.h"
#include "GameTable.h"
#include "GCChat.h"
#include "GCModifySpeed.h"
#include "AvoidOverlap.h"
#include "PathFinder.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "AIScript.h"
#include "Obj_ItemBox.h"
#include "Skill_Core.h"
#include "FileDataMgr.h"
#include "FileDef.h"
#include "Patrol.h"
#include "ScanOperator_Enemy.h"
#include "ScanOperator_Teammate.h"
#include "ItemBoxManager.h"
#include "SinCos.h"
#include "GCCharMove.h"

using namespace Combat_Module::Skill_Module;

AI_Monster::AI_Monster( )
{
	CleanUp( ) ;
}

AI_Monster::~AI_Monster( )
{
}

BOOL AI_Monster::Init( Obj_Character *pCharacter )
{
	CleanUp( ) ;
	BOOL bResult = AI_Character::Init( pCharacter );
	if ( !bResult )
	{
		return FALSE;
	}

	return TRUE;
}

VOID AI_Monster::Term( VOID )
{
__ENTER_FUNCTION
	AI_Character::Term( );
__LEAVE_FUNCTION

}

VOID AI_Monster::CleanUp( )
{
	__ENTER_FUNCTION
	Init( );
	m_ScanTimer.CleanUp( ) ;
	m_RandMoveTimer.CleanUp( ) ;
	// { 千万不能放在Init()中进行初始化
	m_nIndexOfPassed = 0;
	m_nIndexOfMoveTo = 0;
	m_baHead = TRUE;
	m_TeammateCount = 0;
	for (INT j = 0; j < MAX_TEAMMATE_COUNT; ++j)
	{
		m_TeammateID[j] = INVALID_ID;
	}
	// }

	ToIdle();

	__LEAVE_FUNCTION
}

VOID AI_Monster::Init( )
{
	__ENTER_FUNCTION
	m_EnemyCount = 0;
	m_CurEnemyID = INVALID_ID ;
	m_SkillID = 0;
	m_NextSkillID = INVALID_ID;
	m_ItemID = INVALID_ID ;
	m_bContinuePatrol = FALSE ;
	m_bLogicStop = FALSE;

	m_nRefusePaoPaoTime = 0;
	m_nSettleTime = 0;
	m_PatrolScriptID = -1;
	m_SkillSenderID = INVALID_ID;
	m_bConvoyNPC = FALSE;
	SetSpeedyOfRecover();
	for (INT i = 0; i < MAX_ENEMY_COUNT; ++i)
	{
		m_EnemyID[i] = INVALID_ID;
	}
		
    __LEAVE_FUNCTION
}
//=======================================================================================
// 逻辑状态、事件相关
//=======================================================================================
//空闲
VOID AI_Monster::AI_Logic_Idle( UINT uTime )
{
__ENTER_FUNCTION

	GET_MONSTER_SCENE((VOID)0)

	Update_RefusePaoPaoTime();

	if (TRUE == ScanEnemy( uTime ))
		return ;

	/** 随机移动处理 */
	if( AIParam(AIPARAM_RANDMOVETIME)>0	/** 能够随机移动 */
		&& !GetCharacter()->IsMoving()		/** 没有在移动 */
		&& !HasEnemy()					/** 没有敌人 */
		)
	{/** 做随机移动 */
		if( !m_RandMoveTimer.IsSetTimer() )
		{
			m_RandMoveTimer.BeginTimer(AIParam(AIPARAM_RANDMOVETIME)+(rand()%AIParam(AIPARAM_RANDMOVETIME)), uTime ) ;
		}
		if( m_RandMoveTimer.CountingTimer( uTime ) )
		{
			ToRandMove( ) ;
		}
	}
	/** 执行AI脚本 */
	ExcuteAIScript(SIDLE);

__LEAVE_FUNCTION
}

//靠近
VOID AI_Monster::AI_Logic_Approach( UINT uTime )
{
__ENTER_FUNCTION
	/** 先进行Approach类型的判断 */
	GET_MONSTER_SCENE((VOID)0)

	Obj* pObj = pScene->GetObjManager()->GetObj(m_CurEnemyID);
	if (!pObj 
		|| !IsCharacterObj(pObj->GetObjType()) 
		|| !((Obj_Character*)pObj)->IsAlive() 
		|| !((Obj_Character*)pObj)->IsCanViewMe(pMonster) 
		)
	{/** 当前敌人无效后转向下一个敌人，直到全部找完为止 */
		DelEnemy(m_CurEnemyID);
		m_CurEnemyID = GetNextEnemy();
		if (INVALID_ID == m_CurEnemyID) {
			ToGoHome();
		}
		return ;
	}
	Obj_Character* pCurEnemy = (Obj_Character*)pObj;

	
	FLOAT fMTDist,fETDist;
	if (!pCurEnemy)
	{
		Assert(NULL && "AI_Monster::Logic_Approach...pCurEnemy=NULL...");
		ToGoHome();
		return  ;
	}
	FLOAT fDist = MySqrt(pMonster->getWorldPos(), pCurEnemy->getWorldPos());
	if ( IsToGoHome(fMTDist, fETDist) ) 
	{
		return ;
	}

	if (ZERO_VALUE > fMTDist) 
	{/** 如果到达目的地 */
		ToAttack();
	}
	else
	{
		if( fETDist > (FLOAT)AIParam(AIPARAM_RESETTARGET_DIST)/1000.0f)
		{/** 如果目标位置和敌人位置的距离大与一定值则需要重新制定移动目标 */
			ToApproachTar( ) ;
		}
		else if(!GetCharacter()->IsMoving())
		{
			ToApproachTar( );
		}
	}
	ExcuteAIScript(SAPPROACH);

__LEAVE_FUNCTION
}

VOID AI_Monster::AI_Logic_Flee( UINT uTime )
{
	__ENTER_FUNCTION
	GET_MONSTER_SCENE((VOID)0)

	Obj* pObj = pScene->GetObjManager()->GetObj(m_CurEnemyID);
	if (!pObj 
		|| !IsCharacterObj(pObj->GetObjType()) 
		|| !((Obj_Character*)pObj)->IsAlive() 
		|| !((Obj_Character*)pObj)->IsCanViewMe(pMonster) 
		)
	{/** 当前敌人无效后转向下一个敌人，直到全部找完为止 */
		DelEnemy(m_CurEnemyID);
		m_CurEnemyID = GetNextEnemy();
		if (INVALID_ID == m_CurEnemyID) {
			ToGoHome();
		}
		return ;	
	}
	Obj_Character* pCurEnemy = (Obj_Character*)pObj;

	/** 当前位置和目的地之间的距离 */
	FLOAT fDist  = MySqrt(GetCharacter()->getWorldPos(),GetCharacter()->GetFinalTarPos());
	if( pCurEnemy==NULL			/** 无效敌人 */
		|| !pCurEnemy->IsAlive()	/** 敌人死了 */
		|| fDist < ZERO_VALUE		/** 到达目的地 */
		)	
	{/** 忘掉当前目标 */
		/** 将自身的移动速度还原 */
		GetCharacter()->SetMoveMode(Obj_Monster::MOVE_MODE_WALK);

		switch(m_FleeType) 
		{
			/** 返回继续攻击敌人 */
		case MONSTERAI_FLEE_RANDOM:
			ToApproachTar();
			break;
			/** 将自己的仇恨列表copy给队友 */
		case MONSTERAI_FLEE_NEARESTFRIEND:
			for (INT i = 0; i < MAX_TEAMMATE_COUNT; ++i)
			{
				Obj* pObj = pScene->GetObjManager()->GetObj(m_TeammateID[i]);
				if( pObj==NULL || !IsCharacterObj( pObj->GetObjType() ) )
				{
					continue;
				}
				/** 活着的；非敌对的；没有敌人的 */
				if (((Obj_Character*)pObj)->IsAlive() 
					&& !((Obj_Monster*)pObj)->IsEnemy(GetCharacter())
					&& !((Obj_Monster*)pObj)->GetMonsterAI()->HasEnemy()	
					)
				{
					((Obj_Monster*)pObj)->GetMonsterAI()->AddPrimaryEnemy( GetPrimaryEnemyID() );
					//((Monster*)pObj)->GetMonsterAI()->ToApproachTar();
					g_pLog->FastSaveLog( LOG_FILE_1, "Logic_Flee NearestFriend Find Obj! ID:%d",
						pObj->GetID() ) ;
				}
			}
			ToApproachTar();
			break;
			/** 将自己的仇恨列表copy给周围一定范围的队友 */
		case MONSTERAI_FLEE_NEARESTPOINT:
			{
				Scene* pScene = GetCharacter()->getScene() ;

				SCANOPERATOR_TEAMMATE_INIT Init ;
				Init.m_pScene = pScene ;
				Init.m_nZoneRadius = 1 ;
				Init.m_Type = MONSTERAI_SUMMON_REGION;
				Init.m_nCount = 3;
				Init.m_bScanHuman = FALSE ;
				Init.m_ZoneID = GetCharacter()->getZoneID() ;
				Init.m_pMonster = (Obj_Monster*)GetCharacter() ;
				Init.m_fRadius = (FLOAT)AIParam(AIPARAM_SCANTEAMMATEDIST)/1000.0f ;

				ScanOperator_Teammate	sc;
				sc.Init( &Init ) ;

				pScene->Scan( &sc ) ;

				if (HasTeammate()) 
				{
					Obj* pObj = NULL;
					for (INT i = 0; i < MAX_TEAMMATE_COUNT; ++i)
					{
						Obj* pObj = pScene->GetObjManager()->GetObj(m_TeammateID[i]);
						if( pObj==NULL || !IsCharacterObj( pObj->GetObjType() ) )
						{
							continue;
						}
						((Obj_Monster*)pObj)->GetMonsterAI()->AddPrimaryEnemy( GetPrimaryEnemyID() );
						//((Monster*)pObj)->GetMonsterAI()->ToApproachTar();
						g_pLog->FastSaveLog( LOG_FILE_1, "Logic_Flee NearestPoINT Find Obj! ID:%d",
							pObj->GetID() ) ;
					}
				}
				ToApproachTar();
			}
			break;
		default:
			Assert(false);
			break;
		}
	}
	ExcuteAIScript(SFLEE);

	__LEAVE_FUNCTION
}

VOID AI_Monster::AI_Logic_Patrol(UINT uTime)
{
__ENTER_FUNCTION
	GET_MONSTER_SCENE((VOID)0)

	/** 扫描敌人 */
	if (TRUE == ScanEnemy( uTime )) 
	{
		return ;
	}
	/* 是否要继续巡逻 */
	if (!m_bConvoyNPC && !m_bContinuePatrol) 
	{
		Stop();
		return ;
	}

	if(!pMonster->IsMoving())
	{/** 向下一个巡逻点Move */
		//AxTrace(0, 0, "Monster Stop Moving objid=%d", pMonster->GetID());
		// 执行该点的事件, 以后扩展用
		if ( m_PatrolScriptID != (ScriptID_t)(0xFFFFFFFF) ) {
			ProcessPaoPao(); // test only:)
		}
		// 开始逗留
		if (m_nSettleTime > 0) {		
			m_nSettleTime -= pMonster->GetLogicTime();
			return ;
		}

		m_nIndexOfPassed = m_nIndexOfMoveTo;
		PatrolPathMgr* pPatrolPathMgr = pScene->GetPatrolPathMgr();
		INT patrolPathIndex = pMonster->GetPatrolPathIndex();
		WORLD_POS Tar;
		m_nIndexOfMoveTo = pPatrolPathMgr->GetPatrolPoint( patrolPathIndex, 
															m_nIndexOfPassed, 
															m_baHead,
															Tar,
															m_nSettleTime,
															m_PatrolScriptID);

		pScene->GetMap()->VerifyPos(&Tar);
		MonsterGo(&Tar);
		m_nRefusePaoPaoTime = 0; // test only:)
	}
	ExcuteAIScript(SPATROL);

__LEAVE_FUNCTION
}		

VOID AI_Monster::AI_Logic_Gohome(UINT uTime )
{
__ENTER_FUNCTION
	RecoverHP();

	/** 当前位置和目的地之间的距离 */
	FLOAT fDist  = MySqrt(GetCharacter()->getWorldPos(),GetCharacter()->GetFinalTarPos());
	if (fDist < ZERO_VALUE) 
		ToIdle();
	ExcuteAIScript(SRETURN);

__LEAVE_FUNCTION
}

VOID AI_Monster::AI_Logic_Dead(UINT uTime)
{
__ENTER_FUNCTION
	if ( m_BodyTimer.CountingTimer( uTime ) )
	{
		m_BodyTimer.CleanUp();
		// 重生
		Obj_Monster *pMonster = GetCharacter();
		pMonster->Enter_Respawn();
	}
__LEAVE_FUNCTION
}

VOID AI_Monster::AI_Logic_Combat( UINT uTime )
{
__ENTER_FUNCTION
	GET_MONSTER_SCENE((VOID)0)

	Obj* pObj = pScene->GetObjManager()->GetObj(m_CurEnemyID);
	if (
		NULL == pObj ||
		FALSE == IsCharacterObj(pObj->GetObjType()) || 
		FALSE == ((Obj_Character*)pObj)->IsAlive() ||
		FALSE == ((Obj_Character*)pObj)->IsCanViewMe(pMonster) 
		)
	{/** 当前敌人无效后转向下一个敌人，直到全部找完为止 */
		DelEnemy(m_CurEnemyID);
		m_CurEnemyID = GetNextEnemy();
		if (INVALID_ID == m_CurEnemyID) {
			ToGoHome();
		}
		return ;
	}
	Obj_Character* pCurEnemy = (Obj_Character*)pObj;
	/** 当前的逻辑还没停止 即还在释放技能  */
	if (TRUE == pMonster->IsMoving() ||
		FALSE == pMonster->CanUseSkillNow() )
	{
		return ;
	}
	// 假如目标处于无敌状态则转向下一个目标
	if (pCurEnemy->IsUnbreakable()) {
		ObjID_t nextEnemyID = GetNextEnemy();
        if (INVALID_ID != nextEnemyID)
		{
			Obj* pObj = pScene->GetSpecificObjByID(nextEnemyID);
			// 得到的nextenemy有效且不是无敌的才转换攻击目标
			if (pObj && IsCharacterObj(pObj->GetObjType()) == TRUE && 
				((Obj_Character*)pObj)->IsUnbreakable() == FALSE)
			{
				m_CurEnemyID = nextEnemyID;
				ToApproachTar();
				return ;
			}
		}
	}
	
	/* 判断当前敌人有没有辅助者，如果有则一定几率的攻击辅助者 */
	if ( pObj->GetObjType() == Obj_Character::CHARACTER_CLASS_HUMAN &&
		((Obj_Human*)pObj)->GetAssistantID() != INVALID_ID ) 
	{
		if ( AddEnemy( ((Obj_Human*)pObj)->GetAssistantID() ) ) 
		{
			if ( IsNeedChangeEnemy() ) 
			{
				SetCurEnemyID( ((Obj_Human*)pObj)->GetAssistantID() );
				ToApproachTar();
				return ;
			} 
		}
	}

	if (m_SAICommand.commandType != E_COMMAND_TYPE_INVALID)
	{// 如果命令缓存中有数据则先执行缓存中的Command
		ExcuteCommand();
		return ;
	}
	FLOAT fDist = MySqrt(pMonster->getWorldPos(), pCurEnemy->getWorldPos()) ;
    /** 如果怪物与玩家太近，则顺移一定距离 */
	if (MIN_DIST > fDist ) 
	{
		WORLD_POS Tar;
		Tar.m_fX = pMonster->getWorldPos()->m_fX + FX_DELTA;
		Tar.m_fZ = pMonster->getWorldPos()->m_fZ + FZ_DELTA;
		pScene->GetMap()->VerifyPos(&Tar);
		MovePhonily(Tar);
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	// 先施放缓存中的技能，该缓存中只有一个技能
	if (INVALID_ID != m_NextSkillID)
	{
		ORESULT oResult = UseSkill(m_SkillID, 1, m_CurEnemyID, 
			pCurEnemy->getWorldPos()->m_fX, pCurEnemy->getWorldPos()->m_fZ);

		m_NextSkillID = INVALID_ID;
	}

	if (TRUE == ExcuteAIScript(SATTACK)) {
		return ;
	}
	if (TRUE == ExcuteAIScript(SKILLSECTION)) {
		return ;
	}
	ORESULT oResult = UseSkill(0, 1,
					m_CurEnemyID, 
					pCurEnemy->getWorldPos()->m_fX, 
					pCurEnemy->getWorldPos()->m_fZ);

	if (OR_OUT_RANGE == oResult) {
		ToApproachTar();
	}

__LEAVE_FUNCTION
}

VOID AI_Monster::AI_Logic_Service( UINT uTime )
{
__ENTER_FUNCTION

	GET_MONSTER_SCENE((VOID)0)

	if( !m_ScanTimer.IsSetTimer() )
	{/** 设置时间控制器 */
		m_ScanTimer.BeginTimer( AIParam(AIPARAM_SCANTIME), uTime ) ;
		return ;
	}
	if( !m_ScanTimer.CountingTimer( uTime ) )
	{/** 时间没到 */
		return ;
	}
	/** 扫描周围还有没有human */
	HUMANLIST listHuman;
	pScene->ScanHuman(pMonster->getZoneID(), 1, &(listHuman));

	if (listHuman.m_Count <= 0) {
		StopService();
	}

__LEAVE_FUNCTION
}

VOID	AI_Monster::AI_Logic_TeamFollow(UINT uTime)
{
__ENTER_FUNCTION

	GET_MONSTER_SCENE((VOID)0)

	Update_RefusePaoPaoTime();

	if (TRUE == ScanEnemy( uTime ))
	{
		return ;
	}

__LEAVE_FUNCTION
}

VOID AI_Monster::Event_OnBeSkill( Obj_Character* pObj_Character, INT nGoodEffect)
{
__ENTER_FUNCTION

	/** 检测是否会还击 */
	if ( 0 >= AIParam(AIPARAM_STRIKEBACK) )
	{/** 不会还击则返回，如兔子等小动物 */
		return ;
	}
	/**  如果是放弃追击状态则不还击，但会miss掉  */
	if ( ESTATE_GOHOME == GetAIState()->GetStateID() ) 
	{
		/**	向客户端发送被Miss的消息 */
		g_pLog->FastSaveLog( LOG_FILE_1, "OnBeSkill Obj! ID:%d MISS!!!",
			GetCharacter()->GetID()) ;
		return ;
	}
	if(NULL == pObj_Character /** 无效攻击者 */
		|| pObj_Character->GetID() == GetCharacter()->GetID() /** 攻击者是自己 */
		|| !pObj_Character->IsAlive() /** 攻击者是个死人 */
		)
		return ;
	//判断是否是敌对行为
	if (BEHAVIOR_TYPE_HOSTILITY!=nGoodEffect)
	{
		return; //不是敌对行为,返回
	}

    if(INVALID_ID  == m_CurEnemyID)
	{/** 没有敌人时候 */
		AddPrimaryEnemy(pObj_Character->GetID());
		return ;
	}
	else if( m_CurEnemyID == pObj_Character->GetID() )
	{/** 攻击者就是当前的敌人 */
		if (TRUE == GetCharacter()->IsCharacterLogicStopped()) 
		{
			ExcuteAIScript(ONBESKILLSECTION);
		}
		return ;
	}
	else if (TRUE == IsAlreadyExist( pObj_Character->GetID()) ) 
	{/** 攻击者已经在敌人列表中 此时按照一定几率改变敌人 */
		if (TRUE == IsNeedChangeEnemy()) 
		{
			SetCurEnemyID( pObj_Character->GetID() );
			ToApproachTar();
		}
	}
	else
	{/** 是新的攻击者 此时按照一定几率改变敌人 */
		if ( !AddEnemy(pObj_Character->GetID()) )
		{
			return ;
		}
		if (TRUE == IsNeedChangeEnemy()) 
		{
			SetCurEnemyID( pObj_Character->GetID() );
			ToApproachTar();
		}
	}

	__LEAVE_FUNCTION
}

VOID AI_Monster::Event_OnDie( Obj *pKiller/* = NULL*/ )
{
	__ENTER_FUNCTION

	ObjID_t idKiller = INVALID_ID;
	if ( pKiller != NULL )
	{
		idKiller = pKiller->GetID() ;
	}
	
	ChangeState(ESTATE_DEAD);

	m_BodyTimer.BeginTimer( g_Config.m_ConfigInfo.m_DefaultBodyTime, g_pTimeManager->CurrentTime() );

	__LEAVE_FUNCTION

}

VOID AI_Monster::Event_OnDamage(INT nDamage, Obj_Character* pAttacker)
{
	__ENTER_FUNCTION
	Obj_Monster* pMe=(Obj_Monster*)GetCharacter();
	if(NULL!=pMe)
	{
		pMe->UpdateDamageList(nDamage, pAttacker);
	}

	/** 执行AI脚本 */
	ExcuteAIScript(ONDAMAGESECTION);
	__LEAVE_FUNCTION
}

//=======================================================================================
//  敌人相关
//=======================================================================================
BOOL AI_Monster::ScanEnemy( UINT uTime )
{
	__ENTER_FUNCTION
		if( AIParam(AIPARAM_SCANTIME)>0 /** 主动攻击的怪才会搜索敌人 */
			&& !HasEnemy()				/** 没有敌人的怪才去搜索敌人 */
			)
		{
			if( !m_ScanTimer.IsSetTimer() )
			{/** 设置时间控制器 */
				m_ScanTimer.BeginTimer( AIParam(AIPARAM_SCANTIME), uTime ) ;
				return FALSE ;
			}
			if( !m_ScanTimer.CountingTimer( uTime ) )
			{/** 时间没到 */
				return FALSE ;
			}
			/** 扫描敌人 */
			Scene* pScene = GetCharacter()->getScene() ;
			
			SCANOPERATOR_ENEMY_INIT Init ;
			Init.m_pScene = pScene ;
			Init.m_nZoneRadius = 1;
			Init.m_bScanHuman = FALSE;
			Init.m_ZoneID = GetCharacter()->getZoneID() ;
			Init.m_pMonster = (Obj_Monster*)GetCharacter() ;
			Init.m_fRadius = (FLOAT)AIParam(AIPARAM_SCANENEMYDIST)/1000.0f ;

			Monster_ScanOperator_Enemy	sc;
			sc.Init( &Init ) ;

			pScene->Scan( &sc ) ;

			if( HasEnemy( ) )
			{/** 扫描到了敌人 */
				/* 1. 执行特殊的扩展AI，例如根据玩家身上的某些标识决定是否真正的去攻击玩家 */

				/* 2. 根据与玩家的等级差决定是否真正的攻击玩家 */

				// 将Monster设置成跑步状态
				GetCharacter()->SetMoveMode(Obj_Monster::MOVE_MODE_RUN);
				ToApproachTar();
				//Log::SaveLog( SERVER_LOGFILE, "ScanEnemy Find Obj! ID:%d",
				//	GetCharacter()->GetID() ) ;
				return TRUE ;
			}
		}
		return FALSE ;
		__LEAVE_FUNCTION	
			return FALSE ;
}

BOOL AI_Monster::AddEnemy( ObjID_t EnemyID )
{
	__ENTER_FUNCTION
	if (INVALID_ID == EnemyID)
	{
		return FALSE;
	}
	for (INT i = 0; i < MAX_ENEMY_COUNT; ++i)
	{
		if (m_EnemyID[i] == INVALID_ID)
		{
			m_EnemyID[i] = EnemyID;
			++m_EnemyCount;
			return TRUE;
		}
	}

    return FALSE;
	__LEAVE_FUNCTION
	return FALSE ;
}

VOID AI_Monster::AddPrimaryEnemy( ObjID_t EnemyID )
{
	__ENTER_FUNCTION

	AddEnemy(EnemyID);
	
	Obj_Monster* pMonster = GetCharacter();
	if (!pMonster)
	{
		Assert(NULL && "AI_Monster::AddPrimaryEnemy...pMonster=NULL");
		return ;
	}
	Scene* pScene = pMonster->getScene();
	if (!pScene)
	{
		Assert(NULL && "AI_Monster::AddPrimaryEnemy...pScene=NULL");
		return ;
	}
	// 如果是宠物则将他的主人也加入仇恨列表(如果有的话)
	Obj* pObj = pScene->GetObjManager()->GetObj(EnemyID);
	if (pObj && pObj->GetObjType() == Obj::OBJ_TYPE_PET) {
		Obj_Human* pCreator = ((Obj_Pet*)pObj)->GetCreator();
		if (pCreator) {
			AddEnemy(pCreator->GetID());
		}
	}
	// 招呼所有的队友战斗
	Teammate_Go_Fight(EnemyID);

	m_CurEnemyID = EnemyID;
	m_ToAttackPos = *(pMonster->getWorldPos()) ;

	ToApproachTar();

	__LEAVE_FUNCTION
}

BOOL AI_Monster::IsAlreadyExist( ObjID_t EnemyID )
{
	__ENTER_FUNCTION
	for (INT i = 0; i < MAX_ENEMY_COUNT; ++i)
	{
		if (m_EnemyID[i] == EnemyID)
		{
			return TRUE;
		}
	}

	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL AI_Monster::IsNeedChangeEnemy( )
{
	__ENTER_FUNCTION
	INT ret =  rand() % 100 ;
	if( ret >= 0 && ret < CHANGE_ENEMY_RATE)
		return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

ObjID_t	AI_Monster::GetNextEnemy( )
{
	__ENTER_FUNCTION
	ObjID_t tmpEnemyID = INVALID_ID;
	for (INT i = 0; i < MAX_ENEMY_COUNT; ++i)
	{
		if (INVALID_ID != m_EnemyID[i] && 
			m_CurEnemyID != m_EnemyID[i])
		{
			GET_MONSTER_SCENE(INVALID_ID);
			Obj* pObj = pScene->GetObjManager()->GetObj(m_EnemyID[i]);
			if (pObj && IsCharacterObj(pObj->GetObjType()))
			{
				if (FALSE == ((Obj_Character*)pObj)->IsUnbreakable())
				{// 如果不是无敌状态则直接返回该敌人
					return m_EnemyID[i];		
				}
				else
				{
					tmpEnemyID = m_EnemyID[i];
				}
			}
		}
	}

	return tmpEnemyID;

	__LEAVE_FUNCTION
	return INVALID_ID;
}

BOOL AI_Monster::DelEnemy( ObjID_t EnemyID )
{
	__ENTER_FUNCTION
	if (INVALID_ID == EnemyID)
	{
		return FALSE;
	}
	for (INT i = 0; i < MAX_ENEMY_COUNT; ++i)
	{
		if (EnemyID == m_EnemyID[i])
		{
			if (m_CurEnemyID == EnemyID)
			{
				m_CurEnemyID = INVALID_ID;
			}
			m_EnemyID[i] = INVALID_ID;
			--m_EnemyCount;
			return TRUE;
		}
	}

	return FALSE;
	__LEAVE_FUNCTION
	return FALSE ;
}

VOID AI_Monster::DelAllEnemy()
{
	__ENTER_FUNCTION
	for (INT i = 0; i < MAX_ENEMY_COUNT; ++i)
	{
		m_EnemyID[i] = INVALID_ID;
		m_EnemyCount = 0;
	}
	__LEAVE_FUNCTION
}

Obj_Character*  AI_Monster::GetCurEnemy(VOID)
{
__ENTER_FUNCTION
	Obj_Monster* pMonster = GetCharacter();
	if (!pMonster)
	{
		Assert(NULL && "AI_Monster::GetCurEnemy...pMonster=NULL...");
		return NULL;
	}
	if (FALSE == pMonster->IsAlive())
	{// 自身已经死亡
		return NULL;
	}
		
	Obj* pObj = pMonster->getScene()->GetObjManager()->GetObj(m_CurEnemyID) ;
	if( pObj && IsCharacterObj(pObj->GetObjType()) )
	{
		return (Obj_Character*)pObj;
	}
	
	return NULL;
__LEAVE_FUNCTION
	return NULL;
}

VOID AI_Monster::ChangePrimaryEnemy()
{
	ObjID_t idNextEnemy = GetNextEnemy();
	if (INVALID_ID != idNextEnemy)
	{
		m_CurEnemyID = idNextEnemy;
	}
}
//=======================================================================================
// 队友相关
//=======================================================================================
VOID AI_Monster::AddTeammate( ObjID_t TeammateID )
{
	__ENTER_FUNCTION
	for (INT i = 0; i < MAX_TEAMMATE_COUNT; ++i)
	{
		if (INVALID_ID == m_TeammateID[i])
		{
			m_TeammateID[i] = TeammateID;
			++m_TeammateCount;
			break;
		}
	}
	__LEAVE_FUNCTION
}

VOID AI_Monster::DelAllTeammate( )
{
	__ENTER_FUNCTION
	for (INT i = 0; i < MAX_TEAMMATE_COUNT; ++i)
	{
		m_TeammateID[i] = INVALID_ID;
		m_TeammateCount = 0;
		break;
	}

	__LEAVE_FUNCTION
}

VOID AI_Monster::SummonTeammate( UINT uTime, INT type, INT count )
{
	__ENTER_FUNCTION
	Obj_Monster* pMonster = GetCharacter();
	if (!pMonster)
	{
		AssertEx(FALSE,"[AI_Monster::SummonTeammate]...pMonster = NULL...");
		return ;
	}
	Scene* pScene = pMonster->getScene();
	if (!pScene) 
	{
		AssertEx(FALSE,"[AI_Monster::SummonTeammate]...pScene = NULL...");
		return ;
	}

	switch(type) 
	{
	case MONSTERAI_SUMMON_REGION :
		{
			SCANOPERATOR_TEAMMATE_INIT Init ;
			Init.m_pScene = pScene ;
			Init.m_nZoneRadius = 1;
			Init.m_Type = MONSTERAI_SUMMON_REGION;
			Init.m_nCount = count;
			Init.m_bScanHuman = FALSE ;
			Init.m_ZoneID = GetCharacter()->getZoneID() ;
			Init.m_pMonster = (Obj_Monster*)GetCharacter() ;
			Init.m_fRadius = (FLOAT)AIParam(AIPARAM_SCANTEAMMATEDIST)/1000.0f ;

			ScanOperator_Teammate	sc;
			sc.Init( &Init ) ;

			pScene->Scan( &sc ) ;

		}

		break;
	case MONSTERAI_SUMMON_TEAM:
		/**	获取所在群组的成员，并将自己的当前敌人copy给队友 */

		break;
	default:
		Assert(false);
		break;
	}
	/** 扫描到了队友,是队友转换成APPROCH状态 */	
	if( HasTeammate( ) )
	{	 
		for (INT i = 0; i < MAX_TEAMMATE_COUNT; ++i)
		{
			Obj* pObj = pScene->GetObjManager()->GetObj(m_TeammateID[i]);
			if( pObj==NULL || !IsCharacterObj( pObj->GetObjType() ) )
			{
				continue;
			}
			((Obj_Monster*)pObj)->GetMonsterAI()->AddPrimaryEnemy( GetPrimaryEnemyID() );
			//((Monster*)pObj)->GetMonsterAI()->ToApproachTar();
			g_pLog->FastSaveLog( LOG_FILE_1, "SummonTeammate Find Obj! ID:%d",
				pObj->GetID() ) ;
		}
		return ;
	}	
	__LEAVE_FUNCTION
}

WORLD_POS AI_Monster::FindNearestFriend()
{
	WORLD_POS pos;
	__ENTER_FUNCTION

	Scene* pScene = GetCharacter()->getScene() ;

	SCANOPERATOR_TEAMMATE_INIT Init ;
	Init.m_pScene = pScene ;
	Init.m_nZoneRadius = 1;
	Init.m_Type = MONSTERAI_SUMMON_REGION;
	Init.m_nCount = 3;
	Init.m_bScanHuman = FALSE ;
	Init.m_ZoneID = GetCharacter()->getZoneID() ;
	Init.m_pMonster = (Obj_Monster*)GetCharacter() ;
	Init.m_fRadius = (FLOAT)AIParam(AIPARAM_SCANTEAMMATEDIST)/1000.0f ;

	ScanOperator_Teammate	sc;
	sc.Init( &Init ) ;

	pScene->Scan( &sc ) ;

	/** 扫描到了队友,是队友转换成APPROCH状态 */	
	if( HasTeammate( ) )
	{	 
		for (INT i = 0; i < MAX_TEAMMATE_COUNT; ++i)
		{
			Obj* pObj = pScene->GetObjManager()->GetObj(m_TeammateID[i]);
			if( pObj==NULL || !IsCharacterObj( pObj->GetObjType() ) )
			{
				continue;
			}
			pos = *pObj->getWorldPos();
			pos.m_fX -= 2.f;
			Assert(pos.m_fX > 0.f);
			g_pLog->FastSaveLog( LOG_FILE_1, "FindNearestFriend Find Obj! ID:%d",
				pObj->GetID() ) ;
			return pos;
		}
		pos = GetRandPosOfCircle( GetCharacter()->getWorldPos(),MAX_FLEE_LENGTH) ;
	}
	else
		pos = GetRandPosOfCircle( GetCharacter()->getWorldPos(),MAX_FLEE_LENGTH) ;

	return pos;
	__LEAVE_FUNCTION
	return pos;
}

VOID	AI_Monster::StartTeamFollow()
{
__ENTER_FUNCTION
	Obj_Monster* pMonster = GetCharacter();
	if  (!pMonster)
	{
		Assert(NULL && "AI_Monster::StartTeamFollow...pMonster=NULL");
		return;
	}

	Obj* pObj = pMonster->getScene()->GetObjManager()->GetObj(pMonster->GetLeaderID());
	if (pObj && pObj->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		AI_Monster* pAI = (AI_Monster*)( ((Obj_Monster*)pObj)->GetAIObj() );
		pAI->AddTeammate(pMonster->GetID());
	}

	ChangeState(ESTATE_TEAMFOLLOW);

__LEAVE_FUNCTION
}

VOID	AI_Monster::AddEnemyToTeammate(ObjID_t EnemyID)
{
__ENTER_FUNCTION
	GET_MONSTER_SCENE((VOID)0)
	for (INT i = 0; i < MAX_TEAMMATE_COUNT; ++i)
	{
		Obj* pObj = pScene->GetObjManager()->GetObj(m_TeammateID[i]);
		if (pObj && pObj->GetObjType() == Obj::OBJ_TYPE_MONSTER)
		{
			AI_Monster* pAI = (AI_Monster*)((Obj_Monster*)pObj)->GetAIObj();
			if (pAI) 
			{
				//pAI->AddPrimaryEnemy(EnemyID); //这样会引起循环调用-->crash!!
				pAI->AddEnemy(EnemyID);
				pAI->SetCurEnemyID(EnemyID);
				pAI->ToApproachTar();
				pAI->SetAttackPos(*(pObj->getWorldPos()) );
			}
		}
		else
		{
			break;
		}
	}

__LEAVE_FUNCTION
}

VOID	AI_Monster::Teammate_Go_Fight(ObjID_t EnemyID)
{
__ENTER_FUNCTION
	GET_MONSTER_SCENE((VOID)0)
	// 是队长的不可以再是member(即不可以再跟随别人)==member只可以有leader不可以再有member
	if (m_TeammateCount > 0)
	{// 是队长!
		AddEnemyToTeammate(EnemyID);
	}
	else if (INVALID_ID != pMonster->GetLeaderID())
	{// 是成员
		Obj* pObj = pScene->GetObjManager()->GetObj(pMonster->GetLeaderID());
		if (pObj && pObj->GetObjType() == Obj::OBJ_TYPE_MONSTER)
		{
			AI_Character* pAI = ((Obj_Monster*)pObj)->GetAIObj();
			if (pAI)
			{
				((AI_Monster*)pAI)->AddEnemyToTeammate(EnemyID);
			}
		}
	}

__LEAVE_FUNCTION
}

//=======================================================================================
// 巡逻部分
//=======================================================================================
VOID AI_Monster::StartPatrol(BOOL bConvoyNPC/*=FALSE*/)
{
	__ENTER_FUNCTION
	m_bConvoyNPC = bConvoyNPC;
	m_bContinuePatrol = TRUE;
	
	PatrolPathMgr* pPatrolPathMgr = GetCharacter()->getScene()->GetPatrolPathMgr();
	INT patrolPathIndex = GetCharacter()->GetPatrolPathIndex();
	WORLD_POS Tar;
	m_nIndexOfMoveTo = pPatrolPathMgr->GetPatrolPoint( patrolPathIndex, 
														m_nIndexOfPassed, 
														m_baHead,
														Tar,
														m_nSettleTime,
														m_PatrolScriptID);

	GetCharacter()->getScene()->GetMap()->VerifyPos(&Tar);
	if(GetCharacter()->getScene()->GetMap()->IsCanGo(Tar,0)) {//最小的level
		MonsterGo(&Tar);
	}
	else {
		GetCharacter()->Teleport(&Tar);
	}
	
	ChangeState(ESTATE_PATROL);
	__LEAVE_FUNCTION
}

VOID AI_Monster::PausePatrol( VOID )
{
	m_bContinuePatrol = FALSE ;
}

VOID AI_Monster::StopPatrol( VOID )
{
__ENTER_FUNCTION
	m_bContinuePatrol = FALSE ;
	m_bConvoyNPC = FALSE;

	Stop();

	m_nIndexOfPassed = 0;
	Obj_Monster* pMonster = GetCharacter();
	// 将NPC瞬移到出生点
	if (pMonster)
	{
		pMonster->Teleport( pMonster->GetRespawnPos() );
	}

	ChangeState(ESTATE_IDLE);

__LEAVE_FUNCTION    
}

//=======================================================================================
//  行为部分 
//===============================================================================u========
BOOL AI_Monster::ToRandMove( )
{
__ENTER_FUNCTION

	WORLD_POS Tar ;
	BOOL FindPos=FALSE ;
	for( INT i=0; i<MAX_RAND_TIMES; i++ )
	{
		Tar.m_fX = GetCharacter()->GetRespawnPos()->m_fX+rand()%GetCharacter()->GetPosRange()-rand()%GetCharacter()->GetPosRange() ;
		Tar.m_fZ = GetCharacter()->GetRespawnPos()->m_fZ+rand()%GetCharacter()->GetPosRange()-rand()%GetCharacter()->GetPosRange() ;
		GetCharacter()->getScene()->GetMap()->VerifyPos( &Tar ) ;
		if( GetCharacter()->getScene()->GetMap()->IsCanGo( Tar,0 ) )
		{
			FindPos = TRUE ;
			break ;
		}
	}

	if( FindPos==FALSE )
		return FALSE;

	if(OR_OK != Move( &Tar ))
		return FALSE;

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL AI_Monster::ToIdle( )
{
__ENTER_FUNCTION
	
	// 将Monster设置成走步状态
	Obj_Monster* pMonster = GetCharacter();
	if (pMonster)
	{
		pMonster->ClearUnbreakableFlag();
		pMonster->SetMoveMode(Obj_Monster::MOVE_MODE_WALK);

		Init();
		if ( pMonster->isPatrolMonster() )
		{// 如果是巡逻怪则重新开始巡逻
			StartPatrol();
		}
		else if (INVALID_ID != pMonster->GetLeaderID())
		{
			ChangeState(ESTATE_TEAMFOLLOW);
		}
		else
		{
			ChangeState(ESTATE_IDLE);
		}

		/* 重置自身的Killer */
		pMonster->ClearKiller();
		/** 重置脚本信息 */
		if (m_AIScriptID >= 0 && m_AIScriptID < AISCRIPT_NUM) {
			g_pAIScriptList[m_AIScriptID]->ResetStateList(this);
		}
	}

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL AI_Monster::ToFlee(INT type,FLOAT fX,FLOAT fZ)
{	
__ENTER_FUNCTION
	Obj_Monster* pMonster = GetCharacter();
	if (!pMonster)
	{
		Assert(NULL && "AI_Monster::ToFlee...pMonster = NULL...");
		return FALSE;
	}
	WORLD_POS Tar = *(pMonster->getWorldPos());
	m_FleeType = type;
	switch(type) {
	case MONSTERAI_FLEE_RANDOM:
		/**	向以自身为圆心，AIParam(AIPARAM_SCANENEMYDIST)/1000.0f为半径的圆周上的随机一点逃去 */
		Tar = *pMonster->getWorldPos();
		Tar = GetRandPosOfCircle(&Tar ,MAX_FLEE_LENGTH) ;
		break;
	case MONSTERAI_FLEE_NEARESTFRIEND:
		/** 向自己身边最近的队友跑去 */
		Tar = FindNearestFriend();
		break;
	case MONSTERAI_FLEE_NEARESTPOINT:
		/** 向指定逃跑点的最近一处逃去 */
		Tar.m_fX = fX;
		Tar.m_fZ = fZ;
		break;
	default:
		Assert(FALSE);
		break;
	}
	pMonster->getScene()->GetMap()->VerifyPos(&Tar) ;
	if (FALSE == pMonster->getScene()->GetMap()->IsCanGo(Tar,10))//逃跑用最大level
	{
		return FALSE;
	}

	/** 直线判断 */
	//const WORLD_POS* pCur = GetCharacter()->getWorldPos() ;
	//WORLD_POS posNode[MAX_CHAR_PATH_NODE_NUMBER];
	//INT numNode = 0;
	//WORLD_POS curPos = *pCur;
	//GetCharacter()->getScene()->GetMap()->GetPathFinder()->FindPath(&curPos,&Tar,posNode,numNode,true);	
	//Assert( numNode == 1 ) ;
	/** 将Monster的移动速度变慢 */
	GetCharacter()->SetMoveMode(Obj_Monster::MOVE_MODE_HOBBLE);

	if (OR_OK != Move( &Tar ))
	{
		GetCharacter()->SetMoveMode(Obj_Monster::MOVE_MODE_WALK);
		return FALSE;
	}

	ChangeState(ESTATE_FLEE);
__LEAVE_FUNCTION
	return TRUE ;
}

BOOL AI_Monster::ToAttack(VOID)
{
__ENTER_FUNCTION

	Obj_Monster* pMonster = GetCharacter();
	if (!pMonster) {
		Assert(NULL && "AI_Monster::ToAttack...pMonster=NULL...");
		return FALSE;
	}
	Scene* pScene = pMonster->getScene();
	if (!pScene) {
		Assert(NULL && "AI_Monster::ToAttack...pScene=NULL...");
		return FALSE;
	}
	// 设置成走步模式
	pMonster->SetMoveMode(Obj_Character::MOVE_MODE_WALK);

	/** 随机偏移处理，防止所有怪重叠在一起 */ 
	WORLD_POS Tar;
	if (TRUE == GetBestPos(Tar))	
	{// 已经做了位置有效性检测
		pMonster->DirectMoveTo(&Tar);
	}

	ChangeState(ESTATE_COMBAT);

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL AI_Monster::ToGoHome()
{
__ENTER_FUNCTION
	GET_MONSTER_SCENE(FALSE)
	WORLD_POS Tar;
	Tar = m_ToAttackPos;
	// 忘掉敌人
	DelAllEnemy();
	// 将自己设置成无敌状态
	GetCharacter()->MarkUnbreakableFlag();
	// 将Monster设置成跑步状态
	GetCharacter()->SetMoveMode(Obj_Monster::MOVE_MODE_RUN);

	if(OR_OK != Move(&Tar)) {
		GetCharacter()->DirectMoveTo(&Tar);
	}

	ChangeState(ESTATE_GOHOME);
	return TRUE;
		
__LEAVE_FUNCTION
	return FALSE;
}

BOOL AI_Monster::ToApproachTar( )
{
	__ENTER_FUNCTION
	GET_MONSTER_SCENE(FALSE);

	// 设置成跑步模式
	pMonster->SetMoveMode(Obj_Character::MOVE_MODE_RUN);

	Obj* pObj = pScene->GetObjManager()->GetObj(m_CurEnemyID);
	if(!pObj || !IsCharacterObj(pObj->GetObjType()) || !((Obj_Character*)pObj)->IsAlive() )
	{/** 当前敌人无效后转向下一个敌人，直到全部找完为止 */
		DelEnemy(m_CurEnemyID);
		m_CurEnemyID = GetNextEnemy();
		if (INVALID_ID == m_CurEnemyID) {
			ToGoHome();
		}
		return FALSE;
	}
	Obj_Character* pCurEnemy = (Obj_Character*)pObj;

    if (!pCurEnemy)
	{
		Assert(NULL && "AI_Monster::ToApproachTar...pCurEnemy = NULL...");
		return FALSE;
	}
	FLOAT fMaxRange = 0.0f;
	/** 现在尝试能否使用技能 */
	if (TRUE == ExcuteAIScript(SKILLSECTION))
	{ 
		ToAttack();
		return TRUE;
	}
	else
	{/** 根据技能攻击距离确定要到达的目的地 */
		SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(m_SkillID);
		if (!pSkillTemplate)
		{
			Assert(NULL && "AI_Monster::ToApproachTar...pSkillTemplate=NULL...");
			return FALSE;
		}
		/** 这部分以后由技能模块来支持 */
		fMaxRange = pSkillTemplate->GetOptimalRangeMax();
	}
	WORLD_POS Tar ;
	BOOL bFindPos = FALSE ;

	FLOAT vX = pMonster->getWorldPos()->m_fX - pCurEnemy->getWorldPos()->m_fX;
	FLOAT vZ = pMonster->getWorldPos()->m_fZ - pCurEnemy->getWorldPos()->m_fZ;
	FLOAT fDist = MySqrt(pMonster->getWorldPos(), pCurEnemy->getWorldPos());
	if (fDist < ZERO_VALUE)
	{
		return FALSE;
	}
	FLOAT fCos = vX / fDist;
	FLOAT fSin = vZ / fDist;
	if (fDist < fMaxRange)
	{
		Tar.m_fX = pCurEnemy->getWorldPos()->m_fX + fCos*fDist;
		Tar.m_fZ = pCurEnemy->getWorldPos()->m_fZ + fSin*fDist;
	}
	else
	{
		Tar.m_fX = pCurEnemy->getWorldPos()->m_fX + fCos*fMaxRange;
		Tar.m_fZ = pCurEnemy->getWorldPos()->m_fZ + fSin*fMaxRange;
	}
	
	fDist = MySqrt(&Tar, pCurEnemy->getWorldPos());

	ORESULT oResult = Move( &Tar );
	if (OR_OK != oResult)
	{
		pMonster->DirectMoveTo(pCurEnemy->getWorldPos());
	}
	
	ChangeState(ESTATE_APPROACH);

	return TRUE ;
	__LEAVE_FUNCTION
	return FALSE ;
}

WORLD_POS 	AI_Monster::GetPosOfMemeberGo(const WORLD_POS& rTargetPos, INT index, FLOAT fDist)
{
	WORLD_POS Pos;

__ENTER_FUNCTION
	Obj_Monster* pMonster = GetCharacter();
	if (!pMonster)
	{
		Assert(NULL && "AI_Monster::GetPosOfMemeberGo...pMonster=NULL...");
		return Pos;
	}

	float radian;
	switch (index)
	{
	case 0:
		radian = __HALF_PI + __QUARTER_PI;
		break;
	case 1:
		radian = __QUARTER_PI;
		break;
	case 2:
		radian = 0.f;
		break;
	case 3:
		radian = -__QUARTER_PI;
		break;
	case 4:
		radian = -(__HALF_PI + __QUARTER_PI);
		break;
	case 5:
		radian = -__PI;
		break;
	default:
		break;
	}

	radian += pMonster->getDir();
	Pos.m_fX = rTargetPos.m_fX + fDist * SinCos::Sin(radian);
	Pos.m_fZ = rTargetPos.m_fZ + fDist * SinCos::Cos(radian);

	return Pos;

__LEAVE_FUNCTION
	return Pos;
}

VOID AI_Monster::MemeberGo(const WORLD_POS& rTargetPos, FLOAT fDist/* = 2.0f*/)
{
__ENTER_FUNCTION
	GET_MONSTER_SCENE((VOID)0)

	for (INT i = 0; i < MAX_TEAMMATE_COUNT; ++i)
	{
		Obj* pObj = pScene->GetObjManager()->GetObj(m_TeammateID[i]);
		if (pObj && pObj->GetObjType() == Obj::OBJ_TYPE_MONSTER)
		{
			const WORLD_POS* pPos = &GetPosOfMemeberGo(rTargetPos, i, fDist);
			AI_Character* pAI = ((Obj_Monster*)pObj)->GetAIObj();
			if (pAI) 
			{
				ORESULT oResult = pAI->Move(pPos);
				if (OR_OK != oResult && OR_LIMIT_MOVE != oResult)
				{
					((Obj_Character*)pObj)->DirectMoveTo(pPos);
				}
			}
			pAI->ChangeState(ESTATE_TEAMFOLLOW);
		}
	}
    
__LEAVE_FUNCTION
}

ORESULT AI_Monster::MonsterGo(const WORLD_POS* pPos)
{
__ENTER_FUNCTION
	GET_MONSTER_SCENE(OR_ERROR)

	//AxTrace(0, 0, "objid=%d, x=%f, z=%f",
	//	pMonster->GetID(), pPos->m_fX, pPos->m_fZ);

	ORESULT oResult = Move(pPos);
	MemeberGo(*pPos);
	if (OR_OK != oResult && OR_LIMIT_MOVE != oResult)
	{
		GetCharacter()->DirectMoveTo(pPos);
	}
	
	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR;
}

VOID	AI_Monster::MovePhonily(const WORLD_POS& rTar)
{
	GET_MONSTER_SCENE((VOID)0)

	pMonster->setWorldPos(&rTar);
	pMonster->AddLogicCount();
	// send msg to client...
	GCCharMove Msg;
	Msg.setObjID(pMonster->GetID());
	Msg.setStartTime(g_pTimeManager->CurrentTime());
	Msg.setHandleID(pMonster->GetLogicCount());
	Msg.setTargetPos(rTar);
	pScene->BroadCast(&Msg, pMonster, TRUE);
}
//=======================================================================================
// AIScript相关 
//=======================================================================================
FLOAT AI_Monster::GetNeedGoDist(ObjID_t SkillID)
{
	__ENTER_FUNCTION
	GET_MONSTER_SCENE(-1.0f)

	Obj* pObj = pScene->GetObjManager()->GetObj(m_CurEnemyID);
	if (!pObj || !IsCharacterObj(pObj->GetObjType()) )
	{
		return -1.0f;
	}
	Obj_Character* pCurEnemy = (Obj_Character*)pObj;

    SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(SkillID);
	if(NULL != pSkillTemplate)
	{/** 这部分以后由技能模块来支持 */
		FLOAT fDist = MySqrt(GetCharacter()->getWorldPos(),pCurEnemy->getWorldPos());
		if(fDist > pSkillTemplate->GetOptimalRangeMax() )
			return fDist - pSkillTemplate->GetOptimalRangeMax();
		else if(fDist < pSkillTemplate->GetOptimalRangeMin())
			return fDist - pSkillTemplate->GetOptimalRangeMin();
	}
	return -1.0f;
	__LEAVE_FUNCTION
	return -1.0f;
}

VOID AI_Monster::GetScriptFile(INT ScriptID)
{
	__ENTER_FUNCTION

	SFileData * pSFileData = g_AIFileDataMgr.GetFileData(ScriptID);
	if (!pSFileData)
	{
		m_AIScriptID = -1;
		return ;
	}
	else
	{
		m_AIScriptID = ScriptID;
	}

	if(pSFileData)
	{// 只是证明该脚本的存在性
		/////////////////////////////////////////////////////////////////
		memset((VOID*)m_aAIScriptTimes, 0, sizeof(INT)*AISCRIPT_NUM);
		if (m_AIScriptID >= 0 && m_AIScriptID < AISCRIPT_NUM) {
			g_pAIScriptList[m_AIScriptID]->ResetStateList(this);
		}
	}
	__LEAVE_FUNCTION    
}

BOOL	AI_Monster::ExcuteAIScript(INT nState)
{
__ENTER_FUNCTION
	if (m_AIScriptID >= 0 && m_AIScriptID < AISCRIPT_NUM) {
		return g_pAIScriptList[m_AIScriptID]->ProcessScript(nState, this);
	}
	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}
//=======================================================================================
// 辅助方法 
//=======================================================================================

INT AI_Monster::AIParam( UINT AIParam )
{
	__ENTER_FUNCTION
	Assert( GetCharacter() ) ;
	return g_MonsterAITbl.Get( GetCharacter()->GetAIType(), AIParam ) ;
	__LEAVE_FUNCTION
		return 0 ;
}

BOOL AI_Monster::IsToGoHome(FLOAT& fMTDist,FLOAT& fETDist)
{
	__ENTER_FUNCTION
	Obj_Monster* pMonster =	GetCharacter();
	if (!pMonster)
	{
		Assert(NULL && "AI_Monster::IsToGoHome...pMonster = NULL...");
		return FALSE;
	}
	Obj* pObj = pMonster->getScene()->GetObjManager()->GetObj(GetPrimaryEnemyID()) ;
	if( pObj==NULL || !IsCharacterObj( pObj->GetObjType() ) )
	{/** 转向下一个敌人，直到全部找完为止 */
		DelEnemy(m_CurEnemyID);
		m_CurEnemyID = GetNextEnemy();
		if (INVALID_ID == m_CurEnemyID)
		{
			ToGoHome();
			return TRUE ;
		}
	}
	/** 取得敌人的Character指针 */
	Obj_Character* pCurEnemy = (Obj_Character*)pObj ;
	/** 敌人当前位置与当前移动目标位置的距离，即FinalTarPosition */
	fETDist = MySqrt(pCurEnemy->getWorldPos(), pMonster->GetFinalTarPos());
	/** 自己与目标点的距离 */
	fMTDist = MySqrt(pMonster->getWorldPos(), pMonster->GetFinalTarPos());
	/** 当前位置和敌人位置之间的距离 */
	FLOAT fDist  = MySqrt(pMonster->getWorldPos(), pCurEnemy->getWorldPos()) ;
	/** 自己位置和被攻击或发现敌人的位置的距离 */
	FLOAT fRDist = MySqrt(pMonster->getWorldPos(), &m_ToAttackPos) ;
	if(	/** 不在同一个场景 */
		pCurEnemy->getScene()->SceneID()!=pMonster->getScene()->SceneID()
		/**  敌人死了 */
		|| !pCurEnemy->IsAlive()	
		/** 离开战斗开始的位置太远了，该回家了 */
		|| fRDist>AIParam(AIPARAM_RETURN)/1000.0f )
	{/** 转向下一个敌人，直到全部找完为止 */
		DelEnemy(m_CurEnemyID);
		m_CurEnemyID = GetNextEnemy();
		if (INVALID_ID == m_CurEnemyID)
		{
			ToGoHome();
			return TRUE;
		}
	}

	return FALSE ;

	__LEAVE_FUNCTION
		return FALSE ;
}

VOID AI_Monster::Update_RefusePaoPaoTime()
{
	Obj_Monster* pMonster = GetCharacter();
	if (!pMonster) {
		return ;
	}
	if (m_nRefusePaoPaoTime > 0) {
		m_nRefusePaoPaoTime -= GetCharacter()->GetLogicTime();
	}
}

VOID AI_Monster::RecoverHP()
{
__ENTER_FUNCTION

	/** 进行快速回血*/
	INT nCurHP = GetCharacter()->GetHP( ) ; 
	INT nMaxHP = GetCharacter()->GetMaxHP() ;
	if( nCurHP < nMaxHP )
	{
		nCurHP += (INT)(GetSpeedyOfRecover()*nMaxHP);
		if( nCurHP > nMaxHP )
			nCurHP = nMaxHP ;

		GetCharacter()->SetHP( nCurHP ) ;
	}
__LEAVE_FUNCTION
}

VOID AI_Monster::ExcuteCommand()
{
__ENTER_FUNCTION
	switch(m_SAICommand.commandType) {
	case E_COMMAND_TYPE_TOFLEE:
		{
			ToFlee(m_SAICommand.param[0], 
				(FLOAT)m_SAICommand.param[1], 
				(FLOAT)m_SAICommand.param[2]);
		}
		break;
	case E_COMMAND_TYPE_TOATTACK:
		break;
	case E_COMMAND_TYPE_TOIDLE:
		break;
	default:
		break;
	}
	
	m_SAICommand.Cleanup();
__LEAVE_FUNCTION
}

BOOL AI_Monster::GetBestPos(WORLD_POS& Tar)
{
	__ENTER_FUNCTION
	GET_MONSTER_SCENE(FALSE);

	Obj* pObj = pScene->GetObjManager()->GetObj(m_CurEnemyID);
	if (!pObj || !IsCharacterObj(pObj->GetObjType()) )
	{
		return false;
	}
	Obj_Character* pCurEnemy = (Obj_Character*)pObj;

	if (TRUE == pMonster->IsLimitMove() )
	{
		return FALSE;
	}
	FLOAT fMaxRange = 0.f;
	FLOAT fMinRange = 0.f;
	SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(m_SkillID);
	if(NULL != pSkillTemplate)
	{/** 这部分以后由技能模块来支持 */
		fMaxRange = pSkillTemplate->GetOptimalRangeMax();
		fMinRange = pSkillTemplate->GetOptimalRangeMin();
		fMinRange = fMinRange < fMaxRange*0.8f ? fMaxRange*0.8f : fMinRange;
	}

	/** 目标有可能是Human */
	if ( pCurEnemy->GetObjType() == Obj::OBJ_TYPE_HUMAN)
	{
		AvoidOverlap* pAvoidOverLap = ((Obj_Human*)pCurEnemy)->GetAvoidOverLap();
		if (pAvoidOverLap)
		{
			pAvoidOverLap->GetDirPos(Tar, *(pMonster->getWorldPos()), *pCurEnemy->getWorldPos(), fMaxRange, fMinRange);			
			if (Tar == *(pMonster->getWorldPos()) )
			{
				return FALSE;
			}
		}
	}
	else
	{
		FLOAT vX =  pCurEnemy->getWorldPos()->m_fX - pMonster->getWorldPos()->m_fX;
		FLOAT vZ =  pCurEnemy->getWorldPos()->m_fZ - pMonster->getWorldPos()->m_fZ;
		FLOAT fDist = MySqrt(pMonster->getWorldPos(), pCurEnemy->getWorldPos());
		if (fDist < ZERO_VALUE)
		{
			return FALSE;
		}
		FLOAT fCos = vX / fDist;
		FLOAT fSin = vZ / fDist;
		Tar.m_fX = pMonster->getWorldPos()->m_fX + fCos*(fDist - fMinRange);
		Tar.m_fZ = pMonster->getWorldPos()->m_fZ + fSin*(fDist - fMinRange);

	}

	pScene->GetMap()->VerifyPos(&Tar);
	if (pScene->GetMap()->IsCanGo(Tar,0))
	{
		return TRUE;
	}

	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID	AI_Monster::ProcessPaoPao(VOID)
{
__ENTER_FUNCTION
	Obj_Monster* pMonster = GetCharacter();
	if (!pMonster) {
		return ;
	}
	Scene* pScene = pMonster->getScene();
	if (!pScene) {
		return ;
	}
	// 如果正处于繁忙状态则直接返回
	if (m_nRefusePaoPaoTime > 0)
	{
		return ;
	}

	m_nRefusePaoPaoTime = REFUSEPAOPAO_TIME;
	// 从泡泡索引表中找出一个泡泡ID
	INT nDataID = pMonster->GetDataID();
	Assert(nDataID >= 0 && nDataID < MAXTYPE_NUMBER);

	INT nBasePaoPaoID = g_NPCPaoPaoIndexTbl[nDataID].m_nBaseIndex;
	INT nPaoPaoStep = g_NPCPaoPaoIndexTbl[nDataID].m_nIndexStep;
	if (nBasePaoPaoID < 0 || nPaoPaoStep < 0) {
		return ;
	}

	pMonster->PaoPao(nBasePaoPaoID, nPaoPaoStep);

__LEAVE_FUNCTION
}

VOID AI_Monster::SetSpeedyOfRecover(FLOAT fSpeedyOfRecover/* = 1.0f*/)
{
	fSpeedyOfRecover < 0.f ? fSpeedyOfRecover = 0.f : NULL;
	fSpeedyOfRecover > 1.f ? fSpeedyOfRecover = 1.f : NULL;
	m_fSpeedyOfRecover = fSpeedyOfRecover; 
}

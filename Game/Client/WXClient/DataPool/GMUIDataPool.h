/****************************************\
*										*
* 			   UI数据池					*
*										*
\****************************************/
#pragma once

#include "TDUIDataPool.h"
#include "..\GameCommand.h"
#include "DetailAttrib_Player.h"
#include "..\Interface\GMGameInterface_Script.h"
#include "TeamOrGroup.h"							// 组队相关. 
#include "GCStudyXinfa.h"
#include "GameStruct_Skill.h"


struct ScriptParam_EventList;
struct ScriptParam_MissionInfo;
struct ScriptParam_MissionDemandInfo;
struct ScriptParam_MissionContinueInfo;
struct ScriptParam_MissionTips;
struct SSkillItem;
struct ScriptParam_SkillStudy;
struct SMissionBonus;
struct SMissionBonusItem;

#define MAX_INVITE_COUNT	5
struct MEMBERINVITEINFO
{
	GUID_t	SourceId;
	GUID_t	DestinId;
	BOOL	bHave;

	MEMBERINVITEINFO()
	{
		bHave = FALSE;
	}
};

class CAI_Base;

class CUIDataPool : public tDataPool
{
public:
	virtual VOID		Initial( VOID* );
	virtual VOID		Release(VOID);
	virtual VOID		Tick( VOID );

public:
	CUIDataPool( VOID );
	virtual ~CUIDataPool( VOID );

public:
	virtual RC_RESULT OnCommand_( const SCommand_DPC *pCmd );

public:
	const CDetailAttrib_Player *GetMySelfDetailAttrib( VOID )const
	{
		return m_pPlayerData;
	}

protected:
	//virtual VOID OnMySelfIDChanged( VOID );
	virtual VOID OnEventListResponse( VOID );
	virtual VOID OnMissionInfoResponse( VOID );
	virtual VOID OnMissionRegie( VOID );
	virtual VOID OnMissionDemandInfoResponse( VOID );
	virtual VOID OnMissionContinueInfoResponse( VOID );
	virtual VOID OnMissionTips( VOID );
	virtual	VOID OnSkillStudy( VOID);
public:
	BOOL ConvertServerString( std::string& strConvert );

protected:
	// Player attrib
	CDetailAttrib_Player	*m_pPlayerData;

	//第一次和npc对话，得到npc所能激活的操作
	ScriptParam_EventList				*m_pEventList;
	//在接任务时，看到的任务信息
	ScriptParam_MissionInfo				*m_pMissionInfo;
	//接受任务后，再次和npc对话，所得到的任务需求信息
	ScriptParam_MissionDemandInfo		*m_pMissionDemandInfo;
	//完成任务后，点击继续，出现的任务信息
	ScriptParam_MissionContinueInfo		*m_pMissionContinueInfo;
	//完成任务过程中，阶段性提示信息
	ScriptParam_MissionTips				*m_pMissionTips;
	//技能学习
	ScriptParam_SkillStudy				*m_pSkillStudy;

public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 复活相关的信息
	//
	// 这里只是提供一个复活的倒计时间功能，因没有相对应的控件支持，所以只能临时用这种方法实现
	//

	// 开始灵魂出窍时间的计时
	VOID BeginOutGhostTimer( INT nOutGhostTimer );

	// 停止灵魂出窍时间的计时
	VOID EndOutGhostTimer( VOID );

protected:
	BOOL		m_bOutGhostTimerWorking;	// 灵魂出窍的剩余时间计数是否为工作状态
	INT			m_nOutGhostTimer;			// 灵魂出窍的剩余时间

	// 当前对话NpcId
	INT			m_nCurDialogNpcId;
	// 当前商人的Id
	INT			m_nCurShopNpcId;

public:
	// 当前对话NpcId
	INT			GetCurDialogNpcId(){return m_nCurDialogNpcId;}
	VOID		SetCurDialogNpcId(INT nCurDialogNpcId){ m_nCurDialogNpcId = nCurDialogNpcId; }

	// 当前商人的Id
	INT			GetCurShopNpcId(){return m_nCurShopNpcId;}
	VOID		SetCurShopNpcId(INT nCurShopNpcId){ m_nCurShopNpcId = nCurShopNpcId; }

	//----------------------------------------------------------------------------------------------------------------------------
	// 组队相关的操作
	//
	// 由于队伍信息只与当前玩家有关系, 所以在游戏客户端只存在一份, 应该写在数据池中.

	// 当前显示的队伍的类型.
	INT m_iCurTeamType;

	// 设置当前队伍的类型.
	void SetCurTeamType(INT iTeamType);

	// 得到当前的队伍的类型.
	INT  GetCurTeamType();


	//----------------------------------------------------------------------------------------------------------------------------
	// 打开队长界面

	// 打开队长界面, 选择的人物.
	INT m_iSelectedTeamMember_Leader;

	// 选中的队友索引.
	INT m_iSelectedTeamMember;

	// 选中的申请者的索引
	INT m_iSelectApply_Apply;


	//---------------------------------------------
	//  邀请, 申请, 踢人使用.

	// 队伍邀请者id
	GUID_t m_IdSource;

	// 被邀请者id
	GUID_t m_IdDestination;


	// 队员要求其他队员的数据结构2006－4－28
	MEMBERINVITEINFO	m_MemberInviteInfo[MAX_INVITE_COUNT];

	// 设置队员邀请者和被邀请者guid2006－4－28
	INT SetMemberInviteGUID(GUID_t SId, GUID_t DId);

	//	得到队员邀请者和被邀请者guid， 并删除2006－4－28
	BOOL GetMemberInviteGUID(int iPos, GUID_t& SId, GUID_t& DId, BOOL bDel = TRUE);
	
	// 队伍|团队信息
	CTeamOrGroup	m_TeamOrGroup;

	// 设置队伍邀请者的Id
	VOID SetTeamInvitorGUID(GUID_t Id) ;

	// 得到队伍邀请者的id
	GUID_t GetTeamInvitorGUID();

	// 得到队伍邀请者的id
	GUID_t GetTeamInvitorGUID(int iTeamIndex);

	// 设置SOURCE_Id
	VOID SetSourceGUID(GUID_t Id) ;

	// 得到SOURCE_Id
	GUID_t GetSourceGUID();

	// 得到申请者SOURCE_Id
	GUID_t GetApplySourceGUID(int iApplyIndex);


	// 设置DESTINATION_Id
	VOID SetDestinationGUID(GUID_t Id) ;

	// 得到DESTINATION_Id
	GUID_t GetDestinationGUID();



	// 得到队伍信息
	CTeamOrGroup* GetTeamOrGroup( ){ return &m_TeamOrGroup; }

	// 清除队伍信息
	VOID ClearTeamInfo(VOID);

	// 离开队伍
	VOID LeaveTeam();

	// 解散队伍
	VOID DismissTeam();	

	// 踢人信息
	VOID KickTeamMember();

	// 踢人信息
	VOID KickTeamMember(int iIndex);

	// 提升队长
	VOID AppointLeader();

	// 提升队长
	VOID AppointLeader(int iIndex);

	// 某个队员是否是队长.
	BOOL IsTeamLeader(INT iIndex);		

	// 自己是否是队长.
	BOOL IsTeamLeader_Me();

	// 当前是否在一个队伍中
	BOOL IsInTeam();

	// 队员与自己是否在同一个场景中
	BOOL IsInScene(INT iIndex);

	// 判断一个玩家与自己是否在同一个队伍中。
	BOOL IsTeamMember(GUID_t Id);


	// 通过guid得到队友名字.2006-4-12
	LPCTSTR GetTeamNameByGUID(GUID_t Id);

	// 通过guid得到队友HP.2006-4-12
	int GetTeamHPByGUID(GUID_t Id);

	// 通过guid得到队友MP.2006-4-12
	int GetTeamMPByGUID(GUID_t Id);

	// 通过guid得到队友Range.2006-4-12
	int GetTeamRangeByGUID(GUID_t Id);

	// 通过guid得到队友Level.2006-4-12
	int GetTeamLevelByGUID(GUID_t Id);

	// 通过guid得到队友头像.2006-4-12
	LPCTSTR GetTeamIconByGUID(GUID_t Id);

	// 通过guid得到队友HP.2006-4-12
	float GetTeamHPPercentByGUID(GUID_t Id);

	// 通过guid得到队友MP.2006-4-12
	float GetTeamMPPercentByGUID(GUID_t Id);
	//技能学习
	int GetStudySkill_Num();
	int GetStudySkill_Id(GUID_t iIndex);
	int GetStudySkill_Level(GUID_t iIndex);
	int GetStudySkill_Type(GUID_t iIndex);
	int GetStudySkill_NPCID();





	//-----------------------------------------------------------------------------------
	// 邀请队伍操作

	// 当前拒绝队伍的索引.
	INT m_iCurRejectTeamIndex;
	
	// 同意加入一个队伍后, 清空所有的邀请队伍.
	VOID ClearAllInviteTeam();	


	// 拒绝当前拒绝的队伍.
	VOID EraseCurRejectTeam();

	//
	// 组队相关的操作
	//---------------------------------------------------------------------------------------------------------------------------------


public:



public:
	VOID			 			SendSelectEvent(INT nIndex,INT nExIndex1,INT nExIndex2);
	VOID						SendAcceptEvent();
	VOID						SendRefuseEvent();
	VOID						SendContinueEvent();
	VOID						SendCompleteEvent(INT nIndex);
	VOID						SendAbandonEvent(INT nIndex);
	VOID						SendMissionItemInfo(INT nPetIndex);


	//------------------------------------------------------
	//任务奖励物品
	struct QuestRewardItem
	{
		SMissionBonus*			pItemData;
		tObject_Item*			pItemImpl;
		BOOL					bSelected;
	};

	//------------------------------------------------------
	//任务需要物品
	struct QuestDemandItem
	{
		SMissionBonusItem*		pDemandItem;
		tObject_Item*			pItemImpl;
	};

	VOID						QuestReward_Clear(VOID);
	VOID						QuestDemand_Clear(VOID);
	VOID						QuestReward_AddItem(const QuestRewardItem& itemReward);
	VOID						QuestDemand_AddItem(const QuestDemandItem& itemDemand);

public:
	static CUIDataPool*	GetMe(VOID) { return s_pMe; }

protected:
	static CUIDataPool*	s_pMe;

protected:

	std::vector< QuestRewardItem >	m_vecQuestRewardItem;

	//任务需求物品
	std::vector< QuestDemandItem >  m_vecQuestDemandItem;

	WX_DECLARE_DYNAMIC( CUIDataPool );
	friend class SCRIPT_SANDBOX::DataPool;

};
#pragma once

#include "Type.h"
#include "TDBasicType.h"
#include "GameStruct.h"
#include "GameStruct_Team.h"

//--------------------------------------------------------------------------------------------------------------------------
//
// 在界面上显示的模型.
//

class CObject_PlayerOther;
class CModelShowInUI
{
public:

	//-----------------------------------------------------------------------------------------------------------------------
	//
	// 用于在ui界面上显示的信息.
	//
	static UINT				m_TeamNumCount;					// 模型计数
	CObject_PlayerOther*	m_pAvatar;						// 用于UI显示的逻辑对象.
	TCHAR					m_szBufModel[128];				// 模型名字.

public:

	CModelShowInUI();
	~CModelShowInUI();

public:

	// 创建新的模型
	BOOL CreateNewUIModel();

	// 删除ui模型
	BOOL DestroyUIModel();

	// 设置模型信息
	VOID SetUIModelInfo(HUMAN_EQUIP point, INT nID);

	// 得到ui模型名字
	const TCHAR* GetUIModelName();

	// 脸部模型
	VOID SetFaceMeshId(INT nID);

	// 头发模型
	VOID SetFaceHairId(INT nID);

	// 头发颜色
	VOID SetHairColor(INT nID);

	
};




// 客户端队伍|团队数据结构
struct TeamMemberInfo
{
	GUID_t					m_GUID;
	ObjID_t					m_OjbID;						//
	SceneID_t				m_SceneID;
	CHAR					m_szNick[MAX_CHARACTER_NAME];	// 1.昵称
	UINT					m_uFamily;						// 2.门派
	USHORT					m_uDataID;						// 3.性别
	UINT					m_uLevel;						// 4.等级
	INT						m_nPortrait;					// 5.头像
	WORLD_POS				m_WorldPos;						// 6.位置（坐标）
	INT						m_nHP;							// 7.HP
	UINT					m_dwMaxHP;						// 8.HP上限
	INT						m_nMP;							// 9.MP
	UINT					m_dwMaxMP;						// 10.MP上限
	INT						m_nAnger;						// 11.怒气
	UINT					m_WeaponID;						// 12.武器
	UINT					m_CapID;						// 13.帽子
	UINT					m_ArmourID;						// 14.衣服
	UINT					m_CuffID;						// 15.护腕
	UINT					m_FootID;						// 16.靴子
	// 17.buff，暂时不考虑
	SimpleImpactList		m_SimpleImpactList;				// Buff 列表
	BOOL					m_bDeadLink;					// 18.断线
	BOOL					m_bDead;						// 19.死亡
	UINT					m_uFaceMeshID;					// 20.面部模型
	UINT					m_uHairMeshID;					// 21.头发模型
	UINT					m_uHairColor;					// 22.头发颜色

	CModelShowInUI			m_UIModel;						// 在界面显示的模型
	TeamMemberInfo()
	{
		memset(m_szNick, 0, sizeof(m_szNick));
		m_GUID		= 0;
		m_OjbID		= 0;						//
		m_SceneID	= 0;
		m_uFamily	= 9;						// 2.门派
		m_uDataID   = 0;						// 3.性别
		m_uLevel    = 0;						// 4.等级
		m_nPortrait	= -1;						// 5.头像
		m_nHP		= 0;						// 7.HP
		m_dwMaxHP	= 0;						// 8.HP上限
		m_nMP		= 0;						// 9.MP
		m_dwMaxMP	= 0;						// 10.MP上限
		m_nAnger	= 0;						// 11.怒气
		m_WeaponID	= 0;						// 12.武器
		m_CapID		= 0;						// 13.帽子
		m_ArmourID	= 0;						// 14.衣服
		m_CuffID	= 0;						// 15.护腕
		m_FootID	= 0;						// 16.靴子
		m_bDeadLink = FALSE;					// 18.断线
		m_bDead		= FALSE;					// 19.死亡
		m_uFaceMeshID = 0;						// 20.面部模型
		m_uHairMeshID = 0;						// 21.头发模型
		m_uHairColor = 0;						// 22.头发颜色
		// 创建新的模型
		//m_UIModel.CreateNewUIModel();
	}


};



typedef std::vector< TeamMemberInfo* >		CTeamMembers;

struct TeamInfo
{
	TeamID_t				m_TeamID;			// 1 or 2, 3, ...
	CTeamMembers			m_TeamMembers;		// [MAX_TEAM_MEMBER]
};

typedef std::vector< TeamInfo* >			CTeamInfo;

struct TeamCacheInfo // 这个东西是为了面板显示申请人或者邀请队伍的信息所设立
{
	
	GUID_t					m_GUID;
	CHAR					m_szNick[MAX_CHARACTER_NAME];	// 1.昵称
	UINT					m_uFamily;						// 2.门派
	SceneID_t				m_Scene;						// 3.场景
	UINT					m_uLevel;						// 4.等级
	UINT					m_uDetailFlag;					// 以下信息是否显示
	USHORT					m_uDataID;						// 5.性别
	UINT					m_WeaponID;						// 7.武器
	UINT					m_CapID;						// 8.帽子
	UINT					m_ArmourID;						// 9.衣服
	UINT					m_CuffID;						// 10.护腕
	UINT					m_FootID;						// 11.靴子
	UINT					m_uFaceMeshID;					// 12.面部模型
	UINT					m_uHairMeshID;					// 13.头发模型
	UINT					m_uHairColor;					// 14.头发颜色

	CModelShowInUI			m_UIModel;						// 在界面显示的模型
	TeamCacheInfo()
	{
		//memset((VOID*)this, 0, sizeof(TeamMemberInfo));
		memset(m_szNick, 0, sizeof(m_szNick));
		m_GUID			= 0;
		m_uFamily		= 0;
		m_Scene			= 0;						// 3.场景
		m_uLevel		= 0;						// 4.等级
		m_uDetailFlag	= 0;					// 以下信息是否显示
		m_uDataID		= 0;						// 5.性别
		m_WeaponID		= 0;						// 7.武器
		m_CapID			= 0;						// 8.帽子
		m_ArmourID		= 0;						// 9.衣服
		m_CuffID		= 0;						// 10.护腕
		m_FootID		= 0;						// 11.靴子
		m_uFaceMeshID	= 0;						// 12.面部模型
		m_uHairMeshID	= 0;						// 13.头发模型
		m_uHairColor	= 0;						// 14.头发颜色

		// 创建新的模型
		//m_UIModel.CreateNewUIModel();
	}	
	
	

	
};

typedef std::deque< TeamCacheInfo* >		CProposers;

struct InviteTeam
{
	GUID_t					m_InviterGUID;
	CProposers				m_InvitersInfo;
};

typedef std::deque< InviteTeam* >		CInviteTeams;

class CTeamOrGroup
{ // when a team change to a group, the member of m_TeamMembers will be assigned to m_TeamInfo[0].
public:
	enum TEAM_OR_GROUP_TYPE
	{
		TEAM_TYPE				= 1,
		GROUP_TYPE				= 2,
	};

	enum UI_ON_OPEN
	{
		UI_ALL_CLOSE			= 0,
		UI_INVITE_ON_OPEN		= 1,
		UI_APPLY_ON_OPEN		= 2,
	};

	enum
	{
		MAX_INVITE_TEAM			= 7,
		MAX_PROPOSER			= 18,
	};

public:
	CTeamOrGroup()
	{
		CleanUp();
	}
	~CTeamOrGroup();

	VOID					CleanUp();

	BOOL					HasTeam()
	{
		return (m_Type != (TEAM_OR_GROUP_TYPE)0);
	}

	// 组建队伍，设置队伍的 leader，并且加入为第一个成员
	VOID					CreateTeam(TeamMemberInfo* leader, TeamID_t TeamID);

	// 解散队伍
	VOID					DismissTeam();

	// 增加一个组员，如果是团队的话，需要传入团队所在的小组号
	VOID					AddMember(TeamMemberInfo* member, TeamID_t TeamID, BYTE TeamIndex = -1);

	// 一个组员离开
	VOID					DelMember(GUID_t guid);

	// 更换队员位置
	VOID					ExchangeMemberPosition(GUID_t guid1, GUID_t guid2, BYTE TeamIndex1 = -1, BYTE TeamIndex2 = -1);

	// 更新队员信息（暂时没有用到）
	VOID					UpdateMemberInfo(TeamMemberInfo* member, GUID_t guid);

	// 队长
	GUID_t					GetLeaderGUID() { return m_LeaderID; }

	// 设置队长 GUID
	VOID					SetLeaderGUID(GUID_t guid);

	// 任命新队长
	VOID					Appoint(GUID_t guid);
	// 得到队员的数量
	INT						GetTeamMemberCount(BYTE TeamIndex = -1);

	// 得到某个队友显示在队友界面里面的具体位置 1,2,3...
	INT						GetMemberUIIndex(GUID_t guid, BYTE TeamIndex = -1);

	// 将界面的索引转换成队伍数组中的索引
	const TeamMemberInfo*	GetMemberByUIIndex(INT UIIndex, BYTE TeamIndex = -1);

	// 通过界面索引, 得到server id
	INT						GetMemberServerIdByUIIndex(INT UIIndex, BYTE TeamIndex = -1);

	// 通过界面索引得到选中队员的guid
	GUID_t					GetMemberGUIDByUIIndex(INT UIIndex, BYTE TeamIndex = -1);

	// 得到自己的索引
	INT						GetSelfIndex(BYTE TeamIndex = -1);

	// 根据某个 guid 找到具体队友
	TeamMemberInfo*			GetMember(GUID_t guid);

	// 根据server id 找到具体队友
	TeamMemberInfo*			GetMemberByServerId(INT iServerId);

	// 得到队伍中第 idx 个队员
	const TeamMemberInfo*	GetMemberByIndex(UINT idx, BYTE TeamIndex = -1);

	// 改变申请、邀请界面的打开状态
	VOID					SetUIFlag(UI_ON_OPEN flag);

	// 取得申请、邀请界面的打开状态
	UI_ON_OPEN				GetUIFlag() { return m_UIFlag; }

	// 增加一个邀请队伍，TRUE 成功，反之失败
	BOOL					AddInviteTeam(InviteTeam* pTeam);

	// 得到当前邀请队伍的数量
	INT						GetInviteTeamCount() { return (INT)m_InviteTeams.size(); }

	// 根据索引得到某个队伍
	const InviteTeam*		GetInviteTeamByIndex(INT idx);

	// 清除某个邀请队伍
	VOID					EraseInviteTeam(INT idx);

	// 清除邀请队列
	VOID					ClearInviteTeam();

	// 增加一个申请者
	BOOL					AddProposer(TeamCacheInfo* pProposer);

	// 清除一个申请者
	VOID					EraseProposer(INT idx);

	// 清除申请队列
	VOID					ClearProposer();

	// 得到当前申请队列的数量
	INT						GetProposerCount() { return (INT)m_Proposers.size(); }

	// 根据索引得到某个申请人
	const TeamCacheInfo*	GetProposerByIndex(INT idx);

	// 填充队伍中当前玩家的详细信息
	VOID					FillMyInfo();

	// 设置模型信息
	VOID					SetModelLook();

	// 是否在同一个场景中.
	BOOL					IsInScene(INT iIndex);

	// 更新 Buff 列表
	VOID					UpdateImpactsList( INT ObjID, const SimpleImpactList* pSimpleImpactList );

	// 增加 Buff
	VOID					AddImpact( INT ObjID, ImpactID_t ImpactID );

	// 减少 Buff
	VOID					RemoveImpact( INT ObjID, ImpactID_t ImpactID );




private:

	// 得到自己的队伍信息（从自己的数据池获取）
	VOID					FillMyInfo(TeamMemberInfo* member);

	// 根据小组号 N 得到第 N 个小组
	TeamInfo*				GetTeam(BYTE TeamIndex) const;

private:
	TEAM_OR_GROUP_TYPE		m_Type;				// team or group
	TeamID_t				m_ID;				// the serial number of the team or group in the game world
	// it can be used to identify the empty team or group
	BYTE					m_MyTeamID;			// the team ID of mine in my group
	GUID_t					m_LeaderID;			// guid of the team leader
	CTeamMembers			m_TeamMembers;		// [MAX_TEAM_MEMBER]
	CTeamInfo				m_TeamInfo;			// [MAX_TEAMS_IN_GROUP]

	// 申请邀请信息
	UI_ON_OPEN				m_UIFlag;			// 用于判断邀请人界面或者申请人界面是否已经打开
	CProposers				m_Proposers;		// 申请人列表，队长可见
	CInviteTeams			m_InviteTeams;		// 邀请队伍信息列表，被邀请人可见
};

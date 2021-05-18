/********************************************************************************
 *	文件名：	Team.h
 *	全路径：	d:\Prj\Server\Server\Team\Team.h
 *	创建时间：	2005 年 11 月 4 日	9:25
 *
 *	功能说明：	队伍
 *	修改记录：
*********************************************************************************/

#ifndef __TEAM_H__
#define __TEAM_H__

#include "Type.h"

//组队成员

struct TEAMMEMBER
{
	GUID_t							m_GUID;
	ObjID_t							m_ObjID;
	SceneID_t						m_SceneID;

	TEAMMEMBER( )
	{
		CleanUp( );
	}

	VOID							CleanUp( )
	{
		m_GUID = INVALID_ID;
		m_ObjID = INVALID_ID;
		m_SceneID= INVALID_ID; 
	}
};

class Obj_Human;

struct _FOLLOWEDMEMBER
{
	GUID_t							m_GUID;
	Obj_Human*						m_pHuman;

	_FOLLOWEDMEMBER( )
	{
		CleanUp( );
	}

	VOID							CleanUp( )
	{
		m_GUID = INVALID_ID;
		m_pHuman= NULL; 
	}
};

#define LEADER_ID 0

class TeamInfo
{
public:
	TeamInfo( ) { CleanUp(); }
	~TeamInfo( ) { }

	inline VOID						CleanUp( );

	// 设置自己的 GUID
	inline VOID						SetMyGUID( GUID_t guid );

	// 设置自己所在的场景，玩家第一次组队的时候要设一次（设 TeamID 的时候）
	inline VOID						SetMySceneID( SceneID_t sid );

	inline VOID						DisMiss( );

	// 判断是否在队伍中
	inline BOOL						HasTeam( ) const;

	// 判断队伍是否已经满员
	inline BOOL						IsFull( );

	// 判断是否队长
	inline BOOL						IsLeader( ) const;

	// 得到队伍 ID
	inline TeamID_t					GetTeamID( ) const;

	// 设置队伍 ID
	inline VOID						SetTeamID( const TeamID_t teamid );

	// 得到队伍人数
	inline INT						GetTeamMemberCount( ) const;

	// 得到同场景队友的数量（不包含自己）
	inline INT						GetSceneMemberCount() const;

	// 得到同场景队友的 ObjID 列表（不包含自己）
	inline const ObjID_t			GetSceneMemberObjID( const INT memberindex ) const;

	// 增加一个队友
	VOID							AddMember( const TEAMMEMBER* pMember );

	// 减少一个队友
	VOID							DelMember( GUID_t guid );

	// 任命新队长
	VOID							Appoint( GUID_t guid );

	// 队员开始切换场景
	VOID							StartChangeScene( GUID_t guid );
	
	// 队员进入新场景
	VOID							EnterScene( GUID_t guid, SceneID_t SceneID, ObjID_t oid );

	// 队友断线
	VOID							MemberOffLine( GUID_t guid );

	// 得到队长的信息
	inline const TEAMMEMBER*		Leader( ) const;

	// 得到某个队员的信息
	inline const TEAMMEMBER*		GetTeamMember( const INT memberindex ) const;

	// 通过 GUID 得到某个队员
	inline const TEAMMEMBER*		GetTeamMemberByGUID( const GUID_t guid ) const;

	// 设置自己的队伍跟随状态
	inline VOID						SetTeamFollowFlag(BOOL flag);

	// 获得自己的队伍跟随状态
	inline const BOOL				GetTeamFollowFlag() const;

	// 增加一个跟随队员
	VOID							AddFollowedMember( const _FOLLOWEDMEMBER& FollowedMember );

	// 移出某个跟随队员
	VOID							DelFollowedMember( GUID_t guid );
	// 得到跟随队员的数量
	inline const INT				GetFollowedMembersCount() const;

	// 得到第i个跟随的队友
	inline const _FOLLOWEDMEMBER*	GetFollowedMember( INT i ) const;

	// 得到自己跟随的队友
	const Obj_Human*				GetMyGuide() const;

	// 得到跟随自己的队友
	const Obj_Human*				GetGuideMe() const;

	// 清除所有的跟随队友列表
	inline VOID						ClearFollowedMembers();

protected:

	// 增加到同场景队友列表	
	inline VOID						AddSceneMember( ObjID_t oid );

	// 减掉一个同场景队友
	inline VOID						DelSceneMember( ObjID_t oid );

	// 清除跟随者指针
	inline VOID						EraseFollowedMemberPointer( GUID_t guid );
private:
	TeamID_t						m_TeamID;						// 队伍号
	INT								m_MemberCount;					// 成员数量
	TEAMMEMBER						m_aMember[MAX_TEAM_MEMBER];		// 队员基本信息

	GUID_t							m_MyGUID;						// 自己的 GUID
	SceneID_t						m_MySceneID;					// 自己的场景 ID
	INT								m_SceneMemberCount;				// 同场景成员数量
	ObjID_t							m_aScenePlayer[MAX_TEAM_MEMBER];// 同场景成员的 ObjID 列表

	BOOL							m_bTeamFollowFlag;				// 队伍是否出于组队跟随状态的标记
	INT								m_nFollowedMembersCount;
	_FOLLOWEDMEMBER					m_FollowedMembers[MAX_TEAM_MEMBER];	// 跟随者列表
};

#include "Team.inl"
#endif // __TEAM_H__

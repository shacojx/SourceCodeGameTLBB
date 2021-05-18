#pragma once

#include "Type.h"
#include "GameStruct_Relation.h"

// 名单人数
#define LIST_MEMBER_COUNT			20
#define RELATION_DESC_SIZE			32
#define	LOCATION_DESC_SIZE			32
#define	TEAM_DESC_SIZE				32

struct SDATA_RELATION_MEMBER
{
	GUID_t								m_GUID;								// GUID
	CHAR								m_szName[MAX_CHARACTER_NAME];		// 名字

	RELATION_TYPE						m_RelationType;						// 关系类型
	INT									m_nFriendPoint;						// 好友度

	INT									m_nLevel;							// 角色等级
	INT									m_nMenPai;							// 门派 MENPAI_ATTRIBUTE
	INT									m_nPortrait;						// 头像
	GuildID_t							m_GuildID;							// 帮会ID
	CHAR								m_szGuildName[MAX_GUILD_NAME_SIZE];	// 帮会名称
	BOOL								m_bOnlineFlag;						// 是否在线
	CHAR								m_szMood[MOOD_DATA_SIZE];			// 心情
	CHAR								m_szTitle[MAX_CHARACTER_TITLE];		// 称号
	SceneID_t							m_SceneID;							// 所在场景
	CHAR								m_szRelation[RELATION_DESC_SIZE];	// 具体关系描述
	CHAR								m_szLocation[LOCATION_DESC_SIZE];	// 具体位置（离线或者场景）
	INT									m_nTeamSize;						// 队伍人数（0 表示未组队）
	CHAR								m_szTeamDesc[TEAM_DESC_SIZE];		// 组队描述
	std::vector< INT >					m_vMailList;						// 邮件索引列表

	INT									m_nEnterOrder;						// 临时好友进入的顺序

	SDATA_RELATION_MEMBER()				{ CleanUp(); }
	VOID								CleanUp();
};

// 名单
class RelationList
{
public:
	~RelationList();

	VOID								CleanUp();

	// 加入名单
	virtual BOOL						Add( SDATA_RELATION_MEMBER* pMember );

	// 从名单里移除，只移除指针，不释放数据
	virtual BOOL						Remove( INT idx );

	// 从名单里移除，只移除指针，不释放数据
	virtual BOOL						RemoveByGUID( GUID_t guid );

	// 从名单里移除，并释放数据
	virtual BOOL						Erase( INT idx );

	// 从名单里移除，并释放数据
	virtual BOOL						EraseByGUID( GUID_t guid );

	// 得到当前名单人数
	inline INT							GetMemberCount();

	// 得到一个关系数据（不可修改数据）
	const SDATA_RELATION_MEMBER*		GetMember( INT idx );

	// 得到一个关系数据（可以修改数据）
	SDATA_RELATION_MEMBER*				GetMemberByGUID( GUID_t guid );

	// 通过名字得到一个关系数据（可以修改数据）
	SDATA_RELATION_MEMBER*				GetMemberByName( const CHAR* szName );
	
	// 通过名字得到在名单中的位置（可以修改数据）
	INT									GetMemberIndexByName( const CHAR* szName );

	// 判断当前名单是否已满
	BOOL								IsFull();

	// 得到某个 GUID 在名单中的位置，如果不存在，则返回 -1
	inline INT							GetMemberIndex(GUID_t guid);

protected:

	// 判断是否名单需要的类型
	virtual BOOL						IsFitType( SDATA_RELATION_MEMBER* pMember ) = 0;

	// 返回值：CSTR_LESS_THAN, CSTR_EQUAL, CSTR_GREATER_THAN,
	// 错误时返回 0
	static INT							CompareByName(const CHAR* szName1, const CHAR* szName2);

	// 比较两个关系人的优先排放顺序（<0表示优先级低，0表示相等，>0表示优先级高）
	virtual INT							Compare(const SDATA_RELATION_MEMBER* pMember1, const SDATA_RELATION_MEMBER* pMember2);

protected:

	// 有社会关系的人员
	std::vector<SDATA_RELATION_MEMBER*>	m_vecRelationMember;
};

// 好友名单
class FriendList : public RelationList
{
public:

protected:

	// 判断是否名单需要的类型
	virtual BOOL						IsFitType( SDATA_RELATION_MEMBER* pMember );

	// 计算一个好友的优先级，用于排序，目前仅用于 Compare 使用，所以返回值不定义
	INT									CalcPriority( const SDATA_RELATION_MEMBER* pMember );

	// 比较两个关系人的优先排放顺序（<0表示优先级低，0表示相等，>0表示优先级高）
	virtual INT							Compare(const SDATA_RELATION_MEMBER* pMember1, const SDATA_RELATION_MEMBER* pMember2);

private:

	// 最后一次获取详细信息的时间，这个变量的使用与否取决于好友界面（或 Tip）需要显示的信息量
	//DWORD								m_dwTimeStamp;
};

// 临时关系名单
class TempRelationList : public RelationList
{
public:

	// 加入名单
	virtual BOOL						Add( SDATA_RELATION_MEMBER* pMember );

protected:

	// 判断是否名单需要的类型
	virtual BOOL						IsFitType( SDATA_RELATION_MEMBER* pMember );

private:

	INT									m_nMaxOrder;
};

// 黑名单
class BlackList : public RelationList
{
public:

protected:

	// 判断是否名单需要的类型
	virtual BOOL						IsFitType( SDATA_RELATION_MEMBER* pMember );

private:
};

class Relation
{
public:
	// 从服务器端接收一个好友
	BOOL								AddRelation( const _FRIEND_INFO* pFriend );

	// 从服务器端接收一个黑名单玩家
	BOOL								AddRelation( const _BLACKNAME_INFO* pBlackName );

	// 获得一个名单的人数
	INT									GetListCount( RELATION_GROUP RelationGroup );

	// 客户端获得关系人信息，供界面使用
	const SDATA_RELATION_MEMBER*		GetRelationInfo( RELATION_GROUP RelationGroup, INT idx );

	// 加入一个关系，用于游戏过程中，加入时需要指定将要加入的组
	BOOL								AddRelation( RELATION_GROUP RelationGroup, SDATA_RELATION_MEMBER* pMember );

	// 服务器端发消息来更新关系人信息
	BOOL								UpdateRelationInfo( RETURN_RELATION_INFO* pRelationInfo );

	// 客户端移除一个关系，通常用于移除临时好友
	BOOL								RemoveRelation( RELATION_GROUP RelationGroup, INT idx );

	// 客户端移除一个关系，通常用于移除临时好友
	BOOL								RemoveRelation( RELATION_GROUP RelationGroup, const CHAR* szName );

	// 从服务器端收到一条移除关系人的消息
	BOOL								RemoveRelation( GUID_t guid );

	// 移动一个关系，Srg 是原来的组，guid 是要移动的玩家 GUID，Dtype 是移动后的关系类型，Drg 是移动后的组
	BOOL								MoveRelation( RELATION_GROUP Srg, RELATION_TYPE Dtype, RELATION_GROUP Drg, GUID_t guid );

	// 通知 UI 刷新列表，默认更新所有列表
	VOID								UpdateUIList( RELATION_GROUP RelationGroup = RELATION_GROUP_FRIEND_ALL );

	// 通知 UI 刷新 RelationGroup 中第 idx 号关系人的详细信息
	VOID								UpdateUIInfo( RELATION_GROUP RelationGroup, INT idx );

	// 得到一个 GUID 所在的组以及在组里的索引，没有找到返回 RELATION_TYPE_STRANGER，这个功能不考虑临时好友
	RELATION_TYPE						GetRelationPosition( GUID_t guid, RELATION_GROUP& RelationGroup, INT& idx );

	// 得到一个 名字 所在的组以及在组里的索引，没有找到返回 RELATION_TYPE_STRANGER，这个功能不考虑临时好友
	RELATION_TYPE						GetRelationByName( const CHAR* szName, INT& RelationGroup, INT& idx );

	// 更新在线好友
	BOOL								UpdateOnlineFriend( const _RELATION_ONLINE* pOnlineRelation );

	// 关系人上线
	BOOL								RelationOnline(const CHAR* szName, const CHAR* szMood);

	// 关系人下线
	BOOL								RelationOffLine(GUID_t guid);

	// 判断是否在自己黑名单中
	BOOL								IsBlackName( const CHAR* szName );

	// 给某个好友增加一条邮件（邮件池中的索引号）
	BOOL								AddMail( const CHAR* szName, UINT uIndex );

	// 得到某个好友的历史邮件数量
	UINT								GetMailCount( RELATION_GROUP RelationGroup, INT idx );

	// 得到某个好友的第 uIndex 封历史邮件的邮件池索引号
	UINT								GetMailIndex( RELATION_GROUP RelationGroup, INT idx, UINT uIndex );

	// 给某个（临时）好友重建历史信息
	VOID								RebuildMailHistory( RELATION_GROUP RelationGroup, const CHAR* szName );

	// 得到心情
	const CHAR*							GetMood( );

	// 设置心情
	VOID								SetMood( const CHAR* szMood );

	// 查看玩家界面的信息
	const SDATA_RELATION_MEMBER*		GetPlayerCheckInfo( );

protected:

	// 获得一个名单
	inline RelationList*				GetRelationList( RELATION_GROUP RelationGroup );

	// 根据场景 ID 获得场景名字，并存入 szSceneName
	inline BOOL							GetLocationName( SceneID_t sid, CHAR* szSceneName );

	// 根据关系类型或者友好度确定双方关系，并存入 m_szRelation
	inline BOOL							SetRelationDesc( SDATA_RELATION_MEMBER* pMember );

	// 根据队伍人数设置组队显示数据，并存入 m_szTeamDesc
	inline BOOL							SetTeamDesc( SDATA_RELATION_MEMBER* pMember );

private:
	// MAX_MOOD_SIZE 才是允许输入的字符长度
	CHAR								m_szMood[MOOD_DATA_SIZE];// 心情数据
	FriendList							m_FriendGroup1;			// 好友 1 组
	FriendList							m_FriendGroup2;			// 好友 2 组
	FriendList							m_FriendGroup3;			// 好友 3 组
	FriendList							m_FriendGroup4;			// 好友 4 组
	TempRelationList					m_TeamFriendGroup;		// 临时好友组
	BlackList							m_BlackListGroup;		// 黑名单组
	SDATA_RELATION_MEMBER				m_ViewPlayer;			// 查看玩家
};

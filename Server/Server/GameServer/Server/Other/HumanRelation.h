
/////////////////////////////////////////////////////////////////////////////////
//文件名：HumanRelation.h
//功能描述：实现玩家好友以及黑名单相关功能
//修改记录：2005-11-30 创建
//			2005-12-16 增改数据结构以及功能模块
/////////////////////////////////////////////////////////////////////////////////

#ifndef __HUMANRELATION_H__
#define __HUMANRELATION_H__

#include "Type.h"
#include "DB_Struct.h"

class Obj_Human;

class HumanRelation
{
public :
	HumanRelation( );
	~HumanRelation( );

	BOOL							Init( Obj_Human* pHuman );
	VOID							CleanUp( );

	// 判断是否自己的好友
	BOOL							IsFriend( GUID_t guid );

	// 判断是否在黑名单
	BOOL							IsBlackName( GUID_t guid );

	// 得到两个玩家之间的关系类型
	RELATION_TYPE					GetRelationType( GUID_t guid );

	// 判断两个玩家之间是否存在某种关系
	BOOL							HaveRelation( RELATION_TYPE RelationType, GUID_t guid );

	// 添加一个关系户
	BOOL							AddRelation( RELATION_TYPE RelationType, _RELATION* pRelationData );

	// 添加一个关系户
	BOOL							AddRelation( RELATION_TYPE RelationType, const RELATION_MEMBER* pMember );

	// 删除一个关系户
	BOOL							DelRelation( RELATION_TYPE RelationType, GUID_t guid );

	// 变更关系
	BOOL							RelationTransition( RELATION_TYPE newRelationType, GUID_t guid );

	// 是否某组已经满员
	BOOL							IsGroupFull( RELATION_GROUP RelationGroup );

	// 判断一种关系是否能够直接转换成另一种关系
	BOOL							CanTransition( RELATION_TYPE oldRelationType, RELATION_TYPE newRelationType );

	// 得到心情
	const CHAR*						GetMood( );

	// 设置心情
	VOID							SetMood( const CHAR* szMood );

	// 查询关系值
	INT								GetFriendPoint( GUID_t guid );

	// 增加关系值
	VOID							IncFriendPoint( GUID_t guid, INT nPoint = 1 );

	// 减少关系值
	VOID							DecFriendPoint( GUID_t guid, INT nPoint = 1 );

	// 设置分组（该接口纯粹是为了客户端显示而设置）
	BOOL							SetFriendGroup( GUID_t guid, RELATION_GROUP RelationGroup );

	// 填充关系列表消息结构
	VOID							FillRelationList( GC_RELATIONLIST& RelationList );

	// 更新关系人数据，并返回更新后的关系人数据
	const _OWN_RELATION*			UpdateRelationInfo( _RELATION* pRelationData );

	// 获得关系人数据
	const _OWN_RELATION*			GetRelationInfo( GUID_t guid );

	// 将关系人数据中的缺少项填充
	//BOOL							CompleteRelationInfo( _RELATION& RelationData );

	// 以下是结婚相关的一些操作

	// 是否已婚
	BOOL							IsMarried();

	// 得到配偶的 GUID
	GUID_t							GetSpouseGUID();

	// 和某个 GUID 对应的玩家结婚（在这里不管性别）
	BOOL							Marry( GUID_t guid );

	// 离婚
	BOOL							Divorce( );

	// 以下是师徒的一些相关操作

	// 判断 guid 对应的玩家是否是自己的师傅
	BOOL							IsMaster( GUID_t guid );

	// 得到师傅的 GUID
	GUID_t							GetMasterGUID();

	// 判断 guid 对应的玩家是否是自己的徒弟
	BOOL							IsPrentice( GUID_t guid );

	// 判断是否有师傅
	BOOL							HaveMaster( );

	// 取得有几个徒弟
	INT								GetPrenticeCount();

	// 取得第 nIndex 个徒弟的 GUID
	GUID_t							GetNthPrentice( INT nIndex );

	// 取得师德点
	UINT							GetMoralPoint( );

	// 设置师德点
	BOOL							SetMoralPoint( UINT uPoint );

	// 取得最后一次徒弟叛师时间据当前时间的长度（返回秒数）
	UINT							GetPrenticeBetrayTime();

	// 拜 guid 对应的玩家为师
	BOOL							Aprentice( GUID_t guid );

	// 收 guid 对应的玩家为徒
	BOOL							Recruit( GUID_t guid );

	// 脱离师门
	BOOL							LeaveMaster( );

	// 驱逐 GUID 为 guid 的徒弟
	BOOL							ExpelPrentice( GUID_t guid );

	// 以下是结拜的一些操作

	// 判断是否是结拜兄弟
	BOOL							IsBrother( GUID_t guid );

	// 给所有好友发送邮件
	VOID							SendMailToAllFriend( const CHAR* szMail );

protected :
	inline const _RELATION_DB_LOAD*	GetRelationDB( );

	inline const _OWN_RELATION*		GetRelationList( );

	// 取得联系人的数据，返回指针以及 Index 值
	const _OWN_RELATION*			GetRelation( RELATION_TYPE RelationType, GUID_t guid, INT& index );

protected :
	Obj_Human*						m_pHuman;
};




#endif

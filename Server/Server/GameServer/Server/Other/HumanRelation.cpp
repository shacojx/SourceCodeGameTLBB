#include "stdafx.h"

#include "HumanRelation.h"
#include "Obj_Human.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GWRelation.h"
#include "GWBatchMail.h"
#include "TimeManager.h"

HumanRelation::HumanRelation( )
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}

HumanRelation::~HumanRelation( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL HumanRelation::Init( Obj_Human* pHuman )
{
__ENTER_FUNCTION

	Assert( pHuman );
	m_pHuman = pHuman;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID HumanRelation::CleanUp( )
{
__ENTER_FUNCTION

	m_pHuman = NULL;

__LEAVE_FUNCTION
}

// 判断是否自己的好友
BOOL HumanRelation::IsFriend( GUID_t guid )
{
__ENTER_FUNCTION

	Assert( guid != INVALID_ID );

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=RELATION_FRIEND_OFFSET; i<RELATION_BLACKNAME_OFFSET; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			return TRUE;
		}
	}

__LEAVE_FUNCTION

	return FALSE;
}

// 判断是否在黑名单
BOOL HumanRelation::IsBlackName( GUID_t guid )
{
__ENTER_FUNCTION

	Assert( guid != INVALID_ID );

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=RELATION_BLACKNAME_OFFSET; i<MAX_RELATION_SIZE; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			return TRUE;
		}
	}

__LEAVE_FUNCTION

	return FALSE;
}

// 得到两个玩家之间的关系类型
RELATION_TYPE HumanRelation::GetRelationType( GUID_t guid )
{
__ENTER_FUNCTION

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=RELATION_FRIEND_OFFSET; i<MAX_RELATION_SIZE; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			return (RELATION_TYPE)(pRelationList[i].m_Type);
		}
	}

	return RELATION_TYPE_STRANGER;

__LEAVE_FUNCTION

	return RELATION_TYPE_STRANGER;
}

// 判断两个玩家之间是否存在某种关系
BOOL HumanRelation::HaveRelation( RELATION_TYPE RelationType, GUID_t guid )
{
__ENTER_FUNCTION

	if( RelationType <= RELATION_TYPE_NONE || RelationType >= RELATION_TYPE_SIZE )
	{
		Assert(FALSE);
		return FALSE;
	}

	INT nStart, nEnd;

	switch( RelationType )
	{
	case RELATION_TYPE_FRIEND:
		{
			nStart = RELATION_FRIEND_OFFSET;
			nEnd = RELATION_BLACKNAME_OFFSET;
		}
		break;
	case RELATION_TYPE_BLACKNAME:
		{
			nStart = RELATION_BLACKNAME_OFFSET;
			nEnd = MAX_RELATION_SIZE;
		}
		break;
	case RELATION_TYPE_TEMPFRIEND:
	case RELATION_TYPE_STRANGER:
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_MARRY:
		{
			Assert(FALSE);
			return FALSE;
		}
		break;
	default:
		Assert(FALSE);
		return FALSE;
	}

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=nStart; i<nEnd; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			if( pRelationList[i].m_Type == RelationType )
			{
				return TRUE;
			}

			break;
		}
	}

	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

// 添加一个关系户
BOOL HumanRelation::AddRelation( RELATION_TYPE RelationType, _RELATION* pRelationData )
{
__ENTER_FUNCTION

	Assert( pRelationData );

	RELATION_MEMBER Member;
	Member.m_MemberGUID = pRelationData->GetGUID();
	strncpy(Member.m_szMemberName, pRelationData->GetName(), sizeof(Member.m_szMemberName) - 1);
	Member.m_nLevel = pRelationData->GetLevel();
	Member.m_nMenPai = pRelationData->GetMenPai();
	Member.m_nPortrait = pRelationData->GetPortrait();
	Member.m_GuildID = pRelationData->GetGuildID();

	return AddRelation( RelationType, &Member );

__LEAVE_FUNCTION

	return FALSE;
}

// 添加一个关系户
BOOL HumanRelation::AddRelation( RELATION_TYPE RelationType, const RELATION_MEMBER* pMember )
{
__ENTER_FUNCTION

	Assert( pMember );

	if( HaveRelation( RelationType, pMember->m_MemberGUID ) )
	{ // 有同组人员在了
		return FALSE;
	}

	UCHAR uGroup = 0;

	switch( RelationType )
	{
	case RELATION_TYPE_FRIEND:
		{
			uGroup = RELATION_GROUP_FRIEND_ALL;
		}
		break;
	case RELATION_TYPE_BLACKNAME:
		{
			uGroup = RELATION_GROUP_BLACK;
		}
		break;
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_MARRY:
	case RELATION_TYPE_TEMPFRIEND:
	case RELATION_TYPE_STRANGER:
		{
			Assert(FALSE);
			return FALSE;
		}
		break;
	default:
		Assert(FALSE);
		return FALSE;
	}

	if( IsGroupFull((RELATION_GROUP)uGroup) )
	{ // 人满了
		return FALSE;
	}

	INT index;

	if( GetRelation(RelationType, INVALID_ID, index) != NULL )
	{ // 找出一个空位置
		_OWN_RELATION Relation;
		Relation.m_Type = RelationType;
		Relation.m_Group = uGroup;
		memcpy((void*)&(Relation.m_Member), pMember, sizeof(RELATION_MEMBER));

		m_pHuman->GetDB()->SetRelation(index, &Relation);
		m_pHuman->GetDB()->IncRelationCount(uGroup);
		return TRUE;
	}

	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

// 删除一个关系户
BOOL HumanRelation::DelRelation( RELATION_TYPE RelationType, GUID_t guid )
{
__ENTER_FUNCTION

	BOOL bFriendFlag = FALSE;

	switch( RelationType )
	{
	case RELATION_TYPE_FRIEND:
		{
			bFriendFlag = TRUE;
		}
		break;
	case RELATION_TYPE_BLACKNAME:
		{
		}
		break;
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_MARRY:
	case RELATION_TYPE_TEMPFRIEND:
	case RELATION_TYPE_STRANGER:
		{
			Assert(FALSE);
			return FALSE;
		}
		break;
	default:
		Assert(FALSE);
		return FALSE;
	}

	INT index;

	const _OWN_RELATION* pRelation;
	pRelation = GetRelation(RelationType, guid, index);
	if( pRelation != NULL )
	{
		if( bFriendFlag )
		{ // 减少好友的数量
			m_pHuman->GetDB()->DecRelationCount(RELATION_GROUP_FRIEND_ALL);
		}

		m_pHuman->GetDB()->DecRelationCount(pRelation->m_Group);
		m_pHuman->GetDB()->DeleteRelation(index);
		return TRUE;
	}

	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

// 变更关系
BOOL HumanRelation::RelationTransition( RELATION_TYPE newRelationType, GUID_t guid )
{
__ENTER_FUNCTION

	// 1、得到转换前关系
	RELATION_TYPE oldRelationType = GetRelationType(guid);

	// 2、验证两种关系的转换合法性
	if( CanTransition(oldRelationType, newRelationType) != TRUE )
	{
		return FALSE;
	}

	// 3、验证转换后的关系所占据的存储空间足够
	if( newRelationType == RELATION_TYPE_FRIEND )
	{
		if( IsGroupFull(RELATION_GROUP_FRIEND_ALL) )
		{
			return FALSE;
		}
	}
	else if( newRelationType == RELATION_TYPE_BLACKNAME )
	{
		if( IsGroupFull(RELATION_GROUP_BLACK) )
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	// 4、实行转换，并更正计数
	INT index;
	const _OWN_RELATION* pRelation;
	pRelation = GetRelation( oldRelationType, guid, index );
	if( pRelation == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	AddRelation( newRelationType, &(pRelation->m_Member) );
	DelRelation( oldRelationType, guid );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 是否某组已经满员
BOOL HumanRelation::IsGroupFull( RELATION_GROUP RelationGroup )
{
__ENTER_FUNCTION

	if( RelationGroup < RELATION_GROUP_FRIEND_ALL
	 || RelationGroup >= RELATION_GROUP_NUMBER
	 )
	{
		Assert(FALSE);
		return TRUE;
	}

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return TRUE;
	}

	INT nCount;
	nCount = pRelationDB->m_aRelationCount[RelationGroup];

	switch(RelationGroup)
	{
	case RELATION_GROUP_FRIEND_ALL:
		{
			if( nCount >= RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET )
			{
				return TRUE;
			}
		}
		break;
	case RELATION_GROUP_F1:
	case RELATION_GROUP_F2:
	case RELATION_GROUP_F3:
	case RELATION_GROUP_F4:
		{
			if( nCount >= (RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET) / 4 )
			{
				return TRUE;
			}
		}
		break;
	case RELATION_GROUP_BLACK:
		{
			if( nCount >= MAX_RELATION_SIZE - RELATION_BLACKNAME_OFFSET )
			{
				return TRUE;
			}
		}
		break;
	default:
		Assert(FALSE);
		return TRUE;
	}

	return FALSE;

__LEAVE_FUNCTION

	return TRUE;
}

// 判断一种关系是否能够直接转换成另一种关系
BOOL HumanRelation::CanTransition( RELATION_TYPE oldRelationType, RELATION_TYPE newRelationType )
{
__ENTER_FUNCTION

	if( oldRelationType <= RELATION_TYPE_NONE || oldRelationType >= RELATION_TYPE_SIZE
	 || newRelationType <= RELATION_TYPE_NONE || newRelationType >= RELATION_TYPE_SIZE
	 )
	{
		Assert(FALSE);
		return FALSE;
	}

	switch(oldRelationType)
	{
	case RELATION_TYPE_FRIEND:
		{
			if( newRelationType == RELATION_TYPE_BLACKNAME )
			{
				return TRUE;
			}
		}
		break;
	case RELATION_TYPE_BLACKNAME:
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_MARRY:
	case RELATION_TYPE_TEMPFRIEND:
	case RELATION_TYPE_STRANGER:
	case RELATION_TYPE_MASTER:
	case RELATION_TYPE_PRENTICE:
		{
			return FALSE;
		}
		break;
	default:
		Assert(FALSE);
		return FALSE;
	}

	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

// 得到心情
const CHAR* HumanRelation::GetMood( )
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return (pRelationDB->m_szMood);

__LEAVE_FUNCTION

	return NULL;
}

// 设置心情
VOID HumanRelation::SetMood( const CHAR* szMood )
{
__ENTER_FUNCTION

	if( szMood == NULL )
	{
		Assert( szMood );
		return;
	}

	m_pHuman->GetDB()->SetMood(szMood);

__LEAVE_FUNCTION
}

// 查询关系值
INT HumanRelation::GetFriendPoint( GUID_t guid )
{
__ENTER_FUNCTION

	INT index;
	const _OWN_RELATION* pRelation;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, index);
	if( pRelation == NULL )
	{
		Assert(FALSE);
		return 0;
	}

	return pRelation->m_FriendPoint;

__LEAVE_FUNCTION

	return 0;
}

// 增加关系值
VOID HumanRelation::IncFriendPoint( GUID_t guid, INT nPoint )
{
__ENTER_FUNCTION

	Assert( nPoint > 0 );

	INT index;
	const _OWN_RELATION* pRelation;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, index );
	if( pRelation == NULL )
	{
		Assert(FALSE);
		return;
	}

	if( pRelation->m_FriendPoint + nPoint > MAX_FRIEND_POINT )
	{
		nPoint = MAX_FRIEND_POINT - pRelation->m_FriendPoint;
	}

	if( nPoint <= 0 )
	{ // 已经到了上限
		return;
	}

	m_pHuman->GetDB()->ModifyFriendPoint(index, nPoint);

	if( pRelation->m_FriendPoint > g_Config.m_ConfigInfo.m_nPromptPoint )
	{
		GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
		GW_RELATION* pRelation = pMsg->GetRelation();
		pRelation->CleanUp();
		pRelation->m_Type = REQ_NEWGOODFRIEND;
		pRelation->SetGUID( m_pHuman->GetGUID() );
		pRelation->m_RelationGUID.CleanUp();
		pRelation->m_RelationGUID.SetTargetGUID(guid);

		g_pServerManager->SendPacket( pMsg, INVALID_ID );
	}

__LEAVE_FUNCTION
}

// 减少关系值
VOID HumanRelation::DecFriendPoint( GUID_t guid, INT nPoint )
{
__ENTER_FUNCTION

	Assert( nPoint > 0 );

	INT index;
	if( GetRelation( RELATION_TYPE_FRIEND, guid, index ) == NULL )
	{
		Assert(FALSE);
		return;
	}

	m_pHuman->GetDB()->ModifyFriendPoint(index, -nPoint);

	__LEAVE_FUNCTION
}

// 设置分组（该接口纯粹是为了客户端显示而设置）
BOOL HumanRelation::SetFriendGroup( GUID_t guid, RELATION_GROUP RelationGroup )
{
__ENTER_FUNCTION

	INT index;
	const _OWN_RELATION* pRelation;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, index );
	if( pRelation == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	RELATION_GROUP oldRelationGroup;
	oldRelationGroup = (RELATION_GROUP)pRelation->m_Group;

	if( oldRelationGroup == RelationGroup )
	{
		return TRUE;
	}

	if( IsGroupFull(RelationGroup) )
	{
		return FALSE;
	}

	if( oldRelationGroup != RELATION_GROUP_FRIEND_ALL )
	{
		m_pHuman->GetDB()->DecRelationCount(oldRelationGroup);
	}

	m_pHuman->GetDB()->AdjustRelationGroup(index, RelationGroup);

	if( RelationGroup != RELATION_GROUP_FRIEND_ALL )
	{
		m_pHuman->GetDB()->IncRelationCount(RelationGroup);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 填充关系列表消息结构
VOID HumanRelation::FillRelationList( GC_RELATIONLIST& RelationList )
{
__ENTER_FUNCTION

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		switch( pRelationList[i].m_Type )
		{
		case RELATION_TYPE_NONE:
			{
			}
			break;
		case RELATION_TYPE_FRIEND:
		case RELATION_TYPE_BROTHER:
		case RELATION_TYPE_MARRY:
		case RELATION_TYPE_MASTER:
		case RELATION_TYPE_PRENTICE:
			{
				RelationList.AddFriend( &pRelationList[i] );
			}
			break;
		case RELATION_TYPE_BLACKNAME:
			{
				RelationList.AddBlackName( &pRelationList[i] );
			}
			break;
		default:
			Assert(FALSE);
			return;
		}

	}

__LEAVE_FUNCTION
}

// 更新关系人数据
const _OWN_RELATION* HumanRelation::UpdateRelationInfo( _RELATION* pRelationData )
{
__ENTER_FUNCTION
	// 先找到这个人，如果找不到就不更新了
	Assert( pRelationData );

	GUID_t guid = pRelationData->GetGUID();

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=RELATION_FRIEND_OFFSET; i<MAX_RELATION_SIZE; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			RELATION_MEMBER Member;
			Member.m_MemberGUID = pRelationData->GetGUID();
			strncpy(Member.m_szMemberName, pRelationData->GetName(), sizeof(Member.m_szMemberName) - 1);
			Member.m_nLevel = pRelationData->GetLevel();
			Member.m_nMenPai = pRelationData->GetMenPai();
			Member.m_nPortrait = pRelationData->GetPortrait();
			Member.m_GuildID = pRelationData->GetGuildID();

			m_pHuman->GetDB()->UpdateRelationInfo(i, &Member);
			return &(pRelationList[i]);
		}
	}

	// 更新进去
__LEAVE_FUNCTION

	return NULL;
}

// 获得关系人数据
const _OWN_RELATION* HumanRelation::GetRelationInfo( GUID_t guid )
{
__ENTER_FUNCTION

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=RELATION_FRIEND_OFFSET; i<MAX_RELATION_SIZE; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			return &(pRelationList[i]);
		}
	}

__LEAVE_FUNCTION

	return NULL;
}

// 将关系人数据中的缺少项填充
//BOOL HumanRelation::CompleteRelationInfo( _RELATION& RelationData )
//{
//__ENTER_FUNCTION
//
//	if( RelationData.GetGUID() == NULL )
//	{
//		Assert( FALSE );
//		return FALSE;
//	}
//
//__LEAVE_FUNCTION
//
//	return FALSE;
//}

// 以下是结婚相关的一些操作
BOOL HumanRelation::IsMarried()
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return (pRelationDB->m_MarriageInfo.m_SpouseGUID != INVALID_ID);

__LEAVE_FUNCTION

	return NULL;
}

// 得到配偶的 GUID
GUID_t HumanRelation::GetSpouseGUID()
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return INVALID_ID;
	}

	return pRelationDB->m_MarriageInfo.m_SpouseGUID;

__LEAVE_FUNCTION

	return INVALID_ID;
}

// 和某个 GUID 对应的玩家结婚（在这里不管性别）
BOOL HumanRelation::Marry( GUID_t guid )
{
__ENTER_FUNCTION

	if( IsMarried() )
	{
		Assert( IsMarried() );
		return FALSE;
	}

	// 找到该 GUID 的好友
	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_FRIEND) )
	{ // 没有这个朋友或关系不是普通好友关系
		AssertEx(FALSE, "对不起，目前有且只有普通好友关系可以结婚。");
		return FALSE;
	}

	// 将好友的类型设为夫妻
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_MARRY );

	// 将配偶的 GUID 设为 guid
	MarriageInfo info;
	info.m_SpouseGUID = guid;

	m_pHuman->GetDB()->SetSpouse(&info);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 离婚
BOOL HumanRelation::Divorce( )
{
__ENTER_FUNCTION

	GUID_t guid;
	guid = GetSpouseGUID();

	if( guid == INVALID_ID )
	{
		Assert( INVALID_ID != GetSpouseGUID() );
		return FALSE;
	}

	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_MARRY, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_MARRY) )
	{ // 没有这个朋友或关系不是夫妻关系
		AssertEx(FALSE, "对不起，只有夫妻关系可以离婚。");
		return FALSE;
	}

	// 将好友的类型设为普通好友
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_FRIEND );

	// 将配偶的 GUID 置空
	MarriageInfo info;
	info.m_SpouseGUID = INVALID_ID;

	m_pHuman->GetDB()->SetSpouse(&info);

__LEAVE_FUNCTION

	return FALSE;
}

// 判断 guid 对应的玩家是否是自己的师傅
BOOL HumanRelation::IsMaster( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert( guid != INVALID_ID );
		return FALSE;
	}

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return (pRelationDB->m_MasterInfo.m_MasterGUID == guid);

__LEAVE_FUNCTION

	return FALSE;
}

// 得到师傅的 GUID
GUID_t HumanRelation::GetMasterGUID()
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return INVALID_ID;
	}

	return pRelationDB->m_MasterInfo.m_MasterGUID;

__LEAVE_FUNCTION

	return INVALID_ID;
}

// 判断 guid 对应的玩家是否是自己的徒弟
BOOL HumanRelation::IsPrentice( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert( guid != INVALID_ID );
		return FALSE;
	}

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	for( INT i=0; i<MAX_PRENTICE_COUNT; ++i )
	{
		if( pRelationDB->m_PrenticeInfo.m_PrenticeGUID[i] == guid )
		{
			return TRUE;
		}
	}

__LEAVE_FUNCTION

	return FALSE;
}

// 判断是否有师傅
BOOL HumanRelation::HaveMaster( )
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return (pRelationDB->m_MasterInfo.m_MasterGUID != INVALID_ID);

__LEAVE_FUNCTION

	return FALSE;
}

// 取得有几个徒弟
INT HumanRelation::GetPrenticeCount()
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return 0;
	}

	return pRelationDB->m_PrenticeInfo.m_uPrenticeCount;

__LEAVE_FUNCTION

	return 0;
}

// 取得第 nIndex 个徒弟的 GUID
GUID_t HumanRelation::GetNthPrentice( INT nIndex )
{
__ENTER_FUNCTION

	if( nIndex >= MAX_PRENTICE_COUNT )
	{
		Assert( nIndex < MAX_PRENTICE_COUNT );
		return INVALID_ID;
	}

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return pRelationDB->m_PrenticeInfo.m_PrenticeGUID[nIndex];

__LEAVE_FUNCTION

	return INVALID_ID;

}

// 取得师德点
UINT HumanRelation::GetMoralPoint( )
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return pRelationDB->m_PrenticeInfo.m_uMoralPoint;

__LEAVE_FUNCTION

	return 0;

}

// 设置师德点
BOOL HumanRelation::SetMoralPoint( UINT uPoint )
{
__ENTER_FUNCTION

	if( uPoint > MAX_MORAL_POINT )
	{
		Assert( uPoint <= MAX_MORAL_POINT );
		return FALSE;
	}

	m_pHuman->GetDB()->SetMoralPoint(uPoint);

__LEAVE_FUNCTION

	return FALSE;

}

// 取得最后一次徒弟叛师时间据当前时间的长度（返回秒数）
UINT HumanRelation::GetPrenticeBetrayTime()
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return 0;
	}

	UINT sec; // 最后一次叛师的秒数
	sec = (UINT)(g_pTimeManager->GetANSITime() - pRelationDB->m_PrenticeInfo.m_BetrayingTime);

	return sec;

__LEAVE_FUNCTION

	return 0;
}

// 拜 guid 对应的玩家为师
BOOL HumanRelation::Aprentice( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert(guid != INVALID_ID);
		return FALSE;
	}

	if( HaveMaster() )
	{
		Assert( FALSE );
		return FALSE;
	}

	// 找到该 GUID 的好友
	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_FRIEND) )
	{ // 没有这个朋友或关系不是普通好友关系
		AssertEx(FALSE, "对不起，目前有且只有普通好友关系可以拜师。");
		return FALSE;
	}

	// 将好友的类型设为师傅
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_MASTER );

	// 将师傅的 GUID 设为 guid
	MasterInfo info;
	info.m_MasterGUID = guid;

	m_pHuman->GetDB()->SetMaster( &info );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 收 guid 对应的玩家为徒
BOOL HumanRelation::Recruit( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert(guid != INVALID_ID);
		return FALSE;
	}

	if( GetPrenticeCount() >= MAX_PRENTICE_COUNT )
	{
		Assert( GetPrenticeCount() < MAX_PRENTICE_COUNT );
		return FALSE;
	}

	if( IsPrentice( guid ) )
	{
		Assert( FALSE );
		return FALSE;
	}

	// 找到该 GUID 的好友
	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_FRIEND) )
	{ // 没有这个朋友或关系不是普通好友关系
		AssertEx(FALSE, "对不起，目前只能将普通好友收为徒弟。");
		return FALSE;
	}

	// 将好友的类型设为师傅
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_PRENTICE );

	m_pHuman->GetDB()->AddPrentice( guid );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 脱离师门
BOOL HumanRelation::LeaveMaster( )
{
__ENTER_FUNCTION

	GUID_t guid;
	guid = GetMasterGUID();

	if( guid == INVALID_ID )
	{
		Assert( INVALID_ID != GetMasterGUID() );
		return FALSE;
	}

	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_MASTER, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_MASTER) )
	{ // 没有这个朋友或他不是师傅
		AssertEx(FALSE, "对不起，只有对师傅可以执行此操作。");
		return FALSE;
	}

	// 将好友的类型设为普通朋友
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_FRIEND );

	// 将师傅的 GUID 置空
	MasterInfo info;
	info.m_MasterGUID = INVALID_ID;

	m_pHuman->GetDB()->SetMaster(&info);

__LEAVE_FUNCTION

	return FALSE;
}

// 驱逐 GUID 为 guid 的徒弟
BOOL HumanRelation::ExpelPrentice( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert(guid != INVALID_ID);
		return FALSE;
	}

	if( IsPrentice( guid ) == FALSE )
	{
		Assert( IsPrentice( guid ) );
		return FALSE;
	}

	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_PRENTICE, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_PRENTICE) )
	{ // 没有这个朋友或他不是徒弟
		AssertEx(FALSE, "对不起，只有对徒弟可以执行此操作。");
		return FALSE;
	}

	// 将好友的类型设为普通朋友
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_FRIEND );
	m_pHuman->GetDB()->DelPrentice( guid );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 判断是否是结拜兄弟
BOOL HumanRelation::IsBrother( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert( guid != INVALID_ID );
		return FALSE;
	}

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	// Not Accomplished.
	// return TRUE; // (pRelationDB->m_MasterInfo.m_MasterGUID == guid);

__LEAVE_FUNCTION

	return FALSE;
}

// 给所有好友发送邮件
VOID HumanRelation::SendMailToAllFriend( const CHAR* szMail )
{
__ENTER_FUNCTION

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	GWBatchMail* pMsg = (GWBatchMail*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_BATCHMAIL);
	BATCH_MAIL* pBatchMail = pMsg->GetBatchMail();

	pBatchMail->SetGUID( m_pHuman->GetGUID() );
	pBatchMail->SetSourName( m_pHuman->GetName() );

	for( INT i=RELATION_FRIEND_OFFSET; i<RELATION_BLACKNAME_OFFSET; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID != INVALID_ID )
		{
			pBatchMail->AddDestName( pRelationList[i].m_Member.m_szMemberName );
		}
	}

	pBatchMail->SetMailContent( szMail );
	pBatchMail->SetMailFlag( MAIL_TYPE_NORMAL2 );
	pBatchMail->SetCreateTime( g_pTimeManager->GetANSITime() );
	pBatchMail->SetSourCamp( m_pHuman->GetCampData()->m_nCampID );

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

__LEAVE_FUNCTION
}

const _RELATION_DB_LOAD* HumanRelation::GetRelationDB( )
{
__ENTER_FUNCTION

	if( m_pHuman == NULL )
	{
		Assert(FALSE);
		return NULL;
	}

	return m_pHuman->GetDB()->GetRelationDB();

__LEAVE_FUNCTION

	return NULL;
}

const _OWN_RELATION* HumanRelation::GetRelationList( )
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return pRelationDB->m_aRelation;

__LEAVE_FUNCTION

	return NULL;
}

// 取得联系人的数据，返回指针以及 Index 值
const _OWN_RELATION* HumanRelation::GetRelation( RELATION_TYPE RelationType, GUID_t guid, INT& index )
{
__ENTER_FUNCTION

	INT nStart, nEnd;

	switch( RelationType )
	{
	case RELATION_TYPE_FRIEND:
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_MARRY:
	case RELATION_TYPE_MASTER:
	case RELATION_TYPE_PRENTICE:
		{
			nStart = RELATION_FRIEND_OFFSET;
			nEnd = RELATION_BLACKNAME_OFFSET;
		}
		break;
	case RELATION_TYPE_BLACKNAME:
		{
			nStart = RELATION_BLACKNAME_OFFSET;
			nEnd = MAX_RELATION_SIZE;
		}
		break;
	case RELATION_TYPE_TEMPFRIEND:
	case RELATION_TYPE_STRANGER:
		{
			Assert(FALSE);
			return NULL;
		}
		break;
	default:
		Assert(FALSE);
		return NULL;
	}

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=nStart; i<nEnd; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			index = i;
			return &pRelationList[i];
		}
	}

__LEAVE_FUNCTION

	return NULL;
}


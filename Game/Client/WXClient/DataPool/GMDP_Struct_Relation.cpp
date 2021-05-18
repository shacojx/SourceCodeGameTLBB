#include "StdAfx.h"
#include "GMDataPool.h"
#include "GMDP_Struct_Relation.h"
#include "GMDP_Struct_Mail.h"
#include "GMDP_CharacterData.h"
#include "..\Object\ObjectManager.h"
#include "../Object/Character/Obj_PlayerMySelf.h"

#include "../Event/GMEventSystem.h"
#include "../Procedure/GameProcedure.h"
#include "TDWorldSystem.h"
#include "../DBC/GMDataBase.h"
VOID SDATA_RELATION_MEMBER::CleanUp()
{
	m_GUID = INVALID_ID;
	memset((void*)m_szName, 0, sizeof(m_szName));

	m_RelationType = RELATION_TYPE_NONE;
	m_nFriendPoint = 0;

	m_nLevel = 0;
	m_nMenPai = 9;
	m_nPortrait = -1;
	m_GuildID = INVALID_ID;
	memset((void*)m_szGuildName, 0, sizeof(m_szGuildName));
	m_bOnlineFlag = FALSE;
	sprintf( m_szMood, "" );
	memset((void*)m_szTitle, 0, sizeof(m_szTitle));
	m_SceneID = INVALID_ID;
	memset((void*)m_szRelation, 0, sizeof(m_szRelation));
	memset((void*)m_szLocation, 0, sizeof(m_szLocation));
	m_nTeamSize = 0;
	memset((void*)m_szTeamDesc, 0, sizeof(m_szTeamDesc));
	m_nEnterOrder = 0;
}

RelationList::~RelationList()
{
	CleanUp();
}

VOID RelationList::CleanUp()
{
	for(INT i = 0; i < (INT)m_vecRelationMember.size(); ++i)
	{
		SAFE_DELETE(m_vecRelationMember[i]);
	}

	m_vecRelationMember.clear();
}

// 加入名单
BOOL RelationList::Add( SDATA_RELATION_MEMBER* pMember )
{
	Assert(pMember);

	BOOL bAddedFlag = FALSE;

	if ( m_vecRelationMember.size() >= LIST_MEMBER_COUNT )
	{ // 人数太多
		return FALSE;
	}

	if ( !IsFitType( pMember ) )
	{ // 不应该放到这类名单里
		return FALSE;
	}

	for( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{ // 按拼音顺序插入现有名单
		INT nRet;

		nRet = Compare(pMember, m_vecRelationMember[i]);
		if ( nRet < 0 )
		{ // 优先级低
			continue;
		}
		else
		{
			m_vecRelationMember.insert(m_vecRelationMember.begin() + i, pMember);
			bAddedFlag = TRUE;
			break;
		}
	}

	if ( !bAddedFlag )
	{
		m_vecRelationMember.push_back(pMember);
	}

	return TRUE;
}

// 从名单里移除，只移除指针，不释放数据
BOOL RelationList::Remove( INT idx )
{
	if ( (INT)m_vecRelationMember.size() <= idx || idx < 0 )
	{
		Assert( idx );
		return FALSE;
	}

	m_vecRelationMember.erase( m_vecRelationMember.begin() + idx );

	return TRUE;
}

// 从名单里移除，只移除指针，不释放数据
BOOL RelationList::RemoveByGUID( GUID_t guid )
{
	INT idx;
	idx = GetMemberIndex( guid );

	if ( idx == -1 )
	{
		return FALSE;
	}

	return Remove( idx );
}

// 从名单里移除，并释放数据
BOOL RelationList::Erase( INT idx )
{
	if ( (INT)m_vecRelationMember.size() <= idx || idx < 0 )
	{
		Assert( idx );
		return FALSE;
	}

	SAFE_DELETE( m_vecRelationMember[idx] );
	m_vecRelationMember.erase( m_vecRelationMember.begin() + idx );

	return TRUE;
}

// 从名单里移除，并释放数据
BOOL RelationList::EraseByGUID( GUID_t guid )
{
	INT idx;
	idx = GetMemberIndex( guid );

	if ( idx == -1 )
	{
		return FALSE;
	}

	return Erase( idx );
}

// 得到当前名单人数
INT RelationList::GetMemberCount()
{
	return (INT)m_vecRelationMember.size();
}

// 得到一个关系数据（不可修改数据）
const SDATA_RELATION_MEMBER* RelationList::GetMember( INT idx )
{
	if ( idx < 0 || idx >= (INT)m_vecRelationMember.size() )
	{
		Assert( FALSE );
		return NULL;
	}

	return m_vecRelationMember[idx];
}

// 得到一个关系数据（可以修改数据）
SDATA_RELATION_MEMBER* RelationList::GetMemberByGUID( GUID_t guid )
{
	for( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{
		if ( m_vecRelationMember[i]->m_GUID == guid )
		{
			return m_vecRelationMember[i];
		}
	}

	return NULL;
}

// 通过名字得到一个关系数据（可以修改数据）
SDATA_RELATION_MEMBER* RelationList::GetMemberByName( const CHAR* szName )
{
	for( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{
		if ( strcmp(m_vecRelationMember[i]->m_szName, szName) == 0 )
		{
			return m_vecRelationMember[i];
		}
	}

	return NULL;
}

// 通过名字得到一个关系数据（可以修改数据）
INT RelationList::GetMemberIndexByName( const CHAR* szName )
{
	for( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{
		if ( strcmp(m_vecRelationMember[i]->m_szName, szName) == 0 )
		{
			return i;
		}
	}

	return -1;
}

// 判断当前名单是否已满
BOOL RelationList::IsFull()
{
	return (m_vecRelationMember.size() >= LIST_MEMBER_COUNT);
}

// 得到某个 GUID 在名单中的位置，如果不存在，则返回 -1
INT RelationList::GetMemberIndex(GUID_t guid)
{
	for( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{
		if ( m_vecRelationMember[i]->m_GUID == guid )
		{
			return i;
		}
	}

	return -1;
}

// 返回值：CSTR_LESS_THAN, CSTR_EQUAL, CSTR_GREATER_THAN,
// 错误时返回 0
INT RelationList::CompareByName(const CHAR* szName1, const CHAR* szName2)
{
	Assert(szName1);
	Assert(szName2);

	return CompareString(LOCALE_SYSTEM_DEFAULT, 0,
		szName1, (int)strlen(szName1),
		szName2, (int)strlen(szName2) );
}

// 比较两个关系人的优先排放顺序（<0表示优先级低，0表示相等，>0表示优先级高）
INT RelationList::Compare(const SDATA_RELATION_MEMBER* pMember1, const SDATA_RELATION_MEMBER* pMember2)
{
	INT nRet;

	nRet = CompareByName( pMember1->m_szName, pMember2->m_szName );

	switch(nRet)
	{ // 名字越大优先级越低
	case CSTR_LESS_THAN:
		return 1;
	case CSTR_EQUAL:
		return 0;
	case CSTR_GREATER_THAN:
		return -1;
	default:
		AssertEx( nRet, "Error occurred when compare name." );
		return -1;
	}
}

// CGameProcedure::s_pTimeSystem->GetTimeNow()

// 判断是否名单需要的类型
BOOL FriendList::IsFitType( SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );

	return (pMember->m_RelationType == RELATION_TYPE_FRIEND)
		|| (pMember->m_RelationType == RELATION_TYPE_BROTHER)
		|| (pMember->m_RelationType == RELATION_TYPE_MARRY)
		|| (pMember->m_RelationType == RELATION_TYPE_MASTER)
		|| (pMember->m_RelationType == RELATION_TYPE_PRENTICE);
}

// 计算一个好友的优先级，用于排序，目前仅用于 Compare 使用，所以返回值不定义
INT FriendList::CalcPriority( const SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );

	if( pMember->m_RelationType == RELATION_TYPE_MARRY )
	{
		return 10;
	}
	else if( pMember->m_RelationType == RELATION_TYPE_BROTHER )
	{
		return 9;
	}
	else if( pMember->m_nFriendPoint >= FRIEND_POINT_ENOUGH_NOTIFY )
	{ // 亲密好友
		if( pMember->m_bOnlineFlag )
		{ // 如果在线
			return 8;
		}
		else
		{
			return 7;
		}
	}
	else
	{
		return 6;
	}
}

// 比较两个关系人的优先排放顺序（<0表示优先级低，0表示相等，>0表示优先级高）
INT FriendList::Compare(const SDATA_RELATION_MEMBER* pMember1, const SDATA_RELATION_MEMBER* pMember2)
{
	Assert( pMember1 );
	Assert( pMember2 );

	INT nPriority1, nPriority2;

	nPriority1 = CalcPriority( pMember1 );
	nPriority2 = CalcPriority( pMember2 );

	if( nPriority1 > nPriority2 )
	{
		return 1;
	}
	else if( nPriority1 < nPriority2 )
	{
		return -1;
	}
	else
	{
		return RelationList::Compare( pMember1, pMember2 );
	}
}

// 加入名单
BOOL TempRelationList::Add( SDATA_RELATION_MEMBER* pMember )
{
	if ( m_vecRelationMember.size() < 1 )
	{ // 初始化最大 Order
		m_nMaxOrder = 0;
	}

	while ( m_vecRelationMember.size() >= LIST_MEMBER_COUNT )
	{ // 人数太多
		INT nSize;

		nSize = (INT)m_vecRelationMember.size();
		for( INT i=0; i<nSize; ++i )
		{
			if ( m_vecRelationMember[i]->m_nEnterOrder == 1 )
			{
				m_vecRelationMember.erase(m_vecRelationMember.begin()+i);
				--m_nMaxOrder;
			}
			else
			{
				--(m_vecRelationMember[i]->m_nEnterOrder);
			}
		}
	}

	pMember->m_nEnterOrder = ++m_nMaxOrder; // 所以最小 Order 为 1

	return RelationList::Add( pMember );
}

// 判断是否名单需要的类型
BOOL TempRelationList::IsFitType( SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );
	return (pMember->m_RelationType == RELATION_TYPE_TEMPFRIEND);
}

// 判断是否名单需要的类型
BOOL BlackList::IsFitType( SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );
	return (pMember->m_RelationType == RELATION_TYPE_BLACKNAME);
}

// 从服务器端接收一个好友
// 将此好友根据 _FRIEND_INFO 中的信息在相应组增加一个好友
// 可以在好友和黑名单都加完了以后统一将刷新界面的事件发出去
BOOL Relation::AddRelation( const _FRIEND_INFO* pFriend )
{
	Assert( pFriend );

	RelationList* pRelationList;

	switch( pFriend->m_uGroup )
	{
	case RELATION_GROUP_F1:
	case RELATION_GROUP_F2:
	case RELATION_GROUP_F3:
	case RELATION_GROUP_F4:
		pRelationList = GetRelationList( (RELATION_GROUP)pFriend->m_uGroup );
		break;
	default:
		Assert( FALSE );
		return FALSE;
	}

	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pNewRelation;
	pNewRelation = pRelationList->GetMemberByGUID( pFriend->m_GUID );

	if( pNewRelation != NULL )
	{
		strncpy( pNewRelation->m_szName, pFriend->m_szTargetName, sizeof(pNewRelation->m_szName) - 1 );
		pNewRelation->m_RelationType = (RELATION_TYPE)pFriend->m_uRelationType;
		pNewRelation->m_nFriendPoint = pFriend->m_nFriendpoint;
		return TRUE;
	}
	else
	{
		pNewRelation = new SDATA_RELATION_MEMBER;

		if( pNewRelation == NULL )
		{
			Assert( pNewRelation );
			return FALSE;
		}

		pNewRelation->m_GUID = pFriend->m_GUID;
		strncpy( pNewRelation->m_szName, pFriend->m_szTargetName, sizeof(pNewRelation->m_szName) - 1 );
		pNewRelation->m_RelationType = (RELATION_TYPE)pFriend->m_uRelationType;
		pNewRelation->m_nFriendPoint = pFriend->m_nFriendpoint;
		return pRelationList->Add(pNewRelation);
	}
}

// 从服务器端接收一个黑名单玩家
// 将此信息在黑名单中增加一个玩家信息
// 可以在好友和黑名单都加完了以后统一将刷新界面的事件发出去
BOOL Relation::AddRelation( const _BLACKNAME_INFO* pBlackName )
{
	Assert( pBlackName );

	RelationList* pRelationList;

	pRelationList = GetRelationList(RELATION_GROUP_BLACK);
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pNewRelation;
	pNewRelation = pRelationList->GetMemberByGUID( pBlackName->m_GUID );

	if( pNewRelation != NULL )
	{
		strncpy( pNewRelation->m_szName, pBlackName->m_szTargetName, sizeof(pNewRelation->m_szName) - 1 );
		pNewRelation->m_RelationType = RELATION_TYPE_BLACKNAME;
		return TRUE;
	}
	else
	{
		pNewRelation = new SDATA_RELATION_MEMBER;

		if( pNewRelation == NULL )
		{
			Assert( pNewRelation );
			return FALSE;
		}

		pNewRelation->m_GUID = pBlackName->m_GUID;
		strncpy( pNewRelation->m_szName, pBlackName->m_szTargetName, sizeof(pNewRelation->m_szName) - 1 );
		pNewRelation->m_RelationType = RELATION_TYPE_BLACKNAME;
		return pRelationList->Add(pNewRelation);
	}
}

// 获得一个名单的人数
INT Relation::GetListCount( RELATION_GROUP RelationGroup )
{
	RelationList* pRelationList;

	pRelationList = GetRelationList(RelationGroup);
	if ( pRelationList == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return pRelationList->GetMemberCount();
}

// 客户端获得关系人信息，供界面使用
const SDATA_RELATION_MEMBER* Relation::GetRelationInfo( RELATION_GROUP RelationGroup, INT idx )
{
	RelationList* pRelationList;

	pRelationList = GetRelationList(RelationGroup);
	if ( pRelationList == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return pRelationList->GetMember( idx );
}

// 客户端加入一个关系，通常用于加入临时好友，加入时需要指定将要加入的组
BOOL Relation::AddRelation( RELATION_GROUP RelationGroup, SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );

	RelationList* pRelationList;

	pRelationList = GetRelationList(RelationGroup);
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	// 不让重复加好友，不能加自己为好友
	if( pMember->m_GUID != INVALID_ID )
	{
		if( CObjectManager::GetMe()->GetMySelf()->GetServerGUID() == pMember->m_GUID )
		{
			return FALSE;
		}

		RELATION_GROUP rg;
		INT idx;

		if( GetRelationPosition( pMember->m_GUID, rg, idx ) != RELATION_TYPE_STRANGER )
		{
			return FALSE;
		}
		else if( pRelationList->GetMemberByGUID(pMember->m_GUID) != NULL )
		{
			return FALSE;
		}
	}
	else if( strlen(pMember->m_szName) > 0 )
	{
		std::string str1 = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name();
		std::string str2 = pMember->m_szName;
		if( str1 == str2 )
		{
			return FALSE;
		}

		INT nrg, idx;

		if( GetRelationByName(pMember->m_szName, nrg, idx ) != RELATION_TYPE_STRANGER )
		{
			return FALSE;
		}
		else if( pRelationList->GetMemberByName(pMember->m_szName) != NULL )
		{
			return FALSE;
		}
	}

	SDATA_RELATION_MEMBER* pNewRelation = new SDATA_RELATION_MEMBER;
	if( pNewRelation == NULL )
	{
		Assert( pNewRelation );
		return FALSE;
	}

	pNewRelation->m_GUID = pMember->m_GUID;
	strncpy( pNewRelation->m_szName, pMember->m_szName, sizeof(pNewRelation->m_szName) - 1 );
	pNewRelation->m_RelationType = pMember->m_RelationType;
	pNewRelation->m_nFriendPoint = pMember->m_nFriendPoint;
	pNewRelation->m_nLevel = pMember->m_nLevel;
	pNewRelation->m_nMenPai = pMember->m_nMenPai;
	pNewRelation->m_nPortrait = pMember->m_nPortrait;
	pNewRelation->m_GuildID = pMember->m_GuildID;
	strncpy( pNewRelation->m_szGuildName, pMember->m_szGuildName, sizeof(pNewRelation->m_szGuildName) - 1 );
	pNewRelation->m_bOnlineFlag = pMember->m_bOnlineFlag;
	SetRelationDesc( pNewRelation );

	if( pMember->m_bOnlineFlag )
	{
		strncpy( pNewRelation->m_szMood, pMember->m_szMood, sizeof(pNewRelation->m_szMood) - 1 );
		strncpy( pNewRelation->m_szTitle,  TransTitleStr(pMember->m_szTitle), sizeof(pNewRelation->m_szTitle) - 1 );
		pNewRelation->m_SceneID = pMember->m_SceneID;
		GetLocationName( pNewRelation->m_SceneID, pNewRelation->m_szLocation );
		pNewRelation->m_nTeamSize = pMember->m_nTeamSize;
		SetTeamDesc( pNewRelation );
	}
	else
	{
		strcpy( pNewRelation->m_szLocation, "离线" );
	}

	pRelationList->Add(pNewRelation);
	UpdateUIList( RelationGroup );

	return TRUE;
}

// 服务器端发消息来更新关系人信息
BOOL Relation::UpdateRelationInfo( RETURN_RELATION_INFO* pRelationInfo )
{
	Assert( pRelationInfo );

	RelationList* pRelationList;
	RELATION_GROUP grp;

	switch( pRelationInfo->GetRelationType() )
	{
	case RELATION_TYPE_BLACKNAME:
		grp = RELATION_GROUP_BLACK;
		break;
	case RELATION_TYPE_TEMPFRIEND:
		grp = RELATION_GROUP_TEMPFRIEND;
		break;
	default:
		grp = (RELATION_GROUP)pRelationInfo->GetGroup();
	}

	_RELATION* pRelationData;
	pRelationData = pRelationInfo->GetRelationData();

	pRelationList = GetRelationList( grp );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	GUID_t guid = pRelationData->GetGUID();
	SDATA_RELATION_MEMBER* pMember;
	pMember = pRelationList->GetMemberByGUID( guid );
	if( pMember == NULL )
	{
		pMember = pRelationList->GetMemberByName( pRelationData->GetName() );
	}
	if( pMember == NULL )
	{ // 陌生人，查看玩家
		m_ViewPlayer.m_GUID = pRelationData->GetGUID();
		strncpy( m_ViewPlayer.m_szName, pRelationData->GetName(), sizeof(m_ViewPlayer.m_szName) - 1 );
		m_ViewPlayer.m_RelationType = (RELATION_TYPE)pRelationInfo->GetRelationType();
		m_ViewPlayer.m_nFriendPoint = pRelationInfo->GetFriendPoint();
		m_ViewPlayer.m_nLevel = pRelationData->GetLevel();
		m_ViewPlayer.m_nMenPai = pRelationData->GetMenPai();
		m_ViewPlayer.m_nPortrait = pRelationData->GetPortrait();
		m_ViewPlayer.m_GuildID = pRelationData->GetGuildID();
		strncpy( m_ViewPlayer.m_szGuildName, pRelationData->GetGuildName(), sizeof(m_ViewPlayer.m_szGuildName) - 1 );
		m_ViewPlayer.m_bOnlineFlag = (BOOL)pRelationData->GetOnlineFlag();
		//		SetRelationDesc( &m_ViewPlayer );

		if( m_ViewPlayer.m_bOnlineFlag )
		{
			strncpy( m_ViewPlayer.m_szMood, pRelationData->GetMood(), sizeof(m_ViewPlayer.m_szMood) - 1 );
			strncpy( m_ViewPlayer.m_szTitle,  TransTitleStr(pRelationData->GetTitle()), sizeof(m_ViewPlayer.m_szTitle) - 1 );
			m_ViewPlayer.m_SceneID = pRelationData->GetSceneID();
			GetLocationName( m_ViewPlayer.m_SceneID, m_ViewPlayer.m_szLocation );
			m_ViewPlayer.m_nTeamSize = pRelationData->GetTeamSize();
			SetTeamDesc( &m_ViewPlayer );
		}

		// TODO: Push_Event here
		CEventSystem::GetMe()->PushEvent( GE_CHAT_SHOWUSERINFO);
		return TRUE;
	}

	pMember->m_GUID = pRelationData->GetGUID();
	strncpy( pMember->m_szName, pRelationData->GetName(), sizeof(pMember->m_szName) - 1 );
	pMember->m_RelationType = (RELATION_TYPE)pRelationInfo->GetRelationType();
	pMember->m_nFriendPoint = pRelationInfo->GetFriendPoint();
	pMember->m_nLevel = pRelationData->GetLevel();
	pMember->m_nMenPai = pRelationData->GetMenPai();
	pMember->m_nPortrait = pRelationData->GetPortrait();
	pMember->m_GuildID = pRelationData->GetGuildID();
	strncpy( pMember->m_szGuildName, pRelationData->GetGuildName(), sizeof(pMember->m_szGuildName) - 1 );
	pMember->m_bOnlineFlag = (BOOL)pRelationData->GetOnlineFlag();
	SetRelationDesc( pMember );

	if( pMember->m_bOnlineFlag )
	{
		strncpy( pMember->m_szMood, pRelationData->GetMood(), sizeof(pMember->m_szMood) - 1 );
		strncpy( pMember->m_szTitle,  TransTitleStr(pRelationData->GetTitle()), sizeof(pMember->m_szTitle) - 1 );
		pMember->m_SceneID = pRelationData->GetSceneID();
		GetLocationName( pMember->m_SceneID, pMember->m_szLocation );
		pMember->m_nTeamSize = pRelationData->GetTeamSize();
		SetTeamDesc( pMember );
	}
	else
	{
		strcpy( pMember->m_szLocation, "离线" );
	}

	UpdateUIInfo( grp, pRelationList->GetMemberIndex(guid) );
	return TRUE;
}

// 客户端移除一个关系，通常用于移除临时好友
BOOL Relation::RemoveRelation( RELATION_GROUP RelationGroup, INT idx )
{
	RelationList* pRelationList;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	if( idx < 0 || idx >= pRelationList->GetMemberCount() )
	{
		Assert( FALSE );
		return FALSE;
	}

	pRelationList->Erase( idx );
	UpdateUIList( RelationGroup );

	return TRUE;
}

// 客户端移除一个关系，通常用于移除临时好友
BOOL Relation::RemoveRelation( RELATION_GROUP RelationGroup, const CHAR* szName )
{
	Assert(szName);

	RelationList* pRelationList;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	INT nIndex;
	nIndex = pRelationList->GetMemberIndexByName(szName);
	if( nIndex >= 0 )
	{
		RemoveRelation( RelationGroup, nIndex );
		return TRUE;
	}

	return FALSE;
}

// 从服务器端收到一条移除关系人的消息
BOOL Relation::RemoveRelation( GUID_t guid )
{
	RELATION_GROUP RelationGroup;
	INT idx;

	if( GetRelationPosition( guid, RelationGroup, idx ) != RELATION_TYPE_STRANGER )
	{
		return RemoveRelation( RelationGroup, idx );
	}

	return FALSE;
}

// 移动一个关系，Srg 是原来的组，guid 是要移动的玩家 GUID，Drg 是移动后的组
BOOL Relation::MoveRelation( RELATION_GROUP Srg, RELATION_TYPE Dtype, RELATION_GROUP Drg, GUID_t guid )
{
	RelationList* pSRelationList;

	pSRelationList = GetRelationList(Srg);
	if ( pSRelationList == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	RelationList* pDRelationList;

	pDRelationList = GetRelationList(Drg);
	if ( pDRelationList == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pMember;
	pMember = pSRelationList->GetMemberByGUID( guid );
	if ( pMember == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	if ( pSRelationList->RemoveByGUID( guid ) )
	{
		UpdateUIList( Srg );

		// 修改关系信息
		pMember->m_RelationType = Dtype;

		if ( pDRelationList->Add( pMember ) )
		{
			UpdateUIList( Drg );
			return TRUE;
		}
		else
		{
			SAFE_DELETE( pMember );
			Assert(FALSE);
			return FALSE;
		}
	}

	return FALSE;
}

// 通知 UI 刷新列表，默认更新所有列表
VOID Relation::UpdateUIList( RELATION_GROUP RelationGroup )
{
	CEventSystem::GetMe()->PushEvent( GE_UPDATE_FRIEND );
}

// 通知 UI 刷新 RelationGroup 中第 idx 号关系人的详细信息
VOID Relation::UpdateUIInfo( RELATION_GROUP RelationGroup, INT idx )
{
	CEventSystem::GetMe()->PushEvent( GE_UPDATE_FRIEND_INFO, int( RelationGroup ), idx );
}

// 得到一个 GUID 所在的组以及在组里的索引，没有找到返回 RELATION_TYPE_STRANGER，这个功能不考虑临时好友
RELATION_TYPE Relation::GetRelationPosition( GUID_t guid, RELATION_GROUP& RelationGroup, INT& idx )
{
	RelationList* pRelationList;

	for( INT i = (INT)RELATION_GROUP_F1; i < (INT)RELATION_GROUP_NUMBER; ++i )
	{
		pRelationList = GetRelationList( (RELATION_GROUP)i );
		if( pRelationList == NULL )
		{
			Assert( pRelationList );
			return RELATION_TYPE_STRANGER;
		}

		idx = pRelationList->GetMemberIndex(guid);
		if( idx != -1 )
		{
			RelationGroup = (RELATION_GROUP)i;
			return pRelationList->GetMember(idx)->m_RelationType;
		}
	}

	return RELATION_TYPE_STRANGER;
}

RELATION_TYPE Relation::GetRelationByName( const CHAR* szName, INT& RelationGroup, INT& idx )
{
	RelationList* pRelationList;

	for( INT i = (INT)RELATION_GROUP_F1; i < (INT)RELATION_GROUP_NUMBER; ++i )
	{
		pRelationList = GetRelationList( (RELATION_GROUP)i );
		if( pRelationList == NULL )
		{
			Assert( pRelationList );
			return RELATION_TYPE_STRANGER;
		}

		idx = pRelationList->GetMemberIndexByName( szName );
		if( idx != -1 )
		{
			RelationGroup = i;
			return pRelationList->GetMember(idx)->m_RelationType;
		}
	}

	return RELATION_TYPE_STRANGER;
}

// 更新在线好友
BOOL Relation::UpdateOnlineFriend( const _RELATION_ONLINE* pOnlineRelation )
{
	Assert( pOnlineRelation );

	RELATION_GROUP RelationGroup;
	INT idx;
	GUID_t guid;

	guid = pOnlineRelation->GetGUID();
	if( GetRelationPosition( guid, RelationGroup, idx ) == RELATION_TYPE_STRANGER )
	{ // 数据池还为空
		return TRUE;
	}

	RelationList* pRelationList;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pMember;
	pMember = pRelationList->GetMemberByGUID( guid );
	if( pMember == NULL )
	{
		Assert( pMember );
		return FALSE;
	}

	pMember->m_bOnlineFlag = TRUE;
	strncpy( pMember->m_szMood, pOnlineRelation->GetMood(), sizeof(pMember->m_szMood) - 1 );

	UpdateUIList( RelationGroup ); // 刷新关系人在线状态
	return TRUE;
}

// 关系人上线
BOOL Relation::RelationOnline(const CHAR* szName, const CHAR* szMood)
{
	INT nGroup;
	INT idx;

	Assert(szName);
	Assert(szMood);

	if(szName)
	{
		char szText[_MAX_PATH];
		_snprintf(szText, _MAX_PATH, "你的好友#Y%s#W进入游戏了", szName);
		ADDTALKMSG(szText);
	}

	if( GetRelationByName( szName, nGroup, idx ) == RELATION_TYPE_STRANGER )
	{ // 数据池还为空
		return TRUE;
	}

	RELATION_GROUP RelationGroup;
	RelationList* pRelationList;

	RelationGroup = (RELATION_GROUP)nGroup;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pMember;
	pMember = pRelationList->GetMemberByName( szName );
	if( pMember == NULL )
	{
		Assert( pMember );
		return FALSE;
	}

	pMember->m_bOnlineFlag = TRUE;
	strncpy( pMember->m_szMood, szMood, sizeof(pMember->m_szMood) - 1 );

	UpdateUIList( RelationGroup ); // 刷新关系人在线状态
	return TRUE;
}

// 关系人下线
BOOL Relation::RelationOffLine(GUID_t guid)
{
	RELATION_GROUP RelationGroup;
	INT idx;

	GetRelationPosition( guid, RelationGroup, idx );

	RelationList* pRelationList;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pMember;
	pMember = pRelationList->GetMemberByGUID( guid );
	if( pMember == NULL )
	{
		Assert( pMember );
		return FALSE;
	}

	pMember->m_bOnlineFlag = FALSE;
	strncpy( pMember->m_szMood, "", sizeof(pMember->m_szMood) - 1 );

	char szText[_MAX_PATH];
	_snprintf(szText, _MAX_PATH, "你的好友#Y%s#W离开游戏了", pMember->m_szName);
	ADDTALKMSG(szText);

	UpdateUIList( RelationGroup ); // 刷新关系人在线状态
	return TRUE;
}

// 判断是否在自己黑名单中
BOOL Relation::IsBlackName( const CHAR* szName )
{
	Assert( szName );

	for( INT i=0; i<m_BlackListGroup.GetMemberCount(); ++i )
	{
		if ( strncmp( m_BlackListGroup.GetMember(i)->m_szName, szName, strlen(szName) ) == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

// 给某个好友增加一条邮件（邮件池中的索引号）
BOOL Relation::AddMail( const CHAR* szName, UINT uIndex )
{
	INT aInt[] = {RELATION_GROUP_F1, RELATION_GROUP_F2, RELATION_GROUP_F3, RELATION_GROUP_F4, RELATION_GROUP_TEMPFRIEND};
	RelationList* pRelationList = NULL;
	SDATA_RELATION_MEMBER* pRelationMember = NULL;

	for( INT i = 0; i < sizeof(aInt)/sizeof(INT); ++i )
	{
		pRelationList = GetRelationList( (RELATION_GROUP)aInt[i] );
		if( pRelationList == NULL )
		{
			Assert( pRelationList );
			return FALSE;
		}

		pRelationMember = pRelationList->GetMemberByName(szName);
		if( pRelationMember != NULL )
		{
			break;
		}
	}

	if( pRelationMember == NULL )
	{ // 没有找到这个好友
		return FALSE;
	}

	pRelationMember->m_vMailList.push_back(uIndex);
	return TRUE;
}

// 得到某个好友的历史邮件数量
UINT Relation::GetMailCount( RELATION_GROUP RelationGroup, INT idx )
{
	const SDATA_RELATION_MEMBER* pRelationMember;

	pRelationMember = GetRelationInfo( RelationGroup, idx );
	if( pRelationMember == NULL )
	{
		Assert(pRelationMember);
		return 0;
	}

	return (UINT)pRelationMember->m_vMailList.size();
}

// 得到某个好友的第 uIndex 封历史邮件的邮件池索引号
UINT Relation::GetMailIndex( RELATION_GROUP RelationGroup, INT idx, UINT uIndex )
{
	const SDATA_RELATION_MEMBER* pRelationMember;

	pRelationMember = GetRelationInfo( RelationGroup, idx );
	if( pRelationMember == NULL )
	{
		Assert(pRelationMember);
		return 0;
	}

	if( pRelationMember->m_vMailList.size() <= uIndex )
	{
		Assert( pRelationMember->m_vMailList.size() > uIndex );
		return 0;
	}

	return pRelationMember->m_vMailList[uIndex];
}

// 给某个（临时）好友重建历史信息
VOID Relation::RebuildMailHistory( RELATION_GROUP RelationGroup, const CHAR* szName )
{
	RelationList* pRelationList = NULL;
	SDATA_RELATION_MEMBER* pRelationMember = NULL;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return;
	}

	pRelationMember = pRelationList->GetMemberByName(szName);
	if( pRelationMember == NULL )
	{ // 没有找到这个好友
		return;
	}

	CMailPool* pMailPool = CDataPool::GetMe()->GetMailPool();

	for( INT i=0; i<pMailPool->GetMailCount(); ++i )
	{
		const SMail* pMail;

		pMail = pMailPool->GetMail(i);
		if( pMail != NULL )
		{
			if( strcmp(pMail->m_szSourName, pRelationMember->m_szName) == 0
			 || strcmp(pMail->m_szDestName, pRelationMember->m_szName) == 0
			 )
			{
				pRelationMember->m_vMailList.push_back(i);
			}
		}
	}
}

// 得到心情
const CHAR* Relation::GetMood( )
{
	return m_szMood;
}

// 设置心情
VOID Relation::SetMood( const CHAR* szMood )
{
	if( szMood == NULL )
	{
		Assert( szMood );
		return;
	}

	strncpy( m_szMood, szMood, sizeof(m_szMood)-1 );
	CEventSystem::GetMe()->PushEvent( GE_MOOD_CHANGE );

}

// 查看玩家界面的信息
const SDATA_RELATION_MEMBER* Relation::GetPlayerCheckInfo( )
{
	return &m_ViewPlayer;
}

// 获得一个名单
RelationList* Relation::GetRelationList( RELATION_GROUP RelationGroup )
{
	switch( RelationGroup )
	{
	case RELATION_GROUP_F1:
		{
			return &m_FriendGroup1;
		}
		break;
	case RELATION_GROUP_F2:
		{
			return &m_FriendGroup2;
		}
		break;
	case RELATION_GROUP_F3:
		{
			return &m_FriendGroup3;
		}
		break;
	case RELATION_GROUP_F4:
		{
			return &m_FriendGroup4;
		}
		break;
	case RELATION_GROUP_TEMPFRIEND:
		{
			return &m_TeamFriendGroup;
		}
		break;
	case RELATION_GROUP_BLACK:
		{
			return &m_BlackListGroup;
		}
		break;
	default:
		Assert(FALSE);
		return NULL;
	}

	return NULL;
}

// 根据场景 ID 获得场景名字，并存入 szSceneName
BOOL Relation::GetLocationName( SceneID_t sid, CHAR* szSceneName )
{
	Assert( szSceneName );

	if( sid != INVALID_ID )
	{
		strncpy( szSceneName, CGameProcedure::s_pWorldManager->GetSceneName(sid), LOCATION_DESC_SIZE - 1 );
	}
	else
	{
		strcpy( szSceneName, "未知场景" );
	}

	return TRUE;
}

// 根据关系类型或者友好度确定双方关系，并存入 m_szRelation
BOOL Relation::SetRelationDesc( SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );

	switch( pMember->m_RelationType )
	{
	case RELATION_TYPE_FRIEND:						//好友
		{
			if( pMember->m_nFriendPoint < 10 )
			{
				strcpy( pMember->m_szRelation, "一面之缘" );
			}
			else if( pMember->m_nFriendPoint <= 200 )
			{
				strcpy( pMember->m_szRelation, "泛泛之交" );
			}
			else if( pMember->m_nFriendPoint <= 500 )
			{
				strcpy( pMember->m_szRelation, "君子之交" );
			}
			else if( pMember->m_nFriendPoint <= 1000 )
			{
				strcpy( pMember->m_szRelation, "莫逆之交" );
			}
			else if( pMember->m_nFriendPoint > 1000 )
			{
				strcpy( pMember->m_szRelation, "刎颈之交" );
			}
			else
			{
				strcpy( pMember->m_szRelation, "普通朋友" );
			}
		}
		break;
	case RELATION_TYPE_BROTHER:						//结拜
		strcpy( pMember->m_szRelation, "金兰之好" );
		break;
	case RELATION_TYPE_MARRY:						//结婚
		strcpy( pMember->m_szRelation, "夫妻" );
		break;
	case RELATION_TYPE_BLACKNAME:					//黑名单
		strcpy( pMember->m_szRelation, "交恶" );
		break;
	case RELATION_TYPE_TEMPFRIEND:					//临时好友
		strcpy( pMember->m_szRelation, "临时好友" );
		break;
//	case RELATION_TYPE_MASTER:						//师傅关系
//	case RELATION_TYPE_PRENTICE:					//徒弟关系
	default:
		AssertEx( FALSE, "Unexpected Relation Type." );
		return FALSE;
	}

	return TRUE;
}

// 根据队伍人数设置组队显示数据，并存入 m_szTeamDesc
BOOL Relation::SetTeamDesc( SDATA_RELATION_MEMBER* pMember )
{
	if( pMember->m_nTeamSize == 0 )
	{
		strcpy( pMember->m_szTeamDesc, "未组队" );
	}
	else
	{
		sprintf( pMember->m_szTeamDesc, "已组队%3d人", pMember->m_nTeamSize );
	}

	return TRUE;
}

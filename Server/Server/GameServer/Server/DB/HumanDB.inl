#ifndef _HUMAN_DB_INL_
#define _HUMAN_DB_INL_




VOID				HumanDB::Lock()
{
	N_SM_LOCK(SM_S_WRITE)
}

VOID				HumanDB::UnLock()
{
	N_SM_UNLOCK(SM_S_WRITE)
}


GUID_t				HumanDB::GetGUID() const
{
	return m_dbHuman->m_GUID ; 
}
VOID				HumanDB::SetGUID( GUID_t guid )
{ 
	m_dbHuman->m_GUID = guid ; 
	SetDBEnumAttrDirty(CATTR_DB_GUID);
} 

INT					HumanDB::GetPortraitID() const
{
	return m_dbHuman->m_PortraitID ; 
}

VOID				HumanDB::SetPortraitID( INT nID )
{
	m_dbHuman->m_PortraitID = nID ; 
	SetDBEnumAttrDirty(CATTR_DB_PORTRAITID);
}

const	CHAR*		HumanDB::GetName()const
{
	return m_dbHuman->m_Name ;
} 
BOOL				HumanDB::GetSex()
{ 
	return m_dbHuman->m_Sex ; 
} 

INT					HumanDB::GetHP()
{
	return m_dbHuman->m_HP ; 
} 
VOID				HumanDB::SetHP( INT hp )
{
	m_dbHuman->m_HP = hp ; 
	SetDBEnumAttrDirty(CATTR_DB_HP);
} 

INT					HumanDB::GetMP()
{
	return m_dbHuman->m_MP ; 
} 
VOID				HumanDB::SetMP( INT mp )
{
	m_dbHuman->m_MP = mp ; 
	SetDBEnumAttrDirty(CATTR_DB_MP);
} 

INT					HumanDB::GetStrikePoint(VOID) 
{
	return m_dbHuman->m_StrikePoint;
}
VOID				HumanDB::SetStrikePoint(INT nStrikePoint) 
{
	m_dbHuman->m_StrikePoint = nStrikePoint;
	SetDBEnumAttrDirty(CATTR_DB_STRIKEPOINT);
}

INT					HumanDB::GetRMB(VOID)
{
	return m_dbHuman->m_RMBMoney;
}
VOID				HumanDB::SetRMB(INT nMoney)
{
	m_dbHuman->m_RMBMoney = nMoney;
	SetDBEnumAttrDirty(CATTR_DB_RMB);
}

INT					HumanDB::GetBankRMB(VOID)
{
	return m_dbHuman->m_BankRMB;
}
VOID				HumanDB::SetBankRMB(INT nMoney)
{
	m_dbHuman->m_BankRMB = nMoney;
	SetDBEnumAttrDirty(CATTR_DB_BANK_RMB);
}

//INT					HumanDB::GetVigorRegeneRate(VOID)
//{
//	return m_dbHuman->m_VigorRegeneRate;
//}
//VOID				HumanDB::SetVigorRegeneRate(INT nRate)
//{
//	m_dbHuman->m_VigorRegeneRate = nRate;
//	SetDBEnumAttrDirty(CATTR_DB_VIGOR_REGENE_RATE);
//}

INT					HumanDB::GetDoubleExpTime_Num(VOID)
{
	return m_dbHuman->m_DoubleExpTime_Num;
}
VOID				HumanDB::SetDoubleExpTime_Num(INT nExpTime_Num)
{
	m_dbHuman->m_DoubleExpTime_Num = nExpTime_Num;
	SetDBEnumAttrDirty(CATTR_DB_ENERGY);
}

INT					HumanDB::GetGmRight(VOID)
{
	return m_dbHuman->m_GmRight;
}
VOID				HumanDB::SetGmRight(INT nRight)
{
	m_dbHuman->m_GmRight = nRight;
	SetDBEnumAttrDirty(CATTR_DB_MAX_ENERGY);
}

//INT					HumanDB::GetExpTime_NumRegeneRate(VOID)
//{
//	return m_dbHuman->m_ExpTime_NumRegeneRate;
//}
//VOID				HumanDB::SetExpTime_NumRegeneRate(INT nRate)
//{
//	m_dbHuman->m_ExpTime_NumRegeneRate = nRate;
//	SetDBEnumAttrDirty(CATTR_DB_ExpTime_Num_REGENE_RATE);
//}

INT 				HumanDB::GetRage( VOID )
{ 
	return m_dbHuman->m_Rage; 
}
VOID				HumanDB::SetRage( INT nRage )
{
	m_dbHuman->m_Rage = nRage; 
	SetDBEnumAttrDirty(CATTR_DB_RAGE);
}

BYTE				HumanDB::GetLevel()
{
	return m_dbHuman->m_Level ; 
} 

VOID				HumanDB::SetLevel( BYTE level )
{ 
	m_dbHuman->m_Level = level ; 
	SetDBEnumAttrDirty(CATTR_DB_LEVEL);
}

INT					HumanDB::GetPKValue( VOID )const
{
	return m_dbHuman->m_nPKValue;
}

VOID				HumanDB::SetPKValue( INT nValue )
{
	m_dbHuman->m_nPKValue = nValue;
	SetDBEnumAttrDirty(CATTR_DB_PKVALUE);
}

PET_GUID_t			HumanDB::GetCurrentPetGUID( VOID )const
{
	return m_dbHuman->m_guidCurrentPet;
}

VOID				HumanDB::SetCurrentPetGUID( PET_GUID_t guid )
{
	m_dbHuman->m_guidCurrentPet = guid;
	SetDBEnumAttrDirty(CATTR_DB_CURRENTPET);
}

UINT				HumanDB::GetExp()
{
	return m_dbHuman->m_Exp ; 
} 
VOID				HumanDB::SetExp( UINT exp )
{
	m_dbHuman->m_Exp = exp ; 
	SetDBEnumAttrDirty(CATTR_DB_EXP);
} 

INT					HumanDB::GetMenPai( )
{ 
	return m_dbHuman->m_MenPai ; 
} 
VOID				HumanDB::SetMenPai( INT MenPai )
{
	m_dbHuman->m_MenPai = MenPai ; 
	SetDBEnumAttrDirty(CATTR_DB_MENPAI);
} 

UINT				HumanDB::GetMoney()
{	
	return m_dbHuman->m_Money;
}
VOID				HumanDB::SetMoney(UINT money)
{
	m_dbHuman->m_Money	=	money;
	SetDBEnumAttrDirty(CATTR_DB_MONEY);
}

_PLAYERSHOP_GUID	HumanDB::GetShopGuid(INT nIndex)
{
	return m_dbHuman->m_ShopGuid[nIndex];
}

VOID				HumanDB::SetShopGuid( INT nIndex, _PLAYERSHOP_GUID pGuid )
{
	m_dbHuman->m_ShopGuid[nIndex] = pGuid; 
	SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_PLAYER_SHOP_BEGIN + nIndex));
}

_PLAYERSHOP_GUID	HumanDB::GetFavoriteShopGuid(INT nIndex)
{
	return m_dbHuman->m_FavoriteList[nIndex];
}

VOID				HumanDB::SetFavoriteShopGuid( INT nIndex, _PLAYERSHOP_GUID pGuid )
{
	m_dbHuman->m_FavoriteList[nIndex] = pGuid; 
	SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_PLAYER_SHOP_FAVORITE_BEGIN + nIndex));
}

const CHAR*			HumanDB::GetPasswd()
{
	return m_dbHuman->m_Passwd;
}

VOID				HumanDB::SetPasswd( const CHAR* szPasswd )
{
	strncpy( m_dbHuman->m_Passwd, szPasswd, MAX_PWD - 1 );
	SetDBEnumAttrDirty(CATTR_DB_PASSWORD);
}

VOID				HumanDB::DelPasswd()
{
	memset( m_dbHuman->m_Passwd, 0, MAX_PWD );
	SetDBEnumAttrDirty(CATTR_DB_PASSWORD);
}

UINT				HumanDB::GetPasswdDeleteTime()
{
	return m_dbHuman->m_uPwdDelTime;
}

VOID				HumanDB::SetPasswdDeleteTime( UINT uTime )
{
	m_dbHuman->m_uPwdDelTime;
	SetDBEnumAttrDirty(CATTR_DB_PWDDELTIME);
}

//BYTE				HumanDB::GetXinfaLevel(INT nXinfaID)
//{
//	for(INT i=0; i<m_dbXinFa->m_Count; i++){
//		if( m_dbXinFa->m_aXinFa[i].m_nXinFaID == nXinfaID)
//			return m_dbXinFa->m_aXinFa[i].m_nLevel;
//	}
//	return -1;
//}

//VOID				HumanDB::SetXinfaUplevel(INT nXinfaID, BYTE UplevelNum)
//{
//	for(INT i=0; i<m_dbXinFa->m_Count; i++){
//		if( m_dbXinFa->m_aXinFa[i].m_nXinFaID == nXinfaID)
//			m_dbXinFa->m_aXinFa[i].m_nLevel = UplevelNum;
//	}
//
//	SetDBEnumAttrDirty(CATTR_DB_XINFA);
//}

UINT				HumanDB::GetHairColor(VOID)const
{
	return m_dbHuman->m_HairColor;
}

VOID				HumanDB::SetHairColor(UINT uColor)
{
	m_dbHuman->m_HairColor = uColor;
	SetDBEnumAttrDirty(CATTR_DB_HAIR_COLOR);
}

BYTE				HumanDB::GetFaceColor(VOID)const
{
	return m_dbHuman->m_FaceColor;
}

VOID				HumanDB::SetFaceColor(BYTE byColor)
{
	m_dbHuman->m_FaceColor = byColor;
	SetDBEnumAttrDirty(CATTR_DB_FACE_COLOR);
}

BYTE				HumanDB::GetHairModel(VOID)const
{
	return m_dbHuman->m_HairModel;
}

VOID				HumanDB::SetHairModel(BYTE byModel)
{
	m_dbHuman->m_HairModel = byModel;
	SetDBEnumAttrDirty(CATTR_DB_HAIR_MODEL);
}

BYTE				HumanDB::GetFaceModel(VOID)const
{
	return m_dbHuman->m_FaceModel;
}

VOID				HumanDB::SetFaceModel(BYTE byModel)
{
	m_dbHuman->m_FaceModel = byModel;
	SetDBEnumAttrDirty(CATTR_DB_FACE_MODEL);
}

UINT				HumanDB::GetEquipID(HUMAN_EQUIP	equipPoint)
{
	return m_dbEquip->m_pItem[equipPoint].m_ItemIndex;
}


INT					HumanDB::GetDBAttrLvl1(CHAR_ATTR_LEVEL1 enAttrType)              //一级属性GET
{
	return	 m_dbHuman->m_BaseAttrLevel1.Get(enAttrType) ;
}
VOID				HumanDB::SetDBAttrLvl1(CHAR_ATTR_LEVEL1 enAttrType,INT iValue)	//一级属性SET
{
		m_dbHuman->m_BaseAttrLevel1.Set(enAttrType,iValue) ;
		SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_STR+enAttrType));
}
INT					HumanDB::GetRemainPoints()
{
	return m_dbHuman->m_Level1Points;
}	//剩余一级属性
VOID				HumanDB::SetDBRemainPoints(INT Point)
{ 
	m_dbHuman->m_Level1Points=Point;
	SetDBEnumAttrDirty(CATTR_DB_REMAINPOINT);
}
const _CAMP_DATA *HumanDB::GetDBCampData(VOID)const
{
	return &(m_dbHuman->m_CampData);
}
VOID				HumanDB::SetDBCampData(const _CAMP_DATA *pCampData)
{
	m_dbHuman->m_CampData	=	*pCampData;
	SetDBEnumAttrDirty(CATTR_DB_CAMP);
}
SceneID_t			HumanDB::GetDBStartScene( )
{
	return		m_dbHuman->m_StartScene ;
}
VOID				HumanDB::SetDBStartScene(SceneID_t	sceneID)
{
	m_dbHuman->m_StartScene	=	sceneID;
	SetDBEnumAttrDirty(CATTR_DB_AT_SCENE);
}
SceneID_t			HumanDB::GetDBBakScene( )
{
	return		m_dbHuman->m_BakScene ;
}
VOID				HumanDB::SetDBBakScene(SceneID_t	sceneID)
{
	m_dbHuman->m_BakScene	=	sceneID;
	SetDBEnumAttrDirty(CATTR_DB_BK_SCENE);
}

const CHAR*			HumanDB::GetCurTitle( ) const
{
	return		m_dbHuman->m_Title;
}
VOID				HumanDB::SetCurTitle( const CHAR *pszTitle, BYTE nSize )
{
	Assert(nSize<MAX_CHARACTER_TITLE );
	memset(m_dbHuman->m_Title, 0, MAX_CHARACTER_TITLE );
	memcpy(m_dbHuman->m_Title, pszTitle, nSize);
	SetDBEnumAttrDirty(CATTR_DB_TITLE);
}

BYTE				HumanDB::GetCurTitleType() const
{
	return m_dbHuman->m_Titles.m_CurTitle;
}

VOID				HumanDB::SetCurTitleType( BYTE nType )
{
	m_dbHuman->m_Titles.m_CurTitle = nType;
	SetDBEnumAttrDirty(CATTR_DB_TITLE);
}

BYTE				HumanDB::GetLevelTitle( )
{
	return		m_dbHuman->m_Titles.m_LevelTitle;
}

VOID				HumanDB::SetLevelTitle( BYTE title )
{
	m_dbHuman->m_Titles.m_LevelTitle	=	title;
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);

}
BYTE				HumanDB::GetLifeTitle( )
{
	return		m_dbHuman->m_Titles.m_LifeTitle;
}

VOID				HumanDB::SetLifeTitle( BYTE title )
{
	m_dbHuman->m_Titles.m_LifeTitle		=	title;
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}

BYTE				HumanDB::GetRankTitle( )
{
	return		m_dbHuman->m_Titles.m_RankTitle;
}

VOID				HumanDB::SetRankTitle( BYTE title )
{
	m_dbHuman->m_Titles.m_RankTitle		=	title;
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);

}

BYTE				HumanDB::GetStoryTitle( )
{
	return		m_dbHuman->m_Titles.m_StoryTitle;
}

VOID				HumanDB::SetStoryTitle( BYTE title )
{
	m_dbHuman->m_Titles.m_StoryTitle	=	title;
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}

BYTE				HumanDB::GetKillBossTitle( )
{
	return		m_dbHuman->m_Titles.m_KillBossTitle;
}
VOID				HumanDB::SetKillBossTitle( BYTE title )
{
	m_dbHuman->m_Titles.m_KillBossTitle	=	title;
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}

BYTE				HumanDB::GetTreasureTitle( )
{
	return		m_dbHuman->m_Titles.m_TreasureTitle;
}

VOID				HumanDB::SetTreasureTitle( BYTE title )
{
	m_dbHuman->m_Titles.m_TreasureTitle	=	title;
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);

}
SHORT				HumanDB::GetDyTitle(BYTE	iTitleIndex)
{
	Assert(iTitleIndex<MAX_DYNAMIC_TITLE_SIZE);
	return		m_dbHuman->m_Titles.m_aDyTitle[iTitleIndex];
}
VOID				HumanDB::SetDyTitle(BYTE	iTitleIndex, SHORT TitleData)
{
	Assert(iTitleIndex<MAX_DYNAMIC_TITLE_SIZE);
	Assert(iTitleIndex>=0);
	m_dbHuman->m_Titles.m_aDyTitle[iTitleIndex] = TitleData;
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);

}

const CHAR*			HumanDB::GetNickName() const
{
	return		m_dbHuman->m_Titles.m_szNick;
}
const CHAR*			HumanDB::GetSpouseName() const
{
	return		m_dbHuman->m_Titles.m_szSpouse;
}
const CHAR*			HumanDB::GetBangPaiName() const
{
	return		m_dbHuman->m_Titles.m_szBangPai;
}
const CHAR*			HumanDB::GetJieBaiName() const
{
	return		m_dbHuman->m_Titles.m_szJieBai;
}
const CHAR*			HumanDB::GetShiTuName() const
{
	return		m_dbHuman->m_Titles.m_szShiTu;
}
const CHAR*			HumanDB::GetShangDianName() const
{
	return		m_dbHuman->m_Titles.m_szShangDian;
}

VOID				HumanDB::SetNickName(const CHAR *pszTitle, BYTE nSize ) 
{
	Assert(nSize<MAX_NICK_NAME);
	memset(m_dbHuman->m_Titles.m_szNick, 0, MAX_NICK_NAME);
	memcpy(m_dbHuman->m_Titles.m_szNick, pszTitle, nSize);
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
	
}
VOID				HumanDB::SetSpouseName(const CHAR *pszTitle, BYTE nSize ) 
{
	Assert(nSize<MAX_NICK_NAME);
	memset(m_dbHuman->m_Titles.m_szSpouse, 0, MAX_NICK_NAME );
	memcpy(m_dbHuman->m_Titles.m_szSpouse, pszTitle, nSize);
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}
VOID				HumanDB::SetBangPaiName(const CHAR *pszTitle, BYTE nSize ) 
{
	Assert(nSize<MAX_NICK_NAME);
	memset(m_dbHuman->m_Titles.m_szBangPai, 0, MAX_NICK_NAME);
	memcpy(m_dbHuman->m_Titles.m_szBangPai, pszTitle, nSize);
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}
VOID				HumanDB::SetJieBaiName(const CHAR *pszTitle, BYTE nSize ) 
{
	Assert(nSize<MAX_NICK_NAME);
	memset(m_dbHuman->m_Titles.m_szJieBai, 0, MAX_NICK_NAME);
	memcpy(m_dbHuman->m_Titles.m_szJieBai, pszTitle, nSize);
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}
VOID				HumanDB::SetShiTuName(const CHAR *pszTitle, BYTE nSize ) 
{
	Assert(nSize<MAX_NICK_NAME);
	memset(m_dbHuman->m_Titles.m_szShiTu, 0, MAX_NICK_NAME);
	memcpy(m_dbHuman->m_Titles.m_szShiTu, pszTitle, nSize);
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}
VOID				HumanDB::SetShangDianName(const CHAR *pszTitle, BYTE nSize ) 
{
	Assert(nSize<MAX_NICK_NAME);
	memset(m_dbHuman->m_Titles.m_szShangDian, 0, MAX_NICK_NAME);
	memcpy(m_dbHuman->m_Titles.m_szShangDian, pszTitle, nSize);
	SetDBEnumAttrDirty(CATTR_DB_TITLEINFO);
}
const INT			HumanDB::GetBankItemCount( ) const
{
	return m_dbBank->m_Count;
}
VOID				HumanDB::SetBankItemCount( const INT Bankitemcount )
{
	Assert(Bankitemcount<MAX_BANK_SIZE);
	m_dbBank->m_Count = Bankitemcount;
}
const _ITEM*		HumanDB::GetBankItem( const INT bankindex ) const
{
	Assert(bankindex<MAX_BANK_SIZE);
	return &m_dbBank->m_pItem[bankindex];
}
VOID				HumanDB::SetBankItem( const INT bankindex, const _ITEM* item )
{
	Assert(bankindex<MAX_BANK_SIZE);
	memcpy(&m_dbBank->m_pItem[bankindex], item, sizeof(_ITEM));
}
const INT			HumanDB::GetBankEndIndex( ) const
{
	return m_dbBank->m_CurEndIndex;
}
VOID				HumanDB::SetBankEndIndex( const INT bankindex )
{
	m_dbBank->m_CurEndIndex = bankindex;
	SetDBEnumAttrDirty(CATTR_DB_BANK_OFFSIZE);
}
const INT			HumanDB::GetBankMoney( ) const
{
	return m_dbBank->m_Money;
}
VOID				HumanDB::SetBankMoney( const INT money )
{
	m_dbBank->m_Money = money;
	SetDBEnumAttrDirty(CATTR_DB_BANK_MONEY);
}

VOID				HumanDB::EraseBankItem(UINT uBankIndex)
{
	memset(_GetBankItem(uBankIndex),0,sizeof(_ITEM));
}
VOID				HumanDB::SwapBankWithBank(UINT iBankIndexFr, UINT iBankIndexTo )
{
	Assert(iBankIndexFr<MAX_BAG_SIZE);
	Assert(iBankIndexTo<MAX_BANK_SIZE );
	SwapItem(&m_dbBank->m_pItem[iBankIndexFr],&m_dbBank->m_pItem[iBankIndexTo]);
}


VOID				HumanDB::SwapBankWithBag(UINT iBagIndex, UINT iBankIndex )
{
	Assert(iBagIndex<MAX_BAG_SIZE);
	Assert(iBankIndex<MAX_BANK_SIZE );
	SwapItem(&m_dbBank->m_pItem[iBankIndex],&m_dbBag->m_pItem[iBagIndex]);
}
VOID				HumanDB::SwapBankWithEquip( HUMAN_EQUIP	equipPoint, UINT iBankIndex )
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBankIndex<MAX_BANK_SIZE);
	SwapItem(&m_dbEquip->m_pItem[equipPoint],&m_dbBank->m_pItem[iBankIndex]);
}

VOID				HumanDB::OverWriteBag2Bank(UINT iBagIndex, UINT iBankIndex)
{
	Assert(iBagIndex<MAX_BAG_SIZE);
	Assert(iBankIndex<MAX_BANK_SIZE);
	OverWriteItem(&m_dbBag->m_pItem[iBagIndex], &m_dbBank->m_pItem[iBankIndex]);
}

VOID				HumanDB::OverWriteEquip2Bank(HUMAN_EQUIP	equipPoint, UINT iBankIndex)
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBankIndex<MAX_BANK_SIZE);
	OverWriteItem(&m_dbEquip->m_pItem[equipPoint] ,&m_dbBank->m_pItem[iBankIndex]);
}

VOID				HumanDB::OverWriteBank2Bag(UINT iBagIndex, UINT iBankIndex)
{
	Assert(iBagIndex<MAX_BAG_SIZE);
	Assert(iBankIndex<MAX_BANK_SIZE);
	OverWriteItem(&m_dbBank->m_pItem[iBankIndex], &m_dbBag->m_pItem[iBagIndex]);
}

VOID				HumanDB::OverWriteBank2Equip(HUMAN_EQUIP	equipPoint, UINT iBankIndex)
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBankIndex<MAX_BANK_SIZE);
	OverWriteItem(&m_dbBank->m_pItem[iBankIndex], &m_dbEquip->m_pItem[equipPoint] );
}




//position 不实时写数据库
VOID				HumanDB::SetDBPosition(const WORLD_POS& pos)
{
	m_dbHuman->m_Position	=	pos;
	SetDBEnumAttrDirty(CATTR_DB_AT_POSITION);
}
inline  const WORLD_POS*	HumanDB::GetDBPosition()
{
	return &m_dbHuman->m_Position;
}

VOID				HumanDB::SetDBBakPosition(const WORLD_POS& pos)
{
	m_dbHuman->m_BakPosition = pos;
	SetDBEnumAttrDirty(CATTR_DB_BK_POSITION);
}

inline  const WORLD_POS*	HumanDB::GetDBBakPosition()
{
	return &m_dbHuman->m_BakPosition;
}
//玩家logout 时调用的函数
VOID				HumanDB::WriteDBPosition(WORLD_POS& pos)
{
	m_dbHuman->m_Position	=	pos;
	SetDBEnumAttrDirty(CATTR_DB_AT_POSITION);
}

VOID				HumanDB::SetOnlineTime(UINT uTime)	//设置总在线时间
{
	m_dbHuman->m_OnlineTime = uTime;
	SetDBEnumAttrDirty(CATTR_DB_ONLINETIME);
}
UINT				HumanDB::GetOnlineTime()			//取得总在线时间	
{
	return m_dbHuman->m_OnlineTime;
}
VOID				HumanDB::SetLoginTime(UINT uTime)	//设置最后登录时间
{
	m_dbHuman->m_LastLoginTime = uTime;
	SetDBEnumAttrDirty(CATTR_DB_LASTLOGINTIME);
}
UINT				HumanDB::GetLoginTime()				//取得最后登录时间
{
	return m_dbHuman->m_LastLoginTime;
}
VOID				HumanDB::SetLogoutTime(UINT uTime)	//设置最后下线时间
{
	m_dbHuman->m_LastLogoutTime = uTime;
	SetDBEnumAttrDirty(CATTR_DB_LASTLOGOUTTIME);
}
UINT				HumanDB::GetLogoutTime()			//取得最后下线时间
{
	return m_dbHuman->m_LastLogoutTime;
}

inline  VOID		HumanDB::SetDBVersion(UINT Ver)		//设置数据库版本
{
	m_dbHuman->m_DBVersion = Ver;
	SetDBEnumAttrDirty(CATTR_DB_DBVERSION);
}
inline  UINT		HumanDB::GetDBVersion()				//取得数据库版本
{
	return m_dbHuman->m_DBVersion;
}

VOID				HumanDB::SetDBGuid(GUID_t	guid)
{
	m_dbHuman->m_GUID	=	guid;
	SetDBEnumAttrDirty(CATTR_DB_GUID);
}

GuildID_t			HumanDB::GetDBGuildID( )
{
	return m_dbHuman->m_GuildID ;
}

VOID				HumanDB::SetDBGuildID( GuildID_t guildID )
{
	m_dbHuman->m_GuildID	=	guildID;
	SetDBEnumAttrDirty(CATTR_DB_GUILDID);
}

TeamID_t			HumanDB::GetDBTeamID( )
{
	return m_dbHuman->m_TeamID ;
}

VOID				HumanDB::SetDBTeamID( TeamID_t teamID )
{
	m_dbHuman->m_TeamID	=	teamID;
	SetDBEnumAttrDirty(CATTR_DB_TEAMID);
}


//////////////////////////////////////////////////////////////////////////
//背包数据访问方法		
//////////////////////////////////////////////////////////////////////////
VOID				HumanDB::EraseBagItem(uint BagIndex)
{
	Assert(BagIndex<MAX_BAG_SIZE);
	memset(_GetBagItem(BagIndex),0,sizeof(_ITEM));
}



//////////////////////////////////////////////////////////////////////////
VOID				HumanDB::SwapEquipBag(HUMAN_EQUIP	equipPoint,uint iBagIndex)
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBagIndex<MAX_BAG_SIZE);
	SwapItem(&m_dbEquip->m_pItem[equipPoint],&m_dbBag->m_pItem[iBagIndex]);
}

VOID				HumanDB::OverWriteBag2Equip(uint iBagIndex,HUMAN_EQUIP	equipPoint)
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBagIndex<MAX_BAG_SIZE);
	OverWriteItem(&m_dbBag->m_pItem[iBagIndex], &m_dbEquip->m_pItem[equipPoint]);
}

VOID				HumanDB::OverWriteEquip2Bag(HUMAN_EQUIP	equipPoint,uint iBagIndex)
{
	Assert(HEQUIP_NUMBER>equipPoint);
	Assert(iBagIndex<MAX_BAG_SIZE);
	OverWriteItem(&m_dbEquip->m_pItem[equipPoint] ,&m_dbBag->m_pItem[iBagIndex]);
}


VOID				HumanDB::PutBagDB(const _ITEM* pItem,uint iBagIndex)
{
	m_dbBag->PutBagDB(pItem,iBagIndex);
}

VOID				HumanDB::SwapBagDB(uint iAIndex,uint iBIndex)
{
	m_dbBag->SwapBagDB(iAIndex,iBIndex);
}

BOOL				HumanDB::IncBagItemCount(uint iBagIndex)
{
	
	return m_dbBag->IncCount(iBagIndex);
}

BOOL				HumanDB::DecBagItemCount(uint iBagIndex)
{
	return m_dbBag->DecCount(iBagIndex);
}

VOID				HumanDB::SetItemBind(uint iBagIndex,BOOL	bBind)
{
	m_dbBag->SetItemBind(iBagIndex,bBind);
}

const _ITEM*		HumanDB::GetBagItem(uint iBagIndex) const
{
	return m_dbBag->GetBagDB(iBagIndex);
}

const	_ITEM*		HumanDB::GetEquipItem(HUMAN_EQUIP equipPoint)
{
	return &m_dbEquip->m_pItem[equipPoint];
}

VOID				HumanDB::EraseEquipItem(HUMAN_EQUIP equipPoint)
{
	m_dbEquip->m_pItem[equipPoint].CleanUp();
}

VOID				HumanDB::SetEquipMark(HUMAN_EQUIP eEquipPoint, BYTE bMark)
{
	m_dbEquip->Set(eEquipPoint,bMark);
}

VOID				HumanDB::SetEquipDur(HUMAN_EQUIP eEquipPoint, INT Dur)
{
	m_dbEquip->SetEquipDur(eEquipPoint,Dur);
}

VOID				HumanDB::SetItemDur(uint BagIndex, INT Dur)
{
	m_dbBag->SetItemDur(BagIndex,Dur);
}


inline	UINT		HumanDB::GetBaseBagSize()
{
	return	m_dbBag->m_BaseBagSize;
}
//inline	UINT		HumanDB::GetTaskBagSize()
//{
//	return m_dbBag->m_TaskBagSize_Nouse;
//}
//inline	UINT		HumanDB::GetMatBagSize()
//{
//	return m_dbBag->m_MatBagSize_Nouse;
//}


inline _ITEM*		HumanDB::_GetBagItem(UINT uBagIndex)
{
	Assert(uBagIndex<MAX_BAG_SIZE);
	return	m_dbBag->m_pItem+uBagIndex;
}
inline _ITEM*		HumanDB::_GetBankItem(UINT uBankIndex)
{
	Assert(uBankIndex<MAX_BANK_SIZE);
	return	m_dbBank->m_pItem+uBankIndex;
}
inline VOID			HumanDB::SetCooldown(CooldownID_t nID,Time_t nTime)
{
	m_dbCooldown->RegisterCooldown(nID, nTime);
	SetDBEnumAttrDirty(CATTR_DB_COOLDOWN);
}
inline BOOL			HumanDB::IsCooldowned(CooldownID_t nID) const
{
	return m_dbCooldown->IsSpecificSlotCooldownedByID(nID);
}
inline VOID			HumanDB::HeartBeat_Cooldown(Time_t nDeltaTime)
{
	//这里不加脏标记
	m_dbCooldown->HeartBeat(nDeltaTime);
}
inline Time_t		HumanDB::GetCooldownRemain(CooldownID_t nID) const
{
	return m_dbCooldown->GetRemainTimeByID(nID);
}
inline BYTE		HumanDB::GetSkillCount()
{
	return m_dbSkill->m_Count;
}
inline VOID		HumanDB::SetSkillCount(BYTE Count)
{
	Assert(Count<=MAX_CHAR_SKILL_NUM);
	
	m_dbSkill->m_Count = m_dbXinFa->m_Count = Count;

	SetDBEnumAttrDirty(CATTR_DB_SKILL);
	SetDBEnumAttrDirty(CATTR_DB_XINFA);
}
inline SkillID_t HumanDB::GetSkillID(BYTE Index)
{
	Assert(Index<MAX_CHAR_SKILL_NUM);
	return m_dbSkill->m_aSkill[Index].m_nSkillID;
}
inline VOID		HumanDB::SetSkillID(SkillID_t nID,BYTE Index)
{
	Assert(Index<MAX_CHAR_SKILL_NUM);
	m_dbSkill->m_aSkill[Index].m_nSkillID	=	nID;
	SetDBEnumAttrDirty(CATTR_DB_SKILL);
}
inline BYTE	HumanDB::GetSkillLevel(BYTE Index)
{
	Assert(Index<MAX_CHAR_SKILL_NUM);
	return m_dbSkill->m_aSkill[Index].GetSkillLevel();
}
inline VOID	HumanDB::SetSkillLevel(BYTE Level,BYTE Index)
{
	Assert(Index<MAX_CHAR_SKILL_NUM);
	Assert(Level<MAX_CHAR_SKILL_LEVEL);
	m_dbSkill->m_aSkill[Index].SetSkillLevel(Level);
	SetDBEnumAttrDirty(CATTR_DB_SKILL);
}

//inline BYTE		HumanDB::GetXinFaCount()
//{
//	return m_dbXinFa->m_Count;
//}
//inline VOID		HumanDB::SetXinFaCount(BYTE Count)
//{
//	m_dbXinFa->m_Count	=	Count;
//	SetDBEnumAttrDirty(CATTR_DB_XINFA);
//}

//inline SkillID_t HumanDB::GetXinfaID(BYTE Index)
//{
//	Assert(Index<MAX_CHAR_XINFA_NUM);
//	return m_dbXinFa->m_aXinFa[Index].m_nXinFaID; 
//
//}
//inline VOID		HumanDB::SetXinfaID(SkillID_t nID,BYTE Index)
//{
//	Assert(Index<MAX_CHAR_XINFA_NUM);
//	m_dbXinFa->m_aXinFa[Index].m_nXinFaID	=	nID;
//	SetDBEnumAttrDirty(CATTR_DB_XINFA);
//}
//inline BYTE	HumanDB::GetXinfaLevel(BYTE Index)
//{
//	Assert(Index<MAX_CHAR_XINFA_NUM);
//	return m_dbXinFa->m_aXinFa[Index].m_nLevel;
//}
//inline VOID	HumanDB::SetXinfaLevel(BYTE Level,BYTE Index)
//{
//	Assert(Index<MAX_CHAR_XINFA_NUM);
//	m_dbXinFa->m_aXinFa[Index].m_nLevel	=	Level;	
//	SetDBEnumAttrDirty(CATTR_DB_XINFA);
//}
inline VOID HumanDB::SetMissionDone(MissionID_t idMission,MissionID_t idIndex)
{
	((m_dbMission->m_aMissionHaveDoneFlags[idIndex] |= (0x00000001 << ((UINT)idMission & 0x0000001F))) != 0);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}

inline VOID HumanDB::SetMissionNotDone(MissionID_t idMission,MissionID_t idIndex)
{
	((m_dbMission->m_aMissionHaveDoneFlags[idIndex] &= (0x00000000 << ((UINT)idMission & 0x0000001F))) != 0);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}

inline VOID HumanDB::SetMissionParam(UINT uIndexMission, UINT uIndexParam, INT nValue )
{
	m_dbMission->m_aMission[uIndexMission].m_anParam[uIndexParam] = nValue;
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}
inline VOID	HumanDB::SetMissionData( INT nIndex, INT nData )
{
	m_dbMission->m_aMissionData[nIndex] = nData ;
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}

inline VOID HumanDB::SetMission(UINT uIndex,MissionID_t idMission,ScriptID_t idScript)
{
	m_dbMission->m_aMission[uIndex].m_idMission = idMission;
	m_dbMission->m_aMission[uIndex].m_idScript  = idScript;
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}

inline VOID	HumanDB::SetMissionCount(UINT uCount)
{
	m_dbMission->m_Count = uCount;
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}

inline VOID	HumanDB::SetFlags_KillObject(UINT uIndex, BOOL bSet )
{
	m_dbMission->m_aMission[uIndex].SetFlags_KillObject(bSet);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}
inline VOID	HumanDB::SetFlags_EnterArea(UINT uIndex, BOOL bSet )
{
	m_dbMission->m_aMission[uIndex].SetFlags_EnterArea(bSet);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}
inline VOID	HumanDB::SetFlags_ItemChanged(UINT uIndex, BOOL bSet )
{
	m_dbMission->m_aMission[uIndex].SetFlags_ItemChanged(bSet);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}
inline VOID	HumanDB::SetFlags_PetChanged(UINT uIndex, BOOL bSet )
{
	m_dbMission->m_aMission[uIndex].SetFlags_PetChanged(bSet);
	SetDBEnumAttrDirty(CATTR_DB_MISSION);
}
inline VOID HumanDB::ClearMission(UINT uIndex)
{
	m_dbMission->m_aMission[uIndex].Cleanup();
	m_dbMission->m_Count--;
}

//////////////////////////////////////////////////////////////////////////
//关系数据访问方法
//////////////////////////////////////////////////////////////////////////

VOID				HumanDB::SetRelation(INT Index, const _OWN_RELATION* pRelation)
{
	Assert(pRelation);
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	m_dbRelation->m_aRelation[Index].Init(pRelation);
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::ModifyFriendType( INT nIndex, RELATION_TYPE RelationType )
{
	m_dbRelation->m_aRelation[nIndex].m_Type = RelationType;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::UpdateRelationInfo(INT Index, const RELATION_MEMBER* pMemberInfo )
{
	Assert(pMemberInfo);
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	memcpy( (void*)&(m_dbRelation->m_aRelation[Index].m_Member), pMemberInfo, sizeof(RELATION_MEMBER) );
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetMood(const CHAR* szMood)
{
	Assert( szMood );
	strncpy( m_dbRelation->m_szMood, szMood, sizeof(m_dbRelation->m_szMood)-1 );
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::ModifyFriendPoint(INT Index, INT nFriendPoint)
{
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	m_dbRelation->m_aRelation[Index].m_FriendPoint += nFriendPoint;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::AdjustRelationType(INT Index, UCHAR uType)
{
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	m_dbRelation->m_aRelation[Index].m_Type = uType;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::AdjustRelationGroup(INT Index, UCHAR uGroup)
{
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	m_dbRelation->m_aRelation[Index].m_Group = uGroup;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::DeleteRelation(INT Index)
{
	Assert(Index>=0 && Index<MAX_RELATION_SIZE);
	m_dbRelation->m_aRelation[Index].CleanUp();
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::IncRelationCount(INT Index)
{
	Assert(Index>=0 && Index<RELATION_GROUP_NUMBER);
	++(m_dbRelation->m_aRelationCount[Index]);
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::DecRelationCount(INT Index)
{
	Assert(Index>=0 && Index<RELATION_GROUP_NUMBER);
	--(m_dbRelation->m_aRelationCount[Index]);
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetSpouse(const MarriageInfo* pMarriageInfo)
{
	Assert(pMarriageInfo);
	m_dbRelation->m_MarriageInfo.m_SpouseGUID = pMarriageInfo->m_SpouseGUID;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::AddPrentice(GUID_t guid)
{
	Assert(m_dbRelation->m_PrenticeInfo.m_uPrenticeCount<=MAX_PRENTICE_COUNT);

	for( INT i=0; i<MAX_PRENTICE_COUNT; ++i )
	{
		if(m_dbRelation->m_PrenticeInfo.m_PrenticeGUID[i] == INVALID_ID)
		{
			m_dbRelation->m_PrenticeInfo.m_PrenticeGUID[i] = guid;
			++(m_dbRelation->m_PrenticeInfo.m_uPrenticeCount);
			break;
		}
	}

	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::DelPrentice(GUID_t guid)
{
	Assert(m_dbRelation->m_PrenticeInfo.m_uPrenticeCount>0);

	for( INT i=0; i<MAX_PRENTICE_COUNT; ++i )
	{
		if(m_dbRelation->m_PrenticeInfo.m_PrenticeGUID[i] == guid)
		{
			m_dbRelation->m_PrenticeInfo.m_PrenticeGUID[i] = INVALID_ID;
			--(m_dbRelation->m_PrenticeInfo.m_uPrenticeCount);
			break;
		}
	}

	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetMoralPoint(UINT uPoint)
{
	Assert(uPoint <= MAX_MORAL_POINT);
	m_dbRelation->m_PrenticeInfo.m_uMoralPoint = uPoint;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetBetrayTime(time_t BetrayTime)
{
	m_dbRelation->m_PrenticeInfo.m_BetrayingTime = BetrayTime;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetMaster(const MasterInfo* pMasterInfo)
{
	Assert(pMasterInfo);
	m_dbRelation->m_MasterInfo.m_MasterGUID = pMasterInfo->m_MasterGUID;
	SetDBEnumAttrDirty(CATTR_DB_RELATION);
}

VOID				HumanDB::SetAbilityLevel(AbilityID_t abilityid,INT lvl)
{
	if( abilityid<1 || abilityid>MAX_CHAR_ABILITY_NUM )
	{
		return;
	}

	m_dbAbility->Get_Ability(abilityid).m_Level = lvl;

	SetDBEnumAttrDirty(CATTR_DB_ABILITY);
}

VOID				HumanDB::SetAbilityExp(AbilityID_t abilityid,INT exp)
{
	if( abilityid<1 || abilityid>MAX_CHAR_ABILITY_NUM )
	{
		return;
	}

	m_dbAbility->Get_Ability(abilityid).m_Exp = exp;
	SetDBEnumAttrDirty(CATTR_DB_ABILITY);
}

VOID				HumanDB::SetPrescrLearnedFlag(const PrescriptionID_t prescrid, BOOL flag )
{
	__ENTER_FUNCTION

	INT idx;
	idx = (INT)prescrid;
	if( idx<0 || idx>=MAX_ABILITY_PRESCRIPTION_NUM )
	{
		Assert(FALSE);
		return;
	}

	if ( flag != FALSE )
	{ // 学会
		m_dbAbility->m_aPrescr[idx>>3] |= (0x01<<idx%8);
	}
	else
	{ // 放弃
		m_dbAbility->m_aPrescr[idx>>3] &= ~(0x01<<idx%8);
	}

	__LEAVE_FUNCTION
}


inline	const _PET_DB_LOAD*	HumanDB::GetPet(uint	iIndex)	const 
{
		Assert(iIndex<HUMAN_PET_MAX_COUNT);
		return &m_dbPetList->m_aPetDB[iIndex];
}

inline VOID			HumanDB::_RegDBAttr(CHAR_ATTR_DB eOffSet,VOID* pArg,INT iSize)
{
	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(pArg);
	Assert(iSize);
	Assert(m_AttrReg);
	m_AttrReg->m_AttrPtr[eOffSet] = pArg;
	m_AttrReg->m_AttrSize[eOffSet] = iSize;
}

inline VOID			HumanDB::_RegSMAttr(CHAR_ATTR_DB eOffSet,VOID* pArg,INT iSize)
{
	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(pArg);
	Assert(iSize);
	Assert(m_AttrRegSM);
	m_AttrRegSM->m_AttrPtr[eOffSet] = pArg;
	m_AttrRegSM->m_AttrSize[eOffSet] = iSize;
}

inline VOID			HumanDB::SetSettingData(INT nType, _OWN_SETTING* Value )
{
	if( nType<0 || nType>=SETTING_TYPE_NUMBER )
		return ;

	m_dbSetting->m_aSetting[nType] = *Value ;
	SetDBEnumAttrDirty(CATTR_DB_SETTING);
}

inline BOOL			HumanDB::isSetBit( INT nType, INT bit )
{
	if( nType<0 || nType>=SETTING_TYPE_NUMBER )
		return FALSE;

	if( m_dbSetting->m_aSetting[nType].m_SettingData & (1<<bit) )
		return TRUE;

	return FALSE;
}

#define SETTINGFLAGISTRUE(Setting, FlagSet) (Setting&(1<<FlagSet))
inline VOID			HumanDB::SetPrivateInfo(_PRIVATE_INFO_DB_LOAD* pPrivateInfo)
{
	__ENTER_FUNCTION
		
		Assert(pPrivateInfo);
		memcpy(m_dbPrivateInfo,pPrivateInfo,sizeof(_PRIVATE_INFO_DB_LOAD));
		SetDBEnumAttrDirty(CATTR_DB_PRIVATEINFO);

	__LEAVE_FUNCTION

}

inline const _OWN_SETTING* HumanDB::GetSetting( INT nType ) const
{
	if( nType<0 || nType>=SETTING_TYPE_NUMBER )
		return NULL ;

	return &(m_dbSetting->m_aSetting[nType]) ;
}

inline	INT					HumanDB::GetReserve(INT index)
{
	Assert(index>=0);
	Assert(index<MAX_RESERVE);
	return	m_dbHuman->m_Reserve[index];
}
inline VOID						HumanDB::SetReserve(INT index,INT value)
{
	Assert(index>=0);
	Assert(index<MAX_RESERVE);
	m_dbHuman->m_Reserve[index] = value;
	SetDBEnumAttrDirty(CATTR_DB_RESERVE);
}


const	_BAG_DB_LOAD*			HumanDB::GetBag()
{
	return m_dbBag;
}
const	_HUMAN_DB_LOAD*			HumanDB::GetHumanDB()	const
{
	return	m_dbHuman;
}
const	_EQUIP_DB_LOAD*			HumanDB::GetEquipDB()
{
	return	m_dbEquip;
}
const	_BANK_DB_LOAD*			HumanDB::GetBankDB()
{
	return m_dbBank;
}

const _SKILL_DB_LOAD*			HumanDB::GetSkillDB()
{
	return m_dbSkill;
}

const _COOLDOWN_DB_LOAD_FOR_HUMAN* HumanDB::GetCooldownDB() const
{
	return m_dbCooldown;	
}

const	_RELATION_DB_LOAD*		HumanDB::GetRelationDB() const
{
	return m_dbRelation;
}
const _ABILITY_DB_LOAD*	HumanDB::GetAbilityDB() const
{
	return m_dbAbility;
}
const _XINFA_DB_LOAD*			HumanDB::GetXinFaDB() const
{
	return m_dbXinFa;
}

const _IMPACT_DB_LOAD*		HumanDB::GetImpactDB() const
{
	return m_dbImpact;
}

const _MISSION_DB_LOAD*		HumanDB::GetMissionDB() const
{
	return m_dbMission;
}

const _SETTING_DB_LOAD*		HumanDB::GetSettingDB() const
{
	return m_dbSetting;
}
const	_PRIVATE_INFO_DB_LOAD*	HumanDB::GetPrivateInfoDB() const
{
	return m_dbPrivateInfo;
}

VOID				HumanDB::WriteHumanDB(const	_HUMAN_DB_LOAD* pHumanDB)
{
	memcpy(m_dbHuman,pHumanDB,sizeof(_HUMAN_DB_LOAD));
}

VOID				HumanDB::WriteBagDB(const	_BAG_DB_LOAD* pBagDB)
{
	memcpy(m_dbBag,pBagDB,sizeof(_BAG_DB_LOAD));
}

VOID				HumanDB::WriteEquipDB(const	_EQUIP_DB_LOAD*	pEquipDB)
{
	memcpy(m_dbEquip,pEquipDB,sizeof(_EQUIP_DB_LOAD));
}


VOID				HumanDB::WriteBankDB(const	_BANK_DB_LOAD*	pBankDB)
{
	memcpy(m_dbBank,pBankDB,sizeof(_BANK_DB_LOAD));					
}

VOID				HumanDB::WriteSkillDB(const _SKILL_DB_LOAD* pSkillDB)
{
	memcpy(m_dbSkill,pSkillDB,sizeof(_SKILL_DB_LOAD));					
}

VOID				HumanDB::WriteCooldownDB(const _COOLDOWN_DB_LOAD_FOR_HUMAN* pCooldownDB)
{
	memcpy(m_dbCooldown,pCooldownDB,sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN));					
}

VOID				HumanDB::WriteRelationDB(const _RELATION_DB_LOAD* pRelationDB)
{
	memcpy(m_dbRelation, pRelationDB, sizeof(_RELATION_DB_LOAD));					
}

VOID				HumanDB::WriteAbilityDB(const _ABILITY_DB_LOAD* pAbilityDB)
{
	memcpy(m_dbAbility,pAbilityDB,sizeof(_ABILITY_DB_LOAD));
}

VOID				HumanDB::WriteXinFaDB(const _XINFA_DB_LOAD* pXinFaDB)
{
	memcpy(m_dbXinFa,pXinFaDB,sizeof(_XINFA_DB_LOAD));
}


VOID				HumanDB::WriteImpactDB(const _IMPACT_DB_LOAD* pImpactDB)
{
	memcpy(m_dbImpact,pImpactDB,sizeof(_IMPACT_DB_LOAD));
}

VOID				HumanDB::WriteMissionDB(const _MISSION_DB_LOAD* pMissionDB)
{
	memcpy(m_dbMission,pMissionDB,sizeof(_MISSION_DB_LOAD));
}

VOID				HumanDB::WriteSettingDB(const _SETTING_DB_LOAD* pSettingDB)
{
	memcpy(m_dbSetting,pSettingDB,sizeof(_SETTING_DB_LOAD));
}
VOID			HumanDB::WritePrivateInfo(const _PRIVATE_INFO_DB_LOAD* pPrivateDB)
{
	memcpy(m_dbPrivateInfo,pPrivateDB,sizeof(_PRIVATE_INFO_DB_LOAD));
}

#endif

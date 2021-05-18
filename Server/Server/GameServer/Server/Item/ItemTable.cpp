#include "stdafx.h"
#include "ItemHelper.h"
#include "ItemTable.h"
#include "TLBB_DBC.h"
#include "ItemHelper.h"
#include "Log.h"
#include "Obj_ItemBox.h"
#include "ItemManager.h"
#include "FileDef.h"

ItemTable	g_ItemTable;

using namespace DBC;


#define SAFE_VALUE(x) ((x>0) && (x!=-1))?x:0



INT CompareMonsterDropTB(const VOID* pArg1,const VOID* pArg2)
{	
	__ENTER_FUNCTION

	MONSTER_DROPBOX_TB* pTB1	=	(MONSTER_DROPBOX_TB*)pArg1;
	MONSTER_DROPBOX_TB* pTB2	=	(MONSTER_DROPBOX_TB*)pArg2;

	if(pTB1->m_MonsterId>pTB2->m_MonsterId)
		return 1;
	else if(pTB1->m_MonsterId<pTB2->m_MonsterId)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT CompareDropBoxTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION
	
	DROP_BOX_TB* pTB1	=	(DROP_BOX_TB*)pArg1;
	DROP_BOX_TB* pTB2	=	(DROP_BOX_TB*)pArg2;

	if(pTB1->m_DropBoxId>pTB2->m_DropBoxId)
		return 1;
	else if(pTB1->m_DropBoxId<pTB2->m_DropBoxId)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT CompareGreenItemTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION
	
	GUID_t	guid1,guid2;
	
	guid1	= ((GREEN_EQUIP_TB*)pArg1)->m_IndexID;
	guid2	= ((GREEN_EQUIP_TB*)pArg2)->m_IndexID;

	if(guid1>guid2)
		return  1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT CompareCommonEquipTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION
	
	GUID_t	guid1,guid2;

	guid1	= ((COMMON_EQUIP_TB*)pArg1)->m_IndexID;
	guid2	= ((COMMON_EQUIP_TB*)pArg2)->m_IndexID;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT CompareBlueItemTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	GUID_t	guid1,guid2;

	guid1	= ((BLUE_EQUIP_TB*)pArg1)->m_IndexID;
	guid2	= ((BLUE_EQUIP_TB*)pArg2)->m_IndexID;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT CompareItemQualityTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	INT itemLvl1,itemLvl2;
	itemLvl1	=	((ITEM_QUALITY_TB*)pArg1)->m_ItemLevel;
	itemLvl2	=	((ITEM_QUALITY_TB*)pArg2)->m_ItemLevel;

	if(itemLvl1>itemLvl2)
			return	1;
	else	if(itemLvl1<itemLvl2)
			return	-1;
	else
			return	0;

	__LEAVE_FUNCTION	
	return 0 ;
}


INT CompareDropAttTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	INT iDelta1,iDelta2;

	iDelta1		=	((DROP_ATT_TB*)pArg1)->m_DeltaLevel;
	iDelta2		=	((DROP_ATT_TB*)pArg2)->m_DeltaLevel;

	if(iDelta1>iDelta2)
		return	 1;
	else if(iDelta1<iDelta2)
	    return	-1;
	else	
		return	 0;
	__LEAVE_FUNCTION	
	return 0 ;
}



INT CompareGemInfoTBByTI(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	INT	TableIndex1,TableIndex2;

	TableIndex1	= ((GEMINFO_TB*)pArg1)->m_nTableIndex;
	TableIndex2	= ((GEMINFO_TB*)pArg2)->m_nTableIndex;

	if(TableIndex1>TableIndex2)
		return 1;
	else if(TableIndex1<TableIndex2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT CompareEquipSetTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	INT Serial1,Serial2;

	Serial1	=	((EQUIP_SET_TB*)pArg1)->m_nEquipSetSerial;
	Serial2	=	((EQUIP_SET_TB*)pArg2)->m_nEquipSetSerial;

	if(Serial1>Serial2)
		return 1;
	else if(Serial1<Serial2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT CompareCommItemInfoTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	GUID_t	guid1,guid2;

	guid1	= ((COMMITEM_INFO_TB*)pArg1)->m_nTableIndex;
	guid2	= ((COMMITEM_INFO_TB*)pArg2)->m_nTableIndex;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

INT	CompareStoreMapInfoTB(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	GUID_t	guid1,guid2;

	guid1	= ((STORE_MAP_INFO_TB*)pArg1)->m_nTableIndex;
	guid2	= ((STORE_MAP_INFO_TB*)pArg2)->m_nTableIndex;

	if(guid1>guid2)
		return 1;
	else if(guid1<guid2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}



ItemTable::ItemTable( )
{
	m_pCommonEquipTableData			=	NULL;
	m_pGreenEquipTableData			=	NULL;
	m_pBlueEquipTableData			=	NULL;
	m_pDropBoxTableData				=	NULL;
	m_pMonsterDropTableData			=	NULL;
	m_pItemQualityData				=	NULL;
	m_pItemValueData				=	NULL;
	m_pGemInfoData					=	NULL;
	m_pCommItemInfoData				=	NULL;
	m_pDropAttData					=	NULL;
	m_pEquipSetData					=	NULL;
	m_pItemRulerData				=	NULL;
	m_pStoreMapData					=	NULL;
}

ItemTable::~ItemTable( )
{
	CleanUp( ) ;
}

VOID	ItemTable::CleanUp( )
{
	__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( m_pCommonEquipTableData ) ;
	SAFE_DELETE_ARRAY( m_pGreenEquipTableData ) ;
	SAFE_DELETE_ARRAY( m_pBlueEquipTableData ) ;
	SAFE_DELETE_ARRAY( m_pDropBoxTableData ) ;
	SAFE_DELETE_ARRAY( m_pMonsterDropTableData ) ;
	SAFE_DELETE_ARRAY( m_pItemQualityData ) ;
	SAFE_DELETE_ARRAY( m_pItemValueData ) ;
	SAFE_DELETE_ARRAY( m_pGemInfoData);
	SAFE_DELETE_ARRAY(m_pCommItemInfoData);
	SAFE_DELETE_ARRAY(m_pDropAttData);
	SAFE_DELETE_ARRAY(m_pEquipSetData);
	SAFE_DELETE_ARRAY(m_pItemRulerData);
	SAFE_DELETE_ARRAY(m_pStoreMapData);

	__LEAVE_FUNCTION	
}



BOOL	ItemTable::Init()
{
	__ENTER_FUNCTION

	m_nHashOffSet			=	0;
	
	m_nGoldenItemCount		=	0;
	m_nGreenItemCount		=	0;
	m_nBlueItemCount		=	0;

	m_nCommonItemCount		=   0;
	m_nDropBoxCount			=	0;
	m_nMonsterDropBoxCount	=	0;

	m_nItemLevelCount		=	0;
	m_nValueCount			=	0;

	m_nGemCount				=	0;
	m_nCommItemCount		=	0;

	m_nDropAttCount			=	0;
	m_nEquipSetCount		=	0;

	m_nItemRulerCount		=	0;
	m_nStoreMapCount		=	0;

	InitWhiteItemTable();
	InitBlueItemTable();
	InitGreenItemTable();
	
	InitGoldenItemTable();
	InitDropBoxTable();
	InitMonsterDropBoxTable();
	InitItemLevelTable();
	InitItemValueTable();
	InitCommItemInfoTable();
	InitGemInfoTable();
	InitDropAttTable();
	InitEquipSetAttrTable();
	InitItemRulerTable();
	InitStoreMapTable();
	

	__LEAVE_FUNCTION	

	return	TRUE;
}

//加载普通装备表
VOID	ItemTable::InitWhiteItemTable()
{
	__ENTER_FUNCTION
	

	enum WhiteTBData
	{
		TBIndex				= 0,
		TBClass				= 1,
		TBQuality			= 2,
		TBType				= 3,
		TBItemIndex			= 4,
		TBEquipPoint		= 5,
		TBVisual			= 6,
		TBRulerID			= 7,
		TBEQuipSetNum		= 8,
		TBEquipSetMaxNum	= 9,
		TBJob				= 10,
		TBEquipName			= 11,
		TBReqLevel			= 12,
		TBDesc				= 13,
		TBBasePrice			= 14,
		TBMaxDur			= 15,
		TBMaxRepair			= 16,
		TBMaxGem			= 17, 	
		TBAttrBegin			= 18,
		TBAttrEnd			= TBAttrBegin+IATTRIBUTE_NUMBER-1
	};


	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( File_WHITE_EQUIP_BASE );

	INT iTableCount		= ThirdFile.GetRecordsNum();
	INT iTableColumn	= ThirdFile.GetFieldsNum();

	m_nCommonItemCount			=	iTableCount;
	m_pCommonEquipTableData		=	new COMMON_EQUIP_TB[m_nCommonItemCount];

	
	for(INT i =0;i<iTableCount;i++)
	{

		m_pCommonEquipTableData[i].m_IndexID				= 		ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pCommonEquipTableData[i].m_ItemType.m_Class		= 		ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pCommonEquipTableData[i].m_ItemType.m_Quality		= 		ThirdFile.Search_Posistion(i,TBQuality)->iValue;	 
		m_pCommonEquipTableData[i].m_ItemType.m_Type		= 		ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pCommonEquipTableData[i].m_ItemType.m_Index		= 		ThirdFile.Search_Posistion(i,TBItemIndex)->iValue;
		m_pCommonEquipTableData[i].m_EquipPoint				= 		ThirdFile.Search_Posistion(i,TBEquipPoint)->iValue;
		m_pCommonEquipTableData[i].m_RulerID				= 		ThirdFile.Search_Posistion(i,TBRulerID)->iValue;

		m_pCommonEquipTableData[i].m_EquipSetNum			= 		ThirdFile.Search_Posistion(i,TBEQuipSetNum)->iValue;
		m_pCommonEquipTableData[i].m_EquipSetMaxNum			= 		ThirdFile.Search_Posistion(i,TBEquipSetMaxNum)->iValue;
		m_pCommonEquipTableData[i].m_Job					= 		ThirdFile.Search_Posistion(i,TBJob)->iValue;
		m_pCommonEquipTableData[i].m_ReqLevel				= 		ThirdFile.Search_Posistion(i,TBReqLevel)->iValue;
		m_pCommonEquipTableData[i].m_BasePrice				= 		ThirdFile.Search_Posistion(i,TBBasePrice)->iValue;
		m_pCommonEquipTableData[i].m_MaxDur					= 		ThirdFile.Search_Posistion(i,TBMaxDur)->iValue;
		m_pCommonEquipTableData[i].m_CanRepaireTimes		= 		ThirdFile.Search_Posistion(i,TBMaxRepair)->iValue;
		
		INT iMaxGem = ThirdFile.Search_Posistion(i,TBMaxGem)->iValue;
		m_pCommonEquipTableData[i].m_MaxGem					= 		SAFE_VALUE(iMaxGem);



		//属性出现率的处理
		INT iAttrCount = 0;
		for(INT rate = 0; rate<IATTRIBUTE_NUMBER;rate++)
		{
			INT iValue	=0;
			INT iColumn = 0;

			if(rate>= IATTRIBUTE_BASE_ATTACK_P)
			{
				iColumn = TBAttrBegin+rate+(rate-IATTRIBUTE_BASE_ATTACK_P);
				Assert(iColumn<iTableColumn);
				iValue = ThirdFile.Search_Posistion(i,iColumn)->iValue; 

				if(iValue>0)
				{
					switch(rate) 
					{

					case IATTRIBUTE_BASE_ATTACK_P:
						{
							m_pCommonEquipTableData[i].m_PhysicAttack	=	iValue;
						}
						break;
					case IATTRIBUTE_BASE_ATTACK_M:
						{
							m_pCommonEquipTableData[i].m_MagicAttack	=	iValue;
						}
						break;
					case IATTRIBUTE_BASE_ATTACK_TIME:
						{
							m_pCommonEquipTableData[i].m_AttackSpeed	=	iValue;
						}
						break;
					case IATTRIBUTE_BASE_DEFENCE_P:
						{
							m_pCommonEquipTableData[i].m_PhysicDefense	=	iValue;
						}
						break;
					case IATTRIBUTE_BASE_DEFENCE_M:
						{	
							m_pCommonEquipTableData[i].m_MagicDefense	=	iValue;
						}
						break;
					case IATTRIBUTE_BASE_MISS:
						{
							m_pCommonEquipTableData[i].m_Miss			=	iValue;
						}
						break;
					default:
						{
							Assert(FALSE);
						}
						break;
					}
				}

			}
		

		}

	}
	
	Log::SaveLog( SERVER_LOGFILE, "InitCommonItemTable()  Ok!") ;
	
	__LEAVE_FUNCTION	
}


//加载绿色装备表
VOID	ItemTable::InitGreenItemTable()
{
	__ENTER_FUNCTION
	
	enum GreenItemTBData
	{
		TBIndex				= 0,
		TBClass				= 1,
		TBQuality			= 2,
		TBType				= 3,
		TBItemIndex			= 4,
		TBEquipPoint		= 5,
		TBVisual			= 6,
		TBRulerID			= 7,
		TBEQuipSetNum		= 8,
		TBEquipSetMaxNum	= 9,
		TBJob				= 10,
		TBEquipName			= 11,
		TBReqLevel			= 12,
		TBDesc				= 13,
		TBBasePrice			= 14,
		TBMaxDur			= 15,
		TBMaxRepair			= 16,
		TBMaxGem			= 17, 	
		TBAttrBegin			= 18,
		TBAttrEnd			= TBAttrBegin+IATTRIBUTE_NUMBER-1
	};


	DBCFile ThirdFile(0);
	
	BOOL ret = ThirdFile.OpenFromTXT( FILE_GREEN_EQUIP_BASE );
	Assert(ret);
	INT iTableCount = ThirdFile.GetRecordsNum();
	INT iTableColumn	= ThirdFile.GetFieldsNum();

	m_nGreenItemCount	    =	iTableCount;
	m_pGreenEquipTableData	=	new GREEN_EQUIP_TB[iTableCount];

	
	for(INT i =0;i<iTableCount;i++)
	{
		m_pGreenEquipTableData[i].m_IndexID				= 		ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pGreenEquipTableData[i].m_ItemType.m_Class	= 		ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pGreenEquipTableData[i].m_ItemType.m_Quality	= 		ThirdFile.Search_Posistion(i,TBQuality)->iValue;	 
		m_pGreenEquipTableData[i].m_ItemType.m_Type		= 		ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pGreenEquipTableData[i].m_ItemType.m_Index	= 		ThirdFile.Search_Posistion(i,TBItemIndex)->iValue;
		m_pGreenEquipTableData[i].m_EquipPoint			= 		ThirdFile.Search_Posistion(i,TBEquipPoint)->iValue;
		
		m_pGreenEquipTableData[i].m_RulerID				= 		ThirdFile.Search_Posistion(i,TBRulerID)->iValue;
		m_pGreenEquipTableData[i].m_EquipSetNum			= 		ThirdFile.Search_Posistion(i,TBEQuipSetNum)->iValue;
		m_pGreenEquipTableData[i].m_EquipSetMaxNum		= 		ThirdFile.Search_Posistion(i,TBEquipSetMaxNum)->iValue;
		m_pGreenEquipTableData[i].m_Job					= 		ThirdFile.Search_Posistion(i,TBJob)->iValue;
		m_pGreenEquipTableData[i].m_ReqLevel			= 		ThirdFile.Search_Posistion(i,TBReqLevel)->iValue;
		m_pGreenEquipTableData[i].m_BasePrice			= 		ThirdFile.Search_Posistion(i,TBBasePrice)->iValue;
		m_pGreenEquipTableData[i].m_MaxDur				= 		ThirdFile.Search_Posistion(i,TBMaxDur)->iValue;
		m_pGreenEquipTableData[i].m_CanRepaireTimes		= 		ThirdFile.Search_Posistion(i,TBMaxRepair)->iValue;
		
		INT iMaxGem = ThirdFile.Search_Posistion(i,TBMaxGem)->iValue;
		m_pGreenEquipTableData[i].m_MaxGem				= 		SAFE_VALUE(iMaxGem);

		//属性出现率的处理
		INT iAttrCount = 0;
		for(INT rate = 0; rate<IATTRIBUTE_NUMBER;rate++)
		{
			INT iValue	=	0;
			INT iColumn	=	0;
			
			if(rate>= IATTRIBUTE_BASE_ATTACK_P) //基础属性,最大最小值
			{
				iColumn	=	TBAttrBegin+rate+(rate-IATTRIBUTE_BASE_ATTACK_P);
				Assert(iColumn<iTableColumn);

				iValue = ThirdFile.Search_Posistion(i,iColumn)->iValue; 
		
				if(iValue>0)
				{
					switch(rate) 
					{

					case IATTRIBUTE_BASE_ATTACK_P:
						{
							m_pGreenEquipTableData[i].m_PhysicAttack  =	iValue;
						}
						break;
					case IATTRIBUTE_BASE_ATTACK_M:
						{
							m_pGreenEquipTableData[i].m_MagicAttack	  =	iValue;
						}
						break;
					case IATTRIBUTE_BASE_ATTACK_TIME:
						{
							m_pGreenEquipTableData[i].m_AttackSpeed	  =	iValue;
						}
						break;
					case IATTRIBUTE_BASE_DEFENCE_P:
						{
							m_pGreenEquipTableData[i].m_PhysicDefense = iValue;
						}
						break;
					case IATTRIBUTE_BASE_DEFENCE_M:
						{	
							m_pGreenEquipTableData[i].m_MagicDefense =	iValue;
						}
						break;
					case IATTRIBUTE_BASE_MISS:
						{
							m_pGreenEquipTableData[i].m_Miss		 =	iValue;
						}
						break;
					default:
						{
							Assert(FALSE);
						}
						break;
					}
				}
			}
			else
			{
				iColumn	=	TBAttrBegin+rate;
				Assert(iColumn<iTableColumn);

				iValue = ThirdFile.Search_Posistion(i,TBAttrBegin+rate)->iValue; 

				if(iValue>0 && iAttrCount<MAX_GREEN_ITEM_EXT_ATTR)
				{
					
					m_pGreenEquipTableData[i].m_Attr[iAttrCount].m_AttrType				=	(ITEM_ATTRIBUTE)rate;
					m_pGreenEquipTableData[i].m_Attr[iAttrCount].m_Value.m_Value		=	iValue;
					iAttrCount++;
				}
			}

			
			
		}

	}
	Log::SaveLog( SERVER_LOGFILE, "InitGreenItemTable()  Ok!") ;
	__LEAVE_FUNCTION
}

//加载黄金装备表
VOID ItemTable::InitGoldenItemTable()
{
	__ENTER_FUNCTION

		Log::SaveLog( SERVER_LOGFILE, "InitGoldenItemTable()  Ok!") ;
	__LEAVE_FUNCTION

}


VOID	ItemTable::InitBlueItemTable()
{
	__ENTER_FUNCTION
	
	enum BlueItemTBData
	{
		TBIndex				= 0,
		TBClass				= 1,
		TBQuality			= 2,
		TBType				= 3,
		TBItemIndex			= 4,
		TBEquipPoint		= 5,
		TBVisual			= 6,
		TBRulerID			= 7,
		TBEQuipSetNum		= 8,
		TBEquipSetMaxNum	= 9,
		TBJob				= 10,
		TBEquipName			= 11,
		TBReqLevel			= 12,
		TBDesc				= 13,
		TBBasePrice			= 14,
		TBMaxDur			= 15,
		TBMaxRepair			= 16,
		TBMaxGem			= 17, 	
		TBAttrBegin			= 18,
		TBAttrEnd			= TBAttrBegin+IATTRIBUTE_NUMBER-1
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_BLUE_EQUIP_BASE );
	
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nBlueItemCount	    =	iTableCount;
	m_pBlueEquipTableData	=	new BLUE_EQUIP_TB[iTableCount];

	for(INT i =0;i<iTableCount;i++)
	{
		m_pBlueEquipTableData[i].m_IndexID				= ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pBlueEquipTableData[i].m_ItemType.m_Class		= ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pBlueEquipTableData[i].m_ItemType.m_Quality	= ThirdFile.Search_Posistion(i,TBQuality)->iValue;	 
		m_pBlueEquipTableData[i].m_ItemType.m_Type		= ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pBlueEquipTableData[i].m_ItemType.m_Index		= ThirdFile.Search_Posistion(i,TBItemIndex)->iValue;
		m_pBlueEquipTableData[i].m_EquipPoint			= ThirdFile.Search_Posistion(i,TBEquipPoint)->iValue;

		m_pBlueEquipTableData[i].m_RulerID				= 		ThirdFile.Search_Posistion(i,TBRulerID)->iValue;
		m_pBlueEquipTableData[i].m_EquipSetNum			= 		ThirdFile.Search_Posistion(i,TBEQuipSetNum)->iValue;
		m_pBlueEquipTableData[i].m_EquipSetMaxNum		= 		ThirdFile.Search_Posistion(i,TBEquipSetMaxNum)->iValue;
		m_pBlueEquipTableData[i].m_Job					= 		ThirdFile.Search_Posistion(i,TBJob)->iValue;
		m_pBlueEquipTableData[i].m_ReqLevel				= 		ThirdFile.Search_Posistion(i,TBReqLevel)->iValue;
		m_pBlueEquipTableData[i].m_BasePrice			= 		ThirdFile.Search_Posistion(i,TBBasePrice)->iValue;
		m_pBlueEquipTableData[i].m_MaxDur				= 		ThirdFile.Search_Posistion(i,TBMaxDur)->iValue;
		m_pBlueEquipTableData[i].m_CanRepaireTimes		= 		ThirdFile.Search_Posistion(i,TBMaxRepair)->iValue;

		INT iMaxGem = ThirdFile.Search_Posistion(i,TBMaxGem)->iValue;
		m_pBlueEquipTableData[i].m_MaxGem				= 		SAFE_VALUE(iMaxGem);

		
		//属性出现率的处理
		for(INT rate = 0; rate<IATTRIBUTE_NUMBER;rate++)
		{
				INT iColumn = 0;

				if(rate>=IATTRIBUTE_BASE_ATTACK_P) //基础属性写死
				{

					m_pBlueEquipTableData[i].m_AttrRate[rate] = -1;
					
					
					iColumn	=	TBAttrBegin+rate+(rate-IATTRIBUTE_BASE_ATTACK_P);

					Assert(iColumn<iTableColumn);
					INT iValue = ThirdFile.Search_Posistion(i,iColumn)->iValue; 
					
					if(iValue>0)
					{
						switch(rate) 
						{

						case IATTRIBUTE_BASE_ATTACK_P:
							{
								m_pBlueEquipTableData[i].m_PhysicAttack =	iValue;
							}
							break;
						case IATTRIBUTE_BASE_ATTACK_M:
							{
								m_pBlueEquipTableData[i].m_MagicAttack	=	iValue;
							}
							break;
						case IATTRIBUTE_BASE_ATTACK_TIME:
							{
								m_pBlueEquipTableData[i].m_AttackSpeed	=	iValue;
							}
							break;
						case IATTRIBUTE_BASE_DEFENCE_P:
							{
								m_pBlueEquipTableData[i].m_PhysicDefense = iValue;
							}
							break;
						case IATTRIBUTE_BASE_DEFENCE_M:
							{	
                                m_pBlueEquipTableData[i].m_MagicDefense =	iValue;
							}
							break;
						case IATTRIBUTE_BASE_MISS:
							{
								m_pBlueEquipTableData[i].m_Miss			=	iValue;
							}
							break;
						default:
							{
								Assert(FALSE);
							}
							break;
						}
					}
						
				}
				else
				{
					iColumn	=	TBAttrBegin+rate;
					Assert(iColumn<iTableColumn);
					m_pBlueEquipTableData[i].m_AttrRate[rate] = ThirdFile.Search_Posistion(i,iColumn)->iValue; 
				}
		}
	}
	Log::SaveLog( SERVER_LOGFILE, "InitBlueItemTable()  Ok!") ;

	__LEAVE_FUNCTION
}

VOID	ItemTable::InitItemLevelTable()
{
	__ENTER_FUNCTION

	enum ItemLevelTBData
    {
		TBItemLevel				=	0,
		TBItemQualityBegin		=	1
	};
	

	DBCFile ThirdFile(0);

	BOOL ret = ThirdFile.OpenFromTXT( FILE_ITEM_QUALITY );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nItemLevelCount	    =	iTableCount;
	m_pItemQualityData		=	new ITEM_QUALITY_TB[iTableCount];
	
	INT iColumn = 0;

	for(uint i = 0;i<m_nItemLevelCount;i++)
	{

		m_pItemQualityData[i].m_ItemLevel	=	i;

		
		for(INT j=0;j<MAX_ITEM_TABLE_QUALITYS;j++)
		{
			iColumn	=	TBItemQualityBegin+j;
			Assert(iColumn<iTableColumn);

			m_pItemQualityData[i].m_Quality[j]	= ThirdFile.Search_Posistion(i,iColumn)->iValue;
		}
	}
	Log::SaveLog( SERVER_LOGFILE, "InitItemLevelTable()  Ok!") ;
	
	__LEAVE_FUNCTION
}


VOID	ItemTable::InitItemValueTable()
{
	__ENTER_FUNCTION
	
	enum	ItemValueTBData
	{
		TBIndex				=		0,
		TBName				=		1,
		TBItemValueBegin	=		2
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_ITEM_VALUE );
	Assert(ret);

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iTableColumn		=	ThirdFile.GetFieldsNum();

	m_nValueCount			=	iTableCount;
	m_pItemValueData		=	new ITEM_VALUE_TB[iTableCount];

	INT iColumn = 0;

	for(uint i = 0;i<m_nValueCount;i++)
	{
		m_pItemValueData[i].m_ValueType = ThirdFile.Search_Posistion(i,TBIndex)->iValue;

		for(INT j=0;j<MAX_ITEM_TABLE_VALUES;j++)
		{
			iColumn		=	TBItemValueBegin+j*2;
			Assert(iColumn<iTableColumn);
			m_pItemValueData[i].m_Values[j].m_minValue = ThirdFile.Search_Posistion(i,iColumn)->iValue;

			iColumn		=	TBItemValueBegin+j*2+1;
			Assert(iColumn<iTableColumn);
			m_pItemValueData[i].m_Values[j].m_maxValue = ThirdFile.Search_Posistion(i,iColumn)->iValue;
		}
	}
	
	Log::SaveLog( SERVER_LOGFILE, "InitItemValueTable()  Ok!") ;

	__LEAVE_FUNCTION
}


VOID	ItemTable::InitGemInfoTable()
{
	__ENTER_FUNCTION

	enum	GemInfoTBData
	{	
		TBTableIndex		=	0,
		TBClass				=	1,
		TBQuality			=	2,
		TBType				=	3,
		TBGemIndex			=	4,
		TBResID				=	5,
		TBRulerID			=	6,
		TBName				=	7,
		TBDesc				=	8,
		TBPrice				=	9,
        TBEffectBegin		=	10				
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_GEM_INFO );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nGemCount			=	iTableCount;
	m_pGemInfoData		=	new GEMINFO_TB[iTableCount];
	
	INT iColumn			=	0;
	for(uint i = 0;i<m_nGemCount;i++)
	{
		
		m_pGemInfoData[i].m_nTableIndex			= ThirdFile.Search_Posistion(i,TBTableIndex)->iValue;
		m_pGemInfoData[i].m_ItemType.m_Class	= ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pGemInfoData[i].m_ItemType.m_Quality	= ThirdFile.Search_Posistion(i,TBQuality)->iValue;
		m_pGemInfoData[i].m_ItemType.m_Type		= ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pGemInfoData[i].m_ItemType.m_Index	= ThirdFile.Search_Posistion(i,TBGemIndex)->iValue;
		m_pGemInfoData[i].m_ResourceID			= ThirdFile.Search_Posistion(i,TBResID)->iValue;
		m_pGemInfoData[i].m_nRulerID			= ThirdFile.Search_Posistion(i,TBRulerID)->iValue;
		m_pGemInfoData[i].m_nPrice				= ThirdFile.Search_Posistion(i,TBPrice)->iValue;

		for(uint j = 0; j<IATTRIBUTE_NUMBER;j++)
		{
			iColumn		=	TBEffectBegin+j;

			Assert(iColumn<iTableColumn);

			INT iValue = ThirdFile.Search_Posistion(i,iColumn)->iValue;

			if(iValue>0)
			{
				m_pGemInfoData[i].m_GenAttr.m_AttrType = (ITEM_ATTRIBUTE)j;
				m_pGemInfoData[i].m_GenAttr.m_Value.m_Value = iValue;
				j = IATTRIBUTE_NUMBER;
			}

		}
		
		
	}
	
	
	Log::SaveLog( SERVER_LOGFILE, "InitGemInfoTable()  Ok!") ;
	__LEAVE_FUNCTION
}
VOID	ItemTable::InitCommItemInfoTable()
{
	__ENTER_FUNCTION
	
	enum	CommonItemInfoTBData
	{
		TBIndex					=		0,
		TBClass					=		1,
		TBQuality				=		2,
		TBType					=		3,
		TBMedicIndex			=		4,
		TBResourceID			=		5,
		TBName					=		6,
		TBDesc					=		7,
		TBLevel					=		8,
		TBBasePrice				=		9,
		TBRulerID				=		10,
		TBTileNumber			=		11,
		TBScriptID				=		12,
		TBSkillID				=		13,
		TBCosSelf				=		14,
		TBReqSkill				=		15,
		TBReqSkillLevel			=		16,
		TBTargetType			=		17
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_COMMON_ITEM );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nCommItemCount			=	iTableCount;
	m_pCommItemInfoData		=	new COMMITEM_INFO_TB[iTableCount];
	
	INT iColumn		=	0;
	
	Assert(TBTargetType<iTableColumn);
	for(uint i = 0;i<m_nCommItemCount;i++)
	{
		m_pCommItemInfoData[i].m_nTableIndex					= ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pCommItemInfoData[i].m_ItemType.m_Class				= ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pCommItemInfoData[i].m_ItemType.m_Quality				= ThirdFile.Search_Posistion(i,TBQuality)->iValue;
		m_pCommItemInfoData[i].m_ItemType.m_Type				= ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pCommItemInfoData[i].m_ItemType.m_Index				= ThirdFile.Search_Posistion(i,TBMedicIndex)->iValue;

		m_pCommItemInfoData[i].m_nLevel							= ThirdFile.Search_Posistion(i,TBLevel)->iValue;
		m_pCommItemInfoData[i].m_nBasePrice						= ThirdFile.Search_Posistion(i,TBBasePrice)->iValue;
		m_pCommItemInfoData[i].m_nRulerID						= ThirdFile.Search_Posistion(i,TBRulerID)->iValue;	
		m_pCommItemInfoData[i].m_nLayedNum						= ThirdFile.Search_Posistion(i,TBTileNumber)->iValue;
		m_pCommItemInfoData[i].m_nScriptID						= ThirdFile.Search_Posistion(i,TBScriptID)->iValue;	 		
		m_pCommItemInfoData[i].m_nSkillID						= ThirdFile.Search_Posistion(i,TBSkillID)->iValue;	 		
		if(ThirdFile.Search_Posistion(i,TBCosSelf)->iValue ==1)
			m_pCommItemInfoData[i].m_bCosSelf							= TRUE;
		m_pCommItemInfoData[i].m_nReqSkill						= ThirdFile.Search_Posistion(i,TBReqSkill)->iValue;	 		
		m_pCommItemInfoData[i].m_nReqSkillLevel					= ThirdFile.Search_Posistion(i,TBReqSkillLevel)->iValue;	 		
		m_pCommItemInfoData[i].m_TargetType						= ThirdFile.Search_Posistion(i,TBTargetType)->iValue;	 		

	
	}

	Log::SaveLog( SERVER_LOGFILE, "InitCommonItemInfoTable()  Ok!") ;
	__LEAVE_FUNCTION

}

VOID	ItemTable::InitItemRulerTable()
{
	__ENTER_FUNCTION

	enum	ItemRulerTBData
	{
		TBRulerIndex	=	0,
		TBDiscard		=	1,
		TBTile			=	2,
		TBShortCut		=	3,
		TBSell			=	4,
		TBExchange		=	5,
		TBUse			=	6,
		TBPickBind		=	7,
		TBEquipBind		=	8,
		TBUnique		=	9
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_ITEM_RULE );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nItemRulerCount		=	iTableCount;
	m_pItemRulerData		=	new ITEM_RULER_TB[iTableCount];

	INT iColumn		=	0;


	Assert(TBUnique<iTableColumn);

	for(uint i = 0;i<m_nItemRulerCount;i++)
	{
		static BOOL bInit = FALSE;		
		
		m_pItemRulerData[i].m_RulerIndex	=	ThirdFile.Search_Posistion(i,TBRulerIndex)->iValue;
		
		if(!bInit)
		{
			m_nHashOffSet	=	m_pItemRulerData[i].m_RulerIndex - i;
		}
		Assert(m_pItemRulerData[i].m_RulerIndex == i+m_nHashOffSet);//连续性检查

		m_pItemRulerData[i].m_Discard		=	ThirdFile.Search_Posistion(i,TBDiscard)->iValue	== 1 ;
		m_pItemRulerData[i].m_Tile			=	ThirdFile.Search_Posistion(i,TBTile)->iValue	== 1;
		m_pItemRulerData[i].m_ShortCut		=	ThirdFile.Search_Posistion(i,TBShortCut)->iValue	== 1;
		m_pItemRulerData[i].m_CanSell		=	ThirdFile.Search_Posistion(i,TBSell)->iValue	==1;
		m_pItemRulerData[i].m_CanExchange	=	ThirdFile.Search_Posistion(i,TBExchange)->iValue	== 1;
		m_pItemRulerData[i].m_CanUse		=	ThirdFile.Search_Posistion(i,TBUse)->iValue	== 1;
		m_pItemRulerData[i].m_PickBind		=	ThirdFile.Search_Posistion(i,TBPickBind)->iValue == 1;
		m_pItemRulerData[i].m_EquipBind		=	ThirdFile.Search_Posistion(i,TBEquipBind)->iValue == 1;
		m_pItemRulerData[i].m_Unique		=	ThirdFile.Search_Posistion(i,TBUnique)->iValue	== 1;
	}
	
	Log::SaveLog( SERVER_LOGFILE, "InitItemRulerTable()  Ok!") ;

	__LEAVE_FUNCTION
}

VOID	ItemTable::InitStoreMapTable()
{
	__ENTER_FUNCTION	

	enum	StoreMapTBData
	{
		TBIndex					=		0,
		TBClass					=		1,
		TBQuality				=		2,
		TBType					=		3,
		TBMapIndex				=		4,
		TBResourceID			=		5,
		TBName					=		6,
		TBDesc					=		7,
		TBLevel					=		8,
		TBRulerID				=		9,
		TBXPos					=		10,
		TBZPos					=		11,
		TBSceneID				=		12,
		TBGrowPointType			=		13,
		TBUseScript				=		14
	};

	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_STORE_MAP_ITEM );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT	iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nStoreMapCount		=	iTableCount;
	m_pStoreMapData			=	new STORE_MAP_INFO_TB[iTableCount];

	INT iColumn		=	0;

	Assert(TBSceneID<iTableColumn);

	for(uint i = 0 ;i<m_nStoreMapCount;i++)
	{
		m_pStoreMapData[i].m_nTableIndex		=	ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		m_pStoreMapData[i].m_ItemType.m_Class	=	ThirdFile.Search_Posistion(i,TBClass)->iValue;
		m_pStoreMapData[i].m_ItemType.m_Quality	=	ThirdFile.Search_Posistion(i,TBQuality)->iValue;
		m_pStoreMapData[i].m_ItemType.m_Type	=	ThirdFile.Search_Posistion(i,TBType)->iValue;
		m_pStoreMapData[i].m_ItemType.m_Index	=	ThirdFile.Search_Posistion(i,TBMapIndex)->iValue;
		m_pStoreMapData[i].m_ResourceID			=	ThirdFile.Search_Posistion(i,TBResourceID)->iValue;
		m_pStoreMapData[i].m_nLevel				=	ThirdFile.Search_Posistion(i,TBLevel)->iValue;
		m_pStoreMapData[i].m_nRulerID			=	ThirdFile.Search_Posistion(i,TBRulerID)->iValue;
		m_pStoreMapData[i].m_xPos				=	ThirdFile.Search_Posistion(i,TBXPos)->fValue;
		m_pStoreMapData[i].m_zPos				=	ThirdFile.Search_Posistion(i,TBZPos)->fValue;
		m_pStoreMapData[i].m_SceneID			=	ThirdFile.Search_Posistion(i,TBSceneID)->iValue;
		m_pStoreMapData[i].m_GrowPointType		=	ThirdFile.Search_Posistion(i,TBGrowPointType)->iValue;
		m_pStoreMapData[i].m_ScriptID			=	ThirdFile.Search_Posistion(i,TBUseScript)->iValue;

	}
	
	__LEAVE_FUNCTION
}


ITEM_RULER_TB*	ItemTable::GetRuleValueByID(INT& iIndex)
{

	__ENTER_FUNCTION

	INT iOffSet = iIndex-m_nHashOffSet;
	
	if((iOffSet>=0)&&(iOffSet<(INT)m_nItemRulerCount))
	{
		return &m_pItemRulerData[iOffSet];
	}

	return NULL;

	__LEAVE_FUNCTION

	return NULL;

}



VOID	ItemTable::InitDropBoxTable()
{
	__ENTER_FUNCTION
	
	enum  DropBoxTBData
	{
		TBDropBoxId			=		0,
		TBDropValue			=		1,
		TBDropBoxBegin		=		2
		
	};
	
	DBCFile ThirdFile(0);
	
	BOOL ret = ThirdFile.OpenFromTXT( FILE_DROP_BOX_CONTENT );
	
	Assert(ret);
	
	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nDropBoxCount	    =	iTableCount;
	m_pDropBoxTableData	=	new DROP_BOX_TB[iTableCount];

	INT iColumn		=	0;
	INT iQuality	=	0;
	INT iValue;
	for(INT i =0;i<iTableCount;i++)
	{
		m_pDropBoxTableData[i].m_DropBoxId		= ThirdFile.Search_Posistion(i,TBDropBoxId)->iValue;
		m_pDropBoxTableData[i].m_DropValue		= ThirdFile.Search_Posistion(i,TBDropValue)->iValue;
		for(INT j =0;j<MAX_DROPBOX_CARRAGE;j++)
		{
			iColumn		=	TBDropBoxBegin+2*j;
			Assert(iColumn<iTableColumn);
			
			iValue	=ThirdFile.Search_Posistion(i,iColumn)->iValue;
			if(iValue!=-1)
			{
				TSerialHelper   help(iValue);
				m_pDropBoxTableData[i].m_DropItem[j].m_Class	= help.GetItemClass();
				m_pDropBoxTableData[i].m_DropItem[j].m_Index	= help.GetItemIndex();
				m_pDropBoxTableData[i].m_DropItem[j].m_Quality	= help.GetItemQual();
				m_pDropBoxTableData[i].m_DropItem[j].m_Type		= help.GetItemType();
			}
			

			iColumn		= TBDropBoxBegin+2*j+1;
			Assert(iColumn<iTableColumn);
			
			iQuality		=	ThirdFile.Search_Posistion(i,iColumn)->iValue;
			
			m_pDropBoxTableData[i].m_Quality[j]	=	iQuality;

		}

	}
	
	Log::SaveLog( SERVER_LOGFILE, "InitDropBoxTable()  Ok!") ;
	
	__LEAVE_FUNCTION
}

VOID	ItemTable::InitMonsterDropBoxTable()
{
	__ENTER_FUNCTION
	
	enum	MonsterDropBoxTBData
	{
		TBMonsterID					=			0,
		TBMonsterValue				=			1,
		TBMonsterDropType			=			2,
		TBMonsterDropBoxBegin		=			3
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MONSTER_DROP_BOXS );
	Assert(ret);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();

	Assert(iTableCount>0);

	m_nMonsterDropBoxCount	    =	iTableCount;
	m_pMonsterDropTableData		=	new	MONSTER_DROPBOX_TB[iTableCount];

	INT iColumn		=		0;
	INT iValue;
	for(INT i =0;i<iTableCount;i++)
	{
		m_pMonsterDropTableData[i].m_MonsterId		=	ThirdFile.Search_Posistion(i,TBMonsterID)->iValue;
		m_pMonsterDropTableData[i].m_MonsterValue	=	ThirdFile.Search_Posistion(i,TBMonsterValue)->iValue;
		m_pMonsterDropTableData[i].m_DropType		=	ThirdFile.Search_Posistion(i,TBMonsterDropType)->iValue;

		for(INT j= 0 ;j<MAX_MONSTER_DROPBOX;j++)
		{
			iColumn		=		TBMonsterDropBoxBegin+j;
			Assert(iColumn<iTableColumn);
			iValue	=	ThirdFile.Search_Posistion(i,iColumn)->iValue;	
			//if(iValue!=-1)
			m_pMonsterDropTableData[i].m_DropBoxs.m_DropBox[j] =	iValue;
		}
	}


	Log::SaveLog( SERVER_LOGFILE, "InitMonsterDropBoxTable()  Ok!") ;

	__LEAVE_FUNCTION
}


VOID	ItemTable::InitDropAttTable()
{
	__ENTER_FUNCTION
	
	enum	DropAttTBData
	{
			TBDeltaLevel			=		0,
			TBRate					=		1
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_DROP_ATTENUATION );
	Assert(ret);
		
	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();

	m_nDropAttCount		=	iTableCount;
	m_pDropAttData		=	new DROP_ATT_TB[iTableCount];

	Assert(iTableColumn>TBRate);
	Assert(m_pDropAttData);

	for(INT i = 0 ;i<iTableCount;i++)
	{
		m_pDropAttData[i].m_DeltaLevel		=	ThirdFile.Search_Posistion(i,TBDeltaLevel)->iValue;
		m_pDropAttData[i].m_AttValue		=	ThirdFile.Search_Posistion(i,TBRate)->fValue;	
	}


	Log::SaveLog( SERVER_LOGFILE, "InitDropAttTable()  Ok!") ;

	__LEAVE_FUNCTION
}




VOID	ItemTable::InitEquipSetAttrTable()
{
	__ENTER_FUNCTION
		
	enum	EquipSetAttr
	{
		TBEquipSetSerial		=		0,
		TBEquipSetAttrBegin		=		1
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_EQUIP_SET_ATTR );
	Assert(ret);
	
	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();
	
	m_nEquipSetCount	=	iTableCount;

	m_pEquipSetData		=	new EQUIP_SET_TB[iTableCount];
	
	Assert(iTableColumn>TBEquipSetAttrBegin);
	Assert(m_pEquipSetData);
	
	INT		iColumn			=	0;
	INT		iValue			=	0;

	for(uint i =0;i<m_nEquipSetCount;i++)
	{
		m_pEquipSetData[i].m_nEquipSetSerial	=	ThirdFile.Search_Posistion(i,TBEquipSetSerial)->iValue;
		
		INT iAttrCount =	0;

		for(INT j = 0;j<IATTRIBUTE_NUMBER;j++)
		{
			iColumn		=	TBEquipSetAttrBegin+j;
			
			Assert(iColumn<iTableColumn);
			
			iValue		=	ThirdFile.Search_Posistion(i,iColumn)->iValue;
			
			if((iValue>0)&&(iAttrCount<MAX_ITEM_SET_ATTR))
			{
				m_pEquipSetData[i].m_ItemAttr[iAttrCount].m_AttrType			=	(ITEM_ATTRIBUTE)j;
				m_pEquipSetData[i].m_ItemAttr[iAttrCount].m_Value.m_Value		=	iValue;
				iAttrCount++;
			}
			
		}

		m_pEquipSetData[i].m_nAttrCount	= iAttrCount;
	}

	__LEAVE_FUNCTION
}


MONSTER_DROPBOX_TB*	ItemTable::GetMonsterDropTB(INT iMonsterType)
{
	__ENTER_FUNCTION
	
		MONSTER_DROPBOX_TB	tb;
		tb.m_MonsterId	=	iMonsterType;
		
		MONSTER_DROPBOX_TB* pResult  = (MONSTER_DROPBOX_TB*)bsearch(&tb,
										m_pMonsterDropTableData,
										m_nMonsterDropBoxCount,
										sizeof(MONSTER_DROPBOX_TB),
										(INT (*)(const VOID*, const VOID*))CompareMonsterDropTB);

		if(pResult)
			return pResult;
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Search Obj_Monster DropBox MonsterType:%d Get Errors", 
				iMonsterType ) ;
		}

		
	__LEAVE_FUNCTION

	return 0;
}


DROP_BOX_TB*	ItemTable::GetDropBoxTB(INT iDropBox)
{
	__ENTER_FUNCTION

		DROP_BOX_TB	tb;
		tb.m_DropBoxId	=	iDropBox;

		DROP_BOX_TB* pResult  = (DROP_BOX_TB*)bsearch(&tb,
			m_pDropBoxTableData,
			m_nDropBoxCount,
			sizeof(DROP_BOX_TB),
			(INT (*)(const VOID*, const VOID*))CompareDropBoxTB);

		if(pResult)
			return pResult;
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Search DropBoxs DropBoxId:%d Get Errors", 
				iDropBox ) ;
		}

	__LEAVE_FUNCTION
		return 0;
}

GREEN_EQUIP_TB*		ItemTable::GetGreenItemTB(UINT ItemSerial)
{
	__ENTER_FUNCTION
		GREEN_EQUIP_TB	tb;
		tb.m_IndexID	=	ItemSerial;

		GREEN_EQUIP_TB* pResult	=	(GREEN_EQUIP_TB*)bsearch(&tb,
															m_pGreenEquipTableData,
															m_nGreenItemCount,
															sizeof(GREEN_EQUIP_TB),
															(INT (*)(const VOID*, const VOID*))CompareGreenItemTB);
		if(pResult)
			return pResult;

		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Search GreenEquip ItemSerial:%d Get Errors", 
				ItemSerial) ;
		}

	__LEAVE_FUNCTION
		return 0;
}

COMMON_EQUIP_TB*	ItemTable::GetWhiteItemTB(UINT ItemSerial)
{
	__ENTER_FUNCTION
		COMMON_EQUIP_TB	tb;
		tb.m_IndexID		=	ItemSerial;

		COMMON_EQUIP_TB*	pResult	=	(COMMON_EQUIP_TB*)bsearch(&tb,
																  m_pCommonEquipTableData,
																  m_nCommonItemCount,
																  sizeof(COMMON_EQUIP_TB),
																  (INT (*)(const VOID*, const VOID*))CompareCommonEquipTB);
		if(pResult)
			return	pResult;
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Search CommonEquip ItemSerial:%d  Get Errors", 
				ItemSerial) ;
		}
		

	__LEAVE_FUNCTION
		return	0;
}

BLUE_EQUIP_TB*		ItemTable::GetBlueItemTB(UINT ItemSerial)
{
		__ENTER_FUNCTION

		BLUE_EQUIP_TB	tb;
		tb.m_IndexID		=	ItemSerial;

		BLUE_EQUIP_TB*	pResult	=	(BLUE_EQUIP_TB*)bsearch(&tb,
															m_pBlueEquipTableData,
															m_nBlueItemCount,
															sizeof(BLUE_EQUIP_TB),
															(INT (*)(const VOID*, const VOID*))CompareBlueItemTB);
	
		if(pResult)
			return	pResult;
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Search BlueEquip ItemSerial:%d  Get Errors", 
				ItemSerial) ;
		}


	__LEAVE_FUNCTION
		return	0;
}

ITEM_QUALITY_TB*		ItemTable::GetItemQualityTB(INT ItemLevel)
{
	__ENTER_FUNCTION

		ITEM_QUALITY_TB tb;
		tb.m_ItemLevel		=		ItemLevel;
		
		ITEM_QUALITY_TB*	pResult	= (ITEM_QUALITY_TB*)bsearch(&tb,
																m_pItemQualityData,
																m_nItemLevelCount,
																sizeof(ITEM_QUALITY_TB),
																(INT (*)(const VOID*, const VOID*))CompareItemQualityTB);
		if(pResult)
				return	pResult;
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Search GetItemQualityTB ItemLevel:%d Get Errors", 
				ItemLevel) ;
		}
							

	__LEAVE_FUNCTION
		return	0;
}

MINMAX_VALUE		ItemTable::GetItemValue(ITEM_ATTRIBUTE iAtt,INT QualityLevel)
{
		MINMAX_VALUE	mValue;

	__ENTER_FUNCTION
		

		mValue.m_minValue		=	m_pItemValueData[iAtt].m_Values[QualityLevel].m_minValue;
		mValue.m_maxValue		=	m_pItemValueData[iAtt].m_Values[QualityLevel].m_maxValue;

	__LEAVE_FUNCTION

		return	mValue;	
}

DROP_ATT_TB*	ItemTable::GetDropAttTB(INT iDeltaLevel)
{
	__ENTER_FUNCTION
		DROP_ATT_TB	tb;

		tb.m_DeltaLevel	=	iDeltaLevel;


		DROP_ATT_TB*	pResult		=	(DROP_ATT_TB*)bsearch(&tb,
															  m_pDropAttData,
															  m_nDropAttCount,
															  sizeof(DROP_ATT_TB),
															  (INT (*)(const VOID*, const VOID*))CompareDropAttTB);

		if(pResult)	
			return pResult;
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Search DropAttTB Delta Level: %d Get Errors", 
				iDeltaLevel) ;
		}

	__LEAVE_FUNCTION
		return	0;
}

GEMINFO_TB*		ItemTable::GetGemInfoTB(UINT TableIndex)
{
	__ENTER_FUNCTION
	GEMINFO_TB tb;

	tb.m_nTableIndex	= TableIndex;


	GEMINFO_TB* pResult  =  (GEMINFO_TB*)bsearch(&tb,
												m_pGemInfoData,
												m_nGemCount,
												sizeof(GEMINFO_TB),
												(INT (*)(const VOID*, const VOID*))CompareGemInfoTBByTI);

	if(pResult)
		return pResult;
	else
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "Search GemInfoTB Index:%d  Get Errors", 
			TableIndex) ;

	}


	__LEAVE_FUNCTION
	return	0;
}


COMMITEM_INFO_TB*		ItemTable::GetCommItemInfoTB(UINT	itemSerial)
{
	__ENTER_FUNCTION

		COMMITEM_INFO_TB tb;
			tb.m_nTableIndex		=	itemSerial;
		
		COMMITEM_INFO_TB* pResult = (COMMITEM_INFO_TB*)bsearch(&tb,
														 m_pCommItemInfoData,
														 m_nCommItemCount,
														 sizeof(COMMITEM_INFO_TB),
														(INT (*)(const VOID*, const VOID*))CompareCommItemInfoTB);
		if(pResult)
				return	pResult;
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Search CommItemInfoTB itemSerial:%d  Type:%d Get Errors", 
						itemSerial) ;
			return 0;
		}

	__LEAVE_FUNCTION
		return 0;

}

STORE_MAP_INFO_TB* ItemTable::GetStoreMapTB(UINT itemSerial)
{
	__ENTER_FUNCTION

		STORE_MAP_INFO_TB	tb;
		tb.m_nTableIndex	=	 itemSerial;

		STORE_MAP_INFO_TB* pResult = (STORE_MAP_INFO_TB*)bsearch(&tb,
																m_pStoreMapData,
																m_nStoreMapCount,
																sizeof(STORE_MAP_INFO_TB),
																(INT (*)(const VOID*, const VOID*))CompareStoreMapInfoTB);
		if(pResult)
			return		pResult;
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Search StoreMapInfoTB itemSerial:%d Get Errors", 
						itemSerial) ;
		}

	__LEAVE_FUNCTION
		return	0;
}
EQUIP_SET_TB*		ItemTable::GetEquipSetTB(INT EquipSetSerial)
{
	__ENTER_FUNCTION
		
		EQUIP_SET_TB	tb;

		tb.m_nEquipSetSerial	=	EquipSetSerial;

		EQUIP_SET_TB* pResult = (EQUIP_SET_TB*)bsearch(&tb,
														m_pEquipSetData,
														m_nEquipSetCount,
														sizeof(EQUIP_SET_TB),
														(INT (*)(const VOID*, const VOID*))CompareEquipSetTB);

		if(pResult)
				return pResult;
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Search EquipSetTB Serial:%d Get Errors", 
				EquipSetSerial) ;
		}

	__LEAVE_FUNCTION
		return	0;
}


BYTE	GetItemTileMax(_ITEM_TYPE& it)
{
	__ENTER_FUNCTION

	if(it.m_Class==ICLASS_EQUIP)
	{
		return 1;
	}
	else if(it.m_Class==ICLASS_GEM)
	{
		return 1;
	}
	else if(it.m_Class==ICLASS_STOREMAP)
	{
		return 1;
	}
	else if(it.m_Class==ICLASS_COMITEM)
	{
		COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(it.ToSerial());
		if(pGET)
		{
			return pGET->m_nLayedNum;
		}
		else
		{
			Assert(FALSE);
			return 0;
		}
	}
	else if(it.m_Class==ICLASS_MATERIAL)
	{
		COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(it.ToSerial());
		if(pGET)
		{
			return pGET->m_nLayedNum;
		}
		else
		{
			Assert(FALSE);
			return 0;
		}
	}
	else if(it.m_Class==ICLASS_TASKITEM)
	{
		COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(it.ToSerial());
		if(pGET)
		{
			return pGET->m_nLayedNum;
		}
		else
		{
			Assert(FALSE);
			return 0;
		}
	}
	else
	{
		AssertEx(FALSE,"Not Implement Type in GetItemTileMax()");	// Not Implement Type
		return 0;
	}

	__LEAVE_FUNCTION
	return 0 ;
}
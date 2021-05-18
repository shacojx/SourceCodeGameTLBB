#include "StdAfx.h"
#include ".\hairmodelmng.h"
#include "..\dbc\GMDataBase.h"
#include "GameProcedure.h"
#include "TDException.h"

CHairModelMng::CHairModelMng(void)
{
}

CHairModelMng::~CHairModelMng(void)
{
}

// 得到主角发型个数
//
// iRace = 0 - 女
//         1 - 男
// iIndex = 发型信息索引
int CHairModelMng::GetHairModelCount(int iRace)
{
	switch(iRace)
	{
	case 0:
		{
			
			return GetWomanHairModelCount();
			break;
		}
	case 1:
		{
			return GetManHairModelCount();
			break;
		}
	}
	return 0;
}

	
// 得到男主角发型个数
int CHairModelMng::GetManHairModelCount()
{

	// 男主角发形个数
	return  m_iManHairModelCount;
	
}


// 得到女主角发型个数
int CHairModelMng::GetWomanHairModelCount()
{
	// 女主角发形个数
	return m_iWomanHairModelCount;
}


// 得到主角发型信息
//
// iRace = 0 - 女
//         1 - 男
// iIndex = 发型信息索引
LPCSTR  CHairModelMng::GetHairModel(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanHairModel(iIndex);
			break;
		}
	case 1:
		{
			return GetManHairModel(iIndex);
			break;
		}
	}
	return NULL;
}


// 得到男主角发型信息
//
// iIndex = 发型信息索引
LPCSTR  CHairModelMng::GetManHairModel(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_ManHairModelVector.size()))
	{
		return m_ManHairModelVector[iIndex].pMeshFile;
	}
	return NULL;
}


// 得到女主角发型信息
//
// iIndex = 发型信息索引
LPCSTR	CHairModelMng::GetWomanHairModel(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanHairModelVector.size()))
	{
		return m_WomanHairModelVector[iIndex].pMeshFile;
	}
	return NULL;
}

// 得到角色发型信息
// 0 -- 得到角色发型成功
// 其他 -- 失败.
int CHairModelMng::GetCharacterModel()
{
	// 清空数据
	ClearData();

	DBC_DEFINEHANDLE(s_CharacterHairModel, DBC_CHARACTER_HAIR_GEO);
	if(NULL == s_CharacterHairModel)
	{
		return 1;
	}

	int iItemCount = s_CharacterHairModel->GetRecordsNum();

	for(int i = 0; i < iItemCount; i++)
	{
		//搜索纪录
		const _DBC_CHAR_HAIR_GEO* pHaireInfo = (const _DBC_CHAR_HAIR_GEO*)s_CharacterHairModel->Search_LineNum_EQU((UINT)i);
		if(pHaireInfo)
		{
			switch(pHaireInfo->nRace)			
			{
			case 0:
				{
					// 女主角头像个数
					m_iWomanHairModelCount++;
					m_WomanHairModelVector.push_back(*pHaireInfo);
					break;
				}
			case 1:
				{
					// 男主角头像个数
					m_iManHairModelCount++;
					m_ManHairModelVector.push_back(*pHaireInfo);
					break;
				}
			default:
				{
					break;
				}
			}
		}

	}//for(int i = 0; i < iItemCount; i++)

	return 0;
}

// 清空数据
void CHairModelMng::ClearData()
{
	// 男主角发形个数
	m_iManHairModelCount   = 0;
	// 女主角发形个数
	m_iWomanHairModelCount = 0;

	// 男主角发型信息数组
	m_ManHairModelVector.clear();
	// 女主角发型信息数组
	m_WomanHairModelVector.clear();

}


// 得到女主角发型Id
int CHairModelMng::GetWomanHairModelId(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanHairModelVector.size()))
	{
		return m_WomanHairModelVector[iIndex].nID;
	}
	return 0;
}


// 得到男主角发型Id
int CHairModelMng::GetManHairModelId(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_ManHairModelVector.size()))
	{
		return m_ManHairModelVector[iIndex].nID;
	}
	return 0;
}


// 得到发型model Id
int CHairModelMng::GetHairModelId(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanHairModelId(iIndex);
			break;
		}
	case 1:
		{
			return GetManHairModelId(iIndex);
			break;
		}
	}
	return 0;
}

// 得到主角发型信息名字2006－4－6
//
// iRace = 0 - 女
//         1 - 男
// iIndex = 发型信息索引
LPCSTR  CHairModelMng::GetHairModelShowName(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanHairModelShowName(iIndex);
			break;
		}
	case 1:
		{
			return GetManHairModelShowName(iIndex);
			break;
		}
	}
	return NULL;
}

	// 得到男主角发型信息名字2006－4－6
	//
	// iIndex = 发型信息索引
LPCSTR  CHairModelMng::GetManHairModelShowName(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_ManHairModelVector.size()))
	{
		return m_ManHairModelVector[iIndex].pShowName;
	}
	return NULL;
}

	// 得到女主角发型信息名字2006－4－6
	//
	// iIndex = 发型信息索引
LPCSTR	CHairModelMng::GetWomanHairModelShowName(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanHairModelVector.size()))
	{
		return m_WomanHairModelVector[iIndex].pShowName;
	}
	return NULL;
}

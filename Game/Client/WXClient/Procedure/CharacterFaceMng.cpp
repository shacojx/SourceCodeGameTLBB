#include "StdAfx.h"
#include ".\characterfacemng.h"
#include "..\dbc\GMDataBase.h"
#include "GameProcedure.h"
#include "TDException.h"
CCharacterFaceMng::CCharacterFaceMng(void)
{
	// 男主角头像个数
	m_iManFaceCount   = 0;
	// 女主角头像个数
	m_iWomanFaceCount = 0;
}

CCharacterFaceMng::~CCharacterFaceMng(void)
{
}

// 得到主角头像个数
//
// iRace = 0 - 女
//         1 - 男
// iIndex = 头像信息索引
int CCharacterFaceMng::GetManFaceCount(int iRace)
{
	switch(iRace)
	{
	case 0://女
		{
			int iCount = GetWomanFaceCount();
			return iCount;
			break;
		}
	case 1://男
		{
			int iCount = GetManFaceCount();
			return iCount;
			break;
		}
	default:
		{
			return 0;
		}
	}
}

	
// 得到男主角头像个数
int CCharacterFaceMng::GetManFaceCount()
{
	return m_iManFaceCount;
}

	// 得到女主角头像个数
int CCharacterFaceMng::GetWomanFaceCount()
{
	return m_iWomanFaceCount;
}

// 得到主角头像信息
//
// iRace = 0 - 女
//         1 - 男
// iIndex = 头像信息索引
LPCSTR  CCharacterFaceMng::GetFaceImageInfo(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0://女
		{
			return GetWomanFaceImageInfo(iIndex);
			break;
		}
	case 1://男
		{
			return GetManFaceImageInfo(iIndex);
			break;
		}
	default:
		{
			return NULL;
		}
	}

	return NULL;
}

// 得到男主角头像信息
//
// iIndex = 头像信息索引
LPCSTR  CCharacterFaceMng::GetManFaceImageInfo(int iIndex)
{
	if((iIndex < m_iManFaceCount)&&(iIndex >= 0))
	{
		return m_ManFaceInfoVector[iIndex].pImageSetName;
	}
	return NULL;
}

// 得到女主角头像信息
//
// iIndex = 头像信息索引
LPCSTR	CCharacterFaceMng::GetWomanFaceImageInfo(int iIndex)
{
	
	if((iIndex < m_iWomanFaceCount)&&(iIndex >= 0))
	{
		LPCSTR pImageName = NULL;
		const _DBC_CHAR_FACE& FaceInfo = m_WomanFaceInfoVector[iIndex];
		pImageName = FaceInfo.pImageSetName;

		return pImageName;
	}
	return NULL;
}

// 得到角色头像信息
int CCharacterFaceMng::GetCharacterFaceIfo()
{
	// 清空数据
	ClearData();

	DBC_DEFINEHANDLE(s_CharacterFaceInfo, DBC_CHARACTER_FACE);
	if(NULL == s_CharacterFaceInfo)
	{
		return 1;
	}

	int iItemCount = s_CharacterFaceInfo->GetRecordsNum();

	for(int i = 0; i < iItemCount; i++)
	{
		//搜索纪录
		const _DBC_CHAR_FACE* pFaceInfo = (const _DBC_CHAR_FACE*)s_CharacterFaceInfo->Search_LineNum_EQU((UINT)i);
		if(pFaceInfo)
		{
			switch(pFaceInfo->nRace)			
			{
			case 0:
				{
					// 女主角头像个数
					m_iWomanFaceCount++;
					m_WomanFaceInfoVector.push_back(*pFaceInfo);
					break;
				}
			case 1:
				{
					// 男主角头像个数
					m_iManFaceCount++;
					m_ManFaceInfoVector.push_back(*pFaceInfo);
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}//

	return 0;
}

// 清空数据
void CCharacterFaceMng::ClearData()
{
	// 男主角头像个数
	m_iManFaceCount   = 0;
	// 女主角头像个数
	m_iWomanFaceCount = 0;

	// 男主角头像信息数组
	m_ManFaceInfoVector.clear();
	// 女主角头像信息数组
	m_WomanFaceInfoVector.clear();
}


// 得到女主角头像Id
int CCharacterFaceMng::GetWomanFaceId(int iIndex)
{
	if(iIndex >= (int)m_WomanFaceInfoVector.size())
	{
		return  0;
	}

	return m_WomanFaceInfoVector[iIndex].nID;
}


// 得到男主角头像Id
int CCharacterFaceMng::GetManFaceId(int iIndex)
{
	if(iIndex >= (int)m_ManFaceInfoVector.size())
	{
		return 0;
	}

	return m_ManFaceInfoVector[iIndex].nID;
}


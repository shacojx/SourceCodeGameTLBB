#include "StdAfx.h"
#include ".\facemodelmng.h"
#include "..\dbc\GMDataBase.h"
#include "GameProcedure.h"
#include "TDException.h"

CFaceModelMng::CFaceModelMng(void)
{
}

CFaceModelMng::~CFaceModelMng(void)
{
}


// 得到主角脸形个数
//
// iRace = 0 - 女
//         1 - 男
// iIndex = 头像信息索引
int CFaceModelMng::GetFaceModelCount(int iRace)
{

	switch(iRace)
	{
	case 0:
		{
			
			return GetWomanFaceModelCount();
			break;
		}
	case 1:
		{
			return GetManFaceModelCount();
			break;
		}
	}
	return 0;
}

// 得到男主角头像个数
int CFaceModelMng::GetManFaceModelCount()
{
	return m_iManFaceModelCount;
}

// 得到女主角头像个数
int CFaceModelMng::GetWomanFaceModelCount()
{
	return m_iWomanFaceModelCount;
}

// 得到主角头像信息
//
// iRace = 0 - 女
//         1 - 男
// iIndex = 头像信息索引
LPCSTR  CFaceModelMng::GetFaceModel(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanFaceModel(iIndex);
			break;
		}
	case 1:
		{
			return GetManFaceModel(iIndex);
			break;
		}
	}
	return NULL;
}

// 得到男主角头像信息
//
// iIndex = 头像信息索引
LPCSTR  CFaceModelMng::GetManFaceModel(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_ManFaceModelVector.size()))
	{
		return m_ManFaceModelVector[iIndex].pMeshFile;
	}
	return NULL;
}

// 得到女主角头像信息
//
// iIndex = 头像信息索引
LPCSTR	CFaceModelMng::GetWomanFaceModel(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanFaceModelVector.size()))
	{
		return m_WomanFaceModelVector[iIndex].pMeshFile;
	}
	return NULL;
}

// 得到角色头像信息
// 0 -- 得到角色头像成功
// 其他 -- 失败.
int CFaceModelMng::GetCharacterModel()
{
	// 清空数据
	ClearData();

	DBC_DEFINEHANDLE(s_CharacterFaceModel, DBC_CHARACTER_HEAD_GEO);
	if(NULL == s_CharacterFaceModel)
	{
		return 1;
	}

	int iItemCount = s_CharacterFaceModel->GetRecordsNum();

	for(int i = 0; i < iItemCount; i++)
	{
		//搜索纪录
		const _DBC_CHAR_HEAD_GEO* pFaceInfo = (const _DBC_CHAR_HEAD_GEO*)s_CharacterFaceModel->Search_LineNum_EQU((UINT)i);
		if(pFaceInfo)
		{
			switch(pFaceInfo->nRace)			
			{
			case 0:
				{
					// 女主角头像个数
					m_iWomanFaceModelCount++;
					m_WomanFaceModelVector.push_back(*pFaceInfo);
					break;
				}
			case 1:
				{
					// 男主角头像个数
					m_iManFaceModelCount++;
					m_ManFaceModelVector.push_back(*pFaceInfo);
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
void CFaceModelMng::ClearData()
{
	// 男主角头像模型个数
	m_iManFaceModelCount   = 0;
	// 女主角头像模型个数
	m_iWomanFaceModelCount = 0;

	// 男主角头像模型信息数组
	m_ManFaceModelVector.clear();
	// 女主角头像模型信息数组
	m_WomanFaceModelVector.clear();
}

// 得到女主角头像Id
int CFaceModelMng::GetWomanFaceModelId(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanFaceModelVector.size()))
	{
		return m_WomanFaceModelVector[iIndex].nID;
	}
	return 0;
}

// 得到男主角头像Id
int CFaceModelMng::GetManFaceModelId(int iIndex)
{

	if((iIndex >= 0)&&(iIndex < (int)m_ManFaceModelVector.size()))
	{
		return m_ManFaceModelVector[iIndex].nID;
	}
	return 0;
}

// 得到脸形model Id
int CFaceModelMng::GetFaceModelId(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanFaceModelId(iIndex);
			break;
		}
	case 1:
		{
			return GetManFaceModelId(iIndex);
			break;
		}
	}
	
	return 0;
}

// 得到主角头像信息
	//
	// iRace = 0 - 女
	//         1 - 男
	// iIndex = 头像信息索引
LPCSTR  CFaceModelMng::GetFaceModelShowName(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			return GetWomanFaceModelShowName(iIndex);
			break;
		}
	case 1:
		{
			return GetManFaceModelShowName(iIndex);
			break;
		}
	}
	return NULL;
}

	// 得到男主角头像信息
	//
	// iIndex = 头像信息索引
LPCSTR  CFaceModelMng::GetManFaceModelShowName(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_ManFaceModelVector.size()))
	{
		return m_ManFaceModelVector[iIndex].pShowName;
	}
	return NULL;
}

	// 得到女主角头像信息
	//
	// iIndex = 头像信息索引
LPCSTR	CFaceModelMng::GetWomanFaceModelShowName(int iIndex)
{
	if((iIndex >= 0)&&(iIndex < (int)m_WomanFaceModelVector.size()))
	{
		return m_WomanFaceModelVector[iIndex].pShowName;
	}
	return NULL;
}

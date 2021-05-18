#ifndef __CHARACTERFACEMNG_H__
#define __CHARACTERFACEMNG_H__
#pragma once
#include "..\common\TDDBC_Struct.h"
#include <vector>

typedef std::vector<_DBC_CHAR_FACE> FACE_INFO_VECTOR;

class CCharacterFaceMng
{
public:
	CCharacterFaceMng(void);
	~CCharacterFaceMng(void);

public:

	// 男主角头像个数
	int				 m_iManFaceCount;
	// 女主角头像个数
	int              m_iWomanFaceCount;

	// 男主角头像信息数组
	FACE_INFO_VECTOR m_ManFaceInfoVector;
	// 女主角头像信息数组
	FACE_INFO_VECTOR m_WomanFaceInfoVector;

	// 得到主角头像个数
	//
	// iRace = 0 - 女
	//         1 - 男
	// iIndex = 头像信息索引
	int GetManFaceCount(int iRace);

	// 得到男主角头像个数
	int GetManFaceCount();

	// 得到女主角头像个数
	int GetWomanFaceCount();

	// 得到主角头像信息
	//
	// iRace = 0 - 女
	//         1 - 男
	// iIndex = 头像信息索引
	LPCSTR  GetFaceImageInfo(int iRace, int iIndex);

	// 得到男主角头像信息
	//
	// iIndex = 头像信息索引
	LPCSTR  GetManFaceImageInfo(int iIndex);

	// 得到女主角头像信息
	//
	// iIndex = 头像信息索引
	LPCSTR	GetWomanFaceImageInfo(int iIndex);

	// 得到角色头像信息
	// 0 -- 得到角色头像成功
	// 其他 -- 失败.
	int GetCharacterFaceIfo();

	// 清空数据
	void ClearData();

	// 得到女主角头像Id
	int GetWomanFaceId(int iIndex);

	// 得到男主角头像Id
	int GetManFaceId(int iIndex);

};

#endif //__CHARACTERFACEMNG_H__
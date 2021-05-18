#ifndef __FACEMODELMNG_H__
#define __FACEMODELMNG_H__
#pragma once

#include "..\..\common\TDDBC_Struct.h"
#include <vector>
typedef std::vector<_DBC_CHAR_HEAD_GEO> FACE_MODLE_INFO_VECTOR;

class CFaceModelMng
{
public:
	CFaceModelMng(void);
	~CFaceModelMng(void);

public:
	
	// 男主角脸形个数
	int				 m_iManFaceModelCount;
	// 女主角脸形个数
	int              m_iWomanFaceModelCount;

	// 男主角脸形信息数组
	FACE_MODLE_INFO_VECTOR m_ManFaceModelVector;
	// 女主角脸形信息数组
	FACE_MODLE_INFO_VECTOR m_WomanFaceModelVector;

	// 得到主角脸形个数
	//
	// iRace = 0 - 女
	//         1 - 男
	// iIndex = 头像信息索引
	int GetFaceModelCount(int iRace);

	// 得到男主角头像个数
	int GetManFaceModelCount();

	// 得到女主角头像个数
	int GetWomanFaceModelCount();

	// 得到主角头像信息
	//
	// iRace = 0 - 女
	//         1 - 男
	// iIndex = 头像信息索引
	LPCSTR  GetFaceModel(int iRace, int iIndex);

	// 得到男主角头像信息
	//
	// iIndex = 头像信息索引
	LPCSTR  GetManFaceModel(int iIndex);

	// 得到女主角头像信息
	//
	// iIndex = 头像信息索引
	LPCSTR	GetWomanFaceModel(int iIndex);

	// 得到角色头像信息
	// 0 -- 得到角色头像成功
	// 其他 -- 失败.
	int GetCharacterModel();

	// 清空数据
	void ClearData();

	// 得到女主角头像Id
	int GetWomanFaceModelId(int iIndex);

	// 得到男主角头像Id
	int GetManFaceModelId(int iIndex);

	// 得到脸形model Id
	int GetFaceModelId(int iRace, int iIndex);


	
	// 得到主角头像信息
	//
	// iRace = 0 - 女
	//         1 - 男
	// iIndex = 头像信息索引
	LPCSTR  GetFaceModelShowName(int iRace, int iIndex);

	// 得到男主角头像信息
	//
	// iIndex = 头像信息索引
	LPCSTR  GetManFaceModelShowName(int iIndex);

	// 得到女主角头像信息
	//
	// iIndex = 头像信息索引
	LPCSTR	GetWomanFaceModelShowName(int iIndex);
};

#endif //__FACEMODELMNG_H__
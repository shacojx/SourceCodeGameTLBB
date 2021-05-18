#ifndef __HAIRMODELMNG_H__
#define __HAIRMODELMNG_H__
#pragma once

#include "..\..\..\common\TDDBC_Struct.h"
#include <vector>
typedef std::vector<_DBC_CHAR_HAIR_GEO> HAIR_MODLE_INFO_VECTOR;

class CHairModelMng
{
public:
	CHairModelMng(void);
	~CHairModelMng(void);

public:
	
	// 男主角发形个数
	int				 m_iManHairModelCount;
	// 女主角发形个数
	int              m_iWomanHairModelCount;

	// 男主角发型信息数组
	HAIR_MODLE_INFO_VECTOR m_ManHairModelVector;
	// 女主角发型信息数组
	HAIR_MODLE_INFO_VECTOR m_WomanHairModelVector;

	// 得到主角发型个数
	//
	// iRace = 0 - 女
	//         1 - 男
	// iIndex = 发型信息索引
	int GetHairModelCount(int iRace);

	// 得到男主角发型个数
	int GetManHairModelCount();

	// 得到女主角发型个数
	int GetWomanHairModelCount();

	// 得到主角发型信息
	//
	// iRace = 0 - 女
	//         1 - 男
	// iIndex = 发型信息索引
	LPCSTR  GetHairModel(int iRace, int iIndex);

	// 得到男主角发型信息
	//
	// iIndex = 发型信息索引
	LPCSTR  GetManHairModel(int iIndex);

	// 得到女主角发型信息
	//
	// iIndex = 发型信息索引
	LPCSTR	GetWomanHairModel(int iIndex);

	// 得到角色发型信息
	// 0 -- 得到角色发型成功
	// 其他 -- 失败.
	int GetCharacterModel();

	// 清空数据
	void ClearData();

	// 得到女主角发型Id
	int GetWomanHairModelId(int iIndex);

	// 得到男主角发型Id
	int GetManHairModelId(int iIndex);

	// 得到发型model Id
	int GetHairModelId(int iRace, int iIndex);

	// 得到主角发型信息名字
	//
	// iRace = 0 - 女
	//         1 - 男
	// iIndex = 发型信息索引
	LPCSTR  GetHairModelShowName(int iRace, int iIndex);

	// 得到男主角发型信息名字
	//
	// iIndex = 发型信息索引
	LPCSTR  GetManHairModelShowName(int iIndex);

	// 得到女主角发型信息名字
	//
	// iIndex = 发型信息索引
	LPCSTR	GetWomanHairModelShowName(int iIndex);
};

#endif //__HAIRMODELMNG_H__
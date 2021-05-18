
#pragma once
#include "TDWorldSystem.h"
#include "Zone.h"
#include "Region.h"
#include "GameDefine.h"
#include ".\buildingcollisionmng.h"
struct _DBC_ENV_SOUND;//环境音效
class tSoundSource;
struct _DBC_CITY_DEFINE;
struct _DBC_CITY_BUILDING;

class CScene : public tScene
{
public:
	enum  { SIZE_OF_ZONE = 10  };	//Zone的大小

	//取得场景定义
	virtual const _DBC_SCENE_DEFINE*	GetSceneDefine(VOID)const	{ return m_pTheDefine; }
	//场景位置是否合法
	virtual BOOL	IsValidPosition(const fVector2& fvPos);
	//根据逻辑坐标取得地形高度，考虑行走面，如果传入坐标非法，返回FALSE
	virtual BOOL	GetMapHeight(const fVector2& fvMapPos, FLOAT& fHeight);
	//
	//加载不可行走区域Region数据
	//  1. 从二进制文件szRegionFile中读取所有Region定义，并贮存到m_theRegion结构中
	//  2. 根据每个Region的位置，将其注册到相应的CZone中
	//  3. 如果发生错误，则直接抛出异常
	//  上一个函数的二进制版本.
	VOID			LoadRegionBinary(LPCTSTR szRegionFile);
	//得到指定网格，如果位置非法，返回NULL
	CZone*			GetZone(INT nXPos, INT nZPos);
	//根据世界X坐标得到所在的Zone的x坐标
	INT				GetZoneX(FLOAT fx)const { return (INT)(fx/SIZE_OF_ZONE); }
	//根据世界Z坐标得到所在的Zone的z坐标
	INT				GetZoneZ(FLOAT fz)const { return (INT)(fz/SIZE_OF_ZONE); }

	//得到大小
	INT				GetSizeX(VOID) const { return m_nZoneXSize; }
	INT				GetSizeZ(VOID) const { return m_nZoneZSize; }


	//控制背景音乐
	VOID			ControlBkgSound(BOOL bEnable);
	VOID			ControlBkgSoundVol();
	//控制环境音
	VOID			ControlEnvSound(BOOL bEnable);
	VOID			ControlEnvSoundVol();

	/***************************************************
		玩家城市相关
	****************************************************/
	bool IsUserCity(void) const { return m_bUserCity; }
	//玩家城市升级
	bool UpdateUserCity(int nNewLevel);
	//更新建筑物
	bool UpdateCityBuilding(int nBuildingID, int nBuildingLevel);

public:
	//场景初始化，加载地形，静态物品等
	virtual VOID	Initial(VOID);
	virtual VOID	EnterScene(VOID);
	virtual VOID	LeaveScene(VOID);
	virtual VOID	Tick(VOID);

public:
	CScene(const _DBC_SCENE_DEFINE* pSceneDef, bool bUserCity);
	virtual ~CScene();

protected:
	//将所有的Region注册到Zone
	VOID						RegisteAllRegion(VOID);

	VOID						LoadDijk();

	//环境音效
	BOOL						LoadEnvSound(VOID);
	//销毁所有环境音效
	void						DestroyEnvSound(void);

protected:
	//场景定义结构, 从文件中读取
	const _DBC_SCENE_DEFINE* 	m_pTheDefine;

	//Zone数据
	INT							m_nZoneXSize;
	INT							m_nZoneZSize;
	std::vector< CZone >		m_theZoneBuf;	// 大小为 m_nZoneXSize*m_nZoneZSize

	//背景音乐
	tSoundSource*				m_pBackSound;

	//环境音效列表
	struct EnvSound_t
	{
		INT				nID;
		INT				nSoundID;
		UINT			nPosx;
		UINT			nPosz;
		UINT			nDistance;
		tSoundSource*	pSoundSource;
	};

	std::vector<EnvSound_t>		m_pEnvSoundList;	

	/***************************************************
		玩家城市相关
	****************************************************/
	bool m_bUserCity;	//是否是玩家城市

	//建筑物
	struct BUILDING
	{
		const _DBC_CITY_BUILDING*	pDefine;	//资源定义
		INT							nLevel;		//当前等级
		INT							nObjID;		//运行时实例ID
	};

	//当前场景中建筑物(资源定义id为索引)
	typedef std::map< INT, BUILDING >	BUILDING_REGISTER;
	BUILDING_REGISTER				m_allBuilding;	//所有建筑物

public:
	//Region链表	
	std::vector< CRegion >		m_theRegion;
	// 建筑物行走面管理
	CBuildingCollisionMng		m_WalkCollisionMng;

	FLOAT*						m_Weights;
	FLOAT*						m_Dist;
	std::vector<fVector2>		m_Points;
};


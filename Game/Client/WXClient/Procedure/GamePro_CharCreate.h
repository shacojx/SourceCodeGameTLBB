/****************************************\
*										*
* 			   人物创建流程				*
*										*
\****************************************/
#pragma once

#include "GameProcedure.h"
#include "CharacterFaceMng.h"
#include "FaceModelMng.h"
#include "HairModelMng.h"

class CObject_PlayerOther;
class CGamePro_CharCreate : public CGameProcedure
{

public:
	

	//登录状态
	enum PLAYER_CHAR_CREATE_STATUS
	{
		CHAR_CREATE_CREATE_OK = 0 ,	// 创建角色成功
		CHAR_CREATE_CREATEING,		// 创建角色
	};


	PLAYER_CHAR_CREATE_STATUS	m_Status;
	void					CallInit(){ Init(); }

	CObject_PlayerOther*	m_pAvatar[2];						// 用于UI显示的逻辑对象.
	TCHAR					m_szBufModelName1[128];				// 女模型名字.
	TCHAR					m_szBufModelName2[128];				// 男模型名字.


	TCHAR					m_szCreateRoleName[128];			// 要创建的角色的名字
	int						m_iFaceColor1;						// 设置面部颜色
	int 					m_iFaceModle1;						// 设置面部模型
	int 					m_iHairColor1;						// 设置头发颜色
	int 					m_iHairModle1;						// 设置头发模型
	int 					m_iSex;								// 设置性别
	int						m_iFaceId;							// 设置头像
	CampID_t				m_iCamp;
	int						m_iMenpai;

	

	// 创建角色
	int						CreateRole();

	// 切换到角色选择界面
	void					ChangeToRoleSel();


public:

	//////////////////////////////////////////////////////////////////////////////
	//
	//
	// 头像管理器.
	CCharacterFaceMng	m_FaceMng;

	// 是否读取头像信息.
	int					m_iIsLoadFace;

	// 设置头像id
	void	SetFace(int iFaceId);	

	// 通过性别和索引设置头像id
	void SetFaceByRaceAndIndex(int iRace, int iIndex);	

	/////////////////////////////////////////////////////////////////////////////
	//
	// 
	//脸形信息管理器
	CFaceModelMng		m_FaceModelmng;

	// 是否读取脸形模型信息.
	int					m_iIsLoadFaceModel;

	// 设置脸形id
	void	SetFaceModel(int iFaceModelId);	

	// 通过性别和索引设置头像id
	void SetFaceModelByRaceAndIndex(int iRace, int iIndex);	


	////////////////////////////////////////////////////////////////////////////
	//
	//
	// 发型信息管理器
	CHairModelMng		m_HairModelMng;

	// 是否读取发形模型信息.
	int					m_iIsLoadHairModel;

	// 设置脸形id
	void	SetHairModel(int iHairModelId);	

	// 通过性别和索引设置头像id
	void SetHairModelByRaceAndIndex(int iRace, int iIndex);	





protected:
	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
	virtual VOID	MouseInput(VOID);
	//处理输入
	virtual VOID	ProcessInput(VOID);

public:
	CGamePro_CharCreate(VOID);
	virtual ~CGamePro_CharCreate(VOID);

	// 创建界面模型
	int CreateModel();

	// 切换到选择人物界面
	void ChangeToSelectRole();
};

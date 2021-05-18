/***********************************\
*									*
*	 角色头顶上的信息UI管理器		*
*									*
\************************************/
#pragma once
#include "TDBasicType.h"
#include "ceguicolourrect.h"
#include "ifalagardbehitboard.h"
#include <list>
#include "ceguivector.h"
namespace CEGUI
{
	class Window;
};
class CCreatureBoard;

struct STRUCT_BEHIT_DATA
{
	float	m_fXPos;
	float	m_fYPos;
	float   m_nXSpeed; // X象素级移动速度
	float   m_nYSpeed; // Y象素级移动速度

	float   m_nXAcceleration;
	float   m_nYAcceleration; // 两个方向的加速度

	
	int		m_nMaxLife;

	bool    m_bAlphaMode;	// 是否是alpha模式
	int		m_nMoveMode;
	int		m_nUseTempPos;
	
	CEGUI::colour  m_ColorType;
	CEGUI::colour  m_BackColorType;
	
	float   m_fWidth;
	float   m_fHeight;
};
class CBeHitBoard
{
	
public:
	CBeHitBoard();
	~CBeHitBoard();

	void ResetData( LPCTSTR szText  );

	CEGUI::Window *	m_pWindow;
	char		m_szWindowName[ 128 ];
	
	bool	m_bDoubleHit;

	float   m_nStartX;
	float   m_nStartY;

	float   m_fAlpha;
	int		m_nLife;			// 生存期
	CEGUI::colour  m_ColorType;//int		m_nColorType;
	CEGUI::colour  m_BackColorType;//int		m_nColorType;

	float   m_nXSpeed; // X象素级移动速度
	float   m_nYSpeed; // Y象素级移动速度
	float   m_nXAcceleration;
	float   m_nYAcceleration; // 两个方向的加速度
	int		m_nMaxLife;
	bool    m_bAlphaMode;	// 是否是alpha模式

	float   m_fScale;
	STRUCT_BEHIT_DATA  *m_sData;

	float   m_fDoubleTime;
	bool    m_bZoomOn;
	int		m_nMoveMode;
	int		m_nUseTempPos;
	int     m_nTempPosIndex;
	int     m_nMoveType;
public:
	void Update();
	bool IsInUse(){ return m_bInUse; };
	void InUse( bool bUse );

private:
	bool  m_bInUse; // 是否正在使用
	
};
// 用来管理头上冒的字得管理器
class CBeHitBoardManager
{

public:
	CBeHitBoardManager();
	virtual ~CBeHitBoardManager();
public:
	int				m_nRaisSize; // 自动增长得尺寸
	std::list< CBeHitBoard* >  m_aWindowList;

	CBeHitBoard* GetFreeWindow();
	void FreeWindow( CBeHitBoard* pWindow );


	static CBeHitBoardManager*	GetMe(VOID) { return s_pMe; }

	static CBeHitBoardManager* s_pMe;

	STRUCT_BEHIT_DATA	m_BeHitData[ CEGUI::MAX_MOVE_TYPE_NUMBER ];
	CEGUI::colour		m_ColorType[ CEGUI::MAX_COLOR_TYPE_NUMBER ];
	CEGUI::colour		m_BackColorType[ CEGUI::MAX_COLOR_TYPE_NUMBER ];
	CEGUI::Point		m_TempPoint[ CEGUI::MAX_TEMP_TYPE_NUMBER ]; 
	float				getNormalSize() { return m_fNormalSize;}
	float				getDoubleSize() { return m_fDoubleSize;}
	float				getDoubleEndSize() { return m_fDoubleEndSize;}
	float				getDoubleTime() { return m_fDoubleTime; }
private:
	int					m_nCounter;
	
	CBeHitBoard*		NewWindow();

	float				m_fNormalSize;
	float				m_fDoubleSize;
	float				m_fDoubleEndSize;

	float				m_fDoubleTime;

};
class CUICreatureBoardSystem
{
public:
	static CEGUI::Window*	GetClientScreen(VOID) { return CUICreatureBoardSystem::m_pCreatureBoardGroundSheet; };
	static CEGUI::Window*   m_pCreatureBoardGroundSheet; // 客户端窗口底板

	//创建/销毁 信息板
	CCreatureBoard*		CreateCreatureBoard();
	void				DestroyCreatureBoard(CCreatureBoard* pBoard);
	// 增加一个击中效果消息框
	// bDoubleHit 是否为重击
	// szValue 显示的字符串
	// nStartX nStartY 其实屏幕坐标
	void				AddNewBeHit( bool bDoubleHit, LPCTSTR szValue, float nStartX, float nStartY, int nType = 0, int nMoveType = 0 );

public:
	//管理器初始化
	VOID				Initial(VOID);
	//释放所有资源
	VOID				Release(VOID);

	VOID				Tick();

protected:
	//所有信息板的链表
	std::list< CCreatureBoard* >	m_listAllBoard;

	std::list< CBeHitBoard* >		m_listBeHitBoard;

	CBeHitBoardManager*				m_pBeHitBoardManager;

protected:
	//信息板池
	std::list< CCreatureBoard* >	m_listBoardDataPool;

public:
	CUICreatureBoardSystem();
	virtual ~CUICreatureBoardSystem();
	static CUICreatureBoardSystem*	GetMe(VOID) { return s_pMe; }

	static CUICreatureBoardSystem* s_pMe;

	float   GetMaxDistance() { return m_nMaxDistance; };
	float   GetMaxDispearDistance() { return m_nMaxDispearDistance; };

protected:
	int m_nBeHitCounter;
	float m_nMaxDistance; // 最大的显示范围
	float m_nMaxDispearDistance;// 超过找个范围就消失
};
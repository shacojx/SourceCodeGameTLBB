/****************************************\
*										*
* 			   物体管理器				*
*										*
\****************************************/

#pragma once
#include "TDObjectSystem.h"
#include "Type.h"
class CObject;
class CObject_Character;
class CObject_PlayerMySelf;
class CObject_PlayerOther;
class CObject_PlayerNPC;
class CObject_Surface;
class CObjectLoadQueue;

class CObjectManager :
	public tObjectSystem
{


	
protected:
	static CObjectManager*			s_pMe;

public:
	static CObjectManager*  GetMe( VOID ){ return s_pMe; }
	CObject_PlayerOther* GetTarget_Avatar( VOID ) { return m_pMainTargetAvatar;}

public:
	//快捷操作，取得玩家自己
	CObject_PlayerMySelf*	GetMySelf( VOID ){ return m_pMySelf; }

public:
	CObjectManager( VOID );
	virtual ~CObjectManager( VOID );

	virtual VOID	Initial( VOID* );
	virtual VOID	Release( VOID );

	virtual VOID	Tick( VOID );

public:
	// 产生新物体,并加入数据链
	virtual tObject*		NewObject(LPCTSTR szClass, INT idServer=-1, tObject* pParent=NULL);
	// 销毁物体,并从数据链上拆除
	virtual BOOL			DestroyObject(tObject* pObject);
	// 根据ID得到某物体
	virtual tObject*		FindObject( INT id );
	// 获取一个SERVER OBJ对象的指针
	virtual tObject*		FindServerObject( INT idServer );
	// 获得物品
	virtual tObject_Item*	FindItem(INT id);
	// 销毁所有物体
	virtual VOID			ClearAllObject( VOID );
	/// 设置主目标对象，如果id非法，取消选中物体
	virtual VOID			SetMainTarget(INT id, DESTROY_MAIN_TARGET_TYPE delType = DEL_OBJECT);
	//取得当前选中物体
	virtual tObject*		GetMainTarget(VOID);

	//取得当前选中物体
	virtual tObject*		GetMainTarget(GUID_t& guid);

	//将最近的一个敌人设为主目标
	void					Debug_LockNearestTarget(void);
	//将最近的一个敌人设为主目标攻击
	void					Debug_LockNearestTargetToHit(void);
	//设置某ojbect得ServerID
	virtual BOOL			SetObjectServerID( INT idObj, INT idServer );
	// 根据屏幕坐标计算选中得物体
	virtual tObject*		GetMouseOverObject( INT nX, INT nY, fVector3& fvMouseHitPlan );
	//关心某物体的指定事件 id-物体ID		bCare-关心或者取消关心
	virtual VOID			CareObject(INT id, BOOL bCare, std::string szSign);
	//从本地资源表中读取物品名字
	virtual LPCTSTR			ItemNameByTBIndex(UINT idTable);
	//资源加载队列
	CObjectLoadQueue*		GetLoadQueue(void) { return m_pLoadQueue; }
	//根据人物名字获得对应的objcet
	virtual CObject_Character*		FindCharacterByName(std::string  szName);
	VOID					SetTargetAvatarAsTarget( VOID );
	//响应分辨率改变
	void					OnScreenSizeChanged(void);
	//响应主角等级提高,更改所有npc的等级
	void					OnMySelfLevelUp(void);
	//立即加载NPC
	void					SetLoadNPCDirect(BOOL b);

public:
	// 产生新物体,并加入数据链
	virtual CObject_PlayerOther*	NewPlayerOther( INT idServer );

	// 产生新物体,并加入数据链
	virtual CObject_PlayerNPC*		NewPlayerNPC( INT idServer );

	// 创建表面物体
	virtual CObject_Surface*		NewSurfaceObj( LPCTSTR szClass );


public:

	// 设置当前选择的是队友
	void SetCurSelTeam(GUID_t id);
	GUID_t GetCurSelTeam();

	// 设置队友是否在当前可视范围
	void SetPresent(BOOL  bIsPresent);
	
	// 当前选择的是否是对友2006－4－1
	BOOL m_bIsSelTeam;

	// 选择的队友是否在眼前2006－4－1
	BOOL m_bIsSelTeamInPresent;

	// 当前选择的是否是对友2006－4－1
	BOOL IsSelTeam();

	// 选择的队友是否在眼前2006－4－1
	BOOL IsSelTeamInPresent();

	// 选中队友的gudi
	GUID_t					m_GUID;
	
	// 不选择mainTarget
	void UnSelMainTarget(void);
	CObject*						m_pNearestTargetOLD;

protected:
	CObject_PlayerMySelf*			m_pMySelf;				// 自已本身
	CObject*						m_pObjectRoot;			// 物体根节点
		CObject*					m_pLogicalObject;		// 逻辑物体节点
			CObject*				m_pPlayerOther;			// 其他玩家节点
			CObject*				m_pNPC;					// NPC节点
			CObject*				m_pSurface;				// 地表物体节点
		CObject*					m_pAlogicalObject;		// 非逻辑物体节点
	
	typedef std::map< INT, CObject* > CObjectMap;
	CObjectMap						m_hmObject;				// 根据ID的物体HASH表
	CObjectMap						m_hmServerObject;		// 根据ServerID的物体HASH表

	CObject*						m_pMainTarget;			// 主目标对象
	CObject_PlayerOther*			m_pMainTargetAvatar;	// 用于UI显示的逻辑对象.

	//被UI关心的逻辑对象
	struct OBJECT_BECARED
	{
		INT			id;					//物体ID
		FLOAT		fLastDistance;		//和主角之间的距离
	};
	//目前被关心的逻辑对象
	std::map< std::string, OBJECT_BECARED >	m_mapCaredObject;

	//资源加载队列
	CObjectLoadQueue*				m_pLoadQueue;

protected:
	VOID AttachObject( tObject *pObject );
	VOID DetchObject( tObject *pObject );

private:
	INT							IDFactory( VOID );

protected:
	WX_DECLARE_DYNAMIC(CObjectManager);
};


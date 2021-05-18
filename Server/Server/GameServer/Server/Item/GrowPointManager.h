/********************************************************************
	创建日期:	2005年11月1日
	创建时间:	9:35
	文件名称:	GrowPointManager.h
	文件路径:	d:\Prj\Server\Server\Item\GrowPointManager.h
	
	文件功能： 对生长点的管理
			   每个场景一份

	修改纪录：
	
*********************************************************************/


#ifndef		_GROWPOINT_MANAGER_H_
#define		_GROWPOINT_MANAGER_H_
#include	"Type.h"
#include	"LuaInterface.h"

class Scene;


//场景生长点数据
struct  GROW_POINT_GROUP_SCENE_DATA 
{
	WORD		GrowPointType;								//生长点类型
	FLOAT		GrowPointX;									//生长点x坐标
	FLOAT		GrowPointY;									//生长点Y坐标
	INT			GrowPointRandValue;							//生长点排序随机值
	BOOL		Used;
	GROW_POINT_GROUP_SCENE_DATA()
	{
		memset(this,0,sizeof(GROW_POINT_GROUP_SCENE_DATA));
	}
};

//生长点设置数据
struct GROW_POINT_GROUP_SCENE_SETUP 
{
	WORD		GrowPointType;						//生长点类型
	WORD		GrowPointMaxCount;					//生长点同时出现个数
	INT			GrowPointInterval;					//生长时间间隔

	GROW_POINT_GROUP_SCENE_SETUP()
	{
		GrowPointType = -1;
		GrowPointMaxCount = 0;
		GrowPointInterval = 1000000;
	}

};

//单一类型生长点管理器
class TypeGrowPointManager
{
public:

	TypeGrowPointManager()
	{
		m_pData				=	NULL;
		m_AddOffSet			=	0;
		m_TypeCount			=	0;
		m_TypeOffSet		=	0;
		m_IntervalPerSeed	=	0;
		m_CurrentElapse		=	0;
		m_bStartCount		=	FALSE;
		m_LastUpdateTime	=	0;
		m_CurrentCount		=	0;
		m_MaxApperaCount	=	0;
		m_bEnable			=	FALSE;		

	}

	~TypeGrowPointManager()
	{
		if(m_pData)
			SAFE_DELETE_ARRAY(m_pData);
	}



	//生长点数据
	GROW_POINT_GROUP_SCENE_DATA*	m_pData;
	//当前数量
	INT								m_CurrentCount;
	//最大共生数量
	INT								m_MaxApperaCount;
	//场景编辑器中最大数量
	INT								m_TypeCount;
	//添加偏移计数
	INT								m_AddOffSet;
	//生成偏移计数
	INT								m_TypeOffSet;
	//生长点类型
	WORD							m_GrowPointType;
	//生长控制脚本编号
	ScriptID_t						m_ScriptID;
	//补充种子的间隔时间
	UINT							m_IntervalPerSeed;
	//当前的时间
	UINT							m_CurrentElapse;
	//最后更新时间
	UINT							m_LastUpdateTime;
	//开始计数
	BOOL							m_bStartCount;
	//当前类型管理器是否激活
	BOOL							m_bEnable;
	//添加生长点位置数据
	inline	VOID					AddData(GROW_POINT_GROUP_SCENE_DATA& RefData);

	//获得一个生长点位置
	inline	BOOL					CreateGrowPointPos(FLOAT& x,FLOAT& y);
	//释放一个生长点位置			
	inline  BOOL					ReleaseGrowPointPos(FLOAT x,FLOAT y);

	/*-----------对生长点计数管理---------------*/

	//设置最大数量
	inline	VOID					SetCount(INT count); 
	//获取最大数量
	inline	INT						GetCount();
	
	//增加生长点场景数据个数
	inline	VOID					IncCount();
	//增加当前已经出现的个数
	inline	VOID					IncCurrentCount();
	//减少当前出现的个数
	inline	VOID					DecCurrentCount();
	
	

	//获得管理器管理类型
	inline	WORD					GetGrowPointType();
	
	//设置管理器管理类型
	inline	VOID					SetGrowPointType(WORD gpType);
	
	//设置管理器播种间隔
	inline	VOID					SetIntervalTime(INT iInterval);
	

public:
	/* 
	*	特定类型生长点管理器心跳
	*	主要完成对生长点的个数补充
	*/
	inline	BOOL					DoTicks(UINT uTime);

	/*
	 * * 对生长点进行排序//
	 */
	VOID							RandSort();
};


struct GP_OWNER
{
	GP_OWNER()
	{
		OwnerID		= INVALID_ID;
		OwnerCount	= 0;
	}

	ObjID_t	OwnerID;
	INT		OwnerCount;
};


struct GROWPOINT_LOAD
{
	INT									m_DataCount;		//原始数据个数
	GROW_POINT_GROUP_SCENE_DATA*		m_pData;			//原始数据
	INT									m_SetupCount;		//配置数据个数
	GROW_POINT_GROUP_SCENE_SETUP*		m_pSetup;			//配置数据

};

class GrowPointManager
{

	//生长点管理器数量
	TypeGrowPointManager*				m_GrowPointMgr[MAX_SCENE_GROW_POINT_NUM];
	//生长点数量总和
	INT									m_GrowPointCount;
	//生长点类型数量
	INT									m_GrowPointTypeCount;
	//场景指针
	Scene*								m_pScene;
	//脚本接口
	LuaInterface*						m_pLuaInterface;

	GP_OWNER							m_GPOwner[MAX_SCENE_GP_OWNER];
	INT									m_OwnerCount;
public:

	GrowPointManager()
	{
		memset(m_GrowPointMgr,0,sizeof(TypeGrowPointManager*)*MAX_SCENE_GROW_POINT_NUM);
		m_pLuaInterface	=	0;
		m_OwnerCount	= 0;
		m_GrowPointCount	= 0;
		m_GrowPointTypeCount = 0;

	};

	~GrowPointManager()
	{
		for(INT i =0;i<MAX_SCENE_GROW_POINT_NUM;i++	)
		{
			if(m_GrowPointMgr[i])
				SAFE_DELETE(m_GrowPointMgr[i]);
		}
	};

	/*
	 *	 获得某种类型生长点管理器
	 *
	 *	 参数说明
	 *	 GrowPointType	生长点类型
	 *
	 */

	inline	TypeGrowPointManager*		GetGpManagerByGrowPointType(INT GrowPointType);
	

public:
	/*
	 *	加载操作
	 *	参数说明
	 *
	 *	CHAR*	strGrowPointDataFile			每个场景一个的GrowPoint数据文件
	 *												
	 *	CHAR*   strGrowPointSetupFile			每个场景一个的GrowPoint配置文件			
	 *	
	 */
	BOOL		Load(CHAR*	strGrowPointDataFile,CHAR* strGrowPointSetupFile);
		
	BOOL		Load(GROWPOINT_LOAD* pGPLoad,CHAR*	strGrowPointDataFile,CHAR* strGrowPointSetupFile);

	BOOL		Load(GROWPOINT_LOAD* pGPLoad);
	/*
	 *	清除操作
	 */
	VOID		CleanUp();
	
	
	/*
	 *	设置场景指针
	 */
	VOID		SetScene(Scene* pScene)
	{
		m_pScene	=	pScene;
	}

	/*
	 *	获得场景指针
	 */
	Scene*		GetScene(){return m_pScene;}

	/* 心跳入口
	 *	
	 */
	BOOL		HeartBeat(UINT	uTime);
	/*		 减少生长点类型数量
	 *
	 *		 参数说明
	 *				GrowPointType	生长点类型
	 *			
	 *		
	 */
	BOOL		DecGrowPointTypeCount(INT GrowPointType,FLOAT x,FLOAT y);

	/*---------			脚本接口处理		-----------------*/
	
	/*静态函数接口
	 *	
	 */
	LuaInterface*	GetLuaInterface(){return m_pLuaInterface;}
	
	/*调用脚本打开函数,OnOpen
	 *
	 *	参数说明:
	 *			FileID		脚本文件编号
	 *			SelfID		参与者id,一般填HumanID ,没有则添-1
	 *			TargetID	对象ID,一般填ItemBoxID
	 *			SceneID		场景编号
	 */
	INT			CallScriptOpenBoxFunc(ScriptID_t	FileID,ObjID_t SelfId,ObjID_t TargetId,SceneID_t	SceneID);
	
	/*
	 *	调用脚本回收函数,OnRecycle
	 *
	 *	参数说明:
	 *			FileID		脚本文件编号
	 *			SelfID		参与者id,一般填HumanID ,没有则添-1
	 *			TargetID	对象ID,一般填ItemBoxID
	 *			SceneID		场景编号
	 */
	INT			CallScriptRecycleFunc(ScriptID_t	FileID,ObjID_t SelfId,ObjID_t TargetId,SceneID_t	SceneID);
	/*	调用脚本过程结束函数,OnProcOver
	 *
	 *	参数说明:
	 *			FileID		脚本文件编号
	 *			SelfID		参与者id,一般填HumanID ,没有则添-1
	 *			TargetID	对象ID,一般填ItemBoxID
	 *			SceneID		场景编号
	 *	
	 */

	INT			CallScriptProcOverFunc(ScriptID_t	FileID,ObjID_t SelfId,ObjID_t TargetId,SceneID_t	SceneID);
	/*
	 *	调用脚本创建函数,OnCreate
	 *	参数说明:
	 *			FileID		脚本文件编号
	 *			SelfID		参与者id,一般填HumanID ,没有则添-1
	 *			TargetID	对象ID,一般填ItemBoxID
	 *			SceneID		场景编号
	 *	
	 */
	BOOL		CallScriptCreateFunc(ScriptID_t		FileID,FLOAT x,FLOAT y,INT iItemBoxType,SceneID_t SceneId);
private:
	/*			生长点数据初始化
	 *	
	 *			参数1:
	 *			szGrowPointDataFile
	 *	
	 *			参数1描叙
	 *			生长点数据文件
	 *
	 */
	BOOL	    GrowPointDataInit(CHAR*	szGrowPointDataFile);
	/*			生长点配置初始化
	 *
	 *			参数1:
	 *				szGrowPointSetupFile
	 *
	 *          参数1描叙
	 *				生长点配置文件
	 */
	BOOL		GrowPointSetupInit(CHAR* szGrowPointSetupFile);
public:
	
	/*
	 *		   添加生长点所有者信息访问方法
	 */

	BOOL		AddGrowPointOwner(ObjID_t OwnerID)
	{
		if(OwnerID == INVALID_ID)
			return FALSE;
		
		BOOL bNewOwner = TRUE;
		for(INT i = 0;i<m_OwnerCount;i++)
		{
			if(m_GPOwner[i].OwnerID == OwnerID)
			{
				m_GPOwner[i].OwnerCount++;
				bNewOwner = FALSE;
			}
		}
		
		if(bNewOwner)
		{
			m_GPOwner[m_OwnerCount].OwnerID = OwnerID;
			m_GPOwner[m_OwnerCount].OwnerCount = 1;
			
		}
		return TRUE;
	}

	BOOL		DelGrowPointOwner(ObjID_t OwnerID)
	{
		if(OwnerID == INVALID_ID)
			return FALSE;

		BOOL bNewOwner = TRUE;
		for(INT i = 0;i<m_OwnerCount;i++)
		{
			if(m_GPOwner[i].OwnerID == OwnerID)
			{
				m_GPOwner[i].OwnerCount--;
				bNewOwner = FALSE;
				if(m_GPOwner[i].OwnerCount==0)
				{
					m_GPOwner[i].OwnerID = INVALID_ID;
					m_OwnerCount--;
				}	
			}
		}

		if(bNewOwner)
		{
			return FALSE;
		}
		return TRUE;
	}

	INT			GetGrowPointCountByOwner(ObjID_t OwnerID)
	{
		if(OwnerID == INVALID_ID)
			return -1;

		for(INT i = 0;i<m_OwnerCount;i++)
		{
			if(m_GPOwner[i].OwnerID == OwnerID)
			{
				return m_GPOwner[i].OwnerCount;
			
			}
		}
		return -1;
	}



};

#include "GrowPointManager.inl"


#endif
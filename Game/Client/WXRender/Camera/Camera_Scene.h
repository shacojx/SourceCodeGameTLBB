#pragma once
#include "Camera.h"
#include <OgreSphere.h>
#include <OgreAxisAlignedBox.h>

/*
|	游戏流程中的摄像机
|
*/

namespace WX
{
	class TerrainData;
}

class CCamera_Scene : public CCamera
{
public:
	//-------------------------------------------------
	//移动所关注的位置到...Pitch
	//所传进的参数为相对于地形的坐标，以米为单位
	//一般来说，这个位置是玩家自己的头顶
	virtual VOID		SetLookAt(const fVector3& fvPos);
	
	//-------------------------------------------------
	//摄像机眼睛距离注视位置的距离, 以米为单位
	virtual VOID		SetAddHeight(FLOAT fAddHeight);
		   FLOAT		GetAddHeight(VOID) const { return m_fAddHeight; }

	//-------------------------------------------------
	//摄像机视线与水平线的夹角，一般为负数(向下看)
	static FLOAT		STATIC_PITCH;		//固定视角

	virtual VOID		SetPitch(FLOAT fPitch);
	
	//-------------------------------------------------
	//设置摄像机眼睛与人物之间的距离
	static FLOAT		MIN_DISTANCE;	//极限情况，摄像机能够离人物的最近距离
	static FLOAT		MAX_DISTANCE;	//极限情况，摄像机能够远离人物的最远距离

	virtual VOID		SetDistance(FLOAT fDistance);
	virtual FLOAT		GetDistance(VOID) const;
	//-------------------------------------------------
	//设置机视线与世界坐标x轴的角度 Should[0, 2л)
	virtual VOID		SetDirection(FLOAT fDirection);

	//-------------------------------------------------
	//设置加入一次震荡
			VOID		AddOneVib(FLOAT VibRange, FLOAT StepLen, UINT Times = 1 );



public:
	//摄像机构造函数
	// pOgreCamera  - 所附加到的ogre摄像机
	/*
	|
	|	!关于摄像机初始化
	|
	|		o 人物第一次进入场景时，面朝x轴所指向的方向
	|		o 摄像机则在人物的背后，注视人物的头部
	|
	*/
	CCamera_Scene(Ogre::Camera* pOgreCamera, 
						const fVector3& fvPos	= fVector3(0.0f, 0.0f, 0.0f),
						FLOAT fAddHeight		= 0.7f,
						FLOAT fPitch			= STATIC_PITCH, 
						FLOAT fDistance			= MAX_DISTANCE, 
						FLOAT fDirection		= 3.1416);
	virtual ~CCamera_Scene(VOID);

public:
	virtual VOID		Update(VOID);

protected:
	//-------------------------------------------------
	//摄像机状态
	enum CAMERA_STATUS
	{
		NORMAL_STATUS = 0,	//普通状态
		VIBRATION_STATUS,	//震荡状态
	};
	//-------------------------------------------------
	//震荡状态参数结构
	struct VibParam_t
	{
		FLOAT	fMaxBiasY;	//Y方向的向上的最大偏移
		FLOAT	fMinBiasY;	//Y方向的向下的最大偏移
		FLOAT	fBiasStep;
		UINT	nTimes;		//震荡周期个数
		VibParam_t()
		{
			fMaxBiasY = 0.0;
			fMinBiasY = 0.0;
			fBiasStep = 0.0;
			nTimes	  = 0;
		}
	};

protected:
	//相机所看的位置距离地面的高度，一般是人物的高度, 以米为单位
	FLOAT				m_fAddHeight;
	
	//摄像机当前状态
	CAMERA_STATUS		m_status;

	//当前震荡参数
	VibParam_t			m_CurVibParam;

protected:
	//计算眼睛位置
	static fVector3		_GetEyePos(
							const fVector3& fvLookAt,
							FLOAT fDistance,
							FLOAT fDirection,
							FLOAT fPitch);

	//-------------------------------------------------
	//物理碰撞检测

	//最小计算误差
	static FLOAT MIN_ANGLE;
	//最小调动范围
	static FLOAT STEP_ANGLE;

	//摄像机眼部的近距碰撞球
	struct CAMERA_NEAR_BALL
	{
		Ogre::Sphere sphere;	
	};

	//和摄像机眼睛有交互的地形网格(最多4个网格)
	struct CAMERA_INTER_GRIDS
	{
		std::vector< Ogre::AxisAlignedBox >	m_fvGrid;
	};

	//根据碰撞得到合理的Pitch
	static FLOAT		_GetValidPitch(
							const WX::TerrainData* pTerrainData,	//地形数据
							const fVector3& fvLookAt,				//关注点
							FLOAT fDistance,						//距离
							FLOAT fDirection,						//旋转角度
							FLOAT fPitchNow,						//目前的Pitch(尽量接近此值)
							FLOAT fPitchApproch,					//要趋近的Pitch
							FLOAT fCameraNearDis);					//眼睛的近裁减距离

	//眼睛是否合理
	static BOOL		_IsValidEye(
							const WX::TerrainData* pTerrainData,	//地形数据
							const fVector3& fvEye,					//眼睛点
							FLOAT fCameraNearDis);					//近距离

	//取得距离眼睛最近的地形数据
	static BOOL		_GetInterPatch(
							const WX::TerrainData* pTerrainData,	//地形数据
							const fVector3& fvEyePos,				//眼睛的投影位置
							CAMERA_INTER_GRIDS& theInterPatch);		//地形数据

	//设置当前摄像机状态
	BOOL			_SetStatus(CAMERA_STATUS nStatus );				

	//根据当前状态刷新摄像机
	VOID			_UpdateCurStatus(
							fVector3& vEyePos, 
							fVector3& vLookatPos);							


};

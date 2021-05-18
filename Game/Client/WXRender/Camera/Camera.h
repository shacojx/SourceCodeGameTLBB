#pragma once

/*
|
|	人物第三人称视角摄像机
|
|
|       CameraEye -> * _______
|                  ╱ ╲╯	Pitch
|                   ↖  ╲      
|                 Ditance ╲    
|                       ↘  ↘  
|                          ╱ o <-  CameraLookAt (PlayerHead)
|                            _|_
|                             |   <-PlayerMySelf
|                            / \
|        ---------------------+----------------------
|                              ↖
|                                ╲ PlayerPos
|
*/
namespace Ogre
{
	class Camera;
}

class CCamera
{
public:
	virtual VOID		SetLookAt(const fVector3& fvPos);

	virtual VOID		SetPitch(FLOAT fPitch);
	virtual FLOAT		GetPitch(VOID) const { return m_fPitch; }
	
	virtual VOID		SetDistance(FLOAT fDistance);
	virtual FLOAT		GetDistance(VOID) const { return m_fDistance; }

	virtual VOID		SetDirection(FLOAT fDirection);
	virtual FLOAT		GetDirection(VOID) const { return m_fDirection; }

public:
	Ogre::Camera*		GetOgreCamera(VOID) { return m_pOgreCamera; }
	virtual VOID		Update(VOID);

public:
	CCamera(Ogre::Camera* pOgreCamera);
	virtual ~CCamera();

protected:
	//摄像机所注视的位置 (渲染系统坐标)
	fVector3			m_fvLookatPos;
	//摄像机视线与水平线的夹角(弧度)
	FLOAT				m_fPitch;
	//摄像机眼睛距离注视位置的距离(渲染系统坐标)
	FLOAT				m_fDistance;
	//摄像机视线与世界坐标x轴的角度
	FLOAT				m_fDirection;
	//Ogre相机
	Ogre::Camera*		m_pOgreCamera;
};



#include "StdAfx.h"
#include "GMSoundLister.h"
#include "fmod374/fmod.h"


CSoundListenerFMod* CSoundListenerFMod::s_pMe = NULL;
CSoundListenerFMod::CSoundListenerFMod()
{
	s_pMe = this;

	m_vPos= fVector3(0.0f, 0.0f, 0.0f);
	m_vVel= fVector3(0.0f, 0.0f, 0.0f);
	m_vFront= fVector3(0.0f, 0.0f, 1.0f);
	m_vUp= fVector3(0.0f, 1.0f, 0.0f);
}

CSoundListenerFMod::~CSoundListenerFMod()
{
}

void CSoundListenerFMod::SetPos( const fVector3& pos )
{
	m_vPos = pos;
	_updateFModPos();
}

void CSoundListenerFMod::SetVelocity( const fVector3& vel )
{
	m_vVel= vel;
	_updateFModPos();
}

void CSoundListenerFMod::SetFront(const fVector3& front)
{
	m_vFront= front;
	_updateFModPos();
}

void CSoundListenerFMod::SetUp(const fVector3& up)
{
	m_vUp= up;
	_updateFModPos();
}

void CSoundListenerFMod::_updateFModPos()
{
	// set up FMod attributes
	float		pos[3];
	pos[0] = m_vPos.x;
	pos[1] = m_vPos.y;
	pos[2] = m_vPos.z;

	float		vel[3];
	vel[0] = m_vVel.x;
	vel[1] = m_vVel.y;
	vel[2] = m_vVel.z;

	FSOUND_3D_Listener_SetAttributes(pos, vel, 
		m_vFront.x, m_vFront.y, m_vFront.z, 
		m_vUp.x, m_vUp.y, m_vUp.z);
}

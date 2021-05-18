/****************************************\
*										*
* 				声音收听者				*
*										*
*						 by jinchao		*
\****************************************/
#pragma once

class CSoundListenerFMod
{
public:
    //收听者位置
	virtual void			SetPos(const fVector3& pos);
	virtual const fVector3&	GetPos(void) const	{ return m_vPos; }

    //收听者移动速度(缺省为0)
	virtual void			SetVelocity(const fVector3& vel);
	virtual const fVector3&	GetVelocity(void) const { return m_vVel; }

    //收听者的方向，面朝的方向和头顶的方向 (default: (0,0,1), (0,1,0))
	/*
	|     (top)
	|       y    
	|       |  x (left)
	|       | /
	|       -----z (front)
	*/
	virtual void			SetFront(const fVector3& front);
	virtual const fVector3&	GetFont(void) const { return m_vFront; }

	virtual void			SetUp(const fVector3& up);
	virtual const fVector3&	GetUp(void) const { return m_vUp; }

protected:
	void	_updateFModPos(void);

	// Nel Basis
	fVector3			m_vPos;
	fVector3			m_vVel;
	fVector3			m_vFront;
	fVector3			m_vUp;

public:
	CSoundListenerFMod();
	virtual ~CSoundListenerFMod();

	static CSoundListenerFMod* GetMe(VOID) { return s_pMe; }

protected:
	static CSoundListenerFMod*	s_pMe;
};


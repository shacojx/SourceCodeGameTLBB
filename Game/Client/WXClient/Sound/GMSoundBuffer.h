/****************************************\
*										*
* 				声音资源				*
*										*
\****************************************/
#pragma once
#include "TDSound.h"

struct FSOUND_SAMPLE;
class CSoundSourceFMod;

class CSoundBufferFMod : public tSoundBuffer
{
	friend class CSoundSystemFMod;
public:
	//读取wav资源
	virtual BOOL		ReadWavBuffer(const STRING& name);
	//释放资源
	virtual VOID		Release(VOID);
	//增加计数
	void				AddRefrence(CSoundSourceFMod* pSource) {m_nRefCount++;}
	//减少计数
	void				DecRefrence(CSoundSourceFMod* pSource) {m_nRefCount--;}
	//取得计数
	INT					GetRefrence(void) const { return m_nRefCount; }

protected:
	CSoundBufferFMod();
	virtual ~CSoundBufferFMod();

protected:
	friend class CSoundSourceFMod;

	//FMOD实例指针
	FSOUND_SAMPLE*		m_pFModSample;

	//引用计数
	INT					m_nRefCount;
};
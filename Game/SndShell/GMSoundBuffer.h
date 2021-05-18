/****************************************\
*										*
* 				声音资源				*
*										*
*						 by jinchao		*
\****************************************/
#pragma once

struct FSOUND_SAMPLE;
class CSoundBufferFMod
{
public:
	//读取wav资源
	virtual BOOL		ReadWavBuffer(const STRING& name);
	//释放资源
	virtual VOID		Release(VOID);

public:
	CSoundBufferFMod();
	virtual ~CSoundBufferFMod();

protected:
	friend class CSoundSourceFMod;

	//FMOD实例指针
	FSOUND_SAMPLE*		m_pFModSample;
};
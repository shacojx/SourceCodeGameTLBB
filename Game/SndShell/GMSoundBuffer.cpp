#include "StdAfx.h"
#include "GMSoundBuffer.h"
#include "GMSoundSystem.h"
#include "fmod374/fmod.h"
#include <assert.h>
#pragma comment(lib, "Winmm.lib.")

CSoundBufferFMod::CSoundBufferFMod()
{
	m_pFModSample= NULL;
}

CSoundBufferFMod::~CSoundBufferFMod()
{
	Release();
}

VOID CSoundBufferFMod::Release(VOID)
{
	if(m_pFModSample)
	{
		FSOUND_Sample_Free(m_pFModSample);
		m_pFModSample= NULL;
	}
}

BOOL CSoundBufferFMod::ReadWavBuffer(const STRING &name)
{
	Release();

	m_pFModSample = FSOUND_Sample_Load(FSOUND_FREE, 
		(const char*)name.c_str(), 
		FSOUND_HW3D, 0, 0);
	
	if(!m_pFModSample) return FALSE;

	//缺省所有声音都是不循环的
    FSOUND_Sample_SetMode(m_pFModSample, FSOUND_LOOP_OFF);

	return TRUE;
}

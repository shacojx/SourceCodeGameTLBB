#include "StdAfx.h"
#include "GMSoundBuffer.h"
#include "GMSoundSystem.h"
#include "../Procedure/GameProcedure.h"
#include "../WXRender/ResourceProvider.h"
#include "fmod375/fmod.h"
#include "TDException.h"
#include <assert.h>
#pragma comment(lib, "Winmm.lib.")

CSoundBufferFMod::CSoundBufferFMod()
{
	m_pFModSample= NULL;
	m_nRefCount = 0;
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

	char* lpAddress = NULL;
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource(name.c_str(), lpAddress, "General");
	if(dwSize > 0)
	{
		m_pFModSample = FSOUND_Sample_Load(FSOUND_FREE, 
				lpAddress, 
				FSOUND_HW3D | FSOUND_LOADMEMORY, 0, dwSize);
		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
	}
	
	if(!m_pFModSample) 
	{
		TDThrow("Invalid sound file:%s", name.c_str());
		return FALSE;
	}

	//缺省所有声音都是不循环的
    FSOUND_Sample_SetMode(m_pFModSample, FSOUND_LOOP_OFF);

	return TRUE;
}

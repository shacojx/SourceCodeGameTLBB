// SndShell.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "GMSoundSystem.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


static CSoundSystemFMod theSoundSystem;
bool g_bInit = false;

bool __stdcall SSInit(const char* szSndPath, const char* szDBCFile)
{
    g_bInit = true;

    try
    {
        theSoundSystem.Initial(szSndPath, szDBCFile);
    }
    catch (...)
    {
    	g_bInit = false;
    }

    return g_bInit;
}

bool __stdcall SSEnumSoundFile(int nIndex, char* szSoundFileBuf, int nBufSize, int& id)
{
	if(!g_bInit) return false;

	STRING strSndFile;
	BOOL bHave = CSoundSystemFMod::GetMe()->EnumSoundFile(nIndex, strSndFile, id);
	if(!bHave) return false;

	strncpy(szSoundFileBuf, strSndFile.c_str(), nBufSize-1);
	return true;
}

int __stdcall SSPlaySoundFunc(const char* szSoundFile, float* fvPos, bool bLoop)
{
	if(!g_bInit) return -1;

	return CSoundSystemFMod::GetMe()->_PlaySoundFunc(szSoundFile, fvPos, bLoop);
}


void __stdcall SSStopSoundFunc(int nHandle)
{
	if(!g_bInit) return;

	CSoundSystemFMod::GetMe()->_StopSoundFunc(nHandle);
}

void __stdcall  SSDeInit(void)
{
    if (g_bInit)
    {
        theSoundSystem.Release();
        g_bInit = false;
    }
}

void __stdcall  SSSetSoundVolume(int handle, float gain)
{
    if(!g_bInit) return;

    CSoundSystemFMod::GetMe()->SetSoundVolume(handle, gain);
}
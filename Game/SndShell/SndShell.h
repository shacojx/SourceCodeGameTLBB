
#pragma once

//对外提供的声音播放接口,返回声音源HANDLE
typedef int (__stdcall* FUNC_PLAYSOUND)(const char* szSoundFile, float* fvPos, bool bLoop);
//对外提供声音的停止接口
typedef VOID (__stdcall* FUNC_STOPSOUND)(int);



bool __stdcall SSInit(const char* szSndPath, const char* szDBCFile);

bool __stdcall SSEnumSoundFile(int nIndex, char* szSoundFileBuf, int nBufSize, int& id);

int __stdcall SSPlaySoundFunc(const char* szSoundFile, float* fvPos, bool bLoop);

void __stdcall  SSStopSoundFunc(int nHandle);

void __stdcall  SSDeInit(void);

void __stdcall  SSSetSoundVolume(int handle, float gain);
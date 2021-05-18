#include "stdafx.h"

#include "ShareMemAPI.h"

#if __LINUX__
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <errno.h>
#elif __WINDOWS__
#include <WinBase.h>
#endif

namespace ShareMemAPI
{


SMHandle		CreateShareMem(SM_KEY key,UINT Size)
{
	__ENTER_FUNCTION
	//CHAR keybuf[64];
	//memset(keybuf,0,64);
	//sprintf(keybuf,"./%d",key);
#if __LINUX__
	//key = ftok(keybuf,'w'); 
	SMHandle hd =  shmget(key ,Size,IPC_CREAT|IPC_EXCL|0666); 
	printf("handle = %d ,key = %d ,error: %d \r\n",hd,key,errno);
	return hd;
#elif __WINDOWS__
	CHAR keybuf[64];
	memset(keybuf,0,64);
	sprintf(keybuf,"%d",key);
	return  CreateFileMapping( (HANDLE)0xFFFFFFFFFFFFFFFF, NULL, PAGE_READWRITE, 0, Size, keybuf);
#endif
	__LEAVE_FUNCTION
	return SMHandle(-1);

}

SMHandle		OpenShareMem(SM_KEY key,UINT Size)
{
	__ENTER_FUNCTION
	//CHAR keybuf[64];
	//memset(keybuf,0,64);
	//sprintf(keybuf,"./%d",key);
#if __LINUX__
	//key = ftok(keybuf,'w'); 
	SMHandle hd =   shmget(key , Size,0);
	printf("handle = %d ,key = %d ,error: %d \r\n",hd,key,errno);
	return hd;
#elif __WINDOWS__
	CHAR keybuf[64];
	memset(keybuf,0,64);
	sprintf(keybuf,"%d",key);
	return OpenFileMapping( FILE_MAP_ALL_ACCESS, TRUE, keybuf);
#endif
	__LEAVE_FUNCTION
		return SMHandle(-1);}

CHAR*			MapShareMem(SMHandle handle)
{
	__ENTER_FUNCTION

#if __LINUX__
	return  (CHAR*)shmat(handle,0,0);
#elif __WINDOWS__
	return (CHAR *)MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#endif
	__LEAVE_FUNCTION
		return 0;

}


VOID			UnMapShareMem(CHAR* MemoryPtr)
{
	__ENTER_FUNCTION
#if __LINUX__
	  shmdt(MemoryPtr);
#elif __WINDOWS__
	 UnmapViewOfFile(MemoryPtr);
#endif
	__LEAVE_FUNCTION
}


VOID			CloseShareMem(SMHandle handle)
{
	__ENTER_FUNCTION
#if __LINUX__
	shmctl(handle,IPC_RMID,0); 
#elif __WINDOWS__
	CloseHandle(handle);
#endif
	__LEAVE_FUNCTION
}

}
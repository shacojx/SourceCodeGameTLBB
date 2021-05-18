#include "stdafx.h"
#include "ShareMemAO.h"
#include "Log.h"
#include "ShareMemAPI.h"

using namespace ShareMemAPI;






BOOL	ShareMemAO::Create(SM_KEY key,uint Size)
{
	__ENTER_FUNCTION
		
		if(m_CmdArg == CMD_MODE_CLEARALL )
			return FALSE;
	
		m_hold	= ShareMemAPI::CreateShareMem(key,Size);
		if(m_hold == INVALID_SM_HANDLE)
		{
			Log::SaveLog(SHMEM_LOG_PATH,"Create ShareMem Error SM_KET = %d",key);
			return FALSE;
		}
		m_pHeader = ShareMemAPI::MapShareMem(m_hold);

		if(m_pHeader)
		{	
			m_pDataPtr = m_pHeader+sizeof(SMHead);
			((SMHead*)m_pHeader)->m_Key	 =	key;
			((SMHead*)m_pHeader)->m_Size =	Size;
			m_Size	=	Size;
			Log::SaveLog(SHMEM_LOG_PATH,"Create ShareMem Ok SM_KET = %d",key);
			return TRUE;
		}
		else
		{
			Log::SaveLog(SHMEM_LOG_PATH,"Map ShareMem Error SM_KET = %d",key);
			return FALSE;
		}
		
	
	
	__LEAVE_FUNCTION
		return FALSE ;
}

VOID	ShareMemAO::Destory()
{
	__ENTER_FUNCTION


	if( m_pHeader )
	{
		ShareMemAPI::UnMapShareMem(m_pHeader);
		m_pHeader = 0;
	}
	if( m_hold )
	{
		ShareMemAPI::CloseShareMem(m_hold);
		m_hold = 0;
	}

	m_Size	=	0;

	__LEAVE_FUNCTION
}

BOOL	ShareMemAO::Attach(SM_KEY key,uint	Size)
{

	__ENTER_FUNCTION

	m_hold	=	ShareMemAPI::OpenShareMem(key,Size);

	if(m_CmdArg == CMD_MODE_CLEARALL)
	{
		Destory();
		printf("Close ShareMemory key = %d \r\n",key);
		return FALSE;
	}
	

	if(m_hold == INVALID_SM_HANDLE)
	{
		
		Log::SaveLog(SHMEM_LOG_PATH,"Attach ShareMem Error SM_KET = %d",key);
		return FALSE;
	}
	
	m_pHeader = ShareMemAPI::MapShareMem(m_hold);

	if(m_pHeader)
	{
		m_pDataPtr = m_pHeader+sizeof(SMHead);
		Assert(((SMHead*)m_pHeader)->m_Key	 ==	key);
		Assert(((SMHead*)m_pHeader)->m_Size  ==	Size);
		m_Size	=	Size;
		Log::SaveLog(SHMEM_LOG_PATH,"Attach ShareMem OK SM_KET = %d",key);
		return TRUE;
	}
	else
	{
		Log::SaveLog(SHMEM_LOG_PATH,"Map ShareMem Error SM_KET = %d",key);
		return FALSE;
	}

	__LEAVE_FUNCTION

		return FALSE;

}

BOOL	ShareMemAO::DumpToFile(CHAR* FilePath)
{
	__ENTER_FUNCTION	

		Assert(FilePath);
		
		FILE* f	= fopen(FilePath,"wb");
		if(!f)	
			return FALSE;
		fwrite(m_pHeader,1,m_Size,f);
		fclose(f);
		
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL ShareMemAO::MergeFromFile(CHAR* FilePath)
{
	__ENTER_FUNCTION

		Assert(FilePath);
		
		FILE*	f = fopen(FilePath,"rb");
		if(!f)
			return FALSE;
		fseek(f,0L,SEEK_END);
		INT FileLength =ftell(f);
		fseek(f,0L,SEEK_SET);
		fread(m_pHeader,FileLength,1,f);
		fclose(f);

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

VOID ShareMemAO::SetHeadVer(UINT ver)
{
	__ENTER_FUNCTION
		
		((SMHead*)m_pHeader)->m_HeadVer = ver;

	__LEAVE_FUNCTION
}

UINT ShareMemAO::GetHeadVer()
{
	__ENTER_FUNCTION

		UINT ver = ((SMHead*)m_pHeader)->m_HeadVer;
		return ver;

	__LEAVE_FUNCTION

		return 0;
}
/****************************************\
*										*
* 			声音/音效管理器				*
*										*
*						 by jinchao		*
\****************************************/

//
//ref NEL(R) Copyright, 2001 Nevrax Ltd.
//
#include "StdAfx.h"
#include "GMSoundSystem.h"
#include "GMSoundBuffer.h"
#include "GMSoundSource.h"
#include "../SceneEdit/src/TLBB_DBC.h"

CSoundSystemFMod* CSoundSystemFMod::s_pMe = NULL;
CSoundSystemFMod::CSoundSystemFMod()
{
	s_pMe = this;

	m_bMuteAll = FALSE;
	m_fMasterGain = 1.0f;
	m_vListenerPos = fVector3(0.0f, 0.0f, 0.0f);
}

CSoundSystemFMod::~CSoundSystemFMod()
{
}

VOID CSoundSystemFMod::Initial(LPCTSTR szSndPath, LPCTSTR szDBCFile)
{
	assert(szSndPath);
	assert(szDBCFile);

    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
		throw("FMOD: You are using the wrong DLL version! Should be %.2f", FMOD_VERSION);
    }

	FSOUND_SetOutput(FSOUND_OUTPUT_WINMM);
	FSOUND_SetDriver(0);
    FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
	FSOUND_3D_SetRolloffFactor(0);

	if(!FSOUND_Init(44100, 32, 0))
	{
		throw("Failed to create the FMod driver object");
	}

	m_strSoundPath = szSndPath;
	m_strSndDBCFile = szDBCFile;

	//查询资源定义
	static DBC::DBCFile fileSnd(0);
	BOOL fileOpened = fileSnd.OpenFromTXT(m_strSndDBCFile.c_str());

    if (!fileOpened)
    {
        throw("Failed to open config file!");
    }

	INT nLineNum = (INT)fileSnd.GetRecordsNum();
	for(INT i=0; i<nLineNum; i++)
	{
		const _DBC_SOUND_INFO* pDefine = (const _DBC_SOUND_INFO*)fileSnd.Search_Posistion(i, 0);
		if(!pDefine) continue;

		SOUND_BUFFER newSound;
		newSound.pDefine = pDefine;
		newSound.pSoundBuf = NULL;

		//加入列表
		m_listSoundBuffer.push_back(newSound);

		//加入索引
		m_mapSoundID[pDefine->nID] = &(m_listSoundBuffer.back());
		m_mapSoundFile[pDefine->pSoundFile] = &(m_listSoundBuffer.back());
	}
}

VOID CSoundSystemFMod::Release(VOID)
{
	//---------------------------------
	//释放所有声音源
	std::list< SOUND_SOURCE >::iterator itSndSource;
	for(itSndSource=m_listSoundSource.begin(); itSndSource!=m_listSoundSource.end(); itSndSource++)
	{
		SOUND_SOURCE& sndSource = *itSndSource;

		if(sndSource.pSoundSource)
		{
			delete sndSource.pSoundSource;
			sndSource.pSoundSource = NULL;
		}
	}
	m_listSoundSource.clear();

	//---------------------------------
	//释放所有声音资源
	std::list< SOUND_BUFFER >::iterator itSndBuffer;
	for(itSndBuffer = m_listSoundBuffer.begin(); itSndBuffer!=m_listSoundBuffer.end(); itSndBuffer++)
	{
		SOUND_BUFFER& sndBuffer= *itSndBuffer;

		//释放声音资源
		if(sndBuffer.pSoundBuf)
		{
			delete sndBuffer.pSoundBuf;
			sndBuffer.pSoundBuf = NULL;
		}
	}

	//释放列表
	m_listSoundBuffer.clear();
	m_mapSoundID.clear();
	m_mapSoundFile.clear();
}

BOOL CSoundSystemFMod::EnumSoundFile(INT nIndex, STRING& strFileName, int& id)
{
	if(nIndex <0 || nIndex >= (INT)m_listSoundBuffer.size()) return FALSE;

	int nTheIndex = 0;
	std::list< SOUND_BUFFER >::iterator it;
	for(it=m_listSoundBuffer.begin(); it!=m_listSoundBuffer.end(); it++,nTheIndex++)
	{
		SOUND_BUFFER& theBuf = *it;
		if(nTheIndex == nIndex) 
		{
			strFileName = theBuf.pDefine->pSoundFile;
            id = theBuf.pDefine->nID;
			return TRUE;
		}
	}

	return FALSE;
}

VOID CSoundSystemFMod::Tick(VOID)
{
#if 0
	//------------------------------------
	//刷新听者位置
	CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
	if(pMySelf)
	{
		Listener_SetPos(pMySelf->GetPosition());
	}

	//------------------------------------
	//更新声音源音量
	std::list< SOUND_SOURCE >::iterator it;
	std::list< SOUND_SOURCE >::iterator itNext;
	for(it = m_listSoundSource.begin(); it!=m_listSoundSource.end(); it=itNext)
	{
		itNext = it; itNext++;

		SOUND_SOURCE& theSnd = *it;

		//如果是自动销毁的声音源,自动销毁
		theSnd.pSoundSource->UpdateStatus();
		if(theSnd.bAutoDestroy && theSnd.pSoundSource->IsStopped())
		{
			delete theSnd.pSoundSource;
			m_listSoundSource.erase(it);
			continue;
		}

		//更新音量
		theSnd.pSoundSource->UpdateVolume();
	}

	//------------------------------------
	//如果窗口非激活，静音
	if(!CGameProcedure::IsWindowActive() && !m_bMuteAll)
	{
		MuteAll(TRUE);
	}
	else if(CGameProcedure::IsWindowActive() && m_bMuteAll)
	{
		MuteAll(FALSE);
	}
#endif
}

//当窗口不激活时，将所有声音静音
VOID CSoundSystemFMod::MuteAll(BOOL bMute)
{
	if(bMute == m_bMuteAll) return;

	std::list< SOUND_SOURCE >::iterator it;
	for(it=m_listSoundSource.begin(); it!=m_listSoundSource.end(); it++)
	{
		SOUND_SOURCE& sndSource = *it;

		if(sndSource.pSoundSource && sndSource.pSoundSource->GetFModChannel() >= 0)
			FSOUND_SetMute(sndSource.pSoundSource->GetFModChannel(), bMute);
	}
	m_bMuteAll = bMute;
}

//创建一个声音资源
CSoundBufferFMod* CSoundSystemFMod::Buffer_Create(INT nID)
{
	std::map< INT, SOUND_BUFFER* >::iterator it;
	it = m_mapSoundID.find(nID);

	//非法ID
	if(it == m_mapSoundID.end()) return NULL;

	return Source_Load(*(it->second));
}

CSoundBufferFMod* CSoundSystemFMod::Buffer_Create(LPCTSTR szFileName)
{
	std::map< STRING, SOUND_BUFFER* >::iterator it;
	it = m_mapSoundFile.find(szFileName);

	//非法文件名
	if(it == m_mapSoundFile.end()) return NULL;

	return Source_Load(*(it->second));
}

CSoundBufferFMod* CSoundSystemFMod::Source_Load(SOUND_BUFFER& sndBuffer)
{
	//尚未加载
	if(sndBuffer.pSoundBuf == NULL)
	{
		//加载资源
		CSoundBufferFMod* pSoundBuf = new CSoundBufferFMod;

		STRING strFullFileName;
		strFullFileName = m_strSoundPath;
		strFullFileName += sndBuffer.pDefine->pSoundFile;

		BOOL bSucc = pSoundBuf->ReadWavBuffer(strFullFileName);
		if(!bSucc) 
        {
            delete pSoundBuf;
            pSoundBuf = NULL;
        }

		//放入声音map
		sndBuffer.pSoundBuf = pSoundBuf;
	}

	return sndBuffer.pSoundBuf;
}


//创建一个播放源
CSoundSourceFMod* CSoundSystemFMod::Source_Create(BOOL b3DMode, BOOL bAutoDestroy)
{
	CSoundSourceFMod* pSource = new CSoundSourceFMod(b3DMode);

	SOUND_SOURCE newSource;
	newSource.bAutoDestroy = bAutoDestroy;
	newSource.pSoundSource = pSource;

	m_listSoundSource.push_back(newSource);
	return (CSoundSourceFMod*)pSource;
}

VOID CSoundSystemFMod::Source_Destroy(CSoundSourceFMod* pSource)
{
	//查询ID
	std::list< SOUND_SOURCE >::iterator it;
	for(it=m_listSoundSource.begin(); it!=m_listSoundSource.end(); it++)
	{
		SOUND_SOURCE& theSnd = *it;

		if(theSnd.pSoundSource == pSource)
		{
			//删除实例
			delete theSnd.pSoundSource;
			m_listSoundSource.erase(it);
			return;
		}
	}
}

int WINAPI CSoundSystemFMod::_PlaySoundFunc(const char* szSoundFile, float* fvPos, bool bLoop)
{
	return GetMe()->PlaySoundFunc(szSoundFile, fvPos, bLoop);
}

void WINAPI CSoundSystemFMod::_StopSoundFunc(int nHandle)
{
	GetMe()->StopSoundFunc(nHandle);
}

INT CSoundSystemFMod::PlaySoundFunc(LPCTSTR szSoundFile, FLOAT* fvPos, BOOL bLoop)
{
	//-----------------------------------------------------------------------
	//创建3d声音播放源

	//加载资源
	CSoundBufferFMod* pSndBuffer = Buffer_Create(szSoundFile);
	if(!pSndBuffer) return -1;

	//创建一个声音源，自动删除
	CSoundSourceFMod* pSndSource = Source_Create(FALSE, TRUE);
	if(!pSndSource) return -1;

	//设置参数开始播放
	pSndSource->SetBuffer(pSndBuffer);
	pSndSource->SetLooping(bLoop);

	pSndSource->Play();

	return (INT)(INT_PTR)pSndSource;
}

VOID CSoundSystemFMod::StopSoundFunc(INT nHandle)
{
	if(nHandle == -1) return;

	//转化为声音指针
	CSoundSourceFMod* pSnd = (CSoundSourceFMod*)(INT_PTR)nHandle;

	//查找是否有该声音正在播放
	std::list< SOUND_SOURCE >::iterator it;
	for(it=m_listSoundSource.begin(); it!=m_listSoundSource.end(); it++)
	{
		SOUND_SOURCE& theSnd = *it;
		if(theSnd.pSoundSource == pSnd)
		{
			//删除实例
			delete theSnd.pSoundSource;
			m_listSoundSource.erase(it);
			return;
		}
	}

}

VOID WINAPI CSoundSystemFMod::SetSoundVolume(INT nHandle, FLOAT gain)
{
    if(nHandle == -1) return;

    //转化为声音指针
    CSoundSourceFMod* pSnd = (CSoundSourceFMod*)(INT_PTR)nHandle;

    pSnd->SetGain(gain);
}
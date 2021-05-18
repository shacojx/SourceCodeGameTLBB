/****************************************\
*										*
* 			声音/音效管理器				*
*										*
\****************************************/

//
//ref NEL(R) Copyright, 2001 Nevrax Ltd.
//
#include "StdAfx.h"
#include "GMSoundSystem.h"
#include "GMSoundBuffer.h"
#include "GMSoundSource.h"
#include "../DBC/GMDataBase.h"
#include "TDException.h"
#include "../Procedure/GameProcedure.h"
#include "../Object/ObjectManager.h"
#include "../Object/Character/Obj_PlayerMySelf.h"
#include "fmod375/fmod.h"
#include "fmod375/fmod_errors.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Variable\GMVariable.h"
#include "..\Event\GMEventSystem.h"
#include "..\Script\GMScriptSystem.h"
#include "..\WxRender\RenderSystem.h"

#pragma comment(lib, "Sound/fmod375/fmodvc.lib")

const CHAR CSoundSystemFMod::szAllSoundDisable[] = "DisableAllSound"; //关闭所有音效的开关
const CHAR CSoundSystemFMod::szBKSoundVar[] = "EnableBGSound";	//背景音效
const CHAR CSoundSystemFMod::sz3DSoundVar[] = "Enable3DSound";	//环境音效
const CHAR CSoundSystemFMod::szSKSoundVar[] = "EnableSKSound";	//技能音效
const CHAR CSoundSystemFMod::szUISoundVar[] = "EnableUISound";	//事件音效

const CHAR CSoundSystemFMod::szBKVolumeVar[] = "VOLUME_BG";	//背景音量
const CHAR CSoundSystemFMod::sz3DVolumeVar[] = "VOLUME_3D";	//环境音量
const CHAR CSoundSystemFMod::szSKVolumeVar[] = "VOLUME_SK";	//技能音量
const CHAR CSoundSystemFMod::szUIVolumeVar[] = "VOLUME_UI";	//事件音量

WX_IMPLEMENT_DYNAMIC(CSoundSystemFMod, GETCLASS(tSoundSystem));
CSoundSystemFMod* CSoundSystemFMod::s_pMe = NULL;
CSoundSystemFMod::CSoundSystemFMod()
{
	s_pMe = this;

	m_bMuteAll = FALSE;
	m_fMasterGain = 1.0f;
	m_vListenerPos = fVector3(0.0f, 0.0f, 0.0f);

	m_strSoundPath = "..\\Sound\\";

	m_pMetaTable = 0;
}

CSoundSystemFMod::~CSoundSystemFMod()
{
	if(m_pMetaTable) delete m_pMetaTable;
	m_pMetaTable = 0;
}

VOID CSoundSystemFMod::Initial(VOID*)
{
    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
		TDThrow("FMOD: You are using the wrong DLL version! Should be %.2f", FMOD_VERSION);
    }

	FSOUND_SetOutput(FSOUND_OUTPUT_WINMM);
	FSOUND_SetDriver(0);
    FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
	FSOUND_3D_SetRolloffFactor(0);

	if(!FSOUND_Init(44100, 32, 0))
	{
		TDThrow("Failed to create the FMod driver object");
	}

	//查询资源定义
	DBC_DEFINEHANDLE(s_dbcSound, DBC_SOUND_INFO);
	INT nLineNum = (INT)s_dbcSound->GetRecordsNum();
	for(INT i=0; i<nLineNum; i++)
	{
		const _DBC_SOUND_INFO* pDefine = (const _DBC_SOUND_INFO*)s_dbcSound->Search_LineNum_EQU(i);
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

	//挂接变量系统
	CEventSystem::GetMe()->RegisterEventHandle("VARIABLE_CHANGED", _OnVariableChangedEvent);

	//设置如果未定义变量初试值
	CVariableSystem::GetMe()->SetVariableDefault(szBKSoundVar, "1");
	CVariableSystem::GetMe()->SetVariableDefault(sz3DSoundVar, "1");
	CVariableSystem::GetMe()->SetVariableDefault(szSKSoundVar, "1");
	CVariableSystem::GetMe()->SetVariableDefault(szUISoundVar, "1");

	//-----------------------------------------------------------
	//导出函数
	m_pMetaTable = new LuaPlus::LuaObject;
	*m_pMetaTable = CScriptSystem::GetMe()->GetLuaState()->GetGlobals().CreateTable("Sound");
	m_pMetaTable->SetObject("__index", *m_pMetaTable);

	m_pMetaTable->RegisterObjectFunctor("PlaySound", &CSoundSystemFMod::Lua_PlaySound);
	m_pMetaTable->RegisterObjectFunctor("StopSound", &CSoundSystemFMod::Lua_StopSound);

	LuaPlus::LuaObject obj_Variable = CScriptSystem::GetMe()->GetLuaState()->BoxPointer(this);
	obj_Variable.SetMetaTable(*m_pMetaTable);
	CScriptSystem::GetMe()->GetLuaState()->GetGlobals().SetObject("Sound", obj_Variable);
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

	//释放FMod
	FSOUND_Close();
}

BOOL CSoundSystemFMod::IsTypeEnabled(tSoundSource::Type type)
{
	CVariableSystem* pVarSystem = CVariableSystem::GetMe();
	bool bRet = false;
	switch(type)
	{
	case tSoundSource::type_background:
		bRet = (pVarSystem->GetAs_Int(szBKSoundVar)==1);
		break;

	case tSoundSource::type_circumstance:
		bRet = (pVarSystem->GetAs_Int(sz3DSoundVar)==1);
		break;

	case tSoundSource::type_skill:
		bRet = (pVarSystem->GetAs_Int(szSKSoundVar)==1);
		break;

	case tSoundSource::type_UI:
		bRet = (pVarSystem->GetAs_Int(szUISoundVar)==1);
		break;

	default:
		bRet = false;
		break;
	}

	return bRet;
}

float CSoundSystemFMod::GetTypeVolume(tSoundSource::Type type)
{
	if(m_bMuteAll) return 0.0f;

	CVariableSystem* pVarSystem = CVariableSystem::GetMe();
	float gain = 0.0f;
	switch(type)
	{
	case tSoundSource::type_background:
		gain = pVarSystem->GetAs_Float(szBKVolumeVar);
		break;

	case tSoundSource::type_circumstance:
		gain = pVarSystem->GetAs_Float(sz3DVolumeVar);
		break;

	case tSoundSource::type_skill:
		gain = pVarSystem->GetAs_Float(szSKVolumeVar);
		break;

	case tSoundSource::type_UI:
		gain = pVarSystem->GetAs_Float(szUIVolumeVar);
		break;

	default:
		gain = 0.0f;
		break;
	}

	return gain;
}

VOID CSoundSystemFMod::Tick(VOID)
{
	//------------------------------------
	//刷新听者位置
	CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
	if(pMySelf)
	{
		Listener_SetPos(pMySelf->GetPosition());
	}

	//------------------------------------
	//更新声音源音量
	{
		std::list< SOUND_SOURCE >::iterator it, itNext;
		for(it = m_listSoundSource.begin(); it!=m_listSoundSource.end(); it=itNext)
		{
			itNext = it; itNext++;

			SOUND_SOURCE& theSnd = *it;

			//如果是自动销毁的声音源,自动销毁
			theSnd.pSoundSource->UpdateStatus();

			if(theSnd.bAutoDestroy && theSnd.pSoundSource->IsStopped())
			{
				theSnd.pSoundSource->Stop();
				delete theSnd.pSoundSource;
				m_listSoundSource.erase(it);
				continue;
			}

			//更新音量
			theSnd.pSoundSource->UpdateVolume();
		}
	}

	//------------------------------------
	//销毁没有被引用的声音资源
	{
		std::list< SOUND_BUFFER >::iterator it;

		for(it = m_listSoundBuffer.begin(); it!=m_listSoundBuffer.end(); it++)
		{
			SOUND_BUFFER& theBuf = *it;

			//如果引用计数为0，销毁
			if(theBuf.pSoundBuf && theBuf.pSoundBuf->GetRefrence() == 0)
			{
				delete theBuf.pSoundBuf;
				theBuf.pSoundBuf = 0;
			}
		}
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
tSoundBuffer* CSoundSystemFMod::Buffer_Create(INT nID)
{
	std::map< INT, SOUND_BUFFER* >::iterator it;
	it = m_mapSoundID.find(nID);

	//非法ID
	if(it == m_mapSoundID.end())
	{
		TDThrow("Invalid sound source id: %d", nID);
		return NULL;
	}

	return Source_Load(*(it->second));
}

tSoundBuffer* CSoundSystemFMod::Buffer_Create(LPCTSTR szFileName)
{
	std::map< STRING, SOUND_BUFFER* >::iterator it;
	it = m_mapSoundFile.find(szFileName);

	//非法文件名
	if(it == m_mapSoundFile.end()) return NULL;

	return Source_Load(*(it->second));
}

tSoundBuffer* CSoundSystemFMod::Source_Load(SOUND_BUFFER& sndBuffer)
{
	//尚未加载
	if(sndBuffer.pSoundBuf == NULL)
	{
		//加载资源
		CSoundBufferFMod* pSoundBuf = new CSoundBufferFMod;

		if(!(pSoundBuf->ReadWavBuffer(sndBuffer.pDefine->pSoundFile)))
			return NULL;

		//放入声音map
		sndBuffer.pSoundBuf = pSoundBuf;
	}

	return sndBuffer.pSoundBuf;
}


//创建一个播放源
tSoundSource* CSoundSystemFMod::Source_Create(tSoundSource::Type type, BOOL b3DMode, 
												BOOL bAutoDestroy)
{
	static INT s_idSource = 1;

	CSoundSourceFMod* pSource = new CSoundSourceFMod(s_idSource++, type, b3DMode);

	SOUND_SOURCE newSource;
	newSource.bAutoDestroy = bAutoDestroy;
	newSource.pSoundSource = pSource;

	m_listSoundSource.push_back(newSource);
	return (tSoundSource*)pSource;
}

VOID CSoundSystemFMod::Source_Destroy(tSoundSource* pSource)
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

VOID CSoundSystemFMod::Source_Destroy(INT id)
{
	//查询ID
	std::list< SOUND_SOURCE >::iterator it;
	for(it=m_listSoundSource.begin(); it!=m_listSoundSource.end(); it++)
	{
		SOUND_SOURCE& theSnd = *it;

		if(theSnd.pSoundSource && theSnd.pSoundSource->GetID() == id)
		{
			//删除实例
			delete theSnd.pSoundSource;
			m_listSoundSource.erase(it);
			return;
		}
	}
}

void CSoundSystemFMod::PlayUISoundFunc( INT nID )
{
	//-----------------------------------------------------------------------
	//创建3d声音播放源
	if(CVariableSystem::GetMe()->GetAs_Int(szUISoundVar) != TRUE) return;

	//加载资源
	tSoundBuffer* pSndBuffer = Buffer_Create(nID);
	if(!pSndBuffer) return;

	//创建一个声音源，自动删除
	tSoundSource* pSndSource = Source_Create(CSoundSourceFMod::type_UI, FALSE, TRUE);
	if(!pSndSource) return;

	//设置参数开始播放
	pSndSource->SetBuffer(pSndBuffer);
	pSndSource->SetLooping(FALSE);

	pSndSource->Play();
}

INT CSoundSystemFMod::PlaySoundFunc(LPCTSTR szSoundFile, FLOAT* fvPos, BOOL bLoop)
{
	//-----------------------------------------------------------------------
	//坐标转换
	fVector3 fvGfx(fvPos[0], fvPos[1], fvPos[2]);
	fVector3 fvGame;
	if(!CGameProcedure::s_pGfxSystem->Axis_Trans(CRenderSystem::AX_GFX, fvGfx, 
			CRenderSystem::AX_GAME, fvGame))
	{
		return -1;
	}

	//-----------------------------------------------------------------------
	//创建3d声音播放源

	//加载资源
	tSoundBuffer* pSndBuffer = Buffer_Create(szSoundFile);
	if(!pSndBuffer) return -1;

	//创建一个声音源，自动删除
	tSoundSource* pSndSource = Source_Create(CSoundSourceFMod::type_skill, TRUE, TRUE);
	if(!pSndSource) return -1;

	//设置参数开始播放
	pSndSource->SetBuffer(pSndBuffer);
	pSndSource->SetPos(fvGame);
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

VOID WINAPI CSoundSystemFMod::_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData)
{
	if(pEvent && pEvent->pEventDef && pEvent->pEventDef->idEvent == GE_VARIABLE_CHANGED)
	{
		const STRING& strVariable = pEvent->vArg[0];
		const STRING& strValue = pEvent->vArg[1];

		BOOL bEnable = (strValue=="1");

		//停止指定声音
		if(!bEnable)
		{
			if(strVariable == szSKSoundVar) 
			{
				GetMe()->StopSpecialSounds(tSoundSource::type_skill);
			}
			else if(strVariable == szUISoundVar)
			{
				GetMe()->StopSpecialSounds(tSoundSource::type_UI);
			}
		}
	}
}

VOID CSoundSystemFMod::StopSpecialSounds(tSoundSource::Type type)
{
	//更新声音源音量
	std::list< SOUND_SOURCE >::iterator it, itNext;
	for(it = m_listSoundSource.begin(); it!=m_listSoundSource.end(); it=itNext)
	{
		itNext = it; itNext++;
		SOUND_SOURCE& theSnd = *it;
	
		//各种音效的开关
		if(type == theSnd.pSoundSource->GetType())
		{
			theSnd.pSoundSource->Stop();
			delete theSnd.pSoundSource;
			m_listSoundSource.erase(it);
			continue;
		}
	}

}

INT CSoundSystemFMod::Lua_PlaySound(LuaPlus::LuaState* state)
{
	LuaPlus::LuaStack args(state);
	if(!args[2].IsInteger()) 
	{
		TDThrow("LUA:CSoundSystemFMod::Lua_PlaySound[2] param parameter error");
		return 0;
	}
	if(!args[3].IsBoolean()) 
	{
		TDThrow("LUA:CSoundSystemFMod::Lua_PlaySound[3] param parameter error");
		return 0;
	}

	int idSource = args[2].GetInteger();
	bool bLoop =  args[3].GetBoolean();

	//生成Source
	tSoundBuffer* pSndBuffer = CSoundSystemFMod::GetMe()->Buffer_Create(idSource);
	if(!pSndBuffer) 
	{
		state->PushInteger(-1);
		return 1;
	}

	//设置资源
	tSoundSource* pSound = Source_Create(CSoundSourceFMod::type_background, FALSE, !bLoop);
	if(pSndBuffer)
	{
		pSound->SetBuffer(pSndBuffer);
		pSound->SetLooping(bLoop);
		pSound->Play();
		state->PushInteger(pSound->GetID());
		return 1;
	}

	state->PushInteger(-1);
	return 1;
}

INT CSoundSystemFMod::Lua_StopSound(LuaPlus::LuaState* state)
{
	LuaPlus::LuaStack args(state);
	if(!args[2].IsInteger()) 
	{
		TDThrow("LUA:CSoundSystemFMod::Lua_StopSound[2] param parameter error");
		return 0;
	}

	int idSound = args[2].GetInteger();

	Source_Destroy(idSound);
	return 0;
}



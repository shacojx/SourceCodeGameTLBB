/****************************************\
*										*
* 			声音/音效管理器				*
*										*
\****************************************/

#pragma once
#include "TDSound.h"
#include <set>
#include "GMSoundSource.h"

class CSoundBufferFMod;
struct FSOUND_STREAM;
struct _DBC_SOUND_INFO;
class CSoundSourceFMod;
struct EVENT;
namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

//声音系统
class CSoundSystemFMod : public tSoundSystem
{
public:
	//---------------------------------------------------------
	//声音资源

	//创建一个声音资源
	virtual	tSoundBuffer*		Buffer_Create(INT nID);
	virtual	tSoundBuffer*		Buffer_Create(LPCTSTR szFileName);

	//---------------------------------------------------------
	//播放源

	//创建/删除播放源
	// 3D模式			- 音量会随着位置变化
	// 自动删除模式		- 只播放一遍,然后自动释放
	virtual	tSoundSource*		Source_Create(tSoundSource::Type type, BOOL b3DMode, BOOL bAutoDestroy);
	virtual VOID				Source_Destroy(tSoundSource* pSource);
	virtual VOID				Source_Destroy(INT id);

	//---------------------------------------------------------
	//收听者

	//设置听者位置
	virtual VOID				Listener_SetPos(const fVector3& fvPos) { m_vListenerPos = fvPos; }
	virtual const fVector3&		Listener_GetPos(VOID) const { return m_vListenerPos; }

	//---------------------------------------------------------
	//对外提供的声音播放接口
	virtual FUNC_PLAYSOUND		GetPlaySoundFunc(VOID) { return _PlaySoundFunc; }
	virtual FUNC_STOPSOUND		GetStopSoundFunc(VOID) { return _StopSoundFunc; }

	//---------------------------------------------------------
	//变量截获函数，用于控制各种音效的开关
	static VOID	WINAPI			_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData);

	//---------------------------------------------------------
	//外接播放技能声音接口
	static int WINAPI 		_PlaySoundFunc(const char* szSoundFile, float* fvPos, bool bLoop) 
	{
		return GetMe()->PlaySoundFunc(szSoundFile, fvPos, bLoop);
	}
	static void WINAPI 		_StopSoundFunc(int nHandle) 
	{ 
		GetMe()->StopSoundFunc(nHandle); 
	}

	//UI播放声音接口
	static void _PlayUISoundFunc(INT nID) { GetMe()->PlayUISoundFunc(nID); }

	//某种类型声音目前是否允许播放
	BOOL	IsTypeEnabled(tSoundSource::Type type);
	float	GetTypeVolume(tSoundSource::Type type);

	//---------------------------------------------------------
	//外在变量系统中控制声音系统的变量名
	static const CHAR szAllSoundDisable[]; //关闭所有音效的开关
	static const CHAR szBKSoundVar[];	//背景音效
	static const CHAR sz3DSoundVar[];	//环境音效
	static const CHAR szSKSoundVar[];	//技能音效
	static const CHAR szUISoundVar[];	//事件音效

	static const CHAR szBKVolumeVar[];	//背景音量
	static const CHAR sz3DVolumeVar[];	//环境音量
	static const CHAR szSKVolumeVar[];	//技能音量
	static const CHAR szUIVolumeVar[];	//事件音量

	//---------------------------------------------------------
	//用于脚本调用的函数
public:
	//播放一个声音，按照“背景音效”配置
	//Param1[int]	- 声音id
	//Param2[loop]	- 是否循环，不循环的声音播放完毕后自动释放
	//return[int]	- 声音句柄
	INT Lua_PlaySound(LuaPlus::LuaState* state);
	//Param1[int]	- 声音句柄
	INT Lua_StopSound(LuaPlus::LuaState* state);

protected:
	LuaPlus::LuaObject* m_pMetaTable;

	//------------------------
	//核心继承
	//------------------------
public:
	virtual VOID			Initial(VOID*);
	virtual VOID			Release(VOID);
	virtual VOID			Tick(VOID);

protected:
	INT						PlaySoundFunc(LPCTSTR szSoundFile, FLOAT* fvPos, BOOL bLoop);
	VOID					StopSoundFunc(INT nHandle);
	VOID					PlayUISoundFunc(INT nID );
	VOID					MuteAll(BOOL bMute);
	//停止指定类型声音
	VOID					StopSpecialSounds(tSoundSource::Type type);
	
	//声音资源目录
	STRING					m_strSoundPath;
	//主音量 [0.0, 1.0]
	FLOAT					m_fMasterGain;
	//听者位置
	fVector3				m_vListenerPos;
	//全部静音状态
	BOOL					m_bMuteAll;

	//------------------------------------------
	//声音资源
	struct SOUND_BUFFER
	{
		const _DBC_SOUND_INFO*		pDefine;
		//已经加载的资源,如果为空表示尚未加载
		CSoundBufferFMod*			pSoundBuf;
	};
    //所有声音资源列表
	std::list< SOUND_BUFFER >			m_listSoundBuffer;
	//按照ID查找
	std::map< INT, SOUND_BUFFER* >		m_mapSoundID;
	//按照文件名查找
	std::map< STRING, SOUND_BUFFER* >	m_mapSoundFile;

	//------------------------------------------
	//正在播放的声音源
	struct SOUND_SOURCE
	{
		INT					id;				//id
		BOOL				bAutoDestroy;	//自动播放
		CSoundSourceFMod*	pSoundSource;	//声音
	};
	std::list< SOUND_SOURCE >	m_listSoundSource;

protected:
	//加载资源并返回
	tSoundBuffer*		Source_Load(SOUND_BUFFER& sndBuffer);

public:
	CSoundSystemFMod();
	virtual ~CSoundSystemFMod();
	static CSoundSystemFMod* GetMe() { return s_pMe; }

protected:
	static CSoundSystemFMod* s_pMe;
	WX_DECLARE_DYNAMIC(CSoundSystemFMod);
};
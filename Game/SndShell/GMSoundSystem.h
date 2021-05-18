/****************************************\
*										*
* 			声音/音效管理器				*
*										*
*						 by jinchao		*
\****************************************/

#pragma once
#include <set>
#include "fmod374/fmod.h"
#include "fmod374/fmod_errors.h"
#include "SndShell.h"

class CSoundBufferFMod;
struct FSOUND_STREAM;
struct _DBC_SOUND_INFO;
class CSoundSourceFMod;

//声音系统
class CSoundSystemFMod
{
public:
	//---------------------------------------------------------
	//声音资源

	//创建一个声音资源
	virtual	CSoundBufferFMod*		Buffer_Create(INT nID);
	virtual	CSoundBufferFMod*		Buffer_Create(LPCTSTR szFileName);

	//---------------------------------------------------------
	//播放源

	//创建/删除播放源
	// 3D模式			- 音量会随着位置变化
	// 自动删除模式		- 只播放一遍,然后自动释放
	virtual	CSoundSourceFMod*		Source_Create(BOOL b3DMode, BOOL bAutoDestroy);
	virtual VOID				Source_Destroy(CSoundSourceFMod* pSource);

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
	//得到所有可用声音文件名
	BOOL		EnumSoundFile(INT nIndex, STRING& strFileName, int& id);

	//------------------------
	//核心继承
	//------------------------
public:
	virtual VOID			Initial(LPCTSTR szSndPath, LPCTSTR szDBCFile);
	virtual VOID			Release(VOID);
	virtual VOID			Tick(VOID);
	//播放声音接口
	static int WINAPI 		_PlaySoundFunc(const char* szSoundFile, float* fvPos, bool bLoop);
	static void WINAPI 		_StopSoundFunc(int nHandle);
    static void WINAPI 		SetSoundVolume(INT nHandle, FLOAT gain);


protected:
	INT						PlaySoundFunc(LPCTSTR szSoundFile, FLOAT* fvPos, BOOL bLoop);
	VOID					StopSoundFunc(INT nHandle);
	VOID					MuteAll(BOOL bMute);
	
	//声音资源目录
	STRING					m_strSoundPath;
	//声音配置文件
	STRING					m_strSndDBCFile;
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
		BOOL				bAutoDestroy;	//自动播放
		CSoundSourceFMod*	pSoundSource;	//声音
	};
	std::list< SOUND_SOURCE >	m_listSoundSource;

protected:
	//加载资源并返回
	CSoundBufferFMod*		Source_Load(SOUND_BUFFER& sndBuffer);

public:
	CSoundSystemFMod();
	virtual ~CSoundSystemFMod();
	static CSoundSystemFMod* GetMe() { return s_pMe; }

protected:
	static CSoundSystemFMod* s_pMe;
};
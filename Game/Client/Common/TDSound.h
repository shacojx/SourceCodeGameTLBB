/****************************************\
*										*
* 			声音/音效管理器				*
*										*
\****************************************/

#pragma once
#include "TDBasicType.h"
#include "TDNode.h"
#include "TDMath.h"

//-------------------------------------------------
//声音资源接口
class tSoundBuffer
{
public:
	//加载文件
	virtual BOOL		ReadWavBuffer(const STRING& name)	= 0;
	//释放资源
	virtual VOID		Release(VOID)						= 0;
};

//对外提供的声音播放接口,返回声音源HANDLE
typedef int (__stdcall* FUNC_PLAYSOUND)(const char* szSoundFile, float* fvPos, bool bLoop);
//对外提供声音的停止接口
typedef VOID (__stdcall* FUNC_STOPSOUND)(int);

//--------------------------------------------------
//声音播放者者接口
class tSoundSource
{
public:
	//声音逻辑类型
	enum Type
	{
		type_background,		//背景音
		type_skill,				//技能音
		type_circumstance,		//环境音
		type_UI,				//UI音
	};
	//返回id
	virtual INT				GetID(void) const = 0;

	//所使用的声音源
	/*
	|	!!!如果在播放中设置该项，则不会马上生效，
	|需要执行Stop(); Play()后才会生效
	|
	*/
	virtual VOID			SetBuffer(tSoundBuffer *buffer) = 0;
	virtual tSoundBuffer*	GetBuffer(VOID) = 0;

    //是否循环播放
	virtual VOID			SetLooping(BOOL loop) = 0;
	virtual BOOL			GetLooping(VOID) const = 0;

    //播放
	virtual VOID			Play(VOID) = 0;
    //停止播放
	virtual VOID			Stop(VOID) = 0;
    //是否在播放中
	virtual BOOL			IsPlaying(VOID) const = 0;
    //是否已经停止
	virtual BOOL			IsStopped(VOID) const = 0;

	//播放源位置(default(0,0,0))
	virtual VOID			SetPos(const fVector3& pos) = 0;
	virtual const fVector3& GetPos(VOID) = 0;
	//更新音量
	virtual VOID			UpdateVolume(VOID) = 0;
};

//--------------------------------------------------
//声音管理系统
class tSoundSystem : public tNode
{
public:
	//---------------------------------------------------------
	//声音资源

	//创建一个声音资源
	virtual	tSoundBuffer*		Buffer_Create(INT nID)	= 0;

	//---------------------------------------------------------
	//播放源

	//创建/删除播放源
	// 3D模式 - 音量会随着位置变化
	// 自动删除模式		- 只播放一遍,然后自动释放
	virtual	tSoundSource*		Source_Create(tSoundSource::Type nType, BOOL b3DMode, BOOL bAutoDestroy) = 0;
	virtual VOID				Source_Destroy(tSoundSource* pSource)			= 0;

	//---------------------------------------------------------
	//收听者

	//设置听者位置
	virtual VOID				Listener_SetPos(const fVector3& fvPos)	= 0;
	virtual const fVector3&		Listener_GetPos(VOID) const				= 0;

	//---------------------------------------------------------
	//对外提供的声音接口
	virtual FUNC_PLAYSOUND		GetPlaySoundFunc(VOID)	= 0;
	virtual FUNC_STOPSOUND		GetStopSoundFunc(VOID)	= 0;

	WX_DECLARE_DYNAMIC(tSoundSystem);
};

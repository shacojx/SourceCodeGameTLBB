/****************************************\
*										*
* 			声音播放源					*
*										*
\****************************************/

#pragma once

#include "TDSound.h"

class CSoundSourceFMod : public tSoundSource
{
	friend class CSoundSystemFMod;

public:
	//返回id
	virtual INT				GetID(void) const { return _ID; }
 	//所使用的声音源
	/*
	|	!!!如果在播放中设置该项，则不会马上生效，
	|需要执行Stop(); Play()后才会生效
	|
	*/
	virtual VOID			SetBuffer(tSoundBuffer *buffer);
	virtual tSoundBuffer*	GetBuffer(VOID);

    //是否循环播放
	virtual VOID			SetLooping(BOOL loop);
	virtual BOOL			GetLooping(VOID) const { return _Loop; }

    //播放
	virtual VOID			Play(VOID);
    //停止播放
	virtual VOID			Stop(VOID);
    //是否在播放中
	virtual BOOL			IsPlaying(VOID) const;
    //是否已经停止
	virtual BOOL			IsStopped(VOID) const;

	//播放源位置(default(0,0,0))
	virtual VOID			SetPos(const fVector3& pos);
	virtual const fVector3& GetPos(VOID) { return _Pos; }

	//更新音量
	virtual VOID			UpdateVolume(VOID);
	//更新状态
	virtual VOID			UpdateStatus(VOID);

	//声音类型
	virtual Type			GetType(void)const		{return _nType;}

protected:
	CSoundSourceFMod(INT id, Type type, BOOL b3DMode);
	virtual ~CSoundSourceFMod();

	//得到Channel
	INT		GetFModChannel(VOID) const { return _FModChannel; }

	//播放源音量([0, 1], default:1)
	VOID			_SetGain(FLOAT gain);

private:
	enum TSourceState
	{
		source_stoped,			//停止中
		source_playing,			//正在播放
		source_silencing,		//播放完毕
	};
	TSourceState			_State;

	INT						_ID;
	tSoundBuffer			*_Sample;
	INT						_FModChannel;
	
    // States
	BOOL					_b3DMode;	//TRUE- 3D模式,		 FALSE - 2D模式
    BOOL					_Loop;
	DOUBLE					_Alpha;
	fVector3				_Pos;

	// Type
	Type					_nType;

protected:
	//根据距离计算音量
	INT		_ComputeManualRollOff(INT volumeDB, INT dbMin, INT dbMax, DOUBLE alpha, FLOAT sqrdist) const;

};
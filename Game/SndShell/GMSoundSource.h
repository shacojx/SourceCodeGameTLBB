/****************************************\
*										*
* 			声音播放源					*
*										*
*						 by jinchao		*
\****************************************/

#pragma once

class CSoundBufferFMod;
class CSoundSourceFMod
{
public:
 	//所使用的声音源
	/*
	|	!!!如果在播放中设置该项，则不会马上生效，
	|需要执行Stop(); Play()后才会生效
	|
	*/
	virtual VOID				SetBuffer(CSoundBufferFMod *buffer);
	virtual CSoundBufferFMod*	GetBuffer(VOID);

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

	//播放源音量([0, 1], default:1)
	virtual VOID			SetGain(FLOAT gain);
	virtual FLOAT			GetGain(VOID) const { return _Gain; }

	//更新音量
	virtual VOID			UpdateVolume(VOID);
	//更新状态
	virtual VOID			UpdateStatus(VOID);

public:
	CSoundSourceFMod(BOOL b3DMode);
	virtual ~CSoundSourceFMod();

	//得到Channel
	INT		GetFModChannel(VOID) const { return _FModChannel; }

private:
	enum TSourceState
	{
		source_stoped,			//停止中
		source_playing,			//正在播放
		source_silencing,		//播放完毕
	};
	TSourceState			_State;

	CSoundBufferFMod			*_Sample;
	INT						_FModChannel;
	
    // States
	BOOL					_b3DMode;	//TRUE- 3D模式,		 FALSE - 2D模式
    BOOL					_Loop;
	FLOAT					_Gain;
	double					_Alpha;
	fVector3				_Pos;

protected:
	//根据距离计算音量
	INT		_ComputeManualRollOff(INT volumeDB, INT dbMin, INT dbMax, double alpha, FLOAT sqrdist) const;

};
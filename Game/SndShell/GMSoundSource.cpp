#include "StdAfx.h"
#include "GMSoundSource.h"
#include "GMSoundBuffer.h"
#include "GMSoundSystem.h"
#include "fmod374/fmod.h"
#include "fmod374/fmod_errors.h"
#include <assert.h>
#include <math.h>

CSoundSourceFMod::CSoundSourceFMod(BOOL b3DMode) 
{
	_Sample = NULL;
	_State = source_stoped;

	_Loop = TRUE;
	_b3DMode = b3DMode;
	_Gain = 1.0f;
	_Alpha = 0.0;
	_Pos= fVector3(0.0f, 0.0f, 0.0f);

	_FModChannel= -1;
}

CSoundSourceFMod::~CSoundSourceFMod()
{
	Stop();
}

void CSoundSourceFMod::SetBuffer(CSoundBufferFMod *buffer)
{
	assert(buffer);

	_Sample = buffer;
}

CSoundBufferFMod *CSoundSourceFMod::GetBuffer(VOID)
{
	return _Sample;
}

void CSoundSourceFMod::SetLooping( BOOL loop )
{
	if(_Loop!=loop)
	{
		_Loop = loop;
		if(_FModChannel!=-1)
		{
			//首先暂停
			FSOUND_SetPaused(_FModChannel, true);
			FSOUND_SetLoopMode(_FModChannel, _Loop?FSOUND_LOOP_NORMAL:FSOUND_LOOP_OFF);
			FSOUND_SetPaused(_FModChannel, false);
		}
	}
}

VOID CSoundSourceFMod::Play(VOID)
{
	//停止
	Stop();

	//播放
	if(_Sample)
	{
		CSoundBufferFMod* bufFMod= static_cast<CSoundBufferFMod*>(_Sample);

		_State= source_playing;
		//以暂停模式开始播放声音
		if(bufFMod->m_pFModSample)
			_FModChannel= FSOUND_PlaySoundEx(FSOUND_FREE, bufFMod->m_pFModSample, NULL, true);

		//在Channel播放
		if(_FModChannel!=-1)
		{
			FSOUND_SetLoopMode(_FModChannel, _Loop?FSOUND_LOOP_NORMAL:FSOUND_LOOP_OFF);
			//音量
			UpdateVolume();
			//开始播放
			FSOUND_SetPaused(_FModChannel, false);
		}
	}
}

void CSoundSourceFMod::Stop()
{
	_State= source_stoped;

	// Stop the FMod channel
	if(_FModChannel!=-1)
	{
		FSOUND_StopSound(_FModChannel);
		_FModChannel= -1;
	}
}

BOOL CSoundSourceFMod::IsPlaying(VOID) const
{
	return _State == source_playing;
}

BOOL CSoundSourceFMod::IsStopped(VOID) const
{
	return _State == source_silencing || _State == source_stoped;
}

void CSoundSourceFMod::SetGain( FLOAT gain )
{
	TDU_Clamp(gain, 0.00001f, 1.0f);
	_Gain = gain;

	if(!_b3DMode)
	{
		FSOUND_SetVolume(_FModChannel, UINT(_Gain*255));
	}
}

void CSoundSourceFMod::SetPos( const fVector3& pos )
{
	_Pos = pos;

	//3d模式
	if(_b3DMode)
	{
		UpdateVolume();
	}
}

void CSoundSourceFMod::UpdateVolume()
{
	// only if channel active
	if(_FModChannel==-1)
		return;

	//3D模式
	if(_b3DMode)
	{
		//得到与听者距离平方
		FLOAT fDistanceSQR = TDU_GetDistSq(GetPos(), CSoundSystemFMod::GetMe()->Listener_GetPos());

		//计算声音分贝
		INT  nVolumeDB= INT(floor(2000.0 * log10(_Gain))); // convert to 1/100th decibels
		const static INT s_nDBMin= -10000;
		const static INT s_nDBMax= 0;
		TDU_Clamp(nVolumeDB, s_nDBMin, s_nDBMax);

		//根据距离计算音量
		nVolumeDB= _ComputeManualRollOff(nVolumeDB, s_nDBMin, s_nDBMax, _Alpha, fDistanceSQR);
		
		//计算线性值
		double	dAttGain= pow((double)10.0, double(nVolumeDB)/2000.0);
		TDU_Clamp(dAttGain, 0.0f, 1.0f);

		FSOUND_SetVolume(_FModChannel, UINT(dAttGain*255));
	}
}

VOID CSoundSourceFMod::UpdateStatus(VOID)
{
	if(_FModChannel!=-1 && IsPlaying())
	{
		//播放完毕
		if(!FSOUND_IsPlaying(_FModChannel))
		{
			_State= source_silencing;
		}
	}
}

INT CSoundSourceFMod::_ComputeManualRollOff(INT volumeDB, INT dbMin, INT dbMax, double alpha, FLOAT sqrdist) const
{
	FLOAT min = 0.0;
	FLOAT max = 40.0f;
	
	if (sqrdist < min * min) 
	{
		// no attenuation
		return volumeDB;
	}
	else if (sqrdist > max * max)
	{
		// full attenuation
		return dbMin;
	}
	else
	{
		double dist = (double) sqrt(sqrdist);
		
		// linearly descending volume on a dB scale
		double db1 = dbMin * (dist - min) / (max - min);
		
		if (alpha == 0.0) {
			volumeDB += (INT) db1;
			
		} else if (alpha > 0.0) {
			double amp2 = 0.0001 + 0.9999 * (max - dist) / (max - min); // linear amp between 0.00001 and 1.0
			double db2 = 2000.0 * log10(amp2); // convert to 1/100th decibels
			volumeDB += (INT) ((1.0 - alpha) * db1 + alpha * db2);
			
		} else if (alpha < 0.0) {
			double amp3 = min / dist; // linear amplitude is 1/distance
			double db3 = 2000.0 * log10(amp3); // convert to 1/100th decibels
			volumeDB += (INT) ((1.0 + alpha) * db1 - alpha * db3);
		}
		
		TDU_Clamp(volumeDB, dbMin, dbMax);
		
		return volumeDB;
	}
}
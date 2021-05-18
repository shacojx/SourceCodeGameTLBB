/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#include "OgreMeshRotationAffector.h"
#include "OgreParticleSystem.h"
#include "OgreParticle.h"
#include "OgreStringConverter.h"
#include "OgreExt/OgreMeshParticleVisualData.h"

namespace Ogre {

    // Instantiate statics
	MeshRotationAffector::CmdYawRotationSpeedRangeStart	MeshRotationAffector::msYawRotationSpeedRangeStartCmd;
	MeshRotationAffector::CmdYawRotationSpeedRangeEnd		MeshRotationAffector::msYawRotationSpeedRangeEndCmd;
	MeshRotationAffector::CmdYawRotationRangeStart			MeshRotationAffector::msYawRotationRangeStartCmd;
	MeshRotationAffector::CmdYawRotationRangeEnd			MeshRotationAffector::msYawRotationRangeEndCmd;

	MeshRotationAffector::CmdPitchRotationSpeedRangeStart	MeshRotationAffector::msPitchRotationSpeedRangeStartCmd;
	MeshRotationAffector::CmdPitchRotationSpeedRangeEnd		MeshRotationAffector::msPitchRotationSpeedRangeEndCmd;
	MeshRotationAffector::CmdPitchRotationRangeStart			MeshRotationAffector::msPitchRotationRangeStartCmd;
	MeshRotationAffector::CmdPitchRotationRangeEnd			MeshRotationAffector::msPitchRotationRangeEndCmd;

	MeshRotationAffector::CmdRollRotationSpeedRangeStart	MeshRotationAffector::msRollRotationSpeedRangeStartCmd;
	MeshRotationAffector::CmdRollRotationSpeedRangeEnd		MeshRotationAffector::msRollRotationSpeedRangeEndCmd;
	MeshRotationAffector::CmdRollRotationRangeStart			MeshRotationAffector::msRollRotationRangeStartCmd;
	MeshRotationAffector::CmdRollRotationRangeEnd			MeshRotationAffector::msRollRotationRangeEndCmd;

    //-----------------------------------------------------------------------
    MeshRotationAffector::MeshRotationAffector(ParticleSystem* psys)
		:ParticleAffector(psys),
		mYawRotationSpeedRangeStart(0),
		mYawRotationSpeedRangeEnd(0),
		mYawRotationRangeStart(0),
		mYawRotationRangeEnd(0),
		mPitchRotationSpeedRangeStart(0),
		mPitchRotationSpeedRangeEnd(0),
		mPitchRotationRangeStart(0),
		mPitchRotationRangeEnd(0),
		mRollRotationSpeedRangeStart(0),
		mRollRotationSpeedRangeEnd(0),
		mRollRotationRangeStart(0),
		mRollRotationRangeEnd(0)
    {
        mType = "MeshRotator";
		
		// Init parameters
		if (createParamDictionary("MeshRotationAffector"))
		{
			ParamDictionary* dict = getParamDictionary();

			dict->addParameter(ParameterDef("yaw_rotation_speed_range_start", 
				"The start of a range of yaw rotation speed to be assigned to emitted particles.", PT_REAL),
				&msYawRotationSpeedRangeStartCmd);

			dict->addParameter(ParameterDef("yaw_rotation_speed_range_end", 
				"The end of a range of yaw rotation speed to be assigned to emitted particles.", PT_REAL),
				&msYawRotationSpeedRangeEndCmd);

			dict->addParameter(ParameterDef("yaw_rotation_range_start", 
				"The start of a range of yaw rotation angles to be assigned to emitted particles.", PT_REAL),
				&msYawRotationRangeStartCmd);

			dict->addParameter(ParameterDef("yaw_rotation_range_end", 
				"The end of a range of yaw rotation angles to be assigned to emitted particles.", PT_REAL),
				&msYawRotationRangeEndCmd);

			dict->addParameter(ParameterDef("pitch_rotation_speed_range_start", 
				"The start of a range of pitch rotation speed to be assigned to emitted particles.", PT_REAL),
				&msPitchRotationSpeedRangeStartCmd);

			dict->addParameter(ParameterDef("pitch_rotation_speed_range_end", 
				"The end of a range of pitch rotation speed to be assigned to emitted particles.", PT_REAL),
				&msPitchRotationSpeedRangeEndCmd);

			dict->addParameter(ParameterDef("pitch_rotation_range_start", 
				"The start of a range of pitch rotation angles to be assigned to emitted particles.", PT_REAL),
				&msPitchRotationRangeStartCmd);

			dict->addParameter(ParameterDef("pitch_rotation_range_end", 
				"The end of a range of pitch rotation angles to be assigned to emitted particles.", PT_REAL),
				&msPitchRotationRangeEndCmd);

			dict->addParameter(ParameterDef("roll_rotation_speed_range_start", 
				"The start of a range of roll rotation speed to be assigned to emitted particles.", PT_REAL),
				&msRollRotationSpeedRangeStartCmd);

			dict->addParameter(ParameterDef("roll_rotation_speed_range_end", 
				"The end of a range of roll rotation speed to be assigned to emitted particles.", PT_REAL),
				&msRollRotationSpeedRangeEndCmd);

			dict->addParameter(ParameterDef("roll_rotation_range_start", 
				"The start of a range of roll rotation angles to be assigned to emitted particles.", PT_REAL),
				&msRollRotationRangeStartCmd);

			dict->addParameter(ParameterDef("roll_rotation_range_end", 
				"The end of a range of roll rotation angles to be assigned to emitted particles.", PT_REAL),
				&msRollRotationRangeEndCmd);
		}
    }
	//-----------------------------------------------------------------------
	void MeshRotationAffector::_initParticle(Particle* pParticle)
	{
		MeshParticleVisualData *data = static_cast<MeshParticleVisualData *>(pParticle->getVisualData());

		data->mYawRotationSpeed =
			mYawRotationSpeedRangeStart + 
			(Math::UnitRandom() * 
			(mYawRotationSpeedRangeEnd - mYawRotationSpeedRangeStart));

		data->mPitchRotationSpeed =
			mPitchRotationSpeedRangeStart + 
			(Math::UnitRandom() * 
			(mPitchRotationSpeedRangeEnd - mPitchRotationSpeedRangeStart));

		data->mRollRotationSpeed =
			mRollRotationSpeedRangeStart + 
			(Math::UnitRandom() * 
			(mRollRotationSpeedRangeEnd - mRollRotationSpeedRangeStart));

		data->mYawRotation = mYawRotationRangeStart + Math::UnitRandom() * (mYawRotationRangeEnd - mYawRotationRangeStart);

		data->mPitchRotation = mPitchRotationRangeStart + Math::UnitRandom() * (mPitchRotationRangeEnd - mPitchRotationRangeStart);

		data->mRollRotation = mRollRotationRangeStart + Math::UnitRandom() * (mRollRotationRangeEnd - mRollRotationRangeStart);
	}
	//-----------------------------------------------------------------------
	void MeshRotationAffector::_affectParticles(ParticleSystem* pSystem, Real timeElapsed)
	{
		ParticleIterator pi = pSystem->_getIterator();
		Particle *p;
		Real ds;

		// Rotation adjustments by time
		ds = timeElapsed;

		while (!pi.end())
		{
			p = pi.getNext();

			MeshParticleVisualData *data = static_cast<MeshParticleVisualData *>(p->getVisualData());

			data->mYawRotation = data->mYawRotation + ds * data->mYawRotationSpeed;

			data->mPitchRotation = data->mPitchRotation + ds * data->mPitchRotationSpeed;

			data->mRollRotation = data->mRollRotation + ds * data->mRollRotationSpeed;

			if ( data->mYawRotation != Radian(0) || data->mPitchRotation != Radian(0) || 
				data->mRollRotation != Radian(0) )
				pSystem->_notifyParticleRotated();
		}

	}
	//-----------------------------------------------------------------------
	const Radian& MeshRotationAffector::getRotationSpeedRangeStart(RotationType type) const
	{
		switch (type)
		{
		case YAW:
			return mYawRotationSpeedRangeStart;

		case PITCH:
			return mPitchRotationSpeedRangeStart;

		case ROLL:
			return mRollRotationSpeedRangeStart;
		}

		// make complier happy
		return mYawRotationSpeedRangeStart;
	}
	//-----------------------------------------------------------------------
	const Radian& MeshRotationAffector::getRotationSpeedRangeEnd(RotationType type) const
	{
		switch (type)
		{
		case YAW:
			return mYawRotationSpeedRangeEnd;

		case PITCH:
			return mPitchRotationSpeedRangeEnd;

		case ROLL:
			return mRollRotationSpeedRangeEnd;
		}

		// make complier happy
		return mYawRotationSpeedRangeEnd;
	}
	//-----------------------------------------------------------------------
	void MeshRotationAffector::setRotationSpeedRangeStart(const Radian& val, RotationType type)
	{
		switch (type)
		{
		case YAW:
			mYawRotationSpeedRangeStart = val;
			break;

		case PITCH:
			mPitchRotationSpeedRangeStart = val;
			break;

		case ROLL:
			mRollRotationSpeedRangeStart = val;
			break;
		}
	}
	//-----------------------------------------------------------------------
	void MeshRotationAffector::setRotationSpeedRangeEnd(const Radian& val, RotationType type)
	{
		switch (type)
		{
		case YAW:
			mYawRotationSpeedRangeEnd = val;
			break;

		case PITCH:
			mPitchRotationSpeedRangeEnd = val;
			break;

		case ROLL:
			mRollRotationSpeedRangeEnd = val;
			break;
		}
	}
	//-----------------------------------------------------------------------
	const Radian& MeshRotationAffector::getRotationRangeStart(RotationType type) const
	{
		switch (type)
		{
		case YAW:
			return mYawRotationRangeStart;
		case PITCH:
			return mPitchRotationRangeStart;
		case ROLL:
			return mRollRotationRangeStart;
		}

		// make complier happy
		return mYawRotationRangeStart;
	}
	//-----------------------------------------------------------------------
	const Radian& MeshRotationAffector::getRotationRangeEnd(RotationType type) const
	{
		switch (type)
		{
		case YAW:
			return mYawRotationRangeEnd;
		case PITCH:
			return mPitchRotationRangeEnd;
		case ROLL:
			return mRollRotationRangeEnd;
		}

		// make complier happy
		return mYawRotationRangeEnd;
	}
	//-----------------------------------------------------------------------
	void MeshRotationAffector::setRotationRangeStart(const Radian& val, RotationType type)
	{
		switch (type)
		{
		case YAW:
			mYawRotationRangeStart = val;
			break;

		case PITCH:
			mPitchRotationRangeStart = val;
			break;

		case ROLL:
			mRollRotationRangeStart = val;
			break;
		}
	}
	//-----------------------------------------------------------------------
	void MeshRotationAffector::setRotationRangeEnd(const Radian& val, RotationType type)
	{
		switch (type)
		{
		case YAW:
			mYawRotationRangeEnd = val;
			break;

		case PITCH:
			mPitchRotationRangeEnd = val;
			break;

		case ROLL:
			mRollRotationRangeEnd = val;
			break;
		}
	}
	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdYawRotationSpeedRangeEnd::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationSpeedRangeEnd(YAW) );
	}
	void MeshRotationAffector::CmdYawRotationSpeedRangeEnd::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationSpeedRangeEnd(StringConverter::parseAngle(val), YAW);
	}
	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdYawRotationSpeedRangeStart::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationSpeedRangeStart(YAW) );
	}
	void MeshRotationAffector::CmdYawRotationSpeedRangeStart::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationSpeedRangeStart(StringConverter::parseAngle(val), YAW);
	}

	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdYawRotationRangeEnd::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationRangeEnd(YAW) );
	}
	void MeshRotationAffector::CmdYawRotationRangeEnd::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationRangeEnd(StringConverter::parseAngle(val), YAW);
	}
	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdYawRotationRangeStart::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationRangeStart(YAW) );
	}
	void MeshRotationAffector::CmdYawRotationRangeStart::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationRangeStart(StringConverter::parseAngle(val), YAW);
	}

	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdPitchRotationSpeedRangeEnd::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationSpeedRangeEnd(PITCH) );
	}
	void MeshRotationAffector::CmdPitchRotationSpeedRangeEnd::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationSpeedRangeEnd(StringConverter::parseAngle(val), PITCH);
	}
	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdPitchRotationSpeedRangeStart::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationSpeedRangeStart(PITCH) );
	}
	void MeshRotationAffector::CmdPitchRotationSpeedRangeStart::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationSpeedRangeStart(StringConverter::parseAngle(val), PITCH);
	}

	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdPitchRotationRangeEnd::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationRangeEnd(PITCH) );
	}
	void MeshRotationAffector::CmdPitchRotationRangeEnd::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationRangeEnd(StringConverter::parseAngle(val), PITCH);
	}
	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdPitchRotationRangeStart::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationRangeStart(PITCH) );
	}
	void MeshRotationAffector::CmdPitchRotationRangeStart::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationRangeStart(StringConverter::parseAngle(val), PITCH);
	}

	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdRollRotationSpeedRangeEnd::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationSpeedRangeEnd(ROLL) );
	}
	void MeshRotationAffector::CmdRollRotationSpeedRangeEnd::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationSpeedRangeEnd(StringConverter::parseAngle(val), ROLL);
	}
	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdRollRotationSpeedRangeStart::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationSpeedRangeStart(ROLL) );
	}
	void MeshRotationAffector::CmdRollRotationSpeedRangeStart::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationSpeedRangeStart(StringConverter::parseAngle(val), ROLL);
	}

	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdRollRotationRangeEnd::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationRangeEnd(ROLL) );
	}
	void MeshRotationAffector::CmdRollRotationRangeEnd::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationRangeEnd(StringConverter::parseAngle(val), ROLL);
	}
	//-----------------------------------------------------------------------
	String MeshRotationAffector::CmdRollRotationRangeStart::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshRotationAffector*>(target)->getRotationRangeStart(ROLL) );
	}
	void MeshRotationAffector::CmdRollRotationRangeStart::doSet(void* target, const String& val)
	{
		static_cast<MeshRotationAffector*>(target)->setRotationRangeStart(StringConverter::parseAngle(val), ROLL);
	}
}


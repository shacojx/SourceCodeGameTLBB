/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"
#include "OgreGpuProgram.h"
#include "OgreHighLevelGpuProgram.h"
#include "OgreGpuProgramManager.h"
#include "OgreVector3.h"
#include "OgreVector4.h"
#include "OgreAutoParamDataSource.h"
#include "OgreLight.h"
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreRenderSystemCapabilities.h"
#include "OgreStringConverter.h"
#include "OgreLogManager.h"

namespace Ogre
{
    //-----------------------------------------------------------------------------
    GpuProgram::CmdType GpuProgram::msTypeCmd;
    GpuProgram::CmdSyntax GpuProgram::msSyntaxCmd;
    GpuProgram::CmdSkeletal GpuProgram::msSkeletalCmd;


    GpuProgramParameters::AutoConstantDefinition GpuProgramParameters::AutoConstantDictionary[] = {
        AutoConstantDefinition(ACT_WORLD_MATRIX,                  "world_matrix",                16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_WORLD_MATRIX,          "inverse_world_matrix",        16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_TRANSPOSE_WORLD_MATRIX,             "transpose_world_matrix",            16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_TRANSPOSE_WORLD_MATRIX, "inverse_transpose_world_matrix", 16, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_WORLD_MATRIX_ARRAY_3x4,        "world_matrix_array_3x4",      12, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_WORLD_MATRIX_ARRAY,            "world_matrix_array",          16, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_VIEW_MATRIX,                   "view_matrix",                 16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_VIEW_MATRIX,           "inverse_view_matrix",         16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_TRANSPOSE_VIEW_MATRIX,              "transpose_view_matrix",             16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_TRANSPOSE_VIEW_MATRIX,       "inverse_transpose_view_matrix",     16, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_PROJECTION_MATRIX,             "projection_matrix",           16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_PROJECTION_MATRIX,          "inverse_projection_matrix",         16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_TRANSPOSE_PROJECTION_MATRIX,        "transpose_projection_matrix",       16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_TRANSPOSE_PROJECTION_MATRIX, "inverse_transpose_projection_matrix", 16, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_VIEWPROJ_MATRIX,               "viewproj_matrix",             16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_VIEWPROJ_MATRIX,       "inverse_viewproj_matrix",     16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_TRANSPOSE_VIEWPROJ_MATRIX,          "transpose_viewproj_matrix",         16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_TRANSPOSE_VIEWPROJ_MATRIX,   "inverse_transpose_viewproj_matrix", 16, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_WORLDVIEW_MATRIX,              "worldview_matrix",            16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_WORLDVIEW_MATRIX,      "inverse_worldview_matrix",    16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_TRANSPOSE_WORLDVIEW_MATRIX,         "transpose_worldview_matrix",        16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_TRANSPOSE_WORLDVIEW_MATRIX, "inverse_transpose_worldview_matrix", 16, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_WORLDVIEWPROJ_MATRIX,          "worldviewproj_matrix",        16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_WORLDVIEWPROJ_MATRIX,       "inverse_worldviewproj_matrix",      16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_TRANSPOSE_WORLDVIEWPROJ_MATRIX,     "transpose_worldviewproj_matrix",    16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_TRANSPOSE_WORLDVIEWPROJ_MATRIX, "inverse_transpose_worldviewproj_matrix", 16, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_RENDER_TARGET_FLIPPING,          "render_target_flipping",         1, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_FOG_COLOUR,                    "fog_colour",                   4, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_FOG_PARAMS,                    "fog_params",                   4, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_SURFACE_AMBIENT_COLOUR,          "surface_ambient_colour",           4, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_SURFACE_DIFFUSE_COLOUR,          "surface_diffuse_colour",           4, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_SURFACE_SPECULAR_COLOUR,         "surface_specular_colour",          4, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_SURFACE_EMISSIVE_COLOUR,         "surface_emissive_colour",          4, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_SURFACE_SHININESS,               "surface_shininess",                1, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_DERIVED_AMBIENT_LIGHT_COLOUR,    "derived_ambient_light_colour",     4, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_DERIVED_LIGHT_DIFFUSE_COLOUR,    "derived_light_diffuse_colour",     4, ET_REAL, ACDT_INT),
        AutoConstantDefinition(ACT_DERIVED_LIGHT_SPECULAR_COLOUR,   "derived_light_specular_colour",    4, ET_REAL, ACDT_INT),

        AutoConstantDefinition(ACT_DERIVED_SCENE_COLOUR,            "derived_scene_colour",             4, ET_REAL, ACDT_NONE),

        AutoConstantDefinition(ACT_AMBIENT_LIGHT_COLOUR,          "ambient_light_colour",         4, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_LIGHT_DIFFUSE_COLOUR,          "light_diffuse_colour",         4, ET_REAL, ACDT_INT),
        AutoConstantDefinition(ACT_LIGHT_SPECULAR_COLOUR,         "light_specular_colour",        4, ET_REAL, ACDT_INT),
        AutoConstantDefinition(ACT_LIGHT_ATTENUATION,             "light_attenuation",            4, ET_REAL, ACDT_INT),
        AutoConstantDefinition(ACT_LIGHT_POSITION,                "light_position",               4, ET_REAL, ACDT_INT),
        AutoConstantDefinition(ACT_LIGHT_POSITION_OBJECT_SPACE,   "light_position_object_space",  4, ET_REAL, ACDT_INT),
		AutoConstantDefinition(ACT_LIGHT_POSITION_VIEW_SPACE,          "light_position_view_space",    4, ET_REAL, ACDT_INT),
        AutoConstantDefinition(ACT_LIGHT_DIRECTION,               "light_direction",              4, ET_REAL, ACDT_INT),
        AutoConstantDefinition(ACT_LIGHT_DIRECTION_OBJECT_SPACE,  "light_direction_object_space", 4, ET_REAL, ACDT_INT),
		AutoConstantDefinition(ACT_LIGHT_DIRECTION_VIEW_SPACE,         "light_direction_view_space",   4, ET_REAL, ACDT_INT),
		AutoConstantDefinition(ACT_LIGHT_DISTANCE_OBJECT_SPACE,   "light_distance_object_space",  1, ET_REAL, ACDT_INT),
        AutoConstantDefinition(ACT_LIGHT_POWER_SCALE,   		  "light_power",  1, ET_REAL, ACDT_INT),
        AutoConstantDefinition(ACT_SHADOW_EXTRUSION_DISTANCE,     "shadow_extrusion_distance",    1, ET_REAL, ACDT_INT),
        AutoConstantDefinition(ACT_CAMERA_POSITION,               "camera_position",              3, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_CAMERA_POSITION_OBJECT_SPACE,  "camera_position_object_space", 3, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_TEXTURE_VIEWPROJ_MATRIX,       "texture_viewproj_matrix",     16, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_CUSTOM,                        "custom",                       4, ET_REAL, ACDT_INT),  // *** needs to be tested
        AutoConstantDefinition(ACT_TIME,                               "time",                               1, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_TIME_0_X,                      "time_0_x",                     4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_COSTIME_0_X,                   "costime_0_x",                  4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_SINTIME_0_X,                   "sintime_0_x",                  4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_TANTIME_0_X,                   "tantime_0_x",                  4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_TIME_0_X_PACKED,               "time_0_x_packed",              4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_TIME_0_1,                      "time_0_1",                     4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_COSTIME_0_1,                   "costime_0_1",                  4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_SINTIME_0_1,                   "sintime_0_1",                  4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_TANTIME_0_1,                   "tantime_0_1",                  4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_TIME_0_1_PACKED,               "time_0_1_packed",              4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_TIME_0_2PI,                    "time_0_2pi",                   4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_COSTIME_0_2PI,                 "costime_0_2pi",                4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_SINTIME_0_2PI,                 "sintime_0_2pi",                4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_TANTIME_0_2PI,                 "tantime_0_2pi",                4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_TIME_0_2PI_PACKED,             "time_0_2pi_packed",            4, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_FRAME_TIME,                    "frame_time",                   1, ET_REAL, ACDT_REAL),
        AutoConstantDefinition(ACT_FPS,                           "fps",                          1, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_VIEWPORT_WIDTH,                "viewport_width",               1, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_VIEWPORT_HEIGHT,               "viewport_height",              1, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_VIEWPORT_WIDTH,        "inverse_viewport_width",       1, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_INVERSE_VIEWPORT_HEIGHT,       "inverse_viewport_height",      1, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_VIEWPORT_SIZE,                 "viewport_size",                4, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_VIEW_DIRECTION,                "view_direction",               3, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_VIEW_SIDE_VECTOR,              "view_side_vector",             3, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_VIEW_UP_VECTOR,                "view_up_vector",               3, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_FOV,                           "fov",                          1, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_NEAR_CLIP_DISTANCE,            "near_clip_distance",           1, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_FAR_CLIP_DISTANCE,             "far_clip_distance",            1, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_PASS_NUMBER,                        "pass_number",                        1, ET_REAL, ACDT_NONE),
        AutoConstantDefinition(ACT_PASS_ITERATION_NUMBER,              "pass_iteration_number",              1, ET_REAL, ACDT_NONE),
		AutoConstantDefinition(ACT_ANIMATION_PARAMETRIC,               "animation_parametric",               4, ET_REAL, ACDT_INT),
    };

    

    //-----------------------------------------------------------------------------
    GpuProgram::GpuProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
        const String& group, bool isManual, ManualResourceLoader* loader) 
        :Resource(creator, name, handle, group, isManual, loader),
        mType(GPT_VERTEX_PROGRAM), mLoadFromFile(true), mSkeletalAnimation(false),
        mPassSurfaceAndLightStates(false), mCompileError(false)
    {
    }
    //-----------------------------------------------------------------------------
    void GpuProgram::setType(GpuProgramType t)
    {
        mType = t;
    }
    //-----------------------------------------------------------------------------
    void GpuProgram::setSyntaxCode(const String& syntax)
    {
        mSyntaxCode = syntax;
    }
    //-----------------------------------------------------------------------------
    void GpuProgram::setSourceFile(const String& filename)
    {
        mFilename = filename;
        mSource.clear();
        mLoadFromFile = true;
		mCompileError = false;
    }
    //-----------------------------------------------------------------------------
    void GpuProgram::setSource(const String& source)
    {
        mSource = source;
        mFilename.clear();
        mLoadFromFile = false;
		mCompileError = false;
    }

    //-----------------------------------------------------------------------------
    void GpuProgram::loadImpl(void)
    {
        if (mLoadFromFile)
        {
            // find & load source code
            DataStreamPtr stream = 
                ResourceGroupManager::getSingleton().openResource(
					mFilename, mGroup, true, this);
            mSource = stream->getAsString();
        }

        // Call polymorphic load
		try 
		{
			loadFromSource();
		}
		catch (const Exception&)
		{
			// will already have been logged
			StringUtil::StrStreamType str;
			str << "Gpu program " << mName << " encountered an error "
				<< "during loading and is thus not supported.";
			LogManager::getSingleton().logMessage(str.str());

			mCompileError = true;
		}

    }
    //-----------------------------------------------------------------------------
    bool GpuProgram::isSupported(void) const
    {
        // If skeletal animation is being done, we need support for UBYTE4
        if ((isSkeletalAnimationIncluded() && 
            !Root::getSingleton().getRenderSystem()->getCapabilities()
                ->hasCapability(RSC_VERTEX_FORMAT_UBYTE4)) ||  
			mCompileError)
        {
            return false;
        }
        return GpuProgramManager::getSingleton().isSyntaxSupported(mSyntaxCode);
    }
    //-----------------------------------------------------------------------------
    GpuProgramParametersSharedPtr GpuProgram::createParameters(void)
    {
        // Default implementation simply returns standard parameters.
        GpuProgramParametersSharedPtr ret = 
            GpuProgramManager::getSingleton().createParameters();
        // Copy in default parameters if present
        if (!mDefaultParams.isNull())
            ret->copyConstantsFrom(*(mDefaultParams.get()));
        
        return ret;
    }
    //-----------------------------------------------------------------------------
    GpuProgramParametersSharedPtr GpuProgram::getDefaultParameters(void)
    {
        if (mDefaultParams.isNull())
        {
            mDefaultParams = createParameters();
        }
        return mDefaultParams;
    }
    //-----------------------------------------------------------------------------
    void GpuProgram::setupBaseParamDictionary(void)
    {
        ParamDictionary* dict = getParamDictionary();

        dict->addParameter(
            ParameterDef("type", "'vertex_program' or 'fragment_program'",
                PT_STRING), &msTypeCmd);
        dict->addParameter(
            ParameterDef("syntax", "Syntax code, e.g. vs_1_1", PT_STRING), &msSyntaxCmd);
        dict->addParameter(
            ParameterDef("includes_skeletal_animation", 
            "Whether this vertex program includes skeletal animation", PT_BOOL), 
            &msSkeletalCmd);
    }

    //-----------------------------------------------------------------------
    const String& GpuProgram::getLanguage(void) const
    {
        static const String language = "asm";

        return language;
    }



    //-----------------------------------------------------------------------------
    //      GpuProgramParameters Methods
    //-----------------------------------------------------------------------------
    GpuProgramParameters::GpuProgramParameters()
        : mTransposeMatrices(false), mAutoAddParamName(false), mActivePassIterationEntry(0)
    {
    }
    //-----------------------------------------------------------------------------

    GpuProgramParameters::GpuProgramParameters(const GpuProgramParameters& oth)
    {
        *this = oth;
    }

    //-----------------------------------------------------------------------------
    GpuProgramParameters& GpuProgramParameters::operator=(const GpuProgramParameters& oth)
    {
        // nfz: perform copy of all containers
        // let compiler perform shallow copies of structures 
        // AutoConstantEntry, RealConstantEntry, IntConstantEntry
        mRealConstants = oth.mRealConstants;
        mIntConstants  = oth.mIntConstants;
        mAutoConstants = oth.mAutoConstants;
        mParamNameMap  = oth.mParamNameMap;

        mTransposeMatrices = oth.mTransposeMatrices;
        mAutoAddParamName  = oth.mAutoAddParamName;
        mConstantDefinitions = oth.mConstantDefinitions;

		return *this;
    }

    void GpuProgramParameters::setConstant(size_t index, const Vector4& vec)
    {
        setConstant(index, vec.val, 1);
    }
	//-----------------------------------------------------------------------------
	void GpuProgramParameters::setConstant(size_t index, Real val)
	{
		setConstant(index, Vector4(val, 0.0f, 0.0f, 0.0f));
	}
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::setConstant(size_t index, const Vector3& vec)
    {
        setConstant(index, Vector4(vec.x, vec.y, vec.z, 1.0f));
    }
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::setConstant(size_t index, const Matrix4& m)
    {
        // set as 4x 4-element floats
        if (mTransposeMatrices)
        {
            Matrix4 t = m.transpose();
            GpuProgramParameters::setConstant(index, t[0], 4);
        }
        else
        {
            GpuProgramParameters::setConstant(index, m[0], 4);
        }

    }
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::setConstant(size_t index, const Matrix4* pMatrix, 
        size_t numEntries)
    {
        if (mTransposeMatrices)
        {
            for (size_t i = 0; i < numEntries; ++i)
            {
                Matrix4 t = pMatrix[i].transpose();
                GpuProgramParameters::setConstant(index, t[0], 4);
                index += 4;
            }
        }
        else
        {
            GpuProgramParameters::setConstant(index, pMatrix[0][0], 4 * numEntries);
        }

    }
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::setConstant(size_t index, const ColourValue& colour)
    {
        setConstant(index, colour.val, 1);
    }
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::setConstant(size_t index, const float *val, size_t count)
    {
        // Expand if required
        if (mRealConstants.size() < index + count)
            mRealConstants.resize(index + count);

        // Copy in chunks of 4
        while (count--)
        {
            RealConstantEntry* e = &(mRealConstants[index++]);
            e->isSet = true;
            memcpy(e->val, val, sizeof(float) * 4);
            val += 4;
        }

    }
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::setConstant(size_t index, const double *val, size_t count)
    {
        // Expand if required
        if (mRealConstants.size() < index + count)
            mRealConstants.resize(index + count);

        // Copy, casting to float
        while (count--)
        {
            RealConstantEntry* e = &(mRealConstants[index++]);
            e->isSet = true;
            e->val[0] = static_cast<float>(val[0]);
            e->val[1] = static_cast<float>(val[1]);
            e->val[2] = static_cast<float>(val[2]);
            e->val[3] = static_cast<float>(val[3]);
            val += 4;
        }

    }
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::setConstant(size_t index, const int *val, size_t count)
    {
        // Expand if required
        if (mIntConstants.size() < index + count)
            mIntConstants.resize(index + count);

        // Copy in chunks of 4
        while (count--)
        {
            IntConstantEntry* e = &(mIntConstants[index++]);
            e->isSet = true;
            memcpy(e->val, val, sizeof(int) * 4);
            val += 4;
        }

    }
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::setAutoConstant(size_t index, AutoConstantType acType, size_t extraInfo)
    {
		// update existing index if it exists
		bool found = false;
		for (AutoConstantList::iterator i = mAutoConstants.begin(); 
			i != mAutoConstants.end(); ++i)
		{
			if (i->index == index)
			{
				i->paramType = acType;
				i->data = extraInfo;
				found = true;
				break;
			}
		}
		if (!found)
			mAutoConstants.push_back(AutoConstantEntry(acType, index, extraInfo));
    }
	//-----------------------------------------------------------------------------
	void GpuProgramParameters::clearAutoConstant(size_t index)
	{
		// update existing index if it exists
		bool found = false;
		for (AutoConstantList::iterator i = mAutoConstants.begin(); 
			i != mAutoConstants.end(); ++i)
		{
			if (i->index == index)
			{
				mAutoConstants.erase(i);
				break;
			}
		}

		// Also remove from constant definition
		for (ConstantDefinitionContainer::iterator i = mConstantDefinitions.begin();
			i != mConstantDefinitions.end(); ++i)
		{
			if (i->entryIndex == index)
			{
				mConstantDefinitions.erase(i);
				break;
			}
		}
	}
	//-----------------------------------------------------------------------------
	void GpuProgramParameters::clearNamedAutoConstant(const String& name)
	{
		clearAutoConstant(getParamIndex(name));
	}
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::clearAutoConstants(void)
    {
        mAutoConstants.clear();
    }
    //-----------------------------------------------------------------------------
    GpuProgramParameters::AutoConstantIterator GpuProgramParameters::getAutoConstantIterator(void) const
    {
        return AutoConstantIterator(mAutoConstants.begin(), mAutoConstants.end());
    }
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::setAutoConstantReal(size_t index, AutoConstantType acType, Real rData)
    {
		// update existing index if it exists
		bool found = false;
		for (AutoConstantList::iterator i = mAutoConstants.begin(); 
			i != mAutoConstants.end(); ++i)
		{
			if (i->index == index)
			{
				i->paramType = acType;
				i->fData = rData;
				found = true;
				break;
			}
		}
		if (!found)
	        mAutoConstants.push_back(AutoConstantEntry(acType, index, rData));
    }
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    void GpuProgramParameters::_updateAutoParamsNoLights(const AutoParamDataSource& source)
    {
        if (!hasAutoConstants()) return; // abort early if no autos
        Vector3 vec3;
        Vector4 vec4;
        size_t index;
        size_t numMatrices;
        const Matrix4* pMatrix;
        size_t m;

		mActivePassIterationEntry = 0;

        AutoConstantList::const_iterator i, iend;
        iend = mAutoConstants.end();
        for (i = mAutoConstants.begin(); i != iend; ++i)
        {
            switch(i->paramType)
            {
            case ACT_WORLD_MATRIX:
                setConstant(i->index, source.getWorldMatrix());
                break;
            case ACT_INVERSE_WORLD_MATRIX:
                setConstant(i->index, source.getInverseWorldMatrix());
                break;
            case ACT_TRANSPOSE_WORLD_MATRIX:
               setConstant(i->index, source.getTransposeWorldMatrix());
               break;
            case ACT_INVERSE_TRANSPOSE_WORLD_MATRIX:
               setConstant(i->index, source.getInverseTransposeWorldMatrix());
               break;

            case ACT_WORLD_MATRIX_ARRAY_3x4:
                // Loop over matrices
                pMatrix = source.getWorldMatrixArray();
                numMatrices = source.getWorldMatrixCount();
                index = i->index;
                for (m = 0; m < numMatrices; ++m)
                {
                    GpuProgramParameters::setConstant(index, (*pMatrix)[0], 3);
                    index += 3;
                    ++pMatrix;
                }
                
                break;
            case ACT_WORLD_MATRIX_ARRAY:
                setConstant(i->index, source.getWorldMatrixArray(), 
                    source.getWorldMatrixCount());
                break;
            case ACT_VIEW_MATRIX:
                setConstant(i->index, source.getViewMatrix());
                break;
            case ACT_INVERSE_VIEW_MATRIX:
               setConstant(i->index, source.getInverseViewMatrix());
               break;
            case ACT_TRANSPOSE_VIEW_MATRIX:
               setConstant(i->index, source.getTransposeViewMatrix());
               break;
            case ACT_INVERSE_TRANSPOSE_VIEW_MATRIX:
               setConstant(i->index, source.getInverseTransposeViewMatrix());
               break;

            case ACT_PROJECTION_MATRIX:
                setConstant(i->index, source.getProjectionMatrix());
                break;
            case ACT_INVERSE_PROJECTION_MATRIX:
               setConstant(i->index, source.getInverseProjectionMatrix());
               break;
            case ACT_TRANSPOSE_PROJECTION_MATRIX:
               setConstant(i->index, source.getTransposeProjectionMatrix());
               break;
            case ACT_INVERSE_TRANSPOSE_PROJECTION_MATRIX:
               setConstant(i->index, source.getInverseTransposeProjectionMatrix());
               break;

            case ACT_VIEWPROJ_MATRIX:
                setConstant(i->index, source.getViewProjectionMatrix());
                break;
            case ACT_INVERSE_VIEWPROJ_MATRIX:
               setConstant(i->index, source.getInverseViewProjMatrix());
               break;
            case ACT_TRANSPOSE_VIEWPROJ_MATRIX:
               setConstant(i->index, source.getTransposeViewProjMatrix());
               break;
            case ACT_INVERSE_TRANSPOSE_VIEWPROJ_MATRIX:
               setConstant(i->index, source.getInverseTransposeViewProjMatrix());
               break;

            case ACT_WORLDVIEW_MATRIX:
                setConstant(i->index, source.getWorldViewMatrix());
                break;
            case ACT_INVERSE_WORLDVIEW_MATRIX:
                setConstant(i->index, source.getInverseWorldViewMatrix());
                break;
            case ACT_TRANSPOSE_WORLDVIEW_MATRIX:
               setConstant(i->index, source.getTransposeWorldViewMatrix());
               break;
            case ACT_INVERSE_TRANSPOSE_WORLDVIEW_MATRIX:
               setConstant(i->index, source.getInverseTransposeWorldViewMatrix());
               break;

            case ACT_WORLDVIEWPROJ_MATRIX:
                setConstant(i->index, source.getWorldViewProjMatrix());
                break;
            case ACT_INVERSE_WORLDVIEWPROJ_MATRIX:
               setConstant(i->index, source.getInverseWorldViewProjMatrix());
               break;
            case ACT_TRANSPOSE_WORLDVIEWPROJ_MATRIX:
               setConstant(i->index, source.getTransposeWorldViewProjMatrix());
               break;
            case ACT_INVERSE_TRANSPOSE_WORLDVIEWPROJ_MATRIX:
               setConstant(i->index, source.getInverseTransposeWorldViewProjMatrix());
               break;

            case ACT_RENDER_TARGET_FLIPPING:
               setConstant(i->index, source.getCurrentRenderTarget()->requiresTextureFlipping() ? -1.f : +1.f);
               break;

            case ACT_SURFACE_AMBIENT_COLOUR:
                setConstant(i->index, source.getSurfaceAmbientColour());
                break;
            case ACT_SURFACE_DIFFUSE_COLOUR:
                setConstant(i->index, source.getSurfaceDiffuseColour());
                break;
            case ACT_SURFACE_SPECULAR_COLOUR:
                setConstant(i->index, source.getSurfaceSpecularColour());
                break;
            case ACT_SURFACE_EMISSIVE_COLOUR:
                setConstant(i->index, source.getSurfaceEmissiveColour());
                break;
            case ACT_SURFACE_SHININESS:
                setConstant(i->index, source.getSurfaceShininess());
                break;

            // NB ambient light still here because it's not related to a specific light
            case ACT_AMBIENT_LIGHT_COLOUR: 
                setConstant(i->index, source.getAmbientLightColour());
                break;
            case ACT_DERIVED_AMBIENT_LIGHT_COLOUR:
                setConstant(i->index, source.getDerivedAmbientLightColour());
                break;
            case ACT_DERIVED_SCENE_COLOUR:
                setConstant(i->index, source.getDerivedSceneColour());
                break;

            case ACT_FOG_COLOUR:
                setConstant(i->index, source.getFogColour());
                break;
            case ACT_FOG_PARAMS:
                setConstant(i->index, source.getFogParams());
                break;

            case ACT_CAMERA_POSITION:
                setConstant(i->index, source.getCameraPosition());
                break;
            case ACT_CAMERA_POSITION_OBJECT_SPACE:
                setConstant(i->index, source.getCameraPositionObjectSpace());
                break;

            case ACT_TIME:
               setConstant(i->index, Vector4(source.getTime() * i->fData, 0.f, 0.f, 0.f));
               break;
           case ACT_TIME_0_X:
               setConstant(i->index, Vector4(source.getTime_0_X(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_COSTIME_0_X:
               setConstant(i->index, Vector4(source.getCosTime_0_X(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_SINTIME_0_X:
               setConstant(i->index, Vector4(source.getSinTime_0_X(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_TANTIME_0_X:
               setConstant(i->index, Vector4(source.getTanTime_0_X(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_TIME_0_X_PACKED:
               setConstant(i->index, source.getTime_0_X_packed(i->fData));
               break;
            case ACT_TIME_0_1:
               setConstant(i->index, Vector4(source.getTime_0_1(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_COSTIME_0_1:
               setConstant(i->index, Vector4(source.getCosTime_0_1(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_SINTIME_0_1:
               setConstant(i->index, Vector4(source.getSinTime_0_1(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_TANTIME_0_1:
               setConstant(i->index, Vector4(source.getTanTime_0_1(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_TIME_0_1_PACKED:
               setConstant(i->index, source.getTime_0_1_packed(i->fData));
               break;
            case ACT_TIME_0_2PI:
               setConstant(i->index, Vector4(source.getTime_0_2Pi(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_COSTIME_0_2PI:
               setConstant(i->index, Vector4(source.getCosTime_0_2Pi(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_SINTIME_0_2PI:
               setConstant(i->index, Vector4(source.getSinTime_0_2Pi(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_TANTIME_0_2PI:
               setConstant(i->index, Vector4(source.getTanTime_0_2Pi(i->fData), 0.f, 0.f, 0.f));
               break;
            case ACT_TIME_0_2PI_PACKED:
               setConstant(i->index, source.getTime_0_2Pi_packed(i->fData));
               break;
            case ACT_FRAME_TIME:
               setConstant(i->index, source.getFrameTime() * i->fData);
               break;
            case ACT_FPS:
               setConstant(i->index, source.getFPS());
               break;
            case ACT_VIEWPORT_WIDTH:
               setConstant(i->index, source.getViewportWidth());
               break;
            case ACT_VIEWPORT_HEIGHT:
               setConstant(i->index, source.getViewportHeight());
               break;
            case ACT_INVERSE_VIEWPORT_WIDTH:
               setConstant(i->index, source.getInverseViewportWidth());
               break;
            case ACT_INVERSE_VIEWPORT_HEIGHT:
               setConstant(i->index, source.getInverseViewportHeight());
               break;
            case ACT_VIEWPORT_SIZE:
               setConstant(i->index, Vector4(
                   source.getViewportWidth(),
                   source.getViewportHeight(),
                   source.getInverseViewportWidth(),
                   source.getInverseViewportHeight()));
               break;
            case ACT_VIEW_DIRECTION:
               setConstant(i->index, source.getViewDirection());
               break;
            case ACT_VIEW_SIDE_VECTOR:
               setConstant(i->index, source.getViewSideVector());
               break;
            case ACT_VIEW_UP_VECTOR:
               setConstant(i->index, source.getViewUpVector());
               break;
            case ACT_FOV:
               setConstant(i->index, source.getFOV());
               break;
            case ACT_NEAR_CLIP_DISTANCE:
               setConstant(i->index, source.getNearClipDistance());
               break;
            case ACT_FAR_CLIP_DISTANCE:
               setConstant(i->index, source.getFarClipDistance());
               break;
            case ACT_TEXTURE_VIEWPROJ_MATRIX:
                setConstant(i->index, source.getTextureViewProjMatrix());
                break;
            case ACT_PASS_NUMBER:
                setConstant(i->index, (float)source.getPassNumber());
                break;
            case ACT_PASS_ITERATION_NUMBER:
                setConstant(i->index, 0.0f);
                mActivePassIterationEntry = getRealConstantEntry(i->index);
                mActivePassIterationEntryIndex = i->index;
                break;
            case ACT_CUSTOM:
			case ACT_ANIMATION_PARAMETRIC:
                source.getCurrentRenderable()->_updateCustomGpuParameter(*i, this);
                break;
            default:
                break;
            }
        }
    }
    //-----------------------------------------------------------------------------
    void GpuProgramParameters::_updateAutoParamsLightsOnly(const AutoParamDataSource& source)
    {
        if (!hasAutoConstants()) return; // abort early if no autos
        Vector3 vec3;
        Vector4 vec4;

        AutoConstantList::const_iterator i, iend;
        iend = mAutoConstants.end();
        for (i = mAutoConstants.begin(); i != iend; ++i)
        {
            switch(i->paramType)
            {
            case ACT_DERIVED_LIGHT_DIFFUSE_COLOUR:
                setConstant(i->index,
                    source.getLight(i->data).getDiffuseColour() * source.getSurfaceDiffuseColour());
                break;
            case ACT_DERIVED_LIGHT_SPECULAR_COLOUR:
                setConstant(i->index,
                    source.getLight(i->data).getSpecularColour() * source.getSurfaceSpecularColour());
                break;
            case ACT_LIGHT_DIFFUSE_COLOUR:
                setConstant(i->index, source.getLight(i->data).getDiffuseColour());
                break;
            case ACT_LIGHT_SPECULAR_COLOUR:
                setConstant(i->index, source.getLight(i->data).getSpecularColour());
                break;
            case ACT_LIGHT_POSITION:
                // Get as 4D vector, works for directional lights too
                setConstant(i->index, 
                    source.getLight(i->data).getAs4DVector());
                break;
            case ACT_LIGHT_DIRECTION:
                vec3 = source.getLight(i->data).getDerivedDirection();
                // Set as 4D vector for compatibility
                setConstant(i->index, Vector4(vec3.x, vec3.y, vec3.z, 1.0f));
                break;
            case ACT_LIGHT_POSITION_OBJECT_SPACE:
                setConstant(i->index, 
                    source.getInverseWorldMatrix().transformAffine(source.getLight(i->data).getAs4DVector()));
                break;
            case ACT_LIGHT_DIRECTION_OBJECT_SPACE:
				// We need the inverse transpose of the inverse world matrix
				// == transpose of the world matrix
				vec3 = source.getWorldMatrix().transpose() * 
					source.getLight(i->data).getDerivedDirection();
                vec3.normalise();
                // Set as 4D vector for compatibility
                setConstant(i->index, Vector4(vec3.x, vec3.y, vec3.z, 1.0f));
                break;
			case ACT_LIGHT_POSITION_VIEW_SPACE:
                setConstant(i->index, 
                    source.getViewMatrix() * source.getLight(i->data).getAs4DVector());
                break;
            case ACT_LIGHT_DIRECTION_VIEW_SPACE:
				vec3 = source.getInverseTransposeViewMatrix() *
					source.getLight(i->data).getDerivedDirection();
                vec3.normalise();
                // Set as 4D vector for compatibility
                setConstant(i->index, Vector4(vec3.x, vec3.y, vec3.z, 1.0f));
                break;
            case ACT_LIGHT_DISTANCE_OBJECT_SPACE:
                vec3 = source.getInverseWorldMatrix().transformAffine(source.getLight(i->data).getDerivedPosition());
                setConstant(i->index, vec3.length());
                break;
            case ACT_SHADOW_EXTRUSION_DISTANCE:
                setConstant(i->index, source.getShadowExtrusionDistance());
                break;
            case ACT_LIGHT_POWER_SCALE:
				setConstant(i->index, source.getLight(i->data).getPowerScale());
				break;
            case ACT_LIGHT_ATTENUATION:
            {
                // range, const, linear, quad
                const Light& l = source.getLight(i->data);
                vec4.x = l.getAttenuationRange();
                vec4.y = l.getAttenuationConstant();
                vec4.z = l.getAttenuationLinear();
                vec4.w = l.getAttenuationQuadric();
                setConstant(i->index, vec4);
                break;
            }
            default:
                // do nothing
                break;
            }
        }
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::_mapParameterNameToIndex(const String& name, const size_t index)
    {
        mParamNameMap[name] = index;
    }
    //---------------------------------------------------------------------------
    size_t GpuProgramParameters::getParamIndex(const String& name)
    {
        ParamNameMap::const_iterator i = mParamNameMap.find(name);
        if (i == mParamNameMap.end())
        {
            // name not found in map, should it be added to the map?
            if(mAutoAddParamName)
            {
                // determine index
                // don't know which Constants list the name is for
                // so pick the largest index - a waste
                size_t index = (mRealConstants.size() > mIntConstants.size()) ?
                    mRealConstants.size() : mIntConstants.size();

                _mapParameterNameToIndex(name, index);
                return index;
            }
            else
            {
                OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Cannot find a parameter named " + name,
                    "GpuProgramParameters::getParamIndex");
            }
        }
        return i->second;
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstant(const String& name, Real val)
    {
        setConstant(getParamIndex(name), val);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstant(const String& name, int val)
    {
        setConstant(getParamIndex(name), val);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstant(const String& name, const Vector4& vec)
    {
        setConstant(getParamIndex(name), vec);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstant(const String& name, const Vector3& vec)
    {
        setConstant(getParamIndex(name), vec);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstant(const String& name, const Matrix4& m)
    {
        setConstant(getParamIndex(name), m);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstant(const String& name, const Matrix4* m, 
        size_t numEntries)
    {
        setConstant(getParamIndex(name), m, numEntries);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstant(const String& name, const float *val, size_t count)
    {
        setConstant(getParamIndex(name), val, count);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstant(const String& name, const double *val, size_t count)
    {
        setConstant(getParamIndex(name), val, count);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstant(const String& name, const ColourValue& colour)
    {
        setConstant(getParamIndex(name), colour);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstant(const String& name, const int *val, size_t count)
    {
        setConstant(getParamIndex(name), val, count);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedAutoConstant(const String& name, AutoConstantType acType, size_t extraInfo)
    {
        setAutoConstant(getParamIndex(name), acType, extraInfo);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setConstantFromTime(size_t index, Real factor)
    {
        setAutoConstantReal(index, ACT_TIME, factor);
    }
    //---------------------------------------------------------------------------
    void GpuProgramParameters::setNamedConstantFromTime(const String& name, Real factor)
    {
        setConstantFromTime(getParamIndex(name), factor);
    }
    //---------------------------------------------------------------------------
    GpuProgramParameters::RealConstantIterator GpuProgramParameters::getRealConstantIterator(void) const
    {
        return RealConstantIterator(mRealConstants.begin(), mRealConstants.end());
    }
    //---------------------------------------------------------------------------
    GpuProgramParameters::IntConstantIterator GpuProgramParameters::getIntConstantIterator(void) const
    {
        return IntConstantIterator(mIntConstants.begin(), mIntConstants.end());
    }

    //---------------------------------------------------------------------------
    GpuProgramParameters::RealConstantEntry* GpuProgramParameters::getRealConstantEntry(const size_t index)
    {
        if (index < mRealConstants.size())
        {
            return &(mRealConstants[index]);
        }
        else
        {
            return NULL;
        }
    }

    //---------------------------------------------------------------------------
    GpuProgramParameters::IntConstantEntry* GpuProgramParameters::getIntConstantEntry(const size_t index)
    {
        if (index < mIntConstants.size())
        {
            return &(mIntConstants[index]);
        }
        else
        {
            return NULL;
        }
    }

    //---------------------------------------------------------------------------
    GpuProgramParameters::AutoConstantEntry* GpuProgramParameters::getAutoConstantEntry(const size_t index)
    {
        if (index < mAutoConstants.size())
        {
            return &(mAutoConstants[index]);
        }
        else
        {
            return NULL;
        }
    }

    //---------------------------------------------------------------------------
    GpuProgramParameters::RealConstantEntry* GpuProgramParameters::getNamedRealConstantEntry(const String& name)
    {
        // check if name is found
        ParamNameMap::const_iterator i = mParamNameMap.find(name);

        if (i == mParamNameMap.end())
        {
            // no valid name found
            return NULL;
        }
        else
        {
            // name found: return the entry
            return getRealConstantEntry(i->second);
        }

    }

    //---------------------------------------------------------------------------
    GpuProgramParameters::IntConstantEntry* GpuProgramParameters::getNamedIntConstantEntry(const String& name)
    {
        // check if name is found
        ParamNameMap::const_iterator i = mParamNameMap.find(name);

        if (i == mParamNameMap.end())
        {
            // no valid name found
            return NULL;
        }
        else
        {
            // name found: return the entry
            return getIntConstantEntry(i->second);
        }

    }

    //---------------------------------------------------------------------------
        void GpuProgramParameters::copyConstantsFrom(const GpuProgramParameters& source)
    {
        // Iterate over fixed parameters
        RealConstantIterator ri = source.getRealConstantIterator();
        ushort i = 0;
        while(ri.hasMoreElements())
        {
            RealConstantEntry re = ri.getNext();
            if (re.isSet)
            {
                setConstant(i, re.val, 1);
            }
            ++i;

        }
        IntConstantIterator ii = source.getIntConstantIterator();
        i = 0;
        while (ii.hasMoreElements())
        {
            IntConstantEntry ie = ii.getNext();
            if (ie.isSet)
            {
                setConstant(i, ie.val, 1);
            }
            ++i;
        }

        // Iterate over auto parameters
        // Clear existing auto constants
        clearAutoConstants();
        AutoConstantIterator ai = source.getAutoConstantIterator();
        while (ai.hasMoreElements())
        {
            AutoConstantEntry ae = ai.getNext();
            setAutoConstant(ae.index, ae.paramType, ae.data);
        }

        // need to copy Parameter names from the source
        mParamNameMap = source.mParamNameMap;
        // copy constant definitions
        mConstantDefinitions = source.mConstantDefinitions;

        
    }

    //-----------------------------------------------------------------------
    size_t GpuProgramParameters::addConstantDefinition(const String& name, 
		const size_t index, const size_t elementCount, 
		const ElementType elementType)
    {
        size_t idx;

        // check if definition already exists
        const ConstantDefinition* foundDef = 
			findMatchingConstantDefinition(name, index, elementType);

        // add the definition if it doesn't exist
        if(!foundDef)
        {
            ConstantDefinition def;
            def.elementType = elementType;
            def.entryIndex = index;
            def.elementCount = elementCount;
            def.name = name;

            mConstantDefinitions.push_back( def );
            idx = mConstantDefinitions.size() - 1;
        }
        else
        {
            // calc the index of the found constant definition
            idx = foundDef - &(*mConstantDefinitions.begin());
            // update element count if it was 0 before
            if (foundDef->elementCount == 0)
                mConstantDefinitions[idx].elementCount = elementCount;
        }

        return idx;
    }

    //-----------------------------------------------------------------------
    const GpuProgramParameters::ConstantDefinition* 
	GpuProgramParameters::getConstantDefinition(const String& name) const
    {
        // find a constant definition that matches name by iterating through 
		// the constant definition array
        ConstantDefinitionContainer::const_iterator currentConstDef 
			= mConstantDefinitions.begin();
        ConstantDefinitionContainer::const_iterator endConstDef 
			= mConstantDefinitions.end();
        while (currentConstDef != endConstDef)
        {
            if (name == (*currentConstDef).name)
                break;
            ++currentConstDef;
        }

        if (currentConstDef != endConstDef)
            return &(*currentConstDef);
        else
            return 0;
    }

    //-----------------------------------------------------------------------
    const GpuProgramParameters::ConstantDefinition* 
		GpuProgramParameters::getConstantDefinition(const size_t idx) const
    {
        if (idx < mConstantDefinitions.size())
            return &mConstantDefinitions[idx];
        else
            return 0;
    }

    //-----------------------------------------------------------------------
    const GpuProgramParameters::ConstantDefinition* 
	GpuProgramParameters::findMatchingConstantDefinition(const String& name, 
            const size_t entryIndex, const ElementType elementType) const
    {
        ConstantDefinitionContainer::const_iterator currentConstDef 
			= mConstantDefinitions.begin();
        ConstantDefinitionContainer::const_iterator endConstDef 
			= mConstantDefinitions.end();

        while (currentConstDef != endConstDef)
        {
            if ((name == (*currentConstDef).name)
                && (entryIndex == (*currentConstDef).entryIndex)
                && (elementType == (*currentConstDef).elementType))
                break;

            ++currentConstDef;
        }

        if (currentConstDef != endConstDef)
            return &(*currentConstDef);
        else
            return 0;

    }

    //-----------------------------------------------------------------------
    void GpuProgramParameters::setConstantDefinitionAutoState(
		const size_t index, const bool isAuto, const size_t autoIndex )
    {
        if (index < mConstantDefinitions.size())
        {
            ConstantDefinition* cDef = &mConstantDefinitions[index];
            cDef->isAuto = isAuto;
            // if auto then check to make sure enough storage is allocated and 
			// update element count
            if (isAuto)
            {
                // get the auto constant entry associated with this constant definition
                const GpuProgramParameters::AutoConstantEntry* autoEntry = 
					getAutoConstantEntry(autoIndex);

                if (autoEntry)
                {
                    // get auto constant defintion
                    const AutoConstantDefinition* autoCDef = 
						getAutoConstantDefinition(autoEntry->paramType);

                    // if auto constant exists then its definition will override 
					// constant definition attributes
                    if (autoCDef)
                    {
                        cDef->autoIndex = autoIndex;
                        cDef->elementType = autoCDef->elementType;

                        if (cDef->elementCount < autoCDef->elementCount)
                        {
                            cDef->elementCount = autoCDef->elementCount;
                            // check constant entry list for proper size
                            const size_t endPos = cDef->entryIndex + 
								(cDef->elementCount - 1) / 4 + 1;
                            switch(cDef->elementType)
                            {
                            case ET_INT:
                                if (mIntConstants.size() < endPos)
                                    mIntConstants.resize(endPos);
                                break;

                            case ET_REAL:
                                if (mRealConstants.size() < endPos)
                                    mRealConstants.resize(endPos);
                                break;
                            } // end switch
                        }
                    }
                }
            }
        }
    }

    //-----------------------------------------------------------------------
    const GpuProgramParameters::AutoConstantDefinition* 
	GpuProgramParameters::getAutoConstantDefinition(const String& name)
    {
        // find a constant definition that matches name by iterating through the 
		// constant definition array
        bool nameFound = false;
        size_t i = 0;
        const size_t numDefs = getNumAutoConstantDefinitions();
        while (!nameFound && (i < numDefs))
        {
            if (name == AutoConstantDictionary[i].name) 
                nameFound = true;
            else
                ++i;
        }

        if (nameFound)
            return &AutoConstantDictionary[i];
        else
            return 0;
    }

    //-----------------------------------------------------------------------
    const GpuProgramParameters::AutoConstantDefinition* 
	GpuProgramParameters::getAutoConstantDefinition(const  size_t idx) 
    {

        if (idx < getNumAutoConstantDefinitions())
        {
            assert(idx == AutoConstantDictionary[idx].acType);
            return &AutoConstantDictionary[idx];
        }
        else
            return 0;
    }
    //-----------------------------------------------------------------------
    size_t GpuProgramParameters::getNumAutoConstantDefinitions(void)
    {
        return sizeof(AutoConstantDictionary)/sizeof(AutoConstantDefinition);
    }

    //-----------------------------------------------------------------------
    void GpuProgramParameters::incPassIterationNumber(void)
    {
        if (mActivePassIterationEntry)
        {
            // only increment first element
            ++mActivePassIterationEntry->val[0];
            mActivePassIterationEntry->isSet = true;
        }
    }

    //-----------------------------------------------------------------------
    GpuProgramParameters::RealConstantEntry* GpuProgramParameters::getPassIterationEntry(void)
    {
        return mActivePassIterationEntry;
    }
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    String GpuProgram::CmdType::doGet(const void* target) const
    {
        const GpuProgram* t = static_cast<const GpuProgram*>(target);
        if (t->getType() == GPT_VERTEX_PROGRAM)
        {
            return "vertex_program";
        }
        else
        {
            return "fragment_program";
        }
    }
    void GpuProgram::CmdType::doSet(void* target, const String& val)
    {
        GpuProgram* t = static_cast<GpuProgram*>(target);
        if (val == "vertex_program")
        {
            t->setType(GPT_VERTEX_PROGRAM);
        }
        else
        {
            t->setType(GPT_FRAGMENT_PROGRAM);
        }
    }
    //-----------------------------------------------------------------------
    String GpuProgram::CmdSyntax::doGet(const void* target) const
    {
        const GpuProgram* t = static_cast<const GpuProgram*>(target);
        return t->getSyntaxCode();
    }
    void GpuProgram::CmdSyntax::doSet(void* target, const String& val)
    {
        GpuProgram* t = static_cast<GpuProgram*>(target);
        t->setSyntaxCode(val);
    }
    //-----------------------------------------------------------------------
    String GpuProgram::CmdSkeletal::doGet(const void* target) const
    {
        const GpuProgram* t = static_cast<const GpuProgram*>(target);
        return StringConverter::toString(t->isSkeletalAnimationIncluded());
    }
    void GpuProgram::CmdSkeletal::doSet(void* target, const String& val)
    {
        GpuProgram* t = static_cast<GpuProgram*>(target);
        t->setSkeletalAnimationIncluded(StringConverter::parseBool(val));
    }
    //-----------------------------------------------------------------------
    GpuProgramPtr& GpuProgramPtr::operator=(const HighLevelGpuProgramPtr& r)
    {
        // Can assign direct
        if (pRep == r.getPointer())
            return *this;
        release();
		// lock & copy other mutex pointer
        OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
        {
		    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
            pRep = r.getPointer();
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
        }
        return *this;
    }

}

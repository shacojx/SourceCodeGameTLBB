/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

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
#ifndef __GpuProgram_H_
#define __GpuProgram_H_

// Precompiler options
#include "OgrePrerequisites.h"
#include "OgreResource.h"
#include "OgreSharedPtr.h"
#include "OgreIteratorWrappers.h"

namespace Ogre {

	/** Enumerates the types of programs which can run on the GPU. */
	enum GpuProgramType
	{
		GPT_VERTEX_PROGRAM,
		GPT_FRAGMENT_PROGRAM
	};

    
    /** Collects together the program parameters used for a GpuProgram.
    @remarks
        Gpu program state includes constant parameters used by the program, and
        bindings to render system state which is propagated into the constants 
        by the engine automatically if requested.
    @par
        GpuProgramParameters objects should be created through the GpuProgramManager and
        may be shared between multiple GpuProgram instances. For this reason they
        are managed using a shared pointer, which will ensure they are automatically
        deleted when no program is using them anymore.
    */
    class _OgreExport GpuProgramParameters
    {
    public:
        /** Defines the types of automatically updated values that may be bound to GpuProgram
        parameters, or used to modify parameters on a per-object basis.
        */
        enum AutoConstantType
        {
            /// The current world matrix
            ACT_WORLD_MATRIX,
            /// The current world matrix, inverted
            ACT_INVERSE_WORLD_MATRIX,
 			/** Provides transpose of world matrix.
 			Equivalent to RenderMonkey's "WorldTranspose".
 			*/
 			ACT_TRANSPOSE_WORLD_MATRIX,
            /// The current world matrix, inverted & transposed
            ACT_INVERSE_TRANSPOSE_WORLD_MATRIX,


            /// The current array of world matrices, as a 3x4 matrix, used for blending
            ACT_WORLD_MATRIX_ARRAY_3x4,
            /// The current array of world matrices, used for blending
            ACT_WORLD_MATRIX_ARRAY,


            /// The current view matrix
            ACT_VIEW_MATRIX,
			/// The current view matrix, inverted
			ACT_INVERSE_VIEW_MATRIX,
			/** Provides transpose of view matrix.
			Equivalent to RenderMonkey's "ViewTranspose".
			*/
			ACT_TRANSPOSE_VIEW_MATRIX,
			/** Provides inverse transpose of view matrix.
			Equivalent to RenderMonkey's "ViewInverseTranspose".
			*/
			ACT_INVERSE_TRANSPOSE_VIEW_MATRIX,


            /// The current projection matrix
            ACT_PROJECTION_MATRIX,
			/** Provides inverse of projection matrix.
			Equivalent to RenderMonkey's "ProjectionInverse".
			*/
			ACT_INVERSE_PROJECTION_MATRIX,
			/** Provides transpose of projection matrix.
			Equivalent to RenderMonkey's "ProjectionTranspose".
			*/
			ACT_TRANSPOSE_PROJECTION_MATRIX,
			/** Provides inverse transpose of projection matrix.
			Equivalent to RenderMonkey's "ProjectionInverseTranspose".
			*/
			ACT_INVERSE_TRANSPOSE_PROJECTION_MATRIX,


            /// The current view & projection matrices concatenated
            ACT_VIEWPROJ_MATRIX,
			/** Provides inverse of concatenated view and projection matrices.
			Equivalent to RenderMonkey's "ViewProjectionInverse".
			*/
			ACT_INVERSE_VIEWPROJ_MATRIX,
			/** Provides transpose of concatenated view and projection matrices.
			Equivalent to RenderMonkey's "ViewProjectionTranspose".
			*/
			ACT_TRANSPOSE_VIEWPROJ_MATRIX,
			/** Provides inverse transpose of concatenated view and projection matrices.
			Equivalent to RenderMonkey's "ViewProjectionInverseTranspose".
			*/
			ACT_INVERSE_TRANSPOSE_VIEWPROJ_MATRIX,


            /// The current world & view matrices concatenated
            ACT_WORLDVIEW_MATRIX,
            /// The current world & view matrices concatenated, then inverted
            ACT_INVERSE_WORLDVIEW_MATRIX,
 			/** Provides transpose of concatenated world and view matrices.
 				Equivalent to RenderMonkey's "WorldViewTranspose".
 			*/
 			ACT_TRANSPOSE_WORLDVIEW_MATRIX,
            /// The current world & view matrices concatenated, then inverted & tranposed
            ACT_INVERSE_TRANSPOSE_WORLDVIEW_MATRIX,
			/// view matrices.


            /// The current world, view & projection matrices concatenated
            ACT_WORLDVIEWPROJ_MATRIX,
			/** Provides inverse of concatenated world, view and projection matrices.
			Equivalent to RenderMonkey's "WorldViewProjectionInverse".
			*/
			ACT_INVERSE_WORLDVIEWPROJ_MATRIX,
			/** Provides transpose of concatenated world, view and projection matrices.
			Equivalent to RenderMonkey's "WorldViewProjectionTranspose".
			*/
			ACT_TRANSPOSE_WORLDVIEWPROJ_MATRIX,
			/** Provides inverse transpose of concatenated world, view and projection
			matrices. Equivalent to RenderMonkey's "WorldViewProjectionInverseTranspose".
 			*/
			ACT_INVERSE_TRANSPOSE_WORLDVIEWPROJ_MATRIX,


            /// render target related values
            /** -1 if requires texture flipping, +1 otherwise. It's useful when you bypassed
            projection matrix transform, still able use this value to adjust transformed y position.
            */
            ACT_RENDER_TARGET_FLIPPING,


            /// Fog colour
            ACT_FOG_COLOUR,
            /// Fog params: density, linear start, linear end, 1/(end-start)
            ACT_FOG_PARAMS,


            /// Surface ambient colour, as set in Pass::setAmbient
            ACT_SURFACE_AMBIENT_COLOUR,
            /// Surface diffuse colour, as set in Pass::setDiffuse
            ACT_SURFACE_DIFFUSE_COLOUR,
            /// Surface specular colour, as set in Pass::setSpecular
            ACT_SURFACE_SPECULAR_COLOUR,
            /// Surface emissive colour, as set in Pass::setSelfIllumination
            ACT_SURFACE_EMISSIVE_COLOUR,
            /// Surface shininess, as set in Pass::setShininess
            ACT_SURFACE_SHININESS,


            /** The derived ambient light colour, with 'r', 'g', 'b' components filled with
                product of surface ambient colour and ambient light colour, respectively,
                and 'a' component filled with surface ambient alpha component.
            */
            ACT_DERIVED_AMBIENT_LIGHT_COLOUR,

            /** The derived light diffuse colour (index determined by setAutoConstant call),
                with 'r', 'g' and 'b' components filled with product of surface diffuse colour
                and light diffuse colour, respectively, and 'a' component filled with surface
                diffuse alpha component.
            */
            ACT_DERIVED_LIGHT_DIFFUSE_COLOUR,
            /** The derived light specular colour (index determined by setAutoConstant call),
                with 'r', 'g' and 'b' components filled with product of surface specular colour
                and light specular colour, respectively, and 'a' component filled with surface
                specular alpha component.
            */
            ACT_DERIVED_LIGHT_SPECULAR_COLOUR,


            /** The derived scene colour, with 'r', 'g' and 'b' components filled with sum
                of derived ambient light colour and surface emissive colour, respectively,
                and 'a' component filled with surface diffuse alpha component.
            */
            ACT_DERIVED_SCENE_COLOUR,


			/// The ambient light colour set in the scene
			ACT_AMBIENT_LIGHT_COLOUR, 

            /// Light diffuse colour (index determined by setAutoConstant call)
            ACT_LIGHT_DIFFUSE_COLOUR,
            /// Light diffuse colour (index determined by setAutoConstant call)
            ACT_LIGHT_SPECULAR_COLOUR,
            /// Light attenuation parameters, Vector4(range, constant, linear, quadric)
            ACT_LIGHT_ATTENUATION,
            /// A light position in world space (index determined by setAutoConstant call)
            ACT_LIGHT_POSITION,
            /// A light position in object space (index determined by setAutoConstant call)
            ACT_LIGHT_POSITION_OBJECT_SPACE,
			/// A light position in view space (index determined by setAutoConstant call)
            ACT_LIGHT_POSITION_VIEW_SPACE,
            /// A light direction in world space (index determined by setAutoConstant call)
            ACT_LIGHT_DIRECTION,
            /// A light direction in object space (index determined by setAutoConstant call)
            ACT_LIGHT_DIRECTION_OBJECT_SPACE,
			/// A light direction in view space (index determined by setAutoConstant call)
			ACT_LIGHT_DIRECTION_VIEW_SPACE,
			/** The distance of the light from the center of the object
				a useful approximation as an alternative to per-vertex distance
				calculations.
			*/
			ACT_LIGHT_DISTANCE_OBJECT_SPACE,
			/** Light power level, a single scalar as set in Light::setPowerScale  (index determined by setAutoConstant call) */
			ACT_LIGHT_POWER_SCALE,
			/** The distance a shadow volume should be extruded when using
			    finite extrusion programs.
			*/
			ACT_SHADOW_EXTRUSION_DISTANCE,
            /// The current camera's position in world space
            ACT_CAMERA_POSITION,
            /// The current camera's position in object space 
            ACT_CAMERA_POSITION_OBJECT_SPACE,
            /// The view/projection matrix of the assigned texture projection frustum
            ACT_TEXTURE_VIEWPROJ_MATRIX,
            /// A custom parameter which will come from the renderable, using 'data' as the identifier
            ACT_CUSTOM,
            /** provides current elapsed time
            */
            ACT_TIME,
			/** Single float value, which repeats itself based on given as
			parameter "cycle time". Equivalent to RenderMonkey's "Time0_X".
			*/
			ACT_TIME_0_X,
			/// Cosine of "Time0_X". Equivalent to RenderMonkey's "CosTime0_X".
			ACT_COSTIME_0_X,
			/// Sine of "Time0_X". Equivalent to RenderMonkey's "SinTime0_X".
			ACT_SINTIME_0_X,
			/// Tangent of "Time0_X". Equivalent to RenderMonkey's "TanTime0_X".
			ACT_TANTIME_0_X,
			/** Vector of "Time0_X", "SinTime0_X", "CosTime0_X", 
			"TanTime0_X". Equivalent to RenderMonkey's "Time0_X_Packed".
			*/
			ACT_TIME_0_X_PACKED,
			/** Single float value, which represents scaled time value [0..1],
			which repeats itself based on given as parameter "cycle time".
			Equivalent to RenderMonkey's "Time0_1".
			*/
			ACT_TIME_0_1,
			/// Cosine of "Time0_1". Equivalent to RenderMonkey's "CosTime0_1".
			ACT_COSTIME_0_1,
			/// Sine of "Time0_1". Equivalent to RenderMonkey's "SinTime0_1".
			ACT_SINTIME_0_1,
			/// Tangent of "Time0_1". Equivalent to RenderMonkey's "TanTime0_1".
			ACT_TANTIME_0_1,
			/** Vector of "Time0_1", "SinTime0_1", "CosTime0_1",
			"TanTime0_1". Equivalent to RenderMonkey's "Time0_1_Packed".
			*/
			ACT_TIME_0_1_PACKED,
			/**	Single float value, which represents scaled time value [0..2*Pi],
			which repeats itself based on given as parameter "cycle time".
			Equivalent to RenderMonkey's "Time0_2PI".
			*/
			ACT_TIME_0_2PI,
			/// Cosine of "Time0_2PI". Equivalent to RenderMonkey's "CosTime0_2PI".
			ACT_COSTIME_0_2PI,
			/// Sine of "Time0_2PI". Equivalent to RenderMonkey's "SinTime0_2PI".
			ACT_SINTIME_0_2PI,
			/// Tangent of "Time0_2PI". Equivalent to RenderMonkey's "TanTime0_2PI".
			ACT_TANTIME_0_2PI,
			/** Vector of "Time0_2PI", "SinTime0_2PI", "CosTime0_2PI",
			"TanTime0_2PI". Equivalent to RenderMonkey's "Time0_2PI_Packed".
			*/
			ACT_TIME_0_2PI_PACKED,
			/// provides the scaled frame time, returned as a floating point value.
            ACT_FRAME_TIME,
			/// provides the calculated frames per second, returned as a floating point value.
			ACT_FPS,
			/// viewport-related values
			/** Current viewport width (in pixels) as floating point value.
			Equivalent to RenderMonkey's "ViewportWidth".
			*/
			ACT_VIEWPORT_WIDTH,
			/** Current viewport height (in pixels) as floating point value.
			Equivalent to RenderMonkey's "ViewportHeight".
			*/
			ACT_VIEWPORT_HEIGHT,
			/** This variable represents 1.0/ViewportWidth. 
			Equivalent to RenderMonkey's "ViewportWidthInverse".
			*/
			ACT_INVERSE_VIEWPORT_WIDTH,
			/** This variable represents 1.0/ViewportHeight.
			Equivalent to RenderMonkey's "ViewportHeightInverse".
			*/
			ACT_INVERSE_VIEWPORT_HEIGHT,
            /** Packed of "ViewportWidth", "ViewportHeight", "ViewportWidthInverse",
            "ViewportHeightInverse".
            */
            ACT_VIEWPORT_SIZE,

			/// view parameters
			/** This variable provides the view direction vector (world space).
			Equivalent to RenderMonkey's "ViewDirection".
			*/
			ACT_VIEW_DIRECTION,
			/** This variable provides the view side vector (world space).
			Equivalent to RenderMonkey's "ViewSideVector".
			*/
			ACT_VIEW_SIDE_VECTOR,
			/** This variable provides the view up vector (world space).
			Equivalent to RenderMonkey's "ViewUpVector".
			*/
			ACT_VIEW_UP_VECTOR,
			/** This variable provides the field of view as a floating point value.
			Equivalent to RenderMonkey's "FOV".
			*/
			ACT_FOV,
			/**	This variable provides the near clip distance as a floating point value.
			Equivalent to RenderMonkey's "NearClipPlane".
			*/
			ACT_NEAR_CLIP_DISTANCE,
			/**	This variable provides the far clip distance as a floating point value.
			Equivalent to RenderMonkey's "FarClipPlane".
			*/
			ACT_FAR_CLIP_DISTANCE,

            /** provides the pass index number within the technique
                of the active materil.
            */
            ACT_PASS_NUMBER,

            /** provides the current iteration number of the pass. The iteration
                number is the number of times the current render operation has
                been drawn for the acitve pass.
            */
            ACT_PASS_ITERATION_NUMBER,


			/** Provides a parametric animation value [0..1], only available
				where the renderable specifically implements it.
			*/
			ACT_ANIMATION_PARAMETRIC

        };

        /** Defines the type of the extra data item used by the auto constant.

        */
        enum ACDataType {
            /// no data is required
            ACDT_NONE,
            /// the auto constant requires data of type int
            ACDT_INT,
            /// the auto constant requires data of type real
            ACDT_REAL
        };

        /** Defines the base element type of the auto constant
        */
        enum ElementType {
            ET_INT,
            ET_REAL
        };

        struct AutoConstantDefinition
        {
            AutoConstantType acType;
            String name;
            size_t elementCount;
			/// The type of the constant in the program
            ElementType elementType;
			/// The type of any extra data
            ACDataType dataType;

			AutoConstantDefinition(AutoConstantType _acType, const String& _name, 
				size_t _elementCount, ElementType _elementType, 
				ACDataType _dataType)
				:acType(_acType), name(_name), elementCount(_elementCount), 
				elementType(_elementType), dataType(_dataType)
			{
				
			}
        };

        /** Structure recording the use of an automatic parameter. */
        class _OgrePrivate AutoConstantEntry
        {
        public:
            /// The type of parameter
            AutoConstantType paramType;
            /// The target constant index
            size_t index;
            /// Additional information to go with the parameter
			union{
				size_t data;
				Real fData;
			};

            AutoConstantEntry(AutoConstantType theType, size_t theIndex, size_t theData)
                : paramType(theType), index(theIndex), data(theData) {}

			AutoConstantEntry(AutoConstantType theType, size_t theIndex, Real theData)
				: paramType(theType), index(theIndex), fData(theData) {}

        };
        /** Real parameter entry; contains both a group of 4 values and 
        an indicator to say if it's been set or not. This allows us to 
        filter out constant entries which have not been set by the renderer
        and may actually be being used internally by the program. */
        struct RealConstantEntry
        {
            float val[4];
            bool isSet;
            RealConstantEntry() : isSet(false)  {}
        };
        /** Int parameter entry; contains both a group of 4 values and 
        an indicator to say if it's been set or not. This allows us to 
        filter out constant entries which have not been set by the renderer
        and may actually be being used internally by the program. */
        struct IntConstantEntry
        {
            int val[4];
            bool isSet;
            IntConstantEntry() : isSet(false) {}
        };

        // nfz
        /** stucture used to keep track of attributes for a constant definition.

        */

        struct ConstantDefinition
        {
            String name;
            size_t entryIndex;
            size_t elementCount;
            size_t arraySize;
            ElementType elementType;
            size_t autoIndex;
            bool   isAllocated;
            bool   isAuto;

            ConstantDefinition()
                : entryIndex(0)
                , elementCount(0)
                , arraySize(1)
                , elementType(ET_INT)
                , autoIndex(0)
                , isAllocated(false)
                , isAuto(false)
            {}

        };

    protected:
        static AutoConstantDefinition AutoConstantDictionary[];
        // Constant lists
        typedef std::vector<RealConstantEntry> RealConstantList;
        typedef std::vector<IntConstantEntry> IntConstantList;
        // Auto parameter storage
        typedef std::vector<AutoConstantEntry> AutoConstantList;
        // parameter dictionary container
        typedef std::vector<ConstantDefinition> ConstantDefinitionContainer;
        /// Packed list of floating-point constants
        RealConstantList mRealConstants;
        /// Packed list of integer constants
        IntConstantList mIntConstants;
        /// List of automatically updated parameters
        AutoConstantList mAutoConstants;
        /// Container of parameter definitions
        ConstantDefinitionContainer mConstantDefinitions;
        /// Mapping from parameter names to NamedConstantEntry - high-level programs are expected to populate this
        typedef std::map<String, size_t> ParamNameMap;
        ParamNameMap mParamNameMap;
        /// Do we need to transpose matrices?
        bool mTransposeMatrices;
		/// flag to indicate if names not found will be automatically added
		bool mAutoAddParamName;
        /// active pass iteration parameter real constant entry;
        RealConstantEntry* mActivePassIterationEntry;
        /// index for active pass iteration parameter real constant entry;
        size_t mActivePassIterationEntryIndex;


    public:
		GpuProgramParameters();
		~GpuProgramParameters() {}

        /// Copy constructor
        GpuProgramParameters(const GpuProgramParameters& oth);
        /// Operator = overload
        GpuProgramParameters& operator=(const GpuProgramParameters& oth);


		/** Sets a 4-element floating-point parameter to the program.
		@param index The constant index at which to place the parameter (each constant is
            a 4D float)
		@param vec The value to set
		*/
		void setConstant(size_t index, const Vector4& vec);
		/** Sets a single floating-point parameter to the program.
		@note This is actually equivalent to calling 
		setConstant(index Vector4(val, 0, 0, 0)) since all constants are 4D.
		@param index The constant index at which to place the parameter (each constant is
		a 4D float)
		@param val The value to set
		*/
		void setConstant(size_t index, Real val);
		/** Sets a 4-element floating-point parameter to the program via Vector3.
		@param index The constant index at which to place the parameter (each constant is
            a 4D float).
            Note that since you're passing a Vector3, the last element of the 4-element
            value will be set to 1 (a homogenous vector)
		@param vec The value to set
		*/
		void setConstant(size_t index, const Vector3& vec);
		/** Sets a Matrix4 parameter to the program.
		@param index The constant index at which to place the parameter (each constant is
            a 4D float).
            NB since a Matrix4 is 16 floats long, this parameter will take up 4 indexes.
		@param m The value to set
		*/
		void setConstant(size_t index, const Matrix4& m);
        /** Sets a list of Matrix4 parameters to the program.
        @param index The constant index at which to start placing the parameter (each constant is
        a 4D float).
        NB since a Matrix4 is 16 floats long, so each entry will take up 4 indexes.
        @param m Pointer to an array of matrices to set
        @param numEntries Number of Matrix4 entries
        */
        void setConstant(size_t index, const Matrix4* m, size_t numEntries);
		/** Sets a multiple value constant floating-point parameter to the program.
		@param index The constant index at which to start placing parameters (each constant is
            a 4D float)
		@param val Pointer to the values to write, must contain 4*count floats
		@param count The number of groups of 4 floats to write
		*/
		void setConstant(size_t index, const float *val, size_t count);
		/** Sets a multiple value constant floating-point parameter to the program.
		@param index The constant index at which to start placing parameters (each constant is
            a 4D float)
		@param val Pointer to the values to write, must contain 4*count floats
		@param count The number of groups of 4 floats to write
		*/
		void setConstant(size_t index, const double *val, size_t count);
		/** Sets a ColourValue parameter to the program.
		@param index The constant index at which to place the parameter (each constant is
            a 4D float)
		@param colour The value to set
		*/
        void setConstant(size_t index, const ColourValue& colour);
		
		/** Sets a multiple value constant integer parameter to the program.
        @remarks
            Different types of GPU programs support different types of constant parameters.
            For example, it's relatively common to find that vertex programs only support
            floating point constants, and that fragment programs only support integer (fixed point)
            parameters. This can vary depending on the program version supported by the
            graphics card being used. You should consult the documentation for the type of
            low level program you are using, or alternatively use the methods
            provided on RenderSystemCapabilities to determine the options.
		@param index The constant index at which to place the parameter (each constant is
            a 4D integer)
		@param val Pointer to the values to write, must contain 4*count ints
		@param count The number of groups of 4 ints to write
		*/
		void setConstant(size_t index, const int *val, size_t count);

        /** Deletes the contents of the Real constants registers. */
        void resetRealConstants(void) { mRealConstants.clear(); }
        /** Deletes the contents of the int constants registers. */
        void resetIntConstants(void) { mIntConstants.clear(); }

        typedef ConstVectorIterator<RealConstantList> RealConstantIterator;
        typedef ConstVectorIterator<IntConstantList> IntConstantIterator;
        /// Gets an iterator over the Real constant parameters
        RealConstantIterator getRealConstantIterator(void) const;
        /// Gets an iterator over the integer constant parameters
        IntConstantIterator getIntConstantIterator(void) const;

		/** Gets a specific Real Constant entry if index is in valid range
			otherwise returns a NULL
		@parem index which entry is to be retrieved
		*/
		RealConstantEntry* getRealConstantEntry(const size_t index);
		/** Gets a specific Int Constant entry if index is in valid range
			otherwise returns a NULL
		@parem index which entry is to be retrieved
		*/
		IntConstantEntry* getIntConstantEntry(const size_t index);
        
		/** Gets a Named Real Constant entry if the name is found otherwise returns a NULL
		@parem name The name of the entry  to be retrieved
		*/
		RealConstantEntry* getNamedRealConstantEntry(const String& name);
		/** Gets a named Int Constant entry if name is found otherwise returns a NULL
		@parem name The name of the entry to be retrieved
		*/
		IntConstantEntry* getNamedIntConstantEntry(const String& name);
        /// Gets the number of Real constants that have been set
        size_t getRealConstantCount(void) const { return mRealConstants.size(); }
        /// Gets the number of int constants that have been set
        size_t getIntConstantCount(void) const { return mIntConstants.size(); }
        /// Returns true if there are any Real constants contained here
        bool hasRealConstantParams(void) const { return !(mRealConstants.empty()); }
        /// Returns true if there are any int constants contained here
        bool hasIntConstantParams(void) const { return !(mIntConstants.empty()); }

        /** Sets up a constant which will automatically be updated by the system.
        @remarks
            Vertex and fragment programs often need parameters which are to do with the
            current render state, or particular values which may very well change over time,
            and often between objects which are being rendered. This feature allows you 
            to set up a certain number of predefined parameter mappings that are kept up to 
            date for you.
        @param index The location in the constant list to place this updated constant every time
            it is changed. Note that because of the nature of the types, we know how big the 
            parameter details will be so you don't need to set that like you do for manual constants.
        @param acType The type of automatic constant to set
        @param extraInfo If the constant type needs more information (like a light index) put it here.
        */
        void setAutoConstant(size_t index, AutoConstantType acType, size_t extraInfo = 0);
		void setAutoConstantReal(size_t index, AutoConstantType acType, Real rData);

		/** Unbind an auto constant so that the constant is manually controlled again. */
		void clearAutoConstant(size_t index);

        /** Sets a named parameter up to track a derivation of the current time.
        @param index The index of the parameter
        @param factor The amount by which to scale the time value
        */  
        void setConstantFromTime(size_t index, Real factor);

        /** Clears all the existing automatic constants. */
        void clearAutoConstants(void);
        typedef ConstVectorIterator<AutoConstantList> AutoConstantIterator;
        /** Gets an iterator over the automatic constant bindings currently in place. */
        AutoConstantIterator getAutoConstantIterator(void) const;
        /// Gets the number of int constants that have been set
        size_t getAutoConstantCount(void) const { return mAutoConstants.size(); }
		/** Gets a specific Auto Constant entry if index is in valid range
			otherwise returns a NULL
		@parem index which entry is to be retrieved
		*/
		AutoConstantEntry* getAutoConstantEntry(const size_t index);
        /** Returns true if this instance has any automatic constants. */
        bool hasAutoConstants(void) const { return !(mAutoConstants.empty()); }
        /** Updates the automatic parameters (except lights) based on the details provided. */
        void _updateAutoParamsNoLights(const AutoParamDataSource& source);
        /** Updates the automatic parameters for lights based on the details provided. */
        void _updateAutoParamsLightsOnly(const AutoParamDataSource& source);

		/** Sets the auto add parameter name flag
		@remarks
			Not all GPU programs make named parameters available after the high level
			source is compiled.  GLSL is one such case.  If parameter names are not loaded
			prior to the material serializer reading in parameter names in a script then
			an exception is generated.  Set AutoAddParamName to true to have names not found
			in the map added to the map.
		@note
			The index of the parameter name will be set to the end of the Real Constant List.
		@param state true to enable automatic name
		*/
		void setAutoAddParamName(bool state) { mAutoAddParamName = state; }

		/** Sets a single value constant floating-point parameter to the program.
        @remarks
            Different types of GPU programs support different types of constant parameters.
            For example, it's relatively common to find that vertex programs only support
            floating point constants, and that fragment programs only support integer (fixed point)
            parameters. This can vary depending on the program version supported by the
            graphics card being used. You should consult the documentation for the type of
            low level program you are using, or alternatively use the methods
            provided on RenderSystemCapabilities to determine the options.
        @par
            Another possible limitation is that some systems only allow constants to be set
            on certain boundaries, e.g. in sets of 4 values for example. Again, see
            RenderSystemCapabilities for full details.
        @note
            This named option will only work if you are using a parameters object created
            from a high-level program (HighLevelGpuProgram).
		@param name The name of the parameter
		@param val The value to set
		*/
		void setNamedConstant(const String& name, Real val);
		/** Sets a single value constant integer parameter to the program.
        @remarks
            Different types of GPU programs support different types of constant parameters.
            For example, it's relatively common to find that vertex programs only support
            floating point constants, and that fragment programs only support integer (fixed point)
            parameters. This can vary depending on the program version supported by the
            graphics card being used. You should consult the documentation for the type of
            low level program you are using, or alternatively use the methods
            provided on RenderSystemCapabilities to determine the options.
        @par
            Another possible limitation is that some systems only allow constants to be set
            on certain boundaries, e.g. in sets of 4 values for example. Again, see
            RenderSystemCapabilities for full details.
        @note
            This named option will only work if you are using a parameters object created
            from a high-level program (HighLevelGpuProgram).
        @param name The name of the parameter
		@param val The value to set
		*/
		void setNamedConstant(const String& name, int val);
		/** Sets a Vector4 parameter to the program.
        @param name The name of the parameter
		@param vec The value to set
		*/
		void setNamedConstant(const String& name, const Vector4& vec);
		/** Sets a Vector3 parameter to the program.
        @note
            This named option will only work if you are using a parameters object created
            from a high-level program (HighLevelGpuProgram).
		@param index The index at which to place the parameter
			NB this index refers to the number of floats, so a Vector3 is 3. Note that many 
            rendersystems & programs assume that every floating point parameter is passed in
            as a vector of 4 items, so you are strongly advised to check with 
            RenderSystemCapabilities before using this version - if in doubt use Vector4
            or ColourValue instead (both are 4D).
		@param vec The value to set
		*/
		void setNamedConstant(const String& name, const Vector3& vec);
		/** Sets a Matrix4 parameter to the program.
        @param name The name of the parameter
		@param m The value to set
		*/
		void setNamedConstant(const String& name, const Matrix4& m);
        /** Sets a list of Matrix4 parameters to the program.
        @param name The name of the parameter; this must be the first index of an array,
            for examples 'matrices[0]'
        NB since a Matrix4 is 16 floats long, so each entry will take up 4 indexes.
        @param m Pointer to an array of matrices to set
        @param numEntries Number of Matrix4 entries
        */
        void setNamedConstant(const String& name, const Matrix4* m, size_t numEntries);
		/** Sets a multiple value constant floating-point parameter to the program.
        @remarks
            Different types of GPU programs support different types of constant parameters.
            For example, it's relatively common to find that vertex programs only support
            floating point constants, and that fragment programs only support integer (fixed point)
            parameters. This can vary depending on the program version supported by the
            graphics card being used. You should consult the documentation for the type of
            low level program you are using, or alternatively use the methods
            provided on RenderSystemCapabilities to determine the options.
        @par
            Another possible limitation is that some systems only allow constants to be set
            on certain boundaries, e.g. in sets of 4 values for example. Again, see
            RenderSystemCapabilities for full details.
        @note
            This named option will only work if you are using a parameters object created
            from a high-level program (HighLevelGpuProgram).
        @param name The name of the parameter
		@param val Pointer to the values to write
		@param count The number of floats to write
		*/
		void setNamedConstant(const String& name, const float *val, size_t count);
		/** Sets a multiple value constant floating-point parameter to the program.
        @remarks
            Different types of GPU programs support different types of constant parameters.
            For example, it's relatively common to find that vertex programs only support
            floating point constants, and that fragment programs only support integer (fixed point)
            parameters. This can vary depending on the program version supported by the
            graphics card being used. You should consult the documentation for the type of
            low level program you are using, or alternatively use the methods
            provided on RenderSystemCapabilities to determine the options.
        @par
            Another possible limitation is that some systems only allow constants to be set
            on certain boundaries, e.g. in sets of 4 values for example. Again, see
            RenderSystemCapabilities for full details.
        @note
            This named option will only work if you are using a parameters object created
            from a high-level program (HighLevelGpuProgram).
        @param name The name of the parameter
		@param val Pointer to the values to write
		@param count The number of floats to write
		*/
		void setNamedConstant(const String& name, const double *val, size_t count);
		/** Sets a ColourValue parameter to the program.
        @param name The name of the parameter
		@param colour The value to set
		*/
        void setNamedConstant(const String& name, const ColourValue& colour);
		
		/** Sets a multiple value constant integer parameter to the program.
        @remarks
            Different types of GPU programs support different types of constant parameters.
            For example, it's relatively common to find that vertex programs only support
            floating point constants, and that fragment programs only support integer (fixed point)
            parameters. This can vary depending on the program version supported by the
            graphics card being used. You should consult the documentation for the type of
            low level program you are using, or alternatively use the methods
            provided on RenderSystemCapabilities to determine the options.
        @par
            Another possible limitation is that some systems only allow constants to be set
            on certain boundaries, e.g. in sets of 4 values for example. Again, see
            RenderSystemCapabilities for full details.
        @note
            This named option will only work if you are using a parameters object created
            from a high-level program (HighLevelGpuProgram).
        @param name The name of the parameter
		@param val Pointer to the values to write
		@param count The number of integers to write
		*/
		void setNamedConstant(const String& name, const int *val, size_t count);

        /** Sets up a constant which will automatically be updated by the system.
        @remarks
            Vertex and fragment programs often need parameters which are to do with the
            current render state, or particular values which may very well change over time,
            and often between objects which are being rendered. This feature allows you 
            to set up a certain number of predefined parameter mappings that are kept up to 
            date for you.
        @note
            This named option will only work if you are using a parameters object created
            from a high-level program (HighLevelGpuProgram).
        @param name The name of the parameter
        @param acType The type of automatic constant to set
        @param extraInfo If the constant type needs more information (like a light index) put it here.
        */
        void setNamedAutoConstant(const String& name, AutoConstantType acType, size_t extraInfo = 0);

        /** Sets a named parameter up to track a derivation of the current time.
        @note
            This named option will only work if you are using a parameters object created
            from a high-level program (HighLevelGpuProgram).
        @param name The name of the parameter
        @param factor The amount by which to scale the time value
        */  
        void setNamedConstantFromTime(const String& name, Real factor);

		/** Unbind an auto constant so that the constant is manually controlled again. */
		void clearNamedAutoConstant(const String& name);

        /// Internal method for associating a parameter name with an index
        void _mapParameterNameToIndex(const String& name, const size_t index );

        /** Gets the constant index associated with a named parameter. */
        size_t getParamIndex(const String& name);


        /** Sets whether or not we need to transpose the matrices passed in from the rest of OGRE.
        @remarks
            D3D uses transposed matrices compared to GL and OGRE; this is not important when you
            use programs which are written to process row-major matrices, such as those generated
            by Cg, but if you use a program written to D3D's matrix layout you will need to enable
            this flag.
        */
        void setTransposeMatrices(bool val) { mTransposeMatrices = val; } 
        /// Gets whether or not matrices are to be transposed when set
        bool getTransposeMatrices(void) const { return mTransposeMatrices; } 

		/** Copies the values of all constants (including auto constants) from another
			GpuProgramParameters object.
		*/
		void copyConstantsFrom(const GpuProgramParameters& source);

        /** Add (or update) a constant definition which describes a constant.  
		@remarks
			Mainly used for Material serialization but could also be used by material
            editors. Returns the index of the constant definition.
        @param name The name of the parameter.
        @param index The constant index at which to place the parameter (each constant is
            a 4D float).
        @param elementCount The number of elements that make up the parameter. 
			An example is if the parameter is a matrix4x4 then there are 16 
			elements. 
        @param isReal If true then indicates that the elements are float else they are int.
        */
        size_t addConstantDefinition(const String& name, const size_t index, 
			const size_t elementCount, const ElementType elementType);

        /** gets the constant definition associated with name if found else returns NULL
        @param name The name of the constant
        */
        const ConstantDefinition* getConstantDefinition(const String& name) const;
        /** gets the constant definition using an index into the constant definition array.
            If the index is out of bounds then NULL is returned;
        @param idx The constant index
        */
        const ConstantDefinition* getConstantDefinition(const size_t idx) const;
        /** Find a matching constant defintion.  Matches name, entry index, and element type.
        @returns NULL if no match is found.
        */
        const ConstantDefinition* findMatchingConstantDefinition(const String& name, 
            const size_t entryIndex, const ElementType elementType) const;

        /** Returns the number of constant definitions
        */
        size_t getNumConstantDefinitions(void) const { return mConstantDefinitions.size(); }
        /** Set the constant definition's Auto state.
        @param index The index of the constant definition.
        @param isAuto If true then constant is being updated automatically.
        @param autoIndex Index for AutoConstantEntry.
        */
        void setConstantDefinitionAutoState( const size_t index, 
			const bool isAuto, const size_t autoIndex );
        /** gets the auto constant definition associated with name if found else returns NULL
        @param name The name of the auto constant
        */
        static const AutoConstantDefinition* getAutoConstantDefinition(const String& name);
        /** gets the auto constant definition using an index into the auto constant definition array.
            If the index is out of bounds then NULL is returned;
        @param idx The auto constant index
        */
        static const AutoConstantDefinition* getAutoConstantDefinition(const size_t idx);
        /** Returns the number of auto constant definitions
        */
        static size_t getNumAutoConstantDefinitions(void);
        /** increments the multipass number entry by 1 if it exists
        */
        void incPassIterationNumber(void);
        /** gets the MultipassEntry if it exists.
        @returns NULL if a Multipass constant entry does not exist.
        */
        RealConstantEntry* getPassIterationEntry(void);
        /** gets the MultipassEntry index.  The value returned is only valid if if 
            getMultipassEntry() does not return NULL.
        */
        size_t getPassIterationEntryIndex(void) const { return mActivePassIterationEntryIndex; }
    };

    /// Shared pointer used to hold references to GpuProgramParameters instances
    typedef SharedPtr<GpuProgramParameters> GpuProgramParametersSharedPtr;

    // Forward declaration 
    class GpuProgramPtr;

	/** Defines a program which runs on the GPU such as a vertex or fragment program. 
	@remarks
		This class defines the low-level program in assembler code, the sort used to
		directly assemble into machine instructions for the GPU to execute. By nature,
		this means that the assembler source is rendersystem specific, which is why this
		is an abstract class - real instances are created through the RenderSystem. 
		If you wish to use higher level shading languages like HLSL and Cg, you need to 
		use the HighLevelGpuProgram class instead.
	*/
	class _OgreExport GpuProgram : public Resource
	{
	protected:
		/// Command object - see ParamCommand 
		class _OgreExport CmdType : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		class _OgreExport CmdSyntax : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		class _OgreExport CmdSkeletal : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		// Command object for setting / getting parameters
		static CmdType msTypeCmd;
		static CmdSyntax msSyntaxCmd;
		static CmdSkeletal msSkeletalCmd;
	
		/// The type of the program
		GpuProgramType mType;
		/// The name of the file to load source from (may be blank)
		String mFilename;
        /// The assembler source of the program (may be blank until file loaded)
        String mSource;
        /// Whether we need to load source from file or not
        bool mLoadFromFile;
        /// Syntax code eg arbvp1, vs_2_0 etc
        String mSyntaxCode;
        /// Does this (vertex) program include skeletal animation?
        bool mSkeletalAnimation;
		/// Does this (vertex) program include morph animation?
		bool mMorphAnimation;
		/// Does this (vertex) program include pose animation (count of number of poses supported)
		ushort mPoseAnimation;
		/// The default parameters for use with this object
		GpuProgramParametersSharedPtr mDefaultParams;
		/// Does this program want light states passed through fixed pipeline
		bool mPassSurfaceAndLightStates;
		/// Did we encounter a compilation error?
		bool mCompileError;

		/** Internal method for setting up the basic parameter definitions for a subclass. 
		@remarks
		Because StringInterface holds a dictionary of parameters per class, subclasses need to
		call this to ask the base class to add it's parameters to their dictionary as well.
		Can't do this in the constructor because that runs in a non-virtual context.
		@par
		The subclass must have called it's own createParamDictionary before calling this method.
		*/
		void setupBaseParamDictionary(void);

		/// @copydoc Resource::calculateSize
		size_t calculateSize(void) const { return 0; } // TODO 

		/// @copydoc Resource::loadImpl
		void loadImpl(void);
	public:

		GpuProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
			const String& group, bool isManual = false, ManualResourceLoader* loader = 0);

		virtual ~GpuProgram() {}

        /** Sets the filename of the source assembly for this program.
        @remarks
            Setting this will have no effect until you (re)load the program.
        */
        virtual void setSourceFile(const String& filename);

		/** Sets the source assembly for this program from an in-memory string.
        @remarks
            Setting this will have no effect until you (re)load the program.
        */
        virtual void setSource(const String& source);

        /** Gets the syntax code for this program e.g. arbvp1, fp20, vs_1_1 etc */
        virtual const String& getSyntaxCode(void) const { return mSyntaxCode; }

		/** Sets the syntax code for this program e.g. arbvp1, fp20, vs_1_1 etc */
		virtual void setSyntaxCode(const String& syntax);

		/** Gets the name of the file used as source for this program. */
		virtual const String& getSourceFile(void) const { return mFilename; }
        /** Gets the assembler source for this program. */
        virtual const String& getSource(void) const { return mSource; }
		/// Set the program type (only valid before load)
		virtual void setType(GpuProgramType t);
        /// Get the program type
        virtual GpuProgramType getType(void) const { return mType; }

        /** Returns the GpuProgram which should be bound to the pipeline.
        @remarks
            This method is simply to allow some subclasses of GpuProgram to delegate
            the program which is bound to the pipeline to a delegate, if required. */
        virtual GpuProgram* _getBindingDelegate(void) { return this; }

        /** Returns whether this program can be supported on the current renderer and hardware. */
        virtual bool isSupported(void) const;

        /** Creates a new parameters object compatible with this program definition. 
        @remarks
            It is recommended that you use this method of creating parameters objects
            rather than going direct to GpuProgramManager, because this method will
            populate any implementation-specific extras (like named parameters) where
            they are appropriate.
        */
        virtual GpuProgramParametersSharedPtr createParameters(void);

        /** Sets whether a vertex program includes the required instructions
        to perform skeletal animation. 
        @remarks
        If this is set to true, OGRE will not blend the geometry according to 
        skeletal animation, it will expect the vertex program to do it.
        */
        virtual void setSkeletalAnimationIncluded(bool included) 
        { mSkeletalAnimation = included; }

        /** Returns whether a vertex program includes the required instructions
            to perform skeletal animation. 
        @remarks
            If this returns true, OGRE will not blend the geometry according to 
            skeletal animation, it will expect the vertex program to do it.
        */
        virtual bool isSkeletalAnimationIncluded(void) const { return mSkeletalAnimation; }

        /** Sets whether a vertex program includes the required instructions
        to perform morph animation. 
        @remarks
        If this is set to true, OGRE will not blend the geometry according to 
        morph animation, it will expect the vertex program to do it.
        */
        virtual void setMorphAnimationIncluded(bool included) 
		{ mMorphAnimation = included; }

        /** Sets whether a vertex program includes the required instructions
        to perform pose animation. 
        @remarks
        If this is set to true, OGRE will not blend the geometry according to 
        pose animation, it will expect the vertex program to do it.
		@param poseCount The number of simultaneous poses the program can blend
        */
        virtual void setPoseAnimationIncluded(ushort poseCount) 
		{ mPoseAnimation = poseCount; }

		/** Returns whether a vertex program includes the required instructions
            to perform morph animation. 
        @remarks
            If this returns true, OGRE will not blend the geometry according to 
            morph animation, it will expect the vertex program to do it.
        */
        virtual bool isMorphAnimationIncluded(void) const { return mMorphAnimation; }

		/** Returns whether a vertex program includes the required instructions
            to perform pose animation. 
        @remarks
            If this returns true, OGRE will not blend the geometry according to 
            pose animation, it will expect the vertex program to do it.
        */
        virtual bool isPoseAnimationIncluded(void) const { return mPoseAnimation > 0; }
		/** Returns the number of simultaneous poses the vertex program can 
			blend, for use in pose animation.
        */
        virtual ushort getNumberOfPosesIncluded(void) const { return mPoseAnimation; }

		/** Get a reference to the default parameters which are to be used for all
			uses of this program.
		@remarks
			A program can be set up with a list of default parameters, which can save time when 
			using a program many times in a material with roughly the same settings. By 
			retrieving the default parameters and populating it with the most used options, 
			any new parameter objects created from this program afterwards will automatically include
			the default parameters; thus users of the program need only change the parameters
			which are unique to their own usage of the program.
		*/
		virtual GpuProgramParametersSharedPtr getDefaultParameters(void);

        /** Returns true if default parameters have been set up.  
        */
        virtual bool hasDefaultParameters(void) const { return !mDefaultParams.isNull(); }

		/** Sets whether a vertex program requires light and material states to be passed
		to through fixed pipeline low level API rendering calls.
		@remarks
		If this is set to true, OGRE will pass all active light states to the fixed function
		pipeline.  This is useful for high level shaders like GLSL that can read the OpenGL
		light and material states.  This way the user does not have to use autoparameters to 
		pass light position, color etc.
		*/
		virtual void setSurfaceAndPassLightStates(bool state)
			{ mPassSurfaceAndLightStates = state; }

		/** Returns whether a vertex program wants light and material states to be passed
		through fixed pipeline low level API rendering calls
		*/
		virtual bool getPassSurfaceAndLightStates(void) const { return mPassSurfaceAndLightStates; }

        /** Returns a string that specifies the language of the gpu programs as specified
        in a material script. ie: asm, cg, hlsl, glsl
        */
        virtual const String& getLanguage(void) const;

		/** Did this program encounter a compile error when loading?
		*/
		virtual bool hasCompileError(void) const { return mCompileError; }

		/** Reset a compile error if it occurred, allowing the load to be retried
		*/
		virtual void resetCompileError(void) { mCompileError = false; }
    protected:
        /// Virtual method which must be implemented by subclasses, load from mSource
        virtual void loadFromSource(void) = 0;

	};


	/** Specialisation of SharedPtr to allow SharedPtr to be assigned to GpuProgramPtr 
	@note Has to be a subclass since we need operator=.
	We could templatise this instead of repeating per Resource subclass, 
	except to do so requires a form VC6 does not support i.e.
	ResourceSubclassPtr<T> : public SharedPtr<T>
	*/
	class _OgreExport GpuProgramPtr : public SharedPtr<GpuProgram> 
	{
	public:
		GpuProgramPtr() : SharedPtr<GpuProgram>() {}
		explicit GpuProgramPtr(GpuProgram* rep) : SharedPtr<GpuProgram>(rep) {}
		GpuProgramPtr(const GpuProgramPtr& r) : SharedPtr<GpuProgram>(r) {} 
		GpuProgramPtr(const ResourcePtr& r) : SharedPtr<GpuProgram>()
		{
			// lock & copy other mutex pointer
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			    pRep = static_cast<GpuProgram*>(r.getPointer());
			    pUseCount = r.useCountPointer();
			    if (pUseCount)
			    {
				    ++(*pUseCount);
			    }
            }
		}

		/// Operator used to convert a ResourcePtr to a GpuProgramPtr
		GpuProgramPtr& operator=(const ResourcePtr& r)
		{
			if (pRep == static_cast<GpuProgram*>(r.getPointer()))
				return *this;
			release();
			// lock & copy other mutex pointer
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
                OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			    pRep = static_cast<GpuProgram*>(r.getPointer());
			    pUseCount = r.useCountPointer();
			    if (pUseCount)
			    {
				    ++(*pUseCount);
			    }
            }
			return *this;
		}
        /// Operator used to convert a HighLevelGpuProgramPtr to a GpuProgramPtr
        GpuProgramPtr& operator=(const HighLevelGpuProgramPtr& r);
	};
}

#endif

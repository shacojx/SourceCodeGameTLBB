/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.stevestreeting.com/ogre/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/gpl.html.
-----------------------------------------------------------------------------
*/

#ifndef __MaterialScriptScompiler_H__
#define __MaterialScriptScompiler_H__

#include "OgreCompiler2Pass.h"
#include "OgrePrerequisites.h"
#include "OgreTextureUnitState.h"
#include "OgreMaterial.h"
//#include "OgreBlendMode.h"
//#include "OgreTextureUnitState.h"
#include "OgreGpuProgram.h"
#include "OgreStringVector.h"

namespace Ogre {

    class _OgreExport MaterialScriptCompiler : public Compiler2Pass
    {

    public:
        MaterialScriptCompiler(void);
        ~MaterialScriptCompiler(void);
        /** gets BNF Grammer for Compositor script.
        */
        virtual const String& getClientBNFGrammer(void) { return materialScript_BNF; }

        /** get the name of the BNF grammer.
        */
        virtual const String& getClientGrammerName(void) const { static const String grammerName("Material Script"); return grammerName; }
        /** Compile a material script from a data stream using a specific resource group name.
        @param stream Weak reference to a data stream which is the source of the material script
        @param groupName The name of the resource group that resources which are
			parsed are to become a member of. If this group is loaded or unloaded,
			then the resources discovered in this script will be loaded / unloaded
			with it.
        */
        void parseScript(DataStreamPtr& stream, const String& groupName)
        {
            mScriptContext.groupName = groupName;
            Compiler2Pass::compile(stream->getAsString(),  stream->getName());
        }

    protected:
	    // Token ID enumeration
	    enum TokenID {
		    // Terminal Tokens section
            ID_UNKOWN = 0, ID_OPENBRACE, ID_CLOSEBRACE,
            // GPU Program
            ID_VERTEX_PROGRAM, ID_FRAGMENT_PROGRAM, ID_SOURCE, ID_SYNTAX, ID_CUSTOM_PARAMETER,
            ID_DEFAULT_PARAMS,
            ID_INCLUDES_SKELETAL_ANIMATION, ID_INCLUDES_MORPH_ANIMATION, ID_INCLUDES_POSE_ANIMATION,
            // material
            ID_MATERIAL, ID_CLONE, ID_LOD_DISTANCES, ID_RECEIVE_SHADOWS,
            ID_TRANSPARENCY_CASTS_SHADOWS, ID_SET_TEXTURE_ALIAS,
            // technique
            ID_TECHNIQUE, ID_SCHEME, ID_LOD_INDEX,
            // pass
            ID_PASS, ID_AMBIENT, ID_DIFFUSE, ID_SPECULAR, ID_EMISSIVE,
            ID_VERTEXCOLOUR,
            // scene blend
            ID_SCENE_BLEND, ID_COLOUR_BLEND, ID_DEST_COLOUR,
            ID_SRC_COLOUR, ID_ONE_MINUS_DEST_COLOUR, ID_ONE_MINUS_SRC_COLOUR,
            ID_DEST_ALPHA, ID_SRC_ALPHA, ID_ONE_MINUS_DEST_ALPHA, ID_ONE_MINUS_SRC_ALPHA,
            // Depth
            ID_DEPTH_CHECK, ID_DEPTH_WRITE, ID_DEPTH_FUNC, ID_DEPTH_BIAS, ID_ALWAYS_FAIL, ID_ALWAYS_PASS,
            ID_LESS_EQUAL, ID_LESS, ID_EQUAL, ID_NOT_EQUAL, ID_GREATER_EQUAL, ID_GREATER,

            ID_ALPHA_REJECTION, ID_CULL_HARDWARE, ID_CLOCKWISE, ID_ANTICLOCKWISE,
            ID_CULL_SOFTWARE, ID_CULL_BACK, ID_CULL_FRONT,
            ID_LIGHTING, ID_SHADING, ID_FLAT, ID_GOURAUD, ID_PHONG,
            ID_POLYGON_MODE, ID_SOLID, ID_WIREFRAME, ID_POINTS,
            ID_FOG_OVERRIDE, ID_EXP, ID_EXP2,
            ID_COLOUR_WRITE, ID_MAX_LIGHTS,
            ID_ITERATION, ID_ONCE, ID_ONCE_PER_LIGHT, ID_PER_LIGHT, ID_DIRECTIONAL, ID_SPOT,
			ID_POINT_SIZE, ID_POINT_SPRITES, ID_POINT_SIZE_ATTENUATION,
			ID_POINT_SIZE_MIN, ID_POINT_SIZE_MAX,

            // texture unit state
            ID_TEXTURE_UNIT, ID_TEXTURE_ALIAS, ID_TEXTURE, ID_1D, ID_2D, ID_3D, ID_CUBIC, ID_UNLIMITED,
            ID_ALPHA, ID_ANIM_TEXTURE, ID_CUBIC_TEXTURE, ID_SEPARATE_UV, ID_COMBINED_UVW,
            ID_TEX_COORD_SET, ID_TEX_ADDRESS_MODE, ID_WRAP, ID_CLAMP, ID_MIRROR, ID_BORDER, ID_TEX_BORDER_COLOUR,
            ID_FILTERING, ID_BILINEAR, ID_TRILINEAR, ID_ANISOTROPIC,
            ID_MAX_ANISOTROPY, ID_COLOUR_OP, ID_REPLACE,
            ID_COLOUR_OP_EX, ID_SOURCE1, ID_SOURCE2, ID_MODULATE_X2, ID_MODULATE_X4, ID_ADD_SIGNED,
            ID_ADD_SMOOTH, ID_SUBTRACT, ID_BLEND_DIFFUSE_COLOUR, ID_BLEND_DIFFUSE_ALPHA,
            ID_BLEND_TEXTURE_ALPHA, ID_BLEND_CURRENT_ALPHA, ID_BLEND_MANUAL, ID_DOTPRODUCT,
            ID_SRC_CURRENT, ID_SRC_TEXTURE, ID_SRC_DIFFUSE, ID_SRC_SPECULAR, ID_SRC_MANUAL,
            ID_COLOUR_OP_MULTIPASS_FALLBACK, ID_ALPHA_OP_EX,
            ID_ENV_MAP, ID_SPHERICAL, ID_PLANAR, ID_CUBIC_REFLECTION, ID_CUBIC_NORMAL,
            ID_SCROLL, ID_SCROLL_ANIM, ID_ROTATE, ID_ROTATE_ANIM, ID_SCALE, ID_WAVE_XFORM,
            ID_SCROLL_X, ID_SCROLL_Y, ID_SCALE_X, ID_SCALE_Y, ID_SINE, ID_TRIANGLE,
            ID_SQUARE, ID_SAWTOOTH, ID_INVERSE_SAWTOOTH,
            ID_TRANSFORM,
            // GPU program references
            ID_VERTEX_PROGRAM_REF, ID_FRAGMENT_PROGRAM_REF, ID_SHADOW_CASTER_VERTEX_PROGRAM_REF,
            ID_SHADOW_RECEIVER_VERTEX_PROGRAM_REF, ID_SHADOW_RECEIVER_FRAGMENT_PROGRAM_REF,
            // GPU Parameters
            ID_PARAM_INDEXED_AUTO, ID_PARAM_INDEXED, ID_PARAM_NAMED_AUTO, ID_PARAM_NAMED,

            // general
            ID_ON, ID_OFF, ID_TRUE, ID_FALSE, ID_NONE, ID_POINT, ID_LINEAR, ID_ADD, ID_MODULATE, ID_ALPHA_BLEND,
            ID_ONE, ID_ZERO
        };

        /** Enum to identify material sections. */
        enum MaterialScriptSection
        {
            MSS_NONE,
            MSS_MATERIAL,
            MSS_TECHNIQUE,
            MSS_PASS,
            MSS_TEXTUREUNIT,
            MSS_PROGRAM_REF,
		    MSS_PROGRAM,
            MSS_DEFAULT_PARAMETERS,
		    MSS_TEXTURESOURCE
        };
	    /** Struct for holding a program definition which is in progress. */
	    struct MaterialScriptProgramDefinition
	    {
		    String name;
		    GpuProgramType progType;
            String language;
		    String source;
		    String syntax;
            bool supportsSkeletalAnimation;
		    bool supportsMorphAnimation;
		    ushort supportsPoseAnimation; // number of simultaneous poses supported
		    std::map<String, String> customParameters;
	    };
        /** Struct for holding the script context while parsing. */
        struct MaterialScriptContext
        {
            MaterialScriptSection section;
		    String groupName;
            MaterialPtr material;
            Technique* technique;
            Pass* pass;
            TextureUnitState* textureUnit;
            GpuProgramPtr program; // used when referencing a program, not when defining it
            bool isProgramShadowCaster; // when referencing, are we in context of shadow caster
            bool isVertexProgramShadowReceiver; // when referencing, are we in context of shadow caster
            bool isFragmentProgramShadowReceiver; // when referencing, are we in context of shadow caster
            GpuProgramParametersSharedPtr programParams;
			ushort numAnimationParametrics;
		    MaterialScriptProgramDefinition* programDef; // this is used while defining a program

		    int techLev,	//Keep track of what tech, pass, and state level we are in
			    passLev,
			    stateLev;
            // container of token que positions for default params that require pass 2 processing
            std::vector<size_t> pendingDefaultParams;

            AliasTextureNamePairList textureAliases;
        };

        MaterialScriptContext mScriptContext;

	    // static library database for tokens and BNF rules
	    static TokenRule materialScript_RulePath[];
        // simplified Backus - Naur Form (BNF) grammer for material scripts
        static String materialScript_BNF;

        typedef void (MaterialScriptCompiler::* MSC_Action)(void);
        typedef std::map<size_t, MSC_Action> TokenActionMap;
        typedef TokenActionMap::iterator TokenActionIterator;
        /** Map of Token value as key to an Action.  An Action converts tokens into
            the final format.
            All instances use the same Token Action Map.
        */
        static TokenActionMap mTokenActionMap;

        /** Execute an Action associated with a token.  Gets called when the compiler finishes tokenizing a
            section of the source that has been parsed.
        **/
        virtual void executeTokenAction(const size_t tokenID);
        /** Associate all the lexemes used in a material script with their corresponding tokens and actions.
        **/
        virtual void setupTokenDefinitions(void);
        void addLexemeTokenAction(const String& lexeme, const size_t token, const MSC_Action action = 0);

        void logParseError(const String& error);

        // support methods that convert tokens
        ColourValue _parseColourValue(void);
        CompareFunction convertCompareFunction(void);

        // Token Actions which get called when tokens are created during parsing.
        void parseOpenBrace(void);
        void parseCloseBrace(void);
        // material section Actions
        void parseMaterial(void);
        void parseLodDistances(void);
        void parseReceiveShadows(void);
        void parseTransparencyCastsShadows(void);
        void parseSetTextureAlias(void);
        // Technique related actions
        void parseTechnique(void);
        void parseScheme(void);
        void parseLodIndex(void);
        // Pass related Actions
        void parsePass(void);
        void parseAmbient(void);
        void parseDiffuse(void);
        void parseSpecular(void);
        void parseEmissive(void);
        void parseSceneBlend(void);
        SceneBlendFactor convertBlendFactor(void);
        void parseDepthCheck(void);
        void parseDepthWrite(void);
        void parseDepthFunc(void);
        void parseDepthBias(void);
        void parseAlphaRejection(void);
        void parseCullHardware(void);
        void parseCullSoftware(void);
        void parseLighting(void);
        void parseShading(void);
        void parsePolygonMode(void);
        void parseFogOverride(void);
        void parseMaxLights(void);
        void parseIteration(void);
        void parseIterationLightTypes(void);
        void parseColourWrite(void);
        void parsePointSize(void);
        void parsePointSprites(void);
        void parsePointSizeMin(void);
        void parsePointSizeMax(void);
        void parsePointSizeAttenuation(void);
        // Texture Unit related Actions
        void parseTextureUnit(void);
        void parseTextureAlias(void);
        void parseTexture(void);
        void parseAnimTexture(void);
        void parseCubicTexture(void);
        void parseTexCoord(void);
        TextureUnitState::TextureAddressingMode convTexAddressMode(void);
        void parseTexAddressMode(void);
        void parseTexBorderColour(void);
        void parseFiltering(void);
        FilterOptions convertFiltering();
        void parseMaxAnisotropy(void);
        void parseColourOp(void);
        void parseColourOpEx(void);
        LayerBlendOperationEx convertBlendOpEx(void);
        LayerBlendSource convertBlendSource(void);
        void parseColourOpMultipassFallback(void);
        void parseAlphaOpEx(void);
        void parseEnvMap(void);
        void parseScroll(void);
        void parseScrollAnim(void);
        void parseRotate(void);
        void parseRotateAnim(void);
        void parseScale(void);
        void parseWaveXform(void);
        void parseTransform(void);
        void parseTextureCustomParameter(void);
        // GPU Program
        void parseGPUProgram(void);
        void parseProgramSource(void);
        void parseProgramSyntax(void);
        void parseProgramCustomParameter(void);
        void parseDefaultParams(void);
        void parseProgramSkeletalAnimation(void);
        void parseProgramMorphAnimation(void);
        void parseProgramPoseAnimation(void);
        void parseVertexProgramRef(void);
        void parseFragmentProgramRef(void);
        void parseShadowCasterVertexProgramRef(void);
        void parseShadowReceiverVertexProgramRef(void);
        void parseShadowReceiverFragmentProgramRef(void);
        void parseParamIndexed(void);
        void parseParamIndexedAuto(void);
        void parseParamNamed(void);
        void parseParamNamedAuto(void);
        void processManualProgramParam(size_t index, const String& commandname, const String& paramName = StringUtil::BLANK);
        void processAutoProgramParam(size_t index, const String& commandname, const String& paramName = StringUtil::BLANK);


    	void finishProgramDefinition(void);

    };
}

#endif

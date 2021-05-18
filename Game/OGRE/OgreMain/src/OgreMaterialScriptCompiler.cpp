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
#include "OgreStableHeaders.h"
#include "OgreMaterialScriptCompiler.h"
#include "OgreStringConverter.h"
#include "OgreLogManager.h"
#include "OgreException.h"
#include "OgreBlendMode.h"
#include "OgreGpuProgram.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreMaterialManager.h"
#include "OgreGpuProgramManager.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreExternalTextureSourceManager.h"

namespace Ogre {

    //-----------------------------------------------------------------------
    // Static definitions
    //-----------------------------------------------------------------------
    MaterialScriptCompiler::TokenActionMap MaterialScriptCompiler::mTokenActionMap;

    String MaterialScriptCompiler::materialScript_BNF =
        "<Script> ::= {<Script_Properties>} \n"

        "<Script_Properties> ::= <Material> | <Vertex_Program> | <Fragment_Program> \n"

        "<Material> ::= 'material' <Flex_Label> [<Material_Clone>] '{' {<Material_Properties>} '}' \n"

        "<Material_Properties> ::= <Technique> | <Set_Texture_Alias> | "
        "                          <Lod_Distances> | <Receive_Shadows> | "
        "                          <Transparency_Casts_Shadows> \n"

        "    <Material_Clone> ::= ':' <Flex_Label> \n"
        "    <Set_Texture_Alias> ::= 'set_texture_alias' <Label> [<Seperator>] <Label> \n"
        "    <Lod_Distances> ::= 'lod_distances' <#distance> {<#distance>} \n"
        "    <Receive_Shadows> ::= 'receive_shadows' <On_Off> \n"
        "    <Transparency_Casts_Shadows> ::= 'transparency_casts_shadows' <On_Off> \n"

        // Technique section rules
        "<Technique> ::= 'technique' [<Label>] '{' {<Technique_Properties>} '}' \n"
        "    <Technique_Properties> ::= <Pass> | <Lod_Index> | <Scheme> \n"
        "    <Lod_Index> ::= 'lod_index' <#value> \n"
        "    <Scheme> ::= 'scheme' <Label> \n"

        // Pass section rules
        "    <Pass> ::= 'pass' [<Label>] '{' {<Pass_Properties>} '}' \n"
        "        <Pass_Properties> ::= <Ambient> | <Diffuse> | <Specular> | <Emissive> | \n"
        "                              <Scene_Blend> | <Depth_Check> | <Depth_Write> | \n"
        "                              <Texture_Unit> | \n"
        "                              <Depth_Func> | <Depth_Bias> | <Alpha_Rejection> | \n"
        "                              <Cull_Hardware> | <Cull_Software> | <Lighting> | \n"
        "                              <GPU_Program_Ref> | \n"
        "                              <Shading> | <PolygonMode> | <Fog_Override> | <Colour_Write> | \n"
        "                              <Max_Lights> | <Iteration> | \n"
		"                              <Point_Sprites> | <Point_Size_Attenuation> | \n"
		"                              <Point_Size_Min> | <Point_Size_Max> | <Point_Size> \n"

        "        <Ambient> ::= 'ambient' <ColourOptions> \n"
        "        <Diffuse> ::= 'diffuse' <ColourOptions> \n"
        "        <Specular> ::= 'specular' <SpecularOptions> \n"
        "           <SpecularOptions> ::= <Specular_Colour_Params> | <Specular_Vertex> \n"
        "           <Specular_Colour_Params> ::= <#red> <#green> <#blue> <#val> [<#val>] \n"
        "           <Specular_Vertex> ::= 'vertexcolour' <#shininess> \n"
        "        <Emissive> ::= 'emissive' <ColourOptions> \n"

        "        <ColourOptions> ::= <Colour_Params> | 'vertexcolour' \n"

        "        <Scene_Blend> ::= 'scene_blend' <SceneBlend_Options> \n"
        "          <SceneBlend_Options> ::= <Simple_Blend> | <User_Blend> \n"
        "            <Simple_Blend> ::= <Base_Blend> | 'colour_blend' \n"
        "            <Base_Blend> ::= 'alpha_blend' | 'modulate' | 'add' \n"
        "            <User_Blend> ::= <Blend_Factor> <Blend_Factor> \n"
        "            <Blend_Factor> ::= 'dest_colour' | 'src_colour' | \n"
        "                               'one_minus_dest_colour' | 'one_minus_src_colour' | \n"
        "                               'dest_alpha' | 'src_alpha' | 'one_minus_dest_alpha' | \n"
        "                               'one_minus_src_alpha' | 'one' | 'zero' \n"

        "        <Depth_Check> ::= 'depth_check' <On_Off> \n"
        "        <Depth_Write> ::= 'depth_write' <On_Off> \n"
        "        <Depth_Func> ::= 'depth_func' <Compare_Func> \n"
        "        <Depth_Bias> ::= 'depth_bias' <#value> \n"
        "        <Alpha_Rejection> ::= 'alpha_rejection' <Compare_Func> <#value> \n"
        "        <Compare_Func> ::= 'always_fail' | 'always_pass' | 'less_equal' | 'less' | \n"
        "                           'equal' | 'not_equal' | 'greater_equal' | 'greater' \n"
        "        <Cull_Hardware> ::= 'cull_hardware' <Cull_Hardware_Otions> \n"
        "           <Cull_Hardware_Otions> ::= 'clockwise' | 'anticlockwise' | 'none' \n"
        "        <Cull_Software> ::= 'cull_software' <Cull_Software_Otions> \n"
        "           <Cull_Software_Otions> ::= 'back' | 'front' | 'none' \n"
        "        <Lighting> ::= 'lighting' <On_Off> \n"
        "        <Shading> ::= 'shading' <Shading_Options> \n"
        "           <Shading_Options> ::= 'flat' | 'gouraud' | 'phong' \n"
        "        <PolygonMode> ::= 'polygon_mode' <PolygonMode_Options> \n"
        "           <PolygonMode_Options> ::= 'solid' | 'wireframe' | 'points' \n"
        "        <Colour_Write> ::= 'colour_write' <On_Off> \n"
		"        <Point_Size> ::= 'point_size' <#size> \n"
		"        <Point_Sprites> ::= 'point_sprites' <On_Off> \n"
		"        <Point_Size_Min> ::= 'point_size_min' <#size> \n"
		"        <Point_Size_Max> ::= 'point_size_max' <#size> \n"
		"        <Point_Size_Attenuation> ::= 'point_size_attenuation' <On_Off> [<Point_Size_Att_Params>] \n"
		"            <Point_Size_Att_Params> ::= <#constant> <#linear> <#quadric> \n"
        "        <Fog_Override> ::= 'fog_override' <Fog_Override_Options> \n"
        "           <Fog_Override_Options> ::= 'false' | <fog_true> \n"
        "             <fog_true> ::= 'true' [<Fog_True_Params>] \n"
        "               <Fog_True_Params> ::= 'none' | <fog_True_Param_Option> \n"
        "                   <fog_True_Param_Option> ::= <fog_type> <#red> <#green> <#blue> <#fog_density> <#start> <#end> \n"
        "                       <fog_type> ::= 'linear' | 'exp2' | 'exp' \n"
        "        <Max_Lights> ::= 'max_lights' <#number> \n"
        "        <Iteration> ::= 'iteration' <Iteration_Options> \n"
        "           <Iteration_Options> ::= <Iteration_Once_Params> | 'once' | <Iteration_Counted> \n"
        "             <Iteration_Once_Params> ::= 'once_per_light' [<light_type>] \n"
        "             <Iteration_Counted> ::= <#number> [<Per_Light>] \n"
        "               <Per_Light> ::= 'per_light' <light_type> \n"
        "             <light_type> ::= 'point' | 'directional' | 'spot' \n"
        // Texture Unit section rules
        "        <Texture_Unit> ::= 'texture_unit' [<Label>] '{' {<TUS_Properties>} '}' \n"
        "        <TUS_Properties> ::= <Texture_Alias> | <Texture> | <Anim_Texture> | <Cubic_Texture> | \n"
        "                             <Tex_Coord_Set> | <Tex_Address_Mode> | <Tex_Border_Colour> | <Filtering> | \n"
        "                             <Max_Anisotropy> | <Colour_Op_Ex> | <Colour_Op_Multipass_Fallback> | <Colour_Op> | \n"
        "                             <Alpha_Op_Ex> | <Env_Map> | <Scroll_Anim> | <Scroll> | <Rotate_Anim> | <Rotate> | \n"
        "                             <Scale> | <Wave_Xform> | <Transform> \n"
        "           <Texture_Alias> ::= 'texture_alias' <Label> \n"
        "           <Texture> ::= 'texture' <Label> {<Texture_Properties>} \n"
        "           <Texture_Properties> ::= '1d' | '2d' | '3d' | 'cubic' | 'unlimited' | 'alpha' | <#mipmap> \n"
        "                                    | " + PixelUtil::getBNFExpressionOfPixelFormats(true) + " \n"
        "           <Anim_Texture> ::= 'anim_texture' <Label> <Anim_Texture_Properties> \n"
        "               <Anim_Texture_Properties> ::= <Numbered_Anim_Texture> | <Seperate_Anim_Textures> \n"
        "               <Numbered_Anim_Texture> ::= <#frames> <#duration> \n"
        "               <Seperate_Anim_Textures> ::= <anim_frame> {<anim_frame>} \n"
        "                   <anim_frame> ::= (?!<TUS_Terminators>) <Label> [<Seperator>] \n"
        "           <TUS_Terminators> ::= '}' | 'texture_alias' | 'texture' | 'anim_texture' | 'cubic_texture' | \n"
        "                                 'tex_coord_set' | 'tex_address_mode' | 'tex_border_colour' | \n"
        "                                 'filtering' | 'max_anisotropy' | 'colour_op' | 'colour_op_ex' | \n"
        "                                 'colour_op_multipass_fallback' | 'alpha_op_ex' | 'env_map' | \n"
        "                                 'scroll' | 'rotate' | 'scale' | 'wave_xform' | 'transform' \n"
        "           <Cubic_Texture> ::= 'cubic_texture' <Label> <Cubic_Texture_Options> \n"
        "               <Cubic_Texture_Options> ::= 'combineduvw' | 'separateuv' | <Cubic_Seperate> \n"
        "               <Cubic_Seperate> ::= <Label> [<Seperator>] <Label> [<Seperator>] <Label> \n"
        "                                    [<Seperator>] <Label> [<Seperator>] <Label> 'separateuv' \n"
        "           <Tex_Coord_Set> ::= 'tex_coord_set' <#set_num> \n"
        "           <Tex_Address_Mode> ::= 'tex_address_mode' <UVW_Mode> [<UVW_Mode>] [<UVW_Mode>] \n"
        "               <UVW_Mode> ::= 'wrap' | 'clamp' | 'mirror' | 'border' \n"
        "           <Tex_Border_Colour> ::= 'tex_border_colour' <Colour_Params> \n"
        "           <Filtering> ::= 'filtering' <Filtering_Options>"
        "               <Filtering_Options> ::= <Simple_Filter> | <Complex_Filter> \n"
        "                 <Simple_Filter> ::= 'bilinear' | 'trilinear' | 'anisotropic' | 'none' \n"
        "                 <Complex_Filter> ::= <MinMagFilter> <MinMagFilter> <MipFilter> \n"
        "                   <MinMagFilter> ::= 'linear' | 'point' | 'anisotropic' \n"
        "                   <MipFilter> ::= 'linear' | 'point' | 'none' \n"
        "           <Max_Anisotropy> ::= 'max_anisotropy' <#val> \n"
        "           <Colour_Op> ::= 'colour_op' <Colour_Op_Options> \n"
        "               <Colour_Op_Options> ::= <Base_Blend> | 'replace' \n"
        "           <Colour_Op_Ex> ::= 'colour_op_ex' <Combine_Operation> <Source_Option> <Source_Option> {<#val>} \n"
        "               <Combine_Operation> ::= 'source1' | 'source2' | 'modulate_x2' | 'modulate_x4' | \n"
        "                                       'modulate' | 'add_signed' | 'add_smooth' | 'add' | \n"
        "                                       'subtract' | 'blend_diffuse_alpha' | 'blend_texture_alpha' | \n"
        "                                       'blend_current_alpha' | 'blend_manual' | 'dotproduct' | \n"
        "                                       'blend_diffuse_colour' \n"
        "               <Source_Option> ::= 'src_current' | 'src_texture' | 'src_diffuse' | \n"
        "                                   'src_specular' | 'src_manual' \n"
        "           <Colour_Op_Multipass_Fallback> ::= 'colour_op_multipass_fallback' <Blend_Factor> <Blend_Factor> \n"
        "           <Alpha_Op_Ex> ::= 'alpha_op_ex' <Combine_Operation> <Source_Option> <Source_Option> {<#val>} \n"
        "           <Env_Map> ::= 'env_map' <Env_Map_Option> \n"
        "               <Env_Map_Option> ::= 'spherical' | 'planar' | 'cubic_reflection' | 'cubic_normal' | 'off' \n"
        "           <Scroll> ::= 'scroll' <#x> <#y> \n"
        "           <Scroll_Anim> ::= 'scroll_anim' <#xspeed> <#yspeed> \n"
        "           <Rotate> ::= 'rotate' <#angle> \n"
        "           <Rotate_Anim> ::= 'rotate_anim' <#revs_per_second> \n"
        "           <Scale> ::= 'scale' <#x> <#y> \n"
        "           <Wave_Xform> ::= 'wave_xform' <Xform_Type> <Wave_Type> <#base> <#frequency> <#phase> <#amplitude> \n"
        "               <Xform_Type> ::= 'scroll_x' | 'scroll_y' | 'rotate' | 'scale_x' | 'scale_y' \n"
        "               <Wave_Type> ::= 'sine' | 'triangle' | 'square' | 'sawtooth' | 'inverse_sawtooth' \n"
        "           <Transform> ::= 'transform' <#m00> <#m01> <#m02> <#m03> <#m10> <#m11> <#m12> <#m13> <#m20> <#m21> <#m22> <#m23> \n"
        "                           <#m30> <#m31> <#m32> <#m33> \n"
        // GPU Programs
        " "
        "<Vertex_Program> ::= 'vertex_program' <Label> [<Seperator>] <Label> '{' {<Vertex_Program_Option>} '}' \n"
        "   <Vertex_Program_Option> ::= <Vertex_Program_Animation> | <GPU_Program_Options> \n"
        "   <Vertex_Program_Animation> ::= <Skeletal_Animation> | <Morph_Animation> | <Pose_Animation> \n"
        "       <Skeletal_Animation> ::= 'includes_skeletal_animation' <True_False> \n"
        "       <Morph_Animation> ::= 'includes_morph_animation' <True_False> \n"
        "       <Pose_Animation> ::= 'includes_pose_animation' <#val> \n"
        "<Fragment_Program> ::= 'fragment_program' <Label> [<Seperator>] <Label> '{' {<GPU_Program_Options>}'}' \n"
        // do custom parameters last since it will consume everything on the line in the source
        "   <GPU_Program_Options> ::= <Program_Source> | <Syntax> | <Default_Params> | <Custom_Parameter> \n"
        "       <Program_Source> ::= 'source' <Label> \n"
        "       <Syntax> ::= 'syntax' <Label> \n"
        "       <Default_Params> ::= 'default_params' '{' {<GPUParams_Option>} '}' \n"
        "       <Custom_Parameter> ::= 'custom_parameter' : <Unquoted_Label> [<Seperator>] <Spaced_Label> \n"

        "   <GPU_Program_Ref> ::= <GPU_Program_Ref_Type> [<Flex_Label>] '{' {<GPUParams_Option>} '}' \n"
        "       <GPU_Program_Ref_Type> ::= 'vertex_program_ref' | 'fragment_program_ref' | \n"
        "                                  'shadow_caster_vertex_program_ref' | \n"
        "                                  'shadow_receiver_vertex_program_ref' | \n"
        "                                  'shadow_receiver_fragment_program_ref' \n"

        "   <GPUParams_Option> ::= <Param_Named_Auto> | <Param_Named> | <Param_Indexed_Auto> | <Param_Indexed> \n"
        "       <Param_Named_Auto> ::= 'param_named_auto' <Unquoted_Label> [<Seperator>] <Unquoted_Label> [<#val>] \n"
        "       <Param_Named> ::= 'param_named' <Unquoted_Label> [<Seperator>] <Param_Value_Option> \n"
        "       <Param_Indexed_Auto> ::= 'param_indexed_auto' <#index> <Unquoted_Label> [<#val>] \n"
        "       <Param_Indexed> ::= 'param_indexed' <#index> <Param_Value_Option> \n"
        "       <Param_Value_Option> ::= <Unquoted_Label> {<#val>} \n"

        // common rules
        "<On_Off> ::= 'on' | 'off' \n"
        "<True_False> ::= 'true' | 'false' \n"
        "<Colour_Params> ::= <#red> <#green> <#blue> [<#alpha>] \n"
        "<Seperator> ::= -' ' \n"

        "<Labels_1_N> ::= <Label> [<Seperator>] {<More_Labels>} \n"
        "<More_Labels> ::=  <Label> [<Seperator>] \n"
		"<Label> ::= <Quoted_Label> | <Unquoted_Label> \n"
		"<Flex_Label> ::= <Quoted_Label> | <Spaced_Label> \n"
		"<Quoted_Label> ::= -'\"' <Spaced_Label> -'\"' \n"
		"<Spaced_Label> ::= <Spaced_Label_Illegals> {<Spaced_Label_Illegals>} \n"
        "<Unquoted_Label> ::= <Unquoted_Label_Illegals> {<Unquoted_Label_Illegals>} \n"
		"<Spaced_Label_Illegals> ::= (!,:\n\r\t{}\") \n"
		"<Unquoted_Label_Illegals> ::= (! :\n\r\t{}\") \n"

        ;

    //-----------------------------------------------------------------------
    MaterialScriptCompiler::MaterialScriptCompiler(void)
    {
        // set default group resource name
        mScriptContext.groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
    }
    //-----------------------------------------------------------------------
    MaterialScriptCompiler::~MaterialScriptCompiler(void)
    {

    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::setupTokenDefinitions(void)
    {
        addLexemeTokenAction("{", ID_OPENBRACE, &MaterialScriptCompiler::parseOpenBrace);
        addLexemeTokenAction("}", ID_CLOSEBRACE, &MaterialScriptCompiler::parseCloseBrace);
        addLexemeTokenAction("vertex_program", ID_VERTEX_PROGRAM, &MaterialScriptCompiler::parseGPUProgram);
            addLexemeTokenAction("includes_skeletal_animation", ID_INCLUDES_SKELETAL_ANIMATION, &MaterialScriptCompiler::parseProgramSkeletalAnimation);
            addLexemeTokenAction("includes_morph_animation", ID_INCLUDES_MORPH_ANIMATION, &MaterialScriptCompiler::parseProgramMorphAnimation);
            addLexemeTokenAction("includes_pose_animation", ID_INCLUDES_POSE_ANIMATION, &MaterialScriptCompiler::parseProgramPoseAnimation);
        addLexemeTokenAction("fragment_program", ID_FRAGMENT_PROGRAM, &MaterialScriptCompiler::parseGPUProgram);

            addLexemeTokenAction("source", ID_SOURCE, &MaterialScriptCompiler::parseProgramSource);
            addLexemeTokenAction("syntax", ID_SYNTAX, &MaterialScriptCompiler::parseProgramSyntax);
            addLexemeTokenAction("default_params", ID_DEFAULT_PARAMS, &MaterialScriptCompiler::parseDefaultParams);
            addLexemeTokenAction("param_indexed", ID_PARAM_INDEXED, &MaterialScriptCompiler::parseParamIndexed);
            addLexemeTokenAction("param_indexed_auto", ID_PARAM_INDEXED_AUTO, &MaterialScriptCompiler::parseParamIndexedAuto);
            addLexemeTokenAction("param_named", ID_PARAM_NAMED, &MaterialScriptCompiler::parseParamNamed);
            addLexemeTokenAction("param_named_auto", ID_PARAM_NAMED_AUTO, &MaterialScriptCompiler::parseParamNamedAuto);
            addLexemeTokenAction("custom_parameter", ID_CUSTOM_PARAMETER, &MaterialScriptCompiler::parseProgramCustomParameter);

        addLexemeTokenAction("material", ID_MATERIAL, &MaterialScriptCompiler::parseMaterial);
            addLexemeTokenAction(":", ID_CLONE);
            addLexemeTokenAction("lod_distances", ID_LOD_DISTANCES, &MaterialScriptCompiler::parseLodDistances);
            addLexemeTokenAction("receive_shadows", ID_RECEIVE_SHADOWS, &MaterialScriptCompiler::parseReceiveShadows);
            addLexemeTokenAction("transparency_casts_shadows", ID_TRANSPARENCY_CASTS_SHADOWS, &MaterialScriptCompiler::parseTransparencyCastsShadows);
            addLexemeTokenAction("set_texture_alias", ID_SET_TEXTURE_ALIAS, &MaterialScriptCompiler::parseSetTextureAlias);

        // Technique section
        addLexemeTokenAction("technique", ID_TECHNIQUE, &MaterialScriptCompiler::parseTechnique);
            addLexemeTokenAction("scheme", ID_SCHEME, &MaterialScriptCompiler::parseScheme);
            addLexemeTokenAction("lod_index", ID_LOD_INDEX, &MaterialScriptCompiler::parseLodIndex);


        // Pass section
        addLexemeTokenAction("pass", ID_PASS, &MaterialScriptCompiler::parsePass);
            addLexemeTokenAction("ambient", ID_AMBIENT, &MaterialScriptCompiler::parseAmbient);
            addLexemeTokenAction("diffuse", ID_DIFFUSE, &MaterialScriptCompiler::parseDiffuse);
            addLexemeTokenAction("specular", ID_SPECULAR, &MaterialScriptCompiler::parseSpecular);
            addLexemeTokenAction("emissive", ID_EMISSIVE, &MaterialScriptCompiler::parseEmissive);
            addLexemeTokenAction("vertexcolour", ID_VERTEXCOLOUR);

            addLexemeTokenAction("scene_blend", ID_SCENE_BLEND, &MaterialScriptCompiler::parseSceneBlend);
                addLexemeTokenAction("colour_blend", ID_COLOUR_BLEND);
                addLexemeTokenAction("dest_colour", ID_DEST_COLOUR);
                addLexemeTokenAction("src_colour", ID_SRC_COLOUR);
                addLexemeTokenAction("one_minus_dest_colour", ID_ONE_MINUS_DEST_COLOUR);
                addLexemeTokenAction("one_minus_src_colour", ID_ONE_MINUS_SRC_COLOUR);
                addLexemeTokenAction("dest_alpha", ID_DEST_ALPHA);
                addLexemeTokenAction("src_alpha", ID_SRC_ALPHA);
                addLexemeTokenAction("one_minus_dest_alpha", ID_ONE_MINUS_DEST_ALPHA);
                addLexemeTokenAction("one_minus_src_alpha", ID_ONE_MINUS_SRC_ALPHA);

            addLexemeTokenAction("depth_check", ID_DEPTH_CHECK, &MaterialScriptCompiler::parseDepthCheck);
            addLexemeTokenAction("depth_write", ID_DEPTH_WRITE, &MaterialScriptCompiler::parseDepthWrite);
            addLexemeTokenAction("depth_func", ID_DEPTH_FUNC, &MaterialScriptCompiler::parseDepthFunc);
            addLexemeTokenAction("depth_bias", ID_DEPTH_BIAS, &MaterialScriptCompiler::parseDepthBias);
                addLexemeTokenAction("always_fail", ID_ALWAYS_FAIL);
                addLexemeTokenAction("always_pass", ID_ALWAYS_PASS);
                addLexemeTokenAction("less_equal", ID_LESS_EQUAL);
                addLexemeTokenAction("less", ID_LESS);
                addLexemeTokenAction("equal", ID_EQUAL);
                addLexemeTokenAction("not_equal", ID_NOT_EQUAL);
                addLexemeTokenAction("greater_equal", ID_GREATER_EQUAL);
                addLexemeTokenAction("greater", ID_GREATER);
            addLexemeTokenAction("alpha_rejection", ID_ALPHA_REJECTION, &MaterialScriptCompiler::parseAlphaRejection);
            addLexemeTokenAction("cull_hardware", ID_CULL_HARDWARE, &MaterialScriptCompiler::parseCullHardware);
                addLexemeTokenAction("clockwise", ID_CLOCKWISE);
                addLexemeTokenAction("anticlockwise", ID_ANTICLOCKWISE);
            addLexemeTokenAction("cull_software", ID_CULL_SOFTWARE, &MaterialScriptCompiler::parseCullSoftware);
                addLexemeTokenAction("back", ID_CULL_BACK);
                addLexemeTokenAction("front", ID_CULL_FRONT);
            addLexemeTokenAction("lighting", ID_LIGHTING, &MaterialScriptCompiler::parseLighting);
            addLexemeTokenAction("shading", ID_SHADING, &MaterialScriptCompiler::parseShading);
                addLexemeTokenAction("flat", ID_FLAT);
                addLexemeTokenAction("gouraud", ID_GOURAUD);
                addLexemeTokenAction("phong", ID_PHONG);
            addLexemeTokenAction("polygon_mode", ID_POLYGON_MODE, &MaterialScriptCompiler::parsePolygonMode);
                addLexemeTokenAction("solid", ID_SOLID);
                addLexemeTokenAction("wireframe", ID_WIREFRAME);
                addLexemeTokenAction("points", ID_POINTS);
            addLexemeTokenAction("fog_override", ID_FOG_OVERRIDE, &MaterialScriptCompiler::parseFogOverride);
                addLexemeTokenAction("exp", ID_EXP);
                addLexemeTokenAction("exp2", ID_EXP2);
            addLexemeTokenAction("colour_write", ID_COLOUR_WRITE, &MaterialScriptCompiler::parseColourWrite);
            addLexemeTokenAction("max_lights", ID_MAX_LIGHTS, &MaterialScriptCompiler::parseMaxLights);
            addLexemeTokenAction("iteration", ID_ITERATION, &MaterialScriptCompiler::parseIteration);
                addLexemeTokenAction("once", ID_ONCE);
                addLexemeTokenAction("once_per_light", ID_ONCE_PER_LIGHT);
                addLexemeTokenAction("per_light", ID_PER_LIGHT);
                addLexemeTokenAction("directional", ID_DIRECTIONAL);
                addLexemeTokenAction("spot", ID_SPOT);
            addLexemeTokenAction("point_size", ID_POINT_SIZE, &MaterialScriptCompiler::parsePointSize);
            addLexemeTokenAction("point_sprites", ID_POINT_SPRITES, &MaterialScriptCompiler::parsePointSprites);
            addLexemeTokenAction("point_size_attenuation", ID_POINT_SIZE_ATTENUATION, &MaterialScriptCompiler::parsePointSizeAttenuation);
            addLexemeTokenAction("point_size_min", ID_POINT_SIZE_MIN, &MaterialScriptCompiler::parsePointSizeMin);
            addLexemeTokenAction("point_size_max", ID_POINT_SIZE_MAX, &MaterialScriptCompiler::parsePointSizeMax);

        // Texture Unit section
        addLexemeTokenAction("texture_unit", ID_TEXTURE_UNIT, &MaterialScriptCompiler::parseTextureUnit);
        addLexemeTokenAction("texture_alias", ID_TEXTURE_ALIAS, &MaterialScriptCompiler::parseTextureAlias);
        addLexemeTokenAction("texture", ID_TEXTURE, &MaterialScriptCompiler::parseTexture);
            addLexemeTokenAction("1d", ID_1D);
            addLexemeTokenAction("2d", ID_2D);
            addLexemeTokenAction("3d", ID_3D);
            addLexemeTokenAction("cubic", ID_CUBIC);
            addLexemeTokenAction("unlimited", ID_UNLIMITED);
            addLexemeTokenAction("alpha", ID_ALPHA);
        addLexemeTokenAction("anim_texture", ID_ANIM_TEXTURE, &MaterialScriptCompiler::parseAnimTexture);
        addLexemeTokenAction("cubic_texture", ID_CUBIC_TEXTURE, &MaterialScriptCompiler::parseCubicTexture);
            addLexemeTokenAction("separateuv", ID_SEPARATE_UV);
            addLexemeTokenAction("combineduvw", ID_COMBINED_UVW);
        addLexemeTokenAction("tex_coord_set", ID_TEX_COORD_SET, &MaterialScriptCompiler::parseTexCoord);
        addLexemeTokenAction("tex_address_mode", ID_TEX_ADDRESS_MODE, &MaterialScriptCompiler::parseTexAddressMode);
            addLexemeTokenAction("wrap", ID_WRAP);
            addLexemeTokenAction("clamp", ID_CLAMP);
            addLexemeTokenAction("mirror", ID_MIRROR);
            addLexemeTokenAction("border", ID_BORDER);
        addLexemeTokenAction("tex_border_colour", ID_TEX_BORDER_COLOUR, &MaterialScriptCompiler::parseTexBorderColour);
        addLexemeTokenAction("filtering", ID_FILTERING, &MaterialScriptCompiler::parseFiltering);
            addLexemeTokenAction("bilinear", ID_BILINEAR);
            addLexemeTokenAction("trilinear", ID_TRILINEAR);
            addLexemeTokenAction("anisotropic", ID_ANISOTROPIC);
        addLexemeTokenAction("max_anisotropy", ID_MAX_ANISOTROPY, &MaterialScriptCompiler::parseMaxAnisotropy);
        addLexemeTokenAction("colour_op", ID_COLOUR_OP, &MaterialScriptCompiler::parseColourOp);
            addLexemeTokenAction("replace", ID_REPLACE);
        addLexemeTokenAction("colour_op_ex", ID_COLOUR_OP_EX, &MaterialScriptCompiler::parseColourOpEx);
            addLexemeTokenAction("source1", ID_SOURCE1);
            addLexemeTokenAction("source2", ID_SOURCE2);
            addLexemeTokenAction("modulate_x2", ID_MODULATE_X2);
            addLexemeTokenAction("modulate_x4", ID_MODULATE_X4);
            addLexemeTokenAction("add_signed", ID_ADD_SIGNED);
            addLexemeTokenAction("add_smooth", ID_ADD_SMOOTH);
            addLexemeTokenAction("subtract", ID_SUBTRACT);
            addLexemeTokenAction("blend_diffuse_colour", ID_BLEND_DIFFUSE_COLOUR);
            addLexemeTokenAction("blend_diffuse_alpha", ID_BLEND_DIFFUSE_ALPHA);
            addLexemeTokenAction("blend_manual", ID_BLEND_MANUAL);
            addLexemeTokenAction("dotproduct", ID_DOTPRODUCT);
            addLexemeTokenAction("src_current", ID_SRC_CURRENT);
            addLexemeTokenAction("src_texture", ID_SRC_TEXTURE);
            addLexemeTokenAction("src_diffuse", ID_SRC_DIFFUSE);
            addLexemeTokenAction("src_specular", ID_SRC_SPECULAR);
            addLexemeTokenAction("src_manual", ID_SRC_MANUAL);
        addLexemeTokenAction("colour_op_multipass_fallback", ID_COLOUR_OP_MULTIPASS_FALLBACK,
            &MaterialScriptCompiler::parseColourOpMultipassFallback);
        addLexemeTokenAction("alpha_op_ex", ID_ALPHA_OP_EX, &MaterialScriptCompiler::parseAlphaOpEx);
        addLexemeTokenAction("env_map", ID_ENV_MAP, &MaterialScriptCompiler::parseEnvMap);
            addLexemeTokenAction("spherical", ID_SPHERICAL);
            addLexemeTokenAction("planar", ID_PLANAR);
            addLexemeTokenAction("cubic_reflection", ID_CUBIC_REFLECTION);
            addLexemeTokenAction("cubic_normal", ID_CUBIC_NORMAL);
        addLexemeTokenAction("scroll", ID_SCROLL, &MaterialScriptCompiler::parseScroll);
        addLexemeTokenAction("scroll_anim", ID_SCROLL_ANIM, &MaterialScriptCompiler::parseScrollAnim);
        addLexemeTokenAction("rotate", ID_ROTATE, &MaterialScriptCompiler::parseRotate);
        addLexemeTokenAction("rotate_anim", ID_ROTATE_ANIM, &MaterialScriptCompiler::parseRotateAnim);
        addLexemeTokenAction("scale", ID_SCALE, &MaterialScriptCompiler::parseScale);
        addLexemeTokenAction("wave_xform", ID_WAVE_XFORM, &MaterialScriptCompiler::parseWaveXform);
            addLexemeTokenAction("scroll_x", ID_SCROLL_X);
            addLexemeTokenAction("scroll_y", ID_SCROLL_Y);
            addLexemeTokenAction("scale_x", ID_SCALE_X);
            addLexemeTokenAction("scale_y", ID_SCALE_Y);
            addLexemeTokenAction("sine", ID_SINE);
            addLexemeTokenAction("triangle", ID_TRIANGLE);
            addLexemeTokenAction("square", ID_SQUARE);
            addLexemeTokenAction("sawtooth", ID_SAWTOOTH);
            addLexemeTokenAction("inverse_sawtooth", ID_INVERSE_SAWTOOTH);
        addLexemeTokenAction("transform", ID_TRANSFORM, &MaterialScriptCompiler::parseTransform);
        // GPU program reference
        addLexemeTokenAction("vertex_program_ref", ID_VERTEX_PROGRAM_REF,
            &MaterialScriptCompiler::parseVertexProgramRef);
        addLexemeTokenAction("fragment_program_ref", ID_FRAGMENT_PROGRAM_REF,
            &MaterialScriptCompiler::parseFragmentProgramRef);
        addLexemeTokenAction("shadow_caster_vertex_program_ref", ID_SHADOW_CASTER_VERTEX_PROGRAM_REF,
            &MaterialScriptCompiler::parseShadowCasterVertexProgramRef);
        addLexemeTokenAction("shadow_receiver_vertex_program_ref", ID_SHADOW_RECEIVER_VERTEX_PROGRAM_REF,
            &MaterialScriptCompiler::parseShadowReceiverVertexProgramRef);
        addLexemeTokenAction("shadow_receiver_fragment_program_ref", ID_SHADOW_RECEIVER_FRAGMENT_PROGRAM_REF,
            &MaterialScriptCompiler::parseShadowReceiverFragmentProgramRef);

        // common section
        addLexemeTokenAction("on", ID_ON);
        addLexemeTokenAction("off", ID_OFF);
        addLexemeTokenAction("true", ID_TRUE);
        addLexemeTokenAction("false", ID_FALSE);
        addLexemeTokenAction("none", ID_NONE);
        addLexemeTokenAction("point", ID_POINT);
        addLexemeTokenAction("linear", ID_LINEAR);
        addLexemeTokenAction("add", ID_ADD);
        addLexemeTokenAction("modulate", ID_MODULATE);
        addLexemeTokenAction("alpha_blend", ID_ALPHA_BLEND);
        addLexemeTokenAction("one", ID_ONE);
        addLexemeTokenAction("zero", ID_ZERO);


    }

    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::addLexemeTokenAction(const String& lexeme, const size_t token, const MSC_Action action)
    {
        addLexemeToken(lexeme, token, action != 0);
        mTokenActionMap[token] = action;
    }

    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::executeTokenAction(const size_t tokenID)
    {
        TokenActionIterator action = mTokenActionMap.find(tokenID);

        if (action == mTokenActionMap.end())
        {
            // BAD command. BAD!
            logParseError("Unrecognised Material Script command action");
            return;
        }
        else
        {
            try
            {
                (this->*action->second)();
            }
            catch (Exception& ogreException)
            {
                // an unknown token found or BNF Grammer rule was not successful
                // in finding a valid terminal token to complete the rule expression.
                logParseError(ogreException.getDescription());
            }
        }
    }

    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::logParseError(const String& error)
    {
        // log material name only if filename not specified
        if (mSourceName.empty() && !mScriptContext.material.isNull())
        {
            LogManager::getSingleton().logMessage(
                "Error in material " + mScriptContext.material->getName() +
                " : " + error);
        }
        else
        {
            if (!mScriptContext.material.isNull())
            {
                LogManager::getSingleton().logMessage(
                    "Error in material " + mScriptContext.material->getName() +
                    " at line " + StringConverter::toString(mCurrentLine) +
                    " of " + mSourceName + ": " + error);
            }
            else
            {
                LogManager::getSingleton().logMessage(
                    "Error at line " + StringConverter::toString(mCurrentLine) +
                    " of " + mSourceName + ": " + error);
            }
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseOpenBrace(void)
    {

    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseCloseBrace(void)
    {
        switch(mScriptContext.section)
        {
        case MSS_NONE:
            logParseError("Unexpected terminating brace.");
            break;
        case MSS_MATERIAL:
            // End of material
            // if texture aliases were found, pass them to the material
            // to update texture names used in Texture unit states
            if (!mScriptContext.textureAliases.empty())
            {
                // request material to update all texture names in TUS's
                // that use texture aliases in the list
                mScriptContext.material->applyTextureAliases(mScriptContext.textureAliases);
            }

            mScriptContext.section = MSS_NONE;
            mScriptContext.material.setNull();
			//Reset all levels for next material
			mScriptContext.passLev = -1;
			mScriptContext.stateLev= -1;
			mScriptContext.techLev = -1;
            mScriptContext.textureAliases.clear();
            break;
        case MSS_TECHNIQUE:
            // End of technique
            mScriptContext.section = MSS_MATERIAL;
            mScriptContext.technique = NULL;
			mScriptContext.passLev = -1;	//Reset pass level (yes, the pass level)
            break;
        case MSS_PASS:
            // End of pass
            mScriptContext.section = MSS_TECHNIQUE;
            mScriptContext.pass = NULL;
			mScriptContext.stateLev = -1;	//Reset state level (yes, the state level)
            break;
        case MSS_TEXTUREUNIT:
            // End of texture unit
            mScriptContext.section = MSS_PASS;
            mScriptContext.textureUnit = NULL;
            break;
		case MSS_TEXTURESOURCE:
			//End texture source section
			//Finish creating texture here

			if(	ExternalTextureSourceManager::getSingleton().getCurrentPlugIn() != 0)
            {
                const String sMaterialName = mScriptContext.material->getName();
				ExternalTextureSourceManager::getSingleton().getCurrentPlugIn()->
				createDefinedTexture( sMaterialName, mScriptContext.groupName );
            }
			//Revert back to texture unit
			mScriptContext.section = MSS_TEXTUREUNIT;
			break;
        case MSS_PROGRAM_REF:
            // End of program
            mScriptContext.section = MSS_PASS;
            mScriptContext.program.setNull();
            break;
        case MSS_PROGRAM:
			// Program definitions are slightly different, they are deferred
			// until all the information required is known
            // End of program
			finishProgramDefinition();
            mScriptContext.section = MSS_NONE;
            delete mScriptContext.programDef;
            mScriptContext.pendingDefaultParams.clear();
            mScriptContext.programDef = NULL;
            break;
        case MSS_DEFAULT_PARAMETERS:
            // End of default parameters
            mScriptContext.section = MSS_PROGRAM;
            break;
        };
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseGPUProgram(void)
    {
        // update section
        mScriptContext.section = MSS_PROGRAM;

		// Create new program definition-in-progress
		mScriptContext.programDef = new MaterialScriptProgramDefinition();
		mScriptContext.programDef->progType =
            (getCurrentTokenID() == ID_VERTEX_PROGRAM) ? GPT_VERTEX_PROGRAM : GPT_FRAGMENT_PROGRAM;
        mScriptContext.programDef->supportsSkeletalAnimation = false;
		mScriptContext.programDef->supportsMorphAnimation = false;
		mScriptContext.programDef->supportsPoseAnimation = 0;

		// Get name and language code
		// Name, preserve case
		mScriptContext.programDef->name = getNextTokenLabel();
		// trim trailing white space only
		StringUtil::trim(mScriptContext.programDef->name);
		// language code
		mScriptContext.programDef->language = getNextTokenLabel();
		// make sure language is lower case
		StringUtil::toLowerCase(mScriptContext.programDef->language);
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseProgramSource(void)
    {
        assert(mScriptContext.programDef);
		mScriptContext.programDef->source = getNextTokenLabel();
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseProgramSyntax(void)
    {
        assert(mScriptContext.programDef);
		mScriptContext.programDef->syntax = getNextTokenLabel();
		// make sure language is lower case
		StringUtil::toLowerCase(mScriptContext.programDef->syntax);
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseProgramSkeletalAnimation(void)
    {
        assert(mScriptContext.programDef);
        mScriptContext.programDef->supportsSkeletalAnimation = testNextTokenID(ID_TRUE);
    }
	//-----------------------------------------------------------------------
	void MaterialScriptCompiler::parseProgramMorphAnimation(void)
	{
        assert(mScriptContext.programDef);
		mScriptContext.programDef->supportsMorphAnimation = testNextTokenID(ID_TRUE);
	}
	//-----------------------------------------------------------------------
	void MaterialScriptCompiler::parseProgramPoseAnimation(void)
	{
        assert(mScriptContext.programDef);
		mScriptContext.programDef->supportsPoseAnimation = static_cast<ushort>(getNextTokenValue());
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseProgramCustomParameter(void)
    {
        assert(mScriptContext.programDef);

        String command = getNextTokenLabel();
		StringUtil::toLowerCase(command);
        String params = getNextTokenLabel();
        StringUtil::trim(params);
		mScriptContext.programDef->customParameters[command] = params;
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseDefaultParams(void)
    {
        mScriptContext.section = MSS_DEFAULT_PARAMETERS;
    }
	//-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseMaterial(void)
    {
        // check params for reference to parent material to copy from
        // syntax: material name : parentMaterialName
        MaterialPtr basematerial;

        String materialName = getNextTokenLabel();
        StringUtil::trim(materialName);
        // Create a brand new material
        const size_t paramCount = getRemainingTokensForAction();
        if (paramCount == 2)
        {
            // this gets the ':' token which we need to consume to get to the label
            getNextToken();
            // if a second parameter exists then assume its the name of the base material
            // that this new material should clone from
            String parentName = getNextTokenLabel();
            StringUtil::trim(parentName);
            // make sure base material exists
            basematerial = MaterialManager::getSingleton().getByName(parentName);
            // if it doesn't exist then report error in log and just create a new material
            if (basematerial.isNull())
            {
                logParseError("parent material: " + parentName + " not found for new material:"
                    + materialName);
            }
        }

        mScriptContext.material =
			MaterialManager::getSingleton().create(materialName, mScriptContext.groupName);

        if (!basematerial.isNull())
        {
            // copy parent material details to new material
            basematerial->copyDetailsTo(mScriptContext.material);
        }
        else
        {
            // Remove pre-created technique from defaults
            mScriptContext.material->removeAllTechniques();
        }

		mScriptContext.material->_notifyOrigin(mSourceName);

        // update section
        mScriptContext.section = MSS_MATERIAL;

    }
    //-----------------------------------------------------------------------
    // material Section Actions
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseLodDistances(void)
    {
        // iterate over the parameters and parse distances out of them
        Material::LodDistanceList lodList;
		while (getRemainingTokensForAction() > 0)
		{
            lodList.push_back(getNextTokenValue());
        }

        mScriptContext.material->setLodLevels(lodList);
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseReceiveShadows(void)
    {
        mScriptContext.material->setReceiveShadows(testNextTokenID(ID_ON));
    }
	//-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseTransparencyCastsShadows(void)
	{
		mScriptContext.material->setTransparencyCastsShadows(testNextTokenID(ID_ON));
	}
	//-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseSetTextureAlias(void)
	{
	    const String& aliasName = getNextTokenLabel();
	    const String& textureName = getNextTokenLabel();
		mScriptContext.textureAliases[aliasName] = textureName;
	}
	//-----------------------------------------------------------------------
	// Technique section Actions
	//-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseTechnique(void)
    {
        String techniqueName;
        if (getRemainingTokensForAction() > 0)
            techniqueName = getNextTokenLabel();
        // if params is not empty then see if the technique name already exists
        if (!techniqueName.empty() && (mScriptContext.material->getNumTechniques() > 0))
        {
            Technique* foundTechnique = mScriptContext.material->getTechnique(techniqueName);
            if (foundTechnique)
            {
                // figure out technique index by iterating through technique container
                // would be nice if each technique remembered its index
                int count = 0;
                Material::TechniqueIterator i = mScriptContext.material->getTechniqueIterator();
                while(i.hasMoreElements())
                {
                    if (foundTechnique == i.peekNext())
                        break;
                    i.moveNext();
                    ++count;
                }

                mScriptContext.techLev = count;
            }
            else
            {
                // name was not found so a new technique is needed
                // position technique level to the end index
                // a new technique will be created later on
                mScriptContext.techLev = mScriptContext.material->getNumTechniques();
            }

        }
        else
        {
            // no name was given in the script so a new technique will be created
		    // Increase technique level depth
		    ++mScriptContext.techLev;
        }

        // Create a new technique if it doesn't already exist
        if (mScriptContext.material->getNumTechniques() > mScriptContext.techLev)
        {
            mScriptContext.technique = mScriptContext.material->getTechnique(mScriptContext.techLev);
        }
        else
        {
            mScriptContext.technique = mScriptContext.material->createTechnique();
            if (!techniqueName.empty())
                mScriptContext.technique->setName(techniqueName);
        }

        // update section
        mScriptContext.section = MSS_TECHNIQUE;

    }
	//-----------------------------------------------------------------------
	void MaterialScriptCompiler::parseScheme(void)
	{
	    assert(mScriptContext.technique);
		mScriptContext.technique->setSchemeName(getNextTokenLabel());
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseLodIndex(void)
    {
	    assert(mScriptContext.technique);
        mScriptContext.technique->setLodIndex(static_cast<uint>(getNextTokenValue()));
    }
	//-----------------------------------------------------------------------
	// Pass Section Actions
	//-----------------------------------------------------------------------
    void MaterialScriptCompiler::parsePass(void)
    {
        String passName;
        if (getRemainingTokensForAction() > 0)
            passName = getNextTokenLabel();
        // if params is not empty then see if the pass name already exists
        if (!passName.empty() && (mScriptContext.technique->getNumPasses() > 0))
        {
            Pass* foundPass = mScriptContext.technique->getPass(passName);
            if (foundPass)
            {
                mScriptContext.passLev = foundPass->getIndex();
            }
            else
            {
                // name was not found so a new pass is needed
                // position pass level to the end index
                // a new pass will be created later on
                mScriptContext.passLev = mScriptContext.technique->getNumPasses();
            }

        }
        else
        {
		    //Increase pass level depth
		    ++mScriptContext.passLev;
        }

        if (mScriptContext.technique->getNumPasses() > mScriptContext.passLev)
        {
            mScriptContext.pass = mScriptContext.technique->getPass(mScriptContext.passLev);
        }
        else
        {
            // Create a new pass
            mScriptContext.pass = mScriptContext.technique->createPass();
            if (!passName.empty())
                mScriptContext.pass->setName(passName);
        }

        // update section
        mScriptContext.section = MSS_PASS;
    }
    //-----------------------------------------------------------------------
    ColourValue MaterialScriptCompiler::_parseColourValue(void)
    {
        Real r = getNextTokenValue();
        Real g = getNextTokenValue();
        Real b = getNextTokenValue();
        Real a = getRemainingTokensForAction() == 1 ? getNextTokenValue() : 1.0f;
        return ColourValue(r, g, b, a);
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseAmbient(void)
    {
        // Must be 1, 3 or 4 parameters
        assert(mScriptContext.pass);
        const size_t paramCount = getRemainingTokensForAction();
        if (paramCount == 1) {
            if(testNextTokenID(ID_VERTEXCOLOUR))
            {
                mScriptContext.pass->setVertexColourTracking(mScriptContext.pass->getVertexColourTracking() | TVC_AMBIENT);
            }
            else
            {
                logParseError("Bad ambient attribute, single parameter flag must be 'vertexcolour'");
            }
        }
        else if (paramCount == 3 || paramCount == 4)
        {
            mScriptContext.pass->setAmbient( _parseColourValue() );
            mScriptContext.pass->setVertexColourTracking(mScriptContext.pass->getVertexColourTracking() & ~TVC_AMBIENT);
        }
        else
        {
            logParseError("Bad ambient attribute, wrong number of parameters (expected 1, 3 or 4)");
        }
    }
   //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseDiffuse(void)
    {
        // Must be 1, 3 or 4 parameters
        assert(mScriptContext.pass);
        const size_t paramCount = getRemainingTokensForAction();
        if (paramCount == 1) {
            if(testNextTokenID(ID_VERTEXCOLOUR))
            {
               mScriptContext.pass->setVertexColourTracking(mScriptContext.pass->getVertexColourTracking() | TVC_DIFFUSE);
            }
            else
            {
                logParseError("Bad diffuse attribute, single parameter flag must be 'vertexcolour'");
            }
        }
        else if (paramCount == 3 || paramCount == 4)
        {
            mScriptContext.pass->setDiffuse( _parseColourValue() );
            mScriptContext.pass->setVertexColourTracking(mScriptContext.pass->getVertexColourTracking() & ~TVC_DIFFUSE);
        }
        else
        {
            logParseError("Bad diffuse attribute, wrong number of parameters (expected 1, 3 or 4)");
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseSpecular(void)
    {
        // Must be 2, 4 or 5 parameters
        assert(mScriptContext.pass);
        const size_t paramCount = getRemainingTokensForAction();
        if(paramCount == 2)
        {
            if(getNextTokenID() == ID_VERTEXCOLOUR)
            {
                mScriptContext.pass->setVertexColourTracking(mScriptContext.pass->getVertexColourTracking() | TVC_SPECULAR);
                mScriptContext.pass->setShininess(getNextTokenValue());
            }
            else
            {
                logParseError("Bad specular attribute, double parameter statement must be 'vertexcolour <shininess>'");
            }
        }
        else if(paramCount == 4 || paramCount == 5)
        {
            Real r = getNextTokenValue();
            Real g = getNextTokenValue();
            Real b = getNextTokenValue();
            Real a = paramCount == 5 ? getNextTokenValue() : 1.0f;
            mScriptContext.pass->setSpecular(r, g, b, a);
            mScriptContext.pass->setVertexColourTracking(mScriptContext.pass->getVertexColourTracking() & ~TVC_SPECULAR);
            mScriptContext.pass->setShininess( getNextTokenValue() );
        }
        else
        {
            logParseError("Bad specular attribute, wrong number of parameters (expected 2, 4 or 5)");
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseEmissive(void)
    {
        // Must be 1, 3 or 4 parameters
        assert(mScriptContext.pass);
        const size_t paramCount = getRemainingTokensForAction();
        if (paramCount == 1) {
            if(testNextTokenID(ID_VERTEXCOLOUR))
            {
               mScriptContext.pass->setVertexColourTracking(mScriptContext.pass->getVertexColourTracking() | TVC_EMISSIVE);
            }
            else
            {
                logParseError("Bad emissive attribute, single parameter flag must be 'vertexcolour'");
            }
        }
        else if (paramCount == 3 || paramCount == 4)
        {
            mScriptContext.pass->setSelfIllumination( _parseColourValue() );
            mScriptContext.pass->setVertexColourTracking(mScriptContext.pass->getVertexColourTracking() & ~TVC_EMISSIVE);
        }
        else
        {
            logParseError("Bad emissive attribute, wrong number of parameters (expected 1, 3 or 4)");
        }
    }
    //-----------------------------------------------------------------------
    SceneBlendFactor MaterialScriptCompiler::convertBlendFactor(void)
    {
        switch(getNextTokenID())
        {
        case ID_ONE:
            return SBF_ONE;
        case ID_ZERO:
            return SBF_ZERO;
        case ID_DEST_COLOUR:
            return SBF_DEST_COLOUR;
        case ID_SRC_COLOUR:
            return SBF_SOURCE_COLOUR;
        case ID_ONE_MINUS_DEST_COLOUR:
            return SBF_ONE_MINUS_DEST_COLOUR;
        case ID_ONE_MINUS_SRC_COLOUR:
            return SBF_ONE_MINUS_SOURCE_COLOUR;
        case ID_DEST_ALPHA:
            return SBF_DEST_ALPHA;
        case ID_SRC_ALPHA:
            return SBF_SOURCE_ALPHA;
        case ID_ONE_MINUS_DEST_ALPHA:
            return SBF_ONE_MINUS_DEST_ALPHA;
        case ID_ONE_MINUS_SRC_ALPHA:
            return SBF_ONE_MINUS_SOURCE_ALPHA;
        default:
            return SBF_ONE;
            break;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseSceneBlend(void)
    {
        assert(mScriptContext.pass);
        const size_t paramCount = getRemainingTokensForAction();
        // Should be 1 or 2 params
        if (paramCount == 1)
        {
            //simple blend types
            SceneBlendType sbtype = SBT_REPLACE;
            switch(getNextTokenID())
            {
            case ID_ADD:
                sbtype = SBT_ADD;
                break;
            case ID_MODULATE:
                sbtype = SBT_MODULATE;
                break;
			case ID_COLOUR_BLEND:
				sbtype = SBT_TRANSPARENT_COLOUR;
				break;
            case ID_ALPHA_BLEND:
                sbtype = SBT_TRANSPARENT_ALPHA;
                break;
            default:
                break;
            }
            mScriptContext.pass->setSceneBlending(sbtype);

        }
        else if (paramCount == 2)
        {
            const SceneBlendFactor src = convertBlendFactor();
            const SceneBlendFactor dest = convertBlendFactor();
            mScriptContext.pass->setSceneBlending(src,dest);
        }
        else
        {
            logParseError(
                "Bad scene_blend attribute, wrong number of parameters (expected 1 or 2)");
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseDepthCheck(void)
    {
        assert(mScriptContext.pass);
        mScriptContext.pass->setDepthCheckEnabled(testNextTokenID(ID_ON));
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseDepthWrite(void)
    {
        assert(mScriptContext.pass);
        mScriptContext.pass->setDepthWriteEnabled(testNextTokenID(ID_ON));
    }
    //-----------------------------------------------------------------------
    CompareFunction MaterialScriptCompiler::convertCompareFunction(void)
    {
        switch (getNextTokenID())
        {
        case ID_ALWAYS_FAIL:
            return CMPF_ALWAYS_FAIL;
        case ID_ALWAYS_PASS:
            return CMPF_ALWAYS_PASS;
        case ID_LESS:
            return CMPF_LESS;
        case ID_LESS_EQUAL:
            return CMPF_LESS_EQUAL;
        case ID_EQUAL:
            return CMPF_EQUAL;
        case ID_NOT_EQUAL:
            return CMPF_NOT_EQUAL;
        case ID_GREATER_EQUAL:
            return CMPF_GREATER_EQUAL;
        case ID_GREATER:
            return CMPF_GREATER;
        default:
            return CMPF_LESS_EQUAL;
            break;
        }
    }

    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseDepthFunc(void)
    {
        assert(mScriptContext.pass);
        mScriptContext.pass->setDepthFunction(convertCompareFunction());
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseDepthBias(void)
    {
        assert(mScriptContext.pass);
        mScriptContext.pass->setDepthBias(static_cast<ushort>(getNextTokenValue()));
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseAlphaRejection(void)
    {
        assert(mScriptContext.pass);
        const CompareFunction cmp = convertCompareFunction();
        mScriptContext.pass->setAlphaRejectSettings(cmp, static_cast<unsigned char>(getNextTokenValue()));
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseCullHardware(void)
    {
        assert(mScriptContext.pass);
        switch (getNextTokenID())
        {
        case ID_NONE:
            mScriptContext.pass->setCullingMode(CULL_NONE);
            break;
        case ID_ANTICLOCKWISE:
            mScriptContext.pass->setCullingMode(CULL_ANTICLOCKWISE);
            break;
        case ID_CLOCKWISE:
            mScriptContext.pass->setCullingMode(CULL_CLOCKWISE);
            break;
        default:
            break;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseCullSoftware(void)
    {
        assert(mScriptContext.pass);
        switch (getNextTokenID())
        {
        case ID_NONE:
            mScriptContext.pass->setManualCullingMode(MANUAL_CULL_NONE);
            break;
        case ID_CULL_BACK:
            mScriptContext.pass->setManualCullingMode(MANUAL_CULL_BACK);
            break;
        case ID_CULL_FRONT:
            mScriptContext.pass->setManualCullingMode(MANUAL_CULL_FRONT);
            break;
        default:
            break;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseLighting(void)
    {
        assert(mScriptContext.pass);
        mScriptContext.pass->setLightingEnabled(testNextTokenID(ID_ON));
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseShading(void)
    {
        assert(mScriptContext.pass);
        switch (getNextTokenID())
        {
        case ID_FLAT:
            mScriptContext.pass->setShadingMode(SO_FLAT);
            break;
        case ID_GOURAUD:
            mScriptContext.pass->setShadingMode(SO_GOURAUD);
            break;
        case ID_PHONG:
            mScriptContext.pass->setShadingMode(SO_PHONG);
            break;
        default:
            break;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parsePolygonMode(void)
    {
        assert(mScriptContext.pass);
        switch (getNextTokenID())
        {
        case ID_SOLID:
            mScriptContext.pass->setPolygonMode(PM_SOLID);
            break;
        case ID_WIREFRAME:
            mScriptContext.pass->setPolygonMode(PM_WIREFRAME);
            break;
        case ID_POINTS:
            mScriptContext.pass->setPolygonMode(PM_POINTS);
            break;
        default:
            break;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseFogOverride(void)
    {
        assert(mScriptContext.pass);
        if (getNextTokenID() == ID_TRUE)
        {
            // if true, we need to see if they supplied all arguments, or just the 1... if just the one,
            // Assume they want to disable the default fog from effecting this material.
            const size_t paramCount = getRemainingTokensForAction();
            if( paramCount == 7 )
            {
                FogMode fogtype;
                switch (getNextTokenID())
                {
                case ID_LINEAR:
                    fogtype = FOG_LINEAR;
                case ID_EXP:
                    fogtype = FOG_EXP;
                case ID_EXP2:
                    fogtype = FOG_EXP2;
                case ID_NONE:
                default:
                    fogtype = FOG_NONE;
                    break;
                }

                const Real red = getNextTokenValue();
                const Real green = getNextTokenValue();
                const Real blue = getNextTokenValue();
                const Real density = getNextTokenValue();
                const Real start = getNextTokenValue();
                const Real end = getNextTokenValue();

                mScriptContext.pass->setFog(
                    true,
                    fogtype,
                    ColourValue(red, green, blue),
                    density, start, end
                    );
            }
            else
            {
                mScriptContext.pass->setFog(true);
            }
        }
        else
            mScriptContext.pass->setFog(false);

    }
   //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseColourWrite(void)
    {
        assert(mScriptContext.pass);
        mScriptContext.pass->setColourWriteEnabled(testNextTokenID(ID_ON));
    }
     //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseMaxLights(void)
    {
        assert(mScriptContext.pass);
		mScriptContext.pass->setMaxSimultaneousLights(static_cast<int>(getNextTokenValue()));
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseIterationLightTypes(void)
    {
        assert(mScriptContext.pass);
        // Parse light type
        switch(getNextTokenID())
        {
        case ID_DIRECTIONAL:
            mScriptContext.pass->setIteratePerLight(true, true, Light::LT_DIRECTIONAL);
            break;
        case ID_POINT:
            mScriptContext.pass->setIteratePerLight(true, true, Light::LT_POINT);
            break;
        case ID_SPOT:
            mScriptContext.pass->setIteratePerLight(true, true, Light::LT_SPOTLIGHT);
            break;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseIteration(void)
    {
        assert(mScriptContext.pass);
        // we could have more than one parameter
        /** combinations could be:
            iteration once
            iteration once_per_light [light type]
            iteration <number>
            iteration <number> [per_light] [light type]
        */
        if (testNextTokenID(ID_ONCE))
            mScriptContext.pass->setIteratePerLight(false);
        else if (testNextTokenID(ID_ONCE_PER_LIGHT))
        {
            getNextToken();
            if (getRemainingTokensForAction() == 1)
            {
                parseIterationLightTypes();
            }
            else
            {
                mScriptContext.pass->setIteratePerLight(true, false);
            }

        }
        else // could be using form: <number> [per_light] [light type]
        {
            uint passIterationCount = static_cast<uint>(getNextTokenValue());
            if (passIterationCount > 0)
            {
                mScriptContext.pass->setPassIterationCount(passIterationCount);
                if (getRemainingTokensForAction() > 1)
                {
                    if (getNextTokenID() == ID_PER_LIGHT)
                    {
                        if (getRemainingTokensForAction() == 1)
                        {
                            parseIterationLightTypes();
                        }
                        else
                        {
                            mScriptContext.pass->setIteratePerLight(true, false);
                        }
                    }
                    else
                        logParseError(
                            "Bad iteration attribute, valid parameters are <number> [per_light] [light type].");
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parsePointSize(void)
    {
        mScriptContext.pass->setPointSize(getNextTokenValue());
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parsePointSprites(void)
	{
        assert(mScriptContext.pass);
        mScriptContext.pass->setPointSpritesEnabled(testNextTokenID(ID_ON));
	}
    //-----------------------------------------------------------------------
	void MaterialScriptCompiler::parsePointSizeMin(void)
	{
        assert(mScriptContext.pass);
        mScriptContext.pass->setPointMinSize(getNextTokenValue());
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parsePointSizeMax(void)
	{
        assert(mScriptContext.pass);
        mScriptContext.pass->setPointMaxSize(getNextTokenValue());
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parsePointSizeAttenuation(void)
	{
        assert(mScriptContext.pass);
        const size_t paramCount = getRemainingTokensForAction();
        if (paramCount != 1 && paramCount != 4)
        {
            logParseError("Bad point_size_attenuation attribute, wrong number of parameters (expected 1 or 4)");
            return;
        }
        switch (getNextTokenID())
        {
        case ID_ON:
			if (paramCount == 4)
			{
				Real constant = getNextTokenValue();
				Real linear = getNextTokenValue();
				Real quadric = getNextTokenValue();
	            mScriptContext.pass->setPointAttenuation(true, constant, linear, quadric);
			}
			else
			{
				mScriptContext.pass->setPointAttenuation(true);
			}

            break;
        case ID_OFF:
            mScriptContext.pass->setPointAttenuation(false);
            break;
        default:
            logParseError("Bad point_size_attenuation attribute, valid values are 'on' or 'off'.");
        }
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseTextureCustomParameter(void)
    {
		// This params object does not have the command stripped
		// Split only up to first delimiter, program deals with the rest

		if (getRemainingTokensForAction() != 2)
		{
            logParseError("Invalid texture parameter entry; "
				"there must be a parameter name and at least one value.");
		}

		else if( ExternalTextureSourceManager::getSingleton().getCurrentPlugIn() != 0 )
        {
			////First is command, next could be a string with one or more values
            const String& param1 = getNextTokenLabel();
            const String& param2 = getNextTokenLabel();
			ExternalTextureSourceManager::getSingleton().getCurrentPlugIn()->setParameter( param1, param2 );
        }
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseTextureUnit(void)
    {
        String tusName;
        if (getRemainingTokensForAction() > 0)
            tusName = getNextTokenLabel();
        // if params is a name then see if that texture unit exists
        // if not then log the warning and just move on to the next TU from current
        if (!tusName.empty() && (mScriptContext.pass->getNumTextureUnitStates() > 0))
        {
            // specifying a TUS name in the script for a TU means that a specific TU is being requested
            // try to get the specific TU
            // if the index requested is not valid, just creat a new TU
            // find the TUS with name
            TextureUnitState * foundTUS = mScriptContext.pass->getTextureUnitState(tusName);
            if (foundTUS)
            {
                mScriptContext.stateLev = mScriptContext.pass->getTextureUnitStateIndex(foundTUS);
            }
            else
            {
                // name was not found so a new TUS is needed
                // position TUS level to the end index
                // a new TUS will be created later on
                mScriptContext.stateLev = static_cast<uint>(mScriptContext.pass->getNumTextureUnitStates());
            }
        }
        else
        {
		    //Increase Texture Unit State level depth
		    ++mScriptContext.stateLev;
        }

        if (mScriptContext.pass->getNumTextureUnitStates() > static_cast<size_t>(mScriptContext.stateLev))
        {
            mScriptContext.textureUnit = mScriptContext.pass->getTextureUnitState(mScriptContext.stateLev);
        }
        else
        {
            // Create a new texture unit
            mScriptContext.textureUnit = mScriptContext.pass->createTextureUnitState();
            if (!tusName.empty())
                mScriptContext.textureUnit->setName(tusName);
        }
        // update section
        mScriptContext.section = MSS_TEXTUREUNIT;
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseTextureAlias(void)
    {
        assert(mScriptContext.textureUnit);
        mScriptContext.textureUnit->setTextureNameAlias(getNextTokenLabel());
    }
    //-----------------------------------------------------------------------
    // Texture layer attributes
    void MaterialScriptCompiler::parseTexture(void)
    {
        assert(mScriptContext.textureUnit);
        TextureType tt = TEX_TYPE_2D;
		int mips = MIP_UNLIMITED; // When passed to TextureManager::load, this means default to default number of mipmaps
        bool isAlpha = false;
        PixelFormat desiredFormat = PF_UNKNOWN;
        const String& textureName = getNextTokenLabel();

		while (getRemainingTokensForAction() > 0)
		{
            switch(getNextTokenID())
            {
            case ID_1D:
                tt = TEX_TYPE_1D;
                break;
            case ID_2D:
                tt = TEX_TYPE_2D;
                break;
            case ID_3D:
                tt = TEX_TYPE_3D;
                break;
            case ID_CUBIC:
                tt = TEX_TYPE_CUBE_MAP;
                break;
            case ID_UNLIMITED:
				mips = MIP_UNLIMITED;
                break;
            case ID_ALPHA:
                isAlpha = true;
                break;
            case _value_:
                replaceToken();
                mips = static_cast<int>(getNextTokenValue());
                break;
            default:
                desiredFormat = PixelUtil::getFormatFromName(getCurrentTokenLexeme(), true);
                break;
            }
		}
        mScriptContext.textureUnit->setTextureName(textureName, tt);
        mScriptContext.textureUnit->setNumMipmaps(mips);
        mScriptContext.textureUnit->setIsAlpha(isAlpha);
        mScriptContext.textureUnit->setDesiredFormat(desiredFormat);
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseAnimTexture(void)
    {
        assert(mScriptContext.textureUnit);
        StringVector vecparams;
        // first token will be a label
        vecparams.push_back(getNextTokenLabel());
        // Determine which form it is
        // if next token is a value then no more labels to be processed
        if (testNextTokenID(_value_))
        {
            // First form using base name & number of frames
            unsigned int frameCount = static_cast<unsigned int>(getNextTokenValue());
            mScriptContext.textureUnit->setAnimatedTextureName(
                vecparams[0],
                frameCount,
                getNextTokenValue());
        }
        else
        {
            unsigned int numParams = 1;
            while (getRemainingTokensForAction() > 1)
            {
                vecparams.push_back(getNextTokenLabel());
                ++numParams;
            }
            // the last label should be a number so convert string label to number
            // Second form using individual names
            mScriptContext.textureUnit->setAnimatedTextureName(
                (String*)&vecparams[0],
                numParams,
                StringConverter::parseReal(getNextTokenLabel()));
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseCubicTexture(void)
    {
        assert(mScriptContext.textureUnit);
        StringVector vecparams;

        // first token will be a label
        vecparams.push_back(getNextTokenLabel());
        // Determine which form it is
        // if next token is a label then 5 more labels to be processed
        if (testNextTokenID(_character_))
        {
            // get next five texture names
            for (int i = 0; i < 5; ++i)
                vecparams.push_back(getNextTokenLabel());
        }

        bool useUVW = testNextTokenID(ID_COMBINED_UVW);

        if (vecparams.size() == 1)
        {
            mScriptContext.textureUnit->setCubicTextureName(vecparams[0], useUVW);
        }
        else
        {
            // Second form using individual names
            // Can use vecparams[0] as array start point
            mScriptContext.textureUnit->setCubicTextureName((String*)&vecparams[0], useUVW);
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseTexCoord(void)
    {
        assert(mScriptContext.textureUnit);
        mScriptContext.textureUnit->setTextureCoordSet(
            static_cast<unsigned int>(getNextTokenValue()));
    }
    //-----------------------------------------------------------------------
	TextureUnitState::TextureAddressingMode MaterialScriptCompiler::convTexAddressMode(void)
	{
	    switch (getNextTokenID())
	    {
		case ID_WRAP:
			return TextureUnitState::TAM_WRAP;
		case ID_CLAMP:
			return TextureUnitState::TAM_CLAMP;
		case ID_MIRROR:
			return TextureUnitState::TAM_MIRROR;
		case ID_BORDER:
			return TextureUnitState::TAM_BORDER;
		default:
            return TextureUnitState::TAM_WRAP;
	    }
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseTexAddressMode(void)
    {
        assert(mScriptContext.textureUnit);
        const size_t paramCount = getRemainingTokensForAction();

		if (paramCount == 1)
		{
			// Single-parameter option
			mScriptContext.textureUnit->setTextureAddressingMode(
				convTexAddressMode());
		}
		else
		{
			// 2-3 parameter option
			TextureUnitState::UVWAddressingMode uvw;
			uvw.u = convTexAddressMode();
			uvw.v = convTexAddressMode();

			if (paramCount == 3)
			{
				// w
				uvw.w = convTexAddressMode();
			}
			else
			{
				uvw.w = TextureUnitState::TAM_WRAP;
			}
			mScriptContext.textureUnit->setTextureAddressingMode(uvw);
		}
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseTexBorderColour(void)
    {
        assert(mScriptContext.textureUnit);
        mScriptContext.textureUnit->setTextureBorderColour( _parseColourValue() );
    }
    //-----------------------------------------------------------------------
    FilterOptions MaterialScriptCompiler::convertFiltering()
    {
        switch (getNextTokenID())
        {
        case ID_NONE:
            return FO_NONE;
        case ID_POINT:
            return FO_POINT;
        case ID_LINEAR:
            return FO_LINEAR;
        case ID_ANISOTROPIC:
            return FO_ANISOTROPIC;
        default:
            return FO_POINT;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseFiltering(void)
    {
        assert(mScriptContext.textureUnit);
        // Must be 1 or 3 parameters
        const size_t paramCount = getRemainingTokensForAction();
        if (paramCount == 1)
        {
            // Simple format
            switch (getNextTokenID())
            {
            case ID_BILINEAR:
                mScriptContext.textureUnit->setTextureFiltering(TFO_BILINEAR);
                break;
            case ID_TRILINEAR:
                mScriptContext.textureUnit->setTextureFiltering(TFO_TRILINEAR);
                break;
            case ID_ANISOTROPIC:
                mScriptContext.textureUnit->setTextureFiltering(TFO_ANISOTROPIC);
                break;
            case ID_NONE:
                mScriptContext.textureUnit->setTextureFiltering(TFO_NONE);
                break;
            }
        }
        else
        {
            // Complex format
            const FilterOptions minFO = convertFiltering();
            const FilterOptions magFO = convertFiltering();
            const FilterOptions mipFO = convertFiltering();
            mScriptContext.textureUnit->setTextureFiltering(minFO, magFO, mipFO);
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseMaxAnisotropy(void)
    {
        assert(mScriptContext.textureUnit);
        mScriptContext.textureUnit->setTextureAnisotropy(
            static_cast<unsigned int>(getNextTokenValue()));
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseColourOp(void)
    {
        assert(mScriptContext.textureUnit);
        switch (getNextTokenID())
        {
        case ID_REPLACE:
            mScriptContext.textureUnit->setColourOperation(LBO_REPLACE);
            break;
        case ID_ADD:
            mScriptContext.textureUnit->setColourOperation(LBO_ADD);
            break;
        case ID_MODULATE:
            mScriptContext.textureUnit->setColourOperation(LBO_MODULATE);
            break;
        case ID_ALPHA_BLEND:
            mScriptContext.textureUnit->setColourOperation(LBO_ALPHA_BLEND);
            break;
        }
    }
    //-----------------------------------------------------------------------
    LayerBlendOperationEx MaterialScriptCompiler::convertBlendOpEx(void)
    {
        switch(getNextTokenID())
        {
        case ID_SOURCE1:
            return LBX_SOURCE1;
        case ID_SOURCE2:
            return LBX_SOURCE2;
        case ID_MODULATE:
            return LBX_MODULATE;
        case ID_MODULATE_X2:
            return LBX_MODULATE_X2;
        case ID_MODULATE_X4:
            return LBX_MODULATE_X4;
        case ID_ADD:
            return LBX_ADD;
        case ID_ADD_SIGNED:
            return LBX_ADD_SIGNED;
        case ID_ADD_SMOOTH:
            return LBX_ADD_SMOOTH;
        case ID_SUBTRACT:
            return LBX_SUBTRACT;
        case ID_BLEND_DIFFUSE_COLOUR:
            return LBX_BLEND_DIFFUSE_COLOUR;
        case ID_BLEND_DIFFUSE_ALPHA:
            return LBX_BLEND_DIFFUSE_ALPHA;
        case ID_BLEND_TEXTURE_ALPHA:
            return LBX_BLEND_TEXTURE_ALPHA;
        case ID_BLEND_CURRENT_ALPHA:
            return LBX_BLEND_CURRENT_ALPHA;
        case ID_BLEND_MANUAL:
            return LBX_BLEND_MANUAL;
        case ID_DOTPRODUCT:
            return LBX_DOTPRODUCT;
        default:
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid blend function", "convertBlendOpEx");
        }
    }
    //-----------------------------------------------------------------------
    LayerBlendSource MaterialScriptCompiler::convertBlendSource(void)
    {
        switch(getNextTokenID())
        {
        case ID_SRC_CURRENT:
            return LBS_CURRENT;
        case ID_SRC_TEXTURE:
            return LBS_TEXTURE;
        case ID_SRC_DIFFUSE:
            return LBS_DIFFUSE;
        case ID_SRC_SPECULAR:
            return LBS_SPECULAR;
        case ID_SRC_MANUAL:
            return LBS_MANUAL;
        default:
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid blend source", "convertBlendSource");
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseColourOpEx(void)
    {

        assert(mScriptContext.textureUnit);

        LayerBlendOperationEx op;
        LayerBlendSource src1, src2;
        Real manual = 0.0;
        ColourValue colSrc1 = ColourValue::White;
        ColourValue colSrc2 = ColourValue::White;

        try {
            op = convertBlendOpEx();
            src1 = convertBlendSource();
            src2 = convertBlendSource();

            if (op == LBX_BLEND_MANUAL)
                manual = getNextTokenValue();

            if (src1 == LBS_MANUAL)
                colSrc1 = _parseColourValue();

            if (src2 == LBS_MANUAL)
                colSrc2 = _parseColourValue();
        }
        catch (Exception& e)
        {
            logParseError("Bad colour_op_ex attribute, " + e.getFullDescription());
            return;
        }

        mScriptContext.textureUnit->setColourOperationEx(op, src1, src2, colSrc1, colSrc2, manual);
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseColourOpMultipassFallback(void)
    {
        assert(mScriptContext.textureUnit);

        SceneBlendFactor src = convertBlendFactor();
        SceneBlendFactor dest = convertBlendFactor();
        mScriptContext.textureUnit->setColourOpMultipassFallback(src,dest);
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseAlphaOpEx(void)
    {
        assert(mScriptContext.textureUnit);

        LayerBlendOperationEx op;
        LayerBlendSource src1, src2;
        Real manual = 0.0;
        Real arg1 = 1.0, arg2 = 1.0;

        try {
            op = convertBlendOpEx();
            src1 = convertBlendSource();
            src2 = convertBlendSource();

            if (op == LBX_BLEND_MANUAL)
                manual = getNextTokenValue();

            if (src1 == LBS_MANUAL)
                arg1 = getNextTokenValue();

            if (src2 == LBS_MANUAL)
                arg2 = getNextTokenValue();
        }
        catch (Exception& e)
        {
            logParseError("Bad alpha_op_ex attribute, " + e.getFullDescription());
            return;
        }

        mScriptContext.textureUnit->setAlphaOperation(op, src1, src2, arg1, arg2, manual);
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseEnvMap(void)
    {
        assert(mScriptContext.textureUnit);

        switch (getNextTokenID())
        {
        case ID_OFF:
            mScriptContext.textureUnit->setEnvironmentMap(false);
            break;
        case ID_SPHERICAL:
            mScriptContext.textureUnit->setEnvironmentMap(true, TextureUnitState::ENV_CURVED);
            break;
        case ID_PLANAR:
            mScriptContext.textureUnit->setEnvironmentMap(true, TextureUnitState::ENV_PLANAR);
            break;
        case ID_CUBIC_REFLECTION:
            mScriptContext.textureUnit->setEnvironmentMap(true, TextureUnitState::ENV_REFLECTION);
            break;
        case ID_CUBIC_NORMAL:
            mScriptContext.textureUnit->setEnvironmentMap(true, TextureUnitState::ENV_NORMAL);
            break;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseScroll(void)
    {
        assert(mScriptContext.textureUnit);

        const Real x = getNextTokenValue();
        const Real y = getNextTokenValue();

        mScriptContext.textureUnit->setTextureScroll(x, y);
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseScrollAnim(void)
    {
        assert(mScriptContext.textureUnit);

        const Real xspeed = getNextTokenValue();
        const Real yspeed = getNextTokenValue();

        mScriptContext.textureUnit->setScrollAnimation(xspeed, yspeed);
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseRotate(void)
    {
        assert(mScriptContext.textureUnit);
        mScriptContext.textureUnit->setTextureRotate(Angle(getNextTokenValue()));
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseRotateAnim(void)
    {
        assert(mScriptContext.textureUnit);
        mScriptContext.textureUnit->setRotateAnimation(getNextTokenValue());
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseScale(void)
    {
        assert(mScriptContext.textureUnit);
        const Real xscale = getNextTokenValue();
        const Real yscale = getNextTokenValue();
        mScriptContext.textureUnit->setTextureScale(xscale, yscale);
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseWaveXform(void)
    {

        assert(mScriptContext.textureUnit);

        TextureUnitState::TextureTransformType ttype;
        WaveformType waveType;
        // Check transform type
        switch (getNextTokenID())
        {
        case ID_SCROLL_X:
            ttype = TextureUnitState::TT_TRANSLATE_U;
            break;
        case ID_SCROLL_Y:
            ttype = TextureUnitState::TT_TRANSLATE_V;
            break;
        case ID_ROTATE:
            ttype = TextureUnitState::TT_ROTATE;
            break;
        case ID_SCALE_X:
            ttype = TextureUnitState::TT_SCALE_U;
            break;
        case ID_SCALE_Y:
            ttype = TextureUnitState::TT_SCALE_V;
            break;
        }
        // Check wave type
        switch (getNextTokenID())
        {
        case ID_SINE:
            waveType = WFT_SINE;
            break;
        case ID_TRIANGLE:
            waveType = WFT_TRIANGLE;
            break;
        case ID_SQUARE:
            waveType = WFT_SQUARE;
            break;
        case ID_SAWTOOTH:
            waveType = WFT_SAWTOOTH;
            break;
        case ID_INVERSE_SAWTOOTH:
            waveType = WFT_INVERSE_SAWTOOTH;
            break;
        }

        const Real base = getNextTokenValue();
        const Real frequency = getNextTokenValue();
        const Real phase = getNextTokenValue();
        const Real amplitude = getNextTokenValue();

        mScriptContext.textureUnit->setTransformAnimation(
            ttype,
            waveType,
            base,
            frequency,
            phase,
            amplitude );
    }
	//-----------------------------------------------------------------------
	void MaterialScriptCompiler::parseTransform(void)
	{
        assert(mScriptContext.textureUnit);

        Real matrixArray[16];

        for (size_t i = 0; i < 16; ++i)
        {
            matrixArray[i] = getNextTokenValue();
        }

		Matrix4 xform(
			matrixArray[0],
			matrixArray[1],
			matrixArray[2],
			matrixArray[3],
			matrixArray[4],
			matrixArray[5],
			matrixArray[6],
			matrixArray[7],
			matrixArray[8],
			matrixArray[9],
			matrixArray[10],
			matrixArray[11],
			matrixArray[12],
			matrixArray[13],
			matrixArray[14],
			matrixArray[15]);

		mScriptContext.textureUnit->setTextureTransform(xform);
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseVertexProgramRef(void)
    {
        assert(mScriptContext.pass);
        // update section
        mScriptContext.section = MSS_PROGRAM_REF;
        String name;

        // get the name of the program definition if it was set
        if (getRemainingTokensForAction() == 1)
        {
            name = getNextTokenLabel();
            StringUtil::trim(name);
        }

        // check if pass has a vertex program already
        if (mScriptContext.pass->hasVertexProgram())
        {
            // if existing pass vertex program has same name as params
            // or params is empty then use current vertex program
            if (name.empty() || (mScriptContext.pass->getVertexProgramName() == name))
            {
                mScriptContext.program = mScriptContext.pass->getVertexProgram();
            }
        }

        // if context.program was not set then try to get the vertex program using the name
        // passed in params
        if (mScriptContext.program.isNull())
        {
            mScriptContext.program = GpuProgramManager::getSingleton().getByName(name);
            if (mScriptContext.program.isNull())
            {
                // Unknown program
                logParseError("Invalid vertex_program_ref entry - vertex program "
                    + name + " has not been defined.");
                return;
            }

            // Set the vertex program for this pass
            mScriptContext.pass->setVertexProgram(name);
        }

        mScriptContext.isProgramShadowCaster = false;
        mScriptContext.isVertexProgramShadowReceiver = false;
        mScriptContext.isFragmentProgramShadowReceiver = false;

        // Create params? Skip this if program is not supported
        if (mScriptContext.program->isSupported())
        {
            mScriptContext.programParams = mScriptContext.pass->getVertexProgramParameters();
			mScriptContext.numAnimationParametrics = 0;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseShadowCasterVertexProgramRef(void)
    {
        assert(mScriptContext.pass);
        // update section
        mScriptContext.section = MSS_PROGRAM_REF;
        String name;

        // get the name of the program definition if it was set
        if (getRemainingTokensForAction() == 1)
        {
            name = getNextTokenLabel();
            StringUtil::trim(name);
        }

        // check if pass has a shadow caster vertex program already
        if (mScriptContext.pass->hasShadowCasterVertexProgram())
        {
            // if existing pass vertex program has same name as params
            // or params is empty then use current vertex program
            if (name.empty() || (mScriptContext.pass->getShadowCasterVertexProgramName() == name))
            {
                mScriptContext.program = mScriptContext.pass->getShadowCasterVertexProgram();
            }
        }

        // if context.program was not set then try to get the vertex program using the name
        // passed in params
        if (mScriptContext.program.isNull())
        {
            mScriptContext.program = GpuProgramManager::getSingleton().getByName(name);
            if (mScriptContext.program.isNull())
            {
                // Unknown program
                logParseError("Invalid shadow_caster_vertex_program_ref entry - vertex program "
                    + name + " has not been defined.");
                return;
            }

            // Set the vertex program for this pass
            mScriptContext.pass->setShadowCasterVertexProgram(name);
        }

        mScriptContext.isProgramShadowCaster = true;
        mScriptContext.isVertexProgramShadowReceiver = false;
		mScriptContext.isFragmentProgramShadowReceiver = false;

        // Create params? Skip this if program is not supported
        if (mScriptContext.program->isSupported())
        {
            mScriptContext.programParams = mScriptContext.pass->getShadowCasterVertexProgramParameters();
			mScriptContext.numAnimationParametrics = 0;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseShadowReceiverVertexProgramRef(void)
    {
        assert(mScriptContext.pass);
        // update section
        mScriptContext.section = MSS_PROGRAM_REF;

        String name;

        // get the name of the program definition if it was set
        if (getRemainingTokensForAction() == 1)
        {
            name = getNextTokenLabel();
            StringUtil::trim(name);
        }

        // check if pass has a shadow caster vertex program already
        if (mScriptContext.pass->hasShadowReceiverVertexProgram())
        {
            // if existing pass vertex program has same name as params
            // or params is empty then use current vertex program
            if (name.empty() || (mScriptContext.pass->getShadowReceiverVertexProgramName() == name))
            {
                mScriptContext.program = mScriptContext.pass->getShadowReceiverVertexProgram();
            }
        }

        // if context.program was not set then try to get the vertex program using the name
        // passed in params
        if (mScriptContext.program.isNull())
        {
            mScriptContext.program = GpuProgramManager::getSingleton().getByName(name);
            if (mScriptContext.program.isNull())
            {
                // Unknown program
                logParseError("Invalid shadow_receiver_vertex_program_ref entry - vertex program "
                    + name + " has not been defined.");
                return;
            }

            // Set the vertex program for this pass
            mScriptContext.pass->setShadowReceiverVertexProgram(name);
        }

        mScriptContext.isProgramShadowCaster = false;
        mScriptContext.isVertexProgramShadowReceiver = true;
		mScriptContext.isFragmentProgramShadowReceiver = false;

        // Create params? Skip this if program is not supported
        if (mScriptContext.program->isSupported())
        {
            mScriptContext.programParams = mScriptContext.pass->getShadowReceiverVertexProgramParameters();
			mScriptContext.numAnimationParametrics = 0;
        }
    }
	//-----------------------------------------------------------------------
	void MaterialScriptCompiler::parseShadowReceiverFragmentProgramRef(void)
	{
        assert(mScriptContext.pass);
		// update section
		mScriptContext.section = MSS_PROGRAM_REF;

        String name;

        // get the name of the program definition if it was set
        if (getRemainingTokensForAction() == 1)
        {
            name = getNextTokenLabel();
            StringUtil::trim(name);
        }
        // check if pass has a fragment program already
        if (mScriptContext.pass->hasShadowReceiverFragmentProgram())
        {
            // if existing pass fragment program has same name as params
            // or params is empty then use current fragment program
            if (name.empty() || (mScriptContext.pass->getShadowReceiverFragmentProgramName() == name))
            {
                mScriptContext.program = mScriptContext.pass->getShadowReceiverFragmentProgram();
            }
        }

        // if context.program was not set then try to get the fragment program using the name
        // passed in
        if (mScriptContext.program.isNull())
        {
            mScriptContext.program = GpuProgramManager::getSingleton().getByName(name);
            if (mScriptContext.program.isNull())
            {
                // Unknown program
                logParseError("Invalid shadow_receiver_fragment_program_ref entry - fragment program "
                    + name + " has not been defined.");
                return;
            }

            // Set the vertex program for this pass
            mScriptContext.pass->setShadowReceiverFragmentProgram(name);
        }

        mScriptContext.isProgramShadowCaster = false;
        mScriptContext.isVertexProgramShadowReceiver = false;
        mScriptContext.isFragmentProgramShadowReceiver = true;

		// Create params? Skip this if program is not supported
		if (mScriptContext.program->isSupported())
		{
			mScriptContext.programParams = mScriptContext.pass->getShadowReceiverFragmentProgramParameters();
			mScriptContext.numAnimationParametrics = 0;
		}
	}
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseFragmentProgramRef(void)
    {
        assert(mScriptContext.pass);
        // update section
        mScriptContext.section = MSS_PROGRAM_REF;

        String name;

        // get the name of the program definition if it was set
        if (getRemainingTokensForAction() == 1)
        {
            name = getNextTokenLabel();
            StringUtil::trim(name);
        }
        // check if pass has a fragment program already
        if (mScriptContext.pass->hasFragmentProgram())
        {
            // if existing pass fragment program has same name as params
            // or params is empty then use current fragment program
            if (name.empty() || (mScriptContext.pass->getFragmentProgramName() == name))
            {
                mScriptContext.program = mScriptContext.pass->getFragmentProgram();
            }
        }

        // if context.program was not set then try to get the fragment program using the name
        // passed in params
        if (mScriptContext.program.isNull())
        {
            mScriptContext.program = GpuProgramManager::getSingleton().getByName(name);
            if (mScriptContext.program.isNull())
            {
                // Unknown program
                logParseError("Invalid fragment_program_ref entry - fragment program "
                    + name + " has not been defined.");
                return;
            }

            // Set the vertex program for this pass
            mScriptContext.pass->setFragmentProgram(name);
        }

        // Create params? Skip this if program is not supported
        if (mScriptContext.program->isSupported())
        {
            mScriptContext.programParams = mScriptContext.pass->getFragmentProgramParameters();
			mScriptContext.numAnimationParametrics = 0;
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::processManualProgramParam(size_t index, const String& commandname, const String& paramName)
    {
        // NB we assume that the first element of vecparams is taken up with either
        // the index or the parameter name, which we ignore

        // Determine type
        size_t start, dims, roundedDims, i;
        bool isReal;
        bool isMatrix4x4 = false;
        String param(getNextTokenLabel());

        StringUtil::toLowerCase(param);

        if (param == "matrix4x4")
        {
            dims = 16;
            isReal = true;
            isMatrix4x4 = true;
        }
        else if ((start = param.find("float")) != String::npos)
        {
            // find the dimensionality
            start = param.find_first_not_of("float");
            // Assume 1 if not specified
            if (start == String::npos)
            {
                dims = 1;
            }
            else
            {
                dims = StringConverter::parseInt(param.substr(start));
            }
            isReal = true;
        }
        else if ((start = param.find("int")) != String::npos)
        {
            // find the dimensionality
            start = param.find_first_not_of("int");
            // Assume 1 if not specified
            if (start == String::npos)
            {
                dims = 1;
            }
            else
            {
                dims = StringConverter::parseInt(param.substr(start));
            }
            isReal = false;
        }
        else
        {
            logParseError("Invalid " + commandname + " attribute - unrecognised "
                "parameter type " + param);
            return;
        }

        if (getRemainingTokensForAction() != dims)
        {
            logParseError("Invalid " + commandname + " attribute - you need " +
                StringConverter::toString(2 + dims) + " parameters for a parameter of "
                "type " + param);
        }

        // Round dims to multiple of 4
        if (dims %4 != 0)
        {
            roundedDims = dims + 4 - (dims % 4);
        }
        else
        {
            roundedDims = dims;
        }

		// clear any auto parameter bound to this constant, it would override this setting
		// can cause problems overriding materials or changing default params
		mScriptContext.programParams->clearAutoConstant(index);

		// Now parse all the values
        if (isReal)
        {
            Real* realBuffer = new Real[roundedDims];
            // Do specified values
            for (i = 0; i < dims; ++i)
            {
                realBuffer[i] = getNextTokenValue();
            }
            // Fill up to multiple of 4 with zero
            for (; i < roundedDims; ++i)
            {
                realBuffer[i] = 0.0f;

            }

            if (isMatrix4x4)
            {
                // its a Matrix4x4 so pass as a Matrix4
                // use specialized setConstant that takes a matrix so matrix is transposed if required
                Matrix4 m4x4(
                    realBuffer[0],  realBuffer[1],  realBuffer[2],  realBuffer[3],
                    realBuffer[4],  realBuffer[5],  realBuffer[6],  realBuffer[7],
                    realBuffer[8],  realBuffer[9],  realBuffer[10], realBuffer[11],
                    realBuffer[12], realBuffer[13], realBuffer[14], realBuffer[15]
                    );
                mScriptContext.programParams->setConstant(index, m4x4);
            }
            else
            {
                // Set
                mScriptContext.programParams->setConstant(index, realBuffer, static_cast<size_t>(roundedDims * 0.25));

            }


            delete [] realBuffer;
            // log the parameter
            mScriptContext.programParams->addConstantDefinition(paramName, index, dims, GpuProgramParameters::ET_REAL);
        }
        else
        {
            int* intBuffer = new int[roundedDims];
            // Do specified values
            for (i = 0; i < dims; ++i)
            {
                intBuffer[i] = static_cast<int>(getNextTokenValue());
            }
            // Fill to multiple of 4 with 0
            for (; i < roundedDims; ++i)
            {
                intBuffer[i] = 0;
            }
            // Set
            mScriptContext.programParams->setConstant(index, intBuffer, static_cast<size_t>(roundedDims * 0.25));
            delete [] intBuffer;
            // log the parameter
            mScriptContext.programParams->addConstantDefinition(paramName, index, dims, GpuProgramParameters::ET_INT);
        }
    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::processAutoProgramParam(const size_t index, const String& commandname, const String& paramName)
    {

        String constantName(getNextTokenLabel());
        // make sure param is in lower case
        StringUtil::toLowerCase(constantName);

        // lookup the param to see if its a valid auto constant
        const GpuProgramParameters::AutoConstantDefinition* autoConstantDef =
            mScriptContext.programParams->getAutoConstantDefinition(constantName);

        // exit with error msg if the auto constant definition wasn't found
        if (!autoConstantDef)
		{
			logParseError("Invalid " + commandname + " attribute - "
				+ constantName);
			return;
		}

        // add AutoConstant based on the type of data it uses
        switch (autoConstantDef->dataType)
        {
        case GpuProgramParameters::ACDT_NONE:
            mScriptContext.programParams->setAutoConstant(index, autoConstantDef->acType, 0);
            break;

        case GpuProgramParameters::ACDT_INT:
            {
				// Special case animation_parametric, we need to keep track of number of times used
				if (autoConstantDef->acType == GpuProgramParameters::ACT_ANIMATION_PARAMETRIC)
				{
					mScriptContext.programParams->setAutoConstant(
						index, autoConstantDef->acType, mScriptContext.numAnimationParametrics++);
				}
				else
				{

					if (getRemainingTokensForAction() != 1)
					{
						logParseError("Invalid " + commandname + " attribute - "
							"expected 3 parameters.");
						return;
					}

					size_t extraParam = static_cast<size_t>(getNextTokenValue());
					mScriptContext.programParams->setAutoConstant(
						index, autoConstantDef->acType, extraParam);
				}
            }
            break;

        case GpuProgramParameters::ACDT_REAL:
            {
                // special handling for time
                if (autoConstantDef->acType == GpuProgramParameters::ACT_TIME ||
                    autoConstantDef->acType == GpuProgramParameters::ACT_FRAME_TIME)
                {
                    Real factor = 1.0f;
                    if (getRemainingTokensForAction() == 1)
                    {
                        factor = getNextTokenValue();
                    }

                    mScriptContext.programParams->setAutoConstantReal(index, autoConstantDef->acType, factor);
                }
                else // normal processing for auto constants that take an extra real value
                {
                    if (getRemainingTokensForAction() != 1)
                    {
                        logParseError("Invalid " + commandname + " attribute - "
                            "expected 3 parameters.");
                        return;
                    }

			        const Real rData = getNextTokenValue();
			        mScriptContext.programParams->setAutoConstantReal(index, autoConstantDef->acType, rData);
                }
            }
            break;

        } // end switch

        // add constant definition based on AutoConstant
        // make element count 0 so that proper allocation occurs when AutoState is set up
        size_t constantIndex = mScriptContext.programParams->addConstantDefinition(
			paramName, index, 0, autoConstantDef->elementType);
        // update constant definition auto settings
        // since an autoconstant was just added, its the last one in the container
        size_t autoIndex = mScriptContext.programParams->getAutoConstantCount() - 1;
        // setup autoState which will allocate the proper amount of storage required by constant entries
        mScriptContext.programParams->setConstantDefinitionAutoState(constantIndex, true, autoIndex);

    }

    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseParamIndexed(void)
    {
        if (mScriptContext.section == MSS_DEFAULT_PARAMETERS)
        {
            // save the pass2 token que position for later processing
            mScriptContext.pendingDefaultParams.push_back(getPass2TokenQuePosition());
            return;
        }

        // NB skip this if the program is not supported or could not be found
        if (mScriptContext.program.isNull() || !mScriptContext.program->isSupported())
        {
            return;
        }

        // Get start index
        const size_t index = static_cast<size_t>(getNextTokenValue());

        processManualProgramParam(index, "param_indexed");

    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseParamIndexedAuto(void)
    {
        if (mScriptContext.section == MSS_DEFAULT_PARAMETERS)
        {
            // save the pass2 token que position for later processing
            mScriptContext.pendingDefaultParams.push_back(getPass2TokenQuePosition());
            return;
        }
        // NB skip this if the program is not supported or could not be found
        if (mScriptContext.program.isNull() || !mScriptContext.program->isSupported())
        {
            return;
        }
        // Get start index
        const size_t index = static_cast<size_t>(getNextTokenValue());

        processAutoProgramParam(index, "param_indexed_auto");

    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseParamNamed(void)
    {
        if (mScriptContext.section == MSS_DEFAULT_PARAMETERS)
        {
            // save the pass2 token que position for later processing
            mScriptContext.pendingDefaultParams.push_back(getPass2TokenQuePosition());
            return;
        }
        // NB skip this if the program is not supported or could not be found
        if (mScriptContext.program.isNull() || !mScriptContext.program->isSupported())
        {
            return;
        }

        // Get start index from name
        size_t index;
        const String& paramName = getNextTokenLabel();
        try {
            index = mScriptContext.programParams->getParamIndex(paramName);
        }
        catch (Exception& e)
        {
            logParseError("Invalid param_named attribute - " + e.getFullDescription());
            return;
        }

        // TEST
        /*
        LogManager::getSingleton().logMessage("SETTING PARAMETER " + vecparams[0] + " as index " +
            StringConverter::toString(index));
        */
        processManualProgramParam(index, "param_named", paramName);

    }
    //-----------------------------------------------------------------------
    void MaterialScriptCompiler::parseParamNamedAuto(void)
    {
        if (mScriptContext.section == MSS_DEFAULT_PARAMETERS)
        {
            // save the pass2 token que position for later processing
            mScriptContext.pendingDefaultParams.push_back(getPass2TokenQuePosition());
            return;
        }
        // NB skip this if the program is not supported or could not be found
        if (mScriptContext.program.isNull() || !mScriptContext.program->isSupported())
        {
            return;
        }

        // Get start index from name
        size_t index;
        const String& paramNamed = getNextTokenLabel();
        try {
            index = mScriptContext.programParams->getParamIndex(paramNamed);
        }
        catch (Exception& e)
        {
            logParseError("Invalid param_named_auto attribute - " + e.getFullDescription());
            return;
        }

        processAutoProgramParam(index, "param_named_auto", paramNamed);
    }
    //-----------------------------------------------------------------------
	void MaterialScriptCompiler::finishProgramDefinition(void)
	{
		// Now it is time to create the program and propagate the parameters
		MaterialScriptProgramDefinition* def = mScriptContext.programDef;
        GpuProgramPtr gp;
		if (def->language == "asm")
		{
			// Native assembler
			// Validate
			if (def->source.empty())
			{
				logParseError("Invalid program definition for " + def->name +
					", you must specify a source file.");
			}
			if (def->syntax.empty())
			{
				logParseError("Invalid program definition for " + def->name +
					", you must specify a syntax code.");
			}
			// Create
			gp = GpuProgramManager::getSingleton().
				createProgram(def->name, mScriptContext.groupName, def->source,
                    def->progType, def->syntax);

		}
		else
		{
			// High-level program
			// Validate
			if (def->source.empty())
			{
				logParseError("Invalid program definition for " + def->name +
					", you must specify a source file.");
			}
			// Create
            try
            {
			    HighLevelGpuProgramPtr hgp = HighLevelGpuProgramManager::getSingleton().
				    createProgram(def->name, mScriptContext.groupName,
                        def->language, def->progType);
                // Check that the program language is supported
                if (hgp->getLanguage() != def->language)
                {
                    logParseError("Not supports language '" + def->language +
                        "' for '" + def->name + "' program.");
				    mScriptContext.program.setNull();
            	    mScriptContext.programParams.setNull();
                    return;
                }

                // Assign to generalised version
                gp = hgp;
                // Set source file
                hgp->setSourceFile(def->source);

			    // Set custom parameters
			    std::map<String, String>::const_iterator i, iend;
			    iend = def->customParameters.end();
			    for (i = def->customParameters.begin(); i != iend; ++i)
			    {
				    if (!hgp->setParameter(i->first, i->second))
				    {
					    logParseError("Error in program " + def->name +
						    " parameter " + i->first + " is not valid.");
				    }
			    }
            }
            catch (Exception& e)
            {
                logParseError("Could not create GPU program '"
                    + def->name + "', error reported was: " + e.getFullDescription());
				mScriptContext.program.setNull();
            	mScriptContext.programParams.setNull();
				return;
            }
        }
        // Set skeletal animation option
        gp->setSkeletalAnimationIncluded(def->supportsSkeletalAnimation);
		// Set morph animation option
		gp->setMorphAnimationIncluded(def->supportsMorphAnimation);
		// Set pose animation option
		gp->setPoseAnimationIncluded(def->supportsPoseAnimation);
		// set origin
		gp->_notifyOrigin(mSourceName);

        // Set up to receive default parameters
        if (gp->isSupported()
            && !mScriptContext.pendingDefaultParams.empty())
        {
            mScriptContext.programParams = gp->getDefaultParameters();
			mScriptContext.numAnimationParametrics = 0;
            mScriptContext.program = gp;
            size_t i, iend;
            iend = mScriptContext.pendingDefaultParams.size();
            for (i = 0;
                i < iend; ++i)
            {
                // invoke the action for the pending default param in the token que
                setPass2TokenQuePosition(mScriptContext.pendingDefaultParams[i], true);
            }
            // Reset
            mScriptContext.program.setNull();
            mScriptContext.programParams.setNull();
        }

	}


}

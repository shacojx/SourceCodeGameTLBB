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
#include "OgreCompositorScriptCompiler.h"
#include "OgreCommon.h"
#include "OgreStringConverter.h"
#include "OgreLogManager.h"
#include "OgreException.h"
#include "OgreCompositorManager.h"
#include "OgreCompositionTechnique.h"
#include "OgreCompositionTargetPass.h"
#include "OgreCompositionPass.h"

namespace Ogre {

	//-----------------------------------------------------------------------
    // Static definitions
    //-----------------------------------------------------------------------
    CompositorScriptCompiler::TokenActionMap CompositorScriptCompiler::mTokenActionMap;

	String CompositorScriptCompiler::compositorScript_BNF =
		// Top level rule
		"<Script> ::= {<Compositor>} \n"
		"<Compositor> ::= 'compositor' <Flex_Label> '{' <Technique> '}' \n"
		// Technique
		"<Technique> ::= 'technique' '{' {<Texture>} {<Target>} <TargetOutput> '}' \n"
		"<Texture> ::= 'texture' <Label> <WidthOption> <HeightOption> <PixelFormat> \n"
		"<WidthOption> ::= 'target_width' | <#width> \n"
		"<HeightOption> ::= 'target_height' | <#height> \n"
		"<PixelFormat> ::= 'PF_A8R8G8B8' | 'PF_R8G8B8A8' | 'PF_R8G8B8' | 'PF_FLOAT16_RGBA' | \n"
        "   'PF_FLOAT16_RGB' | 'PF_FLOAT16_R' | 'PF_FLOAT32_RGBA' | 'PF_FLOAT32_RGB' | 'PF_FLOAT32_R' \n"
		// Target
		"<Target> ::= 'target ' <Label> '{' {<TargetOptions>} {<Pass>} '}' \n"
	    "<TargetOptions> ::=	<TargetInput> | <OnlyInitial> | <VisibilityMask> | \n"
	    "   <LodBias> | <MaterialScheme> \n"
		"<TargetInput> ::= 'input' <TargetInputOptions> \n"
		"<TargetInputOptions> ::= 'none' | 'previous' \n"
		"<OnlyInitial> ::= 'only_initial' <On_Off> \n"
		"<VisibilityMask> ::= 'visibility_mask' <#mask> \n"
		"<LodBias> ::= 'lod_bias' <#lodbias> \n"
		"<MaterialScheme> ::= 'material_scheme' <Label> \n"
		"<TargetOutput> ::= 'target_output' '{' [<TargetInput>] {<Pass>} '}' \n"
		// Pass
		"<Pass> ::= 'pass' <PassTypes> '{' {<PassOptions>} '}' \n"
		"<PassTypes> ::= 'render_quad' | 'clear' | 'stencil' | 'render_scene' \n"
		"<PassOptions> ::= <PassFirstRenderQueue> | <PassLastRenderQueue> | \n"
		"    <PassIdentifier> | <PassMaterial> | <PassInput> | <ClearSection> | <StencilSection> \n"
		"<PassMaterial> ::= 'material' <Label> \n"
		"<PassInput> ::= 'input' <#id> <Label> \n"
		"<PassFirstRenderQueue> ::= 'first_render_queue' <#queue> \n"
		"<PassLastRenderQueue> ::= 'last_render_queue' <#queue> \n"
		"<PassIdentifier> ::= 'identifier' <#id> \n"
		// clear
		"<ClearSection> ::= -'clear' -'{' {<ClearOptions>} -'}' \n"
		"<ClearOptions> ::= <Buffers> | <ColourValue> | <DepthValue> | <StencilValue> \n"
		"<Buffers> ::= 'buffers' {<BufferTypes>} \n"
		"<BufferTypes> ::= <Colour> | <Depth> | <Stencil> \n"
		"<Colour> ::= 'colour' (?!<ValueChk>) \n"
		"<Depth> ::= 'depth' (?!<ValueChk>) \n"
		"<Stencil> ::= 'stencil' (?!<ValueChk>) \n"
		"<ValueChk> ::= '_value' \n"
		"<ColourValue> ::= 'colour_value' <#red> <#green> <#blue> <#alpha> \n"
		"<DepthValue> ::= 'depth_value' <#depth> \n"
		"<StencilValue> ::= 'stencil_value' <#val> \n"
		// stencil
		"<StencilSection> ::= -'stencil' -'{' {<StencilOptions>} -'}' \n"
		"<StencilOptions> ::=  <Check> | <CompareFunction> | <RefVal> | <Mask> | <FailOp> | <DepthFailOp> | \n"
		"   <PassOp> | <TwoSided> \n"
		"<Check> ::= 'check' <On_Off> \n"
		"<CompareFunction> ::= 'comp_func' <CompFunc> \n"
		"<CompFunc> ::= 'always_fail' | 'always_pass' | 'less_equal' | 'less' | 'equal' | \n"
		"   'not_equal' | 'equal' | 'greater_equal' | 'greater' \n"
        "<RefVal> ::= 'ref_value' <#val> \n"
        "<Mask> ::= 'mask' <#mask> \n"
        "<FailOp> ::= 'fail_op' <StencilOperation> \n"
        "<DepthFailOp> ::= 'depth_fail_op' <StencilOperation> \n"
        "<PassOp> ::= 'pass_op' <StencilOperation> \n"
        "<TwoSided> ::= 'two_sided' <On_Off> \n"
		"<StencilOperation> ::= 'keep' | 'zero' | 'replace' | 'increment_wrap' | 'increment' | \n"
		"   'decrement_wrap' | 'decrement' | 'invert' \n"

		// common rules
		"<On_Off> ::= 'on' | 'off' \n"
		"<Label> ::= <Quoted_Label> | <Unquoted_Label> \n"
		"<Flex_Label> ::= <Quoted_Label> | <Spaced_Label> \n"
		"<Quoted_Label> ::= -'\"' <Spaced_Label> -'\"' \n"
		"<Spaced_Label> ::= <Spaced_Label_Illegals> {<Spaced_Label_Illegals>} \n"
        "<Unquoted_Label> ::= <Unquoted_Label_Illegals> {<Unquoted_Label_Illegals>} \n"
		"<Spaced_Label_Illegals> ::= (!,\n\r\t{}\") \n"
		"<Unquoted_Label_Illegals> ::= (! \n\r\t{}\") \n"

		;
	//-----------------------------------------------------------------------
	CompositorScriptCompiler::CompositorScriptCompiler(void)
	{
        // set default group resource name
        mScriptContext.groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	}
	//-----------------------------------------------------------------------
	CompositorScriptCompiler::~CompositorScriptCompiler(void)
	{

	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::setupTokenDefinitions(void)
	{
		addLexemeTokenAction("{", ID_OPENBRACE, &CompositorScriptCompiler::parseOpenBrace);
		addLexemeTokenAction("}", ID_CLOSEBRACE, &CompositorScriptCompiler::parseCloseBrace);
		addLexemeTokenAction("compositor", ID_COMPOSITOR, &CompositorScriptCompiler::parseCompositor);

		// Technique section
		addLexemeTokenAction("technique", ID_TECHNIQUE, &CompositorScriptCompiler::parseTechnique);
		addLexemeTokenAction("texture", ID_TEXTURE, &CompositorScriptCompiler::parseTexture);
		addLexemeTokenAction("target_width", ID_TARGET_WIDTH);
		addLexemeTokenAction("target_height", ID_TARGET_HEIGHT);
		addLexemeTokenAction("PF_A8R8G8B8", ID_PF_A8R8G8B8);
		addLexemeTokenAction("PF_R8G8B8A8", ID_PF_R8G8B8A8);
		addLexemeTokenAction("PF_R8G8B8", ID_PF_R8G8B8);
		addLexemeTokenAction("PF_FLOAT16_R", ID_PF_FLOAT16_R);
		addLexemeTokenAction("PF_FLOAT16_RGB", ID_PF_FLOAT16_RGB);
		addLexemeTokenAction("PF_FLOAT16_RGBA", ID_PF_FLOAT16_RGBA);
		addLexemeTokenAction("PF_FLOAT32_R", ID_PF_FLOAT32_R);
		addLexemeTokenAction("PF_FLOAT32_RGB", ID_PF_FLOAT32_RGB);
		addLexemeTokenAction("PF_FLOAT32_RGBA", ID_PF_FLOAT32_RGBA);

		// Target section
		addLexemeTokenAction("target ", ID_TARGET, &CompositorScriptCompiler::parseTarget);
		addLexemeTokenAction("input", ID_INPUT, &CompositorScriptCompiler::parseInput);
		addLexemeTokenAction("none", ID_NONE);
		addLexemeTokenAction("previous", ID_PREVIOUS);
		addLexemeTokenAction("target_output", ID_TARGET_OUTPUT, &CompositorScriptCompiler::parseTargetOutput);
		addLexemeTokenAction("only_initial", ID_ONLY_INITIAL, &CompositorScriptCompiler::parseOnlyInitial);
		addLexemeTokenAction("visibility_mask", ID_VISIBILITY_MASK, &CompositorScriptCompiler::parseVisibilityMask);
		addLexemeTokenAction("lod_bias", ID_LOD_BIAS, &CompositorScriptCompiler::parseLodBias);
		addLexemeTokenAction("material_scheme", ID_MATERIAL_SCHEME, &CompositorScriptCompiler::parseMaterialScheme);

		// pass section
		addLexemeTokenAction("pass", ID_PASS, &CompositorScriptCompiler::parsePass);
		// input defined above
		addLexemeTokenAction("render_quad", ID_RENDER_QUAD);
		addLexemeTokenAction("clear", ID_CLEAR);
		addLexemeTokenAction("stencil", ID_STENCIL);
		addLexemeTokenAction("render_scene", ID_RENDER_SCENE);
		// pass attributes
		addLexemeTokenAction("material", ID_MATERIAL, &CompositorScriptCompiler::parseMaterial);
		addLexemeTokenAction("first_render_queue", ID_FIRST_RQ, &CompositorScriptCompiler::parseFirstRenderQueue);
		addLexemeTokenAction("last_render_queue", ID_LAST_RQ, &CompositorScriptCompiler::parseLastRenderQueue);
		addLexemeTokenAction("identifier", ID_IDENTIFIER, &CompositorScriptCompiler::parseIdentifier);
		// clear
		addLexemeTokenAction("buffers", ID_CLR_BUFF, &CompositorScriptCompiler::parseClearBuffers);
		addLexemeTokenAction("colour", ID_CLR_COLOUR);
		addLexemeTokenAction("depth", ID_CLR_DEPTH);
		addLexemeTokenAction("colour_value", ID_CLR_COLOUR_VAL, &CompositorScriptCompiler::parseClearColourValue);
		addLexemeTokenAction("depth_value", ID_CLR_DEPTH_VAL, &CompositorScriptCompiler::parseClearDepthValue);
		addLexemeTokenAction("stencil_value", ID_CLR_STENCIL_VAL, &CompositorScriptCompiler::parseClearStencilValue);
		// stencil
		addLexemeTokenAction("check", ID_ST_CHECK, &CompositorScriptCompiler::parseStencilCheck);
		addLexemeTokenAction("comp_func", ID_ST_FUNC, &CompositorScriptCompiler::parseStencilFunc);
		addLexemeTokenAction("ref_value", ID_ST_REF_VAL, &CompositorScriptCompiler::parseStencilRefVal);
		addLexemeTokenAction("mask", ID_ST_MASK, &CompositorScriptCompiler::parseStencilMask);
		addLexemeTokenAction("fail_op", ID_ST_FAILOP, &CompositorScriptCompiler::parseStencilFailOp);
		addLexemeTokenAction("depth_fail_op", ID_ST_DEPTH_FAILOP, &CompositorScriptCompiler::parseStencilDepthFailOp);
		addLexemeTokenAction("pass_op", ID_ST_PASSOP, &CompositorScriptCompiler::parseStencilPassOp);
		addLexemeTokenAction("two_sided", ID_ST_TWOSIDED, &CompositorScriptCompiler::parseStencilTwoSided);
		// compare functions
		addLexemeTokenAction("always_fail", ID_ST_ALWAYS_FAIL);
		addLexemeTokenAction("always_pass", ID_ST_ALWAYS_PASS);
		addLexemeTokenAction("less", ID_ST_LESS);
		addLexemeTokenAction("less_equal", ID_ST_LESS_EQUAL);
		addLexemeTokenAction("equal", ID_ST_EQUAL);
		addLexemeTokenAction("not_equal", ID_ST_NOT_EQUAL);
		addLexemeTokenAction("greater_equal", ID_ST_GREATER_EQUAL);
		addLexemeTokenAction("greater", ID_ST_GREATER);
		// stencil operations
		addLexemeTokenAction("keep", ID_ST_KEEP);
		addLexemeTokenAction("zero", ID_ST_ZERO);
		addLexemeTokenAction("replace", ID_ST_REPLACE);
		addLexemeTokenAction("increment", ID_ST_INCREMENT);
		addLexemeTokenAction("decrement", ID_ST_DECREMENT);
		addLexemeTokenAction("increment_wrap", ID_ST_INCREMENT_WRAP);
		addLexemeTokenAction("decrement_wrap", ID_ST_DECREMENT_WRAP);
		addLexemeTokenAction("invert", ID_ST_INVERT);

		// common section
		addLexemeTokenAction("on", ID_ON);
		addLexemeTokenAction("off", ID_OFF);

	}

	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::addLexemeTokenAction(const String& lexeme,
		const size_t token, const CSC_Action action)
	{
		addLexemeToken(lexeme, token, action != 0);
		mTokenActionMap[token] = action;
	}

	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::executeTokenAction(const size_t tokenID)
	{
		TokenActionIterator action = mTokenActionMap.find(tokenID);

		if (action == mTokenActionMap.end())
		{
			// BAD command. BAD!
			logParseError("Unrecognised compositor script command action");
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
	void CompositorScriptCompiler::logParseError(const String& error)
	{
		// log material name only if filename not specified
		if (mSourceName.empty() && !mScriptContext.compositor.isNull())
		{
			LogManager::getSingleton().logMessage(
				"Error in compositor " + mScriptContext.compositor->getName() +
				" : " + error);
		}
		else
		{
			if (!mScriptContext.compositor.isNull())
			{
				LogManager::getSingleton().logMessage(
					"Error in compositor " + mScriptContext.compositor->getName() +
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
	void CompositorScriptCompiler::parseOpenBrace(void)
	{

	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseCloseBrace(void)
	{
		switch(mScriptContext.section)
		{
		case CSS_NONE:
			logParseError("Unexpected terminating brace.");
			break;
		case CSS_COMPOSITOR:
			// End of compositor
			mScriptContext.section = CSS_NONE;
			mScriptContext.compositor.setNull();
			break;
		case CSS_TECHNIQUE:
			// End of technique
			mScriptContext.section = CSS_COMPOSITOR;
			mScriptContext.technique = NULL;
			break;
		case CSS_TARGET:
			// End of target
			mScriptContext.section = CSS_TECHNIQUE;
			mScriptContext.target = NULL;
			break;
		case CSS_PASS:
			// End of pass
			mScriptContext.section = CSS_TARGET;
			mScriptContext.pass = NULL;
			break;
		};
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseCompositor(void)
	{
		const String compositorName = getNextTokenLabel();
		mScriptContext.compositor = CompositorManager::getSingleton().create(
            compositorName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
			);
		mScriptContext.section = CSS_COMPOSITOR;

	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseTechnique(void)
	{
		mScriptContext.technique = mScriptContext.compositor->createTechnique();
		mScriptContext.section = CSS_TECHNIQUE;
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseTexture(void)
	{
	    assert(mScriptContext.technique);
		const String textureName = getNextTokenLabel();
        CompositionTechnique::TextureDefinition* textureDef = mScriptContext.technique->createTextureDefinition(textureName);
        // if peek next token is target_width then get token and use 0 for width
        // determine width parameter
        if (testNextTokenID(ID_TARGET_WIDTH))
        {
            getNextToken();
            // a value of zero causes texture to be size of render target
            textureDef->width.viewport = 1;
        }
        else
        {
            textureDef->width.adjust = static_cast<size_t>(getNextTokenValue());
        }
        // determine height parameter
        if (testNextTokenID(ID_TARGET_HEIGHT))
        {
            getNextToken();
            // a value of zero causes texture to be size of render target
            textureDef->height.viewport = 1;
        }
        else
        {
            textureDef->height.adjust = static_cast<size_t>(getNextTokenValue());
        }
        // get pixel factor
        switch (getNextTokenID())
        {
        case ID_PF_A8R8G8B8:
            textureDef->format = PF_A8R8G8B8;
            break;

        case ID_PF_R8G8B8A8:
            textureDef->format = PF_R8G8B8A8;
            break;

        case ID_PF_R8G8B8:
            textureDef->format = PF_R8G8B8;
            break;
		case ID_PF_FLOAT16_R:
            textureDef->format = PF_FLOAT16_R;
            break;
		case ID_PF_FLOAT16_RGB:
            textureDef->format = PF_FLOAT16_RGB;
            break;
		case ID_PF_FLOAT16_RGBA:
            textureDef->format = PF_FLOAT16_RGBA;
            break;
		case ID_PF_FLOAT32_R:
            textureDef->format = PF_FLOAT32_R;
            break;
		case ID_PF_FLOAT32_RGB:
            textureDef->format = PF_FLOAT32_RGB;
            break;
		case ID_PF_FLOAT32_RGBA:
            textureDef->format = PF_FLOAT32_RGBA;
            break;

        default:
            // should never get here?
            break;
        }
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseTarget(void)
	{
        assert(mScriptContext.technique);

		mScriptContext.section = CSS_TARGET;
        mScriptContext.target = mScriptContext.technique->createTargetPass();
        mScriptContext.target->setOutputName(getNextTokenLabel());

	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseInput(void)
	{
		// input parameters depends on context either target or pass
		if (mScriptContext.section == CSS_TARGET)
		{
		    // for input in target, there is only one parameter
		    assert(mScriptContext.target);
		    if (testNextTokenID(ID_PREVIOUS))
                mScriptContext.target->setInputMode(CompositionTargetPass::IM_PREVIOUS);
            else
                mScriptContext.target->setInputMode(CompositionTargetPass::IM_NONE);
		}
		else // assume for pass section context
		{
		    // for input in pass, there are two parameters
		    assert(mScriptContext.pass);
		    uint32 id = static_cast<uint32>(getNextTokenValue());
		    const String& textureName = getNextTokenLabel();
		    mScriptContext.pass->setInput(id, textureName);
		}

	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseTargetOutput(void)
	{
		assert(mScriptContext.technique);
		mScriptContext.target = mScriptContext.technique->getOutputTargetPass();
		mScriptContext.section = CSS_TARGET;
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseOnlyInitial(void)
	{
        assert(mScriptContext.target);
        mScriptContext.target->setOnlyInitial(testNextTokenID(ID_ON));
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseVisibilityMask(void)
	{
        assert(mScriptContext.target);
        mScriptContext.target->setVisibilityMask(static_cast<uint32>(getNextTokenValue()));
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseLodBias(void)
	{
        assert(mScriptContext.target);
        mScriptContext.target->setLodBias(getNextTokenValue());
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseMaterialScheme(void)
	{
		assert(mScriptContext.target);
		mScriptContext.target->setMaterialScheme(getNextTokenLabel());
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parsePass(void)
	{
		assert(mScriptContext.target);
        mScriptContext.pass = mScriptContext.target->createPass();
        CompositionPass::PassType passType = CompositionPass::PT_RENDERQUAD;
        switch (getNextTokenID())
        {
        case ID_RENDER_QUAD:
            passType = CompositionPass::PT_RENDERQUAD;
            break;

        case ID_CLEAR:
            passType = CompositionPass::PT_CLEAR;
            break;

        case ID_STENCIL:
            passType = CompositionPass::PT_STENCIL;
            break;

        case ID_RENDER_SCENE:
            passType = CompositionPass::PT_RENDERSCENE;
            break;

        default:
            break;
        }

        mScriptContext.pass->setType(passType);

		mScriptContext.section = CSS_PASS;

	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseMaterial(void)
	{
		assert(mScriptContext.pass);
        mScriptContext.pass->setMaterialName(getNextTokenLabel());
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseFirstRenderQueue(void)
	{
		assert(mScriptContext.pass);
		mScriptContext.pass->setFirstRenderQueue(static_cast<uint8>(getNextTokenValue()));
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseLastRenderQueue(void)
	{
		assert(mScriptContext.pass);
		mScriptContext.pass->setLastRenderQueue(static_cast<uint8>(getNextTokenValue()));
	}
	//-----------------------------------------------------------------------
	void CompositorScriptCompiler::parseIdentifier(void)
	{
		assert(mScriptContext.pass);
		mScriptContext.pass->setIdentifier(static_cast<uint32>(getNextTokenValue()));
	}
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseClearBuffers(void)
    {
		assert(mScriptContext.pass);
		// while there are tokens for the action, get next token and set buffer flag
		uint32 bufferFlags = 0;

		while (getRemainingTokensForAction() > 0)
		{
		    switch (getNextTokenID())
		    {
            case ID_CLR_COLOUR:
                bufferFlags |= FBT_COLOUR;
                break;

            case ID_CLR_DEPTH:
                bufferFlags |= FBT_DEPTH;
                break;

            case ID_STENCIL:
                bufferFlags |= FBT_STENCIL;
                break;

            default:
                break;
		    }
		}
		mScriptContext.pass->setClearBuffers(bufferFlags);
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseClearColourValue(void)
    {
		assert(mScriptContext.pass);
		Real red = getNextTokenValue();
		Real green = getNextTokenValue();
		Real blue = getNextTokenValue();
		Real alpha = getNextTokenValue();
		mScriptContext.pass->setClearColour(ColourValue(red, green, blue, alpha));
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseClearDepthValue(void)
    {
		assert(mScriptContext.pass);
		mScriptContext.pass->setClearDepth(getNextTokenValue());
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseClearStencilValue(void)
    {
		assert(mScriptContext.pass);
		mScriptContext.pass->setClearStencil(static_cast<uint32>(getNextTokenValue()));
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseStencilCheck(void)
    {
		assert(mScriptContext.pass);
		mScriptContext.pass->setStencilCheck(testNextTokenID(ID_ON));
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseStencilFunc(void)
    {
		assert(mScriptContext.pass);
		mScriptContext.pass->setStencilFunc(extractCompareFunc());
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseStencilRefVal(void)
    {
		assert(mScriptContext.pass);
		mScriptContext.pass->setStencilRefValue(static_cast<uint32>(getNextTokenValue()));
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseStencilMask(void)
    {
		assert(mScriptContext.pass);
		mScriptContext.pass->setStencilMask(static_cast<uint32>(getNextTokenValue()));
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseStencilFailOp(void)
    {
		assert(mScriptContext.pass);
		mScriptContext.pass->setStencilFailOp(extractStencilOp());
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseStencilDepthFailOp(void)
    {
		assert(mScriptContext.pass);
		mScriptContext.pass->setStencilDepthFailOp(extractStencilOp());
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseStencilPassOp(void)
    {
		assert(mScriptContext.pass);
		mScriptContext.pass->setStencilPassOp(extractStencilOp());
    }
	//-----------------------------------------------------------------------
    void CompositorScriptCompiler::parseStencilTwoSided(void)
    {
		assert(mScriptContext.pass);
		mScriptContext.pass->setStencilTwoSidedOperation(testNextTokenID(ID_ON));
    }
	//-----------------------------------------------------------------------
	StencilOperation CompositorScriptCompiler::extractStencilOp(void)
	{
	    StencilOperation sop = SOP_KEEP;

        switch (getNextTokenID())
        {
        case ID_ST_KEEP:
            sop = SOP_KEEP;
            break;

        case ID_ST_ZERO:
            sop = SOP_ZERO;
            break;

        case ID_ST_REPLACE:
            sop = SOP_REPLACE;
            break;

        case ID_ST_INCREMENT:
            sop = SOP_INCREMENT;
            break;

        case ID_ST_DECREMENT:
            sop = SOP_DECREMENT;
            break;

        case ID_ST_INCREMENT_WRAP:
            sop = SOP_INCREMENT_WRAP;
            break;

        case ID_ST_DECREMENT_WRAP:
            sop = SOP_DECREMENT_WRAP;
            break;

        case ID_ST_INVERT:
            sop = SOP_INVERT;
            break;

        default:
            break;
        }

        return sop;
	}
    CompareFunction CompositorScriptCompiler::extractCompareFunc(void)
	{
	    CompareFunction compFunc = CMPF_ALWAYS_PASS;

        switch (getNextTokenID())
        {
        case ID_ST_ALWAYS_FAIL:
            compFunc = CMPF_ALWAYS_FAIL;
            break;

        case ID_ST_ALWAYS_PASS:
            compFunc = CMPF_ALWAYS_PASS;
            break;

        case ID_ST_LESS:
            compFunc = CMPF_LESS;
            break;

        case ID_ST_LESS_EQUAL:
            compFunc = CMPF_LESS_EQUAL;
            break;

        case ID_ST_EQUAL:
            compFunc = CMPF_EQUAL;
            break;

        case ID_ST_NOT_EQUAL:
            compFunc = CMPF_NOT_EQUAL;
            break;

        case ID_ST_GREATER_EQUAL:
            compFunc = CMPF_GREATER_EQUAL;
            break;

        case ID_ST_GREATER:
            compFunc = CMPF_GREATER;
            break;

        default:
            break;
        }

        return compFunc;
	}

}

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

#ifndef __CompositorScriptScompiler_H__
#define __CompositorScriptScompiler_H__

#include "OgrePrerequisites.h"
#include "OgreCompiler2Pass.h"
#include "OgreCompositor.h"
#include "OgreRenderSystem.h"


namespace Ogre {

	/** Compiler for parsing & lexing .compositor scripts */
	class _OgreExport CompositorScriptCompiler : public Compiler2Pass
	{

	public:
		CompositorScriptCompiler(void);
		~CompositorScriptCompiler(void);

        /** gets BNF Grammer for Compositor script.
        */
        virtual const String& getClientBNFGrammer(void) { return compositorScript_BNF; }

        /** get the name of the Compositor script BNF grammer.
        */
        virtual const String& getClientGrammerName(void) const { static const String grammerName("Compositor Script"); return grammerName; }
        /** Compile a compositor script from a data stream using a specific resource group name.
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
			// Top level
			ID_COMPOSITOR,
			// Techniques
			ID_TECHNIQUE, ID_TEXTURE, ID_TARGET_WIDTH, ID_TARGET_HEIGHT,
			ID_PF_A8R8G8B8, ID_PF_R8G8B8A8, ID_PF_R8G8B8,
			ID_PF_FLOAT16_R, ID_PF_FLOAT16_RGB, ID_PF_FLOAT16_RGBA,
			ID_PF_FLOAT32_R, ID_PF_FLOAT32_RGB, ID_PF_FLOAT32_RGBA,
			// Targets
			ID_TARGET, ID_INPUT, ID_TARGET_OUTPUT, ID_ONLY_INITIAL,
			ID_VISIBILITY_MASK, ID_LOD_BIAS, ID_MATERIAL_SCHEME,
			ID_PREVIOUS, ID_NONE,
			// Passes
			ID_PASS,
			ID_MATERIAL,
			ID_RENDER_QUAD, ID_CLEAR, ID_STENCIL, ID_RENDER_SCENE,
			ID_FIRST_RQ, ID_LAST_RQ,
			ID_IDENTIFIER,
			// clear
			ID_CLR_BUFF, ID_CLR_COLOUR, ID_CLR_DEPTH,
			ID_CLR_COLOUR_VAL, ID_CLR_DEPTH_VAL, ID_CLR_STENCIL_VAL,
			// stencil
			ID_ST_CHECK, ID_ST_FUNC, ID_ST_REF_VAL, ID_ST_MASK, ID_ST_FAILOP,
			ID_ST_DEPTH_FAILOP, ID_ST_PASSOP, ID_ST_TWOSIDED,

			// compare functions
            ID_ST_ALWAYS_FAIL, ID_ST_ALWAYS_PASS, ID_ST_LESS,
            ID_ST_LESS_EQUAL, ID_ST_EQUAL, ID_ST_NOT_EQUAL,
            ID_ST_GREATER_EQUAL, ID_ST_GREATER,

            // stencil operations
            ID_ST_KEEP, ID_ST_ZERO, ID_ST_REPLACE, ID_ST_INCREMENT,
            ID_ST_DECREMENT, ID_ST_INCREMENT_WRAP, ID_ST_DECREMENT_WRAP,
            ID_ST_INVERT,

			// general
			ID_ON, ID_OFF, ID_TRUE, ID_FALSE
		};

		/** Enum to identify compositor sections. */
		enum CompositorScriptSection
		{
			CSS_NONE,
			CSS_COMPOSITOR,
			CSS_TECHNIQUE,
			CSS_TARGET,
			CSS_PASS
		};
		/** Struct for holding the script context while parsing. */
		struct CompositorScriptContext
		{
			CompositorScriptSection section;
		    String groupName;
			CompositorPtr compositor;
			CompositionTechnique* technique;
			CompositionTargetPass* target;
			CompositionPass* pass;
		};

		CompositorScriptContext mScriptContext;

		// static library database for tokens and BNF rules
		static TokenRule compositorScript_RulePath[];
		// simplified Backus - Naur Form (BNF) grammer for compositor scripts
		static String compositorScript_BNF;

		typedef void (CompositorScriptCompiler::* CSC_Action)(void);
		typedef std::map<size_t, CSC_Action> TokenActionMap;
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
		void addLexemeTokenAction(const String& lexeme, const size_t token, const CSC_Action action = 0);

		void logParseError(const String& error);

		// Token Actions which get called when tokens are created during parsing.
		void parseOpenBrace(void);
		void parseCloseBrace(void);
		void parseCompositor(void);
		void parseTechnique(void);
		void parseTexture(void);
		void parseTarget(void);
		void parseInput(void);
		void parseTargetOutput(void);
		void parseOnlyInitial(void);
		void parseVisibilityMask(void);
		void parseLodBias(void);
		void parseMaterialScheme(void);
		void parsePass(void);
		void parseMaterial(void);
		void parseFirstRenderQueue(void);
		void parseLastRenderQueue(void);
		void parseIdentifier(void);
		void parseClearBuffers(void);
		void parseClearColourValue(void);
		void parseClearDepthValue(void);
		void parseClearStencilValue(void);
		void parseStencilCheck(void);
		void parseStencilFunc(void);
		void parseStencilRefVal(void);
		void parseStencilMask(void);
		void parseStencilFailOp(void);
		void parseStencilDepthFailOp(void);
		void parseStencilPassOp(void);
		void parseStencilTwoSided(void);
		StencilOperation extractStencilOp(void);
        CompareFunction extractCompareFunc(void);
	};
}

#endif

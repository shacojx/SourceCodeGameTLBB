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
#ifndef __GLRenderSystem_H__
#define __GLRenderSystem_H__

#include "OgreGLPrerequisites.h"
#include "OgrePlatform.h"
#include "OgreRenderSystem.h"
#include "OgreGLHardwareBufferManager.h"
#include "OgreGLGpuProgramManager.h"
#include "OgreVector4.h"


namespace Ogre {
    /**
      Implementation of GL as a rendering system.
     */
    class GLRenderSystem : public RenderSystem
    {
    private:
        // Rendering loop control
        bool mStopRendering;

        // Array of up to 8 lights, indexed as per API
        // Note that a null value indicates a free slot
        #define MAX_LIGHTS 8
        Light* mLights[MAX_LIGHTS];

        // clip planes
        typedef std::vector<Vector4> PlaneList2;
        PlaneList2 mClipPlanes;
        void setGLClipPlanes() const;


        // view matrix to set world against
        Matrix4 mViewMatrix;
        Matrix4 mWorldMatrix;
        Matrix4 mTextureMatrix;

        // Last min & mip filtering options, so we can combine them
        FilterOptions mMinFilter;
        FilterOptions mMipFilter;

        // XXX 8 max texture units?
        size_t mTextureCoordIndex[OGRE_MAX_TEXTURE_COORD_SETS];

        /// holds texture type settings for every stage
        GLenum mTextureTypes[OGRE_MAX_TEXTURE_LAYERS];

		/// Number of fixed-function texture units
		unsigned short mFixedFunctionTextureUnits;

        void initConfigOptions(void);
        void initInputDevices(void);
        void processInputDevices(void);

        void setGLLight(size_t index, Light* lt);
        void makeGLMatrix(GLfloat gl_matrix[16], const Matrix4& m);
 
        GLint getBlendMode(SceneBlendFactor ogreBlend) const;
		GLint getTextureAddressingMode(TextureUnitState::TextureAddressingMode tam) const;

        void setLights();

        // Store last depth write state
        bool mDepthWrite;
		// Store last stencil mask state
		uint32 mStencilMask;
		// Store last colour write state
		bool mColourWrite[4];

        GLint convertCompareFunction(CompareFunction func) const;
        GLint convertStencilOp(StencilOperation op, bool invert = false) const;

		// internal method for anisotrophy validation
		GLfloat _getCurrentAnisotropy(size_t unit);
		
        /// GL support class, used for creating windows etc
        GLSupport* mGLSupport;
        
        /// Internal method to set pos / direction of a light
        void setGLLightPositionDirection(Light* lt, GLenum lightindex);

        bool mUseAutoTextureMatrix;
        GLfloat mAutoTextureMatrix[16];

        // check if the GL system has already been initialized
        bool mGLInitialized;
        // Initialise GL system and capabilities
        void initGL(RenderTarget *primary);

        HardwareBufferManager* mHardwareBufferManager;
        GLGpuProgramManager* mGpuProgramManager;

        unsigned short mCurrentLights;

        GLuint getCombinedMinMipFilter(void) const;

        GLGpuProgram* mCurrentVertexProgram;
        GLGpuProgram* mCurrentFragmentProgram;

		/* The main GL context */
        GLContext *mMainContext;
        /* The current GL context */
        GLContext *mCurrentContext;

        /** Manager object for creating render textures.
            Direct render to texture via GL_EXT_framebuffer_object is preferable 
			to pbuffers, which depend on the GL support used and are generally 
			unwieldy and slow. However, FBO support for stencil buffers is poor.
        */
        GLRTTManager *mRTTManager;
    public:
        // Default constructor / destructor
        GLRenderSystem();
        ~GLRenderSystem();

        // ----------------------------------
        // Overridden RenderSystem functions
        // ----------------------------------
        /** See
          RenderSystem
         */
        const String& getName(void) const;
        /** See
          RenderSystem
         */
        ConfigOptionMap& getConfigOptions(void);
        /** See
          RenderSystem
         */
        void setConfigOption(const String &name, const String &value);
        /** See
          RenderSystem
         */
        String validateConfigOptions(void);
        /** See
          RenderSystem
         */
        RenderWindow* initialise(bool autoCreateWindow, const String& windowTitle = "OGRE Render Window");
        /** See
          RenderSystem
         */
        void reinitialise(void); // Used if settings changed mid-rendering
        /** See
          RenderSystem
         */
        void shutdown(void);

        /** See
          RenderSystem
         */
        void setAmbientLight(float r, float g, float b);
        /** See
          RenderSystem
         */
        void setShadingType(ShadeOptions so);
        /** See
          RenderSystem
         */
        void setLightingEnabled(bool enabled);
        
		/// @copydoc RenderSystem::createRenderWindow
		RenderWindow* createRenderWindow(const String &name, unsigned int width, unsigned int height, 
			bool fullScreen, const NameValuePairList *miscParams = 0);

		/// @copydoc RenderSystem::createMultiRenderTarget
		virtual MultiRenderTarget * createMultiRenderTarget(const String & name); 
		
        /** See
          RenderSystem
         */
        void destroyRenderWindow(RenderWindow* pWin);
        /** See
          RenderSystem
         */
        String getErrorDescription(long errorNumber) const;

        /** See
          RenderSystem
         */
        VertexElementType getColourVertexElementType(void) const;
        /** See
          RenderSystem
         */
        void setNormaliseNormals(bool normalise);

        // -----------------------------
        // Low-level overridden members
        // -----------------------------
        /** See
          RenderSystem
         */
        void _useLights(const LightList& lights, unsigned short limit);
        /** See
          RenderSystem
         */
        void _setWorldMatrix(const Matrix4 &m);
        /** See
          RenderSystem
         */
        void _setViewMatrix(const Matrix4 &m);
        /** See
          RenderSystem
         */
        void _setProjectionMatrix(const Matrix4 &m);
        /** See
          RenderSystem
         */
        void _setSurfaceParams(const ColourValue &ambient,
            const ColourValue &diffuse, const ColourValue &specular,
            const ColourValue &emissive, Real shininess,
            TrackVertexColourType tracking);
        /** See
          RenderSystem
         */
		void _setPointParameters(Real size, bool attenuationEnabled, 
			Real constant, Real linear, Real quadratic, Real minSize, Real maxSize);
        /** See
          RenderSystem
         */
		void _setPointSpritesEnabled(bool enabled);
        /** See
          RenderSystem
         */
        void _setTexture(size_t unit, bool enabled, const String &texname);
        /** See
          RenderSystem
         */
        void _setTextureCoordSet(size_t stage, size_t index);
        /** See
          RenderSystem
         */
        void _setTextureCoordCalculation(size_t stage, TexCoordCalcMethod m, 
            const Frustum* frustum = 0);
        /** See
          RenderSystem
         */
        void _setTextureBlendMode(size_t stage, const LayerBlendModeEx& bm);
        /** See
          RenderSystem
         */
        void _setTextureAddressingMode(size_t stage, const TextureUnitState::UVWAddressingMode& uvw);
        /** See
          RenderSystem
         */
        void _setTextureBorderColour(size_t stage, const ColourValue& colour);
        /** See
          RenderSystem
         */
        void _setTextureMatrix(size_t stage, const Matrix4& xform);
        /** See
          RenderSystem
         */
        void _setSceneBlending(SceneBlendFactor sourceFactor, SceneBlendFactor destFactor);
        /** See
          RenderSystem
         */
        void _setAlphaRejectSettings(CompareFunction func, unsigned char value);
        /** See
          RenderSystem
         */
        void _setViewport(Viewport *vp);
        /** See
          RenderSystem
         */
        void _beginFrame(void);
        /** See
          RenderSystem
         */
        void _endFrame(void);
        /** See
          RenderSystem
         */
        void _setCullingMode(CullingMode mode);
        /** See
          RenderSystem
         */
        void _setDepthBufferParams(bool depthTest = true, bool depthWrite = true, CompareFunction depthFunction = CMPF_LESS_EQUAL);
        /** See
          RenderSystem
         */
        void _setDepthBufferCheckEnabled(bool enabled = true);
        /** See
          RenderSystem
         */
        void _setDepthBufferWriteEnabled(bool enabled = true);
        /** See
          RenderSystem
         */
        void _setDepthBufferFunction(CompareFunction func = CMPF_LESS_EQUAL);
        /** See
          RenderSystem
         */
        void _setDepthBias(short bias);
        /** See
          RenderSystem
         */
        void _setColourBufferWriteEnabled(bool red, bool green, bool blue, bool alpha);
		/** See
          RenderSystem
         */
        void _setFog(FogMode mode, const ColourValue& colour, Real density, Real start, Real end);
        /** See
          RenderSystem
         */
        void _convertProjectionMatrix(const Matrix4& matrix,
            Matrix4& dest, bool forGpuProgram = false);
        /** See
          RenderSystem
         */
        void _makeProjectionMatrix(const Radian& fovy, Real aspect, Real nearPlane, Real farPlane, 
            Matrix4& dest, bool forGpuProgram = false);
        /** See
        RenderSystem
        */
        void _makeProjectionMatrix(Real left, Real right, Real bottom, Real top, 
            Real nearPlane, Real farPlane, Matrix4& dest, bool forGpuProgram = false);
        /** See
          RenderSystem
         */
		void _makeOrthoMatrix(const Radian& fovy, Real aspect, Real nearPlane, Real farPlane, 
            Matrix4& dest, bool forGpuProgram = false);
        /** See
        RenderSystem
        */
        void _applyObliqueDepthProjection(Matrix4& matrix, const Plane& plane, 
            bool forGpuProgram);
        /** See
        RenderSystem
        */
        void setClipPlane (ushort index, Real A, Real B, Real C, Real D);
        /** See
        RenderSystem
        */
        void enableClipPlane (ushort index, bool enable);
        /** See
          RenderSystem
         */
        void _setPolygonMode(PolygonMode level);
        /** See
          RenderSystem
         */
        void setStencilCheckEnabled(bool enabled);
        /** See RenderSystem.
         */
        void setStencilBufferParams(CompareFunction func = CMPF_ALWAYS_PASS, 
            uint32 refValue = 0, uint32 mask = 0xFFFFFFFF, 
            StencilOperation stencilFailOp = SOP_KEEP, 
            StencilOperation depthFailOp = SOP_KEEP,
            StencilOperation passOp = SOP_KEEP, 
            bool twoSidedOperation = false);
        /** See
          RenderSystem
         */
        void _setTextureUnitFiltering(size_t unit, FilterType ftype, FilterOptions filter);
        /** See
          RenderSystem
         */
		void _setTextureLayerAnisotropy(size_t unit, unsigned int maxAnisotropy);
        /** See
          RenderSystem
         */
		void setVertexDeclaration(VertexDeclaration* decl);
        /** See
          RenderSystem
         */
		void setVertexBufferBinding(VertexBufferBinding* binding);
        /** See
          RenderSystem
         */
        void _render(const RenderOperation& op);
        /** See
          RenderSystem
         */
        void bindGpuProgram(GpuProgram* prg);
        /** See
          RenderSystem
         */
        void unbindGpuProgram(GpuProgramType gptype);
        /** See
          RenderSystem
         */
        void bindGpuProgramParameters(GpuProgramType gptype, GpuProgramParametersSharedPtr params);
		/** See
		RenderSystem
		*/
		void bindGpuProgramPassIterationParameters(GpuProgramType gptype);
        /** See
          RenderSystem
         */
        void setClipPlanes(const PlaneList& clipPlanes);
        /** See
          RenderSystem
         */
        void setScissorTest(bool enabled, size_t left = 0, size_t top = 0, size_t right = 800, size_t bottom = 600) ;
        void clearFrameBuffer(unsigned int buffers, 
            const ColourValue& colour = ColourValue::Black, 
            Real depth = 1.0f, unsigned short stencil = 0);
        HardwareOcclusionQuery* createHardwareOcclusionQuery(void);
        Real getHorizontalTexelOffset(void);
        Real getVerticalTexelOffset(void);
        Real getMinimumDepthInputValue(void);
        Real getMaximumDepthInputValue(void);

        // ----------------------------------
        // GLRenderSystem specific members
        // ----------------------------------
        /** One time initialization for the RenderState of a context. Things that
            only need to be set once, like the LightingModel can be defined here.
         */
        void _oneTimeContextInitialization();
        /** Switch GL context, dealing with involved internal cached states too
        */
        void _switchContext(GLContext *context);
        /**
         * Set current render target to target, enabling its GL context if needed
         */
        void _setRenderTarget(RenderTarget *target);
        /** Unregister a render target->context mapping. If the context of target 
            is the current context, change the context to the main context so it
            can be destroyed safely. 
            
            @note This is automatically called by the destructor of 
            GLContext.
         */
        void _unregisterContext(GLContext *context);
        /** Get the main context. This is generally the context with which 
            a new context wants to share buffers and textures.
         */
        GLContext *_getMainContext();
    };
}
#endif


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
#ifndef __CompositionTechnique_H__
#define __CompositionTechnique_H__

#include "OgrePrerequisites.h"
#include "OgrePixelFormat.h"
#include "OgreIteratorWrappers.h"

namespace Ogre {
    /** Base composition technique, can be subclassed in plugins.
     */
    class _OgreExport CompositionTechnique
    {
    public:
        CompositionTechnique(Compositor *parent);
        virtual ~CompositionTechnique();

        /// Texture dimension
        class TextureDim
        {
        public:
            int adjust;         // adjustment in pixels 
            Real viewport;      // factor relative to viewport
            Real previous;      // factor relative to previous compositor output

            explicit TextureDim(int _adjust = 0, Real _viewport = 0, Real _previous = 0)
                : adjust(_adjust)
                , viewport(_viewport)
                , previous(_previous)
            {
            }
        };

        /// Local texture definition
        class TextureDefinition
        {
        public:
            String name;
            TextureDim width;       // width of the texture
            TextureDim height;      // height of the texture
            PixelFormat format;

            TextureDefinition(void)
                : name()
                , width()
                , height()
                , format(PF_R8G8B8A8)
            {
            }
        };
        /// Typedefs for several iterators
        typedef std::vector<CompositionTargetPass *> TargetPasses;
        typedef VectorIterator<TargetPasses> TargetPassIterator;
        typedef std::vector<TextureDefinition*> TextureDefinitions;
        typedef VectorIterator<TextureDefinitions> TextureDefinitionIterator;
        
        /** Create a new local texture definition, and return a pointer to it.
            @param name     Name of the local texture
        */
        TextureDefinition *createTextureDefinition(const String &name);
        
        /** Remove and destroy a local texture definition.
        */
        void removeTextureDefinition(size_t idx);
        
        /** Get a local texture definition.
        */
        TextureDefinition *getTextureDefinition(size_t idx);
        
        /** Get the number of local texture definitions.
        */
        size_t getNumTextureDefinitions();
        
        /** Remove all Texture Definitions
        */
        void removeAllTextureDefinitions();
        
        /** Get an iterator over the TextureDefinitions in this Technique. */
        TextureDefinitionIterator getTextureDefinitionIterator(void);
        
        /** Create a new target pass, and return a pointer to it.
        */
        CompositionTargetPass *createTargetPass();
        
        /** Remove a target pass. It will also be destroyed.
        */
        void removeTargetPass(size_t idx);
        
        /** Get a target pass.
        */
        CompositionTargetPass *getTargetPass(size_t idx);
        
        /** Get the number of target passes.
        */
        size_t getNumTargetPasses();
        
        /** Remove all target passes.
        */
        void removeAllTargetPasses();
        
        /** Get an iterator over the TargetPasses in this Technique. */
        TargetPassIterator getTargetPassIterator(void);
        
        /** Get output (final) target pass
         */
        CompositionTargetPass *getOutputTargetPass();
        
        /** Determine if this technique is supported on the current rendering device. 
		@param allowTextureDegradation True to accept a reduction in texture depth
         */
        virtual bool isSupported(bool allowTextureDegradation);
        
        /** Create an instance of this technique.
         */
        virtual CompositorInstance *createInstance(CompositorChain *chain);
        
        /** Destroy an instance of this technique.
         */
        virtual void destroyInstance(CompositorInstance *instance);
        
        /** Get parent object */
        Compositor *getParent();
    private:
        /// Parent compositor
        Compositor *mParent;
        /// Local texture definitions
        TextureDefinitions mTextureDefinitions;
        
        /// Intermediate target passes
        TargetPasses mTargetPasses;
        /// Output target pass (can be only one)
        CompositionTargetPass *mOutputTarget;    

		/// List of instances
		typedef std::vector<CompositorInstance *> Instances;
		Instances mInstances;
    };

}

#endif

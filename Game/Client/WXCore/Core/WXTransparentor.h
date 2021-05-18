/********************************************************************
    filename:   WXTransparentor.h
    
    purpose:    Dealing with auto transparency adjustment.
*********************************************************************/

#ifndef __FAIRYTRANSPARENTOR_H__
#define __FAIRYTRANSPARENTOR_H__

#include "WXPrerequisites.h"

namespace WX {

    class MaterialInstance;

    class EntityTransparentor
    {
    protected:
        Ogre::Entity* mEntity;

        typedef std::vector<MaterialInstance*> MaterialInstanceList;
        typedef std::vector<size_t> IndexList;
        MaterialInstanceList mMaterialInstances;
        IndexList mSubEntityMaterialIndices;

        size_t addMaterial(const Ogre::MaterialPtr& material);

    public:
        EntityTransparentor(void);

        EntityTransparentor(Ogre::Entity* entity);

        ~EntityTransparentor();

        void clear(void);

        void init(Ogre::Entity* entity);

        void apply(Real transparency);
    };

    //////////////////////////////////////////////////////////////////////////

    /** Applies transparency to derived material.
    @remarks
        This method can't support all materials, since the complexity of the material. For now
        only supports opaque, alpha transparent, or add transparent material.
    @param
        origin The origin material
    @param
        derived The clone of origin material that will applied to.
    @param
        transparency The amount transparency to apply.
    @returns
        true If any modification applied to derived material.
    */
    bool applyTransparency(const Ogre::MaterialPtr& origin, const Ogre::MaterialPtr& derived, Real transparency);

}

#endif // 

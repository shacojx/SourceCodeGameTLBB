/********************************************************************
filename:   WXEffectObject.h

purpose:    the object that wrap the effect,so we can put the effect in the scene
*********************************************************************/

#ifndef __EFFECTOBJECT_H__
#define __EFFECTOBJECT_H__

#include "WXPlacementObject.h"

namespace WX {

    class ObjectFactory;
	class Effect;

    class EffectObject : public PlacementObject
    {
    protected:
        class Factory;

    public:
        EffectObject(void);
        virtual ~EffectObject();

        const String& getType(void) const;
        const String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        //////////////////////////////////////////////////////////////////////////

        static const String msType;
        static const String msCategory;

        static ObjectFactory* getFactory(void);

        void setEffectName( const String &name );

        const String &getEffectName(void) const
        {
            return mEffectName;
        }

        //////////////////////////////////////////////////////////////////////////
        Effect * getEffect(void) const
        {
            return mEffect;
        }

    protected:

		/// effect template name
        String mEffectName;
        Effect *mEffect;

    protected:

        void createEffect(void);
        void destroyEffect(void);
    };
}

#endif // __EffectObject_H__

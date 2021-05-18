#ifndef __FAIRYMODIFYOBJECTPROPERTYOPERATOR_H__
#define __FAIRYMODIFYOBJECTPROPERTYOPERATOR_H__

#include "WXOperator.h"

#include "Core/WXVariant.h"

namespace WX
{
    class SceneManipulator;

    class ModifyObjectPropertyOperator : public Operator
    {
    public:
        ModifyObjectPropertyOperator(SceneManipulator* sceneManipulator);
        ~ModifyObjectPropertyOperator();

        void add(const String& object, const String& property, const Variant& oldValue, const Variant& newValue);
        bool empty(void) const;

        /** @copydoc Operator::getGroupName */
        const String& getGroupName(void) const;

        /** @copydoc Operator::getDescription */
        String getDescription(void) const;

        /** @copydoc Operator::getHelp */
        String getHelp(void) const;

        /** @copydoc Operator::undo */
        void undo(void);

        /** @copydoc Operator::redo */
        void redo(void);

    protected:
        SceneManipulator* mSceneManipulator;

        typedef std::pair<Variant, Variant> VariantPair;
        typedef std::map<String, VariantPair> NameVariantPairMap;
        typedef std::map<String, NameVariantPairMap> ObjectPropertiesMap;

        ObjectPropertiesMap mObjectProperties;
    };
}

#endif 

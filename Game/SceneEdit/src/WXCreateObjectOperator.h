#ifndef __FAIRYCREATEOBJECTOPERATOR_H__
#define __FAIRYCREATEOBJECTOPERATOR_H__

#include "WXOperator.h"

#include "Core/WXObject.h"

namespace WX
{
    class SceneManipulator;

    class CreateObjectOperator : public Operator
    {
    public:
        CreateObjectOperator(SceneManipulator* sceneManipulator);
        ~CreateObjectOperator();

        void add(const ObjectPtr& object, bool create);

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
        void apply(bool reserve);

        SceneManipulator* mSceneManipulator;

        struct ObjectInfo
        {
            bool create;
            String type;
            String name;
            PropertyNameVariantPairList properties;
        };

        typedef std::list<ObjectInfo> ObjectInfoList;

        ObjectInfoList mObjectInfos;
    };
}

#endif 

#ifndef __CREATEOBJECTACTION_H__
#define __CREATEOBJECTACTION_H__

#include "Action.h"

#include "Core/WXObject.h"

namespace WX
{

    class CreateObjectAction : public Action
    {
    protected:
        String mObjectType;
        NameValuePairList mParameters;
        bool mDirty;
        ObjectPtr mObject;

    public:
        CreateObjectAction(SceneManipulator* sceneManipulator);
        ~CreateObjectAction();

        const String& getName(void) const;

        void setParameter(const String& name, const String& value);
        String getParameter(const String& name) const;

    protected:
        void _onActive(bool active);

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

    protected:
        void hide(void);
        bool updateInstance(const Point& pt);
    };

}

#endif // __CreateObjectAction_H__

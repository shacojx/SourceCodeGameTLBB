#ifndef __Action_H__
#define __Action_H__

#include "Manip.h"

namespace WX {

    class Action : public Manip
    {
    protected:
        bool mActived;
		void setActive(bool active);

    public:
        Action(SceneManipulator* sceneManipulator);

        virtual void setParameter(const String& name, const String& value);
        virtual String getParameter(const String& name) const;

    protected:
        friend SceneManipulator;

		bool getActive(void) const
        {
            return mActived;
        }

    protected:
        virtual void _onActive(bool active);
    };

}

#endif // __Action_H__

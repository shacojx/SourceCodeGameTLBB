/********************************************************************
filename:   WXLogicModelObject.h

purpose:    把LogicModelObject封装到WXObject系列中
            与actor object类似，都是对LogicModel（原DObject）的封装，
            不同处在于，actor object是从placement object派生，主要
            用于场景物体的摆放，而LogicModelObject直接从object派生，
            自己处理位置，方向等信息，主要用于客户端创建主角，npc，
            怪物等物体。
*********************************************************************/

#ifndef _LOGICMODELOBJECT_H_
#define _LOGICMODELOBJECT_H_

#include "WXObject.h"

namespace WX {

    class ObjectFactory;
    class ObjectProxy;
    class LogicModel;

    class LogicModelObject : public Object
    {
    protected:
        class Factory;

    public:
        LogicModelObject(void);
        virtual ~LogicModelObject();

        const String& getType(void) const;
        const String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);
        void updateRenderInstance(void);

        const String& getLogicModelName(void) const
        {
            return mLogicModelName;
        }

        void setLogicModelName(const String& modelName);

        LogicModel* getLogicModel(void)
        {
            return mLogicModel;
        }

        //////////////////////////////////////////////////////////////////////////

        static const String msType;
        static const String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

    protected:

        void _createLogicModel(void);
        void _destroyLogicModel(void);

    protected:

        ObjectProxy* mProxy;

        LogicModel* mLogicModel;

        Ogre::String mLogicModelName;
    };

}

#endif _LogicModelObject_H_
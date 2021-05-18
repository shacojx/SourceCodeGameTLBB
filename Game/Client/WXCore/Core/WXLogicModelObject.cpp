/********************************************************************
filename:   WXLogicModelObject.cpp

purpose:    把LogicModelObject封装到WXObject系列中
            与actor object类似，都是对LogicModel（原DObject）的封装，
            不同处在于，actor object是从placement object派生，主要
            用于场景物体的摆放，而LogicModelObject直接从object派生，
            自己处理位置，方向等信息，主要用于客户端创建主角，npc，
            怪物等物体。
*********************************************************************/

#include "WXLogicModelObject.h"
#include "WXObjectFactory.h"
#include "WXObjectProxy.h"

#include "ModelSystem/WXLogicModelManager.h"
#include "ModelSystem/WXLogicModel.h"

namespace WX {

    const String LogicModelObject::msType = "LogicModel";
    const String LogicModelObject::msCategory = "LogicModel";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class LogicModelObject::Factory : public ObjectFactory
    {
    public:
        const String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new LogicModelObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////
    namespace
    {
        struct LogicModelNameCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const LogicModelObject* object = static_cast<const LogicModelObject*>(source);
                return object->getLogicModelName();
            }

            String doGetAsString(const void* source) const
            {
                const LogicModelObject* object = static_cast<const LogicModelObject*>(source);
                return object->getLogicModelName();
            }

            void doSet(void* target, const Variant& value)
            {
                LogicModelObject* object = static_cast<LogicModelObject*>(target);
                String actorName = VariantCast<String>(value);
                object->setLogicModelName(actorName);
            }

            void doSetAsString(void* target, const String& value)
            {
                LogicModelObject* object = static_cast<LogicModelObject*>(target);
                object->setLogicModelName(value);
            }
        };

    }

    //////////////////////////////////////////////////////////////////////////
    LogicModelObject::LogicModelObject() :
    mLogicModel(NULL),
    mLogicModelName(""),
    mProxy(NULL)
    {
        if (createPropertyDictionary("LogicModel"))
        {
            static LogicModelNameCmd logicModelNameCmd;
            defineProperty(
                "logic model name",
                "The logic model filename use to create this logic model.",
                "LogicModelName",
                &logicModelNameCmd,
                PF_PRIMARY);
        }
    }

    LogicModelObject::~LogicModelObject()
    {
        destroyRenderInstance();
    }

    //-----------------------------------------------------------------------
    const String& LogicModelObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const String& LogicModelObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void LogicModelObject::createRenderInstance(System* system)
    {
        assert(system);
        assert(!mLogicModel);
        assert(!mProxy);

        mProxy = new ObjectProxy(shared_from_this());

        if (!mLogicModelName.empty())
        {
            _createLogicModel();
        }
    }
    //-----------------------------------------------------------------------
    void LogicModelObject::destroyRenderInstance(void)
    {
        if (mLogicModel)
        {
            _destroyLogicModel();
        }

        if (mProxy)
        {
            delete mProxy;
            mProxy = NULL;
        }
    }
    //-----------------------------------------------------------------------
    void LogicModelObject::setLogicModelName(const String& modelName)
    {
        if (mLogicModelName != modelName)
        {
            if (mLogicModel)
            {
                _destroyLogicModel();
            }

            mLogicModelName = modelName;

            if (mProxy && !mLogicModelName.empty())
            {
                _createLogicModel();
            }
        }
    }
    //-----------------------------------------------------------------------
    ObjectFactory* LogicModelObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }
    //-----------------------------------------------------------------------
    void LogicModelObject::_createLogicModel(void)
    {
        assert(!mLogicModel);
        assert(mProxy && !mLogicModelName.empty());

        mLogicModel = LogicModelManager::getSingleton().createLogicModel(mLogicModelName);
        mLogicModel->setUserObject(mProxy);
    }
    //-----------------------------------------------------------------------
    void LogicModelObject::_destroyLogicModel(void)
    {
        assert (mLogicModel);

        mLogicModel->setUserObject(NULL);
        LogicModelManager::getSingleton().destroyLogicModel(mLogicModel);
        mLogicModel = NULL;
    }
}
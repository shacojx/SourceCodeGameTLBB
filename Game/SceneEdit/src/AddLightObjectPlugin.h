#ifndef __ADDLIGHTOBJECTPLUGIN_H__
#define __ADDLIGHTOBJECTPLUGIN_H__

#include "WXSceneListener.h"

namespace WX {

    class SceneManipulator;

    /** 一个监听者类，负责更新场景中的所有光源在编辑器中的指示器（一个模型
        在SceneManipulator中创建
    */
    class AddLightObjectPlugin : public SceneListener
    {
    public:

        AddLightObjectPlugin(WX::SceneManipulator* sceneManipulator);

        virtual ~AddLightObjectPlugin();

        //////////////////////////////////////////////////////////////////////////
        virtual void onSceneReset(void);
        virtual void onAddObject(const ObjectPtr& object);
        virtual void onRemoveObject(const ObjectPtr& object);
        virtual void onSelectObject(const ObjectPtr& object);
        virtual void onDeselectObject(const ObjectPtr& object);
        virtual void onDeselectAllObjects(void);
        virtual void onObjectPropertyChanged(const ObjectPtr& object, const String& name);

    protected:

        // 类的前向声明，光源的指示器
        class Indicator;

        typedef std::map<ObjectPtr, Indicator *> Indicators;
        Indicators mIndicators;

        WX::SceneManipulator* mSceneManipulator;

	protected:

		void clearIndicators(void);
    };
}
#endif // __AddLightObjectPlugin_H__
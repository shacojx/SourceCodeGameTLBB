#ifndef __ADDLIQUIDOBJECTPLUGIN_H__
#define __ADDLIQUIDOBJECTPLUGIN_H__

#include "WXSceneListener.h"

namespace WX {

	class SceneManipulator;

	/** 一个监听者类，负责更新场景中的所有水面在编辑器中的指示器
	在SceneManipulator中创建
	*/
	class AddLiquidObjectPlugin : public SceneListener
	{
	public:

		AddLiquidObjectPlugin(WX::SceneManipulator* sceneManipulator);

		virtual ~AddLiquidObjectPlugin();

		//////////////////////////////////////////////////////////////////////////
		virtual void onSceneReset(void);
		virtual void onAddObject(const ObjectPtr& object);
		virtual void onRemoveObject(const ObjectPtr& object);
		virtual void onSelectObject(const ObjectPtr& object);
		virtual void onDeselectObject(const ObjectPtr& object);
		virtual void onDeselectAllObjects(void);
		virtual void onObjectPropertyChanged(const ObjectPtr& object, const String& name);

	protected:

		class Indicator;

		typedef std::map<ObjectPtr, Indicator *> Indicators;
		Indicators mIndicators;

		WX::SceneManipulator* mSceneManipulator;
	};
}
#endif 
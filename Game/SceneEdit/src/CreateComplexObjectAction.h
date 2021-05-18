#ifndef __CREATECOMPLEXOBJECTACTION_H__
#define __CREATECOMPLEXOBJECTACTION_H__

#include "Action.h"

#include "Core/WXObject.h"

namespace WX
{

	/** 类似于CreateObjectAction，但是只有当鼠标左键按下时才真正地创建物体，而不是在鼠标移动的时候也创建（更新）
		物体，因为用这个action来创建的物体一般都是比较复杂的，如水面等区域性的物体
	*/
	class CreateComplexObjectAction : public Action
	{
	protected:
		String mObjectType;
		NameValuePairList mParameters;
		bool mDirty;
		ObjectPtr mObject;

	public:
		CreateComplexObjectAction(SceneManipulator* sceneManipulator);
		~CreateComplexObjectAction();

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

#endif // __CreateComplexObjectAction_H__

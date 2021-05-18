#ifndef __IntersectPoint_H__
#define __IntersectPoint_H__

#include "HitIndicator.h"

namespace WX {

    class IntersectPoint : public HitIndicator
    {
    protected:
        Ogre::SceneNode* mIntersectNode;
        Ogre::Entity* mIntersectEntity;

    public:
        IntersectPoint(SceneManipulator* sceneManipulator);
        ~IntersectPoint();

        const String& getName(void) const;

        void reshape(void);
        void hide(void);

		// 设置碰撞小球原来的材质
		void SetMaterial();

		// 设置碰撞小球的新材质.
		void SetMaterial(String& strMaterialName);
	
	

    protected:
        void _setHitPoint(Real x, Real y);
    };

}

#endif // __IntersectPoint_H__

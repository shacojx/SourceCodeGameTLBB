#ifndef __JunctionPoints_H__
#define __JunctionPoints_H__

#include "HitIndicator.h"
#include <OgreMesh.h>

namespace WX {

    class JunctionPoints : public HitIndicator
    {
        struct vectorLess
        {
            bool operator() (const Ogre::Vector4& a, const Ogre::Vector4& b) const
            {
                if (a.x < b.x) return true;
                if (b.x < a.x) return false;
                if (a.y < b.y) return true;
                if (b.y < a.y) return false;
                if (a.z < b.z) return true;
                if (b.z < a.z) return false;
				if (a.w < b.w) return true;
				if (b.w < a.w) return false;

                return false;
            }
        };

    protected:
        Ogre::MeshPtr mBaseMesh;
        Real mScale;

		// 改用Vector4来保存一个节点的信息，xyz保存节点的位置，w保存节点的权重
        typedef std::map<Ogre::Vector4, Ogre::SceneNode*, vectorLess> DisplayNodes;
        typedef std::list<Ogre::SceneNode*> FreeNodes;
        DisplayNodes mDisplayNodes;
        FreeNodes mFreeNodes;

    public:
        JunctionPoints(SceneManipulator* sceneManipulator);
        ~JunctionPoints();

        const String& getName(void) const;

        void reshape(void);
        void hide(void);
        void refresh(void);

    protected:

        void _setHitPoint(Real x, Real y);

        Ogre::SceneNode* allocNode(void);
        void freeNode(Ogre::SceneNode* node);
    };

}

#endif // __JunctionPoints_H__

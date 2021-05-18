#ifndef __ScorpioCollision_H__
#define __ScorpioCollision_H__

#include "ScorpioPrerequisites.h"

// Forware declaration
class PS_Collider;
class PS_ColliderSet;

namespace Ogre
{
    class UserCollideTester
    {
    public:
        virtual ~UserCollideTester() {}

    public:
        virtual bool _support(const Vector3& point, Vector3& position, Vector3& normal) = 0;
    };

    class Collider
    {
    public:
        virtual ~Collider() {}
        virtual PS_Collider* _getImplementation(void) const = 0;

    public:
        virtual const String& getType(void) const = 0;
    };

    class StdCollider : public Collider
    {
    public:
        virtual void setPosition(const Vector3& position) = 0;
        virtual Vector3 getPosition(void) const = 0;
        virtual void setRotation(const Quaternion& rotation) = 0;
        virtual Quaternion getRotation(void) const = 0;
    };

    class UserCollider : public Collider
    {
    public:
        virtual void setTester(UserCollideTester* tester) = 0;
        virtual UserCollideTester* getTester(void) const = 0;
    };

    class ColliderSet
    {
    public:
        virtual ~ColliderSet() {}
        virtual PS_ColliderSet* _getImplementation(void) const = 0;

    public:
        virtual void addCollider(Collider* collider) = 0;
        virtual void removeCollider(Collider* collider) = 0;
        virtual size_t getNumColliders(void) const = 0;
        virtual Collider* getCollider(size_t i) const = 0;
    };

    class CollisionManager
    {
    public:
        virtual ~CollisionManager() {}

    public:
        virtual ColliderSet* createColliderSet(void) = 0;
        virtual void destroyColliderSet(ColliderSet* colliderSet) = 0;

        virtual UserCollider* createUserCollider(UserCollideTester* tester) = 0;
        virtual StdCollider* createBoxCollider(const Vector3& extents) = 0;
        virtual StdCollider* createPlaneCollider(void) = 0;
        virtual StdCollider* createSphereCollider(Real radius) = 0;
        virtual void destroyCollider(Collider* collider) = 0;
    };

    typedef CollisionManager* (*CreateCollisionManagerProc)(void);
    typedef void (*DestroyCollisionManagerProc)(CollisionManager*);

    #define SCORPIO_CREATECOLLISIONMANAGERPROC_NAME     "createCollisionManager"
    #define SCORPIO_DESTROYCOLLISIONMANAGERPROC_NAME    "destroyCollisionManager"
}

#endif // __ScorpioCollision_H__

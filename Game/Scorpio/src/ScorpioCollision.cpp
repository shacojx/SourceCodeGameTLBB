#include "ScorpioCollision.h"

#include "PhysicsSimulation/PS_ColliderSet.h"
#include "PhysicsSimulation/PS_Collider.h"
#include "PhysicsSimulation/PS_BoxCollider.h"
#include "PhysicsSimulation/PS_PlaneCollider.h"
#include "PhysicsSimulation/PS_SphereCollider.h"

#include <OgreVector3.h>
#include <OgreMatrix3.h>
#include <OgreQuaternion.h>

namespace Ogre
{
    template <typename Impl>
    class StdColliderImpl : public StdCollider, protected Impl
    {
    public:
        StdColliderImpl(void)
            : StdCollider()
            , Impl()
        {
        }

        template <typename VT>
        StdColliderImpl(const VT& v)
            : StdCollider()
            , Impl(v)
        {
        }

        PS_Collider* _getImplementation(void) const
        {
            return const_cast<StdColliderImpl*>(this);
        }

        const String& getType(void) const
        {
            static const String type("StdCollider");
            return type;
        }

        void setPosition(const Vector3& pos)
        {
            this->Impl::setPosition(PS_Vector3(pos.x, pos.y, pos.z));
        }

        Vector3 getPosition(void) const
        {
            const PS_Vector3& pos = this->Impl::getPosition();
            return Vector3(pos.x, pos.y, pos.z);
        }

        void setRotation(const Quaternion& rot)
        {
            Matrix3 m;
            rot.ToRotationMatrix(m);
            this->Impl::setRotation(PS_Matrix3x3(
                m[0][0], m[0][1], m[0][2],
                m[1][0], m[1][1], m[1][2],
                m[2][0], m[2][1], m[2][2]));
        }

        Quaternion getRotation(void) const
        {
            const PS_Matrix3x3& m = this->Impl::getRotation();
            return Matrix3(
                m[0][0], m[0][1], m[0][2],
                m[1][0], m[1][1], m[1][2],
                m[2][0], m[2][1], m[2][2]);
        }
    };
    //-----------------------------------------------------------------------
    class UserColliderImpl : public UserCollider, protected PS_Collider
    {
    protected:
        UserCollideTester* mTester;

    public:
        UserColliderImpl(UserCollideTester* tester)
            : mTester(tester)
        {
            assert(tester);
        }
   
        PS_Collider* _getImplementation(void) const
        {
            return const_cast<UserColliderImpl*>(this);
        }

        const String& getType(void) const
        {
            static const String type("UserCollider");
            return type;
        }

        void setTester(UserCollideTester* tester)
        {
            assert(tester);
            mTester = tester;
        }

        UserCollideTester* getTester(void) const
        {
            return mTester;
        }

        /// Overriden PS_Collider
        bool _support(const PS_Vector3& point, PS_Vector3& position, PS_Vector3& normal) const
        {
            if (!mTester)
                return false;

            Vector3 pos, norm;
            if (!mTester->_support(Vector3(point.x, point.y, point.z), pos, norm))
                return false;

            position = PS_Vector3(pos.x, pos.y, pos.z);
            normal = PS_Vector3(norm.x, norm.y, norm.z);
            return true;
        }
    };
    //-----------------------------------------------------------------------
    class ColliderSetImpl : public ColliderSet, protected PS_ColliderSet
    {
    protected:
        std::list<Collider*> mCollider;

    public:
        PS_ColliderSet* _getImplementation(void) const
        {
            return const_cast<ColliderSetImpl*>(this);
        }

        void addCollider(Collider* collider)
        {
            PS_ColliderSet::addCollider(collider->_getImplementation());
            mCollider.push_back(collider);
        }

        void removeCollider(Collider* collider)
        {
            PS_ColliderSet::removeCollider(collider->_getImplementation());
            mCollider.remove(collider);
        }

        size_t getNumColliders(void) const
        {
            return mCollider.size();
        }

        Collider* getCollider(size_t i) const
        {
            assert(i < mCollider.size() && "Index out of bound");
            std::list<Collider*>::const_iterator it = mCollider.begin();
            std::advance(it, i);
            return *it;
        }
    };
    //-----------------------------------------------------------------------
    class CollisionManagerImpl : public CollisionManager
    {
    protected:
        typedef std::set<ColliderSet*> ColliderSetList;
        typedef std::set<Collider*> ColliderList;

        ColliderSetList mColliderSetList;
        ColliderList mColliderList;

        template <typename T>
        T* _addCollider(T* collider)
        {
            mColliderList.insert(collider);
            return collider;
        }

    public:
        ~CollisionManagerImpl()
        {
            for (ColliderSetList::const_iterator si = mColliderSetList.begin(); si != mColliderSetList.end(); ++si)
            {
                delete *si;
            }

            for (ColliderList::const_iterator ci = mColliderList.begin(); ci != mColliderList.end(); ++ci)
            {
                delete *ci;
            }
        }

    public:
        ColliderSet* createColliderSet(void)
        {
            ColliderSet* result = new ColliderSetImpl;
            mColliderSetList.insert(result);
            return result;
        }

        void destroyColliderSet(ColliderSet* colliderSet)
        {
            ColliderSetList::iterator it = mColliderSetList.find(colliderSet);
            if (it != mColliderSetList.end())
            {
                mColliderSetList.erase(it);
                delete colliderSet;
            }
        }

        UserCollider* createUserCollider(UserCollideTester* tester)
        {
            return _addCollider(new UserColliderImpl(tester));
        }

        StdCollider* createBoxCollider(const Vector3& extents)
        {
            return _addCollider(new StdColliderImpl<PS_BoxCollider>(PS_Vector3(extents.x, extents.y, extents.z)));
        }

        StdCollider* createPlaneCollider(void)
        {
            return _addCollider(new StdColliderImpl<PS_PlaneCollider>());
        }

        StdCollider* createSphereCollider(Real radius)
        {
            return _addCollider(new StdColliderImpl<PS_SphereCollider>(PS_Scalar(radius)));
        }

        void destroyCollider(Collider* collider)
        {
            ColliderList::iterator it = mColliderList.find(collider);
            if (it != mColliderList.end())
            {
                mColliderList.erase(it);
                delete collider;
            }
        }
    };
    //-----------------------------------------------------------------------
    _OgreExport CollisionManager* createCollisionManager(void)
    {
        return new CollisionManagerImpl;
    }
    //-----------------------------------------------------------------------
    void _OgreExport destroyCollisionManager(CollisionManager* collisionManager)
    {
        delete collisionManager;
    }
}

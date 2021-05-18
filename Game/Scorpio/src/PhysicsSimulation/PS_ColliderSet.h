#ifndef __PS_ColliderSet_H__
#define __PS_ColliderSet_H__

#include "PS_Prerequisites.h"

class PS_ColliderSet
{
public:
    PS_ColliderSet(void);
    virtual ~PS_ColliderSet();

    typedef std::list<PS_Collider*> ColliderList;

    void addCollider(PS_Collider* collider);
    void removeCollider(PS_Collider* collider);

    const ColliderList& getColliderList(void) const;

protected:
    ColliderList mColliderList;
};

#endif // __PS_ColliderSet_H__

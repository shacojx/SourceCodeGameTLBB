#include "PS_ColliderSet.h"

//------------------------------------------------------------------------------
PS_ColliderSet::PS_ColliderSet(void)
    : mColliderList()
{
}
//------------------------------------------------------------------------------
PS_ColliderSet::~PS_ColliderSet()
{
}
//------------------------------------------------------------------------------
void PS_ColliderSet::addCollider(PS_Collider* collider)
{
    mColliderList.push_back(collider);
}
//------------------------------------------------------------------------------
void PS_ColliderSet::removeCollider(PS_Collider* collider)
{
    mColliderList.remove(collider);
}
//------------------------------------------------------------------------------
const PS_ColliderSet::ColliderList& PS_ColliderSet::getColliderList(void) const
{
    return mColliderList;
}

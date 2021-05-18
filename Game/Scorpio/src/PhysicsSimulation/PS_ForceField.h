#ifndef __PS_ForceField_H__
#define __PS_ForceField_H__

#include "PS_Mass.h"
#include "PS_Motion.h"
#include "PS_Force.h"
#include "PS_SharedPtr.h"

class PS_ForceField
{
public:
    PS_ForceField(void) {}
    virtual ~PS_ForceField() {}

    virtual void prepare(size_t count, const PS_Mass* masses, const PS_Motion* motions, bool derived) {}

    virtual void apply(bool derived,
                       size_t count,
                       const PS_Mass* masses, const PS_Motion* motions,
                       PS_Force* forces) = 0;
};

typedef PS_SharedPtr<PS_ForceField> PS_ForceFieldPtr;

typedef std::list<PS_ForceFieldPtr> PS_ForceFields;

#endif // __PS_ForceField_H__

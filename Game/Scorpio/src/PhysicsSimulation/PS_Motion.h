#ifndef __PS_Motion_H__
#define __PS_Motion_H__

#include "PS_Vector3.h"

class PS_Motion
{
public:
    PS_Vector3 position;
    PS_Vector3 velocity;

    ////////////////////////////////////////////////////////
    // Special constants

    static const PS_Motion ZERO;

    ////////////////////////////////////////////////////////
    // Constructors

    PS_Motion(void)
    {
    }

    PS_Motion(const PS_Vector3& position, const PS_Vector3& velocity)
        : position(position)
        , velocity(velocity)
    {
    }

    PS_Motion(const PS_Motion& other)
        : position(other.position)
        , velocity(other.velocity)
    {
    }

    PS_Motion& operator= (const PS_Motion& other)
    {
        this->position = other.position;
        this->velocity = other.velocity;
        return *this;
    }

    ////////////////////////////////////////////////////////
    // Accessors

    void setPosition(const PS_Vector3& position)
    {
        this->position = position;
    }

    void setVelocity(const PS_Vector3& velocity)
    {
        this->velocity = velocity;
    }

    const PS_Vector3& getPosition(void) const
    {
        return position;
    }

    const PS_Vector3& getVelocity(void) const
    {
        return velocity;
    }
};

#endif // __PS_Motion_H__

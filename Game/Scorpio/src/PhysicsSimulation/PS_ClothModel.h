#ifndef __PS_ClothModel_H__
#define __PS_ClothModel_H__

#include "PS_Simulator.h"

class PS_ClothModel
{
public:
    PS_ClothModel(void);
    virtual ~PS_ClothModel();

    void setGravity(const PS_Vector3& acceleration);
    const PS_Vector3& getGravity(void) const;

    void setSpringConstant(PS_Scalar springConstant);
    PS_Scalar getSpringConstant(void) const { return mSpringConstant; }

    void setFlexuralRigidity(PS_Scalar flexuralRigidity);
    PS_Scalar getFlexuralRigidity(void) const { return mFlexuralRigidity; }
    void setImperfectionConstant(PS_Scalar imperfectionConstant);
    PS_Scalar getImperfectionConstant(void) const { return mImperfectionConstant; }

    void setDampingTermConstant(PS_Scalar dampingTermConstant);
    PS_Scalar getDampingTermConstant(void) const { return mDampingTermConstant; }

    const PS_Simulator& getSimulator(void) const;
    PS_Simulator& getSimulator(void);

    void update(PS_Scalar dt);

    void addSequentialPair(size_t a, size_t b, PS_Scalar restLength);
    void addInterlacedPair(size_t a, size_t b, PS_Scalar restLength);

    void addGrid(size_t width, size_t height, PS_Scalar naturalLength, PS_Scalar mass);

protected:
    PS_Simulator mSimulator;
    PS_IntegratorPtr mIntegrator;
    PS_SharedPtr<PS_GravityForceField> mGravity;
    std::list< PS_SharedPtr<PS_StretchingForceField> > mStretchingForceFields;
    std::list< PS_SharedPtr<PS_BucklingForceField> > mBucklingForceFields;
    std::list< PS_SharedPtr<PS_DampingForceField> > mDampingForceFields;

    PS_Scalar mSpringConstant;
    PS_Scalar mFlexuralRigidity;
    PS_Scalar mImperfectionConstant;
    PS_Scalar mDampingTermConstant;
};

#endif // __PS_ClothModel_H__

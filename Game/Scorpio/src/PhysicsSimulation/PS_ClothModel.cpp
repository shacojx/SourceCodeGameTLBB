#include "PS_ClothModel.h"
#include "PS_RK4Integrator.h"
#include "PS_RK2Integrator.h"
#include "PS_VerletIntegrator.h"
#include "PS_EulerIntegrator.h"
#include "PS_GravityForceField.h"
#include "PS_StretchingForceField.h"
#include "PS_BucklingForceField.h"
#include "PS_DampingForceField.h"

PS_ClothModel::PS_ClothModel(void)
    : mSimulator()
    , mIntegrator(new PS_RK4Integrator)
    , mGravity(new PS_GravityForceField(PS_Vector3(0, -9.81f, 0)))
    , mSpringConstant(0.0f)
    , mFlexuralRigidity(0.0f)
    , mImperfectionConstant(0.0f)
    , mDampingTermConstant(0.0f)
{
    // Sets default integrator
    mSimulator.setIntegrator(mIntegrator);

    // Add gravity
    mSimulator.addForceField(mGravity);
}

PS_ClothModel::~PS_ClothModel()
{
}

void PS_ClothModel::setGravity(const PS_Vector3& acceleration)
{
    mGravity->setAcceleration(acceleration);
}

const PS_Vector3& PS_ClothModel::getGravity(void) const
{
    return mGravity->getAcceleration();
}

void PS_ClothModel::setSpringConstant(PS_Scalar springConstant)
{
    assert(springConstant >= 0);
    if (mSpringConstant != springConstant)
    {
        mSpringConstant = springConstant;

        std::list< PS_SharedPtr<PS_StretchingForceField> >::const_iterator it;
        for (it = mStretchingForceFields.begin(); it != mStretchingForceFields.end(); ++it)
        {
            (*it)->setSpringConstant(springConstant);
        }
    }
}

void PS_ClothModel::setFlexuralRigidity(PS_Scalar flexuralRigidity)
{
    assert(flexuralRigidity >= 0);
    if (mFlexuralRigidity != flexuralRigidity)
    {
        mFlexuralRigidity = flexuralRigidity;

        std::list< PS_SharedPtr<PS_BucklingForceField> >::const_iterator it;
        for (it = mBucklingForceFields.begin(); it != mBucklingForceFields.end(); ++it)
        {
            (*it)->setFlexuralRigidity(flexuralRigidity);
        }
    }
}

void PS_ClothModel::setImperfectionConstant(PS_Scalar imperfectionConstant)
{
    assert(imperfectionConstant >= 0);
    if (mImperfectionConstant != imperfectionConstant)
    {
        mImperfectionConstant = imperfectionConstant;

        std::list< PS_SharedPtr<PS_BucklingForceField> >::const_iterator it;
        for (it = mBucklingForceFields.begin(); it != mBucklingForceFields.end(); ++it)
        {
            (*it)->setImperfectionConstant(imperfectionConstant);
        }
    }
}

void PS_ClothModel::setDampingTermConstant(PS_Scalar dampingTermConstant)
{
    assert(dampingTermConstant >= 0);
    if (mDampingTermConstant != dampingTermConstant)
    {
        mDampingTermConstant = dampingTermConstant;

        std::list< PS_SharedPtr<PS_DampingForceField> >::const_iterator it;
        for (it = mDampingForceFields.begin(); it != mDampingForceFields.end(); ++it)
        {
            (*it)->setDampingTermConstant(dampingTermConstant);
        }
    }
}

const PS_Simulator& PS_ClothModel::getSimulator(void) const
{
    return mSimulator;
}

PS_Simulator& PS_ClothModel::getSimulator(void)
{
    return mSimulator;
}

void PS_ClothModel::update(PS_Scalar dt)
{
    if (dt > 0)
    {
        mSimulator.updateSimulation(dt);
    }
}

void PS_ClothModel::addSequentialPair(size_t a, size_t b, PS_Scalar restLength)
{
    PS_SharedPtr<PS_StretchingForceField> stretchingForceField(
        new PS_StretchingForceField(a, b, restLength, mSpringConstant));
    mStretchingForceFields.push_back(stretchingForceField);
    mSimulator.addForceField(stretchingForceField);

    PS_SharedPtr<PS_DampingForceField> dampingForceField(
        new PS_DampingForceField(a, b, mDampingTermConstant));
    mDampingForceFields.push_back(dampingForceField);
    mSimulator.addForceField(dampingForceField);
}

void PS_ClothModel::addInterlacedPair(size_t a, size_t b, PS_Scalar restLength)
{
    PS_SharedPtr<PS_BucklingForceField> bucklingForceField(
        new PS_BucklingForceField(a, b, restLength, mFlexuralRigidity, mImperfectionConstant));
    mBucklingForceFields.push_back(bucklingForceField);
    mSimulator.addForceField(bucklingForceField);
}

void PS_ClothModel::addGrid(size_t width, size_t height, PS_Scalar naturalLength, PS_Scalar mass)
{
    assert(width > 2 && height > 2);
    assert(naturalLength > 0 && mass > 0);

    //------------------------

    size_t p = mSimulator.getParticleCount();

    // Increase particle count
    mSimulator.setParticleCount(p + width * height);

    PS_Scalar diagonalLength = PS_Math::Sqrt(2) * naturalLength;
    for (size_t j = 0; j < height; ++j)
    {
        for (size_t i = 0; i < width; ++i, ++p)
        {
            // Set particle mass
            mSimulator.setParticleMass(p, mass);

            // Add sequential connections
            if (i > 0)
            {
                addSequentialPair(p, p-1, naturalLength);
            }
            if (j > 0)
            {
                addSequentialPair(p, p-width, naturalLength);
                if (i > 0)
                {
                    addSequentialPair(p, p-width-1, diagonalLength);
                }
                if (i < width-1)
                {
                    addSequentialPair(p, p-width+1, diagonalLength);
                }
            }

            // Add interlaced connections
            if (i > 1)
            {
                addInterlacedPair(p, p-2, naturalLength * 2);
            }
            if (j > 1)
            {
                addInterlacedPair(p, p-2*width, naturalLength * 2);
                if (i > 1)
                {
                    addInterlacedPair(p, p-2*width-2, diagonalLength * 2);
                }
                if (i < width-2)
                {
                    addInterlacedPair(p, p-2*width+2, diagonalLength * 2);
                }
            }
        }
    }
}

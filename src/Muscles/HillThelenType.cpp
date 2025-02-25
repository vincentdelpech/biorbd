#define BIORBD_API_EXPORTS
#include "Muscles/HillThelenType.h"

#include <math.h>
#include "Utils/String.h"
#include "Muscles/Geometry.h"
#include "Muscles/Caracteristics.h"

biorbd::muscles::HillThelenType::HillThelenType() :
    biorbd::muscles::HillType()
{
    setType();
}
biorbd::muscles::HillThelenType::HillThelenType(
        const biorbd::utils::String& name,
        const biorbd::muscles::Geometry& geometry,
        const biorbd::muscles::Caracteristics& caract) :
    biorbd::muscles::HillType (name, geometry, caract)
{
    setType();
}

biorbd::muscles::HillThelenType::HillThelenType(
        const biorbd::utils::String &name,
        const biorbd::muscles::Geometry &geometry,
        const biorbd::muscles::Caracteristics &caract,
        const biorbd::muscles::StateDynamics &dynamicState) :
    biorbd::muscles::HillType (name, geometry, caract, dynamicState)
{
    setType();
}

biorbd::muscles::HillThelenType::HillThelenType(
        const biorbd::utils::String &name,
        const biorbd::muscles::Geometry &geometry,
        const biorbd::muscles::Caracteristics &caract,
        const biorbd::muscles::PathChangers &pathChangers) :
    biorbd::muscles::HillType (name, geometry, caract, pathChangers)
{
    setType();
}

biorbd::muscles::HillThelenType::HillThelenType(
        const biorbd::utils::String& name,
        const biorbd::muscles::Geometry& geometry,
        const biorbd::muscles::Caracteristics& caract,
        const biorbd::muscles::PathChangers & wrapper,
        const biorbd::muscles::StateDynamics & state) :
    biorbd::muscles::HillType (name, geometry, caract, wrapper, state)
{
    setType();
}

biorbd::muscles::HillThelenType::HillThelenType(
        const biorbd::muscles::Muscle &muscle) :
    biorbd::muscles::HillType (muscle)
{

}

biorbd::muscles::HillThelenType::HillThelenType(
        const std::shared_ptr<biorbd::muscles::Muscle> muscle) :
    biorbd::muscles::HillType(muscle)
{

}

biorbd::muscles::HillThelenType biorbd::muscles::HillThelenType::DeepCopy() const
{
    biorbd::muscles::HillThelenType copy;
    copy.DeepCopy(*this);
    return copy;
}

void biorbd::muscles::HillThelenType::DeepCopy(const biorbd::muscles::HillThelenType &other)
{
    biorbd::muscles::HillType::DeepCopy(other);
}

void biorbd::muscles::HillThelenType::computeFlPE(){
    if (position().length() > caract().tendonSlackLength())
        *m_FlPE = (exp( *m_cste_FlPE_1 * (position().length()/caract().optimalLength()-1)) -1)
            /
            (exp( *m_cste_FlPE_2 )-1); //Thelen2003, le 07 août 2019
    else
        *m_FlPE = 0;
}

void biorbd::muscles::HillThelenType::computeFlCE(const biorbd::muscles::StateDynamics&){
    *m_FlCE = exp( -pow(((position().length() / caract().optimalLength())-1), 2 ) /  *m_cste_FlCE_2 ); //Thelen2003, le 26 fevrier 2018
}

void biorbd::muscles::HillThelenType::setType()
{
    *m_type = biorbd::muscles::MUSCLE_TYPE::HILL_THELEN;
}

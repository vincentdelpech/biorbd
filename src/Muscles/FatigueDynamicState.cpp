#define BIORBD_API_EXPORTS
#include "Muscles/FatigueDynamicState.h"

#include "Utils/Error.h"

biorbd::muscles::FatigueDynamicState::FatigueDynamicState(
        double active,
        double fatigued,
        double resting) :
    biorbd::muscles::FatigueState(active,fatigued,resting),
    m_activeFibersDot(std::make_shared<double>(0)),
    m_fatiguedFibersDot(std::make_shared<double>(0)),
    m_restingFibersDot(std::make_shared<double>(0))
{
    setType();
}

biorbd::muscles::FatigueDynamicState::FatigueDynamicState(const std::shared_ptr<biorbd::muscles::FatigueState> m) :
    biorbd::muscles::FatigueState(m)
{
    std::shared_ptr<biorbd::muscles::FatigueDynamicState> muscle_tp(std::dynamic_pointer_cast<biorbd::muscles::FatigueDynamicState>(m));
    if (!muscle_tp)
        biorbd::utils::Error::raise("This is not a dynamically fatigable muscle");
    m_activeFibersDot = muscle_tp->m_activeFibersDot;
    m_fatiguedFibersDot = muscle_tp->m_fatiguedFibersDot;
    m_restingFibersDot = muscle_tp->m_restingFibersDot;
}

void biorbd::muscles::FatigueDynamicState::DeepCopy(const biorbd::muscles::FatigueDynamicState &other)
{
    biorbd::muscles::FatigueState::DeepCopy(other);
    *m_activeFibersDot = *other.m_activeFibersDot;
    *m_fatiguedFibersDot = *other.m_fatiguedFibersDot;
    *m_restingFibersDot = *other.m_restingFibersDot;
}

double biorbd::muscles::FatigueDynamicState::activeFibersDot() const
{
    return *m_activeFibersDot;
}

double biorbd::muscles::FatigueDynamicState::fatiguedFibersDot() const
{
    return *m_fatiguedFibersDot;
}

double biorbd::muscles::FatigueDynamicState::restingFibersDot() const
{
    return *m_restingFibersDot;
}


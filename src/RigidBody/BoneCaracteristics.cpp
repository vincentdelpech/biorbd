#define BIORBD_API_EXPORTS
#include "RigidBody/BoneCaracteristics.h"

#include "Utils/Node3d.h"
#include "RigidBody/Patch.h"
#include "RigidBody/BoneMesh.h"

biorbd::rigidbody::BoneCaracteristics::BoneCaracteristics() :
    Body(),
    m_length(std::make_shared<double>(0)),
    m_mesh(std::make_shared<biorbd::rigidbody::BoneMesh>())
{

}
biorbd::rigidbody::BoneCaracteristics::BoneCaracteristics(
        double mass,
        const biorbd::utils::Node3d &com,
        const RigidBodyDynamics::Math::Matrix3d &inertia) :
    Body(mass, com, inertia),
    m_length(std::make_shared<double>(0)),
    m_mesh(std::make_shared<biorbd::rigidbody::BoneMesh>())
{

}
biorbd::rigidbody::BoneCaracteristics::BoneCaracteristics(
        double mass,
        const biorbd::utils::Node3d &com,
        const RigidBodyDynamics::Math::Matrix3d &inertia,
        const biorbd::rigidbody::BoneMesh &mesh) :
    Body(mass, com, inertia),
    m_length(std::make_shared<double>(0)),
    m_mesh(std::make_shared<biorbd::rigidbody::BoneMesh>(mesh))
{

}

biorbd::rigidbody::BoneCaracteristics biorbd::rigidbody::BoneCaracteristics::DeepCopy() const
{
    biorbd::rigidbody::BoneCaracteristics copy;
    copy.DeepCopy(*this);
    return copy;
}

void biorbd::rigidbody::BoneCaracteristics::DeepCopy(const BoneCaracteristics &other)
{
    static_cast<RigidBodyDynamics::Body&>(*this) = other;
    *m_length = *other.m_length;
    *m_mesh = other.m_mesh->DeepCopy();
}

const biorbd::rigidbody::BoneMesh &biorbd::rigidbody::BoneCaracteristics::mesh() const
{
    return *m_mesh;
}

const Eigen::Matrix3d &biorbd::rigidbody::BoneCaracteristics::inertia() const
{
    return mInertia;
}

double biorbd::rigidbody::BoneCaracteristics::length() const
{
    return *m_length;
}

double biorbd::rigidbody::BoneCaracteristics::mass() const
{
    return mMass;
}

void biorbd::rigidbody::BoneCaracteristics::setLength(double val)
{
    *m_length = val;
}

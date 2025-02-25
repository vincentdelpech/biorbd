#define BIORBD_API_EXPORTS
#include "Muscles/ForceFromInsertion.h"

#include <vector>
#include "Utils/Node3d.h"
#include "Muscles/Geometry.h"

biorbd::muscles::ForceFromInsertion::ForceFromInsertion(double x, double y, double z) :
    biorbd::muscles::Force(x,y,z)
{

}
biorbd::muscles::ForceFromInsertion::ForceFromInsertion(
        const biorbd::muscles::Geometry& geo,
        double vectorNorm) :
    biorbd::muscles::Force()
{
    setForceFromMuscleGeometry(geo, vectorNorm);
}

biorbd::muscles::ForceFromInsertion biorbd::muscles::ForceFromInsertion::DeepCopy() const
{
    biorbd::muscles::ForceFromInsertion copy;
    copy.DeepCopy(*this);
    return copy;
}

void biorbd::muscles::ForceFromInsertion::DeepCopy(const biorbd::muscles::ForceFromInsertion &other)
{
    biorbd::muscles::Force::DeepCopy(other);
}

void biorbd::muscles::ForceFromInsertion::setForceFromMuscleGeometry(
        const biorbd::muscles::Geometry& geo,
        double vectorNorm)
{
    // Trouver le vecteur directeur
    const std::vector<biorbd::utils::Node3d>& tp_via = geo.musclesPointsInGlobal();
    *this = tp_via[tp_via.size()-2] - tp_via[tp_via.size()-1];
    *this /= this->norm();
    *this *= vectorNorm;
}

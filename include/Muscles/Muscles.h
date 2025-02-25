#ifndef BIORBD_MUSCLES_MUSCLES_H
#define BIORBD_MUSCLES_MUSCLES_H

#include <vector>
#include <memory>

#include "biorbdConfig.h"

namespace biorbd {
namespace utils {
class String;
class Matrix;
class Vector;
class Node3d;
}

namespace rigidbody {
class GeneralizedCoordinates;
class GeneralizedTorque;
}

namespace muscles {
class MuscleGroup;
class StateDynamics;
class Force;

class BIORBD_API Muscles
{
public:
    Muscles();
    Muscles(const biorbd::muscles::Muscles& other);
    virtual ~Muscles();
    biorbd::muscles::Muscles DeepCopy() const;
    void DeepCopy(const biorbd::muscles::Muscles& other);

    void addMuscleGroup(
            const biorbd::utils::String &name,
            const biorbd::utils::String &originName,
            const biorbd::utils::String &insertionName);
    int getGroupId(const biorbd::utils::String &name) const; // Trouve l'index d'un groupe musculaire, -1 s'il n'a pas trouvé
    biorbd::muscles::MuscleGroup& muscleGroup(unsigned int idx); //Retourne un groupe musculaire d'index i
    const biorbd::muscles::MuscleGroup& muscleGroup(unsigned int) const; //Retourne un groupe musculaire d'index i
    const biorbd::muscles::MuscleGroup& muscleGroup(const biorbd::utils::String&) const; //Retourne un groupe musculaire du nom demandé

    void updateMuscles(
            const biorbd::rigidbody::GeneralizedCoordinates& Q,
            bool updateKin); // Update les positions/jacobiennes/vitesse, etc
    void updateMuscles(
            const biorbd::rigidbody::GeneralizedCoordinates& Q,
            const biorbd::rigidbody::GeneralizedCoordinates& QDot,
            bool updateKin); // Update les positions/jacobiennes/vitesse, etc
    void updateMuscles(
            std::vector<std::vector<biorbd::utils::Node3d>>& musclePointsInGlobal,
            std::vector<biorbd::utils::Matrix>& jacoPointsInGlobal); // Update les positions/jacobiennes/vitesse, etc
    void updateMuscles(
            std::vector<std::vector<biorbd::utils::Node3d>>& musclePointsInGlobal,
            std::vector<biorbd::utils::Matrix>& jacoPointsInGlobal,
            const biorbd::rigidbody::GeneralizedCoordinates& QDot); // Update les positions/jacobiennes/vitesse, etc

    // Calcul des effets musculaires sur les os
    biorbd::rigidbody::GeneralizedTorque muscularJointTorque(
            const biorbd::utils::Vector& F,
            bool updateKin = true,
            const biorbd::rigidbody::GeneralizedCoordinates* Q = nullptr,
            const biorbd::rigidbody::GeneralizedCoordinates* QDot = nullptr);
    biorbd::rigidbody::GeneralizedTorque muscularJointTorque(
            const std::vector<std::shared_ptr<StateDynamics>> &state,
            biorbd::utils::Vector& F,
            bool updateKin = true,
            const biorbd::rigidbody::GeneralizedCoordinates* Q = nullptr,
            const biorbd::rigidbody::GeneralizedCoordinates* QDot = nullptr);
    biorbd::rigidbody::GeneralizedTorque muscularJointTorque(
            const std::vector<std::shared_ptr<StateDynamics>> &state,
            bool updateKin = true,
            const biorbd::rigidbody::GeneralizedCoordinates* Q = nullptr,
            const biorbd::rigidbody::GeneralizedCoordinates* QDot = nullptr);
    biorbd::utils::Matrix musclesLengthJacobian();
    biorbd::utils::Matrix musclesLengthJacobian(const biorbd::rigidbody::GeneralizedCoordinates& Q);
    std::vector<std::vector<std::shared_ptr<Force>>> musclesForces(
            const std::vector<std::shared_ptr<StateDynamics>> &state,
            bool updateKin = true,
            const biorbd::rigidbody::GeneralizedCoordinates* Q = nullptr,
            const biorbd::rigidbody::GeneralizedCoordinates* QDot = nullptr);

    // Set and get
    unsigned int nbMuscleGroups() const; // retourne le nombre total de groupes musculaires
    unsigned int nbMuscleTotal() const; // retourne le nombre total de muscles
protected:
    std::shared_ptr<std::vector<biorbd::muscles::MuscleGroup>> m_mus;

};

}}

#endif // BIORBD_MUSCLES_MUSCLES_H


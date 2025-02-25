#define BIORBD_API_EXPORTS
#include "RigidBody/KalmanReconsMarkers.h"

#include <rbdl/Model.h>
#include <rbdl/Kinematics.h>
#include "BiorbdModel.h"
#include "Utils/Error.h"
#include "Utils/Matrix.h"
#include "RigidBody/GeneralizedCoordinates.h"
#include "RigidBody/NodeBone.h"

biorbd::rigidbody::KalmanReconsMarkers::KalmanReconsMarkers() :
    biorbd::rigidbody::KalmanRecons(),
    m_PpInitial(std::make_shared<biorbd::utils::Matrix>()),
    m_firstIteration(std::make_shared<bool>(true))
{

}

biorbd::rigidbody::KalmanReconsMarkers::KalmanReconsMarkers(
        biorbd::Model &model,
        biorbd::rigidbody::KalmanRecons::KalmanParam params) :
    biorbd::rigidbody::KalmanRecons(model, model.nTechnicalMarkers()*3, params),
    m_PpInitial(std::make_shared<biorbd::utils::Matrix>()),
    m_firstIteration(std::make_shared<bool>(true))
{

    // Initialiser le filtre
    initialize();

}

biorbd::rigidbody::KalmanReconsMarkers biorbd::rigidbody::KalmanReconsMarkers::DeepCopy() const
{
    biorbd::rigidbody::KalmanReconsMarkers copy;
    copy.DeepCopy(*this);
    return copy;
}

void biorbd::rigidbody::KalmanReconsMarkers::DeepCopy(const biorbd::rigidbody::KalmanReconsMarkers &other)
{
    biorbd::rigidbody::KalmanRecons::DeepCopy(other);
    *m_PpInitial = *other.m_PpInitial;
    *m_firstIteration = *other.m_firstIteration;
}

void biorbd::rigidbody::KalmanReconsMarkers::initialize(){
    biorbd::rigidbody::KalmanRecons::initialize();

    // Se souvenir de m_Pp de départ
    m_PpInitial = m_Pp;
}

void biorbd::rigidbody::KalmanReconsMarkers::manageOcclusionDuringIteration(
        biorbd::utils::Matrix &InvTp,
        utils::Vector &measure,
        const std::vector<unsigned int> &occlusion)
{
    for (unsigned int i = 0; i < occlusion.size(); ++i)
         for (unsigned int j=occlusion[i] * 3; j< occlusion[i] * 3+3; ++j){
             InvTp(j,j) = 0; // Artéfact du au fait que m_R a une valeur à (j:j+2,j:j+2)
             measure[j] = 0;
         }
}

bool biorbd::rigidbody::KalmanReconsMarkers::first()
{
    return *m_firstIteration;
}

void biorbd::rigidbody::KalmanReconsMarkers::reconstructFrame(
        biorbd::Model &model,
        const biorbd::rigidbody::Markers &Tobs,
        biorbd::rigidbody::GeneralizedCoordinates *Q,
        biorbd::rigidbody::GeneralizedCoordinates *Qdot,
        biorbd::rigidbody::GeneralizedCoordinates *Qddot,
        bool removeAxes){
    // Séparer les tobs en un grand vecteur
    biorbd::utils::Vector T(3*Tobs.nMarkers());
    for (unsigned int i=0; i<Tobs.nMarkers(); ++i)
        T.block(i*3, 0, 3, 1) = Tobs.marker(i);

    // Reconstruire la cinématique
    reconstructFrame(model, T, Q, Qdot, Qddot, removeAxes);
}

void biorbd::rigidbody::KalmanReconsMarkers::reconstructFrame(
        biorbd::Model &model,
        const std::vector<biorbd::rigidbody::NodeBone> &Tobs,
        biorbd::rigidbody::GeneralizedCoordinates *Q,
        biorbd::rigidbody::GeneralizedCoordinates *Qdot,
        biorbd::rigidbody::GeneralizedCoordinates *Qddot,
        bool removeAxes){
    // Séparer les tobs en un grand vecteur
    biorbd::utils::Vector T(static_cast<unsigned int>(3*Tobs.size()));
    for (unsigned int i=0; i<Tobs.size(); ++i)
        T.block(i*3, 0, 3, 1) = Tobs[i];

    // Reconstruire la cinématique
    reconstructFrame(model, T, Q, Qdot, Qddot, removeAxes);
}


void biorbd::rigidbody::KalmanReconsMarkers::reconstructFrame(
        biorbd::Model &model,
        const utils::Vector &Tobs,
        biorbd::rigidbody::GeneralizedCoordinates *Q,
        biorbd::rigidbody::GeneralizedCoordinates *Qdot,
        biorbd::rigidbody::GeneralizedCoordinates *Qddot,
        bool removeAxes){
    // Une itération du filtre de Kalman
    if (*m_firstIteration){
        *m_firstIteration = false;
        biorbd::utils::Vector TobsTP(Tobs);
        TobsTP.block(3*model.nTechnicalMarkers(0), 0, 3*model.nTechnicalMarkers()-3*model.nTechnicalMarkers(0), 1) =
                Eigen::VectorXd::Zero(3*model.nTechnicalMarkers()-3*model.nTechnicalMarkers(0)); // Ne conserver que les marqueurs de la racine
        for (unsigned int j = 0; j < 2; ++j){ // Faire la racine, puis le reste du corps
            if (j != 0)
                TobsTP = Tobs; // Reprendre tous les marqueurs

            for (unsigned int i=0; i<50; ++i){
                // La premiere fois, appeler de facon recursive pour avoir une position initiale decente
                reconstructFrame(model, TobsTP, nullptr, nullptr, nullptr);

                // Remettre Pp à initial (parce qu'on ne s'intéresse pas à la vitesse pour se rendre à la position initiale
                m_Pp = m_PpInitial;
                m_xp->block(*m_nDof, 0, *m_nDof*2, 1) = Eigen::VectorXd::Zero(*m_nDof*2); // Mettre vitesse et accélération à 0
            }
        }
    }

    // État projeté
    const biorbd::utils::Vector& xkm(*m_A * *m_xp);
    const biorbd::rigidbody::GeneralizedCoordinates& Q_tp(xkm.topRows(*m_nDof));
    model.UpdateKinematicsCustom (&Q_tp, nullptr, nullptr);

    // Markers projetés
    const std::vector<biorbd::rigidbody::NodeBone>& zest_tp(model.technicalMarkers(Q_tp, removeAxes, false));
    // Jacobienne
   const  std::vector<biorbd::utils::Matrix>& J_tp(model.TechnicalMarkersJacobian(Q_tp, removeAxes, false));
    // Faire une seule matrice pour zest et Jacobienne
    biorbd::utils::Matrix H(biorbd::utils::Matrix::Zero(*m_nMeasure, *m_nDof*3)); // 3*nMarkers => X,Y,Z ; 3*nDof => Q, Qdot, Qddot
    biorbd::utils::Vector zest(biorbd::utils::Vector::Zero(*m_nMeasure));
    std::vector<unsigned int> occlusionIdx;
    for (unsigned int i=0; i<*m_nMeasure/3; ++i) // Divisé par 3 parce qu'on intègre d'un coup xyz
        if (Tobs(i*3)*Tobs(i*3) + Tobs(i*3+1)*Tobs(i*3+1) + Tobs(i*3+2)*Tobs(i*3+2) != 0.0){ // S'il y a un marqueur
            H.block(i*3,0,3,*m_nDof) = J_tp[i];
            zest.block(i*3, 0, 3, 1) = zest_tp[i];
        }
        else
            occlusionIdx.push_back(i);

    // Faire le filtre
    iteration(Tobs, zest, H, occlusionIdx);

    getState(Q, Qdot, Qddot);
}

void biorbd::rigidbody::KalmanReconsMarkers::reconstructFrame()
{
    biorbd::utils::Error::raise("Implémentation impossible");
}

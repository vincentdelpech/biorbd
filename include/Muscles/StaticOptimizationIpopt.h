#ifndef BIORBD_MUSCLES_STATIC_OPTIMIZATION_IPOPT_H
#define BIORBD_MUSCLES_STATIC_OPTIMIZATION_IPOPT_H

#include <IpIpoptApplication.hpp>
#include <IpTNLP.hpp>
#include "biorbdConfig.h"
#include "RigidBody/GeneralizedCoordinates.h"
#include "RigidBody/GeneralizedTorque.h"

namespace biorbd {
class Model;

namespace muscles {
class StateDynamics;

class BIORBD_API StaticOptimizationIpopt : public Ipopt::TNLP
{
public:
    StaticOptimizationIpopt(
            biorbd::Model &model,
            const biorbd::rigidbody::GeneralizedCoordinates &Q,
            const biorbd::rigidbody::GeneralizedCoordinates &Qdot,
            const biorbd::rigidbody::GeneralizedTorque &GeneralizedTorqueTarget,
            const biorbd::utils::Vector &activationInit,
            bool  useResidual = true,
            unsigned int pNormFactor = 2,
            int verbose = 0,
            double eps = 1e-10);

    virtual ~StaticOptimizationIpopt();

    // Method to return some info about the NLP
    virtual bool get_nlp_info(
            Ipopt::Index& n,
            Ipopt::Index& m,
            Ipopt::Index& nnz_jac_g,
            Ipopt::Index& nnz_h_lag,
            IndexStyleEnum& index_style);

    // Method to return the bounds for my problem
    virtual bool get_bounds_info(
            Ipopt::Index n,
            Ipopt::Number* x_l,
            Ipopt::Number* x_u,
            Ipopt::Index m,
            Ipopt::Number* g_l,
            Ipopt::Number* g_u);

    // Method to return the starting point for the algorithm
    virtual bool get_starting_point(
            Ipopt::Index n,
            bool init_x,
            Ipopt::Number* x,
            bool init_z,
            Ipopt::Number* z_L,
            Ipopt::Number* z_U,
            Ipopt::Index m,
            bool init_lambda,
            Ipopt::Number* lambda);

    // Method to return the objective value
    virtual bool eval_f(
            Ipopt::Index n,
            const Ipopt::Number* x,
            bool new_x,
            Ipopt::Number& obj_value);

    // Method to return the gradient of the objective
    virtual bool eval_grad_f(
            Ipopt::Index n,
            const Ipopt::Number* x,
            bool new_x,
            Ipopt::Number* grad_f);

    ///
    /// Method to return the constraint residuals
    ///
    virtual bool eval_g(
            Ipopt::Index n,
            const Ipopt::Number* x,
            bool new_x,
            Ipopt::Index m,
            Ipopt::Number* g);

    // Method to return:
    //   1) The structure of the jacobian (if "values" is NULL)
    //   2) The values of the jacobian (if "values" is not NULL)
    virtual bool eval_jac_g(
            Ipopt::Index n,
            const Ipopt::Number* x,
            bool new_x,
            Ipopt::Index m,
            Ipopt::Index nele_jac,
            Ipopt::Index* iRow,
            Ipopt::Index* jCol,
            Ipopt::Number* values);

    virtual void finalize_solution(
            Ipopt::SolverReturn status,
            Ipopt::Index n,
            const Ipopt::Number* x,
            const Ipopt::Number* z_L,
            const Ipopt::Number* z_U,
            Ipopt::Index m,
            const Ipopt::Number* g,
            const Ipopt::Number* lambda,
            Ipopt::Number obj_value,
            const Ipopt::IpoptData* ip_data,
            Ipopt::IpoptCalculatedQuantities* ip_cq);

    biorbd::utils::Vector finalSolution() const;
    biorbd::utils::Vector finalResidual() const;




protected:
    biorbd::Model &m_model;
    unsigned int m_nQ;
    unsigned int m_nQdot;
    unsigned int m_nMus;
    unsigned int m_nDof;
    unsigned int m_nGeneralizedTorque;
    unsigned int m_nGeneralizedTorqueResidual;
    double m_eps;
    biorbd::utils::Vector m_activations;
    biorbd::rigidbody::GeneralizedCoordinates m_Q;
    biorbd::rigidbody::GeneralizedCoordinates m_Qdot;
    biorbd::rigidbody::GeneralizedTorque m_GeneralizedTorqueTarget;
    biorbd::utils::Vector m_GeneralizedTorqueResidual;
    double m_GeneralizedTorquePonderation;
    std::vector<biorbd::muscles::StateDynamics> m_states;
    unsigned int m_pNormFactor;
    int m_verbose;
    biorbd::utils::Vector m_finalSolution;
    biorbd::utils::Vector m_finalResidual;

    void dispatch(const Ipopt::Number* x);

};

}}

#endif // BIORBD_MUSCLES_OPTIMIZATION_IPOPT_H

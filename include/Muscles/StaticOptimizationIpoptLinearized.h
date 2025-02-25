#ifndef BIORBD_MUSCLES_STATIC_OPTIMIZATION_IPOPT_LINEARIZED_H
#define BIORBD_MUSCLES_STATIC_OPTIMIZATION_IPOPT_LINEARIZED_H

#include "biorbdConfig.h"
#include "Muscles/StaticOptimizationIpopt.h"

namespace biorbd {
namespace utils {
class Matrix;
}

namespace muscles {

class BIORBD_API StaticOptimizationIpoptLinearized : public biorbd::muscles::StaticOptimizationIpopt
{
public:
    StaticOptimizationIpoptLinearized(
            biorbd::Model& model,
            const biorbd::rigidbody::GeneralizedCoordinates& Q,
            const biorbd::rigidbody::GeneralizedCoordinates& Qdot,
            const biorbd::rigidbody::GeneralizedTorque& GeneralizedTorqueTarget,
            const biorbd::utils::Vector& activationInit,
            bool useResidual = true,
            unsigned int pNormFactor = 2,
            int verbose = 0,
            double eps = 1e-10);

    virtual ~StaticOptimizationIpoptLinearized();

    // Method to return the constraint residuals
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

protected:
    std::shared_ptr<biorbd::utils::Matrix> m_jacobian;
    void prepareJacobian();

};

}}

#endif // BIORBD_MUSCLES_STATIC_OPTIMIZATION_IPOPT_LINEARIZED_H

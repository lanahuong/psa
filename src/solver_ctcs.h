#ifndef PROJET_PSA_SOLVER_CTCS_H
#define PROJET_PSA_SOLVER_CTCS_H

#include "solver.h"

class solver_ctcs : public solver {
public:
    solver_ctcs(const arma::cx_mat &phi_0, const arma::mat &pot, double dx, double dy, double dt);

    void step() override;

};

#endif //PROJET_PSA_SOLVER_CTCS_H

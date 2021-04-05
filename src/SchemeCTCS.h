#ifndef PROJET_PSA_SCHEMECTCS_H
#define PROJET_PSA_SCHEMECTCS_H

#include "Solver.h"

class SchemeCTCS : public Solver {
public:
  SchemeCTCS(const arma::cx_mat &phi_0, const arma::mat &pot, double dx,
             double dy, double dt);

  void step() override;
};

#endif // PROJET_PSA_SCHEMECTCS_H

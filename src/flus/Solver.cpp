#include "Solver.h"
#include <cassert>

Solver::Solver(const arma::cx_mat &phi_0, const arma::mat &pot, double dx,
               double dy, double dt)
    : dx_(dx), dy_(dy), dt_(dt) {

    assert(arma::size(phi_0) == arma::size(pot));

  // Size of matrix with padding of 1
  int nx = phi_0.n_rows + 2;
  int ny = phi_0.n_cols + 2;

  // Create phit with 0 padding
  phit_.zeros(nx, ny);
    phit_.submat(1, 1, size(phi_0)) = phi_0;

  // Create mat_pot with 0 padding
  mat_pot_.zeros(nx, ny);
    mat_pot_.submat(1, 1, size(pot)) = pot;

  // Create phitdt withoutpadding
  phitdt_ = phi_0;
}

/**
 * @warning the arma documentation seems to have inverted the dims definition.
 * TODO check how it could've happened :(
 */
arma::cx_mat Solver::shift_mat(arma::cx_mat mat, int rows, int cols) {
  mat = arma::shift(mat, rows, 0);
  return arma::shift(mat, cols, 1);
}

void Solver::normalize_phitdt() {
    phit_ *=
      (1 / std::sqrt(arma::accu(phitdt_ % arma::conj(phitdt_)).real() * dx_ * dy_));
}

arma::cx_mat Solver::get_state() {
    return phitdt_;
}

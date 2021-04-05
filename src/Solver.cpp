#include "Solver.h"

Solver::Solver(const arma::cx_mat &phi_0, const arma::mat &pot, double dx,
               double dy, double dt)
    : dx(dx), dy(dy), dt(dt) {

  // Size of matrix with padding of 1
  int nx = phi_0.n_rows + 2;
  int ny = phi_0.n_cols + 2;

  // Create phit with 0 padding
  phit.zeros(nx, ny);
  phit.submat(1, 1, size(phi_0)) = phi_0;

  // Create mat_pot with 0 padding
  mat_pot.zeros(nx, ny);
  mat_pot.submat(1, 1, size(pot)) = pot;

  // Create phitdt withoutpadding
  phitdt = phi_0;
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
  phit *=
      (1 / std::sqrt(arma::accu(phitdt % arma::conj(phitdt)).real() * dx * dy));
}
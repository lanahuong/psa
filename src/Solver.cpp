#include "Solver.h"

Solver::Solver(const arma::cx_mat &phi_0, const arma::mat &pot, double dx,
               double dy, double dt)
    : mat_pot(pot), dx(dx), dy(dy), dt(dt), internal_state(phi_0) {}

double Solver::compute_internal_norm() const {
  return arma::accu(internal_state % arma::conj(internal_state)).real() * dx *
         dy;
}

arma::cx_mat Solver::add_zero_border(arma::cx_mat mat) {
  mat.insert_rows(0, 1);
  mat.insert_cols(0, 1);
  mat.insert_rows(mat.n_rows, 1);
  mat.insert_cols(mat.n_cols, 1);
  return mat;
}

arma::mat Solver::add_zero_border(arma::mat mat) {
  mat.insert_rows(0, 1);
  mat.insert_cols(0, 1);
  mat.insert_rows(mat.n_rows, 1);
  mat.insert_cols(mat.n_cols, 1);
  return mat;
}

arma::cx_mat Solver::shed_zero_border(arma::cx_mat mat) {
  mat.shed_col(0);
  mat.shed_row(0);
  mat.shed_col(mat.n_cols - 1);
  mat.shed_row(mat.n_rows - 1);
  return mat;
}

/**
 * @warning the arma documentation seems to have inverted the dims definition.
 * TODO check how it could've happened :(
 */
arma::cx_mat Solver::shift_mat(arma::cx_mat mat, int rows, int cols) {
  mat = arma::shift(mat, rows, 0);
  return arma::shift(mat, cols, 1);
}

void Solver::normalize_internal_state() {
  internal_state *= (1 / std::sqrt(compute_internal_norm()));
}

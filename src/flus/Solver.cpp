#include "Solver.h"
#include <cassert>

Solver::Solver(const arma::cx_mat &phi_0, const arma::mat &pot, double dx, double dy, double dt)
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

    // The equation is linear in regard to Phi so we can scale it as we want.
    double norm = arma::norm(phit_, 2);
    if (norm > epsilon) {
        phi_norm_ = norm;
        phitdt_ /= phi_norm_;
        phit_ /= phi_norm_;
    }
}

/**
 * @warning the arma documentation seems to have inverted the dims definition.
 * TODO check how it could've happened :(
 */
arma::cx_mat Solver::shift_mat(arma::cx_mat mat, int rows, int cols) {
    mat = arma::shift(mat, rows, 0);
    return arma::shift(mat, cols, 1);
}

arma::cx_mat Solver::get_phitdt() const {
    return phitdt_ * phi_norm_;
}

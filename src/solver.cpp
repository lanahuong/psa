#include "solver.h"

#include <utility>

solver::solver(arma::cx_mat phi_0, arma::mat pot, double dx, double dy, double dt)
        : mat_pot(std::move(pot)), dx(dx), dy(dy), dt(dt), internal_state(std::move(phi_0)) {}

double solver::compute_internal_norm() const {
    return arma::accu(internal_state % arma::conj(internal_state)).real() * dx * dy;
}
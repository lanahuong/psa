#include "solver_btcs.h"

solver_btcs::solver_btcs(const arma::cx_mat &phi_0, const arma::mat &pot, double dx, double dy, double dt)
        : solver(phi_0, pot, dx, dy, dt) {}

void solver_btcs::step() {
    const auto pot(add_zero_border(mat_pot));
    const auto f_t(add_zero_border(internal_state));
    arma::cx_mat g_np1 = f_t, g_n;

    const cx h2m = reduced_planck * reduced_planck / neutron_mass;
    const arma::cx_mat fact = 1 / (cx(0, reduced_planck / dt) - h2m * (1 / (dx * dx) + 1 / (dy * dy)) - pot);

    do {
        g_n = std::move(g_np1);
        arma::cx_mat gn_pm_dx = shift_mat(g_n, 0, -1) + shift_mat(g_n, 0, 1);
        arma::cx_mat gn_pm_dy = shift_mat(g_n, -1, 0) + shift_mat(g_n, 1, 0);
        arma::cx_mat res = -(h2m / (2 * dx * dx)) * gn_pm_dx - (h2m / (2 * dy * dy)) * gn_pm_dy + cx(0, reduced_planck / dt) * f_t;
        g_np1 = res % fact;
    } while (arma::norm(g_n - g_np1, "inf") > epsilon);

    internal_state = shed_zero_border(g_np1);
}





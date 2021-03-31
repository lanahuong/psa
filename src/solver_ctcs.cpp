#include "solver_ctcs.h"

solver_ctcs::solver_ctcs(const arma::cx_mat &phi_0, const arma::mat &pot, double dx, double dy, double dt)
        : solver(phi_0, pot, dx, dy, dt) {}

void solver_ctcs::step() {
    const auto pot(add_zero_border(mat_pot));
    const auto f_t(add_zero_border(this->internal_state));
    arma::cx_mat g_np1 = f_t, g_n;

    const arma::cx_mat f_t_pm_dx = shift_mat(f_t, 0, -1) + shift_mat(f_t, 0, 1);
    const arma::cx_mat f_t_pm_dy = shift_mat(f_t, -1, 0) + shift_mat(f_t, 1, 0);

    const cx h2m = reduced_planck * reduced_planck / neutron_mass;
    const arma::cx_mat fact = cx(0, 2 * reduced_planck / dt) - h2m * (1 / (dx * dx) + 1 / (dy * dy)) - pot;
    const arma::cx_mat fact2 = (pot + cx(0, 2 * reduced_planck / dt) + h2m * (1 / (dx * dx) + 1 / (dy * dy)));

    do {
        g_n = g_np1;

        arma::cx_mat x_m_dx = shift_mat(g_n, 0, -1);
        arma::cx_mat x_p_dx = shift_mat(g_n, 0, 1);
        arma::cx_mat y_m_dy = shift_mat(g_n, -1, 0);
        arma::cx_mat y_p_dy = shift_mat(g_n, 1, 0);

        g_np1 = [&]() {
            arma::cx_mat res(-h2m / (2 * dx * dx) * (f_t_pm_dx + x_m_dx + x_p_dx));
            res -= h2m / (2 * dy * dy) * (f_t_pm_dy + y_m_dy + y_p_dy);
            res += fact2 % f_t;
            return res / fact;
        }();

    } while (arma::norm(g_n - g_np1, "inf") > epsilon);

    this->internal_state = shed_zero_border(g_np1);
}

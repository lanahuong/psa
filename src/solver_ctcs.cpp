#include "solver_ctcs.h"

arma::cx_mat solver_ctcs::step_ctcs(arma::cx_mat phi_0) {
    const int x_size(phi_0.n_cols);
    const int y_size(phi_0.n_rows);

    arma::cx_mat g_mat(phi_0);
    arma::cx_mat g_mat_prev;

    do {
        g_mat_prev = g_mat;
        for (int x = 0; x < x_size; x++) {
            for (int y = 0; y < y_size; y++) {
                cross_cx t, g;
                t.x_m_dx = x - 1 < 0 ? 0 : phi_0.at(y, x - 1);
                t.x_p_dx = x + 1 < x_size ? phi_0.at(y, x + 1) : 0;
                t.y_m_dy = y - 1 < 0 ? 0 : phi_0.at(y - 1, x);
                t.y_p_dy = y + 1 < y_size ? phi_0.at(y + 1, x) : 0;

                g.x_m_dx = x - 1 < 0 ? 0 : g_mat_prev.at(y, x - 1);
                g.x_p_dx = x + 1 < x_size ? g_mat_prev.at(y, x + 1) : 0;
                g.y_m_dy = y - 1 < 0 ? 0 : g_mat_prev.at(y - 1, x);
                g.y_p_dy = y + 1 < y_size ? g_mat_prev.at(y + 1, x) : 0;

                g_mat.at(x, y) = solver_ctcs::F(mat_pot.at(x, y), phi_0.at(x, y), t, g);
            }
        }
    } while (arma::norm(g_mat_prev - g_mat, "inf") > epsilon);

    return g_mat;
}

cx solver_ctcs::F(double pot, cx psi, cross_cx t, cross_cx t_p_dt) const {
    cx h2m = reduced_planck * reduced_planck / neutron_mass;
    cx fact = cx(0, 2 * reduced_planck / dt) - pot - h2m * (1 / (dx * dx) + 1 / (dy * dy));
    cx res = cx(0, 0);
    res -= h2m / (2 * dx * dx) * (t.x_m_dx + t.x_p_dx + t_p_dt.x_m_dx + t_p_dt.x_p_dx);
    res -= h2m / (2 * dy * dy) * (t.y_m_dy + t.y_p_dy + t_p_dt.y_m_dy + t_p_dt.y_p_dy);
    res += (pot + cx(0, 2 * reduced_planck / dt) + h2m * (1 / (dx * dx) + 1 / (dy * dy))) * psi;
    return res / fact;
}

solver_ctcs::solver_ctcs(arma::mat pot, double dx, double dy, double dt) : mat_pot(std::move(pot)), dx(dx), dy(dy), dt(dt) {}

#include "SchemeCTCS.h"

/*#include <Python/Python.h>
class GILReleaser {
public:
    GILReleaser() : save(PyEval_SaveThread()) {}
    ~GILReleaser() {PyEval_RestoreThread(save);}
    PyThreadState* save;};*/

SchemeCTCS::SchemeCTCS(const arma::cx_mat &phi_0, const arma::mat &pot,
                       double dx, double dy, double dt)
        : Solver(phi_0, pot, dx, dy, dt) {}

void SchemeCTCS::step() {
    // GILReleaser releaser;
    const auto pot = mat_pot_;
    arma::cx_mat g_np1 = phit_, g_n;


    const auto n1 = m_e_scale_ * step_scale_ * step_scale_ / (h_bar_scale_ * h_bar_scale_);
    const auto n2 = m_e_scale_ * h_bar_scale_ / step_scale_;

    const arma::cx_mat f_t_pm_dx = phit_.submat(2, 1, size(phitdt_)) + phit_.submat(0, 1, size(phitdt_));
    const arma::cx_mat f_t_pm_dy = phit_.submat(1, 2, size(phitdt_)) + phit_.submat(1, 0, size(phitdt_));

    const auto pot_sub = pot.submat(1, 1, size(phitdt_));
    const cx h2m = h_bar_ * h_bar_ / m_e_;
    const arma::cx_mat fact = 1 / (n2 * cx(0, 2 * h_bar_ / dt_) - h2m * (1 / (dx_ * dx_) + 1 / (dy_ * dy_)) - n1 * pot_sub);
    const arma::cx_mat fact2 = n1 * pot_sub + n2 * cx(0, 2 * h_bar_ / dt_) + h2m * (1 / (dx_ * dx_) + 1 / (dy_ * dy_));

    do {
        g_n = g_np1;
        arma::cx_mat gn_pm_dx = g_n.submat(2, 1, size(phitdt_)) + g_n.submat(0, 1, size(phitdt_));
        arma::cx_mat gn_pm_dy = g_n.submat(1, 2, size(phitdt_)) + g_n.submat(1, 0, size(phitdt_));
        arma::cx_mat res = -(h2m / (2 * dx_ * dx_)) * (f_t_pm_dx + gn_pm_dx) - (h2m / (2 * dy_ * dy_)) * (f_t_pm_dy + gn_pm_dy) + fact2 % phit_.submat(1, 1, size(phitdt_));
        g_np1.submat(1, 1, size(phitdt_)) = res % fact;
    } while (arma::norm(g_n - g_np1, "inf") > epsilon);

    phit_ = g_np1;
    phitdt_ = g_np1.submat(1, 1, size(phitdt_));
    //phitdt_ = phit_.submat(1, 1, size(phitdt_));
}

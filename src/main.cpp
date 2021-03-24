#include "solver_ctcs.h"


constexpr int size = 100;
constexpr double step = 0.1;


int main() {
    arma::cx_mat phi0(size, size);
    phi0.randn();

    arma::mat V(size, size);
    V.randn();

    solver_ctcs solver(V, step, step, step);
    std::cout << solver.step_ctcs(phi0);

    return 0;
}
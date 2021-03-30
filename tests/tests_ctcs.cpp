#include "../src/solver_ctcs.h"
#include <gtest/gtest.h>
#include <time.h>

constexpr int size = 100;
constexpr double step = 0.01;

TEST(PolyClass, Mandatory)
{
    arma::arma_rng::set_seed(time(0));

    arma::cx_mat phi0(size, size);
    phi0.randn();

    arma::mat V(size, size);
    V.randn();

    solver_ctcs slv(phi0, V, step, step, step);
    for (int i = 0; i<100; i++) {
        arma::cx_mat previous_state = slv.internal_state;
        slv.step();
        std::cout << "Norm " << slv.compute_internal_norm() << " diff " << arma::norm(slv.internal_state - phi0, 2) << std::endl;
    }

    std::cout << slv.internal_state;
}

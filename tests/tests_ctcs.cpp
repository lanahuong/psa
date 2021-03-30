#include "../src/solver_ctcs.h"
#include <gtest/gtest.h>
#include <ctime>

constexpr int size = 50;
constexpr double step = 0.1;

TEST(Solver_CTCS, NormConsistency) {
    arma::arma_rng::set_seed(time(0));
    arma::cx_mat phi0(size, size);
    phi0.randn();
    arma::mat V(size, size);
    V.randn();
    solver_ctcs slv(phi0, V, step, step, step);

    double prev_norm = slv.compute_internal_norm();

    for (int i = 0; i < 3000; i++) {
        slv.step();
        double norm = slv.compute_internal_norm();
        ASSERT_TRUE(std::abs(norm - prev_norm) < epsilon);
        prev_norm = norm;
    }
}

#include "../src/solver_btcs.h"
#include <gtest/gtest.h>
#include <ctime>

constexpr int size = 50;
constexpr double step = 0.1;

TEST(Solver_BTCS, NormConsistency) {
    arma::arma_rng::set_seed((0));
    arma::cx_mat phi0(size, size);
    phi0.randn();
    arma::mat V(size, size);
    V.randn();
    solver_btcs slv(phi0, V, step, step, step);

    double prev_norm = slv.compute_internal_norm();

    for (int i = 0; i < 100; i++) {
        slv.step();
        double norm = slv.compute_internal_norm();
        ASSERT_TRUE(std::abs(norm - prev_norm) < epsilon);
        prev_norm = norm;
    }
}

TEST(Solver_BTCS, Dirac) {
    arma::cx_mat phi0(100, 100);
    phi0.randn();
    // phi0.at(0, 0) = 30;
    arma::mat V(100, 100, arma::fill::zeros);
    solver_btcs slv(phi0, V, 10e-2, 10e-2, 10e-10);

    slv.normalize_internal_state();
    std::cout << "inorm" << slv.compute_internal_norm() << std::endl;
    for (int i = 0; i < 2; i++) {
        std::cout << "Norm " << slv.compute_internal_norm() << slv.internal_state << std::endl;
        slv.step();
        std::cout << "inorm" << slv.compute_internal_norm() << std::endl;
    }
}


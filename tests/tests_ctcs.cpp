#include "../src/solver_ctcs.h"
#include <gtest/gtest.h>
#include <ctime>

constexpr int size = 50;
constexpr double step = 0.1;

TEST(Solver, PaddingTest) {
    arma::cx_mat init({1});
    arma::cx_mat expected({{0, 0, 0},
                           {0, 1, 0},
                           {0, 0, 0}});
    arma::cx_mat res = solver::add_zero_border(init);
    ASSERT_NEAR(arma::norm(res - expected), 0, epsilon);
}

TEST(Solver, SheddingTest) {
    arma::cx_mat expected({1});
    arma::cx_mat init({{0, 0, 0},
                       {0, 1, 0},
                       {0, 0, 0}});
    arma::cx_mat res = solver::shed_zero_border(init);
    ASSERT_NEAR(arma::norm(res - expected), 0, epsilon);
}

TEST(Solver, ShiftTest) {
    arma::cx_mat init({{0, 0, 1},
                       {0, 1, 0},
                       {0, 0, 0}});
    arma::cx_mat expected({{0, 0, 0},
                           {0, 1, 0},
                           {1, 0, 0}});
    arma::cx_mat res = solver::shift_mat(init, 1, -1);
    ASSERT_NEAR(arma::norm(res - expected), 0, epsilon);
}

TEST(Solver_CTCS, NormConsistency) {
    arma::arma_rng::set_seed((0));
    arma::cx_mat phi0(size, size);
    phi0.randn();
    arma::mat V(size, size);
    V.randn();
    solver_ctcs slv(phi0, V, step, step, step);

    double prev_norm = slv.compute_internal_norm();

    for (int i = 0; i < 100; i++) {
        slv.step();
        double norm = slv.compute_internal_norm();
        ASSERT_TRUE(std::abs(norm - prev_norm) < epsilon);
        prev_norm = norm;
    }
}

TEST(Solver_CTCS, Dirac) {
    arma::cx_mat phi0(100, 100);
    phi0.randn();
    // phi0.at(0, 0) = 30;
    arma::mat V(100, 100, arma::fill::zeros);
    solver_ctcs slv(phi0, V, 10e-2, 10e-2, 10e-10);

    slv.normalize_internal_state();
    std::cout << "inorm" << slv.compute_internal_norm() << std::endl;
    for (int i = 0; i < 2; i++) {
 //       std::cout << "Norm " << slv.compute_internal_norm() << slv.internal_state << std::endl;
        slv.step();
        std::cout << "inorm" << slv.compute_internal_norm() << std::endl;
    }
}


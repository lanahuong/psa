#include "../src/flus/SchemeCTCS.h"
#include <ctime>
#include <gtest/gtest.h>

constexpr int size = 5;
constexpr double step = 0.1;


TEST(Solver, ShiftTest) {
    arma::cx_mat init({{0, 0, 1},
                       {0, 1, 0},
                       {0, 0, 0}});
    arma::cx_mat expected({{0, 0, 0},
                           {0, 1, 0},
                           {1, 0, 0}});
    arma::cx_mat res = Solver::shift_mat(init, 1, -1);
    ASSERT_NEAR(arma::norm(res - expected), 0, epsilon);
}

TEST(SchemeCTCS, NormConsistency) {
    arma::arma_rng::set_seed((0));
    arma::cx_mat phi0(size, size);
    phi0.ones();
    arma::mat V(size, size);
    V.ones(); // Building a square of ones;
    V.submat(1, 1, arma::size(size - 2, size - 2)) = arma::mat(size - 2, size - 2, arma::fill::zeros);
    SchemeCTCS slv(phi0, V, step, step, step);
    double prev_norm = slv.phitdt_norm();
    for (int i = 0; i < 5; i++) {
        slv.step();
        std::cout << slv.get_phitdt() << std::endl;
        double norm = slv.phitdt_norm();
        ASSERT_TRUE(std::abs(norm - prev_norm) < epsilon);
        prev_norm = norm;
    }
}

TEST(Solver_CTCS, Dirac) {
    arma::cx_mat phi0(100, 100);
    phi0.randn();
    // phi0.at(0, 0) = 30;
    arma::mat V(100, 100, arma::fill::zeros);
    SchemeCTCS slv(phi0, V, 0.1, 0.1, 0.1);

    //slv.normalize_phitdt();

    std::cout << "inorm" << slv.phitdt_norm() << std::endl;
    for (int i = 0; i < 2; i++) {
        //       std::cout << "Norm " << phitdt_norm(slv) << slv.internal_state <<
        //       std::endl;
        slv.step();
        std::cout << "inorm" << slv.phitdt_norm() << std::endl;
    }
}

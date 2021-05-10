#include "../src/flus/SchemeCTCS.h"
#include "../src/flus/SchemeBTCS.h"
#include "../src/flus/SchemeFTCS.h"
#include <gtest/gtest.h>

constexpr int size = 20;
constexpr double step = 10;


TEST(BTCSvCTCS, NormConsistency) {
    int supersampling = 1000;
    arma::arma_rng::set_seed((0));
    arma::cx_mat phi0(size, size);
    phi0.randn();
    arma::mat V(size, size);
    V.zeros();
    SchemeCTCS ctcs(phi0, V, step, step, step / supersampling);
    SchemeBTCS btcs(phi0, V, step, step, step / supersampling);

    for (int i = 0; i < 100; i++) {
        ctcs.step_n(supersampling);
        btcs.step_n(supersampling);
        auto norm = arma::norm(ctcs.get_phitdt() - btcs.get_phitdt()) / (arma::norm(ctcs.get_phitdt()));
        std::cout << "Relative error " << norm << std::endl;
        ASSERT_TRUE(norm < epsilon);
    }
}

TEST(FTCSvBTCS, NormConsistency) {
    int supersampling = 1000;
    arma::arma_rng::set_seed((0));
    arma::cx_mat phi0(size, size);
    phi0.randn();
    arma::mat V(size, size);
    V.zeros();
    SchemeFTCS ftcs(phi0, V, step, step, step / supersampling);
    SchemeBTCS btcs(phi0, V, step, step, step / supersampling);

    for (int i = 0; i < 100; i++) {
        ftcs.step_n(supersampling);
        btcs.step_n(supersampling);
        auto norm = arma::norm(ftcs.get_phitdt() - btcs.get_phitdt()) / (arma::norm(btcs.get_phitdt()));
        std::cout << "Relative error " << norm << std::endl;
        ASSERT_TRUE(norm < 0.001);
    }
}

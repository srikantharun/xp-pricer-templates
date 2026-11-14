#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "xp/american_binomial.hpp"
using Catch::Matchers::WithinAbs;
TEST_CASE("binomial converges toward Black-Scholes ATM call", "[american_binomial]") {
  // BS ATM call S=K=100,r=5%,q=0,vol=20%,T=1 is ~10.4506; CRR with 256 steps is within ~0.1.
  const double p = xp::american_binomial_call(100, 100, 0.05, 0.20, 1.0, 256);
  CHECK_THAT(p, WithinAbs(10.4506, 0.1));
}
TEST_CASE("binomial rejects bad input", "[american_binomial][validation]") {
  CHECK_THROWS(xp::american_binomial_call(-1, 100, 0.05, 0.2, 1.0));
}

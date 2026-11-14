#include <catch2/catch_test_macros.hpp>
#include "xp/barrier_mc.hpp"
TEST_CASE("up-and-out call is non-negative and below vanilla intrinsic cap", "[barrier_mc]") {
  const double p = xp::barrier_mc_up_and_out_call(100, 100, 130, 0.05, 0.20, 1.0, 20000);
  CHECK(p >= 0.0);
  CHECK(p < 30.0);  // capped: knocked out at/above the 130 barrier
}
TEST_CASE("barrier below spot is rejected", "[barrier_mc][validation]") {
  CHECK_THROWS(xp::barrier_mc_up_and_out_call(100, 100, 90, 0.05, 0.2, 1.0));
}

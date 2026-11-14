#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "xp/bachelier.hpp"
using Catch::Matchers::WithinRel;
TEST_CASE("bachelier ATM call", "[bachelier]") {
  // ATM (F=K): price = vol*sqrt(T)/sqrt(2*pi).
  const double p = xp::bachelier_call(100, 100, 20.0, 1.0);
  CHECK_THAT(p, WithinRel(20.0 / std::sqrt(2.0 * M_PI), 1e-12));
}
TEST_CASE("bachelier call rises with vol", "[bachelier][property]") {
  CHECK(xp::bachelier_call(100, 100, 30.0, 1.0) > xp::bachelier_call(100, 100, 20.0, 1.0));
}

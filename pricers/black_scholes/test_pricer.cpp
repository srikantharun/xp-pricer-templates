// test_pricer.cpp — numerical regression and property tests for xp::price_european_bs.
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "xp/pricer.hpp"

using Catch::Matchers::WithinAbs;
using Catch::Matchers::WithinRel;

namespace {

xp::EuropeanOption atm_call() {
  return {xp::OptionType::Call, /*spot*/ 100.0, /*strike*/ 100.0, /*rate*/ 0.05,
          /*div*/ 0.0,          /*vol*/ 0.20,   /*T*/ 1.0};
}

}  // namespace

TEST_CASE("ATM call matches golden Black-Scholes value", "[regression]") {
  // Golden: closed-form BS, S=K=100, r=5%, q=0, sigma=20%, T=1 — cross-checked against an
  // independent erfc-based closed form. Tolerance 1e-10 absolute: analytic engine, so only
  // floating-point noise is allowed.
  constexpr double kGoldenNpv = 10.450583572185565;
  constexpr double kTolerance = 1e-10;
  const auto r = xp::price_european_bs(atm_call());
  CHECK_THAT(r.npv, WithinAbs(kGoldenNpv, kTolerance));
  CHECK_THAT(r.delta, WithinAbs(0.6368306511756191, 1e-10));
}

TEST_CASE("put-call parity holds", "[property]") {
  auto call = atm_call();
  call.dividend_yield = 0.02;
  call.strike = 110.0;
  auto put = call;
  put.type = xp::OptionType::Put;

  const double c = xp::price_european_bs(call).npv;
  const double p = xp::price_european_bs(put).npv;
  const double T = call.maturity_years;
  const double parity =
      call.spot * std::exp(-call.dividend_yield * T) - call.strike * std::exp(-call.rate * T);
  CHECK_THAT(c - p, WithinAbs(parity, 1e-10));
}

TEST_CASE("greeks have expected signs", "[property]") {
  const auto r = xp::price_european_bs(atm_call());
  CHECK(r.delta > 0.0);
  CHECK(r.delta < 1.0);
  CHECK(r.gamma > 0.0);
  CHECK(r.vega > 0.0);
}

TEST_CASE("invalid inputs are rejected", "[validation]") {
  auto o = atm_call();
  SECTION("negative spot") { o.spot = -1.0; }
  SECTION("zero volatility") { o.volatility = 0.0; }
  SECTION("zero maturity") { o.maturity_years = 0.0; }
  SECTION("infinite strike") { o.strike = std::numeric_limits<double>::infinity(); }
  CHECK_THROWS_AS(xp::price_european_bs(o), std::invalid_argument);
}

// xp/pricer.hpp — public API of xp::core.
// QuantLib is an implementation detail: no QuantLib types appear in this header.
#pragma once

#include <cstdint>

namespace xp {

enum class OptionType : std::uint8_t { Call, Put };

// Inputs for a European option under Black-Scholes(-Merton).
// Rates and yields are continuously compounded; maturity is in years (Actual/365F).
struct EuropeanOption {
  OptionType type{OptionType::Call};
  double spot{0.0};
  double strike{0.0};
  double rate{0.0};
  double dividend_yield{0.0};
  double volatility{0.0};
  double maturity_years{0.0};
};

struct PricingResult {
  double npv{0.0};
  double delta{0.0};
  double gamma{0.0};
  double vega{0.0};  // per 1.00 (100 vol points) change in volatility
};

// Prices `option` with QuantLib's AnalyticEuropeanEngine.
// Throws std::invalid_argument on non-positive spot/strike/vol/maturity or non-finite input.
// Not thread-safe: QuantLib's evaluation date is process-global.
[[nodiscard]] PricingResult price_european_bs(const EuropeanOption& option);

}  // namespace xp

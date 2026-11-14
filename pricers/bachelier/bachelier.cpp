#include "xp/bachelier.hpp"
#include <cmath>
#include <stdexcept>
namespace xp {
namespace {
double norm_cdf(double x) { return 0.5 * std::erfc(-x * M_SQRT1_2); }
double norm_pdf(double x) { return std::exp(-0.5 * x * x) / std::sqrt(2.0 * M_PI); }
}  // namespace
// Bachelier call: (F-K)*Phi(d) + s*phi(d), s = vol*sqrt(T), d = (F-K)/s.
double bachelier_call(double forward, double strike, double vol, double maturity) {
  if (!(vol > 0.0) || !(maturity > 0.0)) throw std::invalid_argument("xp: vol,maturity > 0");
  const double s = vol * std::sqrt(maturity);
  const double d = (forward - strike) / s;
  return (forward - strike) * norm_cdf(d) + s * norm_pdf(d);
}
}  // namespace xp

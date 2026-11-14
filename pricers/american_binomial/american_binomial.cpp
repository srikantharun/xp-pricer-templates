#include "xp/american_binomial.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>
namespace xp {
double american_binomial_call(double spot, double strike, double rate, double vol,
                              double maturity, int steps) {
  if (!(spot > 0 && strike > 0 && vol > 0 && maturity > 0) || steps < 1)
    throw std::invalid_argument("xp: spot,strike,vol,maturity > 0 and steps >= 1");
  const auto n_steps = static_cast<std::size_t>(steps);
  const double dt = maturity / static_cast<double>(steps);
  const double u = std::exp(vol * std::sqrt(dt));
  const double d = 1.0 / u;
  const double disc = std::exp(-rate * dt);
  const double p = (std::exp(rate * dt) - d) / (u - d);
  std::vector<double> v(n_steps + 1);
  for (std::size_t i = 0; i <= n_steps; ++i) {
    const double up = std::pow(u, static_cast<double>(n_steps - i));
    const double dn = std::pow(d, static_cast<double>(i));
    v[i] = std::max(spot * up * dn - strike, 0.0);
  }
  for (std::size_t step = n_steps; step > 0; --step)
    for (std::size_t i = 0; i < step; ++i) v[i] = disc * (p * v[i] + (1 - p) * v[i + 1]);
  return v[0];
}
}  // namespace xp

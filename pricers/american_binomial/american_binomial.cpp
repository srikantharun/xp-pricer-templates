#include "xp/american_binomial.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>
namespace xp {
double american_binomial_call(double spot, double strike, double rate, double vol,
                              double maturity, int steps) {
  if (!(spot > 0 && strike > 0 && vol > 0 && maturity > 0) || steps < 1)
    throw std::invalid_argument("xp: spot,strike,vol,maturity > 0 and steps >= 1");
  const double dt = maturity / steps;
  const double u = std::exp(vol * std::sqrt(dt));
  const double d = 1.0 / u;
  const double disc = std::exp(-rate * dt);
  const double p = (std::exp(rate * dt) - d) / (u - d);
  std::vector<double> v(steps + 1);
  for (int i = 0; i <= steps; ++i)
    v[i] = std::max(spot * std::pow(u, steps - i) * std::pow(d, i) - strike, 0.0);
  for (int n = steps; n > 0; --n)
    for (int i = 0; i < n; ++i) v[i] = disc * (p * v[i] + (1 - p) * v[i + 1]);
  return v[0];
}
}  // namespace xp

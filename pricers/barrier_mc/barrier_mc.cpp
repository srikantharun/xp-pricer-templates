#include "xp/barrier_mc.hpp"
#include <cmath>
#include <random>
#include <stdexcept>
namespace xp {
double barrier_mc_up_and_out_call(double spot, double strike, double barrier, double rate,
                                  double vol, double maturity, int paths) {
  if (!(spot > 0 && strike > 0 && barrier > spot && vol > 0 && maturity > 0) || paths < 1)
    throw std::invalid_argument("xp: need spot,strike,vol,maturity>0, barrier>spot, paths>=1");
  std::mt19937_64 rng(42);  // fixed seed: reproducible across runs (FP-trap tests rely on it)
  std::normal_distribution<double> z(0.0, 1.0);
  const int steps = 50;
  const double dt = maturity / steps;
  const double drift = (rate - 0.5 * vol * vol) * dt;
  const double vsqdt = vol * std::sqrt(dt);
  double payoff_sum = 0.0;
  for (int p = 0; p < paths; ++p) {
    double s = spot;
    bool knocked = false;
    for (int t = 0; t < steps; ++t) {
      s *= std::exp(drift + vsqdt * z(rng));
      if (s >= barrier) { knocked = true; break; }
    }
    if (!knocked && s > strike) payoff_sum += s - strike;
  }
  return std::exp(-rate * maturity) * payoff_sum / paths;
}
}  // namespace xp

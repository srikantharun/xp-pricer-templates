#pragma once
// xp::american_binomial — CRR binomial-tree European call (skeleton; American early-exercise
// is left as a TODO). Method for the split demo.
namespace xp {
double american_binomial_call(double spot, double strike, double rate, double vol,
                              double maturity, int steps = 256);
}  // namespace xp

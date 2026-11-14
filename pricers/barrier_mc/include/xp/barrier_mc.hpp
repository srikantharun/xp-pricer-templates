#pragma once
// xp::barrier_mc — Monte-Carlo up-and-out barrier call (skeleton; fixed seed, GBM terminal +
// discrete barrier check). Method for the split demo.
namespace xp {
double barrier_mc_up_and_out_call(double spot, double strike, double barrier, double rate,
                                  double vol, double maturity, int paths = 20000);
}  // namespace xp

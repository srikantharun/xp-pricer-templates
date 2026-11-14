// pricer.cpp — Black-Scholes European pricing via QuantLib.
#include "xp/pricer.hpp"

#include <array>
#include <cmath>
#include <ql/exercise.hpp>
#include <ql/instruments/vanillaoption.hpp>
#include <ql/pricingengines/vanilla/analyticeuropeanengine.hpp>
#include <ql/processes/blackscholesprocess.hpp>
#include <ql/quotes/simplequote.hpp>
#include <ql/settings.hpp>
#include <ql/termstructures/volatility/equityfx/blackconstantvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/time/calendars/nullcalendar.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <stdexcept>

namespace xp {
namespace {

void validate(const EuropeanOption& o) {
  const std::array values{o.spot,           o.strike,     o.rate,
                          o.dividend_yield, o.volatility, o.maturity_years};
  for (const double v : values) {
    if (!std::isfinite(v)) throw std::invalid_argument("xp: non-finite input");
  }
  if (o.spot <= 0.0) throw std::invalid_argument("xp: spot must be > 0");
  if (o.strike <= 0.0) throw std::invalid_argument("xp: strike must be > 0");
  if (o.volatility <= 0.0) throw std::invalid_argument("xp: volatility must be > 0");
  if (o.maturity_years <= 0.0) throw std::invalid_argument("xp: maturity must be > 0");
}

}  // namespace

PricingResult price_european_bs(const EuropeanOption& option) {
  validate(option);
  using namespace QuantLib;

  // llround -> long long, then an explicit narrowing cast: portable across LP64 and LLP64.
  // Fixed reference date: only the year fraction matters, so results are reproducible.
  const Date today(15, January, 2025);
  const SavedSettings restore;  // restores the global evaluation date on exit
  Settings::instance().evaluationDate() = today;

  const DayCounter dc = Actual365Fixed();
  const Calendar cal = NullCalendar();
  const auto days = static_cast<Date::serial_type>(std::llround(option.maturity_years * 365.0));
  if (days < 1) throw std::invalid_argument("xp: maturity shorter than one day");
  const Date expiry = today + days;

  auto spot = ext::make_shared<SimpleQuote>(option.spot);
  const Handle<YieldTermStructure> rf(ext::make_shared<FlatForward>(today, option.rate, dc));
  const Handle<YieldTermStructure> div(
      ext::make_shared<FlatForward>(today, option.dividend_yield, dc));
  const Handle<BlackVolTermStructure> vol(
      ext::make_shared<BlackConstantVol>(today, cal, option.volatility, dc));
  auto process = ext::make_shared<BlackScholesMertonProcess>(Handle<Quote>(spot), div, rf, vol);

  const auto ql_type = option.type == OptionType::Call ? Option::Call : Option::Put;
  VanillaOption instrument(ext::make_shared<PlainVanillaPayoff>(ql_type, option.strike),
                           ext::make_shared<EuropeanExercise>(expiry));
  instrument.setPricingEngine(ext::make_shared<AnalyticEuropeanEngine>(process));

  return PricingResult{instrument.NPV(), instrument.delta(), instrument.gamma(), instrument.vega()};
}

}  // namespace xp

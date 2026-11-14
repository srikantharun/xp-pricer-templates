# The only build declaration a quant writes: WHAT, never HOW.
xp_pricer(
  NAME     black_scholes
  SOURCES  pricer.cpp
  HEADERS  include/xp/pricer.hpp
  TESTS    test_pricer.cpp
  USES     QuantLib::QuantLib
)

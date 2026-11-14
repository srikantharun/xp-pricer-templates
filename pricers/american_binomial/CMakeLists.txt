# The only build declaration a quant writes: WHAT, never HOW.
xp_pricer(
  NAME     american_binomial
  SOURCES  american_binomial.cpp
  HEADERS  include/xp/american_binomial.hpp
  TESTS    test_american_binomial.cpp
)

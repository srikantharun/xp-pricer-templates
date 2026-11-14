# The only build declaration a quant writes: WHAT, never HOW.
xp_pricer(
  NAME     bachelier
  SOURCES  bachelier.cpp
  HEADERS  include/xp/bachelier.hpp
  TESTS    test_bachelier.cpp
)

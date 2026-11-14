# The only build declaration a quant writes: WHAT, never HOW.
xp_pricer(
  NAME     barrier_mc
  SOURCES  barrier_mc.cpp
  HEADERS  include/xp/barrier_mc.hpp
  TESTS    test_barrier_mc.cpp
)

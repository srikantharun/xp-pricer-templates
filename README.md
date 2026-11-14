# xp-pricer-templates (quants)

Pricer code only — no build machinery. Pinned by tag (pricers-v1.0.0).

Each `pricers/<method>/` is self-contained: `include/xp/<name>.hpp`, source, Catch2 tests,
and a `CMakeLists.txt` that is nothing but a single `xp_pricer(...)` call (guard 4a enforces
that). Methods: `black_scholes` (reused verbatim from xp-pricer — real QuantLib analytic
engine, golden-value test), `bachelier`, `american_binomial`, `barrier_mc` (skeleton methods).

A quant adds a method by adding a folder here and cutting a new pricers-vX.Y.Z tag; they
never touch xp-platform.

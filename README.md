# Exact Cover using Dancing Links, algorithm D

This repository contains different implementations of algorithm D, from the Art of Computer Programming v4 f5C.

## Variations on algorithm D

1. `cover_original.cc` : Original algorithm D from the book.
2. `cover_mrv.cc`: Like (1), but with MRV heuristic added.
3. `cover_refactored.cc`: Like(2), but refactored to idiomatic C++17.
4. `cover_nospacer.cc`: Like (3), but with spacers removed.

## Measurements (perf stat)

Measurements taken while solving the 9-queens problems, as reported by `perf stat`. Results are average of 10 runs, and the model includes all symmetries.

Variation | Time | L1 read misses | LL cache misses | Branch mispredictions
--- | --- | --- | --- | ---
`cover_original` | 273s | 108M | 282k | 3.86G
`cover_mrv` | 167s | 106M | 659k | 930M

## Measurements (cachegrind)

Measurements taken while solving the 8-queens problems, as reported by `cachegrind`. Results are from a single emulated run, and the model includes all symmetries.

Variation | L1 read misses | LL read misses | Branch mispredictions
--- | --- | --- | ---
`cover_original` | 77.3k | 8.1k | 864M
`cover_mrv` | 118k | 8.1k | 556M
`cover_refactor` | 55.7k | 8.1k | 698M
`cover_nospacer` | 24.7k | 8.1k | 218M


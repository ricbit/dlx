# Exact Cover using Dancing Links

This repository contains different implementations of algorithm D, from the Art of Computer Programming v4 f5C.

## Variations on algorithm D

1. `cover_original.cc` : Original algorithm D from the book.
2. `cover_mrv.cc`: MRV heuristic added.
3. `cover_refactored.cc`: Refactored to idiomatic C++17.
4. `cover_nospacer.cc`: Spacers removed.

## Measurements (perf stat)

Measurements taken while solving the 9-queens problem, as reported by `perf stat`. Results are average of 10 runs, and the model includes all symmetries.

Variation | Time | L1 read misses | LL cache misses | Branch mispredictions
--- | --- | --- | --- | ---
`cover_original` | 263s | 136M | 3.99M | 3.88G
`cover_mrv` | 169s | 126M |  1.41M | 933M
`cover_refactor` | 170s | 107M | 1.09M | 771M
`cover_nospacer` | 163s | 57.7M | 319k | 564M

## Measurements (cachegrind)

Measurements taken while solving the 8-queens problem, as reported by `cachegrind`. Results are from a single emulated run, and the model includes all symmetries.

Variation | L1 read misses | LL read misses | Branch mispredictions
--- | --- | --- | ---
`cover_original` | 77.3k | 8.1k | 864M
`cover_mrv` | 118k | 8.1k | 556M
`cover_refactor` | 55.7k | 8.1k | 698M
`cover_nospacer` | 24.7k | 8.1k | 218M


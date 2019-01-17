# Exact Cover using Dancing Links

This repository contains different implementations of algorithm D, from the Art of Computer Programming v4 f5C.

## Variations on algorithm D

1. `cover_original.cc` : Original algorithm D from the book.
2. `cover_mrv.cc`: MRV heuristic added.
3. `cover_refactored.cc`: Refactored to idiomatic C++17.
4. `cover_nospacer.cc`: Spacers removed.
5. `cover_shortcut.cc`: Takes a shortcut on leaves of the branch tree.

## Measurements (8 queens, cachegrind)

Measurements taken while solving the 8-queens problem, as reported by `cachegrind`. Results are from a single emulated run, and the model includes slacks and all symmetries.

Variation | L1 read misses | LL read misses | Branch mispredictions
--- | --- | --- | ---
`cover_original` | 71.3k | 8.1k | 864M
`cover_mrv` | 110k | 8.1k | 557M
`cover_refactor` | 56.1k | 8.1k | 690M
`cover_nospacer` | 25.4k | 8.1k | 226M
`cover_shortcut` | 28.2k | 8.1k | 238M

## Measurements (9 queens, perf stat)

Measurements taken while solving the 9-queens problem, as reported by `perf stat`. Results are average of 3 runs, and the model includes slacks and all symmetries.

Variation | Time | L1 read misses | LL cache misses | Branch mispredictions
--- | --- | --- | --- | ---
`cover_original` | 258s | 114M | 2.13M | 3.11G
`cover_mrv` | 176s | 112M |  1.02M | 1.33G
`cover_refactor` | 169s | 157M | 6.09M | 771M
`cover_nospacer` | 163s | 72.3M | 1.03M | 597M
`cover_shortcut` | 163s | 73.2M | 931k | 570M

## Measurements (random, perf stat)

Measurements taken while solving a random exact cover problem, as reported by `perf stat`. Results are average of 3 runs, and the model has 3 items and 20000 options.

Variation | Time | L1 read misses | LL cache misses | Branch mispredictions
--- | --- | --- | --- | ---
`cover_original` | 366s | 86.5G | 3.42M | 128M
`cover_mrv` | 360s | 86.7G | 4.25M | 127M
`cover_refactor` | 342s | 86.0G | 511k | 117M
`cover_nospacer` | 321s | 76.8G | 7.85M | 125M
`cover_shortcut` | 117s | 30.6G | 6.25M | 92.5M



# Capacity-Aware Completion Bounds for Exact Fixed-Permutation EV Routing Decoding

Preprint candidate, September 5, 2026. Author: Ryutaro Yonezu (Independent Researcher).

## Status

This package is a reproducible preprint candidate and is **not peer reviewed**. The claims are intentionally narrow: completion bounding is established in labeling algorithms; this work specializes an admissible cargo-capacity relaxation to the recently introduced FPSCP / FP-FLA exact decoder.

## Main verified results

- Primary standalone public evaluation: 6 WCCI-2020 EVRP instances x 40 customer permutations = **240 cases**. The six-instance panel was selected for tractability plus coverage (21--50 customers, 4--9 charging stations), not as a random sample of the full suite.
- Differential objective agreement: all **240/240** capacity-bounded runs matched an unbounded source-matched FP-FLA reference that retained the original feasibility and Pareto-dominance pruning but used no incumbent or completion-bound pruning. This is not an independent MIP/CP oracle.
- Fair primary runtime control: incumbent-bounded FP-FLA using the **same feasible incumbent U**.
- Primary median preprocessing-inclusive speedup: **14.41x** (10,000-resample bootstrap 95% CI: **12.62--16.00x**).
- Median charging/reset-node pair reduction: **73.2%**.
- Initial incumbent strictly worse than the unbounded reference objective: **131/240** public cases.
- Synthetic stress validation: `PASS tested=996 skipped=4 nonopt_incumbent=425 no_initial_incumbent=0`.
- Median bound-specific preprocessing cost on the test machine: **1.57 microseconds**.
- Direct-upstream compatibility gate: **340/340** objective matches against the pinned compiled FPSCP core across 17 public EVRP instances; **34/34** retained clean-room checks also matched.
- Direct-upstream auxiliary measurements: median expensive block-oracle call ratio **0.039708** (3.97%); median wrapper/upstream external-time ratio **0.194738** (about **5.14x** at the median). These are gate measurements for a lazy split-DAG wrapper and are **not** the same experiment or implementation as the primary 14.41x capacity-bound benchmark.

## Direct-upstream gate

A separate implementation gate compiled the public FPSCP core pinned at commit `db9ccca30f2def5fabf85e65958d52dcae0cefd6` and compared upstream `OptimalDecode` against a lazy split-DAG wrapper whose expensive fixed-block charging oracle is the upstream `BatteryDecode`. The all-scope run covered 17 public instances and 20 permutations per instance (10 random + 10 nearest-neighbor-biased), for **340 cases**. Final verdict: `PASS_EXACT_REAL_GATE_MEASURE_PERFORMANCE`, with `upstream_lazy_match=340` and `cleanroom_match=34`.

The 17 instances were: `E-n101-k8`, `E-n22-k4`, `E-n23-k3`, `E-n30-k3`, `E-n33-k4`, `E-n51-k5`, `E-n76-k7`, `X-n1001-k43`, `X-n143-k7`, `X-n214-k11`, `X-n351-k40`, `X-n459-k26`, `X-n573-k30`, `X-n685-k75`, `X-n749-k98`, `X-n819-k171`, and `X-n916-k207`.

Important reproduction notes:

- The pinned upstream source files were kept byte-for-byte unchanged. On MSVC, the comparison wrapper force-included `<stdexcept>` because the pinned headers use `std::runtime_error` without directly including that standard header. This is a portability shim, not an algorithm change.
- Partial depot-to-depot block routes are validated with `Solution::is_energy_and_cargo_valid()`, not full `Solution::is_valid()`. Upstream `is_valid()` additionally requires that every customer in the entire instance appear exactly once, which is inappropriate for a block oracle. An earlier gate run used full `is_valid()` and produced 20 systematic mismatches on `E-n30-k3`; correcting that wrapper-level validation error yielded **340/340** all-scope agreement.
- This gate strengthens direct compatibility evidence with the parent implementation but is still **not an independent external-solver certificate** because it deliberately reuses the parent's compiled core.
- The gate's median external-time ratio of 0.194738 (roughly 5.14x) must not be merged with or substituted for the paper's primary 14.41x capacity-bound result; they measure different wrappers/implementations.

## Safety audit note

An earlier experimental bound used hidden `route_start` history while the FP-FLA dominance state did not include that variable. That formulation was discarded. The version in this package uses only the stage, current cargo resource, and precomputed permutation data. The bound therefore does not introduce hidden history into dominance.

## Files

- `paper.tex` - LaTeX source.
- `Yonezu_2026_Capacity_Aware_Completion_Bounds_EV_Routing.pdf` - rendered preprint candidate.
- `artifact/capacity_aware_fpfla_benchmark.cpp` - public benchmark harness and exact decoder variants.
- `artifact/capacity_aware_fpfla_stress.cpp` - synthetic exactness stress harness.
- `artifact/public_results_240.csv` - raw public results.
- `artifact/bound_preprocessing.csv` - bound preprocessing microbenchmark.
- `artifact/group_summary.csv`, `artifact/overall_summary.csv` - derived summaries.
- `artifact/analyze_results.py`, `artifact/make_figures.py` - analysis and figure generation.
- `artifact/stress_results.txt` - stress verdict.
- `artifact/direct_upstream_gate_summary.json` - all-scope direct-upstream gate summary and provenance notes.
- `figures/` - publication figures.
- `SHA256SUMS.txt` - file-integrity manifest.

## Reference implementation matched

The standalone decoder structure was matched to the public FPSCP repository main commit:
`db9ccca30f2def5fabf85e65958d52dcae0cefd6`

The inspected `src/evrp/optimal_decode.cpp` blob SHA was:
`1dd493bcf1410fbb2f626be4a0a06514d9a6d3de`

Parent paper: Leon Stjepan Uroic and Marko Durasevic, *Where to Split and When to Charge: Optimal Route Construction from Customer Permutations in Electric Vehicle Routing*, arXiv:2605.26816 (2026).

## Benchmarks

The benchmark instance files are not redistributed here. Obtain them from the public repository associated with:

M. Mavrovouniotis, C. Menelaou, S. Timotheou, G. Ellinas, C. Panayiotou, and M. Polycarpou, *A Benchmark Test Suite for the Electric Capacitated Vehicle Routing Problem*, IEEE CEC 2020, DOI 10.1109/CEC48606.2020.9185753.

Expected filenames for the public harness:
`E-n29-k4-s7.evrp`, `E-n30-k3-s7.evrp`, `E-n35-k3-s5.evrp`, `E-n37-k4-s4.evrp`, `F-n49-k4-s4.evrp`, `E-n60-k5-s9.evrp`.

## Benchmark panel and permutation sampling

The six public instances were chosen before timing analysis to keep the unbounded reference tractable under repeated measurements while spanning 21--50 customers and 4--9 charging stations. The reported 14.41x median is therefore a result for this 240-case panel, not a suite-wide speedup estimate. For each instance, 20 nearest-neighbor-biased permutations sample the next customer uniformly from the three nearest remaining customers, and 20 permutations are uniformly random. Permutation seed: `20260905`. The later 340-case direct-upstream gate is a separate compatibility experiment over 17 instances; its timing ratios are reported separately and are not pooled with the primary benchmark.

## Reproduction sketch

Compile with a modern C++20 compiler, e.g. `g++ -O3 -std=c++20`. Update the benchmark file paths in the harness if necessary, run it to regenerate `public_results_240.csv`, then run `analyze_results.py` and `make_figures.py`. The public harness exits immediately on any objective mismatch against the unbounded source-matched FP-FLA reference. Optimality preservation is established by the admissibility proof; the 240/240 standalone checks, 996-case synthetic checks, and 340/340 direct-upstream gate are differential implementation validation. The direct-upstream gate reduces source-divergence concern but still does not constitute an independent MIP/CP certificate.

## License

- Source code and software artifacts: MIT License (`LICENSE`).
- Preprint text/PDF: CC BY 4.0 (`LICENSE_PAPER.txt`).

## Prior-art audit

See `PRIOR_ART_AUDIT.md`. The novelty statement is deliberately narrow and the
search result is not presented as proof of priority.

# Computational evidence index

Status: post-publication evidence index for a future manuscript/repository update. The current Zenodo preprint remains unchanged.

This paper already has a substantial benchmark and differential-validation section. No redundant toy simulation is added.

## Primary benchmark

The source-matched capacity-bound experiment contains 240 public benchmark/permutation cases:

- six WCCI-2020 EVRP instances;
- 20 nearest-neighbor-biased permutations per instance;
- 20 random permutations per instance.

Results reported by the preprint:

- 240/240 bounded-decoder objective matches against the unbounded source-matched FP-FLA reference;
- median preprocessing-inclusive speedup: **14.41x**;
- bootstrap 95% CI: **12.62--16.00x**;
- median charging/reset-node pair reduction: **73.2%**;
- 131/240 public cases used an initial incumbent strictly worse than the unbounded reference optimum.

## Stress and compatibility evidence

Additional evidence is intentionally kept separate from the primary timing benchmark:

- 996 feasible synthetic stress cases, zero objective mismatches;
- 340/340 direct-upstream objective matches across 17 public EVRP instances;
- 34/34 retained clean-room checks.

The direct-upstream timing measurements use a different lazy split-DAG wrapper and must not be pooled with the 14.41x primary benchmark.

## Interpretation

The theorem establishes admissibility/optimality preservation. The experiments provide differential implementation validation and performance evidence. They are not an independent MIP/CP optimality certificate.

## Reproducibility rule

For any future manuscript version, the public benchmark, synthetic stress test, and direct-upstream compatibility gate should remain separate tables/figures so that exactness evidence is not conflated with performance measurements from different implementations.

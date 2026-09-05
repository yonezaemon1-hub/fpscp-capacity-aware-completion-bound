# Prior-art audit (2026-09-05)

## Narrow claim under audit

This repository does **not** claim that completion bounds, lower-bound pruning,
or labeling bounds are new in general. The narrow claim is a state-compatible,
cargo-aware completion bound specialized to the recently formalized
Fixed-Permutation Splitting and Charging Problem (FPSCP) / FP-FLA decoder.
The bound preserves the fixed remaining customer order and residual cargo
capacity, drops battery feasibility in the relaxation, and depends only on the
FP-FLA stage and current cargo resource.

## Direct parent check

Parent work:

- Leon Stjepan Uroić and Marko Đurasević, *Where to Split and When to Charge:
  Optimal Route Construction from Customer Permutations in Electric Vehicle
  Routing*, arXiv:2605.26816, submitted 2026-05-26.
- Public implementation checked at commit
  `db9ccca30f2def5fabf85e65958d52dcae0cefd6`.
- `src/evrp/optimal_decode.cpp` blob SHA:
  `1dd493bcf1410fbb2f626be4a0a06514d9a6d3de`.

The pinned FP-FLA implementation accepts an external upper bound and prunes
candidate extensions whose accumulated distance already exceeds that upper
bound. It does not implement the suffix/cargo completion bound proposed here.

## Closest established ideas

1. Froger, Mendoza, Jabali, Laporte (2019), *Improved formulations and
   algorithmic components for the electric vehicle routing problem with
   nonlinear charging functions*, Computers & Operations Research 104,
   256-294. DOI: 10.1016/j.cor.2018.12.013.
   Exact fixed-route charging and backward/bounding machinery are established
   prior art.

2. Kullman, Froger, Mendoza, Goodson (2021), *frvcpy: An Open-Source Solver for
   the Fixed Route Vehicle Charging Problem*, INFORMS Journal on Computing
   33(4), 1277-1283. DOI: 10.1287/ijoc.2020.1035.
   Open-source exact fixed-route charging is established prior art.

3. Enerbäck, Eveborn, Rönnberg (2024), *Pricing for the EVRPTW with Piecewise
   Linear Charging by a Bounding-Based Labeling Algorithm*, ATMOS 2024.
   DOI: 10.4230/OASIcs.ATMOS.2024.3.
   EV-routing completion bounds for labeling algorithms are established prior
   art.

## Search result

Searches through 2026-09-05 for combinations of `FPSCP`, `FP-FLA`, fixed
customer permutation, completion bound, cargo/residual capacity, suffix split,
and electric-vehicle routing did not identify a published follow-up applying
this specific state-compatible cargo-capacity completion relaxation to FPSCP /
FP-FLA.

This is a scoped literature-search result, **not proof of priority**. If a
closer pre-existing source is found, the novelty statement must be narrowed or
withdrawn accordingly.

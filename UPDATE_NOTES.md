# Publication update notes - 2026-09-05

This update folds the completed direct-upstream validation gate into the preprint materials without conflating it with the primary standalone timing experiment.

Updated claims:
- Primary capacity-aware benchmark remains 240/240 source-matched objective agreement and 14.41x median preprocessing-inclusive speedup (95% bootstrap CI 12.62-16.00x) against incumbent-bounded FP-FLA using the same feasible incumbent.
- Synthetic validation remains 996 feasible cases with no objective mismatch against the source-matched reference construction.
- New direct-upstream compatibility evidence: 340/340 objective matches over 17 public EVRP instances, with 34/34 retained clean-room checks.
- Direct-upstream median expensive block-oracle call ratio: 0.039708 (3.97%).
- Direct-upstream median wrapper/upstream external-time ratio: 0.194738 (approximately 5.14x at the median). This is explicitly kept separate from the primary 14.41x capacity-bound result because the gate uses a different lazy split-DAG wrapper.

Reproducibility notes added:
- Pinned upstream commit: db9ccca30f2def5fabf85e65958d52dcae0cefd6.
- Pinned optimal_decode.cpp blob SHA: 1dd493bcf1410fbb2f626be4a0a06514d9a6d3de.
- MSVC portability wrapper force-includes <stdexcept>; pinned upstream source bytes are unchanged.
- Partial block routes must use Solution::is_energy_and_cargo_valid(), not full Solution::is_valid(). The earlier use of full is_valid() caused 20 systematic E-n30-k3 gate mismatches and was corrected before the 340/340 all-scope pass.

This ZIP is an update bundle, not the entire research artifact repository. Merge these files into the existing artifact/repository and regenerate the repository-wide SHA256SUMS.txt afterward.

Final typesetting polish after independent review:
- Rephrased the instance/permutation-group sentence to avoid a nonbreakable slash sequence.
- Added an explicit break opportunity in the long partial-route validator name to prevent overfull text when microtype is unavailable.
- Reordered the Enerback (2024) and Kullman et al. (2021) bibliography entries to match first-citation order.
- The reported 21.51x / 6.42x category medians were left unchanged; the review found no numerical contradiction, only a reminder that these are not the median of the six table-row medians.

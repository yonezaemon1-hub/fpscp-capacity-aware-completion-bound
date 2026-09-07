# Experiment tables

- `experiment_summary.csv` — consolidated headline evidence blocks.
- `../artifact/public_results_240.csv` — all 240 primary public benchmark/permutation cases.
- `../artifact/overall_summary.csv` — primary aggregate speedup/reduction statistics.
- `../artifact/group_summary.csv` — grouped benchmark summaries.
- `../artifact/stress_results.txt` — synthetic stress-test result.
- `../artifact/direct_upstream_gate_summary.json` — 340-case direct-upstream compatibility gate and 34 retained clean-room checks.

Do not pool the direct-upstream timing numbers with the primary 14.41x benchmark; they use a different lazy split-DAG wrapper.

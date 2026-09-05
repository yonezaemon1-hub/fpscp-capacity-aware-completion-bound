import pandas as pd
import numpy as np
from pathlib import Path

HERE = Path(__file__).resolve().parent
df = pd.read_csv(HERE/'public_results_240.csv')
prep = pd.read_csv(HERE/'bound_preprocessing.csv')
df = df.merge(prep,on=['inst','ptype','rep','n'],how='left')
df['boundprep_us']=df['boundprep_ns']/1000.0
for k in ['direct','cap']:
    df[f'speed_{k}_vs_ub'] = df['ub_us'] / df[f'{k}_us']
    df[f'pairred_{k}_vs_ub'] = 1.0 - df[f'{k}_pairs'] / df['ub_pairs']
df['cap_total_us']=df['cap_us']+df['boundprep_us']
df['speed_cap_vs_ub_inclprep']=df['ub_us']/df['cap_total_us']
df['frontred_cap'] = 1.0 - df['maxfront_cap'] / df['maxfront_base']
df['ub_gap_pct'] = (df['init_ub'] / df['opt'] - 1.0) * 100.0
df['ub_nonoptimal'] = df['init_ub'] > df['opt'] + 1e-8

g = df.groupby(['inst','ptype']).agg(
    cases=('rep','size'),
    ub_only_ms=('ub_us', lambda x: np.median(x)/1000.0),
    direct_ms=('direct_us', lambda x: np.median(x)/1000.0),
    capacity_decoder_ms=('cap_us', lambda x: np.median(x)/1000.0),
    bound_preprocess_us=('boundprep_us','median'),
    capacity_total_ms=('cap_total_us', lambda x: np.median(x)/1000.0),
    direct_speedup_vs_ub=('speed_direct_vs_ub','median'),
    capacity_speedup_vs_ub_inclprep=('speed_cap_vs_ub_inclprep','median'),
    capacity_pair_reduction_vs_ub=('pairred_cap_vs_ub','median'),
    baseline_max_front=('maxfront_base','median'),
    capacity_max_front=('maxfront_cap','median'),
    nonoptimal_incumbents=('ub_nonoptimal','sum'),
    median_incumbent_gap_pct=('ub_gap_pct','median'),
).reset_index()
g.to_csv(HERE/'group_summary.csv', index=False)

def bootstrap_ci(x, seed=20260905, B=10000):
    x = np.asarray(x, dtype=float)
    rng = np.random.default_rng(seed)
    vals = np.empty(B)
    for b in range(B):
        vals[b] = np.median(rng.choice(x, len(x), replace=True))
    return np.quantile(vals, [0.025,0.975])

rows=[]
for name,mask in [('all',np.ones(len(df),dtype=bool)),('knn3',df.ptype.eq('knn3')),('random',df.ptype.eq('random'))]:
    sub=df[mask]
    lo,hi=bootstrap_ci(sub.speed_cap_vs_ub_inclprep)
    rows.append({
        'group':name,
        'cases':len(sub),
        'capacity_speedup_median_inclprep':sub.speed_cap_vs_ub_inclprep.median(),
        'capacity_speedup_bootstrap_ci_low':lo,
        'capacity_speedup_bootstrap_ci_high':hi,
        'direct_speedup_median':sub.speed_direct_vs_ub.median(),
        'capacity_pair_reduction_median':sub.pairred_cap_vs_ub.median(),
        'capacity_front_reduction_median':sub.frontred_cap.median(),
        'bound_preprocess_us_median':sub.boundprep_us.median(),
        'nonoptimal_incumbents':int(sub.ub_nonoptimal.sum()),
    })
pd.DataFrame(rows).to_csv(HERE/'overall_summary.csv',index=False)
print(pd.DataFrame(rows).to_string(index=False))
print('\nGroup summary:')
print(g.to_string(index=False))

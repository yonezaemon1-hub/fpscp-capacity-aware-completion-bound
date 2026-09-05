import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path
HERE=Path(__file__).resolve().parent
OUT=HERE.parent/'figures'
g=pd.read_csv(HERE/'group_summary.csv')
g['short']=g['inst'].str.replace('.evrp','',regex=False)+'\n'+g['ptype']
fig,ax=plt.subplots(figsize=(8.2,4.2))
ax.bar(range(len(g)),g['capacity_speedup_vs_ub_inclprep'])
ax.axhline(1.0,linewidth=0.8)
ax.set_ylabel('Median speedup vs. incumbent-bounded FP-FLA')
ax.set_xticks(range(len(g)))
ax.set_xticklabels(g['short'],rotation=55,ha='right',fontsize=8)
ax.set_title('Capacity-aware completion bound (preprocessing included)')
fig.tight_layout();fig.savefig(OUT/'speedup_by_group.pdf',bbox_inches='tight');fig.savefig(OUT/'speedup_by_group.png',dpi=220,bbox_inches='tight');plt.close(fig)
fig,ax=plt.subplots(figsize=(8.2,4.2))
ax.bar(range(len(g)),100*g['capacity_pair_reduction_vs_ub'])
ax.set_ylabel('Median station-pair reduction (%)');ax.set_ylim(0,100)
ax.set_xticks(range(len(g)));ax.set_xticklabels(g['short'],rotation=55,ha='right',fontsize=8)
ax.set_title('Station-pair enumeration removed before expansion')
fig.tight_layout();fig.savefig(OUT/'pair_reduction_by_group.pdf',bbox_inches='tight');fig.savefig(OUT/'pair_reduction_by_group.png',dpi=220,bbox_inches='tight');plt.close(fig)

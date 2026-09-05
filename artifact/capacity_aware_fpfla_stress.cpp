#include <bits/stdc++.h>
using namespace std;
static constexpr double INF=1e100, EPS=1e-9;
struct Node{double x,y; int dem;};
struct Inst{int n,m; int Q; double B; vector<Node> nd; vector<vector<double>> d; double dist(int a,int b)const{return d[a][b];}};
struct Lab{double cost,batt; int cargo,route_start;};
struct Stats{long long popped=0,pairs=0,props=0,lbpr=0; long long us=0; size_t maxfront=0;};

vector<Lab> collapse(vector<Lab> v){
 sort(v.begin(),v.end(),[](const Lab&a,const Lab&b){if(a.cost!=b.cost)return a.cost<b.cost;if(a.batt!=b.batt)return a.batt<b.batt;return a.cargo<b.cargo;});
 map<double,int> sky; vector<Lab> out; out.reserve(v.size());
 for(const auto& f:v){
   auto it=sky.upper_bound(f.batt);
   if(!sky.empty() && it!=sky.begin()){auto q=prev(it); if(q->second<=f.cargo) continue;}
   out.push_back(f);
   while(it!=sky.end() && it->second>=f.cargo) it=sky.erase(it);
   auto ex=sky.find(f.batt); if(ex==sky.end()||f.cargo<ex->second) sky[f.batt]=f.cargo;
 }
 return out;
}

// Reset-node shortest paths exactly mirroring FP-FLA: index 0 = depot, 1..m = charging stations.
struct ResetMat{
 int z; vector<int> id; vector<vector<double>> sp;
 ResetMat(const Inst&I):z(I.m+1),id(z),sp(z,vector<double>(z,INF)){
   id[0]=0; for(int s=0;s<I.m;s++)id[s+1]=I.n+1+s;
   for(int i=0;i<z;i++){sp[i][i]=0;for(int j=i+1;j<z;j++){double w=I.dist(id[i],id[j]);if(w<=I.B+EPS)sp[i][j]=sp[j][i]=w;}}
   for(int k=0;k<z;k++)for(int i=0;i<z;i++)for(int j=0;j<z;j++)if(sp[i][k]+sp[k][j]<sp[i][j])sp[i][j]=sp[i][k]+sp[k][j];
 }
};

// Feasible split-then-charge upper bound (fixed split from cargo-only shortest path; charging only via stations, no extra depot split).
struct StationMat{
 int m; vector<int> id; vector<vector<double>> sp;
 StationMat(const Inst&I):m(I.m),id(m),sp(m,vector<double>(m,INF)){
   for(int s=0;s<m;s++){id[s]=I.n+1+s;sp[s][s]=0;}
   for(int i=0;i<m;i++)for(int j=i+1;j<m;j++){double w=I.dist(id[i],id[j]);if(w<=I.B+EPS)sp[i][j]=sp[j][i]=w;}
   for(int k=0;k<m;k++)for(int i=0;i<m;i++)for(int j=0;j<m;j++)if(sp[i][k]+sp[k][j]<sp[i][j])sp[i][j]=sp[i][k]+sp[k][j];
 }
};
vector<pair<double,double>> collapse2(vector<pair<double,double>> v){sort(v.begin(),v.end());vector<pair<double,double>>o;double best=INF;for(auto x:v)if(x.second<best-EPS){o.push_back(x);best=x.second;}return o;}
double fixedRoute(const Inst&I,const StationMat&M,const vector<int>&p,int a,int b){
 vector<int>s={0};for(int i=a;i<=b;i++)s.push_back(p[i]);s.push_back(0);
 vector<pair<double,double>> f={{0,0}};
 for(int t=1;t<(int)s.size();t++){
   vector<pair<double,double>> g; int u=s[t-1],v=s[t];
   for(auto [c,bat]:f){double w=I.dist(u,v);if(bat+w<=I.B+EPS)g.push_back({c+w,bat+w});
     for(int x=0;x<M.m;x++){double a1=I.dist(u,M.id[x]);if(bat+a1>I.B+EPS)continue;for(int y=0;y<M.m;y++){if(M.sp[x][y]>=INF/2)continue;double a3=I.dist(M.id[y],v);if(a3>I.B+EPS)continue;g.push_back({c+a1+M.sp[x][y]+a3,a3});}}
   }
   if(g.empty())return INF; f=collapse2(move(g));
 }
 double z=INF;for(auto x:f)z=min(z,x.first);return z;
}

struct Pre{
 vector<int> pref,maxEnd,lg; vector<double> chain,suffix,A; vector<vector<double>> st; double cargoLB=INF, feasibleUB=INF; vector<pair<int,int>> split;
};
Pre prep(const Inst&I,const StationMat&M,const vector<int>&p){
 int n=p.size();Pre S;S.pref.assign(n+1,0);for(int i=0;i<n;i++)S.pref[i+1]=S.pref[i]+I.nd[p[i]].dem;
 S.chain.assign(n,0);for(int i=1;i<n;i++)S.chain[i]=S.chain[i-1]+I.dist(p[i-1],p[i]);
 auto seg=[&](int a,int b){return I.dist(0,p[a])+(b>a?S.chain[b]-S.chain[a]:0)+I.dist(p[b],0);};
 vector<double>dp(n+1,INF);vector<int>pr(n+1,-1);dp[0]=0;
 for(int j=1;j<=n;j++)for(int a=j-1;a>=0;a--){if(S.pref[j]-S.pref[a]>I.Q)break;double z=dp[a]+seg(a,j-1);if(z<dp[j]){dp[j]=z;pr[j]=a;}}
 S.cargoLB=dp[n];for(int j=n;j>0;){int a=pr[j];S.split.push_back({a,j-1});j=a;}reverse(S.split.begin(),S.split.end());
 double ub=0;bool ok=true;for(auto [a,b]:S.split){double z=fixedRoute(I,M,p,a,b);if(z>=INF/2){ok=false;break;}ub+=z;}S.feasibleUB=ok?ub:INF;
 S.suffix.assign(n+1,INF);S.suffix[n]=0;for(int a=n-1;a>=0;a--)for(int b=a;b<n;b++){if(S.pref[b+1]-S.pref[a]>I.Q)break;S.suffix[a]=min(S.suffix[a],seg(a,b)+S.suffix[b+1]);}
 S.maxEnd.assign(n,n-1);int r=0;for(int a=0;a<n;a++){r=max(r,a);while(r+1<n&&S.pref[r+2]-S.pref[a]<=I.Q)r++;S.maxEnd[a]=r;}
 S.A.assign(n,INF);for(int b=0;b<n;b++)S.A[b]=S.chain[b]+I.dist(p[b],0)+S.suffix[b+1];
 S.lg.assign(n+1,0);for(int i=2;i<=n;i++)S.lg[i]=S.lg[i/2]+1;int K=S.lg[n]+1;S.st.assign(K,vector<double>(n));S.st[0]=S.A;
 for(int k=1;k<K;k++)for(int i=0;i+(1<<k)<=n;i++)S.st[k][i]=min(S.st[k-1][i],S.st[k-1][i+(1<<(k-1))]);
 return S;
}
double rmq(const Pre&S,int l,int r){int k=S.lg[r-l+1];return min(S.st[k][l],S.st[k][r-(1<<k)+1]);}
enum Mode{NO_LB,DIRECT_LB,CAP_LB};

double solve(const Inst&I,const ResetMat&R,const vector<int>&p,const Pre&S,double U,Mode mode,Stats&st){
 auto t0=chrono::steady_clock::now();int n=p.size(); vector<Lab>f={{0,0,0,0}}; double incumbent=U;
 auto h=[&](const Lab&L,int served){
   if(mode==NO_LB)return 0.0;
   if(served==0){if(mode==CAP_LB)return S.suffix[0];double z=I.dist(0,p[0]);for(int k=0;k<n-1;k++)z+=I.dist(p[k],p[k+1]);z+=I.dist(p[n-1],0);return z;}
   int last=served-1;
   if(mode==DIRECT_LB){double z=0;for(int k=last;k<n-1;k++)z+=I.dist(p[k],p[k+1]);z+=I.dist(p[n-1],0);return z;}
   double close_now=I.dist(p[last],0)+S.suffix[served];
   int remaining=I.Q-L.cargo;
   if(remaining<=0 || served>=n) return close_now;
   int threshold=S.pref[served]+remaining;
   auto it=upper_bound(S.pref.begin()+served+1,S.pref.end(),threshold);
   int J=int(it-S.pref.begin())-2;
   if(J<served) return close_now;
   return min(close_now, rmq(S,served,J)-S.chain[last]);
 };
 for(int step=0;step<=n;step++){
   int prev=(step==0?0:p[step-1]), cur=(step==n?0:p[step]); vector<Lab>g;
   for(auto &L:f){st.popped++;
     if(incumbent<INF/2&&mode!=NO_LB&&L.cost+h(L,step)>=incumbent-EPS){st.lbpr++;continue;}
     auto accept=[&](Lab x,int served){st.props++;if(x.cargo>I.Q||x.batt>I.B+EPS)return; if(x.cost>incumbent+EPS)return; if(incumbent<INF/2&&mode!=NO_LB&&served<=n&&x.cost+h(x,served)>=incumbent-EPS){st.lbpr++;return;}g.push_back(x);};
     // direct
     {Lab x=L;double w=I.dist(prev,cur);x.cost+=w;x.batt+=w;if(step<n){x.cargo+=I.nd[cur].dem;if(step==0)x.route_start=0;} accept(x,step+1);}
     // all reset-node pair transitions exactly as original FP-FLA
     for(int a=0;a<R.z;a++){int fi=R.id[a];double d1=I.dist(prev,fi);if(L.batt+d1>I.B+EPS)continue;
       for(int b=0;b<R.z;b++){st.pairs++;if(R.sp[a][b]>=INF/2)continue;int fo=R.id[b];double d3=I.dist(fo,cur);if(d3>I.B+EPS)continue;
         // charging detour (cargo not reset even if shortest reset path touches depot; mirrors source implementation)
         {Lab x=L;x.cost+=d1+R.sp[a][b]+d3;x.batt=d3;if(step<n){x.cargo+=I.nd[cur].dem;if(step==0)x.route_start=0;}accept(x,step+1);}
         // depot detour: explicit via reset index 0; cargo resets at current customer
         {Lab x=L;x.cost+=d1+R.sp[a][0]+R.sp[0][b]+d3;x.batt=d3;if(step<n){x.cargo=I.nd[cur].dem;x.route_start=step;}else{x.cargo=0;x.route_start=n;}accept(x,step+1);}
       }
     }
   }
   if(g.empty()){f.clear();break;} f=collapse(move(g));st.maxfront=max(st.maxfront,f.size());
 }
 double best=incumbent;for(auto &x:f)best=min(best,x.cost);st.us=chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now()-t0).count();return best;
}

Inst gen(mt19937_64&rng,int n,int m){
 uniform_real_distribution<double>U(0,1000);uniform_int_distribution<int>D(1,10);Inst I;I.n=n;I.m=m;I.Q=(n<=30?28:36);I.B=1000;I.nd.resize(n+m+1);I.nd[0]={500,500,0};
 for(int i=1;i<=n;i++)I.nd[i]={U(rng),U(rng),D(rng)};
 // 8+ ring-like coverage stations; jittered concentric placement to keep reset graph connected
 for(int s=0;s<m;s++){double a=2*M_PI*s/m;double rad=(s%2?260:360);I.nd[n+1+s]={500+rad*cos(a),500+rad*sin(a),0};}
 int N=n+m+1;I.d.assign(N,vector<double>(N));for(int i=0;i<N;i++)for(int j=0;j<N;j++)I.d[i][j]=hypot(I.nd[i].x-I.nd[j].x,I.nd[i].y-I.nd[j].y);return I;
}
vector<int>knn(const Inst&I,mt19937_64&rng,int k=3){vector<int>r(I.n);iota(r.begin(),r.end(),1);vector<int>p;int u=0;while(!r.empty()){vector<pair<double,int>>v;for(int x:r)v.push_back({I.dist(u,x),x});sort(v.begin(),v.end());int K=min(k,(int)v.size());uniform_int_distribution<int>P(0,K-1);int x=v[P(rng)].second;p.push_back(x);u=x;r.erase(find(r.begin(),r.end(),x));}return p;}
int main(){
 mt19937_64 rng(917263); int tested=0, skipped=0, nonopt=0, noinc=0;
 for(int t=0;t<1000;t++){
   int n=8+(rng()%15),m=4+(rng()%7);Inst I=gen(rng,n,m); I.B=900+(rng()%301); I.Q=18+(rng()%25);
   ResetMat R(I);StationMat M(I);auto p=knn(I,rng,(t%2)?3:n);auto S=prep(I,M,p);Stats a,b,c,d;double opt=solve(I,R,p,S,INF,NO_LB,a);if(opt>=INF/2){skipped++;continue;}double U=S.feasibleUB; if(U>=INF/2) noinc++; else if(U>opt+1e-7) nonopt++;
   double q=solve(I,R,p,S,U,NO_LB,b),r=solve(I,R,p,S,U,DIRECT_LB,c),z=solve(I,R,p,S,U,CAP_LB,d);
   if(abs(opt-q)>1e-6||abs(opt-r)>1e-6||abs(opt-z)>1e-6){cerr<<setprecision(15)<<"FAIL t="<<t<<" opt="<<opt<<" ub="<<q<<" direct="<<r<<" cap="<<z<<" U="<<U<<"\n";return 3;} tested++;
 }
 cout<<"PASS tested="<<tested<<" skipped="<<skipped<<" nonopt_incumbent="<<nonopt<<" no_initial_incumbent="<<noinc<<"\n";
}

// Luogu P5043
// DeP
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 55, MAXM = 1e3 + 5;

int m, N[MAXN];
int Ans[MAXN];
vector<int> g[MAXN];

bool notPrime[MAXM];
int Prime[MAXM], tot;

void EulerSieve() {
  notPrime[1] = true;
  for (int i = 2; i < MAXM; ++i) {
    if (!notPrime[i]) Prime[++tot] = i;
    for (int j = 1; j <= tot && i * Prime[j] < MAXM; ++j) {
      notPrime[i * Prime[j]] = true;
      if (i % Prime[j] == 0) break;
    }
  }
}

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXN << 1];
  int head[MAXN], size[MAXN], eidx;
  LL f[MAXN];

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }

  void dfs(int u, int fa) {
    size[u] = 1;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if ((v = edges[i].to) == fa) continue;
      dfs(v, u), size[u] += size[v];
    }
    f[u] = 1;
    for (int v, i = head[u]; ~i; i = edges[i].nxt)
      if ((v = edges[i].to) != fa) f[u] += f[v] * Prime[size[v]];
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  EulerSieve();

  scanf("%d", &m);
  for (int k = 1; k <= m; ++k) {
    Graph::init();
    int& n = N[k];
    scanf("%d", &n);
    for (int fa, i = 1; i <= n; ++i) {
      scanf("%d", &fa);
      if (fa) Graph::AddEdge(fa, i), Graph::AddEdge(i, fa);
    }
    for (int u = 1; u <= n; ++u)
      Graph::dfs(u, 0), g[k].push_back(Graph::f[u]);
    sort(g[k].begin(), g[k].end());
  }

  for (int i = 1; i <= m; ++i) {
    Ans[i] = i;
    for (int j = 1; j < i; ++j)
      if (g[i] == g[j]) { Ans[i] = Ans[j]; break; }
  }

  for (int i = 1; i <= m; ++i) printf("%d\n", Ans[i]);
  return 0;
}

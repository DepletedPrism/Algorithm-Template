// CF 1009F
// DeP
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 1e6 + 5;

int n;
int Ans[MAXN];

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXN << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }
}

namespace LPD {
  using namespace Graph;
  int *f[MAXN], bkt[MAXN], *id = bkt;
  int pre[MAXN], son[MAXN], d[MAXN];

  void dfs1(int u, int fa) {
    d[u] = 1, pre[u] = fa, son[u] = -1;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if ((v = edges[i].to) == fa) continue;
      dfs1(v, u);
      if (son[u] == -1 || d[v] > d[son[u]])
        son[u] = v, d[u] = d[v] + 1;
    }
  }

  inline void update(int u, int j) {
    if (f[u][j] > f[u][Ans[u]] ||
        (f[u][j] == f[u][Ans[u]] && j < Ans[u])) Ans[u] = j;
  }

  void dfs2(int u) {
    if (~son[u])
      f[son[u]] = f[u] + 1, dfs2(son[u]), Ans[u] = Ans[son[u]] + 1;
    f[u][0] = 1, update(u, 0);
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if ((v = edges[i].to) == pre[u] || v == son[u]) continue;
      f[v] = id, id += d[v], dfs2(v);
      for (int j = 1; j <= d[v]; ++j) f[u][j] += f[v][j - 1], update(u, j);
    }
  }

  inline void solve(int rt = 1) { dfs1(rt, 0), f[rt] = id, id += d[rt], dfs2(rt); }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  scanf("%d", &n);
  for (int u, v, i = 1; i < n; ++i)
    scanf("%d%d", &u, &v), Graph::AddEdge(u, v), Graph::AddEdge(v, u);

  LPD::solve();

  for (int i = 1; i <= n; ++i) printf("%d\n", Ans[i]);
  return 0;
}

// Luogu P3379
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

namespace IO {
  const int MAXSIZE = 1 << 18 | 1;
  char buf[MAXSIZE], *p1, *p2;

  inline int Gc() {
    return p1 == p2 &&
      (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++;
  }
  template<typename T> inline void read(T& x) {
    x = 0; int f = 0, ch = Gc();
    while (!isdigit(ch)) f |= ch == '-', ch = Gc();
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

const int MAXN = 5e5 + 5, LOG = 20;

int n, rt, q;
int depth[MAXN << 1], rnk[MAXN << 1], elr[MAXN << 1], clk;

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXN << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }

  void dfs(int u, int fa) {
    rnk[elr[u] = ++clk] = u;
    depth[clk] = depth[elr[fa]] + 1;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if ((v = edges[i].to) == fa) continue;
      dfs(v, u), rnk[++clk] = u, depth[clk] = depth[elr[u]];
    }
  }
}

namespace ST {
  int lg2[MAXN << 1], Mn[LOG][MAXN << 1];

  void build() {
    for (int i = 1; i <= clk; ++i) Mn[0][i] = i;
    for (int j = 1; (1 << j) <= clk; ++j)
      for (int i = 1; i + (1 << j) - 1 <= clk; ++i)
        Mn[j][i] = (depth[Mn[j-1][i]] < depth[Mn[j-1][i + (1 << (j-1))]])
          ? Mn[j-1][i] : Mn[j-1][i + (1 << (j-1))];
    lg2[1] = 0;
    for (int i = 2; i <= clk; ++i) lg2[i] = lg2[i >> 1] + 1;
  }

  inline int Qry(int L, int R) {
    if (L > R) swap(L, R);
    int k = lg2[R - L + 1];
    return (depth[Mn[k][L]] < depth[Mn[k][R - (1 << k) + 1]])
      ? rnk[Mn[k][L]] : rnk[Mn[k][R - (1 << k) + 1]];
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n), read(q), read(rt);
  for (int u, v, i = 1; i < n; ++i)
    read(u), read(v), Graph::AddEdge(u, v), Graph::AddEdge(v, u);

  Graph::dfs(rt, 0), ST::build();

  while (q--) {
    static int u, v;
    read(u), read(v), printf("%d\n", ST::Qry(elr[u], elr[v]));
  }
  return 0;
}

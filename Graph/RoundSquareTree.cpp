// UOJ #416
// DeP
#include <cctype>
#include <cstdio>
#include <vector>
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

typedef long long LL;
const int MAXN = 1e5 + 5, MAXM = 2e5 + 5;

int n, m; LL ans;

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXM << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }
}

vector<int> T[MAXN << 1];
int wgt[MAXN << 1], val[MAXN << 1], sq, subsize;

void dfs(int u, int fa) {
  val[u] = (u <= n);
  for (const int& v: T[u]) if (v != fa) {
    dfs(v, u);
    ans += (LL) wgt[u] * val[u] * val[v], val[u] += val[v];
  }
  ans += (LL) wgt[u] * val[u] * (subsize - val[u]);
}

namespace BCC {
  using namespace Graph;
  int dfn[MAXN], low[MAXN], clk;
  int stk[MAXN], top;

  void tarjan(int u) {
    dfn[u] = low[u] = ++clk;
    stk[++top] = u, ++subsize;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if (!dfn[v = edges[i].to]) {
        tarjan(v), low[u] = min(low[u], low[v]);
        if (dfn[u] == low[v]) {
          wgt[++sq] = 0;
          while (true) {
            int x = stk[top--];
            ++wgt[sq], T[sq].push_back(x), T[x].push_back(sq);
            if (x == v) break;
          }
          ++wgt[sq], T[sq].push_back(u), T[u].push_back(sq);
        }
      } else
        low[u] = min(low[u], dfn[v]);
    }
  }

  void solve() {
    clk = top = 0, sq = n;
    memset(wgt, -1, (n + 1) * sizeof (int));
    for (int u = 1; u <= n; ++u)
      if (!dfn[u]) subsize = top = 0, tarjan(u), dfs(u, 0);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n), read(m);
  for (int u, v, i = 1; i <= m; ++i)
    read(u), read(v), Graph::AddEdge(u, v), Graph::AddEdge(v, u);
  BCC::solve();

  printf("%lld\n", 2 * ans);
  return 0;
}

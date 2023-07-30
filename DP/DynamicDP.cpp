// Luogu P4719
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
    while (isdigit(ch)) x = x * 10 - '0' + ch, ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

const int MAXN = 1e5 + 5, MAXM = 2, INF = 0x3f3f3f3f;

int n, m;
int A[MAXN], f[2][MAXN];

struct Matrix {
  int g[MAXM][MAXM];
  Matrix() { memset(g, 0, sizeof g); }
  Matrix operator * (const Matrix& rhs) const {
    Matrix ret;
    for (int i = 0; i < MAXM; ++i)
      for (int k = 0; k < MAXM; ++k) {
        ret.g[i][0] = max(ret.g[i][0], g[i][k] + rhs.g[k][0]);
        ret.g[i][1] = max(ret.g[i][1], g[i][k] + rhs.g[k][1]);
      }
    return ret;
  }
} g[MAXN];

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXN << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }
}

namespace HLD {
  using namespace Graph;
  int depth[MAXN], size[MAXN], pre[MAXN], son[MAXN];
  int topfa[MAXN], Ed[MAXN], dfn[MAXN], rnk[MAXN], clk;

  void dfs1(int u, int fa) {
    f[0][u] = 0, f[1][u] = A[u];
    depth[u] = depth[fa] + 1, size[u] = 1, son[u] = -1, pre[u] = fa;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if ((v = edges[i].to) == fa) continue;
      dfs1(v, u), size[u] += size[v];
      f[0][u] += max(f[0][v], f[1][v]), f[1][u] += f[0][v];
      if (son[u] == -1 || size[v] > size[son[u]]) son[u] = v;
    }
  }

  void dfs2(int u, int top) {
    g[u].g[1][0] = A[u], g[u].g[1][1] = -INF;
    topfa[u] = top, rnk[dfn[u] = ++clk] = u, Ed[top] = clk;
    if (~son[u]) dfs2(son[u], top);
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if ((v = edges[i].to) == pre[u] || v == son[u]) continue;
      dfs2(v, v);
      g[u].g[0][0] += max(f[0][v], f[1][v]), g[u].g[1][0] += f[0][v];
    }
    g[u].g[0][1] = g[u].g[0][0];
  }

  inline void solve(int rt = 1) { dfs1(rt, 0), dfs2(rt, rt); }
}

#define lc (nd << 1)
#define rc (nd << 1 | 1)
namespace SGT {
  Matrix dat[MAXN << 2];

  inline void maintain(const int& nd) { dat[nd] = dat[lc] * dat[rc]; }

  void build(int nd, int L, int R) {
    if (L == R) return void( dat[nd] = g[HLD::rnk[L]] );
    int Mid = (L + R) / 2;
    build(lc, L, Mid), build(rc, Mid+1, R);
    maintain(nd);
  }

  void Mdy(int nd, int L, int R, const int& p) {
    if (L == R) return void( dat[nd] = g[HLD::rnk[L]] );
    int Mid = (L + R) / 2;
    if (p <= Mid) Mdy(lc, L, Mid, p); else Mdy(rc, Mid+1, R, p);
    maintain(nd);
  }

  Matrix Qry(int nd, int L, int R, const int& opL, const int& opR) {
    if (opL <= L && R <= opR) return dat[nd];
    int Mid = (L + R) / 2;
    if (opR <= Mid) return Qry(lc, L, Mid, opL, opR);
    if (opL > Mid) return Qry(rc, Mid+1, R, opL, opR);
    return Qry(lc, L, Mid, opL, opR) * Qry(rc, Mid+1, R, opL, opR);
  }
}
#undef lc
#undef rc

namespace HLD {
  inline Matrix Qry(const int& u) { return SGT::Qry(1, 1, n, dfn[u], Ed[u]); }

  void Mdy(int u, const int& v) {
    g[u].g[1][0] += v - A[u], A[u] = v;
    while (u > 0) {
      int fa = pre[topfa[u]];
      Matrix lst = Qry(topfa[u]);
      if (fa) {
        g[fa].g[0][0] -= max(lst.g[0][0], lst.g[1][0]);
        g[fa].g[1][0] -= lst.g[0][0];
      }
      SGT::Mdy(1, 1, n, dfn[u]);
      Matrix now = Qry(topfa[u]);
      if (fa) {
        g[fa].g[0][0] += max(now.g[0][0], now.g[1][0]);
        g[fa].g[1][0] += now.g[0][0];
      }
      g[fa].g[0][1] = g[fa].g[0][0];
      u = fa;
    }
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n), read(m);
  for (int i = 1; i <= n; ++i) read(A[i]);
  for (int u, v, i = 1; i < n; ++i)
    read(u), read(v), Graph::AddEdge(u, v), Graph::AddEdge(v, u);

  HLD::solve(), SGT::build(1, 1, n);

  while (m--) {
    static int u, v;
    read(u), read(v), HLD::Mdy(u, v);
    Matrix res = HLD::Qry(1);
    printf("%d\n", max(res.g[0][0], res.g[1][0]));
  }
  return 0;
}

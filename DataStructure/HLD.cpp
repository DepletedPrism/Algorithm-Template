// LOJ #139
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

typedef long long LL;
const int MAXN = 1e5+5;

int n, m, rt;
int A[MAXN];

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXN << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head); eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }
}

namespace HLD {
  using namespace Graph;
  int depth[MAXN], pre[MAXN], son[MAXN], size[MAXN];
  int dfn[MAXN], rnk[MAXN], topfa[MAXN], dfs_clock;

  void dfs1(int u, int fa) {
    depth[u] = depth[fa] + 1;
    son[u] = -1, pre[u] = fa, size[u] = 1;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if ((v = edges[i].to) == fa) continue;
      dfs1(v, u), size[u] += size[v];
      if (son[u] == -1 || size[son[u]] < size[v]) son[u] = v;
    }
  }

  void dfs2(int u, int top) {
    topfa[u] = top, rnk[dfn[u] = ++dfs_clock] = u;
    if (~son[u]) dfs2(son[u], top);
    for (int v, i = head[u]; ~i; i = edges[i].nxt)
      if ((v = edges[i].to) != pre[u] && v != son[u]) dfs2(v, v);
  }

  inline void solve() { rt = 1, dfs1(rt, 0), dfs2(rt, rt); }
}

#define lc (nd << 1)
#define rc (nd << 1 | 1)
namespace SGT {
  LL datSum[MAXN << 2], tagAdd[MAXN << 2];

  inline void maintain(int nd) { datSum[nd] = datSum[lc] + datSum[rc]; }

  inline void pushdown(int nd, int L, int R) {
    if (tagAdd[nd]) {
      int Mid = (L + R) / 2;
      datSum[lc] += tagAdd[nd] * (Mid-L+1), datSum[rc] += tagAdd[nd] * (R-Mid);
      tagAdd[lc] += tagAdd[nd], tagAdd[rc] += tagAdd[nd];
      tagAdd[nd] = 0;
    }
  }

  void build(int nd, int L, int R) {
    if (L == R)
      return void( datSum[nd] = A[HLD::rnk[L]] );
    int Mid = (L + R) / 2;
    build(lc, L, Mid), build(rc, Mid+1, R);
    maintain(nd);
  }

  void Mdy(int nd, int L, int R, int opL, int opR, const LL& val) {
    if (opL <= L && R <= opR) {
      datSum[nd] += val * (R-L+1), tagAdd[nd] += val; return;
    }
    pushdown(nd, L, R);
    int Mid = (L + R) / 2;
    if (opL <= Mid) Mdy(lc, L, Mid, opL, opR, val);
    if (opR > Mid) Mdy(rc, Mid+1, R, opL, opR, val);
    maintain(nd);
  }

  LL Qry(int nd, int L, int R, const int& opL, const int& opR) {
    if (opL <= L && R <= opR) return datSum[nd];
    LL ret = 0;
    pushdown(nd, L, R);
    int Mid = (L + R) / 2;
    if (opL <= Mid) ret += Qry(lc, L, Mid, opL, opR);
    if (opR > Mid) ret += Qry(rc, Mid+1, R, opL, opR);
    return ret;
  }
}
#undef lc
#undef rc

namespace HLD {
  inline int FndSon(int u, int v) {
    while (topfa[u] != topfa[v]) {
      u = topfa[u];
      if (pre[u] == v) return u;
      u = pre[u];
    }
    return son[v];
  }

  void MdyChn(int u, int v, const LL& val) {
    while (topfa[u] != topfa[v]) {
      if (depth[topfa[u]] < depth[topfa[v]]) swap(u, v);
      SGT::Mdy(1, 1, n, dfn[topfa[u]], dfn[u], val);
      u = pre[topfa[u]];
    }
    if (dfn[u] > dfn[v]) swap(u, v);
    SGT::Mdy(1, 1, n, dfn[u], dfn[v], val);
  }
  void MdySub(int u, const LL& val) {
    if (u == rt) return SGT::Mdy(1, 1, n, 1, n, val);
    if (dfn[u] <= dfn[rt] && dfn[rt] <= dfn[u] + size[u] - 1) {
      int t = FndSon(rt, u);
      SGT::Mdy(1, 1, n, 1, n, val);
      return SGT::Mdy(1, 1, n, dfn[t], dfn[t] + size[t] - 1, -val);
    }
    return SGT::Mdy(1, 1, n, dfn[u], dfn[u] + size[u] - 1, val);
  }

  LL QryChn(int u, int v) {
    LL ret = 0;
    while (topfa[u] != topfa[v]) {
      if (depth[topfa[u]] < depth[topfa[v]]) swap(u, v);
      ret += SGT::Qry(1, 1, n, dfn[topfa[u]], dfn[u]);
      u = pre[topfa[u]];
    }
    if (dfn[u] > dfn[v]) swap(u, v);
    return ret + SGT::Qry(1, 1, n, dfn[u], dfn[v]);
  }
  LL QrySub(int u) {
    if (u == rt) return SGT::Qry(1, 1, n, 1, n);
    if (dfn[u] <= dfn[rt] && dfn[rt] <= dfn[u]+size[u]-1) {
      int t = FndSon(rt, u);
      return SGT::Qry(1, 1, n, 1, n)
        - SGT::Qry(1, 1, n, dfn[t], dfn[t] + size[t] - 1);
    }
    return SGT::Qry(1, 1, n, dfn[u], dfn[u] + size[u] - 1);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n);
  for (int i = 1; i <= n; ++i) read(A[i]);
  for (int i = 2; i <= n; ++i) {
    int fa; read(fa);
    Graph::AddEdge(i, fa), Graph::AddEdge(fa, i);
  }

  HLD::solve(), SGT::build(1, 1, n);

  read(m);
  while (m--) {
    int opt, u, v; LL K;
    read(opt), read(u);
    switch (opt) {
      case 1: rt = u; break;
      case 2: read(v), read(K), HLD::MdyChn(u, v, K); break;
      case 3: read(K), HLD::MdySub(u, K); break;
      case 4: read(v), printf("%lld\n", HLD::QryChn(u, v)); break;
      case 5: printf("%lld\n", HLD::QrySub(u)); break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

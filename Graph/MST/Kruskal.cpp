// LOJ #123
// DeP
#include <cctype>
#include <cstdio>
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
const int MAXN = 2e5 + 5, MAXM = 5e5 + 5;

struct Edge {
  int u, v, w;
  bool operator < (const Edge& rhs) const {
    return w < rhs.w;
  }
} E[MAXM];

int n, m;

namespace DSU {
  int fa[MAXN], size[MAXN];

  inline void init() {
    for (int i = 1; i <= n; ++i) fa[i] = i, size[i] = 1;
  }

  int Fnd(int u) {
    return (fa[u] == u)? u: fa[u] = Fnd(fa[u]);
  }
  inline void Jon(int fu, int fv) {
    if (size[fu] < size[fv]) swap(fu, fv);
    fa[fv] = fu, size[fu] += size[fv];
  }
}

namespace MST {
  LL Kruskal() {
    LL ret = 0;
    DSU::init(), sort(E + 1, E + m + 1);
    for (int cnt = 0, i = 1; i <= m; ++i) {
      int fu = DSU::Fnd(E[i].u), fv = DSU::Fnd(E[i].v);
      if (fu != fv) {
        ret += E[i].w, DSU::Jon(fu, fv);
        if (++cnt == n - 1) return ret;
      }
    }
    return 0;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(m);
  for (int u, v, w, i = 1; i <= m; ++i) {
    read(u), read(v), read(w);
    E[i] = (Edge){ u, v, w };
  }

  printf("%lld\n", MST::Kruskal());
  return 0;
}

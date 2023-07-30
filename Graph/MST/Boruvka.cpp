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

template<typename T> inline void ckmin(T& a, const T& b) { if (a > b) a = b; }

typedef long long LL;
typedef pair<int, int> Pii;
const int MAXN = 2e5 + 5, MAXM = 5e5 + 5, INF = 0x3f3f3f3f;

struct Edge {
  int u, v, w;
} E[MAXM];

int n, m;

namespace MST {
  Pii V[MAXN];
  int fa[MAXN], size[MAXN];

  int findfa(int u) {
    return fa[u] == u? u: fa[u] = findfa(fa[u]);
  }

  LL Boruvka() {
    LL ret = 0; int cnt = 0;
    for (int i = 1; i <= n; ++i) fa[i] = i, size[i] = 1;
    while (cnt < n - 1) {
      int idx = 0;
      for (int u = 1; u <= n; ++u)
        V[findfa(u)] = Pii(INF, MAXM);
      for (int i = 1; i <= m; ++i) {
        int fu = findfa(E[i].u), fv = findfa(E[i].v);
        if (fu == fv) continue;
        Pii e = Pii(E[i].w, i);
        ++idx, ckmin(V[fu], e), ckmin(V[fv], e);
      }
      if (idx == 0) break;
      for (int i = 1; i <= m; ++i) {
        int fu = findfa(E[i].u), fv = findfa(E[i].v);
        if (fu == fv) continue;
        Pii e = Pii(E[i].w, i);
        if (V[fu] == e || V[fv] == e) {
          if (size[fu] < size[fv]) swap(fu, fv);
          ret += E[i].w, ++cnt;
          fa[fv] = fu, size[fu] += size[fv];
        }
      }
    }
    return ret;
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

  printf("%lld\n", MST::Boruvka());
  return 0;
}

// Luogu P3367
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

const int MAXN = 1e4 + 5, MAXM = 2e5 + 5;

int n, q;

namespace DSU {
  int fa[MAXN], size[MAXN];

  inline void init() {
    for (int i = 1; i <= n; ++i) fa[i] = i, size[i] = 1;
  }

  int Fnd(int u) {
    return (fa[u] == u)? u: fa[u] = Fnd(fa[u]);
  }
  inline void join(int fu, int fv) {
    if (fu == fv) return;
    if (size[fu] < size[fv]) swap(fu, fv);
    fa[fv] = fu, size[fu] += size[fv];
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(q);

  DSU::init();
  for (int opt, u, v; q; --q) {
    read(opt), read(u), read(v);
    int fu = DSU::Fnd(u), fv = DSU::Fnd(v);
    switch (opt) {
      case 1: DSU::join(fu, fv); break;
      case 2: puts((fu == fv)? "Y": "N"); break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

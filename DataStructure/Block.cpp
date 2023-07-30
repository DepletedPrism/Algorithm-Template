// Luogu P2801
// DeP
#include <cmath>
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
using IO::read; using IO::Gc;

typedef long long LL;
const int MAXN = 1000005;

int n, q;
LL A[MAXN];

namespace BLK {
  int BL[MAXN], BR[MAXN];
  int belong[MAXN], Block, BCnt;
  LL tagAdd[MAXN], dat[MAXN], D[MAXN];

  void init() {
    Block = (int) sqrt(n);
    BCnt = n / Block + (n % Block != 0);
    for (int i = 1; i <= n; ++i)
      belong[i] = (i-1) / Block + 1, D[i] = A[i];
    for (int i = 1; i <= BCnt; ++i)
      BL[i] = (i-1) * Block + 1, BR[i] = i * Block;
    BR[BCnt] = n;
    for (int i = 1; i <= BCnt; ++i)
      sort(D + BL[i], D + BR[i] + 1);
  }

  void modify(int L, int R, int x) {
    if (belong[L] == belong[R]) {
      for (int i = L; i <= R; ++i) A[i] += x;
      for (int i = BL[belong[L]]; i <= BR[belong[R]]; ++i)
        D[i] = A[i];
      sort(D + BL[belong[L]], D + BR[belong[R]] + 1);
      return;
    }
    for (int i = L; i <= BR[belong[L]]; ++i) A[i] += x;
    for (int i = BL[belong[L]]; i <= BR[belong[L]]; ++i)
      D[i] = A[i];
    sort(D + BL[belong[L]], D + BR[belong[L]] + 1);
    for (int i = BL[belong[R]]; i <= R; ++i) A[i] += x;
    for (int i = BL[belong[R]]; i <= BR[belong[R]]; ++i)
      D[i] = A[i];
    sort(D + BL[belong[R]], D + BR[belong[R]] + 1);
    for (int i = belong[L]+1; i < belong[R]; ++i)
      tagAdd[i] += x;
  }

  int query(int L, int R, int x) {
    int ret = 0;
    if (belong[L] == belong[R]) {
      for (int i = L; i <= R; ++i)
        if (A[i] + tagAdd[belong[L]] >= x) ++ret;
      return ret;
    }
    for (int i = L; i <= BR[belong[L]]; ++i)
      if (A[i] + tagAdd[belong[L]] >= x) ++ret;
    for (int i = BL[belong[R]]; i <= R; ++i)
      if (A[i] + tagAdd[belong[R]] >= x) ++ret;
    for (int i = belong[L]+1; i < belong[R]; ++i) {
      int l = BL[i], r = BR[i], ans = 0;
      while (l <= r) {
        int Mid = (l + r) / 2;
        if (D[Mid] + tagAdd[i] >= x) ans = BR[i] - Mid + 1, r = Mid - 1;
        else l = Mid + 1;
      }
      ret += ans;
    }
    return ret;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(q);
  for (int i = 1; i <= n; ++i) read(A[i]);

  BLK::init();

  while (q--) {
    int L, R, x, opt = Gc();
    while (isspace(opt)) opt = Gc();
    read(L); read(R); read(x);
    switch (opt) {
      case 'M': BLK::modify(L, R, x); break;
      case 'A': printf("%d\n", BLK::query(L, R, x)); break;
      default: fprintf(stderr, "NMSL\n");
    }
  }
  return 0;
}

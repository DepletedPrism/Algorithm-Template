// Luogu P3812
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
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
const int MAXL = 52;

int n, m;

struct LinearBasis {
  LL p[MAXL], g[MAXL], ng;

  LinearBasis() { memset(p, 0, sizeof p); }

  inline void Add(LL x) {
    for (int i = MAXL - 1; i >= 0; --i) if ((x >> i) & 1) {
      if (p[i]) x ^= p[i];
      else {
        for (int j = i - 1; j >= 0; --j)
          if ((x >> j) & 1) x ^= p[j];
        for (int j = i + 1; j < MAXL; ++j)
          if ((p[j] >> i) & 1) p[j] ^= x;
        p[i] = x;
        break;
      }
    }
  }

  inline void build() {
    for (int i = 0; i < MAXL; ++i)
      if (p[i]) g[ng++] = p[i];
  }

  LL Kth(LL k) {
    k -= (ng < n);
    if (k >= (1LL << ng)) return -1;
    LL ret = 0;
    for (int i = 0; i < ng; ++i)
      if ((k >> i) & 1) ret ^= g[i];
    return ret;
  }
} B;

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n);
  for (LL x; n; --n)
    read(x), B.Add(x);

  B.build(), read(m);
  for (LL k; m; --m)
    read(k), printf("%lld\n", B.Kth(k));
  return 0;
}

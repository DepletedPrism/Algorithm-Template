// Luogu P4245
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
const int MAXN = 3e5+5, invP1 = 669690699, invP12 = 354521948;
const int P1 = 998244353, P2 = 1004535809, P3 = 469762049, G = 3;
const LL P12 = 1LL * P1 * P2;

int mod;

struct Num {
  int a, b, c;
  Num() { a = b = c = 0; }
  Num(int _x): a(_x), b(_x), c(_x) { }
  Num(int _a, int _b, int _c): a(_a), b(_b), c(_c) { }
  Num Mod(const Num& x) const {
    return Num(x.a + (x.a >> 31 & P1), x.b + (x.b >> 31 & P2), x.c + (x.c >> 31 & P3));
  }
  Num operator + (const Num& rhs) const { return Mod(Num(a + rhs.a - P1, b + rhs.b - P2, c + rhs.c - P3)); }
  Num operator - (const Num& rhs) const { return Mod(Num(a - rhs.a, b - rhs.b, c - rhs.c)); }
  Num operator * (const Num& rhs) const {
    return Num(1LL * a * rhs.a % P1, 1LL * b * rhs.b % P2, 1LL * c * rhs.c % P3);
  }
  int Merge() {
    LL x = 1LL * (b - a + P2) % P2 * invP1 % P2 * P1 + a;
    return (1LL * (c - x % P3 + P3) % P3 * invP12 % P3 * (P12 % mod) % mod + x) % mod;
  }
};

int fpow(int base, int b, int P) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = 1LL * base * ret % P;
    base = 1LL * base * base % P, b >>= 1;
  }
  return ret;
}

namespace Poly {
  int R[MAXN];
  inline void init(const int& Lim, const int& L) {
    for (int i = 1; i < Lim; ++i) R[i] = (R[i>>1] >> 1) | ((i & 1) << (L-1));
  }

  void NTT(Num* f, const int& Lim, const int& type) {
    for (int i = 1; i < Lim; ++i) if (i < R[i]) swap(f[i], f[R[i]]);
    for (int Mid = 1; Mid < Lim; Mid <<= 1) {
      Num unit( fpow(type > 0? G: fpow(G, P1-2, P1), (P1-1) / (Mid << 1), P1), 
          fpow(type > 0? G: fpow(G, P2-2, P2), (P2-1) / (Mid << 1), P2),
          fpow(type > 0? G: fpow(G, P3-2, P3), (P3-1) / (Mid << 1), P3) );
      for (int i = 0; i < Lim; i += Mid << 1) {
        Num w(1);
        for (int j = 0; j < Mid; ++j, w = w * unit) {
          Num f0 = f[i+j], f1 = w * f[i+j+Mid];
          f[i+j] = f0 + f1, f[i+j+Mid] = f0 - f1;
        }
      }
    }
    if (type < 0) {
      Num inv( fpow(Lim, P1-2, P1), fpow(Lim, P2-2, P2), fpow(Lim, P3-2, P3) );
      for (int i = 0; i < Lim; ++i) f[i] = f[i] * inv;
    }
  }
}

int n, m;
Num f[MAXN], g[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(m), read(mod); ++n, ++m;
  for (int x, i = 0; i < n; ++i) read(x), f[i] = Num(x);
  for (int x, i = 0; i < m; ++i) read(x), g[i] = Num(x);
  int Lim = 1, L = 0;
  while (Lim < n+m) Lim <<= 1, ++L;
  Poly::init(Lim, L);
  Poly::NTT(f, Lim, 1), Poly::NTT(g, Lim, 1);
  for (int i = 0; i < Lim; ++i) f[i] = f[i] * g[i];
  Poly::NTT(f, Lim, -1);
  for (int i = 0; i < n+m-1; ++i) printf("%d%c", f[i].Merge(), " \n"[i==n+m-2]);
  return 0;
}

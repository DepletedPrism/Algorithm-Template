// Luogu P5050
// DeP
#include <cctype>
#include <cstdio>
#include <vector>
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

const int LOG = 18, MAXN = 1 << LOG | 1, P = 998244353, G = 3;

int W[LOG][MAXN];

int fpow(int base, int b) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = 1LL * ret * base % P;
    base = 1LL * base * base % P, b >>= 1;
  }
  return ret;
}

namespace Poly {
  int r[MAXN];
  inline void init(const int& Lim) {
    for (int i = 1; i < Lim; ++i) r[i] = (r[i>>1] >> 1) | ((i & 1) * Lim >> 1);
  }

  void NTT(int* f, const int& Lim, const int& type) {
    for (int i = 1; i < Lim; ++i) if (i < r[i]) swap(f[i], f[r[i]]);
    for (int k = 0, Mid = 1; Mid < Lim; Mid <<= 1, ++k) {
      const int* w = W[k];
      for (int i = 0; i < Lim; i += Mid << 1)
        for (int j = 0; j < Mid; ++j) {
          int f0 = f[i+j], f1 = 1LL * w[j] * f[i+j+Mid] % P;
          f[i+j] = (f0 + f1) % P, f[i+j+Mid] = (f0 - f1 + P) % P;
        }
    }
    if (type < 0) {
      int iv = fpow(Lim, P - 2);
      for (int i = 0; i < Lim; ++i) f[i] = 1LL * f[i] * iv % P;
      reverse(f + 1, f + Lim);
    }
  }

  void Inv(int* f, int *g, const int& n) {
    static int A[MAXN], B[MAXN];
    g[0] = fpow(f[0], P - 2);
    for (int Mid = 2, Lim = Mid << 1; Mid < 2 * n; Mid <<= 1, Lim <<= 1) {
      for (int i = 0; i < Lim; ++i)
        A[i] = (i < Mid)? f[i]: 0, B[i] = (i < Mid)? g[i]: 0;
      init(Lim), NTT(A, Lim, 1), NTT(B, Lim, 1);
      for (int i = 0; i < Lim; ++i)
        g[i] = ((B[i] + B[i]) % P - 1LL * A[i] * B[i] % P * B[i] % P + P) % P;
      NTT(g, Lim, -1);
      for (int i = min(n, Mid); i < Lim; ++i) g[i] = 0;
    }
  }

  typedef vector<int> poly;

  poly Mul(const poly& f, const poly& g) {
    static int A[MAXN], B[MAXN];
    int Lim = 1, n = f.size(), m = g.size();
    poly h(n + m - 1);
    if (n < 200) {
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
          h[i + j] = (h[i + j] + 1LL * f[i] * g[j]) % P;
      return h;
    }
    while (Lim < n + m - 1) Lim <<= 1;
    for (int i = 0; i < Lim; ++i)
      A[i] = (i < n)? f[i]: 0, B[i] = (i < m)? g[i]: 0;
    init(Lim), NTT(A, Lim, 1), NTT(B, Lim, 1);
    for (int i = 0; i < Lim; ++i) A[i] = 1LL * A[i] * B[i] % P;
    NTT(A, Lim, -1);
    for (int i = 0; i < n + m - 1; ++i) h[i] = A[i];
    return h;
  }

  poly MulT(const poly& f, const poly& g) {
    static int A[MAXN], B[MAXN];
    int Lim = 1, n = f.size(), m = g.size();
    poly h(n - m + 1);
    if (n < 200) {
      for (int i = n - 1; i >= 0; --i)
        for (int j = max(0, i - n + m); j <= min(i, m - 1); ++j)
          h[i - j] = (h[i - j] + 1LL * f[i] * g[j]) % P;
      return h;
    }
    while (Lim < n + m - 1) Lim <<= 1;
    for (int i = 0; i < Lim; ++i)
      A[i] = (i < n)? f[i]: 0, B[i] = (i < m)? g[m - i - 1]: 0;
    init(Lim), NTT(A, Lim, 1), NTT(B, Lim, 1);
    for (int i = 0; i < Lim; ++i) A[i] = 1LL * A[i] * B[i] % P;
    NTT(A, Lim, -1);
    for (int i = m - 1; i < n; ++i) h[i - m + 1] = A[i];
    return h;
  }

  vector<int> T[MAXN << 1], F[LOG << 1];

  void EvalPre(int nd, int L, int R, const int* a) {
    if (L == R)
      return void( T[nd] = { 1, P - a[L] } );
    int Mid = (L + R) / 2;
    EvalPre(nd << 1, L, Mid, a), EvalPre(nd << 1 | 1, Mid + 1, R, a);
    T[nd] = Mul(T[nd << 1], T[nd << 1 | 1]);
  }
  void EvalDiv(int nd, int p, int L, int R, int* g) {
    if (L == R)
      return void( g[L] = F[p][0] );
    int Mid = (L + R) / 2;
    F[p + 1] = MulT(F[p], T[nd << 1 | 1]);
    EvalDiv(nd << 1, p + 1, L, Mid, g);
    F[p + 1] = MulT(F[p], T[nd << 1]);
    EvalDiv(nd << 1 | 1, p + 1, Mid + 1, R, g);
  }

  // f(a) = g
  void Eval(int* f, int n, int* a, int m, int* g) {
    static int t[MAXN], ivt[MAXN];
    n = max(n, m), EvalPre(1, 0, n - 1, a);
    for (int i = 0; i < n; ++i) t[i] = T[1][i];
    Inv(t, ivt, n), reverse(ivt, ivt + n);
    poly h = Mul(poly(ivt, ivt + n), poly(f, f + n));
    F[0].resize(n);
    for (int i = 0; i < n; ++i) F[0][i] = h[i + n - 1];
    EvalDiv(1, 0, 0, n - 1, g);
  }
}

void PolyPre() {
  for (int w, k = 0, Mid = 1; k < LOG; ++k, Mid <<= 1) {
    W[k][0] = 1, w = fpow(G, (P - 1) / (Mid << 1));
    for (int i = 1; i < Mid; ++i)
      W[k][i] = 1LL * w * W[k][i - 1] % P;
  }
}

int n, m;
int a[MAXN], f[MAXN], g[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(m), ++n;
  for (int i = 0; i < n; ++i) read(f[i]);
  for (int i = 0; i < m; ++i) read(a[i]);

  PolyPre(), Poly::Eval(f, n, a, m, g);

  for (int i = 0; i < m; ++i) printf("%d\n", g[i]);
  return 0;
}

// Luogu P5050
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

const int LOG = 18, MAXN = 1 << LOG | 1, MAXM = MAXN << 3;
const int P = 998244353, G = 3;

int fpow(int base, int b) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = 1LL * ret * base % P;
    base = 1LL * base * base % P, b >>= 1;
  }
  return ret;
}

int W[LOG][MAXN];

namespace Poly {
  int r[MAXN];
  inline void init(const int& Lim, const int& L) {
    for (int i = 1; i < Lim; ++i) r[i] = (r[i>>1] >> 1) | ((i & 1) << (L-1));
  }

  void NTT(int* f, const int& Lim, const int& type) {
    for (int i = 1; i < Lim; ++i) if (i < r[i]) swap(f[i], f[r[i]]);
    for (int k = 0; (1 << k) < Lim; ++k) {
      const int *w = W[k], Mid = 1 << k;
      for (int i = 0; i < Lim; i += Mid << 1)
        for (int j = 0; j < Mid; ++j) {
          int f0 = f[i+j], f1 = 1LL * w[j] * f[i+j+Mid] % P;
          f[i+j] = (f0 + f1) % P, f[i+j+Mid] = (f0 - f1 + P) % P;
        }
    }
    if (type < 0) {
      int inv = fpow(Lim, P - 2);
      for (int i = 0; i < Lim; ++i) f[i] = 1LL * f[i] * inv % P;
      reverse(f + 1, f + Lim);
    }
  }

  void Inv(int* f, int* g, const int& n) {
    static int A[MAXN], B[MAXN];
    g[0] = fpow(f[0], P-2);
    for (int L = 0, Lim = 1, Mid = 2; Mid < 2*n; Mid <<= 1) {
      while (Lim < 2*Mid) Lim <<= 1, ++L;
      for (int i = 0; i < Mid; ++i) A[i] = f[i], B[i] = g[i];
      for (int i = Mid; i < Lim; ++i) A[i] = B[i] = 0;
      init(Lim, L), NTT(A, Lim, 1), NTT(B, Lim, 1);
      for (int i = 0; i < Lim; ++i)
        g[i] = ((B[i] + B[i]) % P - 1LL * A[i] * B[i] % P * B[i] % P + P) % P;
      NTT(g, Lim, -1);
      for (int i = min(n, Mid); i < Lim; ++i) g[i] = 0;
    }
  }

  // f = Q * g + R, f = R (mod g)
  void Mod(int* f, const int& n, int* g, const int& m, int* R) {
    static int A[MAXN], B[MAXN], ig[MAXN], Q[MAXN];
    reverse(f, f + n), reverse(g, g + m);
    Inv(g, ig, n-m+1);
    int Lim = 1, L = 0;
    while (Lim < n + n-m+1) Lim <<= 1, ++L;
    for (int i = 0; i < Lim; ++i)
      A[i] = (i < n)? f[i]: 0, B[i] = (i < n-m+1)? ig[i]: 0;
    init(Lim, L), NTT(A, Lim, 1), NTT(B, Lim, 1);
    for (int i = 0; i < Lim; ++i) Q[i] = 1LL * A[i] * B[i] % P;
    NTT(Q, Lim, -1);
    reverse(Q, Q + n-m+1), reverse(f, f + n), reverse(g, g + m);
    Lim = 1, L = 0;
    while (Lim < m + n-m+1) Lim <<= 1, ++L;
    for (int i = 0; i < Lim; ++i)
      A[i] = (i < n-m+1)? Q[i]: 0, B[i] = (i < m)? g[i]: 0;
    init(Lim, L), NTT(A, Lim, 1), NTT(B, Lim, 1);
    for (int i = 0; i < Lim; ++i) A[i] = 1LL * A[i] * B[i] % P;
    NTT(A, Lim, -1);
    for (int i = 0; i < m-1; ++i) R[i] = (f[i] - A[i] + P) % P;
  }

  int dg[MAXN];
  int *eg[MAXN], bkt[MAXM << 1], *ptr = bkt;

  void EvalPre(int idx, int L, int R, const int* a) {
    static int A[MAXN], B[MAXN];
    if (R-L < 2) {
      dg[idx] = 2, eg[idx] = ptr, ptr += dg[idx];
      return eg[idx][0] = P - a[L], void( eg[idx][1] = 1 );
    }
    int Mid = (L + R) / 2, lc = idx << 1, rc = idx << 1 | 1;
    EvalPre(lc, L, Mid, a), EvalPre(rc, Mid, R, a);
    dg[idx] = dg[lc] + dg[rc] - 1, eg[idx] = ptr, ptr += dg[idx];
    int Lim = 1, k = 0;
    while (Lim < dg[idx]) Lim <<= 1, ++k;
    for (int i = 0; i < Lim; ++i)
      A[i] = (i < dg[lc])? eg[lc][i]: 0, B[i] = (i < dg[rc])? eg[rc][i]: 0;
    init(Lim, k), NTT(A, Lim, 1), NTT(B, Lim, 1);
    for (int i = 0; i < Lim; ++i) A[i] = 1LL * A[i] * B[i] % P;
    NTT(A, Lim, -1);
    for (int i = 0; i < dg[idx]; ++i) eg[idx][i] = A[i];
  }

  void EvalDiv(int idx, int L, int R, int* A, int* f) {
    if (R-L < 2) return void( f[L] = *A );
    int *B = ptr, Mid = (L + R) / 2, lc = idx << 1, rc = idx << 1 | 1;
    ptr += max(dg[lc], dg[rc]);
    Mod(A, dg[idx] - 1, eg[lc], dg[lc], B);
    EvalDiv(lc, L, Mid, B, f);
    Mod(A, dg[idx] - 1, eg[rc], dg[rc], B);
    EvalDiv(rc, Mid, R, B, f);
  }

  // f(a) = g
  void Eval(int* f, const int& n, int* a, const int& m, int* g) {
    EvalPre(1, 0, m, a);
    if (n >= m) Mod(f, n, eg[1], dg[1], f);
    EvalDiv(1, 0, m, f, g);
  }
}

void PolyPre() {
  for (int w, i = 0, Mid = 1; i < LOG; ++i, Mid <<= 1) {
    W[i][0] = 1, w = fpow(G, (P - 1) / (Mid << 1));
    for (int j = 1; j < Mid; ++j)
      W[i][j] = 1LL * w * W[i][j-1] % P;
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

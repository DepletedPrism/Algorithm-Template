// LOJ #150
// DeP
#include <ctime>
#include <cctype>
#include <cstdio>
#include <cstdlib>
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
const int LOG = 18, MAXN = 1 << LOG | 1;
const int P = 998244353, G = 3, iv2 = 499122177;

int W[LOG][MAXN], inv[MAXN];

int fpow(int pw, int b) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = (LL) ret * pw % P;
    pw = (LL) pw * pw % P, b >>= 1;
  }
  return ret;
}

namespace Cipolla {
  struct Complex {
    int x, y;
    Complex(int _x, int _y): x(_x), y(_y) { }
  };

  inline Complex Mul(Complex A, Complex B, const int& i2) {
    return Complex(((LL) A.x * B.x + (LL) i2 * A.y % P * B.y % P) % P,
        ((LL) A.x * B.y + (LL) A.y * B.x) % P);
  }
  Complex Cfpow(Complex pw, int b, const int& i2) {
    Complex ret(1, 0);
    while (b > 0) {
      if (b & 1) ret = Mul(ret, pw, i2);
      pw = Mul(pw, pw, i2), b >>= 1;
    }
    return ret;
  }

  inline int Sqrt(int n) {
    srand(time(nullptr));
    while (true) {
      int a = rand() % P, i2 = ((LL) a * a % P - n + P) % P;
      if (!a || fpow(i2, (P - 1) / 2) == 1) continue;
      int x = Cfpow(Complex(a, 1), (P + 1) / 2, i2).x;
      return min(x, P -x);
    }
    return -1;
  }
}

namespace Poly {
  int r[MAXN];
  inline void init(const int& Lim) {
    for (int i = 1; i < Lim; ++i) r[i] = (r[i>>1] >> 1) | ((i & 1) * Lim >> 1);
  }

  void NTT(int* f, const int& Lim, const int& type) {
    for (int i = 1; i < Lim; ++i) if (i < r[i]) swap(f[i], f[r[i]]);
    for (int k = 0, Mid = 1; Mid < Lim; ++k, Mid <<= 1) {
      const int* w = W[k];
      for (int i = 0; i < Lim; i += Mid << 1)
        for (int j = 0; j < Mid; ++j) {
          int f0 = f[i+j], f1 = (LL) w[j] * f[i+j+Mid] % P;
          f[i+j] = (f0 + f1) % P, f[i+j+Mid] = (f0 - f1 + P) % P;
        }
    }
    if (type < 0) {
      int iv = fpow(Lim, P - 2);
      for (int i = 0; i < Lim; ++i) f[i] = (LL) f[i] * iv % P;
      reverse(f + 1, f + Lim);
    }
  }

  void Inv(int* f, int* g, const int& n) {
    static int A[MAXN], B[MAXN];
    g[0] = fpow(f[0], P - 2);
    for (int Mid = 2, Lim = 4; Mid < 2 * n; Mid <<= 1, Lim <<= 1) {
      for (int i = 0; i < Lim; ++i)
        A[i] = (i < Mid)? f[i]: 0, B[i] = (i < Mid)? g[i]: 0;
      init(Lim), NTT(A, Lim, 1), NTT(B, Lim, 1);
      for (int i = 0; i < Lim; ++i)
        g[i] = ((B[i] + B[i]) % P - (LL) A[i] * B[i] % P * B[i] % P + P) % P;
      NTT(g, Lim, -1);
      for (int i = min(n, Mid); i < Lim; ++i) g[i] = 0;
    }
  }

  void Sqrt(int* f, int* g, const int& n) {
    static int A[MAXN], B[MAXN], ivg[MAXN];
    g[0] = Cipolla::Sqrt(f[0]);
    for (int Mid = 2, Lim = 4; Mid < 2 * n; Mid <<= 1, Lim <<= 1) {
      Inv(g, ivg, Mid);
      for (int i = 0; i < Lim; ++i)
        A[i] = (i < Mid)? f[i]: 0, B[i] = (i < Mid)? ivg[i]: 0;
      init(Lim), NTT(A, Lim, 1), NTT(B, Lim, 1);
      for (int i = 0; i < Lim; ++i) A[i] = (LL) A[i] * B[i] % P;
      NTT(A, Lim, -1);
      for (int i = 0; i < min(n, Mid); ++i)
        g[i] = (LL) iv2 * (g[i] + A[i]) % P;
      for (int i = min(n, Mid); i < Lim; ++i) g[i] = 0;
    }
  }

  inline void Der(int* f, int* g, const int& n) {
    for (int i = 1; i < n; ++i) g[i - 1] = (LL) f[i] * i % P;
    g[n - 1] = 0;
  }
  inline void Int(int* f, int* g, const int& n) {
    for (int i = n - 1; i; --i) g[i] = (LL) f[i - 1] * inv[i] % P;
    g[0] = 0;
  }

  void Ln(int* f, int* g, const int& n) {
    static int df[MAXN], ivf[MAXN];
    Der(f, df, n), Inv(f, ivf, n);
    int Lim = 1;
    while (Lim < n + n - 1) Lim <<= 1;
    for (int i = n; i < Lim; ++i) df[i] = ivf[i] = 0;
    init(Lim), NTT(df, Lim, 1), NTT(ivf, Lim, 1);
    for (int i = 0; i < Lim; ++i) df[i] = (LL) df[i] * ivf[i] % P;
    NTT(df, Lim, -1), Int(df, g, n);
  }

  void Exp(int* f, int* g, const int& n) {
    static int A[MAXN], B[MAXN], lng[MAXN];
    g[0] = 1;
    for (int Mid = 2, Lim = 4; Mid < 2 * n; Mid <<= 1, Lim <<= 1) {
      Ln(g, lng, Mid);
      for (int i = 0; i < Lim; ++i)
        A[i] = (i < Mid)? (f[i] - lng[i] + P) % P: 0, B[i] = (i < Mid)? g[i]: 0;
      A[0] = (A[0] + 1) % P;
      init(Lim), NTT(A, Lim, 1), NTT(B, Lim, 1);
      for (int i = 0; i < Lim; ++i) g[i] = (LL) A[i] * B[i] % P;
      NTT(g, Lim, -1);
      for (int i = min(n, Mid); i < Lim; ++i) g[i] = 0;
    }
  }

  void Pow(int* f, int* g, const int& n, const int& k) {
    static int lnf[MAXN];
    Ln(f, lnf, n);
    for (int i = 0; i < n; ++i) lnf[i] = (LL) k * lnf[i] % P;
    Exp(lnf, g, n);
  }
}

void PolyPre(int N) {
  inv[1] = 1;
  for (int i = 2; i <= N; ++i)
    inv[i] = (LL) inv[P % i] * (P - P / i) % P;
  for (int w, k = 0, Mid = 1; Mid <= 2 * N; ++k, Mid <<= 1) {
    W[k][0] = 1, w = fpow(G, (P - 1) / (Mid << 1));
    for (int i = 1; i < Mid; ++i)
      W[k][i] = (LL) w * W[k][i - 1] % P;
  }
}

int n, k;
int f[MAXN], g[MAXN], h[MAXN];

void Solve() {
  using namespace Poly;
  Sqrt(f, h, n), Inv(h, g, n), Int(g, h, n), Exp(h, g, n);
  f[0] = 2;
  for (int i = 0; i < n; ++i) f[i] = (f[i] - g[i] + P) % P;
  Ln(f, g, n);
  g[0] = (g[0] + 1) % P;
  Pow(g, f, n, k), Der(f, g, n);
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(k), ++n;
  for (int i = 0; i < n; ++i) read(f[i]);

  PolyPre(n), Solve();

  for (int i = 0; i < n - 1; ++i)
    printf("%d%c", g[i], " \n"[i == n - 2]);
  return 0;
}

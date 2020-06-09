// Luogu P4245
// DeP
#include <cmath>
#include <cctype>
#include <cstdio>
#include <algorithm>
using namespace std;

namespace IO {
  const int MAXSIZE = 1 << 18 | 1;
  char buf[MAXSIZE], *p1, *p2;

  inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
  template<typename T> inline void read(T& x) {
    x = 0; int f = 0, ch = Gc();
    while (!isdigit(ch)) f |= ch == '-', ch = Gc();
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

typedef long long LL;
const int MAXN = 1 << 18 | 1;
const double PI = acos(-1.0);

struct Complex {
  double x, y;
  Complex(double _x = 0.0, double _y = 0.0): x(_x), y(_y) { }
  Complex operator + (const Complex& rhs) const {
    return Complex(x + rhs.x, y + rhs.y);
  }
  Complex operator - (const Complex& rhs) const {
    return Complex(x - rhs.x, y - rhs.y);
  }
  Complex operator * (const Complex& rhs) const {
    return Complex(x * rhs.x - y * rhs.y, x * rhs.y + y * rhs.x);
  }
};
inline Complex conj(const Complex& p) { return Complex(p.x, -p.y); }

int Mod;

namespace Poly {
  int r[MAXN];
  Complex W[MAXN];
  inline void init(const int& Lim) {
    for (int i = 1; i < Lim; ++i) r[i] = (r[i>>1] >> 1) | ((i & 1) * Lim >> 1);
    for (int i = 0; i < Lim; ++i)
      W[i] = Complex(cos(PI / Lim * i), sin(PI / Lim * i));
  }

  void FFT(Complex* f, const int& Lim, const int& type) {
    for (int i = 1; i < Lim; ++i) if (i < r[i]) swap(f[i], f[r[i]]);
    for (int Mid = 1; Mid < Lim; Mid <<= 1)
      for (int i = 0; i < Lim; i += Mid << 1)
        for (int j = 0; j < Mid; ++j) {
          Complex f0 = f[i+j], f1 = W[1LL * j * Lim / Mid] * f[i+j+Mid];
          f[i+j] = f0 + f1, f[i+j+Mid] = f0 - f1;
        }
    if (type < 0) {
      for (int i = 0; i < Lim; ++i) f[i].x = round(f[i].x / Lim);
      reverse(f + 1, f + Lim);
    }
  }

  void MTT(int* f, int n, int *g, int m, int* h) {
    static Complex A[MAXN], B[MAXN];
    static Complex dfta[MAXN], dftb[MAXN], dftc[MAXN], dftd[MAXN];

    int Lim = 1;
    while (Lim < n + m - 1) Lim <<= 1;
    init(Lim);
    for (int i = 0; i < n; ++i) f[i] = (f[i] + Mod) % Mod;
    for (int j = 0; j < m; ++j) g[j] = (g[j] + Mod) % Mod;

    for (int i = 0; i < Lim; ++i) A[i] = Complex(f[i] & 32767, f[i] >> 15);
    for (int i = 0; i < Lim; ++i) B[i] = Complex(g[i] & 32767, g[i] >> 15);
    FFT(A, Lim, 1), FFT(B, Lim, 1);
    for (int i = 0; i < Lim; ++i) {
      int j = (Lim - i) & (Lim - 1);
      Complex da = (A[i] + conj(A[j])) * Complex(0.5, 0.0);
      Complex db = (A[i] - conj(A[j])) * Complex(0.0, -0.5);
      Complex dc = (B[i] + conj(B[j])) * Complex(0.5, 0.0);
      Complex dd = (B[i] - conj(B[j])) * Complex(0.0, -0.5);
      dfta[j] = da * dc, dftb[j] = da * dd;
      dftc[j] = db * dc, dftd[j] = db * dd;
    }
    for (int i = 0; i < Lim; ++i) A[i] = dfta[i] + dftb[i] * Complex(0.0, 1.0);
    for (int i = 0; i < Lim; ++i) B[i] = dftc[i] + dftd[i] * Complex(0.0, 1.0);
    FFT(A, Lim, 1), FFT(B, Lim, 1);

    for (int i = 0; i < Lim; ++i) {
      int da = LL(A[i].x / Lim + 0.5) % Mod, db = LL(A[i].y / Lim + 0.5) % Mod;
      int dc = LL(B[i].x / Lim + 0.5) % Mod, dd = LL(B[i].y / Lim + 0.5) % Mod;
      h[i] = (da + (LL(db + dc) << 15) + (LL(dd) << 30)) % Mod;
    }
  }
}

int n, m;
int f[MAXN], g[MAXN], h[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(m), read(Mod), ++n, ++m;
  for (int i = 0; i < n; ++i) read(f[i]);
  for (int j = 0; j < m; ++j) read(g[j]);

  Poly::MTT(f, n, g, m, h);

  for (int i = 0; i < n + m - 1; ++i)
    printf("%d%c", h[i], " \n"[i == n + m - 2]);
  return 0;
}

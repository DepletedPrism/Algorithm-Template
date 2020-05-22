// Luogu P3803
// DeP
#include <cmath>
#include <cstdio>
#include <algorithm>
using namespace std;

const int LOG = 21, MAXN = 1 << LOG | 1;
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

namespace Poly {
  int r[MAXN];
  inline void init(const int& Lim, const int& L) {
    for (int i = 1; i < Lim; ++i) r[i] = (r[i>>1] >> 1) | ((i & 1) << (L-1));
  }

  void FFT(Complex* f, const int& Lim, const int& type) {
    for (int i = 1; i < Lim; ++i) if (i < r[i]) swap(f[i], f[r[i]]);
    for (int Mid = 1; Mid < Lim; Mid <<= 1) {
      Complex unit(cos(PI / Mid), sin(PI / Mid));
      for (int i = 0; i < Lim; i += Mid << 1) {
        Complex w(1.0, 0.0);
        for (int j = 0; j < Mid; ++j, w = w * unit) {
          Complex f0 = f[i+j], f1 = w * f[i+j+Mid];
          f[i+j] = f0 + f1, f[i+j+Mid] = f0 - f1;
        }
      }
    }
    if (type < 0) {
      for(int i = 0; i < Lim; ++i) f[i].x /= Lim;
      reverse(f + 1, f + Lim);
    }
  }
}

int n, m;
Complex f[MAXN], g[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%d%d", &n, &m), ++n, ++m;
  for (int i = 0; i < n; ++i) scanf("%lf", &f[i].x);
  for (int i = 0; i < m; ++i) scanf("%lf", &g[i].x);

  int Lim = 1, L = 0;
  while (Lim < n + m - 1) Lim <<= 1, ++L;
  Poly::init(Lim, L), Poly::FFT(f, Lim, 1), Poly::FFT(g, Lim, 1);
  for (int i = 0; i < Lim; ++i) f[i] = f[i] * g[i];
  Poly::FFT(f, Lim, -1);

  for (int i = 0; i < n + m - 1; ++i)
    printf("%d%c", (int) round(f[i].x), " \n"[i == n + m - 2]);
  return 0;
}

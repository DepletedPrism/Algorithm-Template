// UOJ #34
// DeP
#include <cstdio>
#include <complex>
#include <algorithm>
using namespace std;

using Complex = complex<double>;
constexpr int LOG = 18, MAXN = 1 << LOG | 1;
constexpr double PI = acos(-1.0);

namespace Poly {
  int rev[MAXN];
  void init(int lim, int l) {
    for (int i = 1; i < lim; ++i)
      rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (l - 1));
  }

  void FFT(Complex* f, int lim, int type) {
    for (int i = 1; i < lim; ++i)
      if (i < rev[i]) swap(f[i], f[rev[i]]);
    for (int mid = 1; mid < lim; mid <<= 1) {
      Complex wn = exp(Complex(0.0, PI / mid));
      for (int i = 0; i < lim; i += mid << 1) {
        Complex w = 1.0;
        for (int j = 0; j < mid; ++j, w *= wn) {
          Complex f0 = f[i+j], f1 = w * f[i+j+mid];
          f[i+j] = f0 + f1, f[i+j+mid] = f0 - f1;
        }
      }
    }
    if (type < 0) {
      for (int i = 0; i < lim; ++i) f[i] /= lim;
      reverse(f + 1, f + lim);
    }
  }
}

int n, m;
Complex f[MAXN], g[MAXN];

int main() {
  scanf("%d%d", &n, &m), ++n, ++m;
  for (int x, i = 0; i < n; ++i)
    scanf("%d", &x), f[i] = Complex(x);
  for (int x, i = 0; i < m; ++i)
    scanf("%d", &x), g[i] = Complex(x);

  int lim = 1, l = 0;
  while (lim < n + m - 1) lim <<= 1, ++l;
  Poly::init(lim, l), Poly::FFT(f, lim, 1), Poly::FFT(g, lim, 1);
  for (int i = 0; i < lim; ++i) f[i] = f[i] * g[i];
  Poly::FFT(f, lim, -1);

  for (int i = 0; i < n + m - 1; ++i)
    printf("%.0f%c", f[i].real(), " \n"[i == n + m - 2]);
  return 0;
}

// Luogu P4721
// DeP
#include <cstdio>
#include <algorithm>
using namespace std;

using LL = long long;
constexpr int LOG = 18, MAXN = 1 << LOG | 1, P = 998244353, G = 3;

int W[LOG][MAXN];

int fpow(int b, int m) {
  int ret = 1;
  while (m > 0) {
    if (m & 1) ret = (LL) ret * b % P;
    b = (LL) b * b % P, m >>= 1;
  }
  return ret;
}

namespace Poly {
  int rev[MAXN];
  void init(int lim, int l) {
    for (int i = 1; i < lim; ++i)
      rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (l - 1));
  }

  void NTT(int* f, int lim, int type) {
    for (int i = 1; i < lim; ++i)
      if (i < rev[i]) swap(f[i], f[rev[i]]);
    for (int k = 0, mid = 1; mid < lim; ++k, mid <<= 1) {
      const int* w = W[k];
      for (int i = 0; i < lim; i += mid << 1)
        for (int j = 0; j < mid; ++j) {
          int f0 = f[i+j], f1 = 1LL * w[j] * f[i+j+mid] % P;
          f[i+j] = (f0 + f1) % P, f[i+j+mid] = (f0 - f1 + P) % P;
        }
    }
    if (type < 0) {
      int iv = fpow(lim, P - 2);
      for (int i = 0; i < lim; ++i) f[i] = 1LL * iv * f[i] % P;
      reverse(f + 1, f + lim);
    }
  }

  void Mul(int* f, int n, int* g, int m, int* h) {
    static int A[MAXN], B[MAXN];
    int lim = 1, l = 0;
    while (lim < n + m - 1) lim <<= 1, ++l;
    for (int i = 0; i < lim; ++i)
      A[i] = (i < n)? f[i]: 0, B[i] = (i < m)? g[i]: 0;
    init(lim, l), NTT(A, lim, 1), NTT(B, lim, 1);
    for (int i = 0; i < lim; ++i) h[i] = 1ll * A[i] * B[i] % P;
    NTT(h, lim, -1);
  }
}

void PolyPre() {
  for (int w, i = 0, Mid = 1; i < LOG; ++i, Mid <<= 1) {
    W[i][0] = 1, w = fpow(G, (P - 1) / (Mid << 1));
    for (int j = 1; j < Mid; ++j)
      W[i][j] = (LL) w * W[i][j - 1] % P;
  }
}

int n;
int h[MAXN], g[MAXN], f[MAXN];

void solve(int l, int r) { // [l, r]
  if (l == r) return;
  int mid = (l + r) / 2;
  solve(l, mid);
  Poly::Mul(f + l, mid - l + 1, g, r - l + 1, h);
  for (int i = mid + 1; i <= r; ++i)
    f[i] = (f[i] + h[i - l]) % P;
  solve(mid + 1, r);
}

int main() {
  scanf("%d", &n);
  for (int i = 1; i < n; ++i)
    scanf("%d", g + i);
  PolyPre();
  f[0] = 1, solve(0, n - 1);
  for (int i = 0; i < n; ++i)
    printf("%d%c", f[i], " \n"[i == n - 1]);
  return 0;
}

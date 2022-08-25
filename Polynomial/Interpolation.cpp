// Luogu P4781
// DeP
#include <cstdio>

using LL = long long;
constexpr int MAXN = 2e3 + 5, P = 998244353;

int fpow(int b, int m) {
  int ret = 1;
  while (m > 0) {
    if (m & 1) ret = (LL) ret * b % P;
    b = (LL) b * b % P, m >>= 1;
  }
  return ret;
}

int n, k;
int x[MAXN], y[MAXN];

int main() {
  scanf("%d%d", &n, &k);
  for (int i = 1; i <= n; ++i)
    scanf("%d%d", x + i, y + i);
  // f(x) = \sum_{i=1}^n y_i \prod_{j\neq i} \frac{x - x_j}{x_i - x_j}
  int ans = 0;
  for (int i = 1; i <= n; ++i) {
    int m1 = 1, m2 = 1;
    for (int j = 1; j <= n; ++j) if (i != j) {
      m1 = (LL) m1 * (k - x[j] + P) % P;
      m2 = (LL) m2 * (x[i] - x[j] + P) % P;
    }
    (ans += (LL) y[i] * m1 % P * fpow(m2, P - 2) % P) %= P;
  }
  printf("%d\n", ans);
  return 0;
}

// Luogu P3807
// DeP
#include <cstdio>
using namespace std;

typedef long long LL;
const int MAXN = 1e5 + 5;

int ifac[MAXN], fac[MAXN], inv[MAXN];

void PolyPre(int N, const int& P) {
  inv[1] = 1;
  for (int i = 2; i <= N; ++i)
    inv[i] = (LL) inv[P % i] * (P - P / i) % P;
  ifac[0] = fac[0] = 1;
  for (int i = 1; i <= N; ++i) {
    fac[i] = (LL) fac[i - 1] * i % P;
    ifac[i] = (LL) ifac[i - 1] * inv[i] % P;
  }
}

int binom(int n, int m, const int& p) {
  if (n < p && m < p)
    return (n < m)? 0: (LL) fac[n] * ifac[n - m] % p * ifac[m] % p;
  return (m > 0)? (LL) binom(n % p, m % p, p) * binom(n / p, m / p, p) % p: 1;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  int Ti;
  scanf("%d", &Ti);
  for (int n, m, p; Ti; --Ti) {
    scanf("%d%d%d", &n, &m, &p);
    PolyPre(p - 1, p);
    printf("%d\n", binom(n + m, m, p));
  }
  return 0;
}

// Luogu P1495
// DeP
#include <cstdio>

typedef long long LL;
const int MAXN = 15;

void exgcd(LL a, LL b, LL& x, LL& y) {
  if (!b) x = 1, y = 0;
  else exgcd(b, a % b, y, x), y -= a / b * x;
}
inline LL inv(LL v, LL m) {
  static LL x, y;
  return exgcd(v, m, x, y), (x % m + m) % m;
}

LL CRT(int* a, int* m, int n) {
  LL ret = 0, M = 1;
  for (int i = 1; i <= n; ++i) M *= m[i];
  for (int i = 1; i <= n; ++i) {
    LL w = M / m[i];
    ret = (ret + 1LL * a[i] * w % M * inv(w, m[i]) % M) % M;
  }
  return ret;
}

int n;
int a[MAXN], m[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%d", &n);
  for (int i = 1; i <= n; ++i) scanf("%d%d", m + i, a + i);
  printf("%lld\n", CRT(a, m, n));
  return 0;
}

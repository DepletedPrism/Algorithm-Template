// Luogu P6091
// DeP
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 1e6 + 5, MAXM = 1e3 + 5;

int gcd(int a, int b) { return !b? a: gcd(b, a % b); }

int fpow(int base, int b, int m) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = (LL) ret * base % m;
    base = (LL) base * base % m, b >>= 1;
  }
  return ret % m;
}

bool ip[MAXN], prt[MAXN];
int phi[MAXN], pr[MAXN], cp;

void EulerSieve() {
  ip[1] = true, phi[1] = 1;
  for (int i = 2; i < MAXN; ++i) {
    if (!ip[i]) pr[++cp] = i, phi[i] = i - 1;
    for (int j = 1; j <= cp && i * pr[j] < MAXN; ++j) {
      ip[i * pr[j]] = true;
      if (i % pr[j] == 0) { phi[i * pr[j]] = phi[i] * pr[j]; break; }
      phi[i * pr[j]] = phi[i] * (pr[j] - 1);
    }
  }
  prt[2] = prt[4] = true;
  for (int j = 2; j <= cp; ++j) {
    for (int i = 1; (LL) i * pr[j] < MAXN; i *= pr[j])
      prt[i * pr[j]] = true;
    for (int i = 2; (LL) i * pr[j] < MAXN; i *= pr[j])
      prt[i * pr[j]] = true;
  }
}

int Proot(int p) {
  static int fact[MAXM], tot;
  int x = phi[p]; tot = 0;
  for (int d = 2; d * d <= x; ++d) if (x % d == 0) {
    fact[++tot] = d;
    while (x % d == 0) x /= d;
  }
  if (x > 1) fact[++tot] = x;
  for (int g = 1; g < p; ++g) {
    bool flag = (fpow(g, phi[p], p) == 1);
    for (int i = 1; i <= tot && flag; ++i)
      if (fpow(g, phi[p] / fact[i], p) == 1) flag = false;
    if (flag) return g;
  }
  return -1;
}

int stk[MAXN], top;

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  int Ti;
  scanf("%d", &Ti);

  EulerSieve();
  while (Ti--) {
    static int n, d;
    scanf("%d%d", &n, &d);
    if (!prt[n])
      puts("0\n");
    else {
      int g = Proot(n);
      top = 0;
      for (int pw = 1, i = 1; i <= phi[n]; ++i) {
        pw = (LL) pw * g % n;
        if (gcd(i, phi[n]) == 1) stk[++top] = pw;
      }
      sort(stk + 1, stk + 1 + top);

      printf("%d\n", top);
      for (int i = 1; i <= top / d; ++i) printf("%d ", stk[i * d]);
      puts("");
    }
  }
  return 0;
}

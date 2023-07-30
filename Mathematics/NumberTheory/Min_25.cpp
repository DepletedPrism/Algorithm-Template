// Luogu P5325
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int MAXM = 2e5 + 5, P = 1e9 + 7;
constexpr int iv2 = 500000004, iv6 = 166666668;

bool ip[MAXM];
int pr[MAXM], cp;

LL sp1[MAXM], sp2[MAXM];

void EulerSieve(int N) {
  ip[1] = true;
  for (int i = 2; i <= N; ++i) {
    if (!ip[i]) {
      pr[++cp] = i;
      sp1[cp] = (sp1[cp - 1] + i) % P;
      sp2[cp] = (sp2[cp - 1] + (LL) i * i) % P;
    }
    for (int j = 1; j <= cp && i * pr[j] <= N; ++j) {
      ip[i * pr[j]] = true;
      if (i % pr[j] == 0) break;
    }
  }
}

LL n;
int id1[MAXM], id2[MAXM], nw, m;
LL w[MAXM], g1[MAXM], g2[MAXM];

int& idx(LL d) {
  return (d <= m)? id1[d]: id2[n / d];
}

int F(LL x, int k) {
  if (x <= 1 || x < pr[k]) return 0;
  int ret = (((g2[idx(x)] - g1[idx(x)]) - (sp2[k-1] - sp1[k-1])) % P + P) % P;
  for (int i = k; (LL) pr[i] * pr[i] <= x; ++i) {
    LL t1 = pr[i], t2 = (LL) pr[i] * pr[i];
    for ( ; t2 <= x; t1 = t2, t2 *= pr[i]) {
      LL s1 = t1 % P, s2 = t2 % P;
      s1 = (s1 * s1 % P - s1 + P) % P, s2 = (s2 * s2 % P - s2 + P) % P;
      ret = (ret + s1 * F(x / t1, i + 1) % P + s2) % P;
    }
  }
  return ret;
}

int main() {
  scanf("%lld", &n);
  m = sqrt(n), EulerSieve(m + 114);
  for (LL R, L = 1; L <= n; L = R + 1) {
    LL d = n / L;
    R = n / d, w[++nw] = d, idx(d) = nw, d %= P;
    g1[nw] = (d * (d + 1) % P * iv2 % P - 1 + P) % P;
    g2[nw] = (d * (d + 1) % P * (2 * d % P + 1) % P * iv6 % P - 1 + P) % P;
  }
  for (int j = 1; j <= cp; ++j)
    for (int i = 1; i <= nw && (LL) pr[j] * pr[j] <= w[i]; ++i) {
      LL v = w[i] / pr[j], p1 = pr[j], p2 = (LL) pr[j] * pr[j] % P;
      g1[i] = (g1[i] - (g1[idx(v)] - sp1[j - 1] + P) * p1 % P + P) % P;
      g2[i] = (g2[i] - (g2[idx(v)] - sp2[j - 1] + P) * p2 % P + P) % P;
    }
  printf("%d\n", (F(n, 1) + 1) % P);
  return 0;
}

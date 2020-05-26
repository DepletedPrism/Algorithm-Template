// Luogu P1494
// DeP
#include <cmath>
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long LL;
typedef pair<LL, LL> Pii;
const int MAXN = 5e4 + 5;

LL gcd(LL a, LL b) {
  return !b? a: gcd(b, a % b);
}

int n, q, Block;
int C[MAXN];

Pii Ans[MAXN];

struct Ask {
  int idx, L, R;
  Ask() = default;
  Ask(int _i, int _L, int _R): idx(_i), L(_L), R(_R) { }
  bool operator < (const Ask& rhs) const {
    return (L / Block == rhs.L / Block)?
      (R == rhs.R? 0: ((L / Block) & 1) ^ (R < rhs.R)): L < rhs.L;
  }
} Q[MAXN];

namespace Mo {
  int cnt[MAXN]; LL now;

  inline void Add(const int& p) {
    const int& c = C[p];
    now += cnt[c], ++cnt[c];
  }
  inline void Del(const int& p) {
    const int& c = C[p];
    --cnt[c], now -= cnt[c];
  }

  void solve() {
    Block = n / sqrt(q), sort(Q + 1, Q + 1 + q);
    int L = 1, R = 0;
    for (int i = 1; i <= q; ++i) {
      const Ask& a = Q[i];
      while (a.L < L) Add(--L);
      while (a.R > R) Add(++R);
      while (a.L > L) Del(L++);
      while (a.R < R) Del(R--);
      Ans[a.idx] = Pii(now, 1LL * (a.R - a.L) * (a.R - a.L + 1) / 2);
    }
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%d%d", &n, &q);
  for (int i = 1; i <= n; ++i) scanf("%d", C + i);
  for (int L, R, i = 1; i <= q; ++i)
    scanf("%d%d", &L, &R), Q[i] = Ask(i, L, R);

  Mo::solve();
  for (int i = 1; i <= q; ++i) {
    Pii& p = Ans[i];
    if (p.first > 0) {
      LL g = gcd(p.first, p.second);
      p.first /= g, p.second /= g;
    } else
      p = Pii(0, 1);
    printf("%lld/%lld\n", p.first, p.second);
  }
  return 0;
}

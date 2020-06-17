// Luogu P3370
// DeP
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 1e4 + 5, MAXM = 1.5e3 + 5;

struct Hash {
  static const int B = 19260817, P1 = 1e9 + 9, P2 = 998244353;
  int h1, h2;
  Hash() { h1 = h2 = 0; }
  Hash(int _h): h1(_h), h2(_h) { }
  Hash(int _h1, int _h2): h1(_h1), h2(_h2) { }
  Hash operator + (const Hash& rhs) const {
    return Hash((h1 + rhs.h1) % P1, (h2 + rhs.h2) % P2);
  }
  Hash operator - (const Hash& rhs) const {
    return Hash((h1 - rhs.h1 + P1) % P1, (h2 - rhs.h2 + P2) % P2);
  }
  Hash operator * (const Hash& rhs) const {
    return Hash((LL) h1 * rhs.h1 % P1, (LL) h2 * rhs.h2 % P2);
  }
  bool operator < (const Hash& rhs) const {
    return h1 < rhs.h1 || (h1 == rhs.h1 && h2 < rhs.h2);
  }
  bool operator == (const Hash& rhs) const {
    return !(*this < rhs) && !(rhs < *this);
  }
};

int n;
char str[MAXM];

Hash h[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%d", &n);
  for (int i = 1; i <= n; ++i) {
    scanf("%s", str + 1);
    int lgt = strlen(str + 1);
    for (int j = 1; j <= lgt; ++j)
      h[i] = h[i] * Hash::B + str[j];
  }

  sort(h + 1, h + 1 + n);
  int ans = unique(h + 1, h + 1 + n) - h - 1;

  printf("%d\n", ans);
  return 0;
}

// Luogu P3805
// DeP
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

constexpr int MAXN = 1.1e7 + 5;

struct Hash {
  static const int B = 19260817, P1 = 1145141, P2 = 998244353;
  int h1, h2;

  Hash() { h1 = h2 = 0; }
  Hash(int _h1, int _h2): h1(_h1), h2(_h2) { }
  Hash(Hash h, int ch) {
    h1 = (1LL * h.h1 * B % P1 + ch) % P1, h2 = (1LL * h.h2 * B % P2 + ch) % P2;
  }

  Hash operator + (const Hash &rhs) const {
    return Hash((h1 + rhs.h1) % P1, (h2 + rhs.h2) % P2);
  }
  Hash operator - (const Hash &rhs) const {
    return Hash((h1 - rhs.h1 + P1) % P1, (h2 - rhs.h2 + P2) % P2);
  }
  Hash operator * (const Hash &rhs) const {
    return Hash(1LL * h1 * rhs.h1 % P1, 1LL * h2 * rhs.h2 % P2);
  }

  bool operator < (const Hash &rhs) const {
    return h1 < rhs.h1 || (h1 == rhs.h1 && h2 < rhs.h2);
  }
  bool operator == (const Hash &rhs) const {
    return !(*this < rhs) && !(rhs < *this);
  }
  bool operator != (const Hash &rhs) const {
    return *this < rhs || rhs < *this;
  }
};

char s[MAXN];
Hash pb[MAXN], f[MAXN], g[MAXN];

int main() {
  scanf("%s", s + 1);
  int n = strlen(s + 1);
  pb[0] = Hash(1, 1);
  for (int i = 1; i <= n; ++i)
    pb[i] = Hash(pb[i - 1], 0);
  for (int i = 1; i <= n; ++i)
    f[i] = Hash(f[i - 1], s[i]);
  for (int i = n; i >= 1; --i)
    g[i] = Hash(g[i + 1], s[i]);
  int r = 0, mxr = 0;
  for (int i = 1; i <= n; ++i) {
    r = min(r + 2, i);
    while (f[i] - f[i - r] * pb[r] != g[i - r + 1] - g[i + 1] * pb[r])
      --r;
    if (r > mxr) mxr = r;
  }
  printf("%d\n", mxr);
  return 0;
}

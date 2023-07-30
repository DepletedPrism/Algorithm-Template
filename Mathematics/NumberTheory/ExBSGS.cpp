// Luogu P4195
// DeP
#include <cmath>
#include <cstdio>
#include <unordered_map>
using namespace std;

typedef long long LL;

int fpow(int base, int b, int m) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = (LL) ret * base % m;
    base = (LL) base * base % m, b >>= 1;
  }
  return ret % m;
}

int gcd(int a, int b) {
  return !b? a: gcd(b, a % b);
}

int BSGS(int a, int b, int p, int d = 1) {
  unordered_map<int, int> M;
  if (a % p == 0) return -1;
  int pw = b, m = ceil(sqrt(p));
  M[pw] = 0;
  for (int j = 1; j <= m; ++j)
    pw = (LL) pw * a % p, M[pw] = j;
  int t = fpow(a, m, p), pt = d;
  for (int i = 1; i <= m; ++i) {
    pt = (LL) pt * t % p;
    if (M.count(pt) > 0)
      return (((LL) i * m % p - M[pt] + p) % p);
  }
  return -1;
}

int ExBSGS(int a, int b, int p) {
  int g, d = 1, k = 0;
  while ((g = gcd(a, p)) != 1) {
    if (b % g != 0) return -1;
    ++k, b /= g, p /= g, d = (LL) d * (a / g) % p;
    if (d == b) return k;
  }
  return ((d = BSGS(a, b, p, d)) == -1)? -1: d + k;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  static int a, b, p;
  while (scanf("%d%d%d", &a, &p, &b) == 3) {
    if (a == 0 && b == 0 && p == 0) break;
    int ans = ExBSGS(a, b, p);
    if (ans != -1)
      printf("%d\n", ans);
    else puts("No Solution");
  }
  return 0;
}

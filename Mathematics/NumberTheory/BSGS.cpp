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

int BSGS(int a, int b, int p, int d = 1) {
  unordered_map<int, int> M;
  if (a % p == 0) return -1;
  int pw = b % p, m = (int) ceil(sqrt(p));
  M[pw] = 0;
  for (int j = 1; j <= m; ++j)
    pw = (LL) pw * a % p, M[pw] = j;
  int t = fpow(a, m, p), pt = d;
  for (int i = 1; i <= m; ++i) {
    pt = (LL) pt * t % p;
    if (M.count(pt) > 0)
      return ((LL) i * m % p - M[pt] % p + p) % p;
  }
  return -1;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  static int p, b, n;
  scanf("%d%d%d", &p, &b, &n);

  int ans = BSGS(b, n, p);
  if (ans != -1) printf("%d\n", ans);
  else puts("no solution");
  return 0;
}

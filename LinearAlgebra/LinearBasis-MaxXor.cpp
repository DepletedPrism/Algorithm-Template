// Luogu P3812
// DeP
#include <cstdio>
#include <cstring>
using namespace std;

typedef long long LL;
const int MAXN = 5e1 + 5, LOG = 51;

struct Linear {
  LL p[LOG];
  Linear() { memset(p, 0, sizeof p); }

  inline void Ins(LL x) {
    for (int i = LOG - 1; i >= 0; --i) if ((x >> i) & 1) {
      if (!p[i]) { p[i] = x; break; }
      x ^= p[i];
    }
  }
  inline LL Qry() {
    LL ret = 0;
    for (int i = LOG - 1; i >= 0; --i)
      if (!((ret >> i) & 1)) ret ^= p[i];
    return ret;
  }
} B;

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  static int n;
  scanf("%d", &n);
  for (LL x; n; --n)
    scanf("%lld", &x), B.Ins(x);
  printf("%lld\n", B.Qry());
  return 0;
}

// HDU 7184
// DeP
#include <cstdio>
#include <cstring>

using LL = long long;
constexpr int MAXN = 5e1 + 5, LOG = 51;

struct Basis {
  LL p[LOG];
  Basis() { memset(p, 0, sizeof p); }

  void ins(LL x) {
    for (int i = LOG - 1; i >= 0; --i)
      if ((x >> i) & 1) {
        if (!p[i]) { p[i] = x; break; }
        x ^= p[i];
      }
  }
  LL qry() {
    LL ret = 0;
    for (int i = LOG - 1; i >= 0; --i)
      if (!((ret >> i) & 1)) ret ^= p[i];
    return ret;
  }
};

int main() {
  int ti;
  scanf("%d", &ti);
  while (ti--) {
    int n;
    scanf("%d", &n);
    Basis b;
    for (LL x; n > 0; --n)
      scanf("%lld", &x), b.ins(x);
    printf("%lld\n", b.qry());
  }
  return 0;
}

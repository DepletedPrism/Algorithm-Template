// Luogu P5490
// DeP
#include <cctype>
#include <cstdio>
#include <algorithm>
using namespace std;

namespace IO {
  const int MAXSIZE = 1 << 18 | 1;
  char buf[MAXSIZE], *p1, *p2;

  inline int Gc() {
    return p1 == p2 &&
      (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++;
  }
  template<typename T> inline void read(T& x) {
    x = 0; int f = 0, ch = Gc();
    while (!isdigit(ch)) f |= ch == '-', ch = Gc();
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

typedef long long LL;
const int MAXN = 1e5 + 5, MAXM = MAXN << 1;

int n, na, nm;
int num[MAXM];

struct Sgt {
  int type, r, c1, c2;
  Sgt() = default;
  Sgt(int _t, int _r, int _c1, int _c2): type(_t), r(_r), c1(_c1), c2(_c2) { }
  bool operator < (const Sgt& rhs) const {
    return r < rhs.r || (r == rhs.r && type < rhs.type);
  }
} A[MAXM];

#define lc (nd << 1)
#define rc (nd << 1 | 1)
namespace SGT {
  LL s[MAXM << 2]; int c[MAXM << 2];

  inline void maintain(int nd, int L, int R) {
    if (c[nd] > 0)
      s[nd] = num[R + 1] - num[L];
    else if (L == R) s[nd] = 0;
    else s[nd] = s[lc] + s[rc];
  }

  void Mdy(int nd, int L, int R, int opL, int opR, const int& v) {
    if (opL <= L && R <= opR)
      return c[nd] += v, maintain(nd, L, R);
    int Mid = (L + R) / 2;
    if (opL <= Mid) Mdy(lc, L, Mid, opL, opR, v);
    if (opR > Mid) Mdy(rc, Mid + 1, R, opL, opR, v);
    maintain(nd, L, R);
  }
}
#undef lc
#undef rc

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n);
  for (int r1, c1, r2, c2, i = 1; i <= n; ++i) {
    read(r1), read(c1), read(r2), read(c2);
    num[++nm] = c1, num[++nm] = c2;
    A[++na] = Sgt(1, r1, c1, c2), A[++na] = Sgt(-1, r2, c1, c2);
  }

  sort(num + 1, num + 1 + nm);
  nm = unique(num + 1, num + 1 + nm) - num - 1;
  sort(A + 1, A + 1 + na);

  LL ans = 0;
  for (int i = 1; i <= na; ++i) {
    int L = lower_bound(num + 1, num + 1 + nm, A[i].c1) - num,
        R = lower_bound(num + 1, num + 1 + nm, A[i].c2) - num;
    SGT::Mdy(1, 1, nm, L, R - 1, A[i].type);
    ans += SGT::s[1] * (A[i + 1].r - A[i].r);
  }

  printf("%lld\n", ans);
  return 0;
}

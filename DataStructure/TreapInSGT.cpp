// LOJ #106
// DeP
#include <ctime>
#include <cctype>
#include <cstdio>
#include <cstdlib>
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
    while (isdigit(ch)) x = x * 10 - '0' + ch, ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

const int MAXN = 5e4 + 5, MAXV = 1e8;

int n, m;
int A[MAXN];

namespace Treap {
  const int MAXN = ::MAXN << 6;
  int ch[2][MAXN], rnd[MAXN], val[MAXN], size[MAXN];
  int nidx;

  inline int newnode(const int& v) {
    return val[++nidx] = v, size[nidx] = 1, rnd[nidx] = rand(), nidx;
  }

  inline void maintain(int nd) {
    size[nd] = 1;
    if (ch[0][nd]) size[nd] += size[ch[0][nd]];
    if (ch[1][nd]) size[nd] += size[ch[1][nd]];
  }

  void split(int nd, int k, int& x, int& y) {
    if (!nd) return void( x = y = 0 );
    if (val[nd] <= k) x = nd, split(ch[1][nd], k, ch[1][nd], y);
    else y = nd, split(ch[0][nd], k, x, ch[0][nd]);
    maintain(nd);
  }
  int merge(int x, int y) {
    if (!x || !y) return x + y;
    if (rnd[x] < rnd[y]) {
      ch[1][x] = merge(ch[1][x], y), maintain(x);
      return x;
    }
    ch[0][y] = merge(x, ch[0][y]), maintain(y);
    return y;
  }

  inline void Ins(int& rt, const int& v) {
    static int x, y;
    split(rt, v, x, y), rt = merge(merge(x, newnode(v)), y);
  }
  inline void Rmv(int& rt, const int& v) {
    static int x, y, z;
    split(rt, v, x, z), split(x, v-1, x, y);
    y = merge(ch[0][y], ch[1][y]), rt = merge(merge(x, y), z);
  }

  inline int Rnk(int& rt, const int& v) {
    static int x, y, ret;
    split(rt, v-1, x, y), ret = size[x] + 1, rt = merge(x, y);
    return ret;
  }

  inline int Kth(int nd, int k) {
    while (nd > 0) {
      int t = ch[0][nd]? size[ch[0][nd]] + 1: 1;
      if (k == t) return nd;
      if (k > t) k -= t, nd = ch[1][nd]; else nd = ch[0][nd];
    }
    abort();
  }

  inline int Pre(int& rt, const int& v) {
    static int x, y, ret;
    split(rt, v-1, x, y), ret = x? val[Kth(x, size[x])]: -MAXV, rt = merge(x, y);
    return ret;
  }
  inline int Suf(int& rt, const int& v) {
    static int x, y, ret;
    split(rt, v, x, y), ret = y? val[Kth(y, 1)]: MAXV, rt = merge(x, y);
    return ret;
  }
}

namespace SGT {
#define lc (nd<<1)
#define rc (nd<<1|1)
  int rt[MAXN << 2];

  void build(int nd, int L, int R) {
    for (int i = L; i <= R; ++i) Treap::Ins(rt[nd], A[i]);
    if (L == R) return;
    int Mid = (L + R) / 2;
    build(lc, L, Mid), build(rc, Mid+1, R);
  }

  void Mdy(int nd, int L, int R, const int& pos, const int& val) {
    Treap::Rmv(rt[nd], A[pos]), Treap::Ins(rt[nd], val);
    if (L == R) return;
    int Mid = (L + R) / 2;
    if (pos <= Mid) Mdy(lc, L, Mid, pos, val);
    else Mdy(rc, Mid+1, R, pos, val);
  }

  int Rnk(int nd, int L, int R, const int& opL, const int& opR, const int& val) {
    if (opL <= L && R <= opR) return Treap::Rnk(rt[nd], val) - 1;
    int Mid = (L + R) / 2, ret = 0;
    if (opL <= Mid) ret += Rnk(lc, L, Mid, opL, opR, val);
    if (opR > Mid) ret += Rnk(rc, Mid+1, R, opL, opR, val);
    return ret;
  }

  int Kth(const int& opL, const int& opR, const int& k) {
    int L = -MAXV, R = MAXV, ret = -1;
    while (L <= R) {
      int Mid = (L + R) / 2;
      int rnk = Rnk(1, 1, n, opL, opR, Mid) + 1;
      if (rnk <= k) L = Mid + 1, ret = Mid; else R = Mid - 1;
    }
    return ret;
  }

  int Pre(int nd, int L, int R, const int& opL, const int& opR, const int& val) {
    if (opL <= L && R <= opR) return Treap::Pre(rt[nd], val);
    int Mid = (L + R) / 2, ret = -MAXV;
    if (opL <= Mid) ret = max(ret, Pre(lc, L, Mid, opL, opR, val));
    if (opR > Mid) ret = max(ret, Pre(rc, Mid+1, R, opL, opR, val));
    return ret;
  }

  int Suf(int nd, int L, int R, const int& opL, const int& opR, const int& val) {
    if (opL <= L && R <= opR) return Treap::Suf(rt[nd], val);
    int Mid = (L + R) / 2, ret = MAXV;
    if (opL <= Mid) ret = min(ret, Suf(lc, L, Mid, opL, opR, val));
    if (opR > Mid) ret = min(ret, Suf(rc, Mid+1, R, opL, opR, val));
    return ret;
  }
#undef lc
#undef rc
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  srand(time(nullptr));
  // input
  read(n), read(m);
  for (int i = 1; i <= n; ++i) read(A[i]);
  // solve
  SGT::build(1, 1, n);
  while (m--) {
    static int opt, L, R, x;
    read(opt), read(L), read(R); if (opt != 3) read(x);
    switch (opt) {
      case 1: printf("%d\n", SGT::Rnk(1, 1, n, L, R, x) + 1); break;
      case 2: printf("%d\n", SGT::Kth(L, R, x)); break;
      case 3: SGT::Mdy(1, 1, n, L, R), A[L] = R; break;
      case 4: printf("%d\n", SGT::Pre(1, 1, n, L, R, x)); break;
      case 5: printf("%d\n", SGT::Suf(1, 1, n, L, R, x)); break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

// Luogu P3835
// DeP
#include <ctime>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <climits>

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

const int MAXN = 5e5 + 5;

int n, rt[MAXN];

namespace Treap {
  struct Node { int lc, rc, size, val, rnd; } dat[MAXN * 50];
  int nidx;

  inline void maintain(int nd) {
    dat[nd].size = 1;
    if (dat[nd].lc) dat[nd].size += dat[dat[nd].lc].size;
    if (dat[nd].rc) dat[nd].size += dat[dat[nd].rc].size;
  }

  inline int newnode(int v) {
    return dat[++nidx] = (Node){ 0, 0, 1, v, rand() }, nidx;
  }
  inline int copynode(int idx) {
    return dat[++nidx] = dat[idx], nidx;
  }

  void split(int nd, const int& k, int& x, int& y) {
    if (!nd) return void( x = y = 0 );
    if (dat[nd].val <= k) {
      x = copynode(nd), split(dat[x].rc, k, dat[x].rc, y);
      maintain(x);
    } else {
      y = copynode(nd), split(dat[y].lc, k, x, dat[y].lc);
      maintain(y);
    }
  }

  int merge(int x, int y) {
    if (!x || !y) return x + y;
    if (dat[x].rnd < dat[y].rnd) {
      int nd = copynode(x);
      dat[nd].rc = merge(dat[nd].rc, y);
      return maintain(nd), nd;
    }
    int nd = copynode(y);
    dat[nd].lc = merge(x, dat[nd].lc);
    return maintain(nd), nd;
  }

  inline void Ins(int& root, int v) {
    int x = 0, y = 0;
    split(root, v, x, y), root = merge(merge(x, newnode(v)), y);
  }
  inline void Del(int& root, int v) {
    int x = 0, y = 0, z = 0;
    split(root, v, x, z), split(x, v-1, x, y);
    y = merge(dat[y].lc, dat[y].rc), root = merge(merge(x, y), z);
  }

  inline void Rnk(int& root, int v) {
    int x = 0, y = 0;
    split(root, v-1, x, y);
    printf("%d\n", dat[x].size + 1);
    root = merge(x, y);
  }
  int Kth(int nd, int k) {
    while (true) {
      int t = dat[nd].lc? dat[dat[nd].lc].size+1: 1;
      if (k == t) return nd;
      if (k > t) k -= t, nd = dat[nd].rc;
      else nd = dat[nd].lc;
    }
    return -1;
  }

  inline void Pre(int& root, int v) {
    int x = 0, y = 0;
    split(root, v-1, x, y);
    printf("%d\n", x? dat[Kth(x, dat[x].size)].val: -INT_MAX);
    root = merge(x, y);
  }
  inline void Suf(int& root, int v) {
    int x = 0, y = 0;
    split(root, v, x, y);
    printf("%d\n", y? dat[Kth(y, 1)].val: INT_MAX);
    root = merge(x, y);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  srand(time(nullptr));

  read(n);
  for (int i = 1; i <= n; ++i) {
    static int v, opt, x;
    read(v), read(opt), read(x);
    rt[i] = rt[v];
    switch (opt) {
      case 1: Treap::Ins(rt[i], x); break;
      case 2: Treap::Del(rt[i], x); break;
      case 3: Treap::Rnk(rt[i], x); break;
      case 4: printf("%d\n", Treap::dat[Treap::Kth(rt[i], x)].val); break;
      case 5: Treap::Pre(rt[i], x); break;
      case 6: Treap::Suf(rt[i], x); break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

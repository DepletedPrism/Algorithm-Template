// Luogu P5055
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
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

typedef long long LL;
const int MAXN = 2e5 + 5;

int n, rt[MAXN];
LL lastans;

namespace Treap {
  struct Node { int lc, rc, val, size; LL sum; bool res; } dat[MAXN << 6];
  int nidx;

  inline int newnode(const int& v) {
    return dat[++nidx] = (Node){ 0, 0, v, 1, v, false }, nidx;
  }
  inline int copynode(int idx) { return dat[++nidx] = dat[idx], nidx; }

  inline void maintain(int nd) {
    const int &lc = dat[nd].lc, &rc = dat[nd].rc;
    dat[nd].size = 1, dat[nd].sum = dat[nd].val;
    if (lc > 0)
      dat[nd].size += dat[lc].size, dat[nd].sum += dat[lc].sum;
    if (rc > 0)
      dat[nd].size += dat[rc].size, dat[nd].sum += dat[rc].sum;
  }
  inline void pushdown(int nd) {
    if (!nd || !dat[nd].res) return;
    const int &lc = dat[nd].lc, &rc = dat[nd].rc;
    if (lc > 0)
      dat[nd].lc = copynode(lc), dat[dat[nd].lc].res ^= 1;
    if (rc > 0)
      dat[nd].rc = copynode(rc), dat[dat[nd].rc].res ^= 1;
    swap(dat[nd].lc, dat[nd].rc);
    dat[nd].res = false;
  }

  void split(int nd, const int& k, int& x, int& y) {
    if (!nd) return void( x = y = 0 );
    pushdown(nd);
    if (dat[dat[nd].lc].size < k) {
      x = copynode(nd);
      split(dat[x].rc, k - dat[dat[nd].lc].size - 1, dat[x].rc, y);
      maintain(x);
    } else {
      y = copynode(nd);
      split(dat[y].lc, k, x, dat[y].lc);
      maintain(y);
    }
  }
  int merge(int x, int y) {
    if (!x || !y) return x + y;
    int s =  ((double) rand() / RAND_MAX) * (dat[x].size + dat[y].size);
    if (s < dat[x].size)
      return pushdown(x), dat[x].rc = merge(dat[x].rc, y), maintain(x), x;
    return pushdown(y), dat[y].lc = merge(x, dat[y].lc), maintain(y), y;
  }

  inline void Ins(int& root, int p, int v) {
    int x = 0, y = 0;
    split(root, p, x, y);
    root = merge(merge(x, newnode(v)), y);
  }
  inline void Del(int& root, int p) {
    int x = 0, y = 0, z = 0;
    split(root, p, x, z), split(x, p - 1, x, y);
    root = merge(x, z);
  }

  inline void Rev(int& root, int L, int R) {
    int x = 0, y = 0, z = 0;
    split(root, R, x, z), split(x, L - 1, x, y);
    dat[y].res ^= 1;
    root = merge(merge(x, y), z);
  }

  inline void Qry(int& root, int L, int R) {
    int x = 0, y = 0, z = 0;
    split(root, R, x, z), split(x, L - 1, x, y);
    printf("%lld\n", lastans = dat[y].sum);
    root = merge(merge(x, y), z);
  }

  inline void debug(int u) {
    pushdown(u);
    if (dat[u].lc) debug(dat[u].lc);
    printf("%d ", dat[u].val);
    if (dat[u].rc) debug(dat[u].rc);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  srand(time(nullptr));
  read(n);

  for (int i = 1; i <= n; ++i) {
    static int v, opt, L, R;
    read(v), read(opt), read(L); if (opt != 2) read(R);
    rt[i] = rt[v]; L ^= lastans, R ^= lastans;
    switch (opt) {
      case 1: Treap::Ins(rt[i], L, R); break;
      case 2: Treap::Del(rt[i], L); break;
      case 3: Treap::Rev(rt[i], L, R); break;
      case 4: Treap::Qry(rt[i], L, R); break;
      default: fprintf(stderr, "ERR\n");
    }
    // Treap::debug(rt[i]), putchar('\n');
  }
  return 0;
}

// LOJ #104
// DeP
#include <cctype>
#include <cstdio>
#include <cassert>
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

const int MAXN = 1e5 + 5;

int q;

namespace Splay {
  int root, nidx;
  int ch[2][MAXN], pre[MAXN], size[MAXN], cnt[MAXN], val[MAXN];

  inline int newnode(const int& v, const int& c = 1) {
    return ++nidx, val[nidx] = v, size[nidx] = cnt[nidx] = c, nidx;
  }

  inline void maintain(int nd) {
    size[nd] = cnt[nd];
    if (ch[0][nd] > 0) size[nd] += size[ch[0][nd]];
    if (ch[1][nd] > 0) size[nd] += size[ch[1][nd]];
  }

  inline int which(int u) { return (pre[u] > 0)? ch[1][pre[u]] == u: 0; }

  inline void connect(int u, int fa, int w) {
    if (u > 0) pre[u] = fa;
    if (fa > 0) ch[w][fa] = u; else root = u;
  }
  inline void rotate(int u) {
    int fa = pre[u], w = which(u);
    connect(u, pre[fa], which(fa));
    connect(ch[w ^ 1][u], fa, w), connect(fa, u, w ^ 1);
    maintain(fa);
  }
  void splay(int u, int v) {
    v = pre[v];
    while (pre[u] != v) {
      int fa = pre[u];
      if (pre[fa] != v) which(fa) == which(u)? rotate(fa): rotate(u);
      rotate(u);
    }
    maintain(u);
  }

  void Ins(const int& v) {
    if (!root) return void( root = newnode(v) );
    for (int w, nd = root; nd; nd = ch[w][nd]) {
      if (val[nd] == v)
        return ++cnt[nd], splay(nd, root);
      if (!ch[w = (val[nd] < v)][nd]) {
        ch[w][nd] = newnode(v), pre[ch[w][nd]] = nd;
        return splay(ch[w][nd], root);
      }
    }
  }

  inline void Fnd(const int& v) {
    for (int nd = root; nd; nd = ch[val[nd] < v][nd])
      if (val[nd] == v) return splay(nd, root);
    assert(false);
  }
  void Del(const int& v) {
    Fnd(v);
    if (cnt[root] > 1)
      return void( --cnt[root] );
    if (!ch[0][root] && !ch[1][root])
      return void( root = 0 );
    if (!ch[0][root] || !ch[1][root]) {
      int w = !ch[0][root];
      return root = ch[w][root], void( pre[root] = 0 );
    }
    int nd = ch[0][root];
    while (ch[1][nd] > 0) nd = ch[1][nd];
    splay(nd, ch[0][root]);
    ch[1][nd] = ch[1][root], pre[nd] = 0;
    root = pre[ch[1][nd]] = nd, maintain(nd);
  }

  int build(const int* A, int L, int R, int fa) {
    if (L > R) return 0;
    int Mid = (L + R) / 2, nd = newnode(A[Mid]);
    pre[nd] = fa;
    ch[0][nd] = build(A, L, Mid - 1, nd);
    ch[1][nd] = build(A, Mid + 1, R, nd);
    return maintain(nd), nd;
  }

  void Kth(int nd, int k) {
    while (nd > 0) {
      if (ch[0][nd] > 0) {
        int t = size[ch[0][nd]];
        if (k <= t) { nd = ch[0][nd]; continue; }
        k -= t;
      }
      if (k <= cnt[nd])
        return printf("%d\n", val[nd]), splay(nd, root);
      k -= cnt[nd], nd = ch[1][nd];
    }
    assert(false);
  }

  inline void Rnk(const int& x) {
    Ins(x), printf("%d\n", size[ch[0][root]] + 1), Del(x);
  }
  inline void Nxt(const int& x, int w) {
    Ins(x);
    int nd = ch[w][root]; w ^= 1;
    while (ch[w][nd] > 0) nd = ch[w][nd];
    printf("%d\n", val[nd]);
    Del(x), splay(nd, root);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(q);
  for (int opt, x; q; --q) {
    read(opt), read(x);
    switch (opt) {
      case 1: Splay::Ins(x); break;
      case 2: Splay::Del(x); break;
      case 3: Splay::Rnk(x); break;
      case 4: Splay::Kth(Splay::root, x); break;
      case 5: Splay::Nxt(x, 0); break;
      case 6: Splay::Nxt(x, 1); break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

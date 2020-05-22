// LOJ #104
// DeP
#include <cctype>
#include <cstdio>

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

int n, root;

namespace Splay {
  int nidx;
  int ch[2][MAXN], size[MAXN], pre[MAXN], cnt[MAXN], val[MAXN];

  inline void maintain(const int& nd) {
    size[nd] = cnt[nd];
    if (ch[0][nd]) size[nd] += size[ch[0][nd]];
    if (ch[1][nd]) size[nd] += size[ch[1][nd]];
  }

  inline int which(const int& u) { return pre[u]? ch[1][pre[u]] == u: 0; }
  inline void connect(const int& u, const int& fa, const int& w) {
    if (u) pre[u] = fa;
    if (fa) ch[w][fa] = u; else root = u;
  }
  inline void rotate(const int& u) {
    int fa = pre[u], w = which(u);
    connect(u, pre[fa], which(fa));
    connect(ch[w^1][u], fa, w), connect(fa, u, w ^ 1);
    maintain(fa);
  }

  void splay(const int& u, int v) {
    v = pre[v];
    while (pre[u] != v) {
      int fa = pre[u];
      if (pre[fa] != v) which(u) == which(fa)? rotate(fa): rotate(u);
      rotate(u);
    }
    maintain(u);
  }

  void insert(const int& x) {
    if (!root) return root = ++nidx, size[root] = cnt[root] = 1, void(val[root] = x);
    for (int nd = root; nd; nd = ch[val[nd] < x][nd]) {
      if (val[nd] == x) return ++cnt[nd], splay(nd, root);
      if (!ch[val[nd] < x][nd]) {
        ch[val[nd] < x][nd] = ++nidx, pre[nidx] = nd, size[nidx] = cnt[nidx] = 1, val[nidx] = x;
        return splay(nidx, root);
      }
    }
  }

  inline void find(const int& x) {
    for (int nd = root; nd; nd = ch[val[nd] < x][nd])
      if (val[nd] == x) return splay(nd, root);
  }

  void remove(const int& x) {
    find(x);
    if (cnt[root] > 1) return void(--cnt[root]);
    if (!ch[0][root] && !ch[1][root]) return void(root = 0);
    if (!ch[0][root] || !ch[1][root]) {
      int w = !ch[0][root];
      return root = ch[w][root], void(pre[root] = 0);
    }
    int nd = ch[0][root];
    while (ch[1][nd]) nd = ch[1][nd];
    splay(nd, ch[0][root]);
    ch[1][nd] = ch[1][root], pre[nd] = 0, root = pre[ch[1][nd]] = nd;
    maintain(nd);
  }

  void Rnk(const int& x) {
    insert(x);
    printf("%d\n", size[ch[0][root]] + 1);
    remove(x);
  }

  void Kth(int k) {
    int t, nd = root;
    while (nd) {
      if (ch[0][nd]) t = size[ch[0][nd]]; else t = 0;
      if (t < k && k <= t + cnt[nd]) return (void) printf("%d\n", val[nd]);
      t += cnt[nd];
      if (k >= t) k -= t, nd = ch[1][nd];
      else nd = ch[0][nd];
    }
  }

  void Nxt(const int& x, int w) {
    insert(x);
    int nd = ch[w][root]; w ^= 1;
    while (ch[w][nd]) nd = ch[w][nd];
    printf("%d\n", val[nd]);
    remove(x), splay(nd, root);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  // input
  read(n);
  // operator
  while (n--) {
    int opt, x;
    read(opt), read(x);
    switch (opt) {
      case 1: Splay::insert(x); break;
      case 2: Splay::remove(x); break;
      case 3: Splay::Rnk(x); break;
      case 4: Splay::Kth(x); break;
      case 5: Splay::Nxt(x, 0); break;
      case 6: Splay::Nxt(x, 1); break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

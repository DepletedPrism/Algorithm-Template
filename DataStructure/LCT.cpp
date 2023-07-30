// Luogu P3690
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

const int MAXN = 1e5 + 5;

int n, q;

namespace LCT {
  int ch[2][MAXN], pre[MAXN], val[MAXN], s[MAXN], res[MAXN];

  inline void maintain(int nd) {
    s[nd] = val[nd];
    if (ch[0][nd] > 0) s[nd] ^= s[ch[0][nd]];
    if (ch[1][nd] > 0) s[nd] ^= s[ch[1][nd]];
  }
  inline void pushdown(int nd) {
    if (!res[nd]) return;
    swap(ch[0][nd], ch[1][nd]);
    if (ch[0][nd] > 0) res[ch[0][nd]] ^= 1;
    if (ch[1][nd] > 0) res[ch[1][nd]] ^= 1;
    res[nd] = 0;
  }

  inline int which(int u) { return (pre[u] > 0)? ch[1][pre[u]] == u: 0; }
  inline bool nroot(int u) {
    return (pre[u] > 0)? ch[0][pre[u]] == u || ch[1][pre[u]] == u: false;
  }
  inline void rotate(int u) {
    int fa = pre[u], w = which(u);
    pre[u] = pre[fa];
    if (nroot(fa)) ch[which(fa)][pre[fa]] = u;
    ch[w][fa] = ch[w ^ 1][u];
    if (ch[w ^ 1][u] > 0) pre[ch[w ^ 1][u]] = fa;
    ch[w ^ 1][u] = fa, pre[fa] = u;
    maintain(fa);
  }
  void splay(int u) {
    static int stk[MAXN], top, fa;
    stk[top = 1] = fa = u;
    while (nroot(fa)) stk[++top] = fa = pre[fa];
    while (top > 0) pushdown(stk[top--]);
    while (nroot(u)) {
      fa = pre[u];
      if (nroot(fa)) which(fa) == which(u)? rotate(fa): rotate(u);
      rotate(u);
    }
    maintain(u);
  }

  void access(int u) {
    for (int v = 0; u; v = u, u = pre[u])
      splay(u), ch[1][u] = v, maintain(u);
  }
  inline void evert(const int& u) { access(u), splay(u), res[u] ^= 1; }
  inline void split(const int& u, const int& v) {
    evert(u), access(v), splay(v);
  }

  inline int Fndrt(int u) {
    access(u), splay(u), pushdown(u);
    while (ch[0][u] > 0) u = ch[0][u], pushdown(u);
    return splay(u), u;
  }

  inline void Lnk(const int& u, const int& v) {
    evert(u);
    if (Fndrt(v) != u) pre[u] = v;
  }
  inline void Cut(const int& u, const int& v) {
    evert(u);
    if (Fndrt(v) != u || pre[v] != u || ch[0][v]) return;
    pre[v] = ch[1][u] = 0, maintain(u);
  }

  inline int Qry(const int& u, const int& v) { return split(u, v), s[v]; }
  inline void Mdy(const int& u, const int& d) {
    splay(u), val[u] = d, maintain(u);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(q);
  for (int i = 1; i <= n; ++i) read(LCT::val[i]);

  for (int opt, x, y; q; --q) {
    read(opt), read(x), read(y);
    switch (opt) {
      case 0: printf("%d\n", LCT::Qry(x, y)); break;
      case 1: LCT::Lnk(x, y); break;
      case 2: LCT::Cut(x, y); break;
      case 3: LCT::Mdy(x, y); break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

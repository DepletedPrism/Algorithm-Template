// Luogu P3919
// DeP
#include <cctype>
#include <cstdio>
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

const int MAXN = 1e6 + 5, LOG = 21;

int n, m;
int A[MAXN];

int rt[MAXN];

namespace SGT {
  struct Node { int lc, rc, v; } dat[MAXN * LOG];
  int nidx;

  void build(int& nd, int L, int R) {
    nd = ++nidx;
    if (L == R) return void( dat[nd].v = A[L] );
    int Mid = (L + R) / 2;
    build(dat[nd].lc, L, Mid), build(dat[nd].rc, Mid + 1, R);
  }

  int Mdy(int nd, int L, int R, const int& p, const int& v) {
    int nxt = ++nidx; dat[nxt] = dat[nd];
    if (L == R) return dat[nxt].v = v, nxt;
    int Mid = (L + R) / 2;
    if (p <= Mid)
      dat[nxt].lc = Mdy(dat[nxt].lc, L, Mid, p, v);
    else
      dat[nxt].rc = Mdy(dat[nxt].rc, Mid + 1, R, p, v);
    return nxt;
  }

  int Qry(int nd, int L, int R, const int& p) {
    if (L == R) return dat[nd].v;
    int Mid = (L + R) / 2;
    if (p <= Mid)
      return Qry(dat[nd].lc, L, Mid, p);
    return Qry(dat[nd].rc, Mid + 1, R, p);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(m);
  for (int i = 1; i <= n; ++i) read(A[i]);

  SGT::build(rt[0], 1, n);

  for (int i = 1; i <= m; ++i) {
    static int opt, nth, p, v;
    read(nth), read(opt), read(p);
    switch (opt) {
      case 1: read(v), rt[i] = SGT::Mdy(rt[nth], 1, n, p, v); break;
      case 2: printf("%d\n", SGT::Qry(rt[nth], 1, n, p)), rt[i] = rt[nth]; break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

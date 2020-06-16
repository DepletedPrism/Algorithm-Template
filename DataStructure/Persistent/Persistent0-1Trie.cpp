// Luogu P4735
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
using IO::read; using IO::Gc;

const int MAXN = 3e5 + 5, MAXM = MAXN << 1;

int n, m;
int A[MAXM], Pre[MAXM];

int rt[MAXM];

namespace Trie {
  const int MAXN = ::MAXN * 18 * 25;
  int ch[2][MAXN], size[MAXN], nidx;

  inline void init() { nidx = 1; }

  inline void Ins(int nd, int lst, const int& val) {
    for (int i = 24; i >= 0; --i) {
      int c = (val >> i) & 1;
      size[nd] = size[lst] + 1;
      if (!ch[c][nd]) ch[c][nd] = ++nidx;
      ch[!c][nd] = ch[!c][lst];
      nd = ch[c][nd], lst = ch[c][lst];
    }
    size[nd] = size[lst] + 1;
  }

  inline int Qry(int x, int y, const int& val) {
    int ret = 0;
    for (int i = 24; i >= 0; --i) {
      int c = (val >> i) & 1;
      if (size[ch[!c][y]] > size[ch[!c][x]])
        ret |= 1 << i, x = ch[!c][x], y = ch[!c][y];
      else x = ch[c][x], y = ch[c][y];
    }
    return ret;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Trie::init();

  read(n), read(m);
  for (int i = 1; i <= n; ++i) read(A[i]), Pre[i] = Pre[i-1] ^ A[i];

  for (int i = 1; i <= n; ++i)
    Trie::Ins(rt[i] = ++Trie::nidx, rt[i - 1], Pre[i]);

  while (m--) {
    static int opt, L, R, x;
    opt = Gc();
    while (isspace(opt)) opt = Gc();
    switch (opt) {
      case 'A':
        read(A[++n]), Pre[n] = Pre[n-1] ^ A[n];
        Trie::Ins(rt[n] = ++Trie::nidx, rt[n-1], Pre[n]);
        break;
      case 'Q':
        read(L), read(R), read(x);
        --L, --R;
        printf("%d\n", (L == R && !L)? (Pre[n] ^ x): Trie::Qry(rt[max(0, L-1)], rt[R], Pre[n] ^ x));
        break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

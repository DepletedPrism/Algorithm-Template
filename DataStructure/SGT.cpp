// Luogu P3373
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

typedef long long LL;
const int MAXN = 1e5 + 5, P = 571373;

int n, q, p;
int A[MAXN];

#define lc (nd << 1)
#define rc (nd << 1 | 1)
namespace SGT {
  int s[MAXN << 2], tm[MAXN << 2], ta[MAXN << 2];

  inline void maintain(int nd) { s[nd] = (s[lc] + s[rc]) % p; }

  inline void pushm(int nd, const int& v) {
    s[nd] = (LL) s[nd] * v % p;
    ta[nd] = (LL) ta[nd] * v % p, tm[nd] = (LL) tm[nd] * v % p;
  }
  inline void pusha(int nd, int L, int R, const int& v) {
    ta[nd] = (ta[nd] + v) % p;
    s[nd] = (s[nd] + (LL) (R - L + 1) * v) % p;
  }

  inline void pushdown(int nd, int L, int R) {
    if (tm[nd] != 1)
      pushm(lc, tm[nd]), pushm(rc, tm[nd]), tm[nd] = 1;
    if (ta[nd] > 0) {
      int Mid = (L + R) / 2;
      pusha(lc, L, Mid, ta[nd]), pusha(rc, Mid + 1, R, ta[nd]), ta[nd] = 0;
    }
  }

  void build(int nd, int L, int R) {
    tm[nd] = 1, ta[nd] = s[nd] = 0;
    if (L == R) return void( s[nd] = A[L] );
    int Mid = (L + R) / 2;
    build(lc, L, Mid), build(rc, Mid + 1, R);
    maintain(nd);
  }

  void Add(int nd, int L, int R, int opL, int opR, const int& v) {
    if (opL <= L && R <= opR) return pusha(nd, L, R, v);
    pushdown(nd, L, R);
    int Mid = (L + R) / 2;
    if (opL <= Mid) Add(lc, L, Mid, opL, opR, v);
    if (opR > Mid) Add(rc, Mid + 1, R, opL, opR, v);
    maintain(nd);
  }

  void Mul(int nd, int L, int R, int opL, int opR, const int& v) {
    if (opL <= L && R <= opR) return pushm(nd, v);
    pushdown(nd, L, R);
    int Mid = (L + R) / 2;
    if (opL <= Mid) Mul(lc, L, Mid, opL, opR, v);
    if (opR > Mid) Mul(rc, Mid + 1, R, opL, opR, v);
    maintain(nd);
  }

  int Qry(int nd, int L, int R, int opL, int opR) {
    if (opL <= L && R <= opR) return s[nd];
    pushdown(nd, L, R);
    int Mid = (L + R) / 2;
    if (opR <= Mid) return Qry(lc, L, Mid, opL, opR);
    if (opL > Mid) return Qry(rc, Mid + 1, R, opL, opR);
    return (Qry(lc, L, Mid, opL, opR) + Qry(rc, Mid + 1, R, opL, opR)) % p;
  }
}
#undef lc
#undef rc

int main() {
  read(n), read(q), read(p);
  for (int i = 1; i <= n; ++i) read(A[i]);

  SGT::build(1, 1, n);
  for (int opt, L, R, x; q; --q) {
    read(opt), read(L), read(R);
    switch (opt) {
      case 1:
        read(x), x = (x % p + p) % p;
        SGT::Mul(1, 1, n, L, R, x);
        break;
      case 2:
        read(x), x = (x % p + p) % p;
        SGT::Add(1, 1, n, L, R, x);
        break;
      case 3:
        printf("%d\n", SGT::Qry(1, 1, n, L, R));
        break;
    }
  }
  return 0;
}

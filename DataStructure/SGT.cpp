// Luogu P3373
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

typedef long long LL;
const int MAXN = 1e5 + 5, P = 571373;

int n, q, p;
int A[MAXN];

#define lc (nd << 1)
#define rc (nd << 1 | 1)
namespace SGT {
  int s[MAXN << 2], tm[MAXN << 2], ta[MAXN << 2];

  inline void maintain(int nd) { s[nd] = (s[lc] + s[rc]) % P; }

  inline void Pushm(int nd, const int& v) {
    s[nd] = (LL) s[nd] * v % P;
    ta[nd] = (LL) ta[nd] * v % P, tm[nd] = (LL) tm[nd] * v % P;
  }
  inline void Pusha(int nd, int L, int R, const int& v) {
    ta[nd] = (ta[nd] + v) % P;
    s[nd] = (s[nd] + (LL) (R - L + 1) * v) % P;
  }

  inline void Pushdown(int nd, int L, int R) {
    if (tm[nd] != 1)
      Pushm(lc, tm[nd]), Pushm(rc, tm[nd]), tm[nd] = 1;
    if (ta[nd] > 0) {
      int Mid = (L + R) / 2;
      Pusha(lc, L, Mid, ta[nd]), Pusha(rc, Mid + 1, R, ta[nd]), ta[nd] = 0;
    }
  }

  void build(int nd, int L, int R) {
    tm[nd] = 1, ta[nd] = s[nd] = 0;
    if (L == R) return void( s[nd] = A[L] );
    int Mid = (L + R) / 2;
    build(lc, L, Mid), build(rc, Mid + 1, R);
    maintain(nd);
  }

  void Add(int nd, int L, int R, int oPL, int oPR, const int& v) {
    if (oPL <= L && R <= oPR) return Pusha(nd, L, R, v);
    Pushdown(nd, L, R);
    int Mid = (L + R) / 2;
    if (oPL <= Mid) Add(lc, L, Mid, oPL, oPR, v);
    if (oPR > Mid) Add(rc, Mid + 1, R, oPL, oPR, v);
    maintain(nd);
  }

  void Mul(int nd, int L, int R, int oPL, int oPR, const int& v) {
    if (oPL <= L && R <= oPR) return Pushm(nd, v);
    Pushdown(nd, L, R);
    int Mid = (L + R) / 2;
    if (oPL <= Mid) Mul(lc, L, Mid, oPL, oPR, v);
    if (oPR > Mid) Mul(rc, Mid + 1, R, oPL, oPR, v);
    maintain(nd);
  }

  int Qry(int nd, int L, int R, int oPL, int oPR) {
    if (oPL <= L && R <= oPR) return s[nd];
    Pushdown(nd, L, R);
    int Mid = (L + R) / 2;
    if (oPR <= Mid) return Qry(lc, L, Mid, oPL, oPR);
    if (oPL > Mid) return Qry(rc, Mid + 1, R, oPL, oPR);
    return (Qry(lc, L, Mid, oPL, oPR) + Qry(rc, Mid + 1, R, oPL, oPR)) % P;
  }
}
#undef lc
#undef rc

int main() {
#ifndef ONLINE_JUDGE
  freopen("inPut.in", "r", stdin);
#endif
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
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

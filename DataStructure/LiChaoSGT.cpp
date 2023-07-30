// UOJ #88
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

typedef long long LL;
const int MAXN = 1e5 + 5, MAXQ = 5e5 + 5, MAXM = MAXN + MAXQ;
const LL INFLL = 0x3f3f3f3f3f3f3f3fLL;

struct Point {
  LL t, x, v;
} R[MAXN];

struct Line {
  LL k, b;
  Line() { k = b = 0; }
  Line(LL _k, LL _b): k(_k), b(_b) { }
} Ln[MAXN << 1];

struct Ask {
  int opt, t, k, x; 
  Ask() = default;
  Ask(int _op, int _t, int _k, int _x): opt(_op), t(_t), k(_k), x(_x) { }
} Q[MAXM];

int n, m, nl;
int num[MAXM], nm;

#define lc (nd << 1)
#define rc (nd << 1 | 1) 
namespace SGT {
  int Mn[MAXM << 2], Mx[MAXM << 2];
  bool tmn[MAXM << 2], tmx[MAXM << 2];

  inline LL calc(int idx, int x) { return Ln[idx].k * num[x] + Ln[idx].b; }

  void MdyMx(int nd, int L, int R, const int& opL, const int& opR, int idx) {
    int Mid = (L + R) / 2;
    if (opL <= L && R <= opR) {
      if (!tmx[nd]) return tmx[nd] = true, void( Mx[nd] = idx );
      if (calc(idx, L) <= calc(Mx[nd], L) && calc(idx, R) <= calc(Mx[nd], R))
        return;
      if (calc(idx, L) >= calc(Mx[nd], L) && calc(idx, R) >= calc(Mx[nd], R))
        return void( Mx[nd] = idx );
      if (calc(idx, Mid) >= calc(Mx[nd], Mid)) swap(idx, Mx[nd]);
      if (calc(idx, L) >= calc(Mx[nd], L))
        MdyMx(lc, L, Mid, L, R, idx);
      else
        MdyMx(rc, Mid + 1, R, L, R, idx);
    }
    if (opL <= Mid) MdyMx(lc, L, Mid, opL, opR, idx);
    if (opR > Mid) MdyMx(rc, Mid + 1, R, opL, opR, idx);
  }

  void MdyMn(int nd, int L, int R, const int& opL, const int& opR, int idx) {
    int Mid = (L + R) / 2;
    if (opL <= L && R <= opR) {
      if (!tmn[nd]) return tmn[nd] = true, void( Mn[nd] = idx );
      if (calc(idx, L) >= calc(Mn[nd], L) && calc(idx, R) >= calc(Mn[nd], R))
        return;
      if (calc(idx, L) <= calc(Mn[nd], L) && calc(idx, R) <= calc(Mn[nd], R))
        return void( Mn[nd] = idx );
      if (calc(idx, Mid) <= calc(Mn[nd], Mid)) swap(idx, Mn[nd]);
      if (calc(idx, L) <= calc(Mn[nd], L))
        MdyMn(lc, L, Mid, L, R, idx);
      else
        MdyMn(rc, Mid + 1, R, L, R, idx);
    }
    if (opL <= Mid) MdyMn(lc, L, Mid, opL, opR, idx);
    if (opR > Mid) MdyMn(rc, Mid + 1, R, opL, opR, idx);
  }

  inline void Mdy(int L, int R, const int& idx) {
    MdyMx(1, 1, nm, L, R, idx), MdyMn(1, 1, nm, L, R, idx);
  }

  LL QryMx(int nd, int L, int R, const int& p) {
    if (L == R) return tmx[nd]? calc(Mx[nd], L): -INFLL;
    int Mid = (L + R) / 2;
    LL ret = tmx[nd]? calc(Mx[nd], p): -INFLL;
    if (p <= Mid)
      return max(ret, QryMx(lc, L, Mid, p));
    return max(ret, QryMx(rc, Mid + 1, R, p));
  }

  LL QryMn(int nd, int L, int R, const int& p) {
    if (L == R) return tmn[nd]? calc(Mn[nd], L): INFLL;
    int Mid = (L + R) / 2;
    LL ret = tmn[nd]? calc(Mn[nd], p): INFLL;
    if (p <= Mid)
      return min(ret, QryMn(lc, L, Mid, p));
    return min(ret, QryMn(rc, Mid + 1, R, p));
  }

  inline LL Qry(const int& p) {
    return max(QryMx(1, 1, nm, p), -QryMn(1, 1, nm, p));
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(m);
  for (int i = 1; i <= n; ++i) read(R[i].x);

  num[++nm] = 0;
  for (int opt, k, x, t, i = 1; i <= m; ++i) {
    read(t), opt = Gc();
    while (isspace(opt)) opt = Gc();
    if (opt == 'c')
      read(k), read(x), Q[i] = Ask(opt, t, k, x);
    else Q[i] = Ask(opt, t, 0, 0);
    num[++nm] = t;
  }

  nm = unique(num + 1, num + 1 + nm) - num - 1;
  for (int i = 1; i <= m; ++i) if (Q[i].opt == 'c') {
    const int& idx = Q[i].k;
    int t0 = lower_bound(num + 1, num + 1 + nm, R[idx].t) - num,
        t1 = lower_bound(num + 1, num + 1 + nm, Q[i].t) - num;
    Ln[++nl] = Line(R[idx].v, R[idx].x - R[idx].v * R[idx].t);
    SGT::Mdy(t0, t1, nl);
    R[idx].x += R[idx].v * (Q[i].t - R[idx].t);
    R[idx].v = Q[i].x, R[idx].t = Q[i].t;
  }
  for (int i = 1; i <= n; ++i) {
    int t0 = lower_bound(num + 1, num + 1 + nm, R[i].t) - num;
    Ln[++nl] = Line(R[i].v, R[i].x - R[i].v * R[i].t);
    SGT::Mdy(t0, nm, nl);
  }

  for (int i = 1; i <= m; ++i) if (Q[i].opt == 'q') {
    int t = lower_bound(num + 1, num + 1 + nm, Q[i].t) - num;
    printf("%lld\n", SGT::Qry(t));
  }
  return 0;
}

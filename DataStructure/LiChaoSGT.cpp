// UOJ #88
// DeP
#include <cstdio>
#include <cassert>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 1e5+5, MAXM = 5e5+5;
const LL INFLL = 0x3f3f3f3f3f3f3f3fLL;

struct Point {
  LL t, x, v;
} R[MAXN];

struct Line {
  LL k, b;
  Line() { k = b = 0; }
  Line(LL _k, LL _b): k(_k), b(_b) { }
} A[MAXN + MAXN];

struct Ask {
  int opt, t, k, x;
  Ask() { opt = t = k = x = 0; }
  Ask(int _opt, int _t, int _k, int _x): opt(_opt), t(_t), k(_k), x(_x) { }
} Q[MAXM + MAXN];

int n, m, nA;
int B[MAXM + MAXN], nB;

namespace SGT {
#define lc (nd<<1)
#define rc (nd<<1|1)
#define Mid ((L + R) / 2)
  const int MAXN = ::MAXN + MAXM;
  bool tagMin[MAXN << 2], tagMax[MAXN << 2];
  int datMin[MAXN << 2], datMax[MAXN << 2];

  inline LL calc(int idx, int x) { return A[idx].k * B[x] + A[idx].b; }

  void MdyMax(int nd, int L, int R, const int& opL, const int& opR, int idx) {
    assert(L <= R);
    if (opL <= L && R <= opR) {
      if (!tagMax[nd]) return tagMax[nd] = true, void( datMax[nd] = idx );
      if (calc(idx, L) <= calc(datMax[nd], L) && calc(idx, R) <= calc(datMax[nd], R)) return;
      if (calc(idx, L) >= calc(datMax[nd], L) && calc(idx, R) >= calc(datMax[nd], R))
        return void( datMax[nd] = idx );
      if (calc(idx, Mid) >= calc(datMax[nd], Mid)) swap(datMax[nd], idx);
      if (calc(idx, L) >= calc(datMax[nd], L)) MdyMax(lc, L, Mid, L, R, idx);
      else MdyMax(rc, Mid+1, R, L, R, idx);
    }
    if (opL <= Mid) MdyMax(lc, L, Mid, opL, opR, idx);
    if (opR > Mid) MdyMax(rc, Mid+1, R, opL, opR, idx);
  }

  void MdyMin(int nd, int L, int R, const int& opL, const int&opR, int idx) {
    assert(L <= R);
    if (opL <= L && R <= opR) {
      if (!tagMin[nd]) return tagMin[nd] = true, void( datMin[nd] = idx );
      if (calc(idx, L) >= calc(datMin[nd], L) && calc(idx, R) >= calc(datMin[nd], R)) return;
      if (calc(idx, L) <= calc(datMin[nd], L) && calc(idx, R) <= calc(datMin[nd], R))
        return void ( datMin[nd] = idx );
      if (calc(idx, Mid) <= calc(datMin[nd], Mid)) swap(datMin[nd], idx);
      if (calc(idx, L) <= calc(datMin[nd], L)) MdyMin(lc, L, Mid, L, R, idx);
      else MdyMin(rc, Mid+1, R, L, R, idx);
    }
    if (opL <= Mid) MdyMin(lc, L, Mid, opL, opR, idx);
    if (opR > Mid) MdyMin(rc, Mid+1, R, opL, opR, idx);
  }

  inline void Mdy(int nd, int L, int R, const int& opL, const int& opR, const int& idx) {
    MdyMax(nd, L, R, opL, opR, idx), MdyMin(nd, L, R, opL, opR, idx);
  }

  LL QryMin(int nd, int L, int R, const int& pos) {
    assert(L < R || (L == R && L == pos));
    if (L == R) return tagMin[nd]? calc(datMin[nd], L): INFLL;
    LL ret = tagMin[nd]? calc(datMin[nd], pos): INFLL;
    if (pos <= Mid) ret = min(ret, QryMin(lc, L, Mid, pos));
    else ret = min(ret, QryMin(rc, Mid+1, R, pos));
    return ret;
  }

  LL QryMax(int nd, int L, int R, const int& pos) {
    assert(L < R || (L == R && L == pos));
    if (L == R) return tagMax[nd]? calc(datMax[nd], L): -INFLL;
    LL ret = tagMax[nd]? calc(datMax[nd], pos): -INFLL;
    if (pos <= Mid) ret = max(ret, QryMax(lc, L, Mid, pos));
    else ret = max(ret, QryMax(rc, Mid+1, R, pos));
    return ret;
  }

  inline LL Qry(int nd, int L, int R, const int& pos) {
    return max(QryMax(nd, L, R, pos), -QryMin(nd, L, R, pos));
  }
#undef lc
#undef rc
#undef Mid
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif

  scanf("%d%d", &n, &m);
  for (int i = 1; i <= n; ++i) scanf("%lld", &R[i].x);
  B[++nB] = 0;
  for (int i = 1; i <= m; ++i) {
    static char opt[8]; int k = 0, x = 0, t = 0;
    scanf("%d%s", &t, opt);
    if (opt[0] == 'c') scanf("%d%d", &k, &x);
    Q[i] = Ask(opt[0], t, k, x);
    B[++nB] = t;
  }

  nB = unique(B+1, B+1+nB) - B - 1;
  for (int i = 1; i <= m; ++i) {
    if (Q[i].opt != 'c') continue;
    const int& idx = Q[i].k;
    int t0 = lower_bound(B+1, B+1+nB, R[idx].t) - B, t1 = lower_bound(B+1, B+1+nB, Q[i].t) - B;
    A[++nA] = Line(R[idx].v, R[idx].x - R[idx].v * R[idx].t);
    SGT::Mdy(1, 1, nB, t0, t1, nA);
    R[idx].x += R[idx].v * (Q[i].t - R[idx].t), R[idx].v = Q[i].x, R[idx].t = Q[i].t;
  }
  for (int i = 1; i <= n; ++i) {
    int t0 = lower_bound(B+1, B+1+nB, R[i].t) - B;
    A[++nA] = Line(R[i].v, R[i].x - R[i].v * R[i].t);
    SGT::Mdy(1, 1, nB, t0, nB, nA);
  }

  for (int i = 1; i <= m; ++i) {
    if (Q[i].opt != 'q') continue;
    printf("%lld\n", SGT::Qry(1, 1, nB, lower_bound(B+1, B+1+nB, Q[i].t) - B));
  }
  return 0;
}

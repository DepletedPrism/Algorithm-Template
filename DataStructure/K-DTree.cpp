// Luogu P4148
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

const int MAXN = 2e5 + 5;
const double alpha = 0.725;

int n, nidx, root;
int x1, y1, x2, y2;
struct Point { int x, y, val; } A[MAXN];

namespace KDT {
  int ch[2][MAXN], d[MAXN], g[MAXN], t;
  int size[MAXN], datSum[MAXN], mnL[MAXN], mxR[MAXN], mnD[MAXN], mxU[MAXN];

  inline bool cmp1(const int& a, const int& b) { return A[a].x < A[b].x; }
  inline bool cmp2(const int& a, const int& b) { return A[a].y < A[b].y; }

  inline void maintain(int nd) {
    const int &lc = ch[0][nd], &rc = ch[1][nd];
    size[nd] = 1, datSum[nd] = A[nd].val;
    mnL[nd] = mxR[nd] = A[nd].x, mnD[nd] = mxU[nd] = A[nd].y;
    if (lc) {
      size[nd] += size[lc], datSum[nd] += datSum[lc];
      mnL[nd] = min(mnL[nd], mnL[lc]), mxR[nd] = max(mxR[nd], mxR[lc]);
      mnD[nd] = min(mnD[nd], mnD[lc]), mxU[nd] = max(mxU[nd], mxU[lc]);
    }
    if (rc) {
      size[nd] += size[rc], datSum[nd] += datSum[rc];
      mnL[nd] = min(mnL[nd], mnL[rc]), mxR[nd] = max(mxR[nd], mxR[rc]);
      mnD[nd] = min(mnD[nd], mnD[rc]), mxU[nd] = max(mxU[nd], mxU[rc]);
    }
  }

  int build(int L, int R) {
    if (L > R) return 0;
    int Mid = (L + R) / 2;
    double avx = 0, avy = 0, x = 0, y = 0;
    for (int i = L; i <= R; ++i) avx += A[g[i]].x, avy += A[g[i]].y;
    avx /= (R - L + 1.0), avy /= (R - L + 1.0);
    for (int i = L; i <= R; ++i) {
      x += (A[g[i]].x - avx) * (A[g[i]].x - avx);
      y += (A[g[i]].y - avy) * (A[g[i]].y - avy);
    }
    if (x > y)
      nth_element(g+L, g+Mid, g+R+1, cmp1), d[g[Mid]] = 1;
    else
      nth_element(g+L, g+Mid, g+R+1, cmp2), d[g[Mid]] = 2;
    ch[0][g[Mid]] = build(L, Mid-1), ch[1][g[Mid]] = build(Mid + 1, R);
    return maintain(g[Mid]), g[Mid];
  }

  void dfs(int nd) {
    if (!nd) return;
    if (ch[0][nd]) dfs(ch[0][nd]);
    g[++t] = nd;
    if (ch[1][nd]) dfs(ch[1][nd]);
  }
  void rebuild(int& nd) { t = 0, dfs(nd), nd = build(1, t); }
  inline bool canrb(int nd) {
    return alpha * size[nd] <= max(size[ch[0][nd]], size[ch[1][nd]]);
  }

  void Ins(int& nd, const int& pos) {
    if (!nd) { nd = pos, maintain(nd); return; }
    if (d[nd] == 1) {
      if (A[pos].x <= A[nd].x) Ins(ch[0][nd], pos);
      else Ins(ch[1][nd], pos);
    } else {
      if (A[pos].y <= A[nd].y) Ins(ch[0][nd], pos);
      else Ins(ch[1][nd], pos);
    }
    maintain(nd);
    if (canrb(nd)) rebuild(nd);
  }

  int Qry(int nd) {
    if (!nd || x1 > mxR[nd] || x2 < mnL[nd] || y1 > mxU[nd] || y2 < mnD[nd])
      return 0;
    if (x1 <= mnL[nd] && mxR[nd] <= x2 && y1 <= mnD[nd] && mxU[nd] <= y2)
      return datSum[nd];
    if (x1 <= A[nd].x && A[nd].x <= x2 && y1 <= A[nd].y && A[nd].y <= y2)
      return Qry(ch[0][nd]) + Qry(ch[1][nd]) + A[nd].val;
    return Qry(ch[0][nd]) + Qry(ch[1][nd]);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("11.in", "r", stdin);
#endif
  read(n);
  int opt, val, lstans = 0;
  while (read(opt), opt != 3) {
    if (opt == 1) {
      read(x1), read(y1), read(val);
      x1 ^= lstans, y1 ^= lstans, val ^= lstans;
      A[++nidx] = (Point){ x1, y1, val };
      KDT::Ins(root, nidx);
    }
    if (opt == 2) {
      read(x1), read(y1), read(x2), read(y2);
      x1 ^= lstans, y1 ^= lstans, x2 ^= lstans, y2 ^= lstans;
      printf("%d\n", lstans = KDT::Qry(root));
    }
  }
  return 0;
}

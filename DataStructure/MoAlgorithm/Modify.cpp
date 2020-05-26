// Luogu P1903
// DeP
#include <cmath>
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

const int MAXN = 133335, MAXM = MAXN << 1;

int n, m, qidx, midx, Block;
int A[MAXN], B[MAXM], nB;

struct Ask {
  int idx, L, R, t;
  Ask() = default;
  Ask(int _i, int _L, int _R, int _t): idx(_i), L(_L), R(_R), t(_t) { }
  bool operator < (const Ask& rhs) const {
    return (L / Block == rhs.L / Block)?
      ((R / Block == rhs.R / Block)? t < rhs.t: R < rhs.R): L < rhs.L;
  }
} Q[MAXN];

struct Chg {
  int t, p, c;
  Chg() = default;
  Chg(int _t, int _p, int _c): t(_t), p(_p), c(_c) { }
} M[MAXN];

int Ans[MAXN];

namespace Mo {
  int cnt[MAXM], now;

  inline void Add(const int& c) { now += !cnt[c]++; }
  inline void Del(const int& c) { now -= !(--cnt[c]); }

  inline void Mdy(const int& L, const int& R, const int& t) {
    if (L <= M[t].p && M[t].p <= R) Del(A[M[t].p]), Add(M[t].c);
    swap(A[M[t].p], M[t].c);
  }

  void solve() {
    Block = pow(n, 2.0 / 3.0), sort(Q + 1, Q + 1 + qidx);
    int L = Q[1].L, R = L - 1, t = 0;
    for (int i = 1; i <= qidx; ++i) {
      const Ask& a = Q[i];
      while (a.L < L) Add(A[--L]);
      while (a.R > R) Add(A[++R]);
      while (a.L > L) Del(A[L++]);
      while (a.R < R) Del(A[R--]);
      while (a.t < t) Mdy(L, R, t--);
      while (a.t > t) Mdy(L, R, ++t);
      Ans[a.idx] = now;
    }
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(m);
  for (int i = 1; i <= n; ++i)
    read(A[i]), B[++nB] = A[i];
  for (int opt, x, y, i = 1; i <= m; ++i) {
    opt = Gc();
    while (isspace(opt)) opt = Gc();
    read(x), read(y);
    switch (opt) {
      case 'Q': ++qidx, Q[qidx] = Ask(qidx, x, y, midx); break;
      case 'R': ++midx, M[midx] = Chg(midx, x, y), B[++nB] = y; break;
      default: fprintf(stderr, "ERR\n");
    }
  }

  sort(B + 1, B + 1 + nB);
  nB = unique(B + 1, B + 1 + nB) - B - 1;
  for (int i = 1; i <= n; ++i)
    A[i] = lower_bound(B + 1, B + 1 + nB, A[i]) - B;
  for (int i = 1; i <= midx; ++i)
    M[i].c = lower_bound(B + 1, B + 1 + nB, M[i].c) - B;
  Mo::solve();

  for (int i = 1; i <= qidx; ++i)
    printf("%d\n", Ans[i]);
  return 0;
}

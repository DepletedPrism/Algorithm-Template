// Luogu P5906
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
using IO::read;

const int MAXN = 2e5 + 5;

int n, q, Block;
int A[MAXN], B[MAXN], nB;

int Ans[MAXN];

struct Ask {
  int idx, L, R;
  Ask() = default;
  Ask(int _i, int _L, int _R): idx(_i), L(_L), R(_R) { }
  bool operator < (const Ask& rhs) const {
    return ((L - 1) / Block == (rhs.L - 1) / Block)?  R < rhs.R: L < rhs.L;
  }
} Q[MAXN];

namespace Mo {
  int Idx[MAXN], BR[MAXN], Bcnt;
  int pre[MAXN], suf[MAXN], lst[MAXN], C[MAXN], nC;

  void solve() {
    Block = sqrt(n), Bcnt = n / Block + (n % Block != 0);
    for (int i = 1; i <= n; ++i)
      Idx[i] = (i - 1) / Block + 1;
    for (int i = 1; i <= Bcnt; ++i) BR[i] = i * Block;
    BR[Bcnt] = n;

    sort(Q + 1, Q + 1 + q);
    for (int p = 1, i = 1; i <= Bcnt; ++i) {
      nC = 0;
      int L = BR[i] + 1, R = BR[i], ans = 0;
      for ( ; p <= q && Idx[Q[p].L] == i; ++p) {
        const Ask& a = Q[p];
        if (Idx[a.R] == i) {
          for (int j = a.L; j <= a.R; ++j) {
            if (lst[A[j]] > 0)
              Ans[a.idx] = max(Ans[a.idx], j - lst[A[j]]);
            else lst[A[j]] = j;
          }
          for (int j = a.L; j <= a.R; ++j) lst[A[j]] = 0;
          continue;
        }
        int nxtL = L;
        while (R < a.R) {
          ++R, suf[A[R]] = R;
          if (pre[A[R]] > 0)
            ans = max(ans, R - pre[A[R]]);
          else pre[A[R]] = R, C[++nC] = A[R];
        }
        Ans[a.idx] = ans;
        while (nxtL > a.L) {
          --nxtL;
          if (suf[A[nxtL]] > 0)
            Ans[a.idx] = max(Ans[a.idx], suf[A[nxtL]] - nxtL);
          else suf[A[nxtL]] = nxtL;
        }
        while (nxtL < L) {
          if (suf[A[nxtL]] == nxtL) suf[A[nxtL]] = 0;
          ++nxtL;
        }
      }
      for (int j = 1; j <= nC; ++j) suf[C[j]] = pre[C[j]] = 0;
    }
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n);
  for (int i = 1; i <= n; ++i)
    read(A[i]), B[++nB] = A[i];
  read(q);
  for (int L, R, i = 1; i <= q; ++i)
    read(L), read(R), Q[i] = Ask(i, L, R);

  sort(B + 1, B + 1 + nB);
  nB = unique(B + 1, B + 1 + nB) - B - 1;
  for (int i = 1; i <= n; ++i)
    A[i] = lower_bound(B + 1, B + 1 + nB, A[i]) - B;
  Mo::solve();

  for (int i = 1; i <= q; ++i) printf("%d\n", Ans[i]);
  return 0;
}

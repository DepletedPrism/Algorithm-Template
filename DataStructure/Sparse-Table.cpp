// Luogu P3865
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

const int MAXN = 1e5 + 5, LOG = 18;

int n, q;
int A[MAXN];

namespace ST {
  int lg2[MAXN], Mx[LOG][MAXN];

  void build() {
    for (int i = 1; i <= n; ++i) Mx[0][i] = A[i];
    for (int j = 1; (1 << j) <= n; ++j)
      for (int i = 1; i + (1 << j) - 1 <= n; ++i)
        Mx[j][i] = max(Mx[j - 1][i], Mx[j - 1][i + (1 << (j-1))]);
    lg2[1] = 0;
    for (int i = 2; i <= n; ++i) lg2[i] = lg2[i / 2] + 1;
  }

  inline int Qry(int L, int R) {
    int k = lg2[R - L + 1];
    return max(Mx[k][L], Mx[k][R - (1 << k) + 1]);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(q);
  for (int i = 1; i <= n; ++i) read(A[i]);

  ST::build();
  for (int L, R; q; --q)
    read(L), read(R), printf("%d\n", ST::Qry(L, R));
  return 0;
}

// LOJ #100
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
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
const int MAXN = 5e2 + 5, P = 1e9 + 7;

int n, p, m;

struct Matrix {
  int g[MAXN][MAXN];
  Matrix() { memset(g, 0, sizeof g); }
  int* operator [](const int& i) { return g[i]; }
  const int* operator [](const int& i) const { return g[i]; }
  Matrix operator * (const Matrix& rhs) const {
    Matrix ret;
    for (int i = 0; i < n; ++i)
      for (int k = 0; k < p; ++k)
        for (int j = 0; j < m; ++j)
          ret[i][j] = (ret[i][j] + (LL) g[i][k] * rhs[k][j] % P) % P;
    return ret;
  }
} A, B, R;

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(p), read(m);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < p; ++j) read(A[i][j]);
  for (int i = 0; i < p; ++i)
    for (int j = 0; j < m; ++j) read(B[i][j]);

  R = A * B;

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
      printf("%d%c", (R[i][j] + P) % P, " \n"[j == m - 1]);
  return 0;
}

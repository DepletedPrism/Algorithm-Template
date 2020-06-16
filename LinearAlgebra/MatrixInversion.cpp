// Luogu P4783
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

typedef long long LL;
const int MAXN = 4e2 + 5, P = 1e9 + 7;

int fpow(int pw, int b) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = (LL) ret * pw % P;
    pw = (LL) pw * pw % P, b >>= 1;
  }
  return ret;
}

int n;
int A[MAXN][MAXN << 1];

bool Inv() {
  for (int i = 1; i <= n; ++i) A[i][n + i] = 1;
  for (int i = 1; i <= n; ++i) {
    int r = i;
    for (int j = i + 1; j <= n; ++j)
      if (A[r][i] < A[j][i]) r = j;
    if (A[r][i] == 0) return false;
    if (r != i) swap(A[r], A[i]);
    int iv = fpow(A[i][i], P - 2);
    for (int j = 1; j <= n; ++j) if (i != j) {
      int d = (LL) iv * A[j][i] % P;
      for (int k = i; k <= n + n; ++k)
        A[j][k] = (A[j][k] - (LL) d * A[i][k] % P + P) % P; 
    }
    for (int j = 1; j <= n + n; ++j)
      A[i][j] = (LL) A[i][j] * iv % P;
  }
  return true;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n);
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j) read(A[i][j]);

  if (!Inv())
    puts("No Solution");
  else {
    for (int i = 1; i <= n; ++i)
      for (int j = n + 1; j <= n + n; ++j)
        printf("%d%c", A[i][j], " \n"[j == n + n]);
  }
  return 0;
}

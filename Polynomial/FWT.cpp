// Luogu P4717
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
const int LOG = 17, MAXN = 1 << LOG | 1;
const int P = 998244353, iv2 = 499122177;

namespace FWT {
  void Or(int* f, const int& n, const int& type) {
    for (int Mid = 1; 2 * Mid <= n; Mid <<= 1)
      for (int i = 0; i < n; i += Mid << 1)
        for (int j = 0; j < Mid; ++j)
          f[i+j+Mid] = (f[i+j+Mid] + (LL) type * f[i+j] % P + P) % P;
  }

  void And(int* f, const int& n, const int& type) {
    for (int Mid = 1; 2 * Mid <= n; Mid <<= 1)
      for (int i = 0; i < n; i += Mid << 1)
        for (int j = 0; j < Mid; ++j)
          f[i+j] = (f[i+j] + (LL) type * f[i+j+Mid] % P + P) % P;
  }

  void Xor(int* f, const int& n, const int& type) {
    for (int Mid = 1; 2 * Mid <= n; Mid <<= 1)
      for (int i = 0; i < n; i += Mid << 1)
        for (int j = 0; j < Mid; ++j) {
          int f0 = f[i+j], f1 = f[i+j+Mid];
          f[i+j] = (f0 + f1) % P, f[i+j+Mid] = (f0 - f1 + P) % P;
          if (type < 0) {
            f[i+j] = (LL) iv2 * f[i+j] % P;
            f[i+j+Mid] = (LL) iv2 * f[i+j+Mid] % P;
          }
        }
  }
}

int n, m;
int f[MAXN], g[MAXN];

int A[MAXN], B[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(m), n = 1 << m;
  for (int i = 0; i < n; ++i) read(f[i]);
  for (int j = 0; j < n; ++j) read(g[j]);

  for (int i = 0; i < n; ++i)
    A[i] = f[i], B[i] = g[i];
  FWT::Or(A, n, 1), FWT::Or(B, n, 1);
  for (int i = 0; i < n; ++i) A[i] = (LL) A[i] * B[i] % P;
  FWT::Or(A, n, -1);
  for (int i = 0; i < n; ++i)
    printf("%d%c", A[i], " \n"[i == n - 1]);

  for (int i = 0; i < n; ++i)
    A[i] = f[i], B[i] = g[i];
  FWT::And(A, n, 1), FWT::And(B, n, 1);
  for (int i = 0; i < n; ++i) A[i] = (LL) A[i] * B[i] % P;
  FWT::And(A, n, -1);
  for (int i = 0; i < n; ++i)
    printf("%d%c", A[i], " \n"[i == n - 1]);

  for (int i = 0; i < n; ++i)
    A[i] = f[i], B[i] = g[i];
  FWT::Xor(A, n, 1), FWT::Xor(B, n, 1);
  for (int i = 0; i < n; ++i) A[i] = (LL) A[i] * B[i] % P;
  FWT::Xor(A, n, -1);
  for (int i = 0; i < n; ++i)
    printf("%d%c", A[i], " \n"[i == n - 1]);
  return 0;
}

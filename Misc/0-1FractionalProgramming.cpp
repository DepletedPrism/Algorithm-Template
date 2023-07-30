// LOJ #149
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
    while (isdigit(ch)) x = x * 10 - '0' + ch, ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

const int MAXN = 1e5 + 5;
const double EPS = 1e-9;

inline int dcmp(const double& p) {
  return fabs(p) < EPS? 0: (p < 0? -1: 1);
}

int n, K;
int A[MAXN], B[MAXN];

bool check(double Mid) {
  double t[MAXN];
  for (int i = 1; i <= n; ++i) t[i] = A[i] - Mid * B[i];
  sort(t + 1, t + 1 + n);
  double ret = 0;
  for (int i = n - K + 1; i <= n; ++i) ret += t[i];
  return dcmp(ret) >= 0;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(K);
  double L = 0.0, R = 0.0;
  for (int i = 1; i <= n; ++i) read(A[i]), R += A[i];
  for (int i = 1; i <= n; ++i) read(B[i]);

  double ans = -1.0;
  while (dcmp(R - L) > 0) {
    double Mid = (L + R) / 2.0;
    if (check(Mid)) ans = Mid, L = Mid; else R = Mid;
  }

  printf("%.4lf\n", ans);
  return 0;
}

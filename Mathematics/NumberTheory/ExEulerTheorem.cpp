// Luogu P5091
// DeP
#include <cctype>
#include <cstdio>
using namespace std;

typedef long long LL;

int fpow(int pw, int b, int m) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = (LL) ret * pw % m;
    pw = (LL) pw * pw % m, b >>= 1;
  }
  return ret % m;
}

int Phi(int n) {
  int ret = n;
  for (int d = 2; d * d <= n; ++d) if (n % d == 0) {
    ret = ret / d * (d - 1);
    while (n % d == 0) n /= d;
  }
  if (n > 1) ret = ret / n * (n - 1);
  return ret;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  static int a, m, b = 0;
  scanf("%d%d", &a, &m);

  int phi = Phi(m), flag = false, ch = getchar();
  while (!isdigit(ch)) ch = getchar();
  while (isdigit(ch)) {
    b = flag? (b * 10LL + ch - '0') % phi: (b * 10 + ch - '0');
    flag |= (b >= phi), ch = getchar();
  }

  printf("%d\n", flag? fpow(a, b + phi, m): fpow(a, b % phi, m));
  return 0;
}

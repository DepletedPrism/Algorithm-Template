// Luogu P5491
// DeP
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

typedef long long LL;

int fpow(int pw, int b, int p) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = (LL) ret * pw % p;
    pw = (LL) pw * pw % p, b >>= 1;
  }
  return ret;
}

namespace Cipolla {
  struct Complex {
    int x, y;
    Complex(int _x, int _y): x(_x), y(_y) { }
  };

  inline Complex Mul(Complex A, Complex B, int i2, int p) {
    return Complex(((LL) A.x * B.x + (LL) i2 * A.y % p * B.y % p) % p,
        ((LL) A.x * B.y + (LL) A.y * B.x) % p);
  }
  Complex Cfpow(Complex pw, int b, int i2, int p) {
    Complex ret(1, 0);
    while (b > 0) {
      if (b & 1) ret = Mul(ret, pw, i2, p);
      pw = Mul(pw, pw, i2, p), b >>= 1;
    }
    return ret;
  }

  inline int Sqrt(int n, int p) {
    srand(time(nullptr));
    if (!n) return 0;
    if (fpow(n, (p - 1) / 2, p) != 1) return -1;
    while (true) {
      int a = rand() % p, i2 = (((LL) a * a - n) % p + p) % p;
      if (!a || fpow(i2, (p - 1) / 2, p) == 1) continue;
      int x = Cfpow(Complex(a, 1), (p + 1) / 2, i2, p).x;
      return min(x, p - x);
    }
    return -1;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  int Ti;
  scanf("%d", &Ti);
  for (int n, p; Ti; --Ti) {
    scanf("%d%d", &n, &p);
    int x = Cipolla::Sqrt(n, p);
    if (x == -1)
      puts("Hola!");
    else if (x == p - x || x == 0)
      printf("%d\n", x);
    else
      printf("%d %d\n", x, p - x);
  }
  return 0;
}

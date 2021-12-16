// LOJ #143
// DeP
#include <ctime>
#include <cctype>
#include <cstdio>
#include <cstdlib>
using namespace std;

namespace IO {
  const int MAXSIZE = 1 << 18 | 1;
  char buf[MAXSIZE], *p1, *p2;

  inline int Gc() {
    return p1 == p2 &&
      (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++;
  }
  template<typename T> inline bool read(T& x) {
    x = 0; int f = 0, ch = Gc();
    while (!isdigit(ch)) { if (ch == EOF) return false; f |= ch == '-', ch = Gc(); }
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
    if (f) x = -x;
    return true;
  }
}
using IO::read; using IO::Gc;

typedef long long LL;

namespace MillerRabin {
  inline LL qmul(LL a, LL b, const LL& m) {
    a %= m, b %= m;
    return (a * b - LL((long double) a / m * b) * m + m) % m;
  }

  LL fpow(LL base, LL b, LL m) {
    LL ret = 1;
    while (b > 0) {
      if (b & 1) ret = qmul(ret, base, m);
      base = qmul(base, base, m), b >>= 1;
    }
    return ret;
  }

  bool isPrime(LL p) {
    if (p == 2) return true;
    if (p < 2 || (~p & 1)) return false;
    LL k = p - 1, d = 0;
    while (~k & 1) k >>= 1, ++d;
    for (int i = 0; i < 8; ++i) {
      LL lst = fpow(rand() % (p - 2) + 2, k, p), cur = lst;
      if (lst == 1 || lst == p - 1) continue;
      for (int j = 0; j < d; ++j, lst = cur) {
        cur = qmul(cur, cur, p);
        if (cur == 1 && lst != 1 && lst != p - 1)
          return false;
      }
      if (cur != 1) return false;
    }
    return true;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  srand(time(nullptr));
  static LL x;
  while (read(x))
    putchar(MillerRabin::isPrime(x)? 'Y': 'N'), putchar('\n');
  return 0;
}

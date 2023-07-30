// Luogu P4718
// DeP
#include <ctime>
#include <cmath>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

namespace IO {
  const int MAXSIZE = 1 << 18 | 1;
  char buf[MAXSIZE], *p1, *p2;

  inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
  template<typename T> inline void read(T& x) {
    x = 0; int f = 0, ch = Gc();
    while (!isdigit(ch)) f |= ch == '-', ch = Gc();
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

typedef long long LL;
const int S = 10;

LL gcd(LL a, LL b) { return !b? a: gcd(b, a % b); }

inline LL qmul(LL a, LL b, LL mod) {
  // ???
  return (a * b - LL((long double)a / mod * b) * mod + mod) % mod;
}

inline LL smul(LL a, LL b, LL m) {
  a = (a % m + m) % m, b = (b % m + m) % m;
  return (__int128(a) * b - __int128((long double) a / m * b) * m + m) % m;
}

inline LL fpow(LL base, LL b, LL m) {
  LL ret = 1;
  while (b > 0) {
    if (b & 1) ret = qmul(base, ret, m);
    base = qmul(base, base, m), b >>= 1;
  }
  return ret % m;
}

bool isPrime(LL p) {
  if (p == 2) return true;
  if (p < 2 || !(p & 1)) return false;
  for (int s = 0; s < S; ++s) {
    LL a = rand() % (p-1) + 1;
    if (fpow(a, p-1, p) != 1) return false;
    LL t, k = p - 1;
    while (!(k & 1)) {
      k >>= 1, t = fpow(a, k, p);
      if (t != 1 && t != p-1) return false;
      if (t == p-1) break;
    }
  }
  return true;
}

LL n, maxFact;

void PollardRho(LL x) {
  if (isPrime(x)) return void( maxFact = max(maxFact, x) );
  LL t1 = rand() % (x-1) + 1, B = rand() % (x-1) + 1;
  LL t, p = 1, t2 = (qmul(t1, t1, x) + B) % x;
  int i = 0;
  while (t1 != t2) {
    ++i, p = qmul(p, abs(t1 - t2), x);
    if (!p) {
      t = gcd(abs(t1 - t2), x);
      if (t != 1 && t != x) return PollardRho(t), PollardRho(x / t);
    }
    if (i % 127 == 0) {
      p = gcd(p, x);
      if (p != 1 && p != x) return PollardRho(p), PollardRho(x / p);
      p = 1;
    }
    t1 = (qmul(t1, t1, x) + B) % x;
    t2 = (qmul(t2, t2, x) + B) % x, t2 = (qmul(t2, t2, x) + B) % x;
  }
  p = gcd(p, x);
  if (p != 1 && p != x) return PollardRho(p), PollardRho(x / p);
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  srand(time(nullptr));
  int Ti; read(Ti);
  while (Ti--) {
    read(n);
    if (isPrime(n)) puts("Prime");
    else {
      maxFact = 0;
      while (!maxFact) PollardRho(n);
      printf("%lld\n", maxFact);
    }
  }
  return 0;
}

// Luogu P4720
// DeP
#include <iostream>
using namespace std;

typedef long long LL;
const int MAXM = 23;

LL qmul(LL a, LL b, LL m) {
  a = (a % m + m) % m, b = (b % m + m) % m;
  return (a * b - LL((long double) a / m * b) * m + m) % m;
}

LL fpow(LL base, LL b, LL m) {
  LL ret = 1;
  while (b > 0) {
    if (b & 1) ret = qmul(ret, base, m);
    base = qmul(base, base, m), b >>= 1;
  }
  return ret % m;
}

void exgcd(LL a, LL b, LL& x, LL& y) {
  if (!b) x = 1, y = 0;
  else exgcd(b, a % b, y, x), y -= a / b * x;
}
LL inv(LL v, LL m) {
  static LL x, y;
  return exgcd(v, m, x, y), (x % m + m) % m;
}

LL CRT(LL* a, LL* m, int n) {
  LL M = 1, ret = 0;
  for (int i = 1; i <= n; ++i) M *= m[i];
  for (int i = 1; i <= n; ++i) {
    LL x, y, w = M / m[i];
    exgcd(w, m[i], x, y);
    ret = (ret + qmul( qmul(w, a[i], M), x, M)) % M;
  }
  return (ret % M + M) % M;
}

LL calc(LL n, LL d, LL p) {
  if (!n) return 1;
  LL s = 1;
  for (LL i = 1; i <= p; ++i)
    if (i % d) s = qmul(s, i, p);
  s = fpow(s, n / p, p);
  for (LL i = n / p * p + 1; i <= n; ++i)
    if (i % d) s = qmul(s, i, p);
  return qmul(s, calc(n / d, d, p), p);
}

LL MultiLucas(LL n, LL m, LL d, LL p) {
  int c = 0;
  for (LL i = n; i; i /= d) c += i / d;
  for (LL i = m; i; i /= d) c -= i / d;
  for (LL i = n-m; i; i /= d) c -= i / d;
  return qmul( qmul( qmul(fpow(d, c, p), calc(n, d, p), p), inv(calc(m, d, p), p), p), 
      inv(calc(n-m, d, p), p), p );
}

LL ExLucas(LL n, LL m, LL p) {
  static LL fact[MAXM], a[MAXM];
  int idx = 0;
  for (LL d = 2; d * d <= p; ++d) if (p % d == 0) {
    fact[++idx] = 1;
    while (p % d == 0) fact[idx] *= d, p /= d;
    a[idx] = MultiLucas(n, m, d, fact[idx]);
  }
  if (p > 1) fact[++idx] = p, a[idx] = MultiLucas(n, m, p, p);
  return CRT(a, fact, idx);
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  LL n, m, p;
  cin >> n >> m >> p;
  cout << ExLucas(n, m, p) << endl;
  return 0;
}

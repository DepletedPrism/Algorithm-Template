// LOJ #143
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
using i128 = __int128;

namespace MR {
  inline LL qmul(LL a, LL b, const LL& m) {
    a %= m, b %= m;
    return (a * b - LL((long double) a / m * b) * m + m) % m;
  }

  LL fpow(LL b, LL m, LL p) {
    LL ret = 1;
    while (m > 0) {
      if (m & 1) ret = (i128) ret * b % p;
      b = (i128) b * b % p, m >>= 1;
    }
    return ret;
  }

  bool isprime(LL p) {
    if (p == 2) return true;
    if (p < 2 || (~p & 1)) return false;
    LL k = p - 1, d = 0;
    while (~k & 1) k >>= 1, ++d;
    srand(time(nullptr));
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
  ios::sync_with_stdio(false), cin.tie(nullptr);
  for (LL x; cin >> x; )
    cout << (MR::isprime(x)? 'Y': 'N') << '\n';
  return 0;
}

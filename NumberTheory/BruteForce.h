// brute force algorithm in number theory
// DeP
#include <vector>
using namespace std;

typedef long long LL;

namespace Num {
  int gcd(int a, int b) {
    return !b? a: gcd(b, a % b);
  }
  void exgcd(int a, int b, int& d, int& x, int& y) {
    if (!b) d = a, x = 1, y = 0;
    else exgcd(b, a % b, d, y, x), y -= a / b * x;
  }

  int phi(int n) {
    int ret = n;
    for (int d = 2; d * d <= n; ++d) if (n % d == 0) {
      ret = ret / d * (d - 1);
      while (n % d == 0) n /= d;
    }
    if (n > 1) ret = ret / n * (n - 1);
    return ret;
  }

  vector<int> factor(int n) {
    vector<int> ret;
    for (int d = 2; d * d <= n; ++d) if (n % d == 0) {
      while (n % d == 0)
        ret.push_back(d), n /= d;
    }
    if (n > 1) ret.push_back(n);
    return ret;
  }
}

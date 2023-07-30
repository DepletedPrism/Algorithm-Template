// brute force algorithm in number theory
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;

namespace NT {
  int gcd(int a, int b) {
    return !b? a: gcd(b, a % b);
  }
  int lcm(int a, int b) {
    return a / gcd(a, b) * b;
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

  void euler(int n) {
    vector<int> npr(n), pr;
    npr[1] = true;
    for (int i = 2; i < n; ++i) {
      if (!npr[i]) pr.push_back(i);
      for (int j = 0; j < (int) pr.size() && i * pr[j] < n; ++j) {
        npr[i * pr[j]] = true;
        if (i % pr[j] == 0) break;
      }
    }
  }
}

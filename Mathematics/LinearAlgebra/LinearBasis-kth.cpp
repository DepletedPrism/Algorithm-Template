// LOJ #114
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int LOG = 51;

struct Basis {
  vector<LL> p, g;
  Basis() { p.resize(LOG); }
  void ins(LL x) {
    for (int i = LOG - 1; i >= 0; --i) {
      if ((x >> i) & 1) {
        if (!p[i]) {
          for (int j = i - 1; j >= 0; --j)
            if ((x >> j) & 1) x ^= p[j];
          for (int j = i + 1; j < LOG; ++j)
            if ((p[j] >> i) & 1) p[j] ^= x;
          p[i] = x;
          break;
        }
        x ^= p[i];
      }
    }
  }
  void build() {
    for  (int i = 0; i < LOG; ++i)
      if (p[i]) g.push_back(p[i]);
  }
  LL kth(int n, LL k) {
    int m = g.size();
    k -= (m < n);
    if (k >= (1LL << m))
      return -1;
    LL ret = 0;
    for (int i = 0; i < m; ++i)
      if ((k >> i) & 1) ret ^= g[i];
    return ret;
  }
};

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, q;
  cin >> n;
  Basis b;
  for (int i = 0; i < n; ++i) {
    LL x;
    cin >> x, b.ins(x);
  }
  b.build();
  cin >> q;
  for (LL k; q > 0; --q)
    cin >> k, cout << b.kth(n, k) << '\n';
  return 0;
}

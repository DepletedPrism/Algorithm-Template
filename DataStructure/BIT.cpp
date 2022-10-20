// LOJ #132
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;

namespace BIT {
  vector<LL> C1, C2;

  inline int lowbit(int x) { return x & -x; }
  inline void init(int n) { // 1-index
    C1.resize(n + 1), C2.resize(n + 1);
  }

  void mdy(int p, LL d) {
    int n = C1.size();
    for (int i = p; i < n; i += lowbit(i))
      C1[i] += d, C2[i] += d * p;
  }
  inline void mdy(int l, int r, LL d) {
    mdy(l, d), mdy(r + 1, -d);
  }

  LL qry(int p) {
    LL ret = 0;
    for (int i = p; i > 0; i -= lowbit(i))
      ret += C1[i] * (p + 1) - C2[i];
    return ret;
  }
  inline LL qry(int l, int r) {
    return qry(r) - qry(l - 1);
  }
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  vector<int> A(n + 1);
  for (int i = 1; i <= n; ++i)
    cin >> A[i];
  BIT::init(n);
  for (int i = 1; i <= n; ++i)
    BIT::mdy(i, A[i] - A[i - 1]);
  for (int opt, l, r, x; q > 0; --q) {
    cin >> opt >> l >> r;
    switch (opt) {
      case 1: cin >> x, BIT::mdy(l, r, x); break;
      case 2: cout << BIT::qry(l, r) << '\n'; break;
    }
  }
  return 0;
}

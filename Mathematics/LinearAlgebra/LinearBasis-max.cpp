// Luogu P3812
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int LOG = 50;

struct Basis {
  vector<LL> p;
  Basis() { p.resize(LOG); }
  void ins(LL x) {
    for (int i = LOG - 1; i >= 0; --i)
      if ((x >> i) & 1) {
        if (!p[i]) { p[i] = x; break; }
        x ^= p[i];
      }
  }
  LL qry() {
    LL ret = 0;
    for (int i = LOG - 1; i >= 0; --i)
      if (!((ret >> i) & 1)) ret ^= p[i];
    return ret;
  }
};

int main() {
  int n;
  cin >> n;
  Basis b;
  for (LL x; n > 0; --n)
    cin >> x, b.ins(x);
  cout << b.qry() << '\n';
  return 0;
}

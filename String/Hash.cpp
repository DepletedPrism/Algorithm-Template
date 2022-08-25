// Luogu P3805
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;

struct Hash {
  static constexpr int B = 19260817, P1 = 1e9 + 9, P2 = 998244353;
  int h1, h2;
  Hash() { h1 = h2 = 0; }
  Hash(int _h1, int _h2): h1(_h1), h2(_h2) { }
  Hash(Hash h, int ch) {
    h1 = ((LL) h.h1 * B % P1 + ch) % P1, h2 = ((LL) h.h2 * B % P2 + ch) % P2;
  }
  Hash operator + (const Hash &rhs) const {
    return Hash((h1 + rhs.h1) % P1, (h2 + rhs.h2) % P2);
  }
  Hash operator - (const Hash &rhs) const {
    return Hash((h1 - rhs.h1 + P1) % P1, (h2 - rhs.h2 + P2) % P2);
  }
  Hash operator * (const Hash &rhs) const {
    return Hash((LL) h1 * rhs.h1 % P1, (LL) h2 * rhs.h2 % P2);
  }
  bool operator < (const Hash &rhs) const {
    return h1 < rhs.h1 || (h1 == rhs.h1 && h2 < rhs.h2);
  }
  bool operator == (const Hash &rhs) const {
    return !(*this < rhs) && !(rhs < *this);
  }
  bool operator != (const Hash &rhs) const {
    return *this < rhs || rhs < *this;
  }
};

// h(l, r) = h(r) -  h(l-1) * B^{r-l+1}
Hash part(const vector<Hash>& h, int l, int r) { // [l, r]
  static vector<Hash> pw{Hash(1, 1)};
  if ((int) pw.size() <= r - l + 1) {
    int st = pw.size();
    pw.resize(r - l + 2);
    for (int i = st; i <= r - l + 1; ++i)
      pw[i] = Hash(pw[i - 1], 0);
  }
  return (l > 0)? h[r] - h[l - 1] * pw[r - l + 1]: h[r];
}
vector<Hash> build(const string& s) {
  int n = s.size();
  vector<Hash> f(n);
  f[0] = Hash(s[0], s[0]);
  for (int i = 1; i < n; ++i)
    f[i] = Hash(f[i - 1], s[i]);
  return f;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  string s;
  cin >> s;
  int n = s.size();
  auto f = build(s);
  reverse(s.begin(), s.end());
  auto g = build(s);
  int r = 0, mxr = 0;
  for (int i = 0; i < n; ++i) {
    r = min(r + 2, i + 1);
    while (part(f, i - r + 1, i) != part(g, n - 1 - i, n - 1 - (i - r + 1)))
      --r;
    if (r > mxr) mxr = r;
  }
  cout << mxr << '\n';
  return 0;
}

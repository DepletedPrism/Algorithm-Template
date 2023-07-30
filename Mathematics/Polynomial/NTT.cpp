// UOJ #34
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int P = 998244353, G = 3;

template<typename T> T fpow(T b, int m) {
  T ret = 1;
  for (; m > 0; m >>= 1, b *= b)
    if (m & 1) ret *= b;
  return ret;
}

int norm(int x) {
  if (x < 0)
    return x + P;
  if (x >= P)
    return x - P;
  return x;
}

struct Mint {
  int x;
  Mint(int _x = 0): x(norm(_x)) { }
  Mint inv() const {
    assert(x != 0);
    return fpow(*this, P - 2);
  }
  Mint& operator += (const Mint& rhs) {
    x = norm(x + rhs.x);
    return *this;
  }
  Mint& operator -= (const Mint& rhs) {
    x = norm(x - rhs.x);
    return *this;
  }
  Mint& operator *= (const Mint& rhs) {
    x = (LL) x * rhs.x % P;
    return *this;
  }
  Mint& operator /= (const Mint& rhs) {
    return *this *= rhs.inv();
  }
  friend Mint operator + (const Mint& lsh, const Mint& rhs) {
    Mint ret = lsh;
    return ret += rhs;
  }
  friend Mint operator - (const Mint& lsh, const Mint& rhs) {
    Mint ret = lsh;
    return ret -= rhs;
  }
  friend Mint operator * (const Mint& lsh, const Mint& rhs) {
    Mint ret = lsh;
    return ret *= rhs;
  }
  friend Mint operator / (const Mint& lsh, const Mint& rhs) {
    Mint ret = lsh;
    return ret /= rhs;
  }
  friend istream& operator >> (istream& is, Mint& a) {
    return is >> a.x;
  }
  friend ostream& operator << (ostream& os, const Mint& a) {
    return os << a.x;
  }
};

using poly = vector<Mint>;

namespace Poly {
  vector<int> rev;
  vector<Mint> rts{0, 1};

  void init(int lim, int l) {
    rev.resize(lim);
    for (int i = 1; i < lim; ++i)
      rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (l - 1));
    if (int(rts.size()) < lim) {
      int k = __builtin_ctz(rts.size());
      rts.resize(lim);
      for (; k < l; ++k) {
        Mint w = fpow(Mint(G), (P - 1) >> (k + 1));
        for (int i = 1 << (k - 1); i < (1 << k); ++i)
          rts[i << 1] = rts[i], rts[i << 1 | 1] = w * rts[i];
      }
    }
  }

  void dft(poly& f) {
    int lim = f.size();
    for (int i = 1; i < lim; ++i)
      if (i < rev[i]) swap(f[i], f[rev[i]]);
    for (int k = 1; k < lim; k <<= 1)
      for (int i = 0; i < lim; i += k << 1)
        for (int j = 0; j < k; ++j) {
          Mint f0 = f[i+j], f1 = rts[k+j] * f[i+j+k];
          f[i+j] = f0 + f1, f[i+j+k] = f0 - f1;
        }
  }
  void idft(poly& f) {
    int lim = f.size();
    Mint iv = Mint(1) / lim;
    dft(f);
    for (auto &v: f) v *= iv;
    reverse(f.begin() + 1, f.end());
  }

  poly Mul(poly f, poly g) {
    int lim = 1, l = 0, n = f.size(), m = g.size();
    while (lim < n + m - 1) lim <<= 1, ++l;
    init(lim, l);
    f.resize(lim), dft(f);
    g.resize(lim), dft(g);
    for (int i = 0; i < lim; ++i) f[i] *= g[i];
    idft(f), f.resize(n + m - 1);
    return f;
  }
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  poly f(n + 1), g(m + 1);
  for (auto &v: f) cin >> v;
  for (auto &v: g) cin >> v;
  f = Poly::Mul(f, g);
  for (int i = 0; i <= n + m; ++i)
    cout << f[i] << " \n"[i == n + m];
  return 0;
}

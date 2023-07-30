// Polynomial
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int P = 998244353, G = 3;

int fpow(int b, int m) {
  int ret = 1;
  for (; m > 0; m >>= 1, b = (LL) b * b % P)
    if (m & 1) ret = (LL) ret * b % P;
  return ret;
}

namespace NTT {
  vector<int> rev, rts{0, 1};

  void init(int lim) {
    if ((int) rev.size() != lim) {
      int l = __builtin_ctz(lim) - 1;
      rev.resize(lim);
      for (int i = 1; i < lim; ++i)
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << l);
    }
    if ((int) rts.size() < lim) {
      int k = __builtin_ctz(rts.size());
      rts.resize(lim);
      for (; (1 << k) < lim; ++k) {
        int w = fpow(G, (P - 1) >> (k + 1));
        for (int i = 1 << (k - 1); i < (1 << k); ++i)
          rts[2 * i] = rts[i], rts[2 * i + 1] = (LL) w * rts[i] % P;
      }
    }
  }

  void dft(vector<int>& f) {
    int lim = f.size();
    init(lim);
    for (int i = 1; i < lim; ++i)
      if (i < rev[i]) swap(f[i], f[rev[i]]);
    for (int k = 1; k < lim; k <<= 1)
      for (int i = 0; i < lim; i += k << 1)
        for (int j = 0; j < k; ++j) {
          int f0 = f[i+j], f1 = (LL) rts[k+j] * f[i+j+k] % P;
          f[i+j] = (f0 + f1) % P, f[i+j+k] = (f0 - f1 + P) % P;
        }
  }
  void idft(vector<int>& f) {
    int lim = f.size(), iv = fpow(lim, P - 2);
    dft(f);
    for (auto &v: f) v = (LL) v * iv % P;
    reverse(f.begin() + 1, f.end());
  }
}

struct Poly {
  vector<int> a;

  Poly() = default;
  Poly(int n): a(n) { }
  Poly(const vector<int>& _a): a(_a) { }
  Poly(const initializer_list<int>& _a): a(_a) { }
  int size() const {
    return a.size();
  }
  void resize(int n) {
    a.resize(n);
  }

  int operator [](int idx) const {
    return (0 <= idx && idx < size())? a[idx]: 0;
  }
  int& operator [](int idx) {
    return a[idx];
  }
  Poly operator - () const {
    auto ret = a;
    for (int i = 0; i < (int) ret.size(); ++i)
      if (ret[i] > 0) ret[i] = P - ret[i];
    return ret;
  }
  friend Poly operator * (int lhs, Poly rhs) {
    for (int i = 0; i < rhs.size(); ++i)
      rhs[i] = (LL) rhs[i] * lhs % P;
    return rhs;
  }
  friend Poly operator * (Poly lhs, int rhs) {
    for (int i = 0; i < lhs.size(); ++i)
      lhs[i] = (LL) lhs[i] * rhs % P;
    return lhs;
  }
  friend Poly operator + (const Poly& f, const Poly& g) {
    vector<int> ret(max(f.size(), g.size()));
    for (int i = 0; i < (int) ret.size(); ++i)
      ret[i] = (f[i] + g[i]) % P;
    return ret;
  }
  friend Poly operator - (const Poly& f, const Poly& g) {
    vector<int> ret(max(f.size(), g.size()));
    for (int i = 0; i < (int) ret.size(); ++i)
      ret[i] = (f[i] - g[i] + P) % P;
    return ret;
  }
  friend Poly operator * (Poly f, Poly g) {
    int lim = 1, n = f.size(), m = g.size();
    while (lim < n + m - 1) lim <<= 1;
    f.resize(lim), NTT::dft(f.a);
    g.resize(lim), NTT::dft(g.a);
    for (int i = 0; i < lim; ++i)
      f[i] = (LL) f[i] * g[i] % P;
    NTT::idft(f.a), f.resize(n + m - 1);
    return f;
  }
  Poly& operator += (const Poly& rhs) {
    return *this = *this + rhs;
  }
  Poly& operator -= (const Poly& rhs) {
    return *this = *this - rhs;
  }
  Poly& operator *= (const Poly& rhs) {
    return *this = *this * rhs;
  }

  Poly mulxk(int k) const {
    auto ret = a;
    ret.insert(ret.begin(), k, 0);
    return ret;
  }
  Poly divxk(int k) const {
    if (size() <= k)
      return Poly();
    return vector<int>(a.begin() + k, a.end());
  }
  Poly modxk(int k) const {
    k = min(k, size());
    return vector<int>(a.begin(), a.begin() + k);
  }

  Poly deriv() const {
    if (a.empty())
      return Poly();
    vector<int> ret(size() - 1);
    for (int i = 0; i < size() - 1; ++i)
      ret[i] = (LL) (i + 1) * a[i + 1] % P;
    return ret;
  }
  Poly integr() const {
    vector<int> ret(size() + 1), inv(size() + 1);
    inv[1] = 1;
    for (int i = 2; i < size() + 1; ++i)
      inv[i] = (LL) (P - P / i) * inv[P % i] % P;
    for (int i = 1; i < size() + 1; ++i)
      ret[i] = (LL) a[i - 1] * inv[i] % P;
    return ret;
  }

  Poly inv(int m) const { // assume a[0] != 0
    Poly f{fpow(a[0], P - 2)};
    for (int k = 2, lim = 4; k < 2 * m; k <<= 1, lim <<= 1) {
      Poly h = modxk(k);
      h.resize(lim), NTT::dft(h.a);
      f.resize(lim), NTT::dft(f.a);
      for (int i = 0; i < lim; ++i)
        f[i] = (2 - (LL) f[i] * h[i] % P + P) * f[i] % P;
      NTT::idft(f.a), f.resize(min(k, m));
    }
    return f;
  }
  Poly sqrt(int m) const { // assume a[0] == 1
    Poly f{1};
    int iv2 = fpow(2, P - 2);
    for (int k = 2; k < 2 * m; k <<= 1)
      f = (f + modxk(k) * f.inv(k).modxk(k)) * iv2;
    return f.modxk(m);
  }
  Poly ln(int m) const { // assume a[0] == 1
    return (deriv() * inv(m)).modxk(m - 1).integr();
  }
  Poly exp(int m) const { // assume a[0] == 0
    Poly f{1};
    for (int k = 2; k < 2 * m; k <<= 1)
      f = (f * (Poly{1} - f.ln(k) + modxk(k))).modxk(k);
    return f;
  }
  Poly pow(int m, int k) const {
    int p = 0;
    while (p < size() && a[p] == 0)
      ++p;
    if (p == size() || (LL) p * k >= m)
      return Poly(m);
    Poly g = divxk(p) * fpow(a[p], P - 2);
    return (k * g.ln(m - p * k)).exp(m - p * k).mulxk(p * k) * fpow(a[p], k);
  }
};

Poly Mul(vector<Poly> a) {
  int n = a.size();
  a.resize(2 * n - 1);
  for (int i = 0; i < n - 1; ++i)
    a[n + i] = a[2 * i] * a[2 * i + 1];
  return a.back();
}

int main() {
  return 0;
}

// Mint
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
  Mint operator - () const {
    return norm(-x);
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
  bool operator < (const Mint& rhs) const {
    return x < rhs.x;
  }
  bool operator != (const Mint& rhs) const {
    return x != rhs.x;
  }
  bool operator == (const Mint& rhs) const {
    return x == rhs.x;
  }
};

int main() {
  return 0;
}

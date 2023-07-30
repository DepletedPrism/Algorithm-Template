// Big Integer
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;

struct Int {
  static constexpr int BASE = 10000000, WIDTH = 7;
  vector<int> a;

  Int() = default;
  Int(LL x) {
    do {
      a.push_back(x % BASE);
      x /= BASE;
    } while (x > 0);
  }
  Int(const string& s) {
    int n = s.size(), len = (n - 1) / WIDTH + 1;
    for (int i = 0; i < len; ++i) {
      int x = 0;
      for (int j = max(0, n - (i + 1) * WIDTH); j < n - i * WIDTH; ++j)
        x = x * 10 + s[j] - '0';
      a.push_back(x);
    }
  }
  int size() const {
    return a.size();
  }
  void norm() {
    while (a.size() > 1 && !a.back()) a.pop_back();
  }

  friend Int operator + (const Int& lhs, const Int& rhs) {
    Int ret;
    for (int i = 0, g = 0; ; ++i) {
      if (g == 0 && i >= lhs.size() && i >= rhs.size())
        break;
      int x = g;
      if (i < lhs.size())
        x += lhs.a[i];
      if (i < rhs.size())
        x += rhs.a[i];
      ret.a.push_back(x % BASE), g = x / BASE;
    }
    return ret;
  }
  friend Int operator * (const Int& lhs, const Int& rhs) {
    Int ret;
    vector<LL> c(lhs.size() + rhs.size());
    for (int i = 0; i < lhs.size(); ++i)
      for (int j = 0; j < rhs.size(); ++j)
        c[i + j] += (LL) lhs.a[i] * rhs.a[j];
    LL g = 0;
    for (size_t i = 0; ; ++i) {
      if (g == 0 && i >= c.size())
        break;
      LL x = g;
      if (i < c.size())
        x += c[i];
      ret.a.push_back(x % BASE), g = x / BASE;
    }
    ret.norm();
    return ret;
  }

  Int operator += (const Int& rhs) {
    return *this = *this + rhs;
  }
  Int operator *= (const Int& rhs) {
    return *this = *this * rhs;
  }

  bool operator < (const Int& rhs) const {
    if (size() != rhs.size())
      return size() < rhs.size();
    for (int i = size() - 1; i >= 0; --i)
      if (a[i] != rhs.a[i])
        return a[i] < rhs.a[i];
    return false;
  }
  bool operator > (const Int& rhs) const {
    return rhs < *this;
  }
  bool operator <= (const Int& rhs) const {
    return !(rhs < *this);
  }
  bool operator >= (const Int& rhs) const {
    return !(*this < rhs);
  }
  bool operator == (const Int& rhs) const {
    return !(*this < rhs) && !(rhs < *this);
  }
  bool operator != (const Int& rhs) const {
    return (*this < rhs) || (rhs < *this);
  }

  friend ostream& operator << (ostream& os, const Int& x) {
    os << x.a.back();
    for (int i = (int) x.a.size() - 2; i >= 0; --i)
      os << setw(WIDTH) << setfill('0') << x.a[i];
    return os;
  }
  friend istream& operator >> (istream& is, Int& x) {
    string s;
    if (!(is >> s)) return is;
    x = s;
    return is;
  }
};

int main() {
  Int a = 1;
  for (int i = 1; i <= 1000; ++i) a *= 2;
  cout << a << endl;
  return 0;
}

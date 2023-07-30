// LOJ #100
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int P = 1e9 + 7;

struct Mat {
  vector<vector<int>> g;

  Mat() = default;
  Mat(int n) { g.resize(n, vector<int>(n)); }
  Mat(int n, int m) { g.resize(n, vector<int>(m)); }
  int rsize() const {
    return g.size();
  }
  int csize() const {
    return g.size()? g.begin()->size(): 0;
  }

  const vector<int>& operator [](int idx) const {
    return g[idx];
  }
  vector<int>& operator [](int idx) {
    return g[idx];
  }
  Mat operator - () const {
    int n = rsize(), m = csize();
    Mat ret(n, m);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j)
        if (g[i][j]) ret[i][j] = P - g[i][j];
    return ret;
  }
  friend Mat operator + (const Mat& lhs, const Mat& rhs) {
    assert(lhs.rsize() == rhs.rsize() && lhs.csize() == rhs.csize());
    int n = lhs.rsize(), m = lhs.csize();
    Mat ret(n, m);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j)
        ret[i][j] = (lhs[i][j] + rhs[i][j]) % P;
    return ret;
  }
  friend Mat operator - (const Mat& lhs, const Mat& rhs) {
    assert(lhs.rsize() == rhs.rsize() && lhs.csize() == rhs.csize());
    int n = lhs.rsize(), m = lhs.csize();
    Mat ret(n, m);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j)
        ret[i][j] = (lhs[i][j] - rhs[i][j] + P) % P;
    return ret;
  }
  friend Mat operator * (const Mat& lhs, const Mat& rhs) {
    assert(lhs.csize() == rhs.rsize());
    int n = lhs.rsize(), p = lhs.csize(), m = rhs.csize();
    Mat ret(n, m);
    for (int i = 0; i < n; ++i)
      for (int k = 0; k < p; ++k)
        for (int j = 0; j < m; ++j)
          ret[i][j] = (ret[i][j] + (LL) lhs[i][k] * rhs[k][j] % P) % P;
    return ret;
  }
  Mat& operator += (const Mat& rhs) {
    return *this = *this + rhs;
  }
  Mat& operator -= (const Mat& rhs) {
    return *this = *this - rhs;
  }
  Mat& operator *= (const Mat& rhs) {
    return *this = *this * rhs;
  }
};

Mat eye(int n) {
  Mat ret(n);
  for (int i = 0; i < n; ++i)
    ret[i][i] = 1;
  return ret;
}

Mat fpow(Mat b, int m) {
  assert(b.rsize() == b.csize());
  Mat ret = eye(b.rsize());
  for (; m > 0; m >>= 1, b *= b)
    if (m & 1) ret *= b;
  return ret;
}

// assume -P <= x < P
inline int norm(int x) {
  return x + (x >> 31 & P);
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, p, m, x;
  cin >> n >> p >> m;
  Mat A(n, p), B(p, m);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < p; ++j)
      cin >> x, A[i][j] = norm(x);
  for (int i = 0; i < p; ++i)
    for (int j = 0; j < m; ++j)
      cin >> x, B[i][j] = norm(x);
  A *= B;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
      cout << A[i][j] << " \n"[j + 1 == m];
  return 0;
}

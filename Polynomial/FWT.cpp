// Luogu P4717
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int P = 998244353, iv2 = 499122177;

namespace FWT {
  void Or(vector<int>& f, int type) {
    int n = f.size();
    for (int k = 1; k < n; k <<= 1)
      for (int i = 0; i < n; i += k << 1)
        for (int j = 0; j < k; ++j) {
          if (type > 0)
            f[i+j+k] = (f[i+j+k] + f[i+j]) % P;
          else
            f[i+j+k] = (f[i+j+k] - f[i+j] + P) % P;
        }
  }

  void And(vector<int>& f, int type) {
    int n = f.size();
    for (int k = 1; k < n; k <<= 1)
      for (int i = 0; i < n; i += k << 1)
        for (int j = 0; j < k; ++j) {
          if (type > 0)
            f[i+j] = (f[i+j] + f[i+j+k]) % P;
          else
            f[i+j] = (f[i+j] - f[i+j+k] + P) % P;
        }
  }

  void Xor(vector<int>& f, int type) {
    int n = f.size();
    for (int k = 1; k < n; k <<= 1)
      for (int i = 0; i < n; i += k << 1)
        for (int j = 0; j < k; ++j) {
          int f0 = f[i+j], f1 = f[i+j+k];
          f[i+j] = (f0 + f1) % P, f[i+j+k] = (f0 - f1 + P) % P;
          if (type < 0) {
            f[i+j] = (LL) iv2 * f[i+j] % P;
            f[i+j+k] = (LL) iv2 * f[i+j+k] % P;
          }
        }
  }
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, m;
  cin >> m, n = 1 << m;
  vector<int> f(n), g(n);
  for (int i = 0; i < n; ++i) cin >> f[i];
  for (int j = 0; j < n; ++j) cin >> g[j];

  vector<int> A(f), B(g);
  FWT::Or(A, 1), FWT::Or(B, 1);
  for (int i = 0; i < n; ++i)
    A[i] = (LL) A[i] * B[i] % P;
  FWT::Or(A, -1);
  for (int i = 0; i < n; ++i)
    cout << A[i] << " \n"[i + 1 == n];

  A = f, B = g;
  FWT::And(A, 1), FWT::And(B, 1);
  for (int i = 0; i < n; ++i)
    A[i] = (LL) A[i] * B[i] % P;
  FWT::And(A, -1);
  for (int i = 0; i < n; ++i)
    cout << A[i] << " \n"[i + 1 == n];

  A = f, B = g;
  FWT::Xor(A, 1), FWT::Xor(B, 1);
  for (int i = 0; i < n; ++i)
    A[i] = (LL) A[i] * B[i] % P;
  FWT::Xor(A, -1);
  for (int i = 0; i < n; ++i)
    cout << A[i] << " \n"[i + 1 == n];
  return 0;
}

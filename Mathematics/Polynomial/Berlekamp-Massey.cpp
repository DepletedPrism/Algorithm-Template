// Berlekamp-Massey
// DeP
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 5e3 + 5, P = 1e9 + 7;

int fpow(int base, int b) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = 1LL * ret * base % P;
    base = 1LL * base * base % P, b >>= 1;
  }
  return ret;
}

vector<int> f[MAXN];
int d[MAXN], fail[MAXN];

int BM(const int* A, const int& n) {
  int c = 0;
  for (int i = 1; i <= n; ++i) {
    if (c == 0) {
      if (A[i] > 0)
        fail[c++] = i, d[i] = A[i], f[c].resize(i, 0);
      continue;
    }
    int s = 0;
    for (size_t j = 0; j < f[c].size(); ++j)
      s = (s + 1LL * A[i - 1 - j] * f[c][j] % P) % P;
    d[i] = (A[i] - s + P) % P, fail[c] = i;
    if (!d[i]) continue;
    int lgt, idx = -1;
    for (int j = 0; j < c; ++j)
      if (idx == -1 || i - fail[j] + (int) f[j].size() < lgt)
        idx = j, lgt = i - fail[j] + (int) f[j].size();
    int t = 1LL * d[i] * fpow(d[fail[idx]], P - 2) % P;
    ++c, f[c] = f[c - 1];
    while ((int) f[c].size() < lgt) f[c].push_back(0);
    f[c][i - fail[idx] - 1] = (f[c][i - fail[idx] - 1] + t) % P;
    for (size_t j = 0; j < f[idx].size(); ++j) {
      int& p = f[c][i - fail[idx] + j];
      p = (p - 1LL * t * f[idx][j] % P + P) % P;
    }
  }
  return c;
}

int n;
int A[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%d", &n);
  for (int i = 1; i <= n; ++i) scanf("%d", A + i);
  int c = BM(A, n);
  printf("%lu\n", f[c].size());
  for (size_t i = 0; i < f[c].size(); ++i)
    printf("%d%c", f[c][i], " \n"[i == f[c].size() - 1]);
  return 0;
}

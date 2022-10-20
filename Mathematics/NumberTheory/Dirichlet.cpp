// Luogu P5495
// DeP
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n; unsigned seed;
  cin >> n >> seed;
  vector<unsigned> f(n + 1);
  for (int i = 1; i <= n; ++i) {
    seed ^= seed << 13, seed ^= seed >> 17, seed ^= seed << 5,
    f[i] = seed;
  }
  vector<int> npr(n + 1), pr;
  npr[1] = true;
  for (int i = 2; i <= n; ++i) {
    if (!npr[i]) pr.push_back(i);
    for (int j = 0; j < (int) pr.size() && i * pr[j] <= n; ++j) {
      npr[i * pr[j]] = true;
      if (i % pr[j] == 0) break;
    }
  }
  for (auto p: pr)
    for (int i = 1; i * p <= n; ++i)
      f[i * p] += f[i];
  unsigned ans = 0;
  for (int i = 1; i <= n; ++i)
    ans ^= f[i];
  cout << ans << '\n';
  return 0;
}

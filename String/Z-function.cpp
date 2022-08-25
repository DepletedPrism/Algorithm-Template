// Luogu P5410
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  string s, t;
  cin >> t >> s;
  int n = s.size(), m = t.size();

  vector<int> z(n), p(m);
  z[0] = n;
  for (int l = 0, r = 0, i = 1; i < n; ++i) {
    z[i] = (i > r)? 0: min(r - i + 1, z[i - l]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]])
      ++z[i];
    if (i + z[i] - 1 > r)
      l = i, r = i + z[i] - 1;
  }
  for (int l = 0, r = -1, i = 0; i < m; ++i) {
    p[i] = (i > r)? 0: min(r - i + 1, z[i - l]);
    while (i + p[i] < m && p[i] < n && s[p[i]] == t[i + p[i]])
      ++p[i];
    if (i + p[i] - 1 > r)
      l = i, r = i + p[i] - 1;
  }

  LL a1 = 0, a2 = 0;
  for (int i = 0; i < n; ++i) a1 ^= (LL) (i + 1) * (z[i] + 1);
  for (int i = 0; i < m; ++i) a2 ^= (LL) (i + 1) * (p[i] + 1);
  cout << a1 << '\n' << a2 << '\n';
  return 0;
}

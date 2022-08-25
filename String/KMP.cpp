// Luogu P3375
// DeP
#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  string s, t;
  cin >> t >> s;

  int n = s.size(), m = t.size();
  vector<int> fail(n);
  fail[0] = 0;
  for (int i = 1, j = 0; i < n; ++i) {
    while (j > 0 && s[i] != s[j]) j = fail[j - 1];
    if (s[i] == s[j]) ++j;
    fail[i] = j;
  }
  for (int i = 0, j = 0; i < m; ++i) {
    while (j > 0 && t[i] != s[j]) j = fail[j - 1];
    if (t[i] == s[j]) ++j;
    if (j == n)
      cout << i - n + 2 << '\n';
  }

  for (int i = 0; i < n; ++i)
    cout << fail[i] << " \n"[i + 1 == n];
  return 0;
}

// Luogu P3375
// DeP
#include <bits/stdc++.h>
using namespace std;

constexpr int SIGMA = 26;

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
      cout << i - n + 2 << '\n', j = fail[j - 1];
  }

  // KMP automaton
  vector<array<int, SIGMA>> ch(n);
  for (int j = 0; j < n; ++j) {
    for (int c = 0; c < SIGMA; ++c) {
      if (j > 0 && c != s[j] - 'a')
        ch[j][c] = ch[fail[j - 1]][c];
      else
        ch[j][c] = j + (c == s[j] - 'a');
    }
  }

  for (int i = 0; i < n; ++i)
    cout << fail[i] << " \n"[i + 1 == n];
  return 0;
}

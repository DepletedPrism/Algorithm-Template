// Luogu P3805
// DeP
#include <bits/stdc++.h>
using namespace std;

vector<int> manacher(const string& s) {
  string t = "$#";
  for (auto c: s) t += c, t += '#';
  int n = t.size();
  vector<int> d(n);
  d[0] = 1;
  for (int l = 0, r = 0, i = 1; i < n; ++i) {
    int k = (i > r)? 1: min(r - i, d[l + r - i]);
    while (i + k < n && t[i - k] == t[i + k])
      ++k;
    d[i] = k--;
    if (i + k > r)
      l = i - k, r = i + k;
  }
  return d;
}

int main(void) {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  string s;
  cin >> s;
  auto d = manacher(s);
  cout << *max_element(d.begin(), d.end()) - 1 << '\n';
  return 0;
}

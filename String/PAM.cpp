// Luogu P4287
// DeP
#include <bits/stdc++.h>
using namespace std;

constexpr int SIGMA = 26;

namespace PAM {
  int nidx;
  vector<int> str, len, fail, depth;
  vector<array<int, SIGMA>> ch;

  inline void init(int n) {
    nidx = 1;
    len.resize(n + 2), fail.resize(n + 2), depth.resize(n + 2);
    ch.resize(n + 2);
    len[0] = 0, fail[0] = 1, len[1] = -1;
  }

  int ins(int c, int lst) {
    str.push_back(c);
    auto getfail = [&](int u) {
      int n = str.size();
      while (n < len[u] + 2 || str[n - len[u] - 2] != c)
        u = fail[u];
      return u;
    };
    int nd = getfail(lst);
    if (!ch[nd][c]) {
      int p = ++nidx;
      len[p] = len[nd] + 2;
      fail[p] = ch[getfail(fail[nd])][c];
      ch[nd][c] = p;
      depth[p] = depth[fail[p]] + 1;
    }
    return ch[nd][c];
  }
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  string s;
  cin >> s;
  int n = s.size();
  PAM::init(n);
  for (int lst = 0, k = 0, i = 0; i < n; ++i) {
    int c = (s[i] - 'a' + k) % SIGMA;
    lst = PAM::ins(c, lst);
    cout << (k = PAM::depth[lst]) << " \n"[i + 1 == n];
  }
  return 0;
}

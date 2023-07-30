// Luogu P5357
// DeP
#include <bits/stdc++.h>
using namespace std;

constexpr int SIGMA = 26;

namespace AC {
  vector<array<int, SIGMA>> ch;
  vector<int> fail, mth;

  inline void init() {
    ch.resize(1);
  }
  void ins(const string& s) {
    int m = s.size(), u = 0;
    for (int i = 0; i < m; ++i) {
      int c = s[i] - 'a';
      if (!ch[u][c]) {
        ch[u][c] = ch.size();
        ch.emplace_back();
      }
      u = ch[u][c];
    }
    mth.push_back(u);
  }
  void build() {
    queue<int> Q;
    fail.resize(ch.size());
    for (int c = 0; c < SIGMA; ++c)
      if (ch[0][c] > 0) Q.push(ch[0][c]);
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      for (int c = 0; c < SIGMA; ++c) {
        int &v = ch[u][c];
        if (v > 0)
          fail[v] = ch[fail[u]][c], Q.push(v);
        else
          v = ch[fail[u]][c];
      }
    }
  }

  void solve(const string& s) {
    int m = s.size(), nidx = ch.size();
    vector<int> cnt(nidx);
    for (int u = 0, i = 0; i < m; ++i) {
      int c = s[i] - 'a';
      u = ch[u][c], ++cnt[u];
    }
    vector<vector<int>> G(nidx);
    for (int u = 1; u < nidx; ++u)
      G[fail[u]].push_back(u);
    function<void(int)> dfs = [&](int u) {
      for (auto v: G[u])
        dfs(v), cnt[u] += cnt[v];
    };
    dfs(0);
    for (int i = 0; i < (int) mth.size(); ++i)
      cout << cnt[mth[i]] << '\n';
  }
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  cin >> n;
  string s;
  AC::init();
  for (int i = 0; i < n; ++i)
    cin >> s, AC::ins(s);
  AC::build();
  cin >> s, AC::solve(s);
  return 0;
}

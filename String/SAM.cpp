// Luogu P3804
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int SIGMA = 26;

namespace SAM {
  int nidx;
  vector<int> len, lnk;
  vector<array<int, SIGMA>> ch;

  inline void init(int n) {
    nidx = 1;
    len.resize(2 * n), lnk.resize(2 * n), ch.resize(2 * n);
  }

  int ins(int c, int p) {
    int nd = ++nidx;
    len[nd] = len[p] + 1;
    while (p && !ch[p][c])
      ch[p][c] = nd, p = lnk[p];
    if (!p) lnk[nd] = 1;
    else {
      int q = ch[p][c];
      if (len[q] == len[p] + 1) lnk[nd] = q;
      else {
        int nxt = ++nidx;
        len[nxt] = len[p] + 1;
        lnk[nxt] = lnk[q], ch[nxt] = ch[q];
        while (p && ch[p][c] == q)
          ch[p][c] = nxt, p = lnk[p];
        lnk[q] = lnk[nd] = nxt;
      }
    }
    return nd;
  }

  LL solve(const string& s) {
    int n = s.size();
    init(n);
    vector<int> f(2 * n), idx(2 * n), cnt(n + 1);
    for (int lst = 1, i = 0; i < n; ++i)
      lst = SAM::ins(s[i] - 'a', lst), f[lst] = 1;
    LL ret = 0;
    for (int u = 1; u <= nidx; ++u)
      ++cnt[len[u]];
    for (int i = 1; i <= n; ++i)
      cnt[i] += cnt[i - 1];
    for (int u = 1; u <= nidx; ++u)
      idx[cnt[len[u]]--] = u;
    for (int i = nidx; i >= 1; --i) {
      int u = idx[i];
      if (f[u] > 1)
        ret = max(ret, (LL) f[u] * len[u]);
      f[lnk[u]] += f[u];
    }
    return ret;
  }
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  string s;
  cin >> s;
  cout << SAM::solve(s) << '\n';
  return 0;
}

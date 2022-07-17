// Luogu P5357
// DeP
#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;

const int MAXN = 2e6 + 5, MAXM = 2e5 + 5, SIGMA = 26;

int n;
char s[MAXN], t[MAXM];

int cnt[MAXM], mth[MAXM];

namespace AC {
  int ch[MAXM][SIGMA], fail[MAXM], nidx;
  int Q[MAXM], hd, tl;
  vector<int> G[MAXN];

  void ins(char const *s, int idx) {
    int m = strlen(s + 1), u = 0;
    for (int i = 1; i <= m; ++i) {
      int c = s[i] - 'a';
      if (!ch[u][c]) ch[u][c] = ++nidx;
      u = ch[u][c];
    }
    mth[idx] = u;
  }

  void dfs(int u) {
    for (auto &v: G[u])
      dfs(v), cnt[u] += cnt[v];
  }

  void bld() {
    hd = 1, tl = 0;
    for (int c = 0; c < SIGMA; ++c)
      if (ch[0][c] > 0) Q[++tl] = ch[0][c];
    while (hd <= tl) {
      int u = Q[hd++];
      for (int c = 0; c < SIGMA; ++c) {
        int &v = ch[u][c];
        if (v > 0)
          fail[v] = ch[fail[u]][c], Q[++tl] = v;
        else
          v = ch[fail[u]][c];
      }
    }
    for (int u = 1; u <= nidx; ++u)
      G[fail[u]].push_back(u);
  }

  void qry(char const *s) {
    int m = strlen(s + 1);
    for (int u = 0, i = 1; i <= m; ++i) {
      int c = s[i] - 'a';
      u = ch[u][c], ++cnt[u];
    }
  }
}

int main() {
  scanf("%d", &n);
  for (int i = 1; i <= n; ++i)
    scanf("%s", t + 1), AC::ins(t, i);
  scanf("%s", s + 1);
  AC::bld(), AC::qry(s), AC::dfs(0);
  for (int i = 1; i <= n; ++i)
    printf("%d\n", cnt[mth[i]]);
  return 0;
}

// LOJ #3303
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int MAXN = 525010 + 5, LOG = 21;

int n; LL ans;
int A[MAXN];

int rt[MAXN];
vector<int> G[MAXN];

namespace Trie {
  const int MAXM = 1.2e7 + 5;
  int ch[MAXM][2], s[MAXM], c[MAXM], nidx;

  inline void maintain(int u) {
    s[u] = 0;
    if (ch[u][0] > 0)
      s[u] ^= (s[ch[u][0]] << 1);
    if (ch[u][1] > 0)
      s[u] ^= (s[ch[u][1]] << 1) | c[ch[u][1]];
  }

  void Ins(int &u, int val, int k) {
    if (!u) u = ++nidx;
    c[u] ^= 1;
    if (k <= LOG) {
      Ins(ch[u][(val >> k) & 1], val, k + 1);
      maintain(u);
    }
  }
  void Pls(int u) {
    swap(ch[u][0], ch[u][1]);
    if (ch[u][0] > 0) Pls(ch[u][0]);
    maintain(u);
  }

  int Mrg(int x, int y) { // y --> x
    if (!x || !y) return x? x: y;
    c[x] ^= c[y], s[x] ^= s[y];
    ch[x][0] = Mrg(ch[x][0], ch[y][0]);
    ch[x][1] = Mrg(ch[x][1], ch[y][1]);
    maintain(x);
    return x;
  }
}

void dfs(int u) {
  for (auto &v: G[u]) {
    dfs(v);
    rt[u] = Trie::Mrg(rt[u], rt[v]);
  }
  Trie::Pls(rt[u]);
  Trie::Ins(rt[u], A[u], 0);
  ans += Trie::s[rt[u]];
}

int main() {
  scanf("%d", &n);
  for (int i = 1; i <= n; ++i)
    scanf("%d", A + i);
  for (int p, u = 2; u <= n; ++u)
    scanf("%d", &p), G[p].push_back(u);
  dfs(1);
  printf("%lld\n", ans);
  return 0;
}

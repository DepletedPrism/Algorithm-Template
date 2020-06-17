// Luogu P3804
// DeP
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 1e6 + 5;

int n;
char S[MAXN];

namespace SAM {
  const int MAXN = ::MAXN << 1, SIGMA = 26;
  int ch[SIGMA][MAXN], len[MAXN], lnk[MAXN], nidx, lst;
  int cnt[MAXN], idx[MAXN], f[MAXN];

  inline void init() { nidx = lst = 1; }

  inline void Ins(int v) {
    int nd = ++nidx, p = lst;
    len[nd] = len[lst] + 1, f[nd] = 1;
    while (p && !ch[v][p]) ch[v][p] = nd, p = lnk[p];
    if (!p) lnk[nd] = 1;
    else {
      int q = ch[v][p];
      if (len[q] == len[p] + 1) lnk[nd] = q;
      else {
        int nxt = ++nidx;
        len[nxt] = len[p] + 1, lnk[nxt] = lnk[q];
        for (int c = 0; c < SIGMA; ++c) ch[c][nxt] = ch[c][q];
        while (p && ch[v][p] == q) ch[v][p] = nxt, p = lnk[p];
        lnk[q] = lnk[nd] = nxt;
      }
    }
    lst = nd;
  }

  LL solve() {
    LL ret = 0;
    for (int i = 1; i <= nidx; ++i) ++cnt[len[i]];
    for (int i = 1; i <= n; ++i) cnt[i] += cnt[i-1];
    for (int i = nidx; i; --i) idx[cnt[len[i]]--] = i;
    for (int i = nidx; i; --i) {
      int u = idx[i];
      if (f[u] > 1) ret = max(ret, (LL) f[u] * len[u]);
      f[lnk[u]] += f[u];
    }
    return ret;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  SAM::init();
  scanf("%s", S + 1);

  n = strlen(S + 1);
  for (int i = 1; i <= n; ++i)
    SAM::Ins(S[i] - 'a');

  printf("%lld\n", SAM::solve());
  return 0;
}

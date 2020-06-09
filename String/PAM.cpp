// Luogu P4287
// DeP
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 500005, SIGMA = 26;

int n;
char S[MAXN];

namespace PAM {
  int ch[MAXN][SIGMA], len[MAXN], fail[MAXN], trans[MAXN], last, nidx, ptr;
  char S[MAXN];

  inline void init() {
    last = 0, nidx = 1;
    len[0] = 0, fail[0] = 1, len[1] = -1;
    S[ptr = 0] = '$';
  }

  inline int getfail(int u) {
    while (S[ptr - len[u] - 1] != S[ptr]) u = fail[u];
    return u;
  }

  void insert(char c) {
    S[++ptr] = c;
    int val = c - 'a', nd = getfail(last);
    if (!ch[nd][val]) {
      int p = ++nidx;
      len[p] = len[nd] + 2;
      fail[p] = ch[getfail(fail[nd])][val];
      ch[nd][val] = p;
      if (len[p] <= 2) trans[p] = fail[p];
      else {
        int u = trans[nd];
        while (S[ptr - len[u] - 1] != S[ptr] || 2 * (len[u]+2) > len[p]) u = fail[u];
        trans[p] = ch[u][val];
      }
    }
    last = ch[nd][val];
  }

  int solve() {
    int ret = 0;
    for (int i = nidx; i >= 2; --i)
      if (len[trans[i]] * 2 == len[i] && len[trans[i]] % 2 == 0)
        ret = max(ret, len[i]);
    return ret;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("2.in", "r", stdin);
#endif
  PAM::init();
  scanf("%d%s", &n, S+1);

  for (int i = 1; i <= n; ++i) PAM::insert(S[i]);

  printf("%d\n", PAM::solve());
  return 0;
}

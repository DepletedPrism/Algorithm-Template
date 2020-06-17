// Luogu P5357
// DeP
#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;

const int MAXN = 2e5 + 5, MAXS = 2e6 + 5, SIGMA = 26;

int n;
char str[MAXS];

int siz[MAXN];
vector<int> G[MAXN];

void dfs(int u) {
  for (const int& v: G[u])
    dfs(v), siz[u] += siz[v];
}

namespace AC {
  int ch[MAXN][SIGMA], fail[MAXN], nidx;
  int match[MAXN];

  inline void init() { nidx = 1; }

  void Ins(char* s, const int& lgt, int idx) {
    int u = 1;
    for (int i = 1; i <= lgt; ++i) {
      int c = s[i] - 'a';
      if (!ch[u][c]) ch[u][c] = ++nidx;
      u = ch[u][c];
    }
    match[idx] = u;
  }

  void getfail() {
    static int Q[MAXN], head, tail;
    Q[head = 1] = tail = 0;
    fail[1] = 1;
    for (int c = 0; c < SIGMA; ++c) {
      int& v = ch[1][c];
      if (v) Q[++tail] = v, fail[v] = 1; else v = 1;
    }
    while (head <= tail) {
      int u = Q[head++];
      for (int c = 0; c < SIGMA; ++c) {
        int& v = ch[u][c];
        if (v) Q[++tail] = v, fail[v] = ch[fail[u]][c];
        else v = ch[fail[u]][c];
      }
    }
  }

  void Mrk(char* s, const int& lgt) {
    int u = 1;
    for (int c, i = 1; i <= lgt; ++i)
      c = s[i] - 'a', u = ch[u][c], ++siz[u];
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  AC::init();

  scanf("%d", &n);
  for (int i = 1; i <= n; ++i)
    scanf("%s", str + 1), AC::Ins(str, strlen(str + 1), i);
  scanf("%s", str + 1);

  AC::getfail();
  AC::Mrk(str, strlen(str + 1));
  for (int u = 2; u <= AC::nidx; ++u)
    G[AC::fail[u]].push_back(u);
  dfs(1);

  for (int i = 1; i <= n; ++i)
    printf("%d\n", siz[AC::match[i]]);
  return 0;
}

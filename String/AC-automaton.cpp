// Luogu P5357
// DeP
#include <queue>
#include <cstdio>
#include <cstring>
using namespace std;

const int MAXN = 2e5 + 5, MAXL = 2e6 + 5;

int n;
char str[MAXL];
int subsize[MAXN];

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXN];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
  }

  void dfs(int u) {
    for (int v, i = head[u]; ~i; i = edges[i].nxt)
      dfs(v = edges[i].to), subsize[u] += subsize[v];
  }
}

namespace AC {
  const int SIGMA = 26;
  int fail[MAXN], ch[SIGMA][MAXN], match[MAXN], nidx;

  inline int idx(char c) { return c - 'a'; }

  void insert(char* S, int v) {
    int u = 0, len = strlen(S);
    for (int i = 0; i < len; ++i) {
      int c = idx(S[i]);
      if (!ch[c][u]) ch[c][u] = ++nidx;
      u = ch[c][u];
    }
    match[v] = u;
  }

  void getFail() {
    queue<int> Q;
    fail[0] = 0;
    for (int c = 0; c < SIGMA; ++c) {
      int v = ch[c][0];
      if (v) Q.push(v), fail[v] = 0;
    }
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      for (int c = 0; c < SIGMA; ++c) {
        int v = ch[c][u];
        if (v) Q.push(v), fail[v] = ch[c][fail[u]];
        else ch[c][u] = ch[c][fail[u]];
      }
    }
  }

  void Qry(char* T) {
    int len = strlen(T), u = 0;
    for (int i = 0; i < len; ++i) u = ch[idx(T[i])][u], ++subsize[u];
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  scanf("%d", &n);
  for (int i = 1; i <= n; ++i) scanf("%s", str), AC::insert(str, i);

  AC::getFail();
  scanf("%s", str), AC::Qry(str);

  for (int i = 1; i <= AC::nidx; ++i)
    Graph::AddEdge(AC::fail[i], i);
  Graph::dfs(0);

  for (int i = 1; i <= n; ++i) printf("%d\n", subsize[AC::match[i]]);
  return 0;
}

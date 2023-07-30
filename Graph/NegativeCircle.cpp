// Luogu P3385
// DeP
#include <queue>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

constexpr int MAXN = 2e3 + 5, MAXM = 6e3 + 5;

int n, m;

namespace Graph {
  struct Edge {
    int nxt, v, w;
  } edges[MAXM];
  int head[MAXN], eidx;

  inline void init() {
    memset(head, -1, (n + 1) * sizeof(int)), eidx = 1;
  }
  inline void addEdge(int u, int v, int w) {
    edges[++eidx] = {head[u], v, w}, head[u] = eidx;
  }

  bool inq[MAXN];
  int cnt[MAXN], d[MAXN];

  bool SPFA(int s) {
    queue<int> Q;
    memset(cnt, 0, (n + 1) * sizeof(int));
    memset(d, 0x3f, (n + 1) * sizeof(int));
    memset(inq, false, (n + 1) * sizeof(bool));
    d[s] = 0, inq[s] = true;
    Q.push(s);
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      inq[u] = false;
      for (int v, i = head[u]; ~i; i = edges[i].nxt)
        if (d[v = edges[i].v] > d[u] + edges[i].w) {
          d[v] = d[u] + edges[i].w;
          cnt[v] = cnt[u] + 1;
          if (cnt[v] >= n) return false;
          if (!inq[v])
            Q.push(v), inq[v] = true;
        }
    }
    return true;
  }
}

int main() {
  int ti;
  scanf("%d", &ti);
  while (ti--) {
    scanf("%d%d", &n, &m);
    Graph::init();
    for (int u, v, w, i = 1; i <= m; ++i) {
      scanf("%d%d%d", &u, &v, &w);
      if (w >= 0)
        Graph::addEdge(u, v, w), Graph::addEdge(v, u, w);
      else
        Graph::addEdge(u, v, w);
    }
    puts(Graph::SPFA(1)? "NO": "YES");
  }
  return 0;
}

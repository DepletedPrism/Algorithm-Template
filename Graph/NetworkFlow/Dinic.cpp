// LOJ #101
// Dinic
#include <bits/stdc++.h>
using namespace std;

typedef long long LL; 
const int MAXN = 1e2 + 5, MAXM = 5e3 + 5;

int n, m, S, T;

namespace Graph {
  struct Edge {
    int nxt, to, flow, cap;
  } edges[MAXM << 1];
  int head[MAXN], eidx;

  inline void init() {
    memset(head, -1, sizeof head), eidx = 1;
  }
  inline void addEdge(int u, int v, int c) {
    edges[++eidx] = (Edge){ head[u], v, 0, c }, head[u] = eidx;
    edges[++eidx] = (Edge){ head[v], u, 0, 0 }, head[v] = eidx;
  }
}

namespace Dinic {
  using namespace Graph;
  int cur[MAXN], depth[MAXN], vis[MAXN], Time;

  bool BFS() {
    static int Q[MAXN], h, t;
    depth[S] = 0, vis[S] = ++Time, Q[h = t = 1] = S;
    while (h <= t) {
      int u = Q[h++];
      for (int i = head[u]; ~i; i = edges[i].nxt) {
        const Edge& e = edges[i];
        if (e.flow < e.cap && vis[e.to] != Time)
          vis[e.to] = Time, depth[e.to] = depth[u] + 1, Q[++t] = e.to;
      }
    }
    return vis[T] == Time;
  }

  LL DFS(int u, int a) {
    if (u == T || !a) return a;
    LL flow = 0;
    for (int f, &i = cur[u]; ~i; i = edges[i].nxt) {
      Edge& e = edges[i];
      if (depth[e.to] == depth[u] + 1 &&
          (f = DFS(e.to, min(a, e.cap - e.flow))) > 0) {
        e.flow += f, edges[i ^ 1].flow -= f, flow += f, a -= f;
        if (!a) break;
      }
    }
    return flow;
  }

  LL maxflow() {
    LL ret = 0;
    while (BFS())
      memcpy(cur, head, sizeof cur), ret += DFS(S, INT_MAX);
    return ret;
  }
}

int main(void) {
  Graph::init();

  scanf("%d%d%d%d", &n, &m, &S, &T);
  for (int u, v, c, i = 1; i <= m; ++i)
    scanf("%d%d%d", &u, &v, &c), Graph::addEdge(u, v, c);

  printf("%lld\n", Dinic::maxflow());
  return 0;
}

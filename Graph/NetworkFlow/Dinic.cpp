// LOJ #101
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXV = 1e2 + 5, MAXE = 5e3 + 5;
const LL INFLL = 0x3f3f3f3f3f3f3f3fLL;

int S, T;

namespace Graph {
  struct Edge { int nxt, to; LL cap, flow; } edges[MAXE << 1];
  int head[MAXV], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to, LL c) {
    edges[++eidx] = (Edge){ head[from], to, c, 0 }, head[from] = eidx;
    edges[++eidx] = (Edge){ head[to], from, 0, 0 }, head[to] = eidx;
  }
}

namespace Dinic {
  using namespace Graph;
  int cur[MAXV], depth[MAXV], vis[MAXV], Time;

  bool BFS() {
    static int Q[MAXV], h, t;
    Q[h = t = 1] = S, depth[S] = 1, vis[S] = ++Time;
    while (h <= t) {
      int u = Q[h++];
      for (int i = head[u]; ~i; i = edges[i].nxt) {
        const Edge& e = edges[i];
        if (vis[e.to] != Time && e.cap > e.flow)
          vis[e.to] = Time, depth[e.to] = depth[u] + 1, Q[++t] = e.to;
      }
    }
    return vis[T] == Time;
  }

  LL DFS(int u, LL a) {
    if (u == T || !a) return a;
    LL f, flow = 0;
    for (int& i = cur[u]; ~i; i = edges[i].nxt) {
      Edge& e = edges[i];
      if (depth[e.to] == depth[u] + 1 &&
          (f = DFS(e.to, min(a, e.cap - e.flow))) > 0) {
        flow += f, a -= f, e.flow += f, edges[i ^ 1].flow -= f;
        if (!a) break;
      }
    }
    return flow;
  }

  LL Maxflow() {
    LL flow = 0;
    while (BFS())
      memcpy(cur, head, sizeof cur), flow += DFS(S, INFLL);
    return flow;
  }
}

int n, m;

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  scanf("%d%d%d%d", &n, &m, &S, &T);
  for (int u, v, w, i = 1; i <= m; ++i) {
    scanf("%d%d%d", &u, &v, &w);
    Graph::AddEdge(u, v, w);
  }

  printf("%lld\n", Dinic::Maxflow());
  return 0;
}

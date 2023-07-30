// LOJ #101
// Dinic
#include <bits/stdc++.h>
using namespace std;

using LL = long long;

namespace Dinic {
  struct Edge {
    int nxt, v, flow, cap;
  };
  vector<Edge> edges;
  vector<int> head, cur, depth, vis;

  void init(int n) {
    edges.clear();
    head.resize(n), depth.resize(n), vis.resize(n);
    fill(head.begin(), head.end(), -1);
  }
  void addEdge(int u, int v, int c) {
    edges.push_back({head[u], v, 0, c}), head[u] = edges.size() - 1;
    edges.push_back({head[v], u, 0, 0}), head[v] = edges.size() - 1;
  }

  bool BFS(int s, int t) {
    static int vcnt;
    queue<int> Q;
    depth[s] = 0, vis[s] = ++vcnt, Q.push(s);
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      for (int i = head[u]; ~i; i = edges[i].nxt) {
        const Edge& e = edges[i];
        if (e.flow < e.cap && vis[e.v] != vcnt)
          vis[e.v] = vcnt, depth[e.v] = depth[u] + 1, Q.push(e.v);
      }
    }
    return vis[t] == vcnt;
  }

  LL DFS(int u, int t, int a) {
    if (u == t || !a) return a;
    LL flow = 0;
    for (int f, &i = cur[u]; ~i; i = edges[i].nxt) {
      Edge& e = edges[i];
      if (depth[e.v] == depth[u] + 1
          && (f = DFS(e.v, t, min(a, e.cap - e.flow))) > 0) {
        e.flow += f, edges[i ^ 1].flow -= f, flow += f, a -= f;
        if (!a) break;
      }
    }
    return flow;
  }

  LL maxflow(int s, int t) {
    LL ret = 0;
    while (BFS(s, t))
      cur = head, ret += DFS(s, t, INT_MAX);
    return ret;
  }
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, m, s, t;
  cin >> n >> m >> s >> t;
  Dinic::init(n + 1);
  for (int u, v, c, i = 1; i <= m; ++i)
    cin >> u >> v >> c, Dinic::addEdge(u, v, c);
  cout << Dinic::maxflow(s, t) << '\n';
  return 0;
}

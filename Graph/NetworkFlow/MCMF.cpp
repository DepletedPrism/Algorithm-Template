// LOJ #102
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

namespace IO {
  const int MAXSIZE = 1 << 18 | 1;
  char buf[MAXSIZE], *p1, *p2;

  inline int Gc() {
    return p1 == p2 &&
      (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++;
  }
  template<typename T> inline void read(T& x) {
    x = 0; int f = 0, ch = Gc();
    while (!isdigit(ch)) f |= ch == '-', ch = Gc();
    while (isdigit(ch)) x = x * 10 - '0' + ch, ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

const int MAXV = 405, MAXE = 15005;
const int INF = 0x3f3f3f3f;

int n, m, S, T;

namespace Graph {
  struct Edge { int nxt, to, cap, flow, cost; } edges[MAXE << 1];
  int head[MAXV], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to, int c, int w) {
    edges[++eidx] = (Edge){ head[from], to, c, 0, w }, head[from] = eidx;
    edges[++eidx] = (Edge){ head[to], from, 0, 0, -w }, head[to] = eidx;
  }
}

namespace MCMF {
  using namespace Graph;
  bool inq[MAXV];
  int d[MAXV], a[MAXV], pre[MAXV];

  bool SPFA() {
    static int Q[MAXV << 2], h, t;
    memset(d, 0x3f, sizeof d);
    Q[h = t = 1] = S;
    pre[S] = d[S] = 0, a[S] = INF, inq[S] = true;
    while (h <= t) {
      int u = Q[h++];
      inq[u] = false;
      for (int i = head[u]; ~i; i = edges[i].nxt) {
        const Edge& e = edges[i];
        if (d[e.to] > d[u] + e.cost && e.cap > e.flow) {
          a[e.to] = min(a[u], e.cap - e.flow);
          d[e.to] = d[u] + e.cost, pre[e.to] = i;
          if (!inq[e.to]) Q[++t] = e.to, inq[e.to] = true;
        }
      }
    }
    return d[T] < INF;
  }

  void MCMF(int& flow, int& cost) {
    flow = cost = 0;
    while (SPFA()) {
      flow += a[T], cost += a[T] * d[T];
      for (int u = T; u != S; u = edges[pre[u] ^ 1].to)
        edges[pre[u]].flow += a[T], edges[pre[u] ^ 1].flow -= a[T];
    }
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  // init
  Graph::init();
  // input
  read(n), read(m);
  S = 1, T = n;
  while (m--) {
    static int u, v, c, w;
    read(u), read(v), read(c), read(w);
    Graph::AddEdge(u, v, c, w);
  }
  // output
  int flow, cost;
  MCMF::MCMF(flow, cost);
  printf("%d %d\n", flow, cost);
  return 0;
}

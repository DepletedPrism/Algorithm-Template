// LOJ #119
// DeP
#include <queue>
#include <cctype>
#include <cstdio>
#include <cstring>
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
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

typedef long long LL;
const int MAXN = 2.5e3 + 5, MAXM = 6.2e3 + 5;

int n, m, S, T;
LL dist[MAXN];

namespace Graph {
  struct Edge { int nxt, to, w; } edges[MAXM << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to, int w) {
    edges[++eidx] = (Edge){ head[from], to, w }, head[from] = eidx;
  }

  void SPFA(int s) {
    static bool inq[MAXN];
    queue<int> Q;
    memset(dist, 0x3f, sizeof dist);
    Q.push(s);
    dist[s] = 0, inq[s] = true;
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      inq[u] = false;
      for (int v, i = head[u]; ~i; i = edges[i].nxt)
        if (dist[v = edges[i].to] > dist[u] + edges[i].w) {
          dist[v] = dist[u] + edges[i].w;
          if (!inq[v]) Q.push(v), inq[v] = true;
        }
    }
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n), read(m), read(S), read(T);
  for (int u, v, w, i = 1; i <= m; ++i) {
    read(u), read(v), read(w);
    Graph::AddEdge(u, v, w), Graph::AddEdge(v, u, w);
  }

  Graph::SPFA(S);

  printf("%lld\n", dist[T]);
  return 0;
}

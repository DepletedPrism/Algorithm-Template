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
typedef pair<LL, int> Pli;
const int MAXN = 2505, MAXM = 6205;

int n, m, s, t;
LL d[MAXN];

namespace Graph {
  struct Edge { int nxt, to, w; } edges[MAXM << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head); eidx = 1; }
  inline void AddEdge(int from, int to, int w) {
    edges[++eidx] = (Edge){ head[from], to, w }, head[from] = eidx;
  }

  void Dijkstra(int s) {
    priority_queue<Pli, vector<Pli>, greater<Pli> > PQ;
    memset(d, 0x3f, sizeof d);
    d[s] = 0, PQ.push(Pli(d[s], s));
    while (!PQ.empty()) {
      int u = PQ.top().second, w = PQ.top().first; PQ.pop();
      if (d[u] != w) continue;
      for (int v, i = head[u]; ~i; i = edges[i].nxt)
        if (d[v = edges[i].to] > d[u] + edges[i].w)
          PQ.push(Pli(d[v] = d[u] + edges[i].w, v));
    }
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  Graph::init();

  read(n), read(m), read(s), read(t);
  for (int u, v, w, i = 1; i <= m; ++i) {
    read(u), read(v), read(w);
    Graph::AddEdge(u, v, w), Graph::AddEdge(v, u, w);
  }

  Graph::Dijkstra(s);

  printf("%lld\n", d[t]);
  return 0;
}

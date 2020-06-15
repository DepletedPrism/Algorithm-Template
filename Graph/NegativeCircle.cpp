// Luogu P3385
// DeP
#include <queue>
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
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
    if (f) x = -x;
  }
}
using IO::read;

const int MAXN = 2e3 + 5, MAXM = 3e3 + 5;

int n, m;

namespace Graph {
  struct Edge { int nxt, to, w; } edges[MAXM << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to, int w) {
    edges[++eidx] = (Edge){ head[from], to, w }, head[from] = eidx;
  }

  int vis[MAXN], cnt[MAXN], d[MAXN], Time;

  bool SPFA(int s) {
    queue<int> Q;
    memset(cnt, 0, (n + 1) * sizeof (int));
    memset(d, 0x3f, (n + 1) * sizeof (int));
    Q.push(s), d[s] = 0, vis[s] = ++Time;
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      vis[u] = Time - 1;
      for (int v, i = head[u]; ~i; i = edges[i].nxt)
        if (d[v = edges[i].to] > d[u] + edges[i].w) {
          d[v] = d[u] + edges[i].w;
          if (vis[v] != Time) {
            if (++cnt[v] >= n) return false;
            Q.push(v), vis[v] = Time;
          }
        }
    }
    return true;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  int Ti; read(Ti);
  while (Ti--) {
    Graph::init();

    read(n), read(m);
    for (int u, v, w, i = 1; i <= m; ++i) {
      read(u), read(v), read(w);
      if (w >= 0)
        Graph::AddEdge(u, v, w), Graph::AddEdge(v, u, w);
      else Graph::AddEdge(u, v, w);
    }

    puts(Graph::SPFA(1)? "NO": "YES");
  }
  return 0;
}

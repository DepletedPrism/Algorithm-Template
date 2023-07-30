// Luogu P2056
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
using IO::read; using IO::Gc;

const int MAXN = 1e5+5, LOG = 17;

struct Heap {
  priority_queue<int> A, B;

  inline void insert(const int& v) { A.push(v); }
  inline void erase(const int& v) { B.push(v); }

  inline int top() {
    while (!B.empty() && A.top() == B.top()) A.pop(), B.pop();
    return A.top();
  }
  inline void pop() {
    while (!B.empty() && A.top() == B.top()) A.pop(), B.pop();
    A.pop();
  }

  inline int top2() {
    int ret, tmp = top(); pop(), ret = top();
    return A.push(tmp), ret;
  }

  inline int size() { return A.size() - B.size(); }
} sub[MAXN], ch[MAXN], Ans;

int n, m;
int V[MAXN];
int depth[MAXN], pre[MAXN], dist[LOG][MAXN];

namespace Graph {
  struct Edge { int nxt, to; } edges[MAXN << 1];
  int head[MAXN], eidx;

  inline void init() { memset(head, -1, sizeof head), eidx = 1; }
  inline void AddEdge(int from, int to) {
    edges[++eidx] = (Edge){ head[from], to };
    head[from] = eidx;
  }
}

namespace HLD {
  using namespace Graph;
  int pre[MAXN], depth[MAXN], topfa[MAXN], size[MAXN], son[MAXN];

  void dfs1(int u, int fa) {
    depth[u] = depth[fa] + 1;
    pre[u] = fa, size[u] = 1, son[u] = -1;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if ((v = edges[i].to) == fa) continue;
      dfs1(v, u), size[u] += size[v];
      if (son[u] == -1 || size[v] > size[son[u]]) son[u] = v;
    }
  }

  void dfs2(int u, int top) {
    topfa[u] = top;
    if (~son[u]) dfs2(son[u], top);
    for (int v, i = head[u]; ~i; i = edges[i].nxt)
      if ((v = edges[i].to) != pre[u] && v != son[u]) dfs2(v, v);
  }

  int LCA(int u, int v) {
    while (topfa[u] != topfa[v]) {
      if (depth[topfa[u]] < depth[topfa[v]]) swap(u, v);
      u = pre[topfa[u]];
    }
    return depth[u] > depth[v]? v: u;
  }
  inline int Dist(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[LCA(u, v)];
  }

  inline void solve() { dfs1(1, 0), dfs2(1, 1); }
}

namespace TreeDivide {
  using namespace Graph;
  bool vis[MAXN];
  int Balance[MAXN], size[MAXN], subsize, ct;

  inline void init(const int& x) { Balance[ct = 0] = MAXN, subsize = x; }

  void findCt(int u, int fa) {
    Balance[u] = 0, size[u] = 1;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if ((v = edges[i].to) == fa || vis[v]) continue;
      findCt(v, u), size[u] += size[v];
      Balance[u] = max(Balance[u], size[v]);
    }
    Balance[u] = max(Balance[u], subsize - size[u]);
    if (Balance[u] < Balance[ct]) ct = u;
  }

  void dfs(int u, int fa, int depth, Heap& T) {
    T.insert(depth);
    for (int v, i = head[u]; ~i; i = edges[i].nxt)
      if ((v = edges[i].to) != fa && !vis[v]) dfs(v, u, depth + 1, T);
  }

  void build(int u) {
    vis[u] = true;
    for (int v, i = head[u]; ~i; i = edges[i].nxt) {
      if (vis[v = edges[i].to]) continue;
      init(size[v]), findCt(v, -1), findCt(ct, -1);
      // maintain base
      pre[ct] = u;
      depth[ct] = depth[u] + 1;
      // maintain heap
      dfs(v, -1, 1, sub[ct]);
      ch[u].insert(sub[ct].top());
      // nxt
      build(ct);
    }
    ch[u].insert(0);
    if (ch[u].size() > 1) Ans.insert(ch[u].top() + ch[u].top2());
    else Ans.insert(ch[u].top());
  }

  inline void solve() { init(n), findCt(1, -1), findCt(ct, -1), build(ct); }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  // init
  Graph::init();
  // input
  read(n);
  for (int u, v, i = 1; i < n; ++i)
    read(u), read(v), Graph::AddEdge(u, v), Graph::AddEdge(v, u);
  // solve
  HLD::solve();
  TreeDivide::solve();
  for (int u = 1; u <= n; ++u)
    for (int fa = u; fa; fa = pre[fa])
      dist[depth[u] - depth[fa]][u] = HLD::Dist(u, fa);
  // output
  read(m);
  while (m--) {
    static int opt, u;
    opt = Gc();
    while (isspace(opt)) opt = Gc();
    switch (opt) {
      case 'C':
        read(u);
        if (V[u]) {
          // on --> off
          if (ch[u].size() > 1) Ans.erase(ch[u].top() + ch[u].top2());
          ch[u].insert(0);
          if (ch[u].size() > 1) Ans.insert(ch[u].top() + ch[u].top2());
          for (int i = u; pre[i]; i = pre[i]) {
            int fa = pre[i];
            if (ch[fa].size() > 1)
              Ans.erase(ch[fa].top() + ch[fa].top2());
            if (sub[i].size() > 0)
              ch[fa].erase(sub[i].top());
            sub[i].insert(dist[depth[u] - depth[fa]][u]);
            ch[fa].insert(sub[i].top());
            if (ch[fa].size() > 1)
              Ans.insert(ch[fa].top() + ch[fa].top2());
          }
        } else {
          // off --> on
          if (ch[u].size() > 1) Ans.erase(ch[u].top() + ch[u].top2());
          ch[u].erase(0);
          if (ch[u].size() > 1) Ans.insert(ch[u].top() + ch[u].top2());
          for (int i = u; pre[i]; i = pre[i]) {
            int fa = pre[i];
            if (ch[fa].size() > 1)
              Ans.erase(ch[fa].top() + ch[fa].top2());
            if (sub[i].size() > 0)
              ch[fa].erase(sub[i].top());
            sub[i].erase(dist[depth[u] - depth[fa]][u]);
            if (sub[i].size() > 0)
              ch[fa].insert(sub[i].top());
            if (ch[fa].size() > 1)
              Ans.insert(ch[fa].top() + ch[fa].top2());
          }
        }
        V[u] ^= 1;
        break;
      case 'G': printf("%d\n", Ans.size()? Ans.top(): -1); break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

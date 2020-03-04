// Luogu P2495
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

namespace IO {
    const int MAXSIZE = 1 << 18 | 1;
    char buf[MAXSIZE], *p1, *p2;

    inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
    template<typename T> inline void read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) f |= ch == '-', ch = Gc();
        while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
        if (f) x = -x;
    }
}
using IO::read;

typedef long long LL;
const int MAXN = 500005;
const LL INFLL = 0x3f3f3f3f3f3f3f3fLL;

int n, m;
int stk[MAXN], top, A[MAXN];
int depth[MAXN], dfn[MAXN], vis[MAXN];
LL dist[MAXN], f[MAXN];

namespace Graph {
    struct Edge { int nxt, to, w; } edges[MAXN];
    int head[MAXN], eidx;

    inline void init() { memset(head, -1, sizeof head), eidx = 1; }
    inline void AddEdge(int from, int to, int w = 0) {
        edges[++eidx] = (Edge){ head[from], to, w };
        head[from] = eidx;
    }

    void dfs(int u) {
        f[u] = 0;
        for (int i = head[u]; ~i; i = edges[i].nxt) {
            int v = edges[i].to, w = dist[v];
            dfs(v);
            f[u] += vis[v]? w: min(1LL * w, f[v]);
        }
    }
}

namespace HLD {
    using namespace Graph;
    int size[MAXN], son[MAXN], pre[MAXN], topfa[MAXN], dfs_clock;

    void dfs1(int u, int fa) {
        depth[u] = depth[fa] + 1;
        size[u] = 1, son[u] = -1, pre[u] = fa;
        for (int v, i = head[u]; ~i; i = edges[i].nxt) {
            if ((v = edges[i].to) == fa) continue;
            dist[v] = min(dist[u], 1LL * edges[i].w);
            dfs1(v, u), size[u] += size[v];
            if (son[u] == -1 || size[son[u]] < size[v]) son[u] = v;
        }
    }

    void dfs2(int u, int top) {
        topfa[u] = top, dfn[u] = ++dfs_clock;
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

    inline void solve() { dist[1] = INFLL, dfs1(1, 0), dfs2(1, 1); }
}

inline bool cmp(const int& x, const int& y) { return dfn[x] < dfn[y]; }

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // init
    Graph::init();
    // input
    read(n);
    for (int i = 1; i < n; ++i) {
        static int u, v, w;
        read(u), read(v), read(w); 
        Graph::AddEdge(u, v, w), Graph::AddEdge(v, u, w);
    }
    // solve
    HLD::solve();
    // output
    read(m);
    while (m--) {
        static int K;
        read(K);
        for (int i = 1; i <= K; ++i) read(A[i]), vis[A[i]] = true;
        sort(A+1, A+1+K, cmp);
        Graph::head[1] = -1, Graph::eidx = stk[top = 1] = 1;
        for (int i = 1; i <= K; ++i) if (A[i] != 1) {
            int lca = HLD::LCA(A[i], stk[top]);
            if (lca != stk[top]) {
                while (top > 1 && dfn[lca] < dfn[stk[top-1]])
                    Graph::AddEdge(stk[top-1], stk[top]), --top;
                if (top > 1 && dfn[lca] > dfn[stk[top-1]])
                    Graph::head[lca] = -1, Graph::AddEdge(lca, stk[top]), stk[top] = lca;
                else Graph::AddEdge(lca, stk[top--]);
            }
            Graph::head[A[i]] = -1, stk[++top] = A[i];
        }
        for (int i = 1; i < top; ++i) Graph::AddEdge(stk[i], stk[i+1]);
        Graph::dfs(1);
        printf("%lld\n", f[1]);
        for (int i = 1; i <= K; ++i) vis[A[i]] = false;
    }
    return 0;
}

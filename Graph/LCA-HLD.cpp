// Luogu P3379
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
        while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
        if (f) x = -x;
    }
}
using IO::read;

const int MAXN = 5e5 + 5;

int n, rt, q;

namespace Graph {
    struct Edge { int nxt, to; } edges[MAXN << 1];
    int head[MAXN], eidx;

    inline void init() { memset(head, -1, sizeof head), eidx = 1; }
    inline void AddEdge(int from, int to) {
        edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
    }
}

namespace HLD {
    using namespace Graph;
    int depth[MAXN], size[MAXN], son[MAXN], pre[MAXN], topfa[MAXN];

    void dfs1(int u, int fa) {
        depth[u] = depth[fa] + 1;
        son[u] = -1, size[u] = 1, pre[u] = fa;
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

    inline int LCA(int u, int v) {
        while (topfa[u] != topfa[v]) {
            if (depth[topfa[u]] < depth[topfa[v]]) swap(u, v);
            u = pre[topfa[u]];
        }
        return depth[u] > depth[v]? v: u;
    }

    inline void solve(int rt) { dfs1(rt, 0), dfs2(rt, rt); }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // init
    Graph::init();
    // input
    read(n), read(q), read(rt);
    for (int u, v, i = 1; i < n; ++i)
        read(u), read(v), Graph::AddEdge(u, v), Graph::AddEdge(v, u);
    // solve
    HLD::solve(rt);
    // output
    while (q--) {
        static int u, v;
        read(u), read(v), printf("%d\n", HLD::LCA(u, v));
    }
    return 0;
}

// Luogu P3387
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

const int MAXN = 1e4+5, MAXM = 1e5+5;

int n, m;
int val[MAXN], deg[MAXN];

struct Edge { int nxt, to; };
struct Graph {
    Edge edges[MAXM];
    int head[MAXN], eidx;

    Graph() { init(); }
    inline void init() { memset(head, -1, sizeof head), eidx = 1; }

    inline void AddEdge(int from, int to) {
        edges[++eidx] = (Edge){ head[from], to }, head[from] = eidx;
    }
} G1, G2;

namespace SCC {
    int dfn[MAXN], low[MAXN], clk;
    int SCCidx[MAXN], SCCval[MAXN], SCCcnt;
    int stk[MAXN], top;

    void tarjan(int u, const Graph& G) {
        dfn[u] = low[u] = ++clk, stk[++top] = u;
        for (int i = G.head[u]; ~i; i = G.edges[i].nxt) {
            int v = G.edges[i].to;
            if (!dfn[v]) {
                tarjan(v, G);
                low[u] = min(low[u], low[v]);
            } else if (!SCCidx[v]) low[u] = min(low[u], dfn[v]);
        }
        if (low[u] == dfn[u]) {
            ++SCCcnt;
            while (true) {
                int x = stk[top--];
                SCCidx[x] = SCCcnt, SCCval[SCCcnt] += val[x];
                if (x == u) break;
            }
        }
    }

    void solve() {
        clk = SCCcnt = 0;
        for (int u = 1; u <= n; ++u)
            if (!dfn[u]) tarjan(u, G1);
        for (int u = 1; u <= n; ++u)
            for (int i = G1.head[u]; ~i; i = G1.edges[i].nxt) {
                int SCCu = SCCidx[u], SCCv = SCCidx[G1.edges[i].to];
                if (SCCu != SCCv) G2.AddEdge(SCCu, SCCv), ++deg[SCCv];
            }
    }
}

namespace Topo {
    using SCC::SCCcnt; using SCC::SCCval;
    int f[MAXN];

    int Toposort(const Graph& G) {
        queue<int> Q;
        for (int i = 1; i <= SCCcnt; ++i)
            if (!deg[i]) Q.push(i), f[i] = SCCval[i];
        while (!Q.empty()) {
            int u = Q.front(); Q.pop();
            for (int i = G.head[u]; ~i; i = G.edges[i].nxt) {
                int v = G.edges[i].to;
                f[v] = max(f[v], f[u] + SCCval[v]);
                if (!(--deg[v])) Q.push(v);
            }
        }
        int ret = 0;
        for (int i = 1; i <= SCCcnt; ++i) ret = max(ret, f[i]);
        return ret;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(m);
    for (int i = 1; i <= n; ++i) read(val[i]);
    for (int u, v, i = 1; i <= m; ++i)
        read(u), read(v), G1.AddEdge(u, v);
    // solve
    SCC::solve();
    // output
    printf("%d\n", Topo::Toposort(G2));
    return 0;
}

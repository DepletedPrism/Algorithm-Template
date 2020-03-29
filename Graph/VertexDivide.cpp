// Luogu P3806
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

const int MAXN = 1e4 + 5, MAXK = 1e7 + 5, MAXM = 1e2 + 5;

int n, m;
int Q[MAXM];
bool Ans[MAXM];

namespace Graph {
    struct Edge { int nxt, to, w; } edges[MAXN << 1];
    int head[MAXN], eidx;

    inline void init() { memset(head, -1, sizeof head), eidx = 1; }
    inline void AddEdge(int from, int to, int w) {
        edges[++eidx] = (Edge){ head[from], to, w }, head[from] = eidx;
    }
}

namespace TreeDiv {
    using namespace Graph;
    bool vis[MAXN], Judge[MAXK];
    int Bal[MAXN], size[MAXN], subsize, ct;
    int dist[MAXN], subdist[MAXN], stk[MAXN], subidx, top;

    inline void init(const int& x) { subsize = x, Bal[ct = 0] = MAXN; }

    void findCt(int u, int fa) {
        Bal[u] = 0, size[u] = 1;
        for (int v, i = head[u]; ~i; i = edges[i].nxt) {
            if ((v = edges[i].to) == fa || vis[v]) continue;
            findCt(v, u), size[u] += size[v];
            Bal[u] = max(Bal[u], size[v]);
        }
        Bal[u] = max(Bal[u], subsize - size[u]);
        if (Bal[u] <= Bal[ct]) ct = u;
    }

    void dfs(int u, int fa) {
        subdist[++subidx] = dist[u];
        for (int v, i = head[u]; ~i; i = edges[i].nxt) {
            if ((v = edges[i].to) == fa || vis[v]) continue;
            dist[v] = dist[u] + edges[i].w, dfs(v, u);
        }
    }

    inline void calc(int u) {
        Judge[0] = true;
        stk[top = 0] = 0;
        for (int v, i = head[u]; ~i; i = edges[i].nxt) {
            if (vis[v = edges[i].to]) continue;
            subidx = 0, dist[v] = edges[i].w;
            dfs(v, u);
            for (int j = 1; j <= subidx; ++j)
                for (int k = 1; k <= m; ++k)
                    if (subdist[j] <= Q[k]) Ans[k] |= Judge[Q[k] - subdist[j]];
            for (int j = 1; j <= subidx; ++j)
                if (subdist[j] < MAXK) Judge[subdist[j]] = true, stk[++top] = subdist[j];
        }
        while (top) Judge[stk[top--]] = false;
    }

    void Divide(int u) {
        vis[u] = true, calc(u);
        for (int v, i = head[u]; ~i; i = edges[i].nxt) {
            if (vis[v = edges[i].to]) continue;
            init(size[v]), findCt(v, 0), Divide(ct);
        }
    }

    inline void solve() { findCt(1, 0), Divide(ct); }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // init
    Graph::init();
    // input
    read(n), read(m);
    for (int u, v, w, i = 1; i < n; ++i) {
        read(u), read(v), read(w);
        Graph::AddEdge(u, v, w), Graph::AddEdge(v, u, w);
    }
    for (int i = 1; i <= m; ++i) read(Q[i]);
    // solve
    TreeDiv::solve();
    // output
    for (int i = 1; i <= m; ++i) puts(Ans[i]? "AYE": "NAY");
    return 0;
}

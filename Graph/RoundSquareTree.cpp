// Luogu P4630
// DeP
#include <stack>
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
        while (!isdigit(ch)) { f |= ch == '-'; ch = Gc(); }
        while (isdigit(ch)) { x = x * 10 + ch - '0'; ch = Gc(); }
        if (f) x = -x;
    }
}
using IO::read;

typedef long long LL;
const int MAXN = 100005, MAXM = 200005;

int n, m, SqIdx;
int weight[MAXN << 1];

struct Edge { int nxt, to; };

struct Graph {
    Edge edges[MAXM << 1];
    int head[MAXN << 1], eidx;

    Graph() { eidx = 0; memset(head, -1, sizeof head); }

    inline void AddEdge(int from, int to) {
        edges[++eidx] = (Edge){ head[from], to };
        head[from] = eidx;
    }
} G, T;

namespace BCC {
    int size[MAXN << 1], cnt; LL ans;
    int dfn[MAXN], low[MAXN], dfs_clock;
    stack<int> S;

    void tarjan(int u) {
        low[u] = dfn[u] = ++dfs_clock;
        S.push(u), ++cnt;
        for (int v, i = G.head[u]; ~i; i = G.edges[i].nxt) {
            if (!dfn[v = G.edges[i].to]) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
                if (low[v] == dfn[u]) {
                    weight[++SqIdx] = 0;
                    while (true) {
                        int x = S.top(); S.pop();
                        ++weight[SqIdx];
                        T.AddEdge(SqIdx, x), T.AddEdge(x, SqIdx);
                        if (x == v) break;
                    }
                    ++weight[SqIdx];
                    T.AddEdge(SqIdx, u), T.AddEdge(u, SqIdx);
                }
            } else low[u] = min(low[u], dfn[v]);
        }
    }

    void dfs(int u, int fa) {
        size[u] = u <= n;
        for (int v, i = T.head[u]; ~i; i = T.edges[i].nxt) {
            if ((v = T.edges[i].to) == fa) continue;
            dfs(v, u);
            ans += 2LL * weight[u] * size[u] * size[v];
            size[u] += size[v];
        }
        ans += 2LL * weight[u] * size[u] * (cnt - size[u]);
    }

    void findBCC() {
        dfs_clock = 0, SqIdx = n;
        for (int i = 1; i <= n; ++i) weight[i] = -1;
        for (int i = 1; i <= n; ++i) if (!dfn[i])
            cnt = 0, tarjan(i), S.pop(), dfs(i, -1);
        printf("%lld\n", ans);
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("182", "r", stdin);
#endif
    read(n), read(m);
    for (int u, v, i = 1; i <= m; ++i)
        read(u), read(v), G.AddEdge(u, v), G.AddEdge(v, u);
    BCC::findBCC();
    return 0;
}

// CF 570D
// DeP
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

typedef pair<int, int> Pii;
const int MAXN = 5e5+5;

int n, m;
char A[MAXN];
bool Ans[MAXN];
vector<Pii> Q[MAXN];

inline int lowbit(int x) { return x & -x; }

namespace Graph {
    struct Edge { int nxt, to; } edges[MAXN];
    int head[MAXN], eidx;
    int depth[MAXN], size[MAXN], pre[MAXN], dfn[MAXN], son[MAXN], dfs_clock;
    int check[MAXN]; bool vis[MAXN];

    inline void init() { memset(head, -1, sizeof head), eidx = 1; }
    inline void AddEdge(int from, int to) {
        edges[++eidx] = (Edge){ head[from], to };
        head[from] = eidx;
    }

    void dfs(int u) {
        depth[u] = depth[pre[u]] + 1;
        size[u] = 1, son[u] = -1, dfn[u] = ++dfs_clock;
        for (int v, i = head[u]; ~i; i = edges[i].nxt) {
            dfs(v = edges[i].to); size[u] += size[v];
            if (son[u] == -1 || size[v] > size[son[u]]) son[u] = v;
        }
    }

    void calc(int u) {
        check[depth[u]] ^= 1 << (A[u] - 'a');
        for (int v, i = head[u]; ~i; i = edges[i].nxt)
            if (!vis[v = edges[i].to]) calc(v);
    }

    void dfs(int u, bool keep) {
        for (int v, i = head[u]; ~i; i = edges[i].nxt)
            if ((v = edges[i].to) != son[u]) dfs(v, false);
        if (~son[u]) dfs(son[u], true), vis[son[u]] = true;
        calc(u);
        if (~son[u]) vis[son[u]] = false;
        for (size_t k = 0; k < Q[u].size(); ++k) {
            int idx = Q[u][k].first, d = Q[u][k].second;
            Ans[idx] = check[d] == lowbit(check[d]);
        }
        if (!keep) calc(u);
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // init
    Graph::init();
    // input
    scanf("%d%d", &n, &m);
    for (int i = 2; i <= n; ++i)
        scanf("%d", Graph::pre+i), Graph::AddEdge(Graph::pre[i], i);
    scanf("%s", A+1);
    for (int u, d, i = 1; i <= m; ++i) {
        scanf("%d%d", &u, &d);
        Q[u].push_back(Pii(i, d));
    }
    // solve
    Graph::dfs(1);
    Graph::dfs(1, false);
    // output
    for (int i = 1; i <= m; ++i) puts(Ans[i]? "Yes": "No");
    return 0;
}

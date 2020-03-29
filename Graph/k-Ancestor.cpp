// Luogu P5903
// DeP
#include <cctype>
#include <cstdio>
#include <vector>
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

typedef long long LL;
const int MAXN = 5e5 + 5, LOG = 21;

int n, q;
unsigned s;
int pre[LOG][MAXN], depth[MAXN], lg2[MAXN];

inline unsigned get(unsigned x) {
	return x ^= x << 13, x ^= x >> 17, x ^= x << 5, s = x;
}

namespace SLD {
    vector<int> G[MAXN], U[MAXN], D[MAXN];
    int son[MAXN], d[MAXN], topfa[MAXN];

    inline void AddEdge(int from, int to) { G[from].push_back(to); }

    void dfs1(int u, int fa) {
        depth[u] = d[u] = depth[fa] + 1;
        for (int i = 1; pre[i-1][u]; ++i) pre[i][u] = pre[i-1][pre[i-1][u]];
        for (auto v: G[u]) {
            dfs1(v, u), d[u] = max(d[u], d[v]);
            if (d[v] > d[son[u]]) son[u] = v;
        }
    }

    void dfs2(int u, int top) {
        topfa[u] = top;
        if (u == top) {
            for (int i = 0, v = u; i <= d[u] - depth[u]; ++i)
                U[u].push_back(v), v = pre[0][v];
            for (int i = 0, v = u; i <= d[u] - depth[u]; ++i)
                D[u].push_back(v), v = son[v];
        }
        if (son[u]) dfs2(son[u], top);
        for (auto v: G[u]) if (v != son[u]) dfs2(v, v);
    }

    inline void solve(int rt) { dfs1(rt, 0), dfs2(rt, rt); }

    inline int Jmp(int u, int k) {
        if (!k) return u;
        const int& w = lg2[k];
        u = pre[w][u];
        const int& fa = topfa[u];
        k -= 1 << lg2[k], k -= depth[u] - depth[fa];
        return k >= 0? U[fa][k]: D[fa][-k];
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(q), read(s);
    for (int i = 1; i <= n; ++i) read(pre[0][i]);
    // solve
    lg2[0] = -1;
    for (int i = 1; i <= n; ++i) lg2[i] = lg2[i / 2] + 1;
    int rt = -1;
    for (int i = 1; i <= n; ++i)
        if (pre[0][i]) SLD::AddEdge(pre[0][i], i); else rt = i;
    SLD::solve(rt);
    // output
    LL ans = 0;
    for (int x, k, i = 1, lst = 0; i <= q; ++i) {
        x = (get(s) ^ lst) % n + 1, k = (get(s) ^ lst) % depth[x];
        ans ^= 1LL * i * (lst = SLD::Jmp(x, k));
    }
    printf("%lld\n", ans);
    return 0;
}

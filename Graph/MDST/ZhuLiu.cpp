// LOJ #140
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>

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

const int MAXN = 105, MAXM = 1e4 + 5;
const int INF = 0x3f3f3f3f;

struct Edge {
    int u, v, w;
} E[MAXM];

int n, m, rt;

namespace MDST {
    int id[MAXN], vis[MAXN], pre[MAXN], in[MAXN];

    int ZhuLiu() {
        int ret = 0;
        while (true) {
            memset(in, 0x3f, (n + 1) * sizeof (int));
            for (int i = 1; i <= m; ++i) {
                int u = E[i].u, v = E[i].v;
                if (u != v && E[i].w < in[v]) in[v] = E[i].w, pre[v] = u;
            }
            for (int u = 1; u <= n; ++u)
                if (u != rt && in[u] >= INF) return -1;
            for (int u = 1; u <= n; ++u) id[u] = vis[u] = -1;
            int idx = in[rt] = 0;
            for (int v, u = 1; u <= n; ++u) {
                ret += in[u], v = u;
                while (vis[v] != u && id[v] == -1 && v != rt)
                    vis[v] = u, v = pre[v];
                if (v != rt && id[v] == -1) {
                    id[v] = ++idx;
                    for (int i = pre[v]; i != v; i = pre[i]) id[i] = idx;
                }
            }
            if (idx == 0) break;
            for (int u = 1; u <= n; ++u)
                if (id[u] == -1) id[u] = ++idx;
            for (int i = 1; i <= m; ++i) {
                int u = E[i].u, v = E[i].v;
                E[i].u = id[u], E[i].v = id[v];
                if (id[u] != id[v]) E[i].w -= in[v];
            }
            n = idx, rt = id[rt];
        }
        return ret;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(m), read(rt);
    for (int u, v, w, i = 1; i <= m; ++i)
        read(u), read(v), read(w), E[i] = (Edge){ u, v, w };
    // solve
    printf("%d\n", MDST::ZhuLiu());
    return 0;
}

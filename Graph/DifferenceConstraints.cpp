// Luogu P5960
// DeP
#include <queue>
#include <cctype>
#include <cstdio>
#include <cstring>
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

const int MAXN = 5e3 + 5;

int n, m;
int d[MAXN];

namespace Graph {
    struct Edge { int nxt, to, w; } edges[MAXN + MAXN];
    bool inq[MAXN];
    int cnt[MAXN], head[MAXN], eidx;
 
    inline void init() { memset(head, -1, sizeof head), eidx = 1; }
    inline void AddEdge(int from, int to, int w) {
        edges[++eidx] = (Edge){ head[from], to, w }, head[from] = eidx;
    }

    bool SPFA(int s) {
        queue<int> Q;
        memset(d, 0x3f, sizeof d);
        inq[s] = true, d[s] = 0, Q.push(s);
        while (!Q.empty()) {
            int u = Q.front(); Q.pop();
            inq[u] = false;
            for (int v, i = head[u]; ~i; i = edges[i].nxt)
                if (d[v = edges[i].to] > d[u] + edges[i].w) {
                    d[v] = d[u] + edges[i].w;
                    if (!inq[v]) {
                        inq[v] = true, Q.push(v);
                        if (++cnt[v] == n) return false;
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
    // init
    Graph::init();
    // input
    read(n), read(m);
    for (int u, v, w, i = 1; i <= m; ++i)
        read(u), read(v), read(w), Graph::AddEdge(v, u, w);
    int S = 0;
    for (int i = 1; i <= n; ++i) Graph::AddEdge(S, i, 0);
    // solve
    if (!Graph::SPFA(S)) puts("NO");
    else {
        for (int i = 1; i <= n; ++i) printf("%d%c", d[i], " \n"[i == n]);
    }
    return 0;
}

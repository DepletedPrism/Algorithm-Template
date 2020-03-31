// Luogu P5905
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

typedef long long LL;
typedef pair<int, int> Pii;

const int MAXN = 3e3 + 5, MAXM = 6e3 + 5, INF = 1e9;

int n, m;

namespace Graph {
    struct Edge { int nxt, to, w; } edges[MAXM + MAXN];
    int head[MAXN], eidx;

    inline void init() { memset(head, -1, sizeof head), eidx = 1; }
    inline void AddEdge(int from, int to, int w) {
        edges[++eidx] = (Edge){ head[from], to, w }, head[from] = eidx;
    }
}

namespace Jhonson {
    using namespace Graph;
    bool inq[MAXN];
    int dist[MAXN], h[MAXN], cnt[MAXN];

    bool SPFA(int s) {
        queue<int> Q;
        h[s] = 0, inq[s] = true, Q.push(s);
        while (!Q.empty()) {
            int u = Q.front(); Q.pop();
            inq[u] = false;
            for (int v, i = head[u]; ~i; i = edges[i].nxt)
                if (h[v = edges[i].to] > h[u] + edges[i].w) {
                    h[v] = h[u] + edges[i].w;
                    if (!inq[v]) {
                        Q.push(v), inq[v] = true;
                        if (++cnt[v] == n) return false;
                    }
                }
        }
        return true;
    }

    void Dijkstra(int s) {
        priority_queue<Pii, vector<Pii>, greater<Pii> > PQ;
        PQ.push(Pii(dist[s] = 0, s));
        while (!PQ.empty()) {
            int di = PQ.top().first, u = PQ.top().second; PQ.pop();
            if (di != dist[u]) continue;
            for (int i = head[u]; ~i; i = edges[i].nxt) {
                int v = edges[i].to, w = edges[i].w + h[u] - h[v];
                if (dist[v] > dist[u] + w) PQ.push(Pii(dist[v] = dist[u] + w, v));
            }
        }
    }

    bool solve() {
        for (int i = 1; i <= n; ++i)
            Graph::AddEdge(0, i, 0);
        for (int i = 1; i <= n; ++i) h[i] = INF;
        if (!SPFA(0)) return false;
        for (int s = 1; s <= n; ++s) {
            for (int i = 1; i <= n; ++i) dist[i] = INF;
            Dijkstra(s);
            LL now = 0;
            for (int i = 1; i <= n; ++i)
                now += 1LL * i * ((dist[i] == INF) ? INF: dist[i] + h[i] - h[s]);
            printf("%lld\n", now);
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
        read(u), read(v), read(w), Graph::AddEdge(u, v, w);
    // solve & output
    if (!Jhonson::solve()) puts("-1");
    return 0;
}

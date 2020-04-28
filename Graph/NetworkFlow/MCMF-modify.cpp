// LOJ #102
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
        while (isdigit(ch)) x = x * 10 - '0' + ch, ch = Gc();
        if (f) x = -x;
    }
}
using IO::read;

const int MAXV = 405, MAXE = 15005;
const int INF = 0x3f3f3f3f;

int n, m, S, T;

namespace Graph {
    struct Edge { int nxt, to, cap, flow, cost; } edges[MAXE << 1];
    int head[MAXV], eidx;

    inline void init() { memset(head, -1, sizeof head), eidx = 1; }
    inline void AddEdge(int from, int to, int c, int w) {
        edges[++eidx] = (Edge){ head[from], to, c, 0, w }, head[from] = eidx;
        edges[++eidx] = (Edge){ head[to], from, 0, 0, -w }, head[to] = eidx;
    }
}

namespace MCMF {
    using namespace Graph;
    int cur[MAXV], d[MAXV], vis[MAXV], Time;

    bool SPFA() {
        queue<int> Q;
        memset(d, 0x3f, sizeof d);
        Q.push(S);
        vis[S] = ++Time, d[S] = 0;
        while (!Q.empty()) {
            int u = Q.front(); Q.pop();
            vis[u] = 0;
            for (int i = head[u]; ~i; i = edges[i].nxt) {
                const Edge& e = edges[i];
                if (d[e.to] > d[u] + e.cost && e.cap > e.flow) {
                    d[e.to] = d[u] + e.cost;
                    if (vis[e.to] != Time) Q.push(e.to), vis[e.to] = Time;
                }
            }
        }
        return d[T] < INF;
    }

    int DFS(int u, int a, int& cost) {
        if (u == T || !a) return a;
        vis[u] = Time;
        int f, flow = 0;
        for (int& i = cur[u]; ~i; i = edges[i].nxt) {
            Edge& e = edges[i];
            if (vis[e.to] != Time && d[e.to] == d[u] + e.cost &&
                    (f = DFS(e.to, min(a, e.cap - e.flow), cost)) > 0) {
                cost += f * edges[i].cost;
                flow += f, a -= f, e.flow += f, edges[i ^ 1].flow -= f;
                if (!a) break;
            }
        }
        return flow;
    }

    void MCMF(int& flow, int& cost) {
        flow = cost = 0;
        while (SPFA()) {
            do {
                memcpy(cur, head, sizeof cur), ++Time;
                flow += DFS(S, INF, cost);
            } while (vis[T] == Time);
        }
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
    S = 1, T = n;
    for (int i = 1; i <= m; ++i) {
        static int u, v, c, w;
        read(u), read(v), read(c), read(w);
        Graph::AddEdge(u, v, c, w);
    }
    // output
    int flow, cost;
    MCMF::MCMF(flow, cost);
    printf("%d %d\n", flow, cost);
    return 0;
}

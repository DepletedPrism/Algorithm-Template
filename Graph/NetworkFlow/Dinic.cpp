// LOJ #101
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

typedef long long LL;
const int MAXV = 105, MAXE = 5005;
const LL INFLL = 0x3f3f3f3f3f3f3f3fLL;

int n, m, S, T;

namespace Graph {
    struct Edge { int nxt, to; LL cap, flow; } edges[MAXE << 1];
    int head[MAXV], eidx;

    inline void init() { memset(head, -1, sizeof head); eidx = 1; }
    inline void AddEdge(int from, int to, LL cap) {
        edges[++eidx] = (Edge){ head[from], to, cap, 0 };
        head[from] = eidx;
        edges[++eidx] = (Edge){ head[to], from, 0, 0 };
        head[to] = eidx;
    }
}

namespace Dinic {
    using namespace Graph;
    int depth[MAXV], cur[MAXV], vis[MAXV], Time;

    bool BFS() {
        queue<int> Q;
        Q.push(S), ++Time;
        depth[S] = 1, vis[S] = Time;
        while (!Q.empty()) {
            int u = Q.front(); Q.pop();
            for (int i = head[u]; ~i; i = edges[i].nxt) {
                const Edge& e = edges[i];
                if (vis[e.to] != Time && e.cap > e.flow)
                    vis[e.to] = Time, depth[e.to] = depth[u] + 1, Q.push(e.to);
            }
        }
        return vis[T] == Time;
    }

    LL DFS(int u, LL a) {
        if (u == T || !a) return a;
        LL f, flow = 0;
        for (int& i = cur[u]; ~i; i = edges[i].nxt) {
            Edge& e = edges[i];
            if (depth[e.to] == depth[u] + 1 && (f = DFS(e.to, min(a, e.cap-e.flow))) > 0) {
                flow += f, a -= f, e.flow += f, edges[i^1].flow -= f;
                if (!a) break;
            }
        }
        return flow;
    }

    LL Maxflow() {
        LL flow = 0;
        while (BFS()) memcpy(cur, head, sizeof cur), flow += DFS(S, INFLL);
        return flow;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // init
    Graph::init();
    // input
    read(n), read(m), read(S), read(T);
    while (m--) {
        static int u, v, c;
        read(u), read(v), read(c), Graph::AddEdge(u, v, c);
    }
    // solve & output
    printf("%lld\n", Dinic::Maxflow());
    return 0;
}

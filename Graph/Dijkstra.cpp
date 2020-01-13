// LOJ #119
// DeP
#include <queue>
#include <cctype>
#include <cstdio>
#include <cstring>
using namespace std;

namespace IO {
    const int MAXSIZE = 1 << 18 | 1;
    char buf[MAXSIZE], *p1, *p2;

    inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
    template<typename T> void read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) f |= ch == '-', ch = Gc();
        while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
        if (f) x = -x;
    }
}
using IO::read;

typedef long long LL;
typedef pair<LL, int> Pli;
const int MAXN = 2505, MAXM = 6205;

int n, m, s, t;

namespace Graph {
    struct Edge { int nxt, to, w; } edges[MAXM << 1];
    int head[MAXN], eidx;

    inline void init() { memset(head, -1, sizeof head); eidx = 1; }

    inline void AddEdge(int from, int to, int w) {
        edges[++eidx] = (Edge){ head[from], to, w };
        head[from] = eidx;
    }
}

namespace Dijkstra {
    using namespace Graph;
    bool done[MAXN];
    LL d[MAXN];

    void dijkstra(int s) {
        priority_queue<Pli> PQ;
        memset(d, 0x3f, sizeof d);
        memset(done, false, sizeof done);
        d[s] = 0;
        PQ.push(Pli(-d[s], s));
        while (!PQ.empty()) {
            int u = PQ.top().second; PQ.pop();
            if (done[u]) continue;
            done[u] = true;
            for (int i = head[u]; ~i; i = edges[i].nxt) {
                int v = edges[i].to;
                if (d[v] > d[u] + edges[i].w)
                    d[v] = d[u] + edges[i].w, PQ.push(Pli(-d[v], v));
            }
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
    read(n), read(m), read(s), read(t);
    for (int i = 1; i <= m; ++i) {
        int u, v, w;
        read(u), read(v), read(w);
        Graph::AddEdge(u, v, w), Graph::AddEdge(v, u, w);
    }
    // solve
    Dijkstra::dijkstra(s);
    // output
    printf("%lld\n", Dijkstra::d[t]);
    return 0;
}

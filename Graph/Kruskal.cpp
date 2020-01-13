// LOJ #123
// DeP
#include <cctype>
#include <cstdio>
#include <algorithm>
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
const int MAXN = 2e5+5, MAXM = 5e5+5;

struct Edge {
    int from, to, w;
    bool operator < (const Edge& rhs) const {
        return w < rhs.w;
    }
} edges[MAXM];

int n, m;

namespace DSU {
    int fa[MAXN], rnk[MAXN];

    inline void init() {
        for (int i = 1; i <= n; ++i)
            fa[i] = i, rnk[i] = 1;
    }

    int findfa(int x) {
        return fa[x] = fa[x] == x? x: findfa(fa[x]);
    }
    inline bool join(int x, int y) {
        x = findfa(x), y = findfa(y);
        if (x == y) return false;
        if (rnk[x] < rnk[y]) fa[x] = y;
        else {
            fa[y] = x;
            if (rnk[x] == rnk[y]) ++rnk[x];
        }
        return true;
    }
}

namespace MST {
    LL Kruskal() {
        LL ret = 0; int cnt = 0;
        DSU::init();
        sort(edges+1, edges+1+m);
        for (int i = 1; i <= m; ++i) {
            const Edge& e = edges[i];
            if (DSU::join(e.from, e.to)) {
                ret += e.w;
                if (++cnt == n-1) return ret;
            }
        }
        return 0;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(m);
    for (int i = 1; i <= m; ++i) {
        int u, v, w;
        read(u), read(v), read(w);
        edges[i] = (Edge){ u, v, w };
    }
    // output
    printf("%lld\n", MST::Kruskal());
    return 0;
}

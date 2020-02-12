// Luogu P3346
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
const int MAXN = 2000005, SIGMA = 10;

int n, rt, C; LL ans;
int col[MAXN], deg[MAXN];

namespace SAM {
    const int MAXN = ::MAXN << 1;
    int ch[MAXN][SIGMA], pos[MAXN], len[MAXN], lnk[MAXN], nidx;

    inline void init() { nidx = 1; }

    inline int insert(int val, int last) {
        int p = last;
        if (ch[p][val] && len[ch[p][val]] == len[p] + 1)
            return ch[p][val];
        int nd = ++nidx, flag = 0;
        len[nd] = len[last] + 1;
        while (p && !ch[p][val]) ch[p][val] = nd, p = lnk[p];
        if (!p) lnk[nd] = 1;
        else {
            int q = ch[p][val];
            if (len[q] == len[p] + 1) lnk[nd] = q;
            else {
                int nxt = ++nidx;
                if (len[nd] == len[p] + 1) flag = 1;
                // if (p == last) flag = 1;
                len[nxt] = len[p] + 1, lnk[nxt] = lnk[q];
                memcpy(ch[nxt], ch[q], sizeof ch[nxt]);
                while (p && ch[p][val] == q) ch[p][val] = nxt, p = lnk[p];
                lnk[nd] = lnk[q] = nxt;
                if (flag) return nxt;
            }
        }
        ans += len[nd] - len[lnk[nd]];
        return nd;
    }
}

namespace Graph {
    struct Edge { int nxt, to; } edges[MAXN << 1];
    int head[MAXN], eidx;

    inline void init() { memset(head, -1, sizeof head), eidx = 1; }
    inline void AddEdge(int from, int to) {
        edges[++eidx] = (Edge){ head[from], to };
        head[from] = eidx;
    }

    void dfs(int u, int fa, int Triefa) {
        int nd = SAM::insert(col[u], Triefa);
        for (int v, i = head[u]; ~i; i = edges[i].nxt)
            if ((v = edges[i].to) != fa) dfs(v, u, nd);
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("2.in", "r", stdin);
#endif
    read(n), read(C);
    // init
    Graph::init(), SAM::init();
    // input
    for (int i = 1; i <= n; ++i) read(col[i]);
    for (int i = 1; i < n; ++i) {
        static int u, v;
        read(u), read(v);
        Graph::AddEdge(u, v), Graph::AddEdge(v, u);
        ++deg[u], ++deg[v];
    }
    // solve
    for (int i = 1; i <= n; ++i)
        if (deg[i] == 1) Graph::dfs(i, -1, 1);
    // output
    printf("%lld\n", ans);
    return 0;
}

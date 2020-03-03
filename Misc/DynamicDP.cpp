// Luogu P4719
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

namespace IO {
    const int MAXSIZE = 1 << 18 | 1;
    char buf[MAXSIZE], *p1, *p2;

    inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
    template<typename T> inline void read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) f |= ch == '-', ch = Gc();
        while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
        if (f) x = -x;
    }
}
using IO::read;

const int MAXN = 1e5+5, INF = 0x3f3f3f3f;

struct Matrix {
    int g[2][2];
    Matrix() { memset(g, 0, sizeof g); }
    Matrix operator * (const Matrix& rhs) const {
        Matrix ret;
        for (int i = 0; i < 2; ++i)
            for (int k = 0; k < 2; ++k) {
                ret.g[i][0] = max(ret.g[i][0], g[i][k] + rhs.g[k][0]);
                ret.g[i][1] = max(ret.g[i][1], g[i][k] + rhs.g[k][1]);
            }
        return ret;
    }
};

int n, m;
Matrix A[MAXN];
int W[MAXN], f[2][MAXN];

namespace Graph {
    struct Edge { int nxt, to; } edges[MAXN << 1];
    int head[MAXN], eidx;

    inline void init() { memset(head, -1, sizeof head), eidx = 1; }
    inline void AddEdge(int from, int to) {
        edges[++eidx] = (Edge){ head[from], to };
        head[from] = eidx;
    }
}

namespace HLD {
    using namespace Graph;
    int depth[MAXN], pre[MAXN], size[MAXN], son[MAXN];
    int topfa[MAXN], Ed[MAXN], dfn[MAXN], rnk[MAXN], dfs_clock;

    void dfs1(int u, int fa) {
        depth[u] = depth[fa] + 1;
        size[u] = 1, son[u] = -1, pre[u] = fa;
        f[1][u] = W[u];
        for (int v, i = head[u]; ~i; i = edges[i].nxt) {
            if ((v = edges[i].to) == fa) continue;
            dfs1(v, u), size[u] += size[v];
            if (son[u] == -1 || size[v] > size[son[u]]) son[u] = v;
            f[1][u] += f[0][v], f[0][u] += max(f[0][v], f[1][v]);
        }
    }

    void dfs2(int u, int top) {
        dfn[u] = ++dfs_clock, rnk[dfs_clock] = u;
        topfa[u] = top, Ed[top] = dfs_clock;
        A[u].g[1][0] = W[u], A[u].g[1][1] = -INF;
        if (~son[u]) dfs2(son[u], top);
        for (int v, i = head[u]; ~i; i = edges[i].nxt) {
            if ((v = edges[i].to) == pre[u] || v == son[u]) continue;
            dfs2(v, v);
            A[u].g[1][0] += f[0][v], A[u].g[0][0] += max(f[0][v], f[1][v]);
        }
        A[u].g[0][1] = A[u].g[0][0];
    }

    inline void solve(int root = 1) { dfs1(root, 0), dfs2(root, root); }
}

namespace SGT {
#define lc (nd<<1)
#define rc (nd<<1|1)
    Matrix dat[MAXN << 2];

    inline void maintain(int nd) { dat[nd] = dat[lc] * dat[rc]; }

    void build(int nd, int L, int R) {
        if (L == R) return void( dat[nd] = A[HLD::rnk[L]] );
        int Mid = (L + R) / 2;
        build(lc, L, Mid), build(rc, Mid+1, R);
        maintain(nd);
    }

    void Mdy(int nd, int L, int R, const int& pos) {
        if (L == R) return void( dat[nd] = A[HLD::rnk[L]] );
        int Mid = (L + R) / 2;
        if (pos <= Mid) Mdy(lc, L, Mid, pos);
        else Mdy(rc, Mid+1, R, pos);
        maintain(nd);
    }

    Matrix Qry(int nd, int L, int R, const int& opL, const int& opR) {
        if (opL <= L && R <= opR) return dat[nd];
        int Mid = (L + R) / 2;
        if (opR <= Mid) return Qry(lc, L, Mid, opL, opR);
        if (opL > Mid) return Qry(rc, Mid+1, R, opL, opR);
        return Qry(lc, L, Mid, opL, opR) * Qry(rc, Mid+1, R, opL, opR);
    }
#undef lc
#undef rc
}

namespace HLD {
    void Mdy(int u, const int& val) {
        A[u].g[1][0] += val - W[u];
        W[u] = val;
        while (u > 0) {
            Matrix lst = SGT::Qry(1, 1, n, dfn[topfa[u]], Ed[topfa[u]]);
            SGT::Mdy(1, 1, n, dfn[u]);
            Matrix now = SGT::Qry(1, 1, n, dfn[topfa[u]], Ed[topfa[u]]);
            u = pre[topfa[u]];
            A[u].g[0][0] += max(now.g[0][0], now.g[1][0]) - max(lst.g[0][0], lst.g[1][0]);
            A[u].g[0][1] = A[u].g[0][0];
            A[u].g[1][0] += now.g[0][0] - lst.g[0][0];
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
    for (int i = 1; i <= n; ++i) read(W[i]);
    for (int u, v, i = 1; i < n; ++i)
        read(u), read(v), Graph::AddEdge(u, v), Graph::AddEdge(v, u);
    // solve
    HLD::solve(), SGT::build(1, 1, n);
    // output
    while (m--) {
        static int u, val;
        read(u), read(val);
        HLD::Mdy(u, val);
        Matrix res = SGT::Qry(1, 1, n, 1, HLD::Ed[1]);
        printf("%d\n", max(res.g[0][0], res.g[1][0]));
    }
    return 0;
}

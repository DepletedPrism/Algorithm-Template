// Luogu P3690
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

const int MAXN = 1e5+5;

int n, m;

namespace LCT {
    int ch[2][MAXN], pre[MAXN], topfa[MAXN], val[MAXN];
    int datXor[MAXN]; bool tagRes[MAXN];

    inline void maintain(int nd) {
        datXor[nd] = val[nd];
        if (ch[0][nd]) datXor[nd] ^= datXor[ch[0][nd]];
        if (ch[1][nd]) datXor[nd] ^= datXor[ch[1][nd]];
    }
    inline void pushRes(int nd) {
        tagRes[nd] ^= 1, swap(ch[0][nd], ch[1][nd]);
    }
    inline void pushdown(int nd) {
        if (!nd || !tagRes[nd]) return;
        if (ch[0][nd]) pushRes(ch[0][nd]);
        if (ch[1][nd]) pushRes(ch[1][nd]);
        tagRes[nd] = false;
    }

    inline int which(int u) { return pre[u]? ch[1][pre[u]] == u: 0; }
    inline bool nroot(int u) { return pre[u]? ch[0][pre[u]] == u || ch[1][pre[u]] == u: false; }
    inline void rotate(int u) {
        int fa = pre[u], w = which(u);
        pre[u] = pre[fa];
        if (nroot(fa)) ch[which(fa)][pre[fa]] = u;
        ch[w][fa] = ch[w^1][u];
        if (ch[w^1][u]) pre[ch[w^1][u]] = fa;
        ch[w^1][u] = fa, pre[fa] = u;
        maintain(fa);
    }
    inline void splay(int u) {
        static int st[MAXN], top, fa;
        top = 0, st[++top] = fa = u;
        while (nroot(fa)) st[++top] = fa = pre[fa];
        while (top) pushdown(st[top--]);
        while (nroot(u)) {
            fa = pre[u];
            if (nroot(fa)) which(fa) == which(u)? rotate(fa): rotate(u);
            rotate(u);
        }
        maintain(u);
    }

    inline void access(int u) {
        for (int v = 0; u; v = u, u = pre[u])
            splay(u), ch[1][u] = v, maintain(u);
    }
    inline void evert(int u) { access(u), splay(u), pushRes(u); }

    int findRoot(int u) {
        access(u), splay(u);
        while (ch[0][u]) pushdown(u), u = ch[0][u];
        return splay(u), u;
    }

    inline void Lnk(int u, int v) {
        evert(v);
        if (findRoot(u) != v) pre[v] = u;
    }
    inline void Cut(int u, int v) {
        evert(u);
        if (findRoot(v) != u || pre[v] != u || ch[0][v]) return;
        pre[v] = ch[1][u] = 0;
        maintain(u);
    }
    inline void split(int u, int v) { evert(u), access(v), splay(v); }

    inline int Qry(int u, int v) { split(u, v); return datXor[v]; }
    inline void Mdy(int u, int x) { splay(u), val[u] = x, maintain(u); }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(m);
    for (int i = 1; i <= n; ++i) read(LCT::val[i]);
    // solve
    while (m--) {
        int opt, u, v;
        read(opt), read(u), read(v);
        switch (opt) {
            case 0: printf("%d\n", LCT::Qry(u, v)); break;
            case 1: LCT::Lnk(u, v); break;
            case 2: LCT::Cut(u, v); break;
            case 3: LCT::Mdy(u, v); break;
            default: fprintf(stderr, "ERR\n");
        }
    }
    return 0;
}

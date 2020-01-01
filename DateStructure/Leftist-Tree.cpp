// Luogu P3377
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

namespace Heap {
    int ch[2][MAXN], rt[MAXN], dist[MAXN], val[MAXN];

    inline void init() { for (int i = 1; i <= n; ++i) rt[i] = i; }
    int findrt(int u) { return rt[u] == u? rt[u]: rt[u] = findrt(rt[u]); }

    int Merge(int x, int y) {
        if (!x || !y) return x + y;
        if (val[x] > val[y] || (val[x] == val[y] && x > y)) swap(x, y);
        ch[1][x] = Merge(ch[1][x], y);
        if (dist[ch[0][x]] < dist[ch[1][x]]) swap(ch[0][x], ch[1][x]);
        dist[x] = dist[ch[1][x]] + 1;
        rt[ch[0][x]] = rt[ch[1][x]] = rt[x] = x;
        return x;
    }

    inline void pop(int nd) {
        val[nd] = -1;
        rt[ch[0][nd]] = ch[0][nd], rt[ch[1][nd]] = ch[1][nd];
        rt[nd] = Merge(ch[0][nd], ch[1][nd]);
    }

    inline void Mrg(int x, int y) {
        if (val[x] == -1 || val[y] == -1) return;
        x = findrt(x), y = findrt(y);
        if (x != y) rt[x] = rt[y] = Merge(x, y);
    }
    inline void Pop(int x) {
        if (val[x] == -1) return (void) printf("-1\n");
        x = findrt(x);
        printf("%d\n", val[x]), pop(x);
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(m);
    for (int i = 1; i <= n; ++i) read(Heap::val[i]);
    // init
    Heap::init();
    // solve
    while (m--) {
        int opt, x, y;
        read(opt), read(x);
        switch (opt) {
            case 1: read(y); Heap::Mrg(x, y); break;
            case 2: Heap::Pop(x); break;
            default: fprintf(stderr, "ERR\n");
        }
    }
    return 0;
}

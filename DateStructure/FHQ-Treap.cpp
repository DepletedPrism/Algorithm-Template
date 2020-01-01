// Luogu P3369
// DeP
#include <ctime>
#include <cctype>
#include <cstdio>
#include <cstdlib>

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

int n, root;

namespace Treap {
    int nidx;
    int ch[2][MAXN], size[MAXN], rnd[MAXN], val[MAXN];

    inline void maintain(int nd) {
        size[nd] = 1;
        if (ch[0][nd]) size[nd] += size[ch[0][nd]];
        if (ch[1][nd]) size[nd] += size[ch[1][nd]];
    }

    inline int newnode(int x) {
        return size[++nidx] = 1, val[nidx] = x, rnd[nidx] = rand(), nidx;
    }

    void split(int nd, int k, int& x, int& y) {
        if (!nd) return void(x = y = 0);
        if (val[nd] <= k) x = nd, split(ch[1][nd], k, ch[1][nd], y);
        else y = nd, split(ch[0][nd], k, x, ch[0][nd]);
        maintain(nd);
    }
    int merge(int x, int y) {
        if (!x || !y) return x + y;
        if (rnd[x] < rnd[y]) {
            ch[1][x] = merge(ch[1][x], y), maintain(x);
            return x;
        }
        ch[0][y] = merge(x, ch[0][y]), maintain(y);
        return y;
    }

    inline void insert(int v) {
        static int x, y;
        split(root, v, x, y), root = merge(merge(x, newnode(v)), y);
    }
    inline void remove(int v) {
        static int x, y, z;
        split(root, v, x, z), split(x, v-1, x, y);
        y = merge(ch[0][y], ch[1][y]), root = merge(merge(x, y), z);
    }

    inline void Rnk(int v) {
        static int x, y;
        split(root, v-1, x, y);
        printf("%d\n", size[x] + 1);
        merge(x, y);
    }
    int Kth(int nd, int k) {
        static int t;
        while (nd) {
            if (ch[0][nd]) t = size[ch[0][nd]] + 1; else t = 1;
            if (k == t) return nd;
            if (k > t) k -= t, nd = ch[1][nd];
            else nd = ch[0][nd];
        }
        return 0;
    }

    inline void Pre(int v) {
        static int x, y;
        split(root, v-1, x, y);
        printf("%d\n", val[Kth(x, size[x])]);
        root = merge(x, y);
    }
    inline void Suf(int v) {
        static int x, y;
        split(root, v, x, y);
        printf("%d\n", val[Kth(y, 1)]);
        root = merge(x, y);
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // init
    srand( (unsigned) time(NULL) );
    // input
    read(n);
    // solve
    while (n--) {
        static int opt, x;
        read(opt), read(x);
        switch (opt) {
            case 1: Treap::insert(x); break;
            case 2: Treap::remove(x); break;
            case 3: Treap::Rnk(x); break;
            case 4: printf("%d\n", Treap::val[Treap::Kth(root, x)]); break;
            case 5: Treap::Pre(x); break;
            case 6: Treap::Suf(x); break;
            default: fprintf(stderr, "ERR\n");
        }
    }
    return 0;
}

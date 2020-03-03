// Luogu P3919
// DeP
#include <cctype>
#include <cstdio>

namespace IO {
    const int MAXSIZE = 1 << 18 | 1;
    char buf[MAXSIZE], *p1, *p2;

    inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
    template<class T> void read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) { f |= ch == '-'; ch = Gc(); }
        while (isdigit(ch)) { x = x * 10 + ch - '0'; ch = Gc(); }
        if (f) x = -x;
    }
}
using IO::read;

const int MAXN = (int)1e6+5;

int n, m;
int A[MAXN];

namespace SEG {
#define Mid (L + ((R-L)>>1))
    int dat[MAXN << 5];
    int rt[MAXN << 5], lc[MAXN << 5], rc[MAXN << 5], idx;

    void build(int& nd, int L, int R) {
        nd = ++idx;
        if (L == R) { dat[nd] = A[L]; return; }
        build(lc[nd], L, Mid); build(rc[nd], Mid+1, R);
    }

    void modify(int& nd, int pre, int L, int R, int pos, int val) {
        nd = ++idx;
        dat[nd] = dat[pre];
        lc[nd] = lc[pre]; rc[nd] = rc[pre];
        if (L == R) { dat[nd] = val; return; }
        if (pos <= Mid) modify(lc[nd], lc[pre], L, Mid, pos, val);
        else modify(rc[nd], rc[pre], Mid+1, R, pos, val);
    }

    int query(int nd, int L, int R, int pos) {
        if (L == R) return dat[nd];
        if (pos <= Mid) return query(lc[nd], L, Mid, pos);
        return query(rc[nd], Mid+1, R, pos);
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("testdata.in", "r", stdin);
    freopen("output.out", "w", stdout);
#endif
    // input
    read(n); read(m);
    for (int i = 1; i <= n; ++i) read(A[i]);
    // init
    SEG::build(SEG::rt[0], 1, n);
    // operator & output
    for (int i = 1; i <= m; ++i) {
        int opt, nth, pos, val;
        read(nth); read(opt); read(pos);
        switch (opt) {
            case 1:
                read(val);
                SEG::modify(SEG::rt[i], SEG::rt[nth], 1, n, pos, val);
                break;
            case 2:
                printf("%d\n", SEG::query(SEG::rt[nth], 1, n, pos));
                SEG::rt[i] = SEG::rt[nth];
                break;
            default: puts("ERROR");
        }
    }
    return 0;
}

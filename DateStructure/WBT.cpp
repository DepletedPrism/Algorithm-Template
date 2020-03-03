// Luogu P3369
// DeP
#include <cctype>
#include <cstdio>
#include <cstdlib>
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

const int MAXN = 1e5+5;
const double alpha = 0.7114514;

int n, root;

namespace WBT {
    int g[MAXN], ng, nidx;
    int ch[2][MAXN], size[MAXN], cnt[MAXN], val[MAXN];

    inline void maintain(int nd) {
        size[nd] = cnt[nd];
        if (ch[0][nd]) size[nd] += size[ch[0][nd]];
        if (ch[1][nd]) size[nd] += size[ch[1][nd]];
    }

    int build(int L, int R) {
        if (L > R) return 0;
        int Mid = (L + R) / 2;
        ch[0][g[Mid]] = build(L, Mid-1), ch[1][g[Mid]] = build(Mid+1, R);
        return maintain(g[Mid]), g[Mid];
    }

    void dfs(int u) {
        if (!u) return;
        if (ch[0][u]) dfs(ch[0][u]);
        if (cnt[u] > 0) g[++ng] = u;
        if (ch[1][u]) dfs(ch[1][u]);
    }
    inline void rebuild(int& nd) { ng = 0, dfs(nd), nd = build(1, ng); }
    inline bool canrb(const int& nd) {
        return cnt[nd] > 0 && alpha * size[nd] <= max(size[ch[0][nd]], size[ch[1][nd]]);
    }

    inline void Ins(int& nd, const int& v) {
        if (!nd) {
            nd = ++nidx, val[nd] = v, cnt[nd] = size[nd] = 1;
            if (!root) root = nd;
        } else {
            if (v < val[nd]) Ins(ch[0][nd], v);
            else if (v > val[nd]) Ins(ch[1][nd], v);
            else ++cnt[nd];
            maintain(nd);
            if (canrb(nd)) rebuild(nd);
        }
    }
    inline void Rmv(int& nd, const int& v) {
        if (!nd) return;
        --size[nd];
        if (val[nd] == v) {
            if (cnt[nd]) --cnt[nd];
        } else {
            if (val[nd] < v) Rmv(ch[1][nd], v);
            else Rmv(ch[0][nd], v);
            maintain(nd);
        }
        if (canrb(nd)) rebuild(nd);
    }

    inline int Rnk(int nd, int v) {
        int ret = 1;
        while (nd > 0) {
            if (val[nd] >= v) nd = ch[0][nd];
            else ret += size[ch[0][nd]] + cnt[nd], nd = ch[1][nd];
        }
        return ret;
    }

    inline int Kth(int nd, int k) {
        static int t;
        while (nd > 0) {
            if (ch[0][nd]) t = size[ch[0][nd]]; else t = 0;
            if (t < k && k <= t + cnt[nd]) return val[nd];
            t += cnt[nd];
            if (k > t) k -= t, nd = ch[1][nd];
            else nd = ch[0][nd];
        }
        abort();
    }

    inline int Pre(int nd, int v) { return Kth(nd, Rnk(nd, v) - 1); }
    inline int Suf(int nd, int v) { return Kth(nd, Rnk(nd, v+1)); }
}

int main() {
#ifndef ONLINE_JUDGE
    // freopen("input.in", "r", stdin);
#endif
    read(n);
    while (n--) {
        static int opt, x;
        read(opt), read(x);
        switch (opt) {
            case 1: WBT::Ins(root, x); break;
            case 2: WBT::Rmv(root, x); break;
            case 3: printf("%d\n", WBT::Rnk(root, x)); break;
            case 4: printf("%d\n", WBT::Kth(root, x)); break;
            case 5: printf("%d\n", WBT::Pre(root, x)); break;
            case 6: printf("%d\n", WBT::Suf(root, x)); break;
            default: fprintf(stderr, "ERR\n");
        }
    }
    return 0;
}

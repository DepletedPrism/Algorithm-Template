// Luogu P4585
// DeP
#include <cctype>
#include <cstdio>
#include <vector>
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

const int MAXN = 1e5+5;

struct Ask {
    int L, R, tL, tR, x;
} Q[MAXN];

struct Modify {
    int Time, s, v;
    bool operator < (const Modify& rhs) const { return s < rhs.s; }
} M[MAXN], tmpL[MAXN], tmpR[MAXN], none;

int n, m, nq;
int A[MAXN], rt[MAXN], Ans[MAXN];

namespace Trie {
    const int MAXN = ::MAXN << 5;
    int ch[2][MAXN], size[MAXN], nidx;

    inline void init() { nidx = 1, size[nidx] = ch[0][nidx] = ch[1][nidx] = 0; }
    inline int newnode() { return ++nidx, size[nidx] = ch[0][nidx] = ch[1][nidx] = 0, nidx; }

    inline void insert(int nd, int lst, const int& val) {
        for (int i = 17; i >= 0; --i) {
            int c = (val >> i) & 1;
            size[nd] = size[lst] + 1;
            if (!ch[c][nd]) ch[c][nd] = newnode();
            ch[!c][nd] = ch[!c][lst];
            nd = ch[c][nd], lst = ch[c][lst];
        }
        size[nd] = size[lst] + 1;
    }

    inline int Qry(int x, int y, const int& val) {
        int ret = 0;
        for (int i = 17; i >= 0; --i) {
            int c = (val >> i) & 1;
            if (size[ch[!c][y]] > size[ch[!c][x]])
                ret |= 1 << i, y = ch[!c][y], x = ch[!c][x];
            else y = ch[c][y], x = ch[c][x];
        }
        return ret;
    }
}

namespace SGT {
#define lc (nd<<1)
#define rc (nd<<1|1)
    vector<int> dat[MAXN << 2];

    void Mdy(int nd, int L, int R, const int& opL, const int& opR, const int& idx) {
        if (opL > opR) return;
        if (opL <= L && R <= opR) return void( dat[nd].push_back(idx) );
        int Mid = (L + R) / 2;
        if (opL <= Mid) Mdy(lc, L, Mid, opL, opR, idx);
        if (opR > Mid) Mdy(rc, Mid+1, R, opL, opR, idx);
    }

    void divide(int nd, int L, int R, int opL, int opR) {
        if (L > R || opL > opR) return;
        // calculate nd
        Trie::init();
        int t = 0;
        for (int i = opL; i <= opR; ++i)
            ++t, Trie::insert(rt[t] = Trie::newnode(), rt[t-1], M[i].v);
        for (size_t i = 0; i < dat[nd].size(); ++i) {
            const int& idx = dat[nd][i];
            none.s = Q[idx].L - 1;
            int rtL = upper_bound(M+opL, M+opR+1, none) - M - opL;
            none.s = Q[idx].R;
            int rtR = upper_bound(M+opL, M+opR+1, none) - M - opL;
            Ans[idx] = max(Ans[idx], Trie::Qry(rt[rtL], rt[rtR], Q[idx].x));
        }
        // merge & divide
        int Mid = (L + R) / 2, p = 0, q = 0;
        for (int i = opL; i <= opR; ++i) {
            if (M[i].Time <= Mid) tmpL[++p] = M[i];
            else tmpR[++q] = M[i];
        }
        for (int i = 1; i <= p; ++i) M[opL + i - 1] = tmpL[i];
        for (int i = 1; i <= q; ++i) M[opL + p + i - 1] = tmpR[i];
        if (L != R) divide(lc, L, Mid, opL, opL + p - 1);
        divide(rc, Mid+1, R, opL + p, opR);
    }
#undef lc
#undef rc
}

int main() {
#ifndef ONLINE_JUDGE
    // freopen("input.in", "r", stdin);
#endif
    // init
    Trie::init();
    // input
    read(n), read(m);
    for (int i = 1; i <= n; ++i) read(A[i]);
    // solve
    for (int i = 1; i <= n; ++i)
        Trie::insert(rt[i] = Trie::newnode(), rt[i-1], A[i]);
    int Time = 0;
    for (int i = 1; i <= m; ++i) {
        static int opt, L, R, x, d, s, v;
        read(opt);
        switch (opt) {
            case 0:
                read(s), read(v), ++Time;
                M[Time] = (Modify){ Time, s, v };
                break;
            case 1:
                read(L), read(R), read(x), read(d);
                Ans[++nq] = Trie::Qry(rt[L-1], rt[R], x);
                Q[nq] = (Ask){ L, R, max(1, Time-d+1), Time, x };
                break;
            default: fprintf(stderr, "GG\n");
        }
    }
    sort(M+1, M+1+Time);
    for (int i = 1; i <= nq; ++i)
        SGT::Mdy(1, 1, Time, Q[i].tL, Q[i].tR, i);
    SGT::divide(1, 1, Time, 1, Time);
    // output
    for (int i = 1; i <= nq; ++i) printf("%d\n", Ans[i]);
    return 0;
}

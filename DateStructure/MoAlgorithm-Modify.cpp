// Luogu P1903
// DeP
#include <cmath>
#include <cctype>
#include <cstdio>
#include <algorithm>
using namespace std;

namespace IO {
    template<class T> void read(T& x) {
        x = 0; int f = 0, ch = getchar();
        while (!isdigit(ch)) { f |= ch == '-'; ch = getchar(); }
        while (isdigit(ch)) { x = x * 10 + ch - '0'; ch = getchar(); }
        if (f) x = -x;
    }
}
using IO::read;

const int MAXM = 133335, MAXA = (int)1e6+5;
int block;

struct Query {
    int idx, L, R, Time;
    bool operator < (const Query& rhs) const {
        return L/block == rhs.L/block? (R/block == rhs.R/block? Time < rhs.Time: R < rhs.R): L < rhs.L;
    }
} asks[MAXM];

struct Update {
    int pos, col;
} updates[MAXM];

int n, m, qidx, uidx;
int A[MAXM], ans[MAXM];

namespace MoAlgorithm {
    int L, R, Time, now;
    int cnt[MAXA];

    inline void Add(int val) {
        now += !cnt[val]++;
    }
    inline void Del(int val) {
        now -= !(--cnt[val]);
    }
    inline void modify(int x) {
        if (L <= updates[x].pos && updates[x].pos <= R) {
            Del(A[updates[x].pos]);
            Add(updates[x].col);
        }
        swap(updates[x].col, A[updates[x].pos]);
    }

    inline void solve() {
        block = (int)ceil(exp((log(n)+log(qidx))/3));
        sort(asks+1, asks+qidx+1);
        L = asks[1].L; R = L-1;
        for (register int i = 1; i <= qidx; ++i) {
            const Query& q = asks[i];
            while (q.L < L) Add(A[--L]);
            while (q.L > L) Del(A[L++]);
            while (q.R > R) Add(A[++R]);
            while (q.R < R) Del(A[R--]);
            while (Time < q.Time) modify(++Time);
            while (Time > q.Time) modify(Time--);
            ans[q.idx] = now;
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("testdata.in", "r", stdin);
    freopen("output.out", "w", stdout);
#endif
    // input
    read(n); read(m);
    for (register int i = 1; i <= n; ++i) read(A[i]);
    for (register int i = 1; i <= m; ++i) {
        char opt[5]; int L, R;
        scanf("%s", opt); read(L); read(R);
        switch (opt[0]) {
            case 'Q': asks[++qidx] = (Query){ qidx, L, R, uidx }; break;
            case 'R': updates[++uidx] = (Update){ L, R }; break;
            default: puts("ERROR in opt");
        }
    }
    // aogharg aeioh 
    MoAlgorithm::solve();
    // output
    for (register int i = 1; i <= qidx; ++i)
        printf("%d\n", ans[i]);
    return 0;
}

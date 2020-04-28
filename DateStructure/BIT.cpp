// LOJ #132
// DeP
#include <cstdio>

typedef long long LL;
const int MAXN = 1e6+5;

int n, q;
int A[MAXN];

namespace BIT {
    LL C1[MAXN], C2[MAXN];

    inline int lowbit(int x) { return x & -x; }

    void Mdy(int pos, LL val) {
        for (int i = pos; i <= n; i += lowbit(i))
            C1[i] += val, C2[i] += val * pos;
    }
    inline void Mdy(const int& L, const int& R, const LL& v) {
        Mdy(L, v), Mdy(R + 1, -v);
    }

    LL Qry(int pos) {
        LL ret = 0;
        for (int i = pos; i; i -= lowbit(i))
            ret += C1[i] * (pos+1) - C2[i];
        return ret;
    }
    inline LL Qry(const int& L, const int& R) {
        return Qry(R) - Qry(L - 1);
    }
}

int main() {
    // input
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; ++i)
    	scanf("%d", A+i);
    // output
    for (int i = 1; i <= n; ++i) BIT::Mdy(i, A[i] - A[i-1]);
    while (q--) {
        static int opt, L, R, x;
        scanf("%d%d%d", &opt, &L, &R);
        switch (opt) {
            case 1: scanf("%d", &x), BIT::Mdy(L, R, x); break;
            case 2: printf("%lld\n", BIT::Qry(L, R)); break;
            default: fprintf(stderr, "f**k segment tree.\n");
        }
    }
    return 0;
}

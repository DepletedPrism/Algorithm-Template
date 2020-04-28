// Luogu P4781
// DeP
#include <cctype>
#include <cstdio>

namespace IO {
    const int MAXSIZE = 1 << 18 | 1;
    char buf[MAXSIZE], *p1, *p2;

    inline int Gc() {
        return p1 == p2 &&
            (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++;
    }
    template<typename T> inline void read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) f |= ch == '-', ch = Gc();
        while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
        if (f) x = -x;
    }
}
using IO::read;

const int MAXN = 2e3 + 5, P = 998244353;

int fpow(int base, int b) {
    int ret = 1;
    while (b > 0) {
        if (b & 1) ret = 1LL * ret * base % P;
        base = 1LL * base * base % P, b >>= 1;
    }
    return ret;
}

int n, K;
int X[MAXN], Y[MAXN];

int Lagrange(int x) {
    int ret = 0;
    for (int i = 1; i <= n; ++i) {
        int s1 = Y[i], s2 = 1;
        for (int j = 1; j <= n; ++j) if (i != j) {
            s1 = 1LL * (x - X[j] + P) % P * s1 % P;
            s2 = 1LL * (X[i] - X[j] + P) % P * s2 % P;
        }
        ret = (ret + 1LL * s1 * fpow(s2, P-2) % P) % P;
    }
    return ret;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(K);
    for (int i = 1; i <= n; ++i) read(X[i]), read(Y[i]);
    // solve & output
    printf("%d\n", Lagrange(K));
    return 0;
}

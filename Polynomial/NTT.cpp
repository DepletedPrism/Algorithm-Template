// Luogu P3803
// DeP
#include <cctype>
#include <cstdio>
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

const int MAXN = 3e6+5, P = 998244353, G = 3;

int n, m, Lim;
int P1[MAXN], P2[MAXN];

int fpow(int base, int b) {
    int ret = 1;
    while (b > 0) {
        if (b & 1) ret = 1LL * ret * base % P;
        base = 1LL * base * base % P, b >>= 1;
    }
    return ret;
}

namespace Poly {
    int L, R[MAXN];
    inline void init() {
        for (int i = 1; i < Lim; ++i) R[i] = (R[i>>1] >> 1) | ((i & 1) << (L-1));
    }

    void NTT(int* A, int type) {
        for (int i = 1; i < Lim; ++i) if (i < R[i]) swap(A[i], A[R[i]]);
        for (int Mid = 1; Mid < Lim; Mid <<= 1) {
            int unit = fpow(type > 0? G: fpow(G, P-2), (P-1) / (Mid << 1));
            for (int i = 0; i < Lim; i += Mid << 1) {
                int w = 1;
                for (int j = 0; j < Mid; ++j, w = 1LL * w * unit % P) {
                    int A0 = A[i+j], A1 = 1LL * w * A[i+j+Mid] % P;
                    A[i+j] = (A0 + A1) % P, A[i+j+Mid] = (A0 - A1 + P) % P;
                }
            }
        }
        if (type < 0) {
            int inv = fpow(Lim, P-2);
            for (int i = 0; i < Lim; ++i) A[i] = 1LL * A[i] * inv % P;
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(m);
    for (int i = 0; i <= n; ++i) read(P1[i]);
    for (int j = 0; j <= m; ++j) read(P2[j]);
    // solve
    Lim = 1;
    while (Lim <= n+m) Lim <<= 1, ++Poly::L;
    Poly::init();
    Poly::NTT(P1, 1), Poly::NTT(P2, 1);
    for (int i = 0; i < Lim; ++i) P1[i] = 1LL * P1[i] * P2[i] % P;
    Poly::NTT(P1, -1);
    // output
    for (int i = 0; i <= n+m; ++i) printf("%d%c", P1[i], " \n"[i==n+m]);
    return 0;
}

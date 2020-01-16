// Luogu P4238
// DeP
#include <cctype>
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 3e5+5;
const int P = 998244353, G = 3, iG = 332748118;

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

int fpow(int base, int b) {
    int ret = 1;
    while (b > 0) {
        if (b & 1) ret = 1LL * ret * base % P;
        base = 1LL * base * base % P, b >>= 1;
    }
    return ret;
}

namespace Poly {
    int A[MAXN], B[MAXN], R[MAXN];
    inline void init(int Lim, int L) {
        for (int i = 1; i < Lim; ++i) R[i] = (R[i>>1] >> 1) | ((i & 1) << (L-1));
    }

    void NTT(int* f, int Lim, int type) {
        for (int i = 1; i < Lim; ++i) if (i < R[i]) swap(f[i], f[R[i]]);
        for (int Mid = 1; Mid < Lim; Mid <<= 1) {
            int unit = fpow(type > 0? G: iG, (P-1) / (Mid << 1));
            for (int i = 0; i < Lim; i += Mid << 1) {
                int w = 1;
                for (int j = 0; j < Mid; ++j, w = 1LL * w * unit % P) {
                    int f0 = f[i+j], f1 = 1LL * w * f[i+j+Mid] % P;
                    f[i+j] = (f0 + f1) % P, f[i+j+Mid] = (f0 - f1 + P) % P;
                }
            }
        }
        if (type < 0) {
            int inv = fpow(Lim, P-2);
            for (int i = 0; i < Lim; ++i) f[i] = 1LL * f[i] * inv % P;
        }
    }

    void Inv(int* f, int* g, int n) {
        g[0] = fpow(f[0], P-2);
        for (int L = 0, Lim = 1, Mid = 2; Mid < 2*n; Mid <<= 1) {
            while (Lim < Mid+Mid) Lim <<= 1, ++L;
            init(Lim, L);
            for (int i = 0; i < Mid; ++i) A[i] = f[i], B[i] = g[i];
            for (int i = Mid; i < Lim; ++i) A[i] = B[i] = 0;
            NTT(A, Lim, 1), NTT(B, Lim, 1);
            for (int i = 0; i < Lim; ++i)
                g[i] = (((B[i] + B[i]) % P - 1LL * A[i] * B[i] % P * B[i] % P) % P + P) % P;
            NTT(g, Lim, -1);
            for (int i = min(n, Mid); i < Lim; ++i) g[i] = 0;
        }
    }
}

int n;
int P1[MAXN], P2[MAXN];

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    read(n);
    for (int i = 0; i < n; ++i) read(P1[i]);
    Poly::Inv(P1, P2, n);
    for (int i = 0; i < n; ++i) printf("%d%c", P2[i], " \n"[i==n-1]);
    return 0;
}

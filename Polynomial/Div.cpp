// Luogu P4512
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

const int MAXN = 3e5+5;
const int P = 998244353, G = 3, iG = 332748118;

int fpow(int base, int b) {
    int ret = 1;
    while (b > 0) {
        if (b & 1) ret = 1LL * ret * base % P;
        base = 1LL * base * base % P, b >>= 1;
    }
    return ret;
}

namespace Poly {
    int A[MAXN], B[MAXN], ig[MAXN], r[MAXN];
    inline void init(const int& Lim, const int& L) {
        for (int i = 1; i < Lim; ++i) r[i] = (r[i>>1] >> 1) | ((i & 1) << (L-1));
    }

    void NTT(int* f, int Lim, int type) {
        for (int i = 1; i < Lim; ++i) if (i < r[i]) swap(f[i], f[r[i]]);
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
            while (Lim < 2*Mid) Lim <<= 1, ++L;
            init(Lim, L);
            for (int i = 0; i < Mid; ++i) A[i] = f[i], B[i] = g[i];
            for (int i = Mid; i < Lim; ++i) A[i] = B[i] = 0;
            NTT(A, Lim, 1), NTT(B, Lim, 1);
            for (int i = 0; i < Lim; ++i)
                g[i] = (((B[i] + B[i]) % P - 1LL * A[i] * B[i] % P * B[i] % P) % P + P) % P;
            NTT(g, Lim, -1);
            for (int i = min(Mid, n); i < Lim; ++i) g[i] = 0;
        }
    }

    void Div(int* f, int n, int* g, int m, int* Q, int* R) {
        reverse(f, f+n), reverse(g, g+m);
        Inv(g, ig, n-m+1);
        int Lim = 1, L = 0;
        while (Lim < n + n-m+1) Lim <<= 1, ++L;
        init(Lim, L);
        for (int i = 0; i < Lim; ++i) if (i < n) A[i] = f[i]; else A[i] = 0;
        for (int i = 0; i < Lim; ++i) if (i < n-m+1) B[i] = ig[i]; else B[i] = 0;
        NTT(A, Lim, 1), NTT(B, Lim, 1);
        for (int i = 0; i < Lim; ++i) Q[i] = 1LL * A[i] * B[i] % P;
        NTT(Q, Lim, -1);
        reverse(Q, Q+n-m+1), reverse(f, f+n), reverse(g, g+m);
        Lim = 1, L = 0;
        while (Lim < m + n-m+1) Lim <<= 1, ++L;
        init(Lim, L);
        for (int i = 0; i < Lim; ++i) if (i < n-m+1) A[i] = Q[i]; else A[i] = 0;
        for (int i = 0; i < Lim; ++i) if (i < m) B[i] = g[i]; else B[i] = 0;
        NTT(A, Lim, 1), NTT(B, Lim, 1);
        for (int i = 0; i < Lim; ++i) R[i] = 1LL * A[i] * B[i] % P;
        NTT(R, Lim, -1);
        for (int i = 0; i < m-1; ++i) R[i] = (f[i] - R[i] + P) % P;
    }
}

int n, m;
int f[MAXN], g[MAXN], Q[MAXN], R[MAXN];

int main() {
#ifndef ONLINE_JUDGE
    // freopen("input.in", "r", stdin);
#endif
    read(n), read(m); ++n, ++m;
    for (int i = 0; i < n; ++i) read(f[i]);
    for (int i = 0; i < m; ++i) read(g[i]);
    Poly::Div(f, n, g, m, Q, R);
    for (int i = 0; i < n-m+1; ++i) printf("%d%c", Q[i], " \n"[i==n-m]);
    for (int i = 0; i < m-1; ++i) printf("%d%c", R[i], " \n"[i==m-2]);
    return 0;
}

// Luogu P4723
// DeP
#include <cctype>
#include <cstdio>
#include <algorithm>
using namespace std;

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

const int MAXN = 1 << 18 | 1, MAXK = 3.2e4 + 5;
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
    int r[MAXN];
    inline void init(const int& Lim, const int& L) {
        for (int i = 1; i < Lim; ++i) r[i] = (r[i>>1] >> 1) | ((i & 1) << (L-1));
    }

    void NTT(int* f, const int& Lim, const int& type) {
        for (int i = 1; i < Lim; ++i) if (i < r[i]) swap(f[i], f[r[i]]);
        for (int Mid = 1; Mid < Lim; Mid <<= 1) {
            int unit = fpow(type > 0? G: iG, (P - 1) / (Mid << 1));
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

    void Inv(int* f, int* g, const int& n) {
        static int A[MAXN], B[MAXN];
        g[0] = fpow(f[0], P-2);
        for (int L = 0, Lim = 1, Mid = 2; Mid < 2*n; Mid <<= 1) {
            while (Lim < 2*Mid) Lim <<= 1, ++L;
            for (int i = 0; i < Mid; ++i) A[i] = f[i], B[i] = g[i];
            for (int i = Mid; i < Lim; ++i) A[i] = B[i] = 0;
            init(Lim, L), NTT(A, Lim, 1), NTT(B, Lim, 1);
            for (int i = 0; i < Lim; ++i)
                g[i] = ((B[i] + B[i]) % P - 1LL * A[i] * B[i] % P * B[i] % P + P) % P;
            NTT(g, Lim, -1);
            for (int i = min(n, Mid); i < Lim; ++i) g[i] = 0;
        }
    }

    // f <-- R, f mod g = R, f = g * Q + R
    void Div(int* f, const int& n, int* g, const int& m) {
        static int A[MAXN], B[MAXN], ig[MAXN], Q[MAXN], R[MAXN];
        int df = n;
        while (!f[df - 1]) --df;
        if (df < m - 1) return;
        reverse(f, f + n), reverse(g, g + m);
        Inv(g, ig, n-m+1);
        int Lim = 1, L = 0;
        while (Lim < n + n-m+1) Lim <<= 1, ++L;
        for (int i = 0; i < Lim; ++i)
            A[i] = (i < n)? f[i]: 0, B[i] = (i < n-m+1)? ig[i]: 0;
        init(Lim, L), NTT(A, Lim, 1), NTT(B, Lim, 1);
        for (int i = 0; i < Lim; ++i) Q[i] = 1LL * A[i] * B[i] % P;
        NTT(Q, Lim, -1);
        for (int i = n-m+1; i < Lim; ++i) Q[i] = 0;
        reverse(Q, Q + n-m+1), reverse(f, f + n), reverse(g, g + m);
        Lim = 1, L = 0;
        while (Lim < m + n-m+1) Lim <<= 1, ++L;
        for (int i = 0; i < Lim; ++i)
            A[i] = (i < n-m+1)? Q[i]: 0, B[i] = (i < m)? g[i]: 0;
        init(Lim, L), NTT(A, Lim, 1), NTT(B, Lim, 1);
        for (int i = 0; i < Lim; ++i) R[i] = 1LL * A[i] * B[i] % P;
        NTT(R, Lim, -1);
        for (int i = 0; i < Lim; ++i)
            f[i] = (i < m-1)? (f[i] - R[i] + P) % P: 0;
    }
}

int n, K;
int A[MAXN], f[MAXN], g[MAXN], R[MAXN];
int h[MAXN];

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(K);
    for (int i = 1; i <= K; ++i) read(A[i]);
    for (int i = 0; i < K; ++i) read(f[i]), f[i] = (f[i] + P) % P;
    // init
    g[K] = 1;
    for (int i = 0; i < K; ++i) g[i] = (P - A[K - i] % P) % P;
    h[1] = R[0] = 1;
    int Lim = 1, L = 0;
    while (Lim < K + K) Lim <<= 1, ++L;
    Poly::init(Lim, L);
    // solve
    while (n > 0) {
        if (n & 1) {
            Poly::init(Lim, L), Poly::NTT(R, Lim, 1), Poly::NTT(h, Lim, 1);
            for (int i = 0; i < Lim; ++i) R[i] = 1LL * R[i] * h[i] % P;
            Poly::NTT(R, Lim, -1), Poly::NTT(h, Lim, -1);
            Poly::Div(R, 2*K - 1, g, K+1);
        }
        Poly::init(Lim, L), Poly::NTT(h, Lim, 1);
        for (int i = 0; i < Lim; ++i) h[i] = 1LL * h[i] * h[i] % P;
        Poly::NTT(h, Lim, -1);
        Poly::Div(h, 2*K - 1, g, K+1);
        n >>= 1;
    }
    // output
    int ans = 0;
    for (int i = 0; i < K; ++i)
        ans = (ans + 1LL * f[i] * R[i] % P) % P;
    printf("%d\n", ans);
    return 0;
}

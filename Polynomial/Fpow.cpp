// Luogu P5245
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
using IO::read; using IO::Gc;

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
    int A[MAXN], B[MAXN], C[MAXN], D[MAXN], lng[MAXN], lnf[MAXN], R[MAXN];
    inline void init(const int& Lim, const int& L) {
        for (int i = 1; i < Lim; ++i) R[i] = (R[i>>1] >> 1) | ((i & 1) << (L-1));
    }

    void NTT(int* f, int Lim, int type) {
        for (int i = 1; i < Lim; ++i) if (i < R[i]) swap(f[i], f[R[i]]);
        for (int Mid = 1; Mid < Lim; Mid <<= 1) {
            int unit = fpow(type > 0? G: iG, (P-1) / (Mid << 1));
            for (int i = 0; i < Lim; i += Mid << 1) {
                int w = 1;
                for (int j = 0; j < Mid; ++j, w = 1LL * unit * w % P) {
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
            for (int i = min(n, Mid); i < Lim; ++i) g[i] = 0;
        }
    }

    void Der(int* f, int* g, int n) {
        for (int i = 1; i < n; ++i) g[i-1] = 1LL * i * f[i] % P;
        g[n-1] = 0;
    }
    void Int(int* f, int* g, int n) {
        g[0] = 0;
        for (int i = 1; i < n; ++i) g[i] = 1LL * fpow(i, P-2) * f[i-1] % P;
    }

    void Ln(int* f, int* g, int n) {
        Der(f, C, n), Inv(f, D, n);
        int Lim = 1, L = 0;
        while (Lim < 2*n) Lim <<= 1, ++L;
        init(Lim, L);
        for (int i = n; i < Lim; ++i) C[i] = D[i] = 0;
        NTT(C, Lim, 1), NTT(D, Lim, 1);
        for (int i = 0; i < Lim; ++i) C[i] = 1LL * C[i] * D[i] % P;
        NTT(C, Lim, -1), Int(C, g, n);
    }

    void Exp(int* f, int* g, int n) {
        g[0] = 1;
        for (int L = 0, Lim = 1, Mid = 2; Mid < 2*n; Mid <<= 1) {
            Ln(g, lng, Mid);
            while (Lim < 2*Mid) Lim <<= 1, ++L;
            init(Lim, L);
            for (int i = 0; i < Mid; ++i) A[i] = (f[i] - lng[i] + P) % P, B[i] = g[i];
            A[0] = (A[0] + 1) % P;
            for (int i = Mid; i < Lim; ++i) A[i] = B[i] = 0;
            NTT(A, Lim, 1), NTT(B, Lim, 1);
            for (int i = 0; i < Lim; ++i) g[i] = 1LL * A[i] * B[i] % P;
            NTT(g, Lim, -1);
            for (int i = min(Mid, n); i < Lim; ++i) g[i] = 0;
        }
    }

    void Pow(int* f, int* g, int n, int K) {
        Ln(f, lnf, n);
        for (int i = 0; i < n; ++i) lnf[i] = 1LL * lnf[i] * K % P;
        Exp(lnf, g, n);
    }
}

int n, K;
int f[MAXN], g[MAXN];

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    read(n);
    int ch = Gc();
    while (!isdigit(ch)) ch = Gc();
    while (isdigit(ch)) K = (1LL * K * 10 + ch - '0') % P, ch = Gc();
    for (int i = 0; i < n; ++i) read(f[i]);
    Poly::Pow(f, g, n, K);
    for (int i = 0; i < n; ++i) printf("%d%c", g[i], " \n"[i==n-1]);
    return 0;
}

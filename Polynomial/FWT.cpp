// Luogu P4717
// DeP
#include <cctype>
#include <cstdio>

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

const int MAXN = 1 << 17 | 1, P = 998244353, inv2 = 499122177;

int m, n;

namespace FWT {
    void Or(int* A, int type) {
        for (int k = 1, Mid = 2; Mid <= n; Mid <<= 1, k <<= 1)
            for (int i = 0; i < n; i += Mid)
                for (int j = 0; j < k; ++j) A[i+j+k] = (A[i+j+k] + 1LL * type * A[i+j] % P + P) % P;
    }

    void And(int* A, int type) {
        for (int k = 1, Mid = 2; Mid <= n; Mid <<= 1, k <<= 1)
            for (int i = 0; i < n; i += Mid)
                for (int j = 0; j < k; ++j) A[i+j] = (A[i+j] + 1LL * type * A[i+j+k] % P + P) % P;
    }

    void Xor(int* A, int type) {
        for (int k = 1, Mid = 2; Mid <= n; Mid <<= 1, k <<= 1)
            for (int i = 0; i < n; i += Mid)
                for (int j = 0; j < k; ++j) {
                    int A0 = A[i+j], A1 = A[i+j+k];
                    A[i+j] = (A0 + A1) % P, A[i+j+k] = ((A0 - A1) % P + P) % P;
                    A[i+j] = 1LL * type * A[i+j] % P, A[i+j+k] = 1LL * type * A[i+j+k] % P;
                }
    }
}

int A[MAXN], B[MAXN], P1[MAXN], P2[MAXN];

signed main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(m), n = 1 << m;
    for (int i = 0; i < n; ++i) read(A[i]);
    for (int i = 0; i < n; ++i) read(B[i]);
    // or
    for (int i = 0; i < n; ++i) P1[i] = A[i], P2[i] = B[i];
    FWT::Or(P1, 1), FWT::Or(P2, 1);
    for (int i = 0; i < n; ++i) P1[i] = 1LL * P1[i] * P2[i] % P;
    FWT::Or(P1, -1);
    for (int i = 0; i < n; ++i) printf("%d%c", P1[i], " \n"[i==n-1]);
    // and
    for (int i = 0; i < n; ++i) P1[i] = A[i], P2[i] = B[i];
    FWT::And(P1, 1), FWT::And(P2, 1);
    for (int i = 0; i < n; ++i) P1[i] = 1LL * P1[i] * P2[i] % P;
    FWT::And(P1, -1);
    for (int i = 0; i < n; ++i) printf("%d%c", P1[i], " \n"[i==n-1]);
    // xor
    for (int i = 0; i < n; ++i) P1[i] = A[i], P2[i] = B[i];
    FWT::Xor(P1, 1), FWT::Xor(P2, 1);
    for (int i = 0; i < n; ++i) P1[i] = 1LL * P1[i] * P2[i] % P;
    FWT::Xor(P1, inv2);
    for (int i = 0; i < n; ++i) printf("%d%c", P1[i], " \n"[i==n-1]);
    return 0;
}

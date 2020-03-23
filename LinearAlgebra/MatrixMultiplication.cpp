// LOJ #100
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>

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

const int MAXN = 505, P = 1e9+7;

int n, p, m;

struct Matrix {
    int M[MAXN][MAXN];
    Matrix() { memset(M, 0, sizeof M); }
    Matrix operator * (const Matrix& rhs) const {
        Matrix ret;
        for (int i = 0; i < n; ++i)
            for (int k = 0; k < p; ++k)
                for (int j = 0; j < m; ++j)
                    ret.M[i][j] = (ret.M[i][j] + 1LL * M[i][k] * rhs.M[k][j] % P) % P;
        return ret;
    }
} A, B, R;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(p), read(m);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < p; ++j) read(A.M[i][j]);
    for (int i = 0; i < p; ++i)
        for (int j = 0; j < m; ++j) read(B.M[i][j]);
    // solve
    R = A * B;
    // output
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) printf("%d%c", (R.M[i][j] + P) % P, " \n"[j == m-1]);
    return 0;
}

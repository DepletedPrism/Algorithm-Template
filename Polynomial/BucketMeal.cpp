// LOJ #150
// DeP
#include <ctime>
#include <cctype>
#include <cstdio>
#include <cstdlib>
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
        while (isdigit(ch)) x = x * 10 - '0' + ch, ch = Gc();
        if (f) x = -x;
    }
}
using IO::read;

const int LOG = 18, MAXN = 1 << LOG | 1;
const int P = 998244353, G = 3, iG = 332748118, iv2 = 499122177;

int W[LOG][MAXN], inv[MAXN];

int fpow(int base, int b) {
    int ret = 1;
    while (b > 0) {
        if (b & 1) ret = 1LL * ret * base % P;
        base = 1LL * base * base % P, b >>= 1;
    }
    return ret;
}

namespace Cipolla {
    struct Complex {
        int x, y;
        Complex(int _x, int _y): x(_x), y(_y) { }
    };

    inline Complex mul(const Complex& A, const Complex& B, const int& i2) {
        return Complex((1LL * A.x * B.x % P + 1LL * i2 * A.y % P * B.y % P) % P,
                (1LL * A.x * B.y % P + 1LL * A.y * B.x % P) % P);
    }
    Complex fpow(Complex base, int b, const int& i2) {
        Complex ret = Complex(1, 0);
        while (b > 0) {
            if (b & 1) ret = mul(ret, base, i2);
            base = mul(base, base, i2), b >>= 1;
        }
        return ret;
    }

    inline int Cipolla(int n) {
        static int a, x, i2;
        srand(time(nullptr));
        while (true) {
            a = rand() % P, i2 = (1LL * a * a % P - n + P) % P;
            if (!a || ::fpow(i2, (P - 1) / 2) == 1) continue;
            return x = fpow(Complex(a, 1), (P + 1) / 2, i2).x, min(x, P - x);
        }
        return -1;
    }
}

namespace Poly {
    int r[MAXN];
    inline void init(const int& Lim, const int& L) {
        for (int i = 1; i < Lim; ++i) r[i] = (r[i>>1] >> 1) | ((i & 1) << (L-1));
    }

    void NTT(int* f, const int& Lim, const int& type) {
        for (int i = 1; i < Lim; ++i) if (i < r[i]) swap(f[i], f[r[i]]);
        for (int k = 0; (1 << k) < Lim; ++k) {
            const int *w = W[k], Mid = (1 << k);
            for (int i = 0; i < Lim; i += Mid << 1)
                for (int j = 0; j < Mid; ++j) {
                    int f0 = f[i+j], f1 = 1LL * w[j] * f[i+j+Mid] % P;
                    f[i+j] = (f0 + f1) % P, f[i+j+Mid] = (f0 - f1 + P) % P;
                }
        }
        if (type < 0) {
            int inv = fpow(Lim, P - 2);
            for (int i = 0; i < Lim; ++i) f[i] = 1LL * f[i] * inv % P;
            reverse(f + 1, f + Lim);
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

    void Sqrt(int* f, int* g, const int& n) {
        static int ig[MAXN], A[MAXN], B[MAXN];
        g[0] = Cipolla::Cipolla(f[0]);
        for (int L = 0, Lim = 1, Mid = 2; Mid < 2*n; Mid <<= 1) {
            Inv(g, ig, Mid);
            while (Lim < 2*Mid) Lim <<= 1, ++L;
            for (int i = 0; i < Mid; ++i) A[i] = f[i], B[i] = ig[i];
            for (int i = Mid; i < Lim; ++i) A[i] = B[i] = 0;
            init(Lim, L), NTT(A, Lim, 1), NTT(B, Lim, 1);
            for (int i = 0; i < Lim; ++i) A[i] = 1LL * A[i] * B[i] % P;
            NTT(A, Lim, -1);
            for (int i = 0; i < min(n, Mid); ++i) g[i] = 1LL * iv2 * (g[i] + A[i]) % P;
            for (int i = min(n, Mid); i < Lim; ++i) g[i] = 0;
        }
    }

    inline void Der(int* f, int* g, const int& n) {
        for (int i = 1; i < n; ++i) g[i-1] = 1LL * i * f[i] % P;
        g[n-1] = 0;
    }
    inline void Int(int* f, int* g, const int& n) {
        for (int i = n-1; i; --i) g[i] = 1LL * inv[i] * f[i-1] % P;
        g[0] = 0;
    }

    void Ln(int* f, int* g, const int& n) {
        static int A[MAXN], B[MAXN];
        Der(f, A, n), Inv(f, B, n);
        int Lim = 1, L = 0;
        while (Lim < 2*n) Lim <<= 1, ++L;
        for (int i = n; i < Lim; ++i) A[i] = B[i] = 0;
        init(Lim, L), NTT(A, Lim, 1), NTT(B, Lim, 1);
        for (int i = 0; i < Lim; ++i) A[i] = 1LL * A[i] * B[i] % P;
        NTT(A, Lim, -1), Int(A, g, n);
    }

    void Exp(int* f, int* g, const int& n) {
        static int lng[MAXN], A[MAXN], B[MAXN];
        g[0] = 1;
        for (int L = 0, Lim = 1, Mid = 2; Mid < 2*n; Mid <<= 1) {
            Ln(g, lng, Mid);
            while (Lim < 2*Mid) Lim <<= 1, ++L;
            for (int i = 0; i < Mid; ++i) A[i] = (f[i] - lng[i] + P) % P, B[i] = g[i];
            A[0] = (A[0] + 1) % P;
            for (int i = Mid; i < Lim; ++i) A[i] = B[i] = 0;
            init(Lim, L), NTT(A, Lim, 1), NTT(B, Lim, 1);
            for (int i = 0; i < Lim; ++i) g[i] = 1LL * A[i] * B[i] % P;
            NTT(g, Lim, -1);
            for (int i = min(n, Mid); i < Lim; ++i) g[i] = 0;
        }
    }

    inline void Pow(int* f, int* g, const int& n, const int& k) {
        static int lnf[MAXN];
        Ln(f, lnf, n);
        for (int i = 0; i < n; ++i) lnf[i] = 1LL * lnf[i] * k % P;
        Exp(lnf, g, n);
    }
}

void PolyPre(int N) {
    for (int w, i = 0; i < LOG; ++i) {
        W[i][0] = 1, w = fpow(G, (P - 1) / (1 << (i+1)));
        for (int j = 1; j < (1 << i); ++j)
            W[i][j] = 1LL * w * W[i][j - 1] % P;
    }
    inv[0] = inv[1] = 1;
    for (int i = 2; i <= N; ++i)
        inv[i] = 1LL * (P - P / i) * inv[P % i] % P;
}

int n, K;
int f[MAXN], g[MAXN], h[MAXN];

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n), read(K), ++n;
    for (int i = 0; i < n; ++i) read(f[i]);
    // init
    PolyPre(n);
    // sovle
    Poly::Sqrt(f, h, n), Poly::Inv(h, g, n), Poly::Int(g, h, n), Poly::Exp(h, g, n);
    f[0] = 2;
    for (int i = 0; i < n; ++i) f[i] = (f[i] - g[i] + P) % P;
    Poly::Ln(f, g, n);
    g[0] = (g[0] + 1) % P;
    Poly::Pow(g, f, n, K), Poly::Der(f, g, n);
    // output
    for (int i = 0; i < n-1; ++i) printf("%d%c", g[i], " \n"[i == n-2]);
    return 0;
}

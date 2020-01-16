// Polynomial Bucket Meal
// DeP
#include <cmath>
#include <algorithm>
using namespace std;

const int MAXN = 2e6+5;

struct Complex {
    double x, y;
    Complex(double _x = 0.0, double _y = 0.0): x(_x), y(_y) { }
    Complex operator - (const Complex& rhs) const { return Complex(x - rhs.x, y - rhs.y); }
    Complex operator + (const Complex& rhs) const { return Complex(x + rhs.x, y + rhs.y); }
    Complex operator * (const Complex& rhs) const { return Complex(x*rhs.x - y*rhs.y, x*rhs.y + y*rhs.x); }
};

namespace Poly {
    const double PI = acos(-1.0);
    const int P = 998244353, G = 3, iG = 332748118, inv2 = 499122177;
    int A[MAXN], B[MAXN], C[MAXN], D[MAXN], lng[MAXN], ig[MAXN], r[MAXN];

    inline void init(int Lim, int L) {
        for (int i = 1; i < Lim; ++i) r[i] = (r[i>>1] >> 1) | ((i & 1) << (L-1));
    }

    int fpow(int base, int b) {
        int ret = 1;
        while (b > 0) {
            if (b & 1) ret = 1LL * ret * base % P;
            base = 1LL * base * base % P, b >>= 1;
        }
        return ret;
    }

    void FFT(Complex* f, int Lim, int type) {
        for (int i = 1; i < Lim; ++i) if (i < r[i]) swap(f[i], f[r[i]]);
        for (int Mid = 1; Mid < Lim; Mid <<= 1) {
            Complex unit(cos(PI / Mid), type * sin(PI / Mid));
            for (int i = 0; i < Lim; i += (Mid << 1)) {
                Complex w(1.0, 0.0);
                for (int j = 0; j < Mid; ++j, w = w * unit) {
                    Complex f0 = f[i+j], f1 = w * f[i+j+Mid];
                    f[i+j] = f0 + f1, f[i+j+Mid] = f0 - f1;
                }
            }
        }
        if (type < 0) for(int i = 0; i < Lim; ++i) f[i].x = round(f[i].x / Lim);
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

    // g = f^-1
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

    // f <-- g
    void divide(int* f, int* g, int n) {
        C[0] = 1;
        for (int i = 1; i < n; ++i) C[i] = P - g[i];
        Inv(C, f, n);
    }

    // f(x) = g(x) * Q(x) + R(x)
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

    // f' --> g
    void Der(int* f, int* g, int n) {
        for (int i = 1; i < n; ++i) g[i-1] = 1LL * i * f[i] % P;
        g[n-1] = 0;
    }
    // int f --> g
    void Int(int* f, int* g, int n) {
        g[0] = 0;
        for (int i = 1; i < n; ++i) g[i] = 1LL * fpow(i, P-2) * f[i-1] % P;
    }

    // ln f = g
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

    // exp f = g
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

    // f ^ (1 / 2) = g
    void Sqrt(int* f, int* g, int n) {
        g[0] = 1;
        for (int L = 0, Lim = 1, Mid = 2; Mid < 2*n; Mid <<= 1) {
            Inv(g, ig, Mid);
            while (Lim < Mid+Mid) Lim <<= 1, ++L;
            init(Lim, L);
            for (int i = 0; i < Mid; ++i) A[i] = f[i], B[i] = ig[i];
            for (int i = Mid; i < Lim; ++i) A[i] = B[i] = 0;
            NTT(A, Lim, 1), NTT(B, Lim, 1);
            for (int i = 0; i < Lim; ++i) A[i] = 1LL * A[i] * B[i] % P;
            NTT(A, Lim, -1);
            for (int i = 0; i < n; ++i) g[i] = 1LL * inv2 * (g[i] + A[i]) % P;
        }
    }
}

namespace FWT {
    const int P = 998244353;

    void Or(int* f, int n, int type) {
        for (int k = 1, Mid = 2; Mid <= n; Mid <<= 1, k <<= 1)
            for (int i = 0; i < n; i += Mid)
                for (int j = 0; j < k; ++j) f[i+j+k] = (f[i+j+k] + 1LL * type * f[i+j] % P + P) % P;
    }

    void And(int* f, int n, int type) {
        for (int k = 1, Mid = 2; Mid <= n; Mid <<= 1, k <<= 1)
            for (int i = 0; i < n; i += Mid)
                for (int j = 0; j < k; ++j) f[i+j] = (f[i+j] + 1LL * type * f[i+j+k] % P + P) % P;
    }

    void Xor(int* f, int n, int type) {
        for (int k = 1, Mid = 2; Mid <= n; Mid <<= 1, k <<= 1)
            for (int i = 0; i < n; i += Mid)
                for (int j = 0; j < k; ++j) {
                    int f0 = f[i+j], f1 = f[i+j+k];
                    f[i+j] = (f0 + f1) % P, f[i+j+k] = ((f0 - f1) % P + P) % P;
                    f[i+j] = 1LL * type * f[i+j] % P, f[i+j+k] = 1LL * type * f[i+j+k] % P;
                }
    }
}

int main() { return 0; }

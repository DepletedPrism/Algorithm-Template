// Luogu P3803
// DeP
#include <cmath>
#include <cstdio>
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

int n, m, Lim;
Complex P1[MAXN], P2[MAXN];

namespace Poly {
    const double PI = acos(-1.0);
    int L, R[MAXN];
    inline void init() {
        for (int i = 1; i < Lim; ++i) R[i] = (R[i>>1] >> 1) | ((i & 1) << (L-1));
    }

    void FFT(Complex* A, int type) {
        for (int i = 1; i < Lim; ++i)
            if (i < R[i]) swap(A[i], A[R[i]]);
        for (int Mid = 1; Mid < Lim; Mid <<= 1) {
            Complex unit(cos(PI / Mid), type * sin(PI / Mid));
            for (int i = 0; i < Lim; i += (Mid << 1)) {
                Complex w(1.0, 0.0);
                for (int j = 0; j < Mid; ++j, w = w * unit) {
                    Complex A0 = A[i+j], A1 = w * A[i+j+Mid];
                    A[i+j] = A0 + A1, A[i+j+Mid] = A0 - A1;
                }
            }
        }
        if (type < 0) for(int i = 0; i < Lim; ++i) A[i].x = round(A[i].x / Lim);
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    scanf("%d%d", &n, &m);
    for (int i = 0; i <= n; ++i) scanf("%lf", &P1[i].x);
    for (int i = 0; i <= m; ++i) scanf("%lf", &P2[i].x);
    // solve
    Lim = 1;
    while (Lim <= n+m) Lim <<= 1, ++Poly::L;
    Poly::init();
    Poly::FFT(P1, 1), Poly::FFT(P2, 1);
    for (int i = 0; i <= Lim; ++i) P1[i] = P1[i] * P2[i];
    Poly::FFT(P1, -1);
    // output
    for (int i = 0; i <= n+m; ++i) printf("%d%c", (int) P1[i].x, " \n"[i==n+m]);
    return 0;
}

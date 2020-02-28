// Luogu P5491
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

    inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
    template<typename T> inline void read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) f |= ch == '-', ch = Gc();
        while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
        if (f) x = -x;
    }
}
using IO::read;

typedef long long LL;
int Mod, i2;

struct Complex {
    LL x, y;
    Complex(int _x, int _y): x(_x), y(_y) { }
    Complex operator * (const Complex& rhs) const {
        return Complex((1LL * x * rhs.x + 1LL * i2 * y % Mod * rhs.y % Mod) % Mod,
                (1LL * x * rhs.y % Mod + 1LL * y * rhs.x % Mod) % Mod);
    }
};

Complex fpow(Complex base, int b) {
    Complex ret = Complex(1, 0);
    while (b > 0) {
        if (b & 1) ret = ret * base;
        base = base * base, b >>= 1;
    }
    return ret;
}

int fpow(int base, int b, int m) {
    int ret = 1;
    while (b > 0) {
        if (b & 1) ret = 1LL * ret * base % m;
        base = 1LL * base * base % m, b >>= 1;
    }
    return ret % m;
}

inline bool EulerCriterion(int n, int p) { return fpow(n, (p-1) / 2, p) == 1; }

inline int Cipolla(int n, int p, int* x) {
    if (!n) return x[0] = 0, 1;
    if (!EulerCriterion(n, p)) return 0;
    Mod = p;
    LL a = rand() % p;
    while (!a || EulerCriterion((a * a - n + p) % p, p)) a = rand() % p;
    i2 = (a * a - n + p) % p;
    x[0] = fpow(Complex(a, 1), (p + 1) / 2).x, x[1] = p - x[0];
    if (x[0] > x[1]) swap(x[0], x[1]);
    return 1 + (x[0] != x[1]);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    srand( time(nullptr) );
    int Ti, n, p, x[2]; read(Ti);
    while (Ti--) {
        read(n), read(p);
        int cnt = Cipolla(n, p, x);
        if (!cnt) puts("Hola!");
        for (int i = 0; i < cnt; ++i) printf("%d%c", x[i], " \n"[i==cnt-1]);
    }
    return 0;
}

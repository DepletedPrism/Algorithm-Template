// Luogu P5656
// DeP
#include <cctype>
#include <cstdio>

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

typedef long long LL;

void exgcd(LL a, LL b, LL& d, LL& x, LL& y) {
    if (!b) d = a, x = 1, y = 0;
    else exgcd(b, a % b, d, y, x), y -= a / b * x;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    int Ti; read(Ti);
    while (Ti--) {
        static LL x, y, d, a, b, c;
        read(a), read(b), read(c);
        exgcd(a, b, d, x, y);
        if (c % d != 0) puts("-1");
        else {
            a /= d, b /= d, c /= d;
            x *= c, y *= c;
            LL xmin = (x > 0 && x % b != 0)? x % b: x % b + b;
            LL ymax = (c - xmin * a) / b;
            LL ymin = (y > 0 && y % a != 0)? y % a: y % a + a;
            LL xmax = (c - ymin * b) / a;
            if (xmax > 0) {
                LL cnt = (xmax - xmin) / b + 1;
                printf("%lld %lld %lld %lld %lld\n", cnt, xmin, ymin, xmax, ymax);
            } else printf("%lld %lld\n", xmin, ymin);
        }
    }
    return 0;
}

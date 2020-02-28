// Luogu P4777
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

typedef long long LL;
const int MAXN = 1e5+5;

int n;
LL a[MAXN], m[MAXN];

namespace CRT {
    void exgcd(LL a, LL b, LL& d, LL& x, LL& y) {
        if (!b) d = a, x = 1, y = 0;
        else exgcd(b, a % b, d, y, x), y -= a/b * x;
    }

    LL sMul(LL base, LL b, LL mod) {
        LL ret = 0;
        while (b > 0) {
            if (b & 1) ret = (ret + base) % mod;
            base = (base + base) % mod, b >>= 1;
        }
        return ret;
    }

    LL exCRT() {
        LL ret = a[1], M = m[1];
        for (int i = 2; i <= n; ++i) {
            static LL c, d, x, y, mod;
            c = ((a[i] - ret) % m[i] + m[i]) % m[i];
            exgcd(M, m[i], d, x, y);
            if (c % d != 0) return -1;
            mod = m[i] / d, x = sMul(x, c/d, mod);
            ret += x * M, M *= mod, ret = (ret % M + M) % M;
        }
        return ret;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n);
    for (int i = 1; i <= n; ++i) read(m[i]), read(a[i]);
    // output
    printf("%lld\n", CRT::exCRT());
    return 0;
}

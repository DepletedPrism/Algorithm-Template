// LOJ #113
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
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

typedef long long LL;
const int MAXL = 51;

int n;

struct LinearBasis {
    LL p[MAXL];

    LinearBasis() { memset(p, 0, sizeof p); }

    inline void Add(LL x) {
        for (int i = MAXL - 1; i >= 0; --i) {
            if (!((x >> i) & 1)) continue;
            if (!p[i]) { p[i] = x; break; }
            x ^= p[i];
        }
    }

    inline LL Qry() {
        LL ret = 0;
        for (int i = MAXL - 1; i >= 0; --i)
            if (!((ret >> i) & 1)) ret ^= p[i];
        return ret;
    }
} B;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    read(n);
    while (n--) {
        static LL x;
        read(x), B.Add(x);
    }
    printf("%lld\n", B.Qry());
    return 0;
}

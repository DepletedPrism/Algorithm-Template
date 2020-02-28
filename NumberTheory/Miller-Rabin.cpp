// LOJ #143
// DeP
#include <ctime>
#include <cctype>
#include <cstdio>
#include <cstdlib>

namespace IO {
    const int MAXSIZE = 1 << 18 | 1;
    char buf[MAXSIZE], *p1, *p2;
    
    inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
    template<typename T> bool read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) { if (ch == EOF) return false; f |= ch == '-', ch = Gc(); }
        while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
        if (f) x = -x;
        return true;
    }
}
using IO::read;

typedef long long LL;

LL x;

namespace MR {
    const int MS = 20;

    inline LL qMul(LL a, LL b, LL mod) {
        return (a*b - LL((long double)a/mod*b)*mod + mod) % mod;
    }

    LL qPower(LL base, LL b, LL mod) {
        if (!b) return 1 % mod;
        LL ret = 1;
        while (b > 0) {
            if (b & 1) ret = qMul(ret, base, mod);
            base = qMul(base, base, mod); b >>= 1;
        }
        return ret;
    }

    bool isPrime(LL p) {
        if (p == 2) return true;
        if (p == 1 || !(p & 1)) return false;
        for (int s = 1; s <= MS; ++s) {
            LL a = rand() % (p-1) + 1;
            if (qPower(a, p-1, p) != 1) return false;
            LL k = p-1;
            while (!(k & 1)) {
                k >>= 1;
                LL t = qPower(a, k, p);
                if (t != p-1 && t != 1) return false;
                if (t == p-1) break;
            }
        }
        return true;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    srand((unsigned) time(NULL));
    while (read(x))
        puts(MR::isPrime(x)? "Y": "N");
    return 0;
}

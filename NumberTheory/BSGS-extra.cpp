// Luogu P4195
// DeP
#include <cmath>
#include <cctype>
#include <cstdio>
#include <unordered_map>
using namespace std;

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

int fpow(int base, int b, int m) {
    if (!b) return 1 % m;
    int ret = 1;
    while (b > 0) {
        if (b & 1) ret = int( 1LL * base * ret % m );
        base = int( 1LL * base * base % m ), b >>= 1;
    }
    return ret;
}

int gcd(int a, int b) { return b? gcd(b, a % b): a; }

int BSGS(int a, int b, int P, int d) {
    static unordered_map<int, int> M;
    if (a % P == 0) return -1;
    M.clear();
    int t, now = b % P, m = (int) ceil( sqrt(P) );
    M[now] = 0;
    for (int j = 1; j <= m; ++j)
        now = int( 1LL * now * a % P ), M[now] = j;
    t = fpow(a, m, P), now = d;
    for (int i = 1; i <= m; ++i) {
        now = int( 1LL * now * t % P );
        if (M.count(now)) return ((i * m - M[now]) % P + P) % P;
    }
    return -1;
}

int exBSGS(int a, int b, int P) {
    int g, d = 1, k = 0;
    while ((g = gcd(a, P)) != 1) {
        if (b % g != 0) return -1;
        ++k, b /= g, P /= g, d = int( 1LL * d * (a/g) % P );
        if (d == b) return k;
    }
    return (d = BSGS(a, b, P, d)) == -1? -1: d + k;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    int a, b, P;
    while (read(a), read(P), read(b), a && b && P) {
        int res = exBSGS(a, b, P);
        if (~res) printf("%d\n", res); else puts("No Solution");
    }
    return 0;
}

// Luogu P3370
// DeP
#include <set>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

namespace IO {
    const int MAXSIZE = 1 << 20;
    char buf[MAXSIZE], *p1, *p2;

    inline int Gc() { return getchar(); }
    template<class T> void read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) { f |= ch == '-'; ch = Gc(); }
        while (isdigit(ch)) { x = x * 10 + ch - '0'; ch = Gc(); }
        if (f) x = -x;
    }
}
using IO::read;

const int MAXN = 10005;
const int MOD1 = 1e9+7, MOD2 = 998244353, BASE = 19260817;

struct Hash {
    int H1, H2;

    Hash() { H1 = H2 = 0; }
    Hash(int x, int y): H1(x), H2(y) { }

    bool operator < (const Hash& rhs) const {
        return H1 < rhs.H1 || (H1 == rhs.H1 && H2 < rhs.H2);
    }

    inline void insert(int ch) {
        H1 = (1LL * H1 * BASE + ch) % MOD1;
        H2 = (1LL * H2 * BASE + ch) % MOD2;
    }
};

int n;
char str[MAXN];
set<Hash> C;

Hash solve(char* S) {
    Hash ret;
    int m = strlen(S);
    for (int i = 0; i < m; ++i) ret.insert(S[i]);
    return ret;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("testdata.in", "r", stdin);
#endif
    // input
    read(n);
    for (int i = 1; i <= n; ++i) {
        scanf("%s", str);
        C.insert(solve(str));
    }
    // output
    printf("%lu", C.size());
    return 0;
}

// Luogu P3865
// DeP
#include <cctype>
#include <cstdio>
#include <algorithm>
using namespace std;

namespace IO {
    const int MAXSIZE = 1 << 18 | 1;
    char buf[MAXSIZE], *p1, *p2;

    inline int Gc() {
        return p1 == p2 &&
            (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++;
    }
    template<typename T> void read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) f |= ch == '-', ch = Gc();
        while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
        if (f) x = -x;
    }
}
using IO::read;

const int MAXN = 1e5+5;

int n, m;
int A[MAXN];

namespace ST {
    const int LOG = 21;

    int MaxA[LOG][MAXN], lg2[MAXN];

    void init() {
        lg2[1] = 0;
        for (int i = 2; i <= n; ++i) lg2[i] = lg2[i/2] + 1;
        for (int i = 1; i <= n; ++i) MaxA[0][i] = A[i];
        for (int j = 1; j < LOG; ++j)
            for (int i = 1; i+(1 << j)-1 <= n; ++i)
                MaxA[j][i] = max(MaxA[j-1][i], MaxA[j-1][i+(1<<(j-1))]);
    }

    int queryMax(int L, int R) {
        int k = lg2[R-L+1];
        return max(MaxA[k][L], MaxA[k][R-(1<<k)+1]);
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    read(n); read(m);
    for (int i = 1; i <= n; ++i) read(A[i]);
    // init
    ST::init();
    // output
    while (m--) {
        int L, R;
        read(L); read(R);
        printf("%d\n", ST::queryMax(L, R));
    }
    return 0;
}

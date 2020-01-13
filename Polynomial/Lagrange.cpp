// Luogu P4781
// DeP
#include <cstdio>

const int MAXN = 2e3+5, P = 998244353;

int n, K;
int X[MAXN], Y[MAXN];

int fpow(int base, int b) {
    int ret = 1;
    while (b > 0) {
        if (b & 1) ret = int( 1LL * ret * base % P );
        base = int( 1LL * base * base % P ), b >>= 1;
    }
    return ret;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    scanf("%d%d", &n, &K);
    for (int i = 1; i <= n; ++i) scanf("%d%d", X+i, Y+i);
    // solve
    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        int s1 = Y[i] % P, s2 = 1;
        for (int j = 1; j <= n; ++j) if (i != j) {
            s1 = int( 1LL * (K - X[j]) % P * s1 % P );
            s2 = int( 1LL * (X[i] - X[j]) % P * s2 % P );
        }
        ans = int( ans + 1LL * s1 * fpow(s2, P-2) % P );
        ans = (ans % P + P) % P;
    }
    // output
    printf("%d\n", ans);
    return 0;
}

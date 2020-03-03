// Luogu P5495
// DeP
#include <cstdio>

const int MAXN = 2e7+5;

int n; unsigned seed;
unsigned f[MAXN];

inline unsigned getnxt() {
    return seed ^= seed << 13, seed ^= seed >> 17, seed ^= seed << 5, seed;
}

bool notPrime[MAXN];
int Prime[MAXN], tot;

void EulerSieve() {
    notPrime[1] = true;
    for (int i = 2; i <= n; ++i) {
        if (!notPrime[i]) Prime[++tot] = i;
        for (int j = 1; j <= tot && i * Prime[j] <= n; ++j) {
            notPrime[i * Prime[j]] = true;
            if (i % Prime[j] == 0) break;
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    scanf("%d%d", &n, &seed);
    for (int i = 1; i <= n; ++i) f[i] = getnxt();
    // solve
    EulerSieve();
    for (int j = 1; j <= tot; ++j)
        for (int i = 1; i * Prime[j] <= n; ++i) f[i * Prime[j]] += f[i];
    // output
    unsigned ans = 0;
    for (int i = 1; i <= n; ++i) ans ^= f[i];
    printf("%u\n", ans);
    return 0;
}

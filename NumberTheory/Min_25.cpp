// Luogu P5325
// DeP
#include <cmath>
#include <cstdio>

typedef long long LL;
const int MAXM = 1e5 + 114514, P = 1e9+7;
const int inv2 = 500000004, inv6 = 166666668;

LL n; int m, widx;
bool notPrime[MAXM];
int Prime[MAXM], tot;
LL w[MAXM], idx1[MAXM], idx2[MAXM];
LL G1[MAXM], G2[MAXM], sp1[MAXM], sp2[MAXM];

void EulerSieve(int limit = m) {
    notPrime[1] = true;
    for (int i = 2; i <= limit; ++i) {
        if (!notPrime[i]) {
            Prime[++tot] = i;
            sp1[tot] = (sp1[tot - 1] + i) % P;
            sp2[tot] = (sp2[tot - 1] + 1LL * i * i % P) % P;
        }
        for (int j = 1; j <= tot && i*Prime[j] <= limit; ++j) {
            notPrime[i*Prime[j]] = true;
            if (i % Prime[j] == 0) break;
        }
    }
}

LL Sum(LL x, int k) {
    if (x <= 1 || x < Prime[k]) return 0;
    int idx = (x <= m)? idx1[x]: idx2[n / x];
    LL ret = ((G2[idx] - G1[idx] + P) % P - (sp2[k-1] - sp1[k-1] + P) % P + P) % P;
    for (int i = k; 1LL * Prime[i] * Prime[i] <= x; ++i) {
        LL t1 = Prime[i], t2 = 1LL * Prime[i] * Prime[i];
        for (; t2 <= x; t1 = t2, t2 *= Prime[i]) {
            LL s1 = (t1 % P * (t1 % P) % P - t1 % P + P) % P, s2 = (t2 % P * (t2 % P) % P - t2 % P + P) % P;
            ret = (ret + s1 * Sum(x / t1, i + 1) % P + s2) % P;
        }
    }
    return ret;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    scanf("%lld", &n);
    // init
    m = sqrt(n), EulerSieve(m + 114);
    for (LL R, L = 1; L <= n; L = R + 1) {
        LL d = n / L;
        R = n / d, w[++widx] = d;
        G1[widx] = (d % P * (d % P + 1) % P * inv2 % P - 1 + P) % P;
        G2[widx] = (d % P * (d % P + 1) % P * (2*d % P + 1) % P * inv6 % P - 1 + P) % P;
        if (d <= m) idx1[d] = widx; else idx2[R] = widx;
    }
    for (int j = 1; j <= tot; ++j)
        for (int i = 1; 1LL * Prime[j] * Prime[j] <= w[i]; ++i) {
            LL v = w[i] / Prime[j];
            int idx = (v <= m)? idx1[v]: idx2[n / v];
            G1[i] = (G1[i] - (G1[idx] - sp1[j-1] + P) % P * Prime[j] % P + P) % P;
            G2[i] = (G2[i] - (G2[idx] - sp2[j-1] + P) % P * Prime[j] % P * Prime[j] % P + P) % P;
        }
    // solve
    printf("%lld\n", (Sum(n, 1) + 1) % P);
    return 0;
}

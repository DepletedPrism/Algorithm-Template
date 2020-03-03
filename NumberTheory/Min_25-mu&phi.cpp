// Luogu P4213
// DeP
#include <cmath>
#include <cctype>
#include <cstdio>
using namespace std;

namespace IO {
    const int MAXSIZE = 1 << 18 | 1;
    char buf[MAXSIZE], *p1, *p2;

    inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
    template<typename T> inline void read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) f |= ch == '-', ch = Gc();
        while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
        if (f) x = -x;
    }
}
using IO::read;

typedef long long LL;
const int MAXN = 46345;

bool notPrime[MAXN];
int sp[MAXN], Prime[MAXN], tot;

int n, m, widx;
LL G1[MAXN << 1]; int G2[MAXN << 1];
int w[MAXN << 1], idx1[MAXN], idx2[MAXN];

void EulerSieve() {
    notPrime[1] = true;
    for (int i = 2; i < MAXN; ++i) {
        if (!notPrime[i]) Prime[++tot] = i, sp[tot] = sp[tot-1] + i;
        for (int j = 1; j <= tot && i*Prime[j] < MAXN; ++j) {
            notPrime[i*Prime[j]] = true;
            if (i % Prime[j] == 0) break;
        }
    }
}

int Smu(int x, int k) {
    if (x <= 1 || x < Prime[k]) return 0;
    int idx = (x <= m)? idx1[x]: idx2[n / x], ret = -G2[idx] + (k-1);
    for (int i = k; i <= widx && 1LL * Prime[i] * Prime[i] <= x; ++i)
        ret -= Smu(x / Prime[i], i + 1);
    return ret;
}

LL Sphi(int x, int k) {
    if (x <= 1 || x < Prime[k]) return 0;
    int idx = (x <= m)? idx1[x]: idx2[n / x];
    LL ret = (G1[idx] - G2[idx]) - (sp[k-1] - (k-1));
    for (int i = k; i <= widx && 1LL * Prime[i] * Prime[i] <= x; ++i) {
        for (LL t1 = 1, t2 = Prime[i]; t2 * Prime[i] <= x; t1 = t2, t2 *= Prime[i])
            ret += t1 * (Prime[i]-1) * Sphi(x / t2, i + 1) + t2 * (Prime[i]-1);
    }
    return ret;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    EulerSieve();
    int Ti; read(Ti);
    while (Ti--) {
        read(n); widx = 0, m = sqrt(n);
        for (int L = 1, R; L <= n; L = R + 1) {
            R = n / (n / L), w[++widx] = n / L;
            G1[widx] = 1LL * w[widx] * (w[widx] + 1) / 2 - 1;
            G2[widx] = w[widx] - 1;
            if (w[widx] <= m) idx1[w[widx]] = widx; else idx2[R] = widx;
        }
        for (int j = 1; j <= tot; ++j)
            for (int i = 1; i <= widx && 1LL * Prime[j] * Prime[j] <= w[i]; ++i) {
                int v = w[i] / Prime[j], idx = (v <= m)? idx1[v]: idx2[n / v];
                G1[i] -= 1LL * Prime[j] * (G1[idx] - sp[j-1]);
                G2[i] -= G2[idx] - (j-1);
            }
        printf("%lld %d\n", Sphi(n, 1) + 1, Smu(n, 1) + 1);
    }
    return 0;
}

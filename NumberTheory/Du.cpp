// Luogu P4213
// DeP
#include <map>
#include <cctype>
#include <cstdio>
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
const int MAXN = 1664523;

bool notPrime[MAXN];
int Prime[MAXN], tot;

int phi[MAXN], mu[MAXN], Mu[MAXN];
LL Phi[MAXN];

map<int, int> mMu;
map<int, LL> mPhi;

void EulerSieve() {
    notPrime[1] = true, mu[1] = phi[1] = 1;
    for (int i = 2; i < MAXN; ++i) {
        if (!notPrime[i])
            Prime[++tot] = i, mu[i] = -1, phi[i] = i - 1;
        for (int j = 1; j <= tot && i * Prime[j] < MAXN; ++j) {
            notPrime[i * Prime[j]] = true;
            if (i % Prime[j] == 0) {
                mu[i * Prime[j]] = 0;
                phi[i * Prime[j]] = phi[i] * Prime[j];
                break;
            }
            mu[i * Prime[j]] = -mu[i];
            phi[i * Prime[j]] = phi[i] * (Prime[j] - 1);
        }
    }
    for (int i = 1; i < MAXN; ++i)
        Mu[i] = Mu[i-1] + mu[i], Phi[i] = Phi[i-1] + phi[i];
}

int Smu(int n) {
    if (n < MAXN) return Mu[n];
    if (mMu.count(n) > 0) return mMu[n];
    int ret = 1;
    for (int R, L = 2; L <= n; L = R + 1)
        R = n / (n / L), ret -= Smu(n / L) * (R-L+1);
    return mMu[n] = ret;
}

LL Sphi(int n) {
    if (n < MAXN) return Phi[n];
    if (mPhi.count(n) > 0) return mPhi[n];
    LL ret = 1LL * n * (n+1) / 2;
    for (int R, L = 2; L <= n; L = R + 1)
        R = n / (n / L), ret -= Sphi(n / L) * (R-L+1);
    return mPhi[n] = ret;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    EulerSieve();
    int Ti; read(Ti);
    while (Ti--) {
        static int n;
        read(n), printf("%lld %d\n", Sphi(n), Smu(n));
    }
    return 0;
}

// Luogu P3809
// DeP
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 1e6+5;

int n;
char S[MAXN];

namespace SA {
    int sa[MAXN], rnk[MAXN], id[MAXN], px[MAXN], cnt[MAXN];

    inline bool cmp(const int& x, const int& y, const int& k) {
        return id[x] == id[y] && id[x+k] == id[y+k];
    }

    inline void build(int m) {
        static int k, i, p;
        for (i = 0; i < n; ++i) ++cnt[rnk[i] = S[i]];
        for (i = 1; i < m; ++i) cnt[i] += cnt[i-1];
        for (i = n-1; i >= 0; --i) sa[--cnt[rnk[i]]] = i;
        for (k = 1; k <= n && p < n; k <<= 1, m = p) {
            for (p = 0, i = n-k; i < n; ++i) id[p++] = i;
            for (i = 0; i < n; ++i) if (sa[i] >= k) id[p++] = sa[i] - k;
            memset(cnt, 0, sizeof (int) * m);
            for (i = 0; i < n; ++i) ++cnt[px[i] = rnk[id[i]]];
            for (i = 1; i < m; ++i) cnt[i] += cnt[i-1];
            for (i = n-1; i >= 0; --i) sa[--cnt[px[i]]] = id[i];
            swap(rnk, id), p = 1, rnk[sa[0]] = 0;
            for (i = 1; i < n; ++i) rnk[sa[i]] = cmp(sa[i], sa[i-1], k)? p-1: p++;
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    scanf("%s", S);
    // solve
    n = (int) strlen(S);
    SA::build(128);
    // output
    for (int i = 0; i < n; ++i) printf("%d ", SA::sa[i] + 1);
    return 0;
}

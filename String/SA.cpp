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
    int sa[MAXN], rnk[MAXN], ht[MAXN], id[MAXN], px[MAXN], cnt[MAXN];

    inline bool cmp(const int& x, const int& y, const int& k) {
        return id[x] == id[y] && id[x+k] == id[y+k];
    }

    inline void build(int m) {
        int i, k, p = 0;
        for (i = 1; i <= n; ++i) ++cnt[rnk[i] = S[i]];
        for (i = 1; i <= m; ++i) cnt[i] += cnt[i-1];
        for (i = n; i; --i) sa[cnt[rnk[i]]--] = i;
        for (k = 1; k <= n && p < n; k <<= 1, m = p) {
            for (p = 0, i = n; i > n-k; --i) id[++p] = i;
            for (i = 1; i <= n; ++i) if (sa[i] > k) id[++p] = sa[i] - k;
            memset(cnt, 0, sizeof (int) * (m+1));
            for (i = 1; i <= n; ++i) ++cnt[px[i] = rnk[id[i]]];
            for (i = 1; i <= m; ++i) cnt[i] += cnt[i-1];
            for (i = n; i; --i) sa[cnt[px[i]]--] = id[i];
            swap(rnk, id), rnk[sa[1]] = p = 1;
            for (i = 2; i <= n; ++i) rnk[sa[i]] = cmp(sa[i], sa[i-1], k)? p: ++p;
        }
        for (i = 1; i <= n; ++i) rnk[sa[i]] = i;
        for (k = 0, i = 1; i <= n; ++i) {
            if (k) --k;
            while (S[i + k] == S[sa[rnk[i]-1] + k]) ++k;
            ht[rnk[i]] = k;
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    scanf("%s", S + 1);
    // solve
    n = (int) strlen(S + 1); SA::build(128);
    // output
    for (int i = 1; i <= n; ++i) printf("%d ", SA::sa[i]);
    putchar('\n');
    return 0;
}

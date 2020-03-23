// Luogu P3375
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>

const int MAXN = 1000005;

char S1[MAXN], S2[MAXN];

namespace KMP {
    int fail[MAXN];

    void getFail(char* P) {
        int m = strlen(P);
        fail[0] = fail[1] = 0;
        for (int i = 1; i < m; ++i) {
            int j = fail[i];
            while (j && P[i] != P[j]) j = fail[j];
            fail[i+1] = (P[i] == P[j]? j+1: 0);
        }
    }

    void solve(char* T, char* P) {
        int n = strlen(T), m = strlen(P), j = 0;
        getFail(P);
        for (int i = 0; i < n; ++i) {
            while (j && P[j] != T[i]) j = fail[j];
            if (P[j] == T[i]) ++j;
            if (j == m) printf("%d\n", i-m+2);
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("testdata.in", "r", stdin);
    freopen("output.out", "w", stdout);
#endif
    scanf("%s%s", S1, S2);
    KMP::solve(S1, S2);
    int m = strlen(S2);
    for (int i = 1; i <= m; ++i)
        printf("%d ", KMP::fail[i]);
    return 0;
}

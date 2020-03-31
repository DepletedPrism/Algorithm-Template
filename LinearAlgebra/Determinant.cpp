// Luogu P4111
// DeP
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 11, MAXM = MAXN * MAXN, MOD = 1e9;

int A[MAXM][MAXM];

int Gauss(int n) {
    int ret = 1;
    for (int i = 1; i <= n; ++i) {
        for (int k = i + 1; k <= n; ++k)
            while (A[k][i]) {
                int d = A[i][i] / A[k][i];
                for (int j = i; j <= n; ++j)
                    A[i][j] = (A[i][j] - 1LL * d * A[k][j] % MOD + MOD) % MOD;
                swap(A[i], A[k]), ret = MOD - ret;
            }
        ret = (1LL * ret * A[i][i] % MOD + MOD) % MOD;
    }
    return ret;
}

int n, m;
char G[MAXN][MAXN];
int Idx[MAXN][MAXN], idx;

inline void AddEdge(const int& u, const int& v) {
    ++A[u][u], ++A[v][v], --A[u][v], --A[v][u];
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    // input
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%s", G[i] + 1);
    // solve
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            if (G[i][j] == '.') Idx[i][j] = ++idx;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j) if (G[i][j] == '.') {
            if (G[i + 1][j] == '.') AddEdge(Idx[i][j], Idx[i + 1][j]);
            if (G[i][j + 1] == '.') AddEdge(Idx[i][j], Idx[i][j + 1]);
        }
    // output
    printf("%d\n", Gauss(idx - 1));
    return 0;
}

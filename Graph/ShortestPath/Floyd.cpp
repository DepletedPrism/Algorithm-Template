// Luogu P2910
// DeP
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1e2 + 5, MAXM = 1e4 + 5;
const int INF = 0x3f3f3f3f;

int n, m;
int A[MAXM], G[MAXN][MAXN];

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; ++i) scanf("%d", A + i);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j) scanf("%d", G[i] + j);
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j) G[i][j] = min(G[i][j], G[i][k] + G[k][j]);
    // output
    int ans = 0;
    for (int i = 1; i < m; ++i) ans += G[A[i]][A[i + 1]];
    printf("%d\n", ans);
    return 0;
}

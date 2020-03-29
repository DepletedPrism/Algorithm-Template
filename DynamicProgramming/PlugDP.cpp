// Luogu P5056
// DeP
#include <map>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long LL;
const int MAXN = 12;

int n, m, endr, endc;
char G[MAXN][MAXN+1];

struct State {
    int g[MAXN+1], tot;

    State() { memset(this, 0, sizeof *this); }

    inline void Normalize() {
        static int rep[MAXN];
        memset(rep, -1, sizeof rep);
        rep[0] = 0, tot = 1;
        for (int i = 0; i <= m; ++i) {
            if (rep[g[i]] < 0) rep[g[i]] = tot++;
            g[i] = rep[g[i]];
        }
    }

    inline void merge(int from, int to) {
        if (from == to) return;
        for (int i = 0; i <= m; ++i) if (g[i] == from) g[i] = to;
    }

    inline LL encode() {
        LL ret = 0;
        for (int i = 0; i <= m; ++i) ret = ret * 12 + g[i];
        return ret;
    }

    inline bool check(int row, int col, int s, State& T) const {
        if ((row == n-1 && (s & 4) != 0) || (row == 0 && (s & 8) != 0)) return false;
        if ((col == m-1 && (s & 1) != 0) || (col == 0 && (s & 2) != 0)) return false;
        if (((s & 2) == 0 && g[m] != 0) || ((s & 8) == 0 && g[col] != 0)) return false;
        if (((s & 2) != 0 && g[m] == 0) || ((s & 8) != 0 && g[col] == 0)) return false;
        T.tot = tot;
        for (int i = 0; i <= m; ++i) T.g[i] = g[i];
        switch (s) {
            case 10: T.merge(g[col], g[m]), T.g[col] = T.g[m] = 0; break;
            case 9: T.g[col] = 0, T.g[m] = g[col]; break;
            case 6: T.g[col] = g[m], T.g[m] = 0; break;
            case 5: T.g[col] = T.g[m] = T.tot++; break;
        }
        if (s == 10 && g[col] == g[m])
            if (find(T.g, T.g+m, g[col]) == T.g+m) return row == endr && col == endc;
        return true;
    }
};

map<LL, LL> f[MAXN][MAXN];

LL dfs(int row, int col, State& S) {
    if (col == m) ++row, col = 0;
    S.Normalize();
    if (row == n) return S.tot == 1;
    LL key = S.encode(), ret = 0;
    if (f[row][col].count(key) > 0) return f[row][col][key];
    State T;
    if (G[row][col] == '*') {
        if (S.check(row, col, 0, T)) ret += dfs(row, col + 1, T);
    } else {
        if (S.check(row, col, 12, T)) ret += dfs(row, col + 1, T);
        if (S.check(row, col, 10, T)) ret += dfs(row, col + 1, T);
        if (S.check(row, col, 9, T)) ret += dfs(row, col + 1, T);
        if (S.check(row, col, 6, T)) ret += dfs(row, col + 1, T);
        if (S.check(row, col, 5, T)) ret += dfs(row, col + 1, T);
        if (S.check(row, col, 3, T)) ret += dfs(row, col + 1, T);
    }
    return f[row][col][key] = ret;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.in", "r", stdin);
#endif
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; ++i) scanf("%s", G[i]);
    endr = endc = -1;
    for (int i = n-1; i >= 0; --i) {
        for (int j = m-1; j >= 0; --j)
            if (G[i][j] != '*') { endr = i, endc = j; break; }
        if (endr != -1) break;
    }
    State none; none.Normalize();
    printf("%lld\n", dfs(0, 0, none));
    return 0;
}

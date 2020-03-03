// HDU 2255
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

namespace IO {
    const int MAXSIZE = 1 << 18 | 1;
    char buf[MAXSIZE], *p1, *p2;

    inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
    template<typename T> inline bool read(T& x) {
        x = 0; int f = 0, ch = Gc();
        while (!isdigit(ch)) { if (ch == EOF) return false; f |= ch == '-', ch = Gc(); }
        while (isdigit(ch)) x = x * 10 + ch - '0', ch = Gc();
        if (f) x = -x;
        return true;
    }
}
using IO::read;

const int MAXN = 305;
const int INF = 0x3f3f3f3f;

int n, m;
int W[MAXN][MAXN];

namespace KM {
    int Lx[MAXN], Ly[MAXN];
    int S[MAXN], T[MAXN], Time;
    int slack[MAXN], left[MAXN];

    bool dfs(int u) {
        S[u] = Time;
        for (int v = 1; v <= n; v++) if (T[v] != Time) {
            int d = Lx[u] + Ly[v] - W[u][v];
            if (!d) {
                T[v] = Time;
                if (left[v] == -1 || dfs(left[v])) return left[v] = u, true;
            } else slack[v] = min(slack[v], d);
        }
        return false;
    }

    inline void update() {
        int a = INF;
        for (int j = 1; j <= n; j++)
            if (T[j] != Time) a = min(a, slack[j]);
        for (int i = 1; i <= n; i++) {
            if (S[i] == Time) Lx[i] -= a;
            if (T[i] == Time) Ly[i] += a; else slack[i] -= a;
        }
    }

    inline void KM() {
        for (int i = 1; i <= n; i++)
            left[i] = -1, Ly[i] = 0, Lx[i] = *max_element(W[i]+1, W[i]+n+1);
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) slack[j] = INF;
            while (true) {
                ++Time;
                if (!dfs(i)) update(); else break;
            }
        }
    }
}

int main() {
    while (read(n)) {
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) read(W[i][j]);
        KM::KM();
        int tot = 0;
        for (int i = 1; i <= n; i++)
            if (~KM::left[i]) tot += W[KM::left[i]][i];
        printf("%d\n", tot);
    }
    return 0;
}

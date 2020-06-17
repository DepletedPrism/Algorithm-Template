// UOJ #80
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
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
const int MAXN = 4e2 + 5, INF = 0x3f3f3f3f;
const LL INFLL = 0x3f3f3f3f3f3f3f3fLL;

int n, n1, n2, m;
int W[MAXN][MAXN];

namespace KM {
  int S[MAXN], T[MAXN], Time;
  int left[MAXN], right[MAXN];
  int Lx[MAXN], Ly[MAXN], slack[MAXN];

  bool dfs(int u) {
    S[u] = Time;
    for (int v = 1; v <= n; v++) if (T[v] != Time) {
      int d = Lx[u] - W[u][v] + Ly[v];
      if (!d) {
        T[v] = Time;
        if (!left[v] || dfs(left[v]))
          return left[v] = u, right[u] = v, true;
      } else slack[v] = min(slack[v], d);
    }
    return false;
  }

  inline LL KM() {
    for (int u = 1; u <= n; ++u) {
      right[u] = left[u] = 0;
      Ly[u] = 0, Lx[u] = *max_element(W[u]+1, W[u]+n+1);
    }
    for (int u = 1; u <= n; ++u) {
      for (int v = 1; v <= n; ++v) slack[v] = INF;
      ++Time;
      if (dfs(u)) continue;
      while (true) {
        int p, d = INF;
        for (int v = 1; v <= n; ++v)
          if (T[v] != Time) d = min(d, slack[v]);
        for (int v = 1; v <= n; ++v) {
          if (S[v] == Time) Lx[v] -= d;
          if (T[v] == Time) Ly[v] += d;
          else {
            slack[v] -= d;
            if (!slack[v]) p = v;
          }
        }
        if (!left[p]) break;
        S[left[p]] = T[p] = Time, p = left[p];
        for (int v = 1; v <= n; ++v)
          slack[v] = min(slack[v], Lx[p] + Ly[v] - W[p][v]);
      }
      ++Time, dfs(u);
    }
    LL ret = 0;
    for (int u = 1; u <= n; ++u) ret += Lx[u] + Ly[u];
    return ret;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n1), read(n2), read(m);
  n = max(n2, n1);
  for (int u, v, w, i = 1; i <= m; ++i)
    read(u), read(v), read(w), W[u][v] = max(W[u][v], w);

  printf("%lld\n", KM::KM());
  for (int u = 1; u <= n1; ++u)
    printf("%d%c", W[u][KM::right[u]]? KM::right[u]: 0, " \n"[u == n1]);
  return 0;
}

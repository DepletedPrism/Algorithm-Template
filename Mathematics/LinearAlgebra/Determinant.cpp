// Luogu P6178
// DeP
#include <bits/stdc++.h>
using namespace std;

constexpr int MAXN = 3e2 + 5, MAXM = 1e5 + 5, P = 1e9 + 7;

int fpow(int base, int b) {
  int ret = 1;
  while (b > 0) {
    if (b & 1) ret = 1LL * ret * base % P;
    base = 1LL * base * base % P, b >>= 1;
  }
  return ret;
}

int A[MAXN][MAXN];

int Gauss(int n) {
  int ret = 1;
  for (int i = 2; i <= n; ++i) {
    for (int k = i + 1; k <= n; ++k)
      while (A[k][i] != 0) {
        int d = A[i][i] / A[k][i];
        for (int j = i; j <= n; ++j)
          A[i][j] = (A[i][j] - 1LL * d * A[k][j] % P + P) % P;
        swap(A[k], A[i]), ret = P - ret;
      }
    ret = (1LL * ret * A[i][i] % P + P) % P;
  }
  return ret;
}

int Det(int n) {
  int ret = 1;
  for (int i = 2; i <= n; ++i) {
    if (A[i][i] == 0)
      for (int j = i + 1; j <= n; ++j)
        if (A[j][i] > 0) { swap(A[i], A[j]); break; }
    if (A[i][i] == 0) return 0;
    for (int j = 2; j <= n; ++j) if (i != j) {
      int d = 1LL * A[j][i] * fpow(A[i][i], P - 2) % P;
      for (int k = 2; k <= n; ++k) if (A[i][k] > 0)
        A[j][k] = (A[j][k] - 1LL * d * A[i][k] % P + P) % P;
    }
    ret = 1LL * ret * A[i][i] % P;
  }
  return ret;
}

int n, m, t;

inline void addEdge(int u, int v, int w) {
  if (t == 0) {
    A[u][u] = (A[u][u] + w) % P, A[u][v] = (A[u][v] - w + P) % P;
    A[v][v] = (A[v][v] + w) % P, A[v][u] = (A[v][u] - w + P) % P;
  } else
    A[v][v] = (A[v][v] + w) % P, A[u][v] = (A[u][v] - w + P) % P;
}

int main() {
  scanf("%d%d%d", &n, &m, &t);
  for (int u, v, w, i = 1; i <= m; ++i)
    scanf("%d%d%d", &u, &v, &w), addEdge(u, v, w);

  printf("%d\n", Det(n));
  return 0;
}

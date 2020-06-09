// Luogu P2455
// DeP
#include <cmath>
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 55;
const double EPS = 1e-9;

int n;
double A[MAXN][MAXN];

namespace Gauss {
  inline bool cmp(const int& k, const int& x, const int& y) {
    if (fabs(fabs(A[x][k]) - fabs(A[y][k]))) return fabs(A[x][k]) > fabs(A[y][k]);
    for (int i = k+1; i <= n; ++i)
      if (fabs(fabs(A[x][i]) - fabs(A[y][i]))) return fabs(A[x][i]) < fabs(A[y][i]);
    return false;
  }

  int solve() {
    for (int i = 1; i <= n; ++i) {
      int r = i;
      for (int j = i+1; j <= n; ++j)
        if (cmp(i, j, r)) r = j;
      if (r != i) swap(A[r], A[i]);
      if (fabs(A[i][i]) < EPS) continue;
      for (int j = 1; j <= n; ++j) if (i != j) {
        double d = A[j][i] / A[i][i];
        for (int k = i; k <= n+1; ++k) A[j][k] -= d * A[i][k];
      }
    }
    for (int i = 1; i <= n; ++i) {
      if (fabs(A[i][i]) < EPS && fabs(A[i][n+1]) > EPS) return -1;
      if (fabs(A[i][i]) < EPS && fabs(A[i][n+1]) < EPS) return 0;
    }
    for (int i = 1; i <= n; ++i)
      if (fabs(A[i][i]) > EPS) A[i][n+1] /= A[i][i];
    for (int i = 1; i <= n; ++i)
      if (fabs(A[i][n+1]) < EPS) A[i][n+1] = 0;
    return 1;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  // input
  scanf("%d", &n);
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n+1; ++j) scanf("%lf", A[i]+j);
  // solve
  int res = Gauss::solve();
  // output
  if (res != 1) printf("%d\n", res);
  else for (int i = 1; i <= n; ++i) printf("x%d=%.2lf\n", i, A[i][n+1]);
  return 0;
}

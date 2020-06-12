// Luogu P3805
// DeP
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 1.1e7 + 5, MAXM = MAXN << 1 | 1;

int n, m;
char S[MAXN], T[MAXM];

int MxR[MAXM];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%s", S + 1);

  n = (int) strlen(S + 1);
  T[0] = '$', T[1] = '#';
  for (int i = 1; i <= n; ++i)
    T[i << 1] = S[i], T[i << 1 | 1] = '#';
  m = n << 1 | 1;
  for (int i = 1, p = 0, Mid = 0; i <= m; ++i) {
    MxR[i] = (i < p)? min(MxR[2 * Mid - i], p - i): 1;
    while (T[i - MxR[i]] == T[i + MxR[i]]) ++MxR[i];
    if (p < i + MxR[i])
      p = i + MxR[i], Mid = i;
  }

  int ans = 0;
  for (int i = 1; i <= m; ++i) ans = max(ans, MxR[i] - 1);

  printf("%d\n", ans);
  return 0;
}

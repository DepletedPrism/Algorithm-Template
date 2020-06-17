// Luogu P3375
// DeP
#include <cstdio>
#include <cstring>
using namespace std;

const int MAXN = 1e6 + 5;

int n, m;
char T[MAXN], P[MAXN];

int fail[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%s%s", T + 1, P + 1);

  n = strlen(T + 1), m = strlen(P + 1);
  fail[1] = 0;
  for (int i = 2, j = 0; i <= m; ++i) {
    while (j > 0 && P[i] != P[j + 1]) j = fail[j];
    if (P[i] == P[j + 1]) ++j;
    fail[i] = j;
  }
  for (int i = 1, j = 0; i <= n; ++i) {
    while (j > 0 && T[i] != P[j + 1]) j = fail[j];
    if (T[i] == P[j + 1]) ++j;
    if (j == m) printf("%d\n", i - m + 1);
  }

  for (int i = 1; i <= m; ++i)
    printf("%d%c", fail[i], " \n"[i == m]);
  return 0;
}

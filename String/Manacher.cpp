// Luogu P3805
// DeP
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e7 + 5;

char str[MAXN];
int d1[MAXN], d2[MAXN];

int main(void) {
  scanf("%s", str + 1);
  int ans = 0, n = strlen(str + 1);
  for (int l = 1, r = 0, i = 1; i <= n; ++i) {
    int k = (i > r)? 1: min(r - i, d1[l + r - i]);
    while (i - k >= 1 && i + k <= n && str[i - k] == str[i + k])
      ++k;
    d1[i] = k--;
    if (i + k > r)
      l = i - k, r = i + k;
  }
  for (int i = 1; i <= n; ++i)
    ans = max(ans, d1[i] * 2 - 1);
  for (int l = 1, r = 0, i = 1; i <= n; ++i) {
    int k = (i > r)? 0: min(r - i + 1, d2[l + r - i + 1]);
    while (i - k - 1 >= 1 && i + k <= n && str[i - k - 1] == str[i + k])
      ++k;
    d2[i] = k--;
    if (i + k > r)
      l = i - k - 1, r = i + k;
  }
  for (int i = 1; i <= n; ++i)
    ans = max(ans, d2[i] * 2);
  printf("%d\n", ans);
  return 0;
}

// UOJ #35
// DeP
#include <bits/stdc++.h>
using namespace std;

constexpr int MAXN = 1e5 + 5;

int n;
char s[MAXN];

namespace SA {
  int sa[MAXN], ht[MAXN];
  int px[MAXN], id[MAXN], rnk[MAXN], cnt[MAXN];

  inline bool cmp(int x, int y, int k) {
    return id[x] == id[y] && id[x + k] == id[y + k];
  }

  inline void build(int m) {
    int i, k, p = 0;
    for (i = 1; i <= n; ++i) ++cnt[rnk[i] = s[i]];
    for (i = 1; i <= m; ++i) cnt[i] += cnt[i - 1];
    for (i = n; i; --i) sa[cnt[rnk[i]]--] = i;
    for (k = 1; k <= n && p < n; k <<= 1, m = p) {
      for (p = 0, i = n; i > n - k; --i) id[++p] = i;
      for (i = 1; i <= n; ++i)
        if (sa[i] > k) id[++p] = sa[i] - k;
      memset(cnt, 0, (m + 1) * sizeof (int));
      for (i = 1; i <= n; ++i) ++cnt[px[i] = rnk[id[i]]];
      for (i = 1; i <= m; ++i) cnt[i] += cnt[i - 1];
      for (i = n; i; --i) sa[cnt[px[i]]--] = id[i];
      swap(rnk, id), rnk[sa[1]] = p = 1;
      for (i = 2; i <= n; ++i)
        rnk[sa[i]] = cmp(sa[i], sa[i - 1], k)? p: ++p;
    }
    for (i = 1; i <= n; ++i) rnk[sa[i]] = i;
    for (k = 0, i = 1; i <= n; ++i) {
      if (k) --k;
      while (s[i + k] == s[sa[rnk[i] - 1] + k]) ++k;
      ht[rnk[i]] = k;
    }
  }
}

int main() {
  scanf("%s", s + 1);
  n = strlen(s + 1), SA::build('z');
  for (int i = 1; i <= n; ++i)
    printf("%d%c", SA::sa[i], " \n"[i == n]);
  for (int i = 2; i <= n; ++i)
    printf("%d%c", SA::ht[i], " \n"[i == n]);
  return 0;
}

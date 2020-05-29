// LOJ #103
// DeP
#include <cstdio>
#include <cstring>
using namespace std;

const int MAXN = 1e6 + 5;

int n, m;
char S[MAXN], T[MAXN];

namespace KMP {
  int fail[MAXN];

  inline void getfail() {
    int j = fail[1] = 0;
    for (int i = 2; i <= m; ++i) {
      while (j && T[i] != T[j + 1]) j = fail[j];
      fail[i] = (j += T[i] == T[j + 1]);
    }
  }

  inline int match() {
    getfail();
    int ret = 0;
    for (int i = 1, j = 0; i <= n; ++i) {
      while (j && S[i] != T[j + 1]) j = fail[j];
      if (S[i] == T[j + 1]) ++j;
      if (j == m) ++ret;
    }
    return ret;
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%s%s", S+1, T+1);
  n = strlen(S+1), m = strlen(T+1);
  printf("%d\n", KMP::match());
  return 0;
}

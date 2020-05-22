// Luogu P5410
// DeP
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

namespace IO {
  const int MAXSIZE = 1 << 26 | 1;
  char buf[MAXSIZE], *p = buf;

  inline void init() { buf[fread(buf, 1, MAXSIZE, stdin)] = '\n'; }
  inline void read(char* s) {
    while (isspace(*p)) ++p;
    while (!isspace(*p)) *s++ = *p++;
  }
}
using IO::read;

typedef long long LL;
const int MAXN = 2e7 + 5;

int n, m;
char P[MAXN], T[MAXN];

int z[MAXN], p[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  IO::init();
  read(T + 1), read(P + 1);

  LL a1 = 0, a2 = 0;
  n = strlen(P + 1), m = strlen(T + 1);

  z[1] = n, a1 ^= (n + 1);
  for (int L = 0, R = 0, i = 2; i <= n; ++i) {
    if (i <= R)
      z[i] = min(R - i + 1, z[i - L + 1]);
    while (i + z[i] <= n && P[z[i] + 1] == P[i + z[i]])
      ++z[i];
    if (i + z[i] - 1 > R)
      L = i, R = i + z[i] - 1;
    a1 ^= 1LL * i * (z[i] + 1);
  }

  for (int L = 0, R = 0, i = 1; i <= m; ++i) {
    if (i <= R)
      p[i] = min(R - i + 1, z[i - L + 1]);
    while (i + p[i] <= m && p[i] < n && P[p[i] + 1] == T[i + p[i]])
      ++p[i];
    if (i + p[i] - 1 > R)
      L = i, R = i + p[i] - 1;
    a2 ^= 1LL * i * (p[i] + 1);
  }

  printf("%lld\n%lld\n", a1, a2);
  return 0;
}

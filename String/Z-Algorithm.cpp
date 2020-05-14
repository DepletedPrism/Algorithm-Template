// Luogu P5410
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
  inline int read(char* s) {
    int lgt = 0, ch = Gc();
    while (isspace(ch)) ch = Gc();
    while (!isspace(ch)) s[++lgt] = ch, ch = Gc();
    return s[lgt + 1] = '\0', lgt;
  }
}
using IO::read;

typedef long long LL;
const int MAXN = 2e7 + 5;

int z[MAXN], p[MAXN];

void Z(char* P, const int& n) {
  z[1] = n;
  for (int L = 0, R = 0, i = 2; i <= n; ++i) {
    if (i <= R)
      z[i] = min(R - i + 1, z[i - L + 1]);
    while (i + z[i] <= n && P[z[i] + 1] == P[i + z[i]])
      ++z[i];
    if (i + z[i] - 1 > R)
      L = i, R = i + z[i] - 1;
  }
}

void ExKMP(char* P, const int& n, char* T, const int& m) {
  Z(P, n);
  for (int L = 0, R = 0, i = 1; i <= m; ++i) {
    if (i <= R)
      p[i] = min(R - i + 1, z[i - L + 1]);
    while (i + p[i] <= m && p[i] < n && P[p[i] + 1] == T[i + p[i]])
      ++p[i];
    if (i + p[i] - 1 > R)
      L = i, R = i + p[i] - 1;
  }
}

int n, m;
char P[MAXN], T[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  m = read(T), n = read(P);
  ExKMP(P, n, T, m);

  LL a1 = 0, a2 = 0;
  for (int i = 1; i <= n; ++i) a1 ^= (LL) i * (z[i] + 1);
  for (int i = 1; i <= m; ++i) a2 ^= (LL) i * (p[i] + 1);
  printf("%lld\n%lld\n", a1, a2);
  return 0;
}

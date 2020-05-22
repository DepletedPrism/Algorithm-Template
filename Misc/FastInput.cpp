// LOJ #7
// DeP
#include <cctype>
#include <cstdio>
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

  inline int read(char* s) {
    int lgt = 0, ch = Gc();
    while (isspace(ch)) ch = Gc();
    while (!isspace(ch)) s[++lgt] = ch, ch = Gc();
    return s[lgt + 1] = '\0', lgt;
  }
}
using IO::read;

typedef long long LL;
const int MAXN = 3e6;

LL x, ans;

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  for (int i = MAXN; i; --i) read(x), ans ^= x;
  printf("%lld\n", ans);
  return 0;
}


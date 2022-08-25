// LOJ #7
// DeP
#include <bits/stdc++.h>
using namespace std;

namespace IO {
  constexpr int MAXSIZE = 1 << 18 | 1;
  char buf[MAXSIZE], *p1, *p2;

  inline int gc() {
    return p1 == p2 &&
      (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++;
  }
  template<typename T> inline void read(T& x) {
    x = 0; int f = 0, ch = gc();
    while (!isdigit(ch)) f |= ch == '-', ch = gc();
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = gc();
    if (f) x = -x;
  }

  inline int read(char* s) {
    int lgt = 0, ch = gc();
    while (isspace(ch)) ch = gc();
    while (!isspace(ch)) s[lgt++] = ch, ch = gc();
    return s[lgt] = '\0', lgt;
  }
}
using IO::read;

using LL = long long;

int main() {
  LL x, ans;
  for (int i = 3e6; i; --i)
    read(x), ans ^= x;
  printf("%lld\n", ans);
  return 0;
}


// LOJ #7
// DeP
#include <bits/stdc++.h>
using namespace std;

namespace IO {
  constexpr int MAXSIZE = 1 << 18 | 1;
  char buf[MAXSIZE], *p1, *p2;

  int gc() {
    return p1 == p2 &&
      (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++;
  }

  struct Fin {
    template<typename T> Fin& operator >> (T& x) {
      x = 0; int f = 0, ch = gc();
      while (!isdigit(ch)) f |= ch == '-', ch = gc();
      while (isdigit(ch)) x = x * 10 + ch - '0', ch = gc();
      if (f) x = -x;
      return *this;
    }
    Fin& operator >> (string& s) {
      int ch = gc();
      while (isspace(ch)) ch = gc();
      while (!isspace(ch)) s.push_back(ch), ch = gc();
      return *this;
    }
  } fin;
}
using IO::fin;

int main() {
  long long x, ans = 0;
  for (int i = 3e6; i; --i)
    fin >> x, ans ^= x;
  cout << ans << '\n';
  return 0;
}

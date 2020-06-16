// Luogu P3810
// DeP
#include <cctype>
#include <cstdio>
#include <algorithm>
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
}
using IO::read;

const int MAXN = 1e5 + 5, MAXM = 2e5 + 5;

struct Item {
  int idx, a, b, c, w;
  Item(int _i = 0, int _a = 0, int _b = 0, int _c = 0, int _w = 0):
    idx(_i), a(_a), b(_b), c(_c), w(_w) { }
  bool operator < (const Item& rhs) const {
    return (a == rhs.a)? ((b == rhs.b)? c < rhs.c: b < rhs.b): a < rhs.a;
  }
  bool operator== (const Item& rhs) const {
    return !(*this < rhs) && !(rhs < *this);
  }
} A[MAXN], tmp[MAXN];

int n, m, nA, K;
int Ans[MAXN], f[MAXN];

namespace BIT {
  int C[MAXM];

  inline int lowbit(const int& x) { return x & -x; }

  inline void Mdy(const int& p, const int& d) {
    for (int i = p; i <= K; i += lowbit(i)) C[i] += d;
  }
  inline int Qry(const int& p) {
    int ret = 0;
    for (int i = p; i; i -= lowbit(i)) ret += C[i];
    return ret;
  }
}

void CDQ(const int& L, const int& R) {
  if (L >= R) return;
  int Mid = (L + R) / 2, p = L, q = Mid + 1;
  CDQ(L, Mid), CDQ(Mid + 1, R);
  for (int i = L; i <= R; ++i) {
    if ((p <= Mid && A[p].b <= A[q].b) || q > R)
      BIT::Mdy(A[p].c, A[p].w), tmp[i] = A[p++];
    else Ans[A[q].idx] += BIT::Qry(A[q].c), tmp[i] = A[q++];
  }
  for (int i = L; i <= Mid; ++i)
    BIT::Mdy(A[i].c, -A[i].w);
  for (int i = L; i <= R; ++i) A[i] = tmp[i];
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(K);
  for (int a, b, c, i = 1; i <= n; ++i)
    read(a), read(b), read(c), A[i] = Item(i, a, b, c, 1);

  sort(A+1, A+1+n);
  for (int i = 1; i <= n; ++i) {
    int j = i;
    while (j < n && A[i] == A[j + 1]) ++j;
    A[++nA] = A[i], A[nA].w = j - i + 1;
    i = j;
  }
  CDQ(1, nA);
  for (int i = 1; i <= nA; ++i)
    f[Ans[A[i].idx] + A[i].w] += A[i].w;

  for (int i = 1; i <= n; ++i) printf("%d\n", f[i]);
  return 0;
}

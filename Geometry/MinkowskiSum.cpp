// Luogu P4557
// DeP
#include <cmath>
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

typedef long long LL;
const int MAXN = 1e5+5;

namespace Geo {
  struct Vector {
    LL x, y;
    Vector(LL _x = 0, LL _y = 0): x(_x), y(_y) { }

    Vector operator + (const Vector& rhs) const {
      return Vector(x + rhs.x, y + rhs.y);
    }
    Vector operator - (const Vector& rhs) const {
      return Vector(x - rhs.x, y - rhs.y);
    }
  };
  typedef Vector Point;

  bool operator < (const Point& A, const Point& B) {
    return A.x < B.x || (A.x == B.x && A.y < B.y);
  }
  bool operator == (const Point& A, const Point& B) {
    return !(A < B) && !(B < A);
  }

  inline LL Dot(const Vector& A, const Vector& B) {
    return A.x * B.x + A.y * B.y;
  }
  inline LL Cross(const Vector& A, const Vector& B) {
    return A.x * B.y - A.y * B.x;
  }

  int ConvexHull(Point* P, int n, Point* ch) {
    int m = 0;
    sort(P, P+n);
    for (int i = 0; i < n; ++i) {
      while (m > 1 && Cross(ch[m-1] - ch[m-2], P[i] - ch[m-2]) <= 0) --m;
      ch[m++] = P[i];
    }
    int k = m;
    for (int i = n-2; i >= 0; --i) {
      while (m > k && Cross(ch[m-1] - ch[m-2], P[i] - ch[m-2]) <= 0) --m;
      ch[m++] = P[i];
    }
    return m - (n > 1);
  }
}
using namespace Geo;

int n, m, pidx, K;
Point poly[MAXN << 1];
Vector v1[MAXN], v2[MAXN];
Point P1[MAXN], P2[MAXN], ch1[MAXN], ch2[MAXN];

inline bool cmp(const Point& A, const Point& B) {
  return Cross(A, B) > 0 || (Cross(A, B) == 0 && Dot(A, A) < Dot(B, B));
}

inline bool inPoly(const Vector& v) {
  if (Cross(v, poly[0]) > 0 || Cross(v, poly[pidx-1]) < 0) return 0;
  int pos = lower_bound(poly, poly+pidx, v, cmp) - poly - 1;
  return Cross(v - poly[pos], poly[(pos+1)%pidx] - poly[pos]) <= 0;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  read(n), read(m), read(K);
  for (int x, y, i = 0; i < n; ++i)
    read(x), read(y), P1[i] = Point(x, y);
  for (int x, y, i = 0; i < m; ++i)
    read(x), read(y), P2[i] = Point(-x, -y);

  n = ConvexHull(P1, n, ch1);
  m = ConvexHull(P2, m, ch2);
  ch1[n] = ch1[0];
  for (int i = 0; i < n; ++i) v1[i] = ch1[i+1] - ch1[i];
  ch2[m] = ch2[0];
  for (int i = 0; i < m; ++i) v2[i] = ch2[i+1] - ch2[i];

  // Minkowski Sum
  poly[pidx++] = ch1[0] + ch2[0];
  int p = 0, q = 0;
  while (p < n && q < m) {
    if (Cross(v1[p], v2[q]) >= 0) poly[pidx] = poly[pidx-1] + v1[p++], ++pidx;
    else poly[pidx] = poly[pidx-1] + v2[q++], ++pidx;
  }
  while (p < n) poly[pidx] = poly[pidx-1] + v1[p++], ++pidx;
  while (q < m) poly[pidx] = poly[pidx-1] + v2[q++], ++pidx;
  while (pidx > 1 && Cross(poly[pidx-1] - poly[pidx-2], poly[0] - poly[pidx-2]) <= 0) --pidx;
  Point o = poly[0];
  for (int i = 0; i < pidx; ++i) poly[i] = poly[i] - o;

  while (K--) {
    static int x, y;
    read(x), read(y), printf("%d\n", inPoly(Vector(x, y) - o));
  }
  return 0;
}

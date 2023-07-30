// Luogu P1452
// DeP
#include <cstdio>
#include <algorithm>
using namespace std;

namespace Geo {
  struct Vector {
    int x, y;
    Vector(int _x = 0, int _y = 0): x(_x), y(_y) { }
    Vector operator - (const Vector& rhs) const {
      return Vector(x - rhs.x, y - rhs.y);
    }
  };
  typedef Vector Point;

  bool operator < (const Point& A, const Point& B) {
    return A.x < B.x || (A.x == B.x && A.y < B.y);
  }

  inline int Dot(const Vector& A, const Vector& B) {
    return A.x * B.x + A.y * B.y;
  }
  inline int Cross(const Vector& A, const Vector& B) {
    return A.x * B.y - A.y * B.x;
  }
  inline int Length2(const Vector& A) { return Dot(A, A); }

  int ConvexHull(Point* P, const int& n, Point* ch) {
    int m = 0;
    sort(P, P + n);
    for (int i = 0; i < n; ++i) {
      while (m > 1 && Cross(ch[m - 1] - ch[m - 2], P[i] - ch[m - 2]) <= 0) --m;
      ch[m++] = P[i];
    }
    int k = m;
    for (int i = n - 1; i >= 0; --i) {
      while (m > k && Cross(ch[m - 1] - ch[m - 2], P[i] - ch[m - 2]) <= 0) --m;
      ch[m++] = P[i];
    }
    return m - (n > 1);
  }

  int ConvexHullDiameter2(Point* poly, const int& n) {
    if (n == 1) return 0;
    if (n == 2) return Length2(poly[0] - poly[1]);
    int ret = 0;
    poly[n] = poly[0];
    for (int u = 0, v = 1; u < n; ++u)
      while (true) {
        int d = Cross(poly[u + 1] - poly[u], poly[v + 1] - poly[v]);
        if (d <= 0) {
          ret = max(ret, Length2(poly[u] - poly[v]));
          if (d == 0)
            ret = max(ret, Length2(poly[u] - poly[v + 1]));
          break;
        }
        v = (v + 1) % n;
      }
    return ret;
  }
}
using namespace Geo;

const int MAXN = 5e4 + 5;

int n;
Point P[MAXN], poly[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%d", &n);
  for (int x, y, i = 0; i < n; ++i)
    scanf("%d%d", &x, &y), P[i] = Point(x, y);

  int m = ConvexHull(P, n, poly);
  printf("%d\n", ConvexHullDiameter2(poly, m));
  return 0;
}

// Luogu P4196
// DeP
#include <cmath>
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1e5 + 5;

namespace Geo {
  const double INFD = 1e18, EPS = 1e-9;

  inline int dcmp(const double& p) {
    return (fabs(p) < EPS)? 0: (p < 0? -1: 1);
  }

  struct Vector {
    double x, y;
    Vector(double _x = 0.0, double _y = 0.0): x(_x), y(_y) { }
    Vector operator + (const Vector& rhs) const {
      return Vector(x + rhs.x, y + rhs.y);
    }
    Vector operator - (const Vector& rhs) const {
      return Vector(x - rhs.x, y - rhs.y);
    }
    Vector operator * (const double& p) const { return Vector(x * p, y * p); }
  };
  typedef Vector Point;

  inline double Cross(const Vector& A, const Vector& B) {
    return A.x * B.y - A.y * B.x;
  }

  inline double angle(const Vector& v) { return atan2(v.y, v.x); }

  struct Line {
    Point p; Vector v; double ang;
    Line() { p = v = Point(), ang = INFD; }
    Line(Point _p, Vector _v): p(_p), v(_v) { ang = angle(v); }
    bool operator < (const Line& rhs) const {
      return ang < rhs.ang;
    }
    inline Point point(const double& t) const { return p + v * t; }
  };

  inline Point LineIntersection(const Line& A, const Line& B) {
    return A.point(Cross(B.v, A.p - B.p) / Cross(A.v, B.v));
  }

  inline bool onLeft(const Line& L, const Point& p) {
    return dcmp(Cross(L.v, p - L.p)) > 0;
  }

  int HalfplaneIntersection(Line* L, int n, Point* poly) {
    static int head, tail;
    static Point P[MAXN]; static Line Q[MAXN];
    sort(L, L + n);
    Q[head = tail = 0] = L[0];
    for (int i = 1; i < n; ++i) {
      while (head < tail && !onLeft(L[i], P[tail - 1])) --tail;
      while (head < tail && !onLeft(L[i], P[head])) ++head;
      Q[++tail] = L[i];
      if (dcmp(Cross(Q[tail].v, Q[tail - 1].v)) == 0) {
        --tail;
        if (onLeft(Q[tail], L[i].p)) Q[tail] = L[i];
      }
      if (head < tail) P[tail - 1] = LineIntersection(Q[tail - 1], Q[tail]);
    }
    while (head < tail && !onLeft(Q[head], P[tail - 1])) --tail;
    if (tail - head <= 1) return 0;
    P[tail] = LineIntersection(Q[tail], Q[head]);
    int m = 0;
    for (int i = head; i <= tail; ++i) poly[m++] = P[i];
    return m;
  }

  double PolygonArea(const Point* poly, const int& n) {
    double ret = 0;
    for (int i = 1; i < n - 1; ++i)
      ret += Cross(poly[i] - poly[0], poly[i + 1] - poly[0]);
    return ret / 2.0;
  }
}
using namespace Geo;

int n, m, nl;
Line Ln[MAXN];
Point P[MAXN], poly[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%d", &m);
  while (m--) {
    scanf("%d", &n);
    for (int x, y, i = 0; i < n; ++i)
      scanf("%d%d", &x, &y), P[i] = Point(x, y);

    P[n] = P[0];
    for (int i = 0; i < n; ++i)
      Ln[nl++] = Line(P[i], P[i + 1] - P[i]);
  }

  int np = HalfplaneIntersection(Ln, nl, poly);
  printf("%.3lf\n", PolygonArea(poly, np));
  return 0;
}

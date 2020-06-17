// Luogu P2742
// DeP
#include <cmath>
#include <cstdio>
#include <algorithm>
using namespace std;

namespace Geo {
  const double EPS = 1e-9;

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
  };
  typedef Vector Point;

  bool operator < (const Point& A, const Point& B) {
    return dcmp(A.x - B.x)? A.x < B.x: A.y < B.y;
  }

  inline double Dot(const Vector& A, const Vector& B) {
    return A.x * B.x + A.y * B.y;
  }
  inline double Cross(const Vector& A, const Vector& B) {
    return A.x * B.y - A.y * B.x;
  }

  inline double Length(const Vector& A) { return sqrt(Dot(A, A)); }

  int ConvexHull(Point* P, const int& n, Point* ch) {
    int m = 0;
    sort(P, P + n); 
    for (int i = 0; i < n; ++i) {
      while (m > 1 && dcmp(Cross(ch[m - 1] - ch[m - 2], P[i] - ch[m - 2])) <= 0)
        --m;
      ch[m++] = P[i];
    }
    int k = m;
    for (int i = n - 2; i >= 0; --i) {
      while (m > k && dcmp(Cross(ch[m - 1] - ch[m - 2], P[i] - ch[m - 2])) <= 0)
        --m;
      ch[m++] = P[i];
    }
    return m - (n > 1);
  }
}
using namespace Geo;

const int MAXN = 1e5 + 5;

int n;
Point P[MAXN], poly[MAXN];

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    static double x, y;
    scanf("%lf%lf", &x, &y), P[i] = Point(x, y);
  }

  int m = ConvexHull(P, n, poly);
  double ans = 0.0;
  for (int i = 0; i < m; ++i)
    ans += Length(poly[i] - poly[(i + 1) % m]);

  printf("%.2lf\n", ans);
  return 0;
}

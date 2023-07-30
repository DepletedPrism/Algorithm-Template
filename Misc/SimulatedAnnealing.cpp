// Luogu P1337
// DeP
#pragma GCC optimize("inline", "Ofast")
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
using namespace std;

namespace Geo {
  const double EPS = 1e-5;

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

  inline double Dot(const Vector& A, const Vector& B) {
    return A.x * B.x + A.y * B.y;
  }
  inline double Length(const Vector& A) { return sqrt(Dot(A, A)); }
}
using namespace Geo;

const int MAXN = 1e3 + 5;

int n;
int W[MAXN];
Point P[MAXN];

inline double metaphysics() {
  return (double) rand() / RAND_MAX * 2.0 - 1.0;
}

inline double calc(const Point& p) {
  double s = 0.0;
  for (int i = 1; i <= n; ++i)
    s += Length(P[i] - p) * W[i];
  return s;
}

const double TIME_LIMIT = 0.98, DELTA = 0.995;

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  srand(time(nullptr));

  scanf("%d", &n);
  for (int x, y, i = 1; i <= n; ++i)
    scanf("%d%d%d", &x, &y, W + i), P[i] = Point(x, y);

  Point ans(0.0, 0.0);
  for (int i = 1; i <= n; ++i)
    ans.x += P[i].x, ans.y += P[i].y;
  ans.x /= n, ans.y /= n;

  double as = calc(ans);
  Point lst = ans;
  while (clock() < TIME_LIMIT * CLOCKS_PER_SEC) {
    double ls = calc(lst);
    for (double T = 6376.0; T > 1e-4; T *= DELTA) {
      double dx = metaphysics(), dy = metaphysics();
      Point np = lst + Vector(dx, dy) * T;
      double ns = calc(np);
      if (ls > ns || (double) rand() / RAND_MAX < exp((ls - ns) / T))
        lst = np, ls = ns;
      if (as > ls) ans = lst, as = ls;
      while (clock() > TIME_LIMIT * CLOCKS_PER_SEC)
        return printf("%.3lf %.3f\n", ans.x, ans.y), 0;
    }
  }
  return 0;
}

// geometry
// DeP
#include <bits/stdc++.h>
using namespace std;

// Base

constexpr double PI = acos(-1.0);
constexpr double EPS = 1e-9, INFD = 1e18;

int dcmp(const double& p) {
  return (fabs(p) < EPS)? 0: (p < 0? -1: 1);
}

double torad(const double& deg) {
  return deg / 180.0 * PI;
}
double NormalizeAngle(const double& rad) {
  return rad - 2.0 * PI * floor(rad / (2.0 * PI));
}

// Vector & Point

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
  Vector operator / (const double& p) const { return Vector(x / p, y / p); }
  Vector& operator += (const Vector& rhs) { return *this = *this + rhs; }
  Vector& operator -= (const Vector& rhs) { return *this = *this - rhs; }
  Vector& operator *= (const double& p) { return *this = *this * p; }
  Vector& operator /= (const double& p) { return *this = *this / p; }
};
using Point = Vector;

bool operator < (const Point& a, const Point& b) {
  return (dcmp(a.x - b.x) == 0)? a.y < b.y: a.x < b.x;
}
bool operator== (const Point& a, const Point& b) {
  return dcmp(a.x - b.x) == 0 && dcmp(a.y - b.y) == 0;
}

double Dot(const Vector& A, const Vector& B) {
  return A.x * B.x + A.y * B.y;
}
double Cross(const Vector& A, const Vector& B) {
  return A.x * B.y - A.y * B.x;
}
double Length(const Vector& A) {
  return sqrt(Dot(A, A));
}
double Area2(const Point& A, const Point& B, const Point& C) {
  return Cross(B - A, C - A);
}

double Angle(const Vector& A, const Vector& B) {
  return acos(Dot(A, B) / Length(A) / Length(B));
}
double angle(const Vector& A) {
  return atan2(A.y, A.x); // [-pi, pi]
}

double VectorAngleDegree(const Vector& v) {
  double ang = angle(v) * 180.0 / PI;
  while (dcmp(ang) < 0)
    ang += 360.0;
  while (dcmp(ang - 180.0) >= 0)
    ang -= 180.0;
  return ang;
}

Vector Rotate(const Vector& A, const double& rad) {
  return Vector(A.x * cos(rad) - A.y * sin(rad),
      A.y * cos(rad) + A.x * sin(rad));
}

Vector Normal(const Vector& A) {
  assert(dcmp(A.x) != 0 || dcmp(A.y) != 0);
  double L = Length(A);
  return Vector(-A.y / L, A.x / L);
}

// Line

struct Line {
  Point p; Vector v; double ang;
  Line() { p = v = Point(); ang = INFD; }
  Line(Point _p, Vector _v): p(_p), v(_v) { ang = angle(v);  }

  bool operator < (const Line& rhs) const { return ang < rhs.ang; }
  Point point(const double& t) const {
    return p + v * t;
  }
  Line move(double d) const {
    return Line(p + Normal(v) * d, v);
  }
};

Point LineIntersection(Point P, Vector v, Point Q, Vector w) {
  assert(dcmp(Cross(v, w)) != 0);
  return P + v * (Cross(w, P - Q) / Cross(v, w));
}

Point LineIntersection(const Line& A, const Line& B) {
  assert(dcmp(Cross(A.v, B.v)) != 0);
  return A.point(Cross(B.v, A.p - B.p) / Cross(A.v, B.v));
}

double DistanceToLine(Point P, Point A, Point B) {
  Vector v1 = B - A, v2 = P - A;
  return fabs(Cross(v1, v2)) / Length(v1);
}

double DistanceToSegment(Point P, Point A, Point B) {
  if (A == B) return Length(P - A);
  Vector v1 = B - A, v2 = P - A, v3 = P - B;
  if (dcmp(Dot(v1, v2)) < 0) return Length(v2);
  if (dcmp(Dot(v1, v3)) > 0) return Length(v3);
  return fabs(Cross(v1, v2)) / Length(v1);
}

Point LineProjection(Point P, Point A, Point B) {
  Vector v = B - A;
  return A + v * (Dot(v, P - A) / Dot(v, v));
}

bool SegmentProperIntersection(Point a1, Point a2,
    Point b1, Point b2) {
  double c1 = Cross(a2 - a1, b1 - a1), c2 = Cross(a2 - a1, b2 - a1),
         c3 = Cross(b2 - b1, a1 - b1), c4 = Cross(b2 - b1, a2 - b1);
  return dcmp(c1) * dcmp(c2) < 0 && dcmp(c3) * dcmp(c4) < 0;
}

bool onSegment(Point p, Point a1, Point a2) {
  return dcmp(Cross(a1-p, a2-p)) == 0 && dcmp(Dot(a1-p, a2-p)) < 0;
}

// Circle

struct Circle {
  Point c; double r;
  Circle() { c = Point(0.0, 0.0), r = 0.0; }
  Circle(Point _c, double _r): c(_c), r(_r) { }

  Point point(double a) {
    return Point(c.x + r * cos(a), c.y + r * sin(a));
  }
};

bool inCircle(const Point& p, const Circle& C) {
  return dcmp(Length(p - C.c) - C.r) <= 0;
}

int LineCircleIntersection(Line L, Circle C,
    double& t1, double& t2, vector<Point>& sol) {
  double a = L.v.x, b = L.p.x - C.c.x, c = L.v.y, d = L.p.y - C.c.y;
  double e = a*a + c*c, f = 2.0*a*b + 2.0*c*d, g = b*b + d*d - C.r*C.r;
  double delta = f*f - 4.0*e*g;
  if (dcmp(delta) < 0)
    return 0;
  if (dcmp(delta) == 0) {
    t1 = t2 = -f / (2.0 * e), sol.push_back(L.point(t1));
    return 1;
  }
  t1 = (-f + sqrt(delta)) / (2.0 * e), sol.push_back(L.point(t1));
  t2 = (-f - sqrt(delta)) / (2.0 * e), sol.push_back(L.point(t2));
  return 2;
}

int CircleCircleIntersection(Circle C1, Circle C2,
    vector<Point>& sol) {
  double d = Length(C1.c - C2.c);
  if (dcmp(d) == 0)
    return (dcmp(C1.r - C2.r) == 0)? -1: 0;
  if (dcmp(C1.r + C2.r - d) < 0)
    return 0;
  if (dcmp(fabs(C1.r - C2.r) - d) > 0)
    return 0;
  double a = angle(C2.c - C1.c);
  // da in [0, pi]
  double da = acos((C1.r * C1.r + d * d - C2.r * C2.r) / (2 * C1.r * d));
  Point p1 = C1.point(a - da), p2 = C1.point(a + da);
  sol.push_back(p1);
  if (p1 == p2) return 1;
  sol.push_back(p2);
  return 2;
}

int Tangents(Point p, Circle C, vector<Vector>& v) {
  Vector u = C.c - p;
  double dist = Length(u);
  if (dist < C.r) return 0;
  if (dcmp(dist - C.r) == 0) {
    v.push_back(Rotate(u, PI / 2.0));
    return 1;
  }
  double ang = asin(C.r / dist);
  v.push_back(Rotate(u, -ang)), v.push_back(Rotate(u, ang));
  return 2;
}

int Tangents(Circle C1, Circle C2, vector<Point>& a, vector<Point>& b) {
  if (C1.r < C2.r)
    swap(C1, C2), swap(a, b);
  double d = Length(C1.c - C2.c), rmns = C1.r - C2.r, rpls = C1.r + C2.r;
  if (d < rmns)
    return 0;
  if (dcmp(d) == 0 && dcmp(C1.r - C2.r) == 0)
    return -1;
  double base = angle(C2.c - C1.c);
  int cnt = 0;
  if (dcmp(d - rmns) == 0) {
    ++cnt;
    a.push_back(C1.point(base)), b.push_back(C2.point(base));
    return cnt;
  }
  double ang = acos(rmns / d);
  cnt += 2;
  a.push_back(C1.point(base + ang)), b.push_back(C2.point(base + ang));
  a.push_back(C1.point(base - ang)), b.push_back(C2.point(base - ang));
  if (dcmp(d - rpls) == 0) {
    ++cnt;
    a.push_back(C1.point(base)), b.push_back(C2.point(base + PI));
  } else if (d > rpls) {
    ang = acos(rpls / d);
    cnt += 2;
    a.push_back(C1.point(base + ang)), b.push_back(C2.point(base + ang + PI));
    a.push_back(C1.point(base - ang)), b.push_back(C2.point(base - ang + PI));
  }
  return cnt;
}

Circle CircumscribedCircle(Point p1, Point p2, Point p3) {
  Vector B = p2 - p1, C = p3 - p1;
  double D = 2 * Cross(B, C);
  double cx = (C.y * Dot(B, B) - B.y * Dot(C, C)) / D + p1.x,
         cy = (B.x * Dot(C, C) - C.x * Dot(B, B)) / D + p1.y;
  Point p = Point(cx, cy);
  return Circle(p, Length(p1 - p));
}

Circle InscribedCircle(Point p1, Point p2, Point p3) {
  double a = Length(p2 - p3), b = Length(p3 - p1), c = Length(p1 - p2);
  Point p = (p1 * a + p2 * b + p3 * c) / (a + b + c);
  return Circle(p, DistanceToLine(p, p1, p2));
}

Circle MinCoverCircle(vector<Point> P) {
  mt19937 rng(time(nullptr));
  shuffle(P.begin(), P.end(), rng);
  Circle C = Circle(P[0], 0.0);
  int n = P.size();
  for (int i = 1; i < n; ++i) if (!inCircle(P[i], C)) {
    C = Circle(P[i], 0.0);
    for (int j = 0; j < i; ++j) if (!inCircle(P[j], C)) {
      C = Circle((P[i] + P[j]) / 2.0, Length(P[i] - P[j]) / 2.0);
      for (int k = 0; k < j; ++k)
        if (!inCircle(P[k], C)) C = CircumscribedCircle(P[i], P[j], P[k]);
    }
  }
  return C;
}

// Polygon

double PolygonArea(const vector<Point>& P) {
  double ret = 0;
  int n = P.size();
  for (int i = 1; i + 1 < n; ++i)
    ret += Cross(P[i] - P[0], P[i + 1] - P[0]);
  return ret / 2;
}

int inPolygon(const Point& p, const vector<Point>& poly) {
  int n = poly.size(), wn = 0;
  for (int i = 0; i < n; ++i) {
    const Point &p1 = poly[i], &p2 = poly[(i + 1) % n];
    if (p == p1 || p == p2 || onSegment(p, p1, p2))
      return -1;
    int k = dcmp(Cross(p2 - p1, p - p1));
    int d1 = dcmp(p1.y - p.y), d2 = dcmp(p2.y - p.y);
    if (k > 0 && d1 <= 0 && d2 > 0) ++wn;
    if (k < 0 && d2 <= 0 && d1 > 0) --wn;
  }
  return wn != 0;
}

bool ConvexPolygonDisjoint(const vector<Point>& P1, const vector<Point>& P2) {
  int n = P1.size(), m = P2.size();
  for (int i = 0; i < n; ++i)
    if (inPolygon(P1[i], P2) != 0) return false;
  for (int i = 0; i < m; ++i)
    if (inPolygon(P2[i], P1) != 0) return false;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
      if (SegmentProperIntersection(P1[i], P1[(i+1)%n], P2[j], P2[(j+1)%m]))
        return false;
  return true;
}

// exist points on segments of convex hull when use '<='
// use '<' to avoid it
vector<Point> ConvexHull(vector<Point> P) {
  vector<Point> ch;
  int n = P.size(), m = 0;
  sort(P.begin(), P.end());
  for (int i = 0; i < n; ++i) {
    while (m > 1 && dcmp(Cross(ch[m-1] - ch[m-2], P[i] - ch[m-2])) <= 0)
      --m, ch.pop_back();
    ++m, ch.push_back(P[i]);
  }
  int k = m;
  for (int i = n - 2; i >= 0; --i) {
    while (m > k && dcmp(Cross(ch[m-1] - ch[m-2], P[i] - ch[m-2])) <= 0)
      --m, ch.pop_back();
    ++m, ch.push_back(P[i]);
  }
  if (n > 1)
    ch.pop_back();
  return ch;
}

double ConvexHullDiameter(vector<Point> P) {
  int n = P.size();
  if (n == 1) return 0;
  if (n == 2) return Length(P[0] - P[1]);
  P.push_back(P[0]);
  double ret = 0;
  for (int u = 0, v = 1; u < n; ++u)
    while (true) {
      int d = dcmp(Cross(P[u + 1] - P[u], P[v + 1] - P[v]));
      if (d <= 0) {
        ret = max(ret, Length(P[u] - P[v]));
        if (d == 0) ret = max(ret, Length(P[u] - P[v+1]));
        break;
      }
      v = (v + 1) % n;
    }
  return ret;
}

bool onLeft(const Line& L, const Point& p) {
  return dcmp(Cross(L.v, p - L.p)) > 0;
}

vector<Point> HalfplaneIntersection(vector<Line> L) {
  int head, tail, n = L.size();
  sort(L.begin(), L.end());
  vector<Point> P(n); vector<Line> Q(n);
  Q[head = tail = 0] = L[0];
  for (int i = 1; i < n; ++i) {
    while (head < tail && !onLeft(L[i], P[tail - 1]))
      --tail;
    while (head < tail && !onLeft(L[i], P[head]))
      ++head;
    Q[++tail] = L[i];
    if (dcmp(Cross(Q[tail].v, Q[tail - 1].v)) == 0) {
      --tail;
      if (onLeft(Q[tail], L[i].p)) Q[tail] = L[i];
    }
    if (head < tail)
      P[tail - 1] = LineIntersection(Q[tail - 1], Q[tail]);
  }
  while (head < tail && !onLeft(Q[head], P[tail - 1]))
    --tail;
  vector<Point> poly;
  if (tail - head <= 1)
    return poly;
  P[tail] = LineIntersection(Q[tail], Q[head]);
  for (int i = head; i <= tail; ++i)
    poly.push_back(P[i]);
  return poly;
}

int main() {
  return 0;
}

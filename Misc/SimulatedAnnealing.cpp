// Luogu P2571
// DeP
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

const double TIME_LIMIT = 0.95, DELTA = 0.93, GG = 0.95;

namespace Geo {
    const double EPS = 1e-9, INFD = 1e18;

    inline int dcmp(const double& p) {
        return (fabs(p) < EPS)? 0: (p < 0? -1: 1);
    }

    struct Vector {
        double x, y;
        Vector(double _x = 0.0, double _y = 0.0): x(_x), y(_y) { }

        Vector operator + (const Vector& rhs) const { return Vector(x + rhs.x, y + rhs.y); }
        Vector operator - (const Vector& rhs) const { return Vector(x - rhs.x, y - rhs.y); }
        Vector operator * (const double& p) const { return Vector(x * p, y * p); }
        Vector operator / (const double& p) const { return Vector(x / p, y / p); }
    };
    typedef Vector Point;

    inline double Dot(const Vector& A, const Vector& B) { return A.x * B.x + A.y * B.y; }
    inline double Length(const Vector& A) { return sqrt(Dot(A, A)); }
}
using namespace Geo;

inline void read(Point& x) { scanf("%lf%lf", &x.x, &x.y); }

Point A, B, C, D;
double Limit1, Limit2;
double p, q, r, ans, a1, a2;

inline double metaphysics() { return (double(rand()) / RAND_MAX) * 2.0 - 1.0; }

inline double update(double t1, double t2) {
    static const Vector v1 = (B - A) / (Length(B-A) + EPS), v2 = (C - D) / (Length(C-D) + EPS);
    Point E = A + v1 * (t1 * p), F = D + v2 * (t2 * q);
    double ret = t1 + t2 + Length(E - F) / r;
    if (ret < ans) ans = ret, a1 = t1, a2 = t2;
    return ret;
}

inline double fix(const double& a, const double& mod) {
    return a - floor(a / mod) * mod;
}

void SA() {
    double T = 3422.0, n1 = a1, n2 = a2;
    double now = ans;
    while (T > EPS) {
        double nxt1 = fix(n1 + T * metaphysics() + Limit1, Limit1);
        double nxt2 = fix(n2 + T * metaphysics() + Limit2, Limit2);
        if (clock() > GG * CLOCKS_PER_SEC) printf("%.2lf\n", ans), exit(0);
        double delta = update(nxt1, nxt2) - now;
        if (exp(-delta / T) > rand()) n1 = nxt1, n2 = nxt2;
        T *= DELTA;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    // freopen("input.in", "r", stdin);
#endif
    // init
    srand(time(nullptr));
    // input
    read(A), read(B), read(C), read(D);
    scanf("%lf%lf%lf", &p, &q, &r);
    // solve
    ans = Length(A - D) / r, a1 = 0.0, a2 = 0.0;
    Limit1 = (Length(A - B) + EPS) / p, Limit2 = (Length(C - D) + EPS) / q;
    while (clock() < TIME_LIMIT * CLOCKS_PER_SEC) SA();
    // output
    printf("%.2lf\n", ans);
    return 0;
}

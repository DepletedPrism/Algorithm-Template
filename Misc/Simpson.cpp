// Luogu P4525
// DeP
#include <cmath>
#include <cstdio>

double a, b, c, d;

inline double f(const double& x) {
  return (c * x + d) / (a * x + b);
}

inline double simpson(double L, double R) {
  return (R-L) / 6.0 * (f(L) + f(R) + 4.0 * f((L+R)/2.0));
}

double asr(const double& L, const double& R, const double& eps, double ans) {
  double Mid = (L + R) / 2.0;
  double fl = simpson(L, Mid), fr = simpson(Mid, R);
  if (fabs(fl + fr - ans) < 15 * eps) return fl + fr + (fl + fr - ans) / 15;
  return asr(L, Mid, eps / 2.0, fl) + asr(Mid, R, eps / 2.0, fr);
}

inline double asr(const double& L, const double& R, const double& eps) {
  return asr(L, R, eps, simpson(L, R));
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  static double L, R;
  scanf("%lf%lf%lf%lf%lf%lf", &a, &b, &c, &d, &L, &R);
  printf("%.6lf\n", asr(L, R, 1e-7));
  return 0;
}

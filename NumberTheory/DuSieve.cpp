// Luogu P4213
// DeP
#include <cmath>
#include <cstdio>
#include <unordered_map>
using namespace std;

typedef long long LL;
const int MAXM = 1664515;

bool ip[MAXM];
int pr[MAXM], cp;

LL sphi[MAXM];
int phi[MAXM], mu[MAXM], smu[MAXM];

void EulerSieve() {
  ip[1] = true, phi[1] = mu[1] = 1;
  for (int i = 2; i < MAXM; ++i) {
    if (!ip[i]) pr[++cp] = i, phi[i] = i - 1, mu[i] = -1;
    for (int j = 1; j <= cp && i * pr[j] < MAXM; ++j) {
      ip[i * pr[j]] = true;
      if (i % pr[j] == 0) {
        mu[i * pr[j]] = 0, phi[i * pr[j]] = phi[i] * pr[j];
        break;
      }
      mu[i * pr[j]] = -mu[i], phi[i * pr[j]] = phi[i] * (pr[j] - 1);
    }
  }
  for (int i = 1; i < MAXM; ++i)
    sphi[i] = sphi[i - 1] + phi[i], smu[i] = smu[i - 1] + mu[i];
}

int Mu(int n) {
  static unordered_map<int, int> M;
  if (n < MAXM) return smu[n];
  if (M.count(n) > 0) return M[n];
  int ret = 1;
  for (int R, L = 2; L <= n; L = R + 1)
    R = n / (n / L), ret -= Mu(n / L) * (R - L + 1);
  return M[n] = ret;
}

LL Phi(int n) {
  static unordered_map<int, LL> M;
  if (n < MAXM) return sphi[n];
  if (M.count(n) > 0) return M[n];
  LL ret = (LL) n * (n + 1) / 2;
  for (int R, L = 2; L <= n; L = R + 1)
    R = n / (n / L), ret -= Phi(n / L) * (R - L + 1);
  return M[n] = ret;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.in", "r", stdin);
#endif
  int Ti;
  scanf("%d", &Ti);

  EulerSieve();
  for (int n; Ti; --Ti)
    scanf("%d", &n), printf("%lld %d\n", Phi(n), Mu(n));
  return 0;
}

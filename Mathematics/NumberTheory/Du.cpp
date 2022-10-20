// Luogu P4213
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int ti;
  cin >> ti;

  int N = 2 * pow(INT_MAX, 2.0 / 3.0);
  vector<int> phi(N), mu(N), npr(N), pr;
  vector<LL> sphi(N), smu(N);
  npr[1] = true;
  phi[1] = 1, mu[1] = 1;
  for (int i = 2; i < N; ++i) {
    if (!npr[i]) {
      pr.push_back(i);
      phi[i] = i - 1, mu[i] = -1;
    }
    for (int j = 0; j < (int) pr.size() && i * pr[j] < N; ++j) {
      npr[i * pr[j]] = true;
      if (i % pr[j] == 0) {
        phi[i * pr[j]] = pr[j] * phi[i], mu[i * pr[j]] = 0;
        break;
      }
      phi[i * pr[j]] = phi[i] * (pr[j] - 1), mu[i * pr[j]] = -mu[i];
    }
  }
  for (int i = 1; i < N; ++i) {
    sphi[i] = sphi[i - 1] + phi[i];
    smu[i] = smu[i - 1] + mu[i];
  }
  function<LL(LL)> Phi = [&](LL n) {
    static unordered_map<LL, LL> mphi;
    if (n < N)
      return sphi[n];
    if (mphi.count(n) > 0)
      return mphi[n];
    LL ret = n * (n + 1LL) / 2;
    for (LL l = 2, r; l <= n; l = r + 1) {
      r = n / (n / l);
      ret -= (r - l + 1) * Phi(n / l);
    }
    return mphi[n] = ret;
  };
  function<LL(LL)> Mu = [&](LL n) -> LL {
    static unordered_map<LL, LL> mmu;
    if (n < N)
      return smu[n];
    if (mmu.count(n) > 0)
      return mmu[n];
    LL ret = 1;
    for (LL l = 2, r; l <= n; l = r + 1) {
      r = n / (n / l);
      ret -= (r - l + 1) * Mu(n / l);
    }
    return mmu[n] = ret;
  };

  for (LL n; ti > 0; --ti) {
    cin >> n;
    cout << Phi(n) << ' ' << Mu(n) << '\n';
  }
  return 0;
}

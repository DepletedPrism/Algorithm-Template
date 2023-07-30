// Luogu P1494
// DeP
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
using pll = pair<LL, LL>;

struct Qry {
  int idx, l, r;
};

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  vector<int> C(n + 1);
  for (int i = 1; i <= n; ++i)
    cin >> C[i];
  vector<Qry> Q(q);
  for (int l, r, i = 0; i < q; ++i)
    cin >> l >> r, Q[i] = {i, l, r};
  int block = n / sqrt(q);
  sort(Q.begin(), Q.end(), [&](const Qry& lhs, const Qry& rhs) -> bool {
    if (lhs.l / block != rhs.l / block)
      return lhs.l < rhs.l;
    return (lhs.r == rhs.r)? 0: ((lhs.l / block) & 1) ^ (lhs.r < rhs.r);
  });
  int now = 0, l = 1, r = 0;
  vector<int> cnt(n + 1);
  auto mdy = [&](int p) {
    now += cnt[C[p]], ++cnt[C[p]];
  };
  auto del = [&](int p) {
    --cnt[C[p]], now -= cnt[C[p]];
  };
  vector<pll> ans(q);
  for (auto a: Q) {
    while (a.l < l) mdy(--l);
    while (a.r > r) mdy(++r);
    while (a.l > l) del(l++);
    while (a.r < r) del(r--);
    if (now > 0) {
      LL s = (LL) (a.r - a.l) * (a.r - a.l + 1) / 2, g = gcd(now, s);
      ans[a.idx] = {now / g, s / g};
    } else {
      ans[a.idx] = {0, 1};
    }
  }
  for (auto p: ans) {
    if (p.first > 0) {
      LL g = gcd(p.first, p.second);
      p.first /= g, p.second /= g;
    } else {
      p = {0, 1};
    }
    cout << p.first << '/' << p.second << '\n';
  }
  return 0;
}

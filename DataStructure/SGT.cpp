// Luogu P3373
#include <bits/stdc++.h>
using namespace std;

using LL = long long;
constexpr int P = 571373;

struct Node {
  int s;
  explicit Node(int _s = 0): s(_s) {}
  Node operator + (const Node& rhs) {
    return Node((s + rhs.s) % P);
  }
};

#define lc (nd << 1)
#define rc (nd << 1 | 1)
struct SGT {
  int n;
  vector<Node> s;
  vector<int> ta, tm;

  SGT(int _n, const vector<int>& a)
      : n(_n), s(n << 2), ta(n << 2, 0), tm(n << 2, 1) {
    auto build = [&](auto self, int nd, int l, int r) {
      if (l == r) {
        s[nd] = Node(a[l - 1]);
        return;
      }
      int mid = (l + r) / 2;
      self(self, lc, l, mid);
      self(self, rc, mid + 1, r);
      maintain(nd);
    };
    build(build, 1, 1, n);
  }

  void maintain(int nd) {
    s[nd] = s[lc] + s[rc];
  }

  void pushm(int nd, LL v) {
    ta[nd] = static_cast<int>(ta[nd] * v % P);
    tm[nd] = static_cast<int>(tm[nd] * v % P);
    s[nd].s = static_cast<int>(s[nd].s * v % P);
  }
  void pusha(int nd, int L, int R, int v) {
    ta[nd] = (ta[nd] + v) % P;
    s[nd].s = static_cast<int>((s[nd].s + (R - L + 1LL) * v) % P);
  }
  void pushdown(int nd, int l, int r) {
    if (tm[nd] != 1) {
      pushm(lc, tm[nd]);
      pushm(rc, tm[nd]);
      tm[nd] = 1;
    }
    if (ta[nd] != 0) {
      int mid = (l + r) / 2;
      pusha(lc, l, mid, ta[nd]);
      pusha(rc, mid + 1, r, ta[nd]);
      ta[nd] = 0;
    }
  }

  void add(int nd, int l, int r, int opl, int opr, int v) {
    if (opl <= l && r <= opr)
      return pusha(nd, l, r, v);
    pushdown(nd, l, r);
    int mid = (l + r) / 2;
    if (opl <= mid)
      add(lc, l, mid, opl, opr, v);
    if (opr > mid)
      add(rc, mid + 1, r, opl, opr, v);
    maintain(nd);
  }
  void add(int l, int r, int v) {
    add(1, 1, n, l, r, v);
  }

  void mul(int nd, int l, int r, int opl, int opr, int v) {
    if (opl <= l && r <= opr)
      return pushm(nd, v);
    pushdown(nd, l, r);
    int mid = (l + r) / 2;
    if (opl <= mid)
      mul(lc, l, mid, opl, opr, v);
    if (opr > mid)
      mul(rc, mid + 1, r, opl, opr, v);
    maintain(nd);
  }
  void mul(int l, int r, int v) {
    mul(1, 1, n, l, r, v);
  }

  Node qry(int nd, int l, int r, int opl, int opr) {
    if (opl <= l && r <= opr)
      return s[nd];
    pushdown(nd, l, r);
    int mid = (l + r) / 2;
    if (opr <= mid)
      return qry(lc, l, mid, opl, opr);
    if (opl > mid)
      return qry(rc, mid + 1, r, opl, opr);
    // note: when the length of interval affects the result
    // use [max{opl, l}, mid] and [mid + 1, min{opr, r}]
    return qry(lc, l, mid, opl, opr) + qry(rc, mid + 1, r, opl, opr);
  }
  Node qry(int l, int r) {
    return qry(1, 1, n, l, r);
  }
};
#undef lc
#undef rc

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, q, p;
  cin >> n >> q >> p;
  vector<int> a(n);
  for (int i = 0; i < n; ++i)
    cin >> a[i];
  SGT t(n, a);
  for (int opt, l, r, x; q; --q) {
    cin >> opt >> l >> r;
    if (opt == 1) {
      cin >> x;
      t.mul(l, r, (x % p + p) % p);
    } else if (opt == 2) {
      cin >> x;
      t.add(l, r, (x % p + p) % p);
    } else if (opt == 3) {
      cout << t.qry(l, r).s << '\n';
    }
  }
  return 0;
}

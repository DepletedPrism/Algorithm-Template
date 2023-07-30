// Luogu P4148
// DeP
#include <bits/stdc++.h>
using namespace std;

template<int K> struct KDT {
  static constexpr double alpha = 0.725;
  struct Node {
    int lc, rc, d, size, sum, val;
    array<int, K> x, mn, mx;
    Node() {}
    Node(const array<int, K>& _x, const int& _val)
      : lc(-1), rc(-1), d(0), size(1), sum(_val), val(_val),
        x(_x), mn(_x), mx(_x) {}
  };
  int rt, nidx;
  vector<Node> nodes;

  KDT(int n): rt(-1), nidx(0) { nodes.resize(n); }

  void maintain(int nd) {
    Node& u = nodes[nd];
    u.size = 1, u.sum = u.val;
    for (int k = 0; k < K; ++k)
      u.mn[k] = u.mx[k] = u.x[k];
    if (~u.lc) {
      const Node& lu = nodes[u.lc];
      u.size += lu.size, u.sum += lu.sum;
      for (int k = 0; k < K; ++k) {
        u.mn[k] = min(u.mn[k], lu.mn[k]);
        u.mx[k] = max(u.mx[k], lu.mx[k]);
      }
    }
    if (~u.rc) {
      const Node& ru = nodes[u.rc];
      u.size += ru.size, u.sum += ru.sum;
      for (int k = 0; k < K; ++k) {
        u.mn[k] = min(u.mn[k], ru.mn[k]);
        u.mx[k] = max(u.mx[k], ru.mx[k]);
      }
    }
  }

  int rebuild(int nd) {
    vector<int> g;
    function<void(int)> dfs = [&](int u) {
      if (~nodes[u].lc)
        dfs(nodes[u].lc);
      g.push_back(u);
      if (~nodes[u].rc)
        dfs(nodes[u].rc);
    };
    if (~nd) dfs(nd);
    function<int(int, int)> build = [&](int l, int r) {
      if (l > r)
        return -1;
      int mid = (l + r) / 2;
      array<double, K> avg, sum;
      for (int k = 0; k < K; ++k)
        avg[k] = sum[k] = 0;
      for (int i = l; i <= r; ++i)
        for (int k = 0; k < K; ++k)
          avg[k] += nodes[g[i]].x[k];
      for (int k = 0; k < K; ++k)
        avg[k] /= (r - l + 1);
      for (int i = l; i <= r; ++i)
        for (int k = 0; k < K; ++k) {
          auto v = nodes[g[i]].x[k] - avg[k];
          sum[k] += v * v;
        }
      int mxk = 0;
      for (int k = 1; k < K; ++k)
        if (sum[k] > sum[mxk]) mxk = k;
      nth_element(g.begin() + l, g.begin() + mid, g.begin() + r + 1,
          [&](const int& lhs, const int& rhs) {
            return nodes[lhs].x[mxk] < nodes[rhs].x[mxk];
          });
      nodes[g[mid]].d = mxk;
      nodes[g[mid]].lc = build(l, mid - 1);
      nodes[g[mid]].rc = build(mid + 1, r);
      maintain(g[mid]);
      return g[mid];
    };
    return build(0, g.size() - 1);
  }

  bool canrb(int nd) {
    const Node& u = nodes[nd];
    return alpha * u.size
      <= max((~u.lc)? nodes[u.lc].size: 0, (~u.rc)? nodes[u.rc].size: 0);
  }

  void ins(int& nd, const array<int, K>& x, int val) {
    if (nd == -1) {
      nodes[nd = nidx++] = Node(x, val);
      return maintain(nd);
    }
    int k = nodes[nd].d;
    if (x[k] <= nodes[nd].x[k])
      ins(nodes[nd].lc, x, val);
    else
      ins(nodes[nd].rc, x, val);
    maintain(nd);
    if (canrb(nd))
      nd = rebuild(nd);
  }
  void ins(const array<int, K>& x, int val) {
    ins(rt, x, val);
  }

  int qry(int nd, const array<int, K>& xl, const array<int, K>& xr) {
    if (nd == -1)
      return 0;
    const Node& u = nodes[nd];
    for (int k = 0; k < K; ++k)
      if (xl[k] > u.mx[k] || xr[k] < u.mn[k]) return 0;
    bool flag = true;
    for (int k = 0; k < K; ++k)
      flag &= (xl[k] <= u.mn[k] && u.mx[k] <= xr[k]);
    if (flag)
      return u.sum;
    flag = true;
    for (int k = 0; k < K; ++k)
      flag &= (xl[k] <= u.x[k] && u.x[k] <= xr[k]);
    int ret = qry(u.lc, xl, xr) + qry(u.rc, xl, xr);
    if (flag)
      return ret + u.val;
    return ret;
  }
  int qry(const array<int, K>& xl, const array<int, K>& xr) {
    return qry(rt, xl, xr);
  }
};

constexpr int N = 2e5;

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  cin >> n;
  array<int, 2> x0, x1;
  KDT<2> t(N);
  for(int opt, val, lstans = 0; cin >> opt, opt != 3; ) {
    if (opt == 1) {
      cin >> x0[0] >> x0[1] >> val;
      x0[0] ^= lstans, x0[1] ^= lstans, val ^= lstans;
      t.ins(x0, val);
    } else if (opt == 2) {
      cin >> x0[0] >> x0[1] >> x1[0] >> x1[1];
      x0[0] ^= lstans, x0[1] ^= lstans, x1[0] ^= lstans, x1[1] ^= lstans;
      cout << (lstans = t.qry(x0, x1)) << '\n';
    }
  }
  return 0;
}

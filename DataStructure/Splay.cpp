// LOJ #104
#include <bits/stdc++.h>
using namespace std;

template<typename T = int> struct Splay {
  struct Node {
    array<int, 2> ch;
    int pre, size, cnt;
    T val;
    Node(const T& _v, int _c)
      : ch({-1, -1}), pre(-1), size(_c), cnt(_c), val(_v) {}
  };
  int rt;
  vector<int> stk;
  vector<Node> nodes;

  Splay(): rt(-1) {}
  Splay(const vector<T>& vec) {
    auto build = [&](auto self, int l, int r, int fa) -> int {
      if (l > r) return -1;
      int mid = (l + r) / 2, u = newnode(vec[mid]);
      nodes[u].ch[0] = self(self, l, mid - 1, u);
      nodes[u].ch[1] = self(self, mid + 1, r, u);
      maintain(u);
      return u;
    };
    rt = build(build, 0, (int) vec.size() - 1, -1);
  }

  int newnode(const T& v, int c = 1) {
    if (stk.size() > 0) {
      int nd = stk.back();
      stk.pop_back();
      nodes[nd] = Node(v, c);
      return nd;
    }
    nodes.push_back(Node(v, c));
    return (int) nodes.size() - 1;
  }
  void maintain(int u) {
    auto& nd = nodes[u];
    nd.size = nd.cnt;
    if (~nd.ch[0]) nd.size += nodes[nd.ch[0]].size;
    if (~nd.ch[1]) nd.size += nodes[nd.ch[1]].size;
  }

  int which(int u) {
    return (~nodes[u].pre)? nodes[nodes[u].pre].ch[1] == u: 0;
  }
  void connect(int u, int fa, int w) {
    if (~u) nodes[u].pre = fa;
    if (~fa) nodes[fa].ch[w] = u; else rt = u;
  }
  void rotate(int u) {
    int fa = nodes[u].pre, w = which(u);
    connect(u, nodes[fa].pre, which(fa));
    connect(nodes[u].ch[w ^ 1], fa, w);
    connect(fa, u, w ^ 1);
    maintain(fa);
  }
  void splay(int u, int v) {
    v = nodes[v].pre;
    while (nodes[u].pre != v) {
      int fa = nodes[u].pre;
      if (nodes[fa].pre != v)
        which(fa) == which(u)? rotate(fa): rotate(u);
      rotate(u);
    }
    maintain(u);
  }

  void ins(const T& v) {
    if (rt == -1)
      return void(rt = newnode(v));
    for (int w, u = rt; ~u; u = nodes[u].ch[w]) {
      auto& nd = nodes[u];
      if (nd.val == v)
        return ++nd.cnt, splay(u, rt);
      if (nd.ch[w = (nd.val < v)] == -1) {
        nodes[u].ch[w] = newnode(v);
        nodes[nodes[u].ch[w]].pre = u;
        return splay(nodes[u].ch[w], rt);
      }
    }
  }

  void fnd(const T& v) {
    for (int nd = rt; ~nd; nd = nodes[nd].ch[nodes[nd].val < v])
      if (nodes[nd].val == v) return splay(nd, rt);
    assert(false);
  }
  void del(const T& v) {
    fnd(v);
    auto& nd = nodes[rt];
    if (nd.cnt > 1)
      return void(--nd.cnt);
    if (nd.ch[0] == -1 && nd.ch[1] == -1) {
      stk.push_back(rt);
      return void(rt = -1);
    }
    if (nd.ch[0] == -1 || nd.ch[1] == -1) {
      int w = (nd.ch[0] == -1);
      stk.push_back(rt);
      return rt = nd.ch[w], void(nodes[rt].pre = -1);
    }
    int u = nd.ch[0];
    while (~nodes[u].ch[1]) u = nodes[u].ch[1];
    splay(u, nd.ch[0]);
    nodes[u].ch[1] = nd.ch[1], nodes[u].pre = -1;
    stk.push_back(rt);
    rt = nodes[nodes[u].ch[1]].pre = u;
    maintain(u);
  }

  int rnk(T v) {
    int lst = -1, u = rt, ret = 1;
    while (~u) {
      const auto& nd = nodes[u];
      if (nd.val < v) {
        ret += ((~nd.ch[0])? nodes[nd.ch[0]].size: 0) + nd.cnt;
        lst = u, u = nd.ch[1];
      } else {
        lst = u, u = nd.ch[0];
      }
    }
    if (~lst) splay(lst, rt);
    return ret;
  }

  int kth(int k) {
    int u = rt;
    while (~u) {
      const auto& nd = nodes[u];
      int t = (~nd.ch[0])? nodes[nd.ch[0]].size: 0;
      if (t < k && k <= t + nd.cnt)
        return splay(u, rt), nd.val;
      t += nd.cnt;
      if (k > t) k -= t, u = nd.ch[1];
      else u = nd.ch[0];
    }
    assert(false);
  }

  T pre(const T& v) { return kth(rnk(v) - 1); }
  T suf(const T& v) { return kth(rnk(v + 1)); }
};

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int q;
  cin >> q;
  Splay t;
  for (int opt, x; q; --q) {
    cin >> opt >> x;
    switch (opt) {
      case 1: t.ins(x); break;
      case 2: t.del(x); break;
      case 3: cout << t.rnk(x) << '\n'; break;
      case 4: cout << t.kth(x) << '\n'; break;
      case 5: cout << t.pre(x) << '\n'; break;
      case 6: cout << t.suf(x) << '\n'; break;
    }
  }
  return 0;
}

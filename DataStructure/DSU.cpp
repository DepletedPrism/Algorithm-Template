// Luogu P3367
// DeP
#include <bits/stdc++.h>
using namespace std;

namespace DSU {
  vector<int> fa, size;

  void init(int n) { // 0-index
    fa.resize(n), size.resize(n);
    iota(fa.begin(), fa.end(), 0);
    fill(size.begin(), size.end(), 1);
  }

  int findfa(int u) {
    return (fa[u] == u)? u: fa[u] = findfa(fa[u]);
  }
  void join(int fu, int fv) {
    if (fu == fv)
      return;
    if (size[fu] < size[fv])
      swap(fu, fv);
    fa[fv] = fu, size[fu] += size[fv];
  }
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  DSU::init(n + 1);
  for (int opt, u, v; q > 0; --q) {
    cin >> opt >> u >> v;
    int fu = DSU::findfa(u), fv = DSU::findfa(v);
    switch (opt) {
      case 1: DSU::join(fu, fv); break;
      case 2: puts((fu == fv)? "Y": "N"); break;
      default: fprintf(stderr, "ERR\n");
    }
  }
  return 0;
}

// Luogu P3376
// DeP
#include <queue>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

namespace IO {
    const int MAXSIZE = 1 << 18 | 1;
    char buf[MAXSIZE], *p1, *p2;

    inline int Gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2)? EOF: *p1++; }
    template<typename T> inline void read(T& x) {
        x = 0; int f = 0, ch = getchar();
        while (!isdigit(ch)) { f |= ch == '-'; ch = getchar(); }
        while (isdigit(ch)) { x = x * 10 + ch - '0'; ch = getchar(); }
        if (f) x = -x;
    }
}
using IO::read;

const int MAXN = 10005, MAXM = 100005;
const int INF = 0x3f3f3f3f;

int n, m, S, T;

namespace Graph {
    struct Edge { int nxt, to, cap, flow; } edges[MAXM << 1];
    int head[MAXN], eidx;

    inline void init() { eidx = 1, memset(head, -1, sizeof head); }
    inline void AddEdge(int from, int to, int cap) {
        edges[++eidx] = (Edge){ head[from], to, cap, 0 };
        head[from] = eidx;
        edges[++eidx] = (Edge){ head[to], from, 0, 0 };
        head[to] = eidx;
    }
}

namespace Dinic {
	using namespace Graph;
	int depth[MAXN], cur[MAXN];
	
	bool BFS() {
        static bool vis[MAXN];
		queue<int> Q;
		memset(vis, false, sizeof vis);
		Q.push(S), depth[S] = 0, vis[S] = true;
		while (!Q.empty()) {
			int u = Q.front(); Q.pop();
			for (int i = head[u]; ~i; i = edges[i].nxt) {
				const Edge& e = edges[i];
				if (!vis[e.to] && e.cap > e.flow)
					vis[e.to] = true, depth[e.to] = depth[u] + 1, Q.push(e.to);
			}
		}
		return vis[T];
	}

	int DFS(int u, int a) {
		if (u == T || !a) return a;
		int f, flow = 0;
		for (int& i = cur[u]; ~i; i = edges[i].nxt) {
			Edge& e = edges[i];
			if (depth[e.to] == depth[u] + 1 && (f = DFS(e.to, min(a, e.cap-e.flow))) > 0) {
				flow += f, a -= f;
				e.flow += f, edges[i^1].flow -= f;
				if (!a) break;
			}
		}
		return flow;
	}
	
	int Maxflow() {
		int ret = 0;
		while (BFS()) memcpy(cur, head, sizeof cur), ret += DFS(S, INF);
		return ret;
	}
}

int main() {
	// init
	Graph::init();
	// input
	read(n), read(m), read(S), read(T);
	for (int i = 1; i <= m; ++i) {
		static int u, v, c;
		read(u), read(v), read(c);
		Graph::AddEdge(u, v, c);
	}
	// output
	printf("%d\n", Dinic::Maxflow());
	return 0;
}

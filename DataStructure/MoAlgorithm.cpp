// Luogu P1494 
// DeP 
#include <cmath>
#include <cctype>
#include <cstdio>
#include <algorithm>
using namespace std;

namespace IO {
    template<class T> void read(T& x) {
        x = 0; int f = 0, ch = getchar();
        while (!isdigit(ch)) { f |= ch == '-'; ch = getchar(); }
        while (isdigit(ch)) { x = x * 10 + ch - '0'; ch = getchar(); }
        if (f) x = -x;
    }
}
using IO::read;

typedef long long LL;
typedef pair<LL, LL> Ans;
const int MAXN = 50005;
int block;

struct Query {
    int L, R, idx;
    bool operator < (const Query& rhs) const {
    	return L / block == rhs.L / block? (R == rhs.R? 0: ((L / block) & 1) ^ (R < rhs.R)): L < rhs.L;
//        return (R / block) == (rhs.R / block)? L < rhs.L: R < rhs.R;
    }
} Q[MAXN];

int n, m;
Ans ans[MAXN];
int A[MAXN], col[MAXN];

LL gcd(LL a, LL b) {
    return !b? a: gcd(b, a % b);
}

namespace Mo {
    int L, R; LL tot;

    inline void del(int pos) {
        int c = A[pos];
        --col[c]; tot -= col[c];
    }
    inline void add(int pos) {
        int c = A[pos];
        tot += col[c]; ++col[c];
    }

    void solve() {
        block = int(ceil((double) n / sqrt(m)));
        sort(Q+1, Q+m+1);
        L = R = Q[1].L; ++col[A[L]]; tot = 0;
        for (int i = 1; i <= m; ++i) {
            Query q = Q[i];
            while (L > q.L) add(--L);
            while (R < q.R) add(++R);
            while (L < q.L) del(L++);
            while (R > q.R) del(R--);
            ans[q.idx] = make_pair(tot, ((LL)(R-L+1)*(R-L)) >> 1);
        }
    }
}

int main() {
    //input 
    read(n); read(m);
    for (int i = 1; i <= n; ++i) read(A[i]);
    for (int i = 1; i <= m; ++i) {
        Q[i].idx = i;
        read(Q[i].L); read(Q[i].R);
    }
    //Mo's Algorithm 
    Mo::solve();
    //outpur 
    for (int i = 1; i <= m; ++i) {
        LL  x = ans[i].first, y = ans[i].second;
        if (x) {
        	LL g = gcd(x, y);
        	printf("%lld/%lld\n", x/g, y/g);
		} else puts("0/1");
        
    }
    return 0;
}

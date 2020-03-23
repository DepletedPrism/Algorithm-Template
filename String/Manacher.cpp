// Luogu P3805
// DeP
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 11000005, MAXM = 3 * MAXN;

char S[MAXN];

namespace Manacher {
    const char BEG = '$', MID = '#';
    char T[MAXM];
    int MaxR[MAXM];

    int turnT(char* S) {
        int len = strlen(S), j = 2;
        T[0] = BEG, T[1] = MID;
        for (int i = 0; i < len; ++i)
            T[j++] = S[i], T[j++] = MID;
        return T[j] = '\0', j;
    }

    int solve(char* S) {
        int len = turnT(S);
        int Mid = 0, mx = 0, ret = -1;
        for (int i = 1; i < len; ++i) {
            MaxR[i] = (i < mx)? min(MaxR[Mid*2 - i], mx - i): 1;
            while (T[i+MaxR[i]] == T[i-MaxR[i]]) ++MaxR[i];
            if (mx < i+MaxR[i]) mx = i+MaxR[i], Mid = i;
            ret = max(ret, MaxR[i]-1);
        }
        return ret;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("testdata.in", "r", stdin);
#endif
    // input
    scanf("%s", S);
    // output
    printf("%d", Manacher::solve(S));
    return 0;
}

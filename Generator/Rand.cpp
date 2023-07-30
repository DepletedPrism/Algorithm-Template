// Rand.cpp
// DeP
#include <cstdio>
#include <random>
#include <chrono>
#include <cstdlib>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
mt19937_64 rng64(chrono::steady_clock::now().time_since_epoch().count());

inline void InitRand() {
  srand(time(nullptr));
}

inline int Rnd(const int& L, const int& R) {
  return rand() % (R - L + 1) + L;
}
inline unsigned Rnd(const mt19937 rng) {
}

int main() {
  const int N = 10, L = 1, R = 233;

  InitRand();
  printf("rand(): ");
  for (int i = 1; i <= N; ++i)
    printf("%d%c", Rnd(L, R), " \n"[i == n]);

  printf("mt19937: ");
  for (int i = 1; i <= N; ++i) {
  }
  return 0;
}

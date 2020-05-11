// Rand.cpp
// DeP
#include <cstdio>
#include <random>
#include <cstdlib>
using namespace std;

inline void InitRand() {
  srand(time(nullptr));
}

mt19937 rng;
mt19937_64 rng64;

inline void InitMt19937() {
}

inline int Rnd(const int& L, const int& R) {
  return rand() % (R - L + 1) + L;
}

int main() {
  const int N = 10, L = 1, R = 233;

  InitRand();
  printf("rand(): ");
  for (int i = 1; i <= N; ++i)
    printf("%d%c", Rnd(L, R));

  printf("mt19937: ");
  return 0;
}

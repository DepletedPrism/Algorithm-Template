// Bit hacks
// DeP
#include <bits/stdc++.h>
using namespace std;

// __builtin_ctz
inline int ctz32(uint32_t x) {
  int r = 31;
  x &= ~x + 1;
  if (x & 0x0000FFFF) r -= 16;
  if (x & 0x00FF00FF) r -= 8;
  if (x & 0x0F0F0F0F) r -= 4;
  if (x & 0x33333333) r -= 2;
  if (x & 0x55555555) r -= 1;
  return r;
}
// __builtin_ctzll
inline int ctz64(uint64_t x) {
  int r = 63;
  x &= ~x + 1;
  if (x & 0x00000000FFFFFFFF) r -= 32;
  if (x & 0x0000FFFF0000FFFF) r -= 16;
  if (x & 0x00FF00FF00FF00FF) r -= 8;
  if (x & 0x0F0F0F0F0F0F0F0F) r -= 4;
  if (x & 0x3333333333333333) r -= 2;
  if (x & 0x5555555555555555) r -= 1;
  return r;
}

// __builtin_ffs
// __builtin_ffs(x) == __builtin_ctz(x) + 1
inline int ffs32(uint32_t x) {
  return (x == 0)? 0: ctz32(x) + 1;
}
// __builtin_ffsll
inline int ffs64(uint64_t x) {
  return (x == 0)? 0: ctz64(x) + 1;
}

// __builtin_clz
inline int clz32(uint32_t x) {
  int r = 0;
  if (!(x & 0xFFFF0000)) r += 16, x <<= 16;
  if (!(x & 0xFF000000)) r += 8, x <<= 8;
  if (!(x & 0xF0000000)) r += 4, x <<= 4;
  if (!(x & 0xC0000000)) r += 2, x <<= 2;
  if (!(x & 0x80000000)) r += 1, x <<= 1;
  return r;
}
// __builtin_clzll
inline int clz64(uint64_t x) {
  int r = 0;
  if (!(x & 0xFFFFFFFF00000000)) r += 32, x <<= 32;
  if (!(x & 0xFFFF000000000000)) r += 16, x <<= 16;
  if (!(x & 0xFF00000000000000)) r += 8, x <<= 8;
  if (!(x & 0xF000000000000000)) r += 4, x <<= 4;
  if (!(x & 0xC000000000000000)) r += 2, x <<= 2;
  if (!(x & 0x8000000000000000)) r += 1, x <<= 1;
  return r;
}

// __builtin_popcount
inline int popcount32(uint64_t x) {
  x = (x & 0x55555555) + (x >> 1  & 0x55555555);
  x = (x & 0x33333333) + (x >> 2  & 0x33333333);
  x = (x & 0x0F0F0F0F) + (x >> 4  & 0x0F0F0F0F);
  x = (x & 0x00FF00FF) + (x >> 8  & 0x00FF00FF);
  x = (x & 0x0000FFFF) + (x >> 16 & 0x0000FFFF);
  return x;
}
// __builtin_popcountll
inline int popcount64(uint64_t x) {
  x = (x & 0x5555555555555555) + (x >> 1  & 0x5555555555555555);
  x = (x & 0x3333333333333333) + (x >> 2  & 0x3333333333333333);
  x = (x & 0x0F0F0F0F0F0F0F0F) + (x >> 4  & 0x0F0F0F0F0F0F0F0F);
  x = (x & 0x00FF00FF00FF00FF) + (x >> 8  & 0x00FF00FF00FF00FF);
  x = (x & 0x0000FFFF0000FFFF) + (x >> 16 & 0x0000FFFF0000FFFF);
  x = (x & 0x00000000FFFFFFFF) + (x >> 32 & 0x00000000FFFFFFFF);
  return x;
}

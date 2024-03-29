/*
 * max{d(n)} when n <= 10^x
 * x    1    2    3     4     5     6     7     8      9
 *      4   12   32    64   128   240   448   768   1344
 * x   10   11   12    13    14    15    16    17     18
 *   2304 4032 6720 10752 17280 26880 41472 64512 103680
 * max{omega(n)} when n <= 10^x
 * x    1    2    3     4     5     6     7     8      9
 *      2    3    4     5     6     7     8     8      9
 * x   10   11   12    13    14    15    16    17     18
 *     10   10   11    12    13    13    13    14     15
 *
 * primes for NTT
 *
 * common
 * 469762049 = 7 * (1 << 26) | 1
 * 998244353 = 7 * 17 * (1 << 23) | 1
 * 1004535809 = 479 * (1 << 21) | 1
 * more
 * origin: https://www.cnblogs.com/Guess2/p/8422205.html
 * p = r * 2^k + 1     r   k   g
 * 3                   1   1   2
 * 5                   1   2   2
 * 17                  1   4   3
 * 97                  3   5   5
 * 193                 3   6   5
 * 257                 1   8   3
 * 7681                15  9   17
 * 12289               3   12  11
 * 40961               5   13  3
 * 65537               1   16  3
 * 786433              3   18  10
 * 5767169             11  19  3
 * 7340033             7   20  3
 * 23068673            11  21  3
 * 104857601           25  22  3
 * 167772161           5   25  3
 * 469762049           7   26  3
 * 1004535809          479 21  3
 * 2013265921          15  27  31
 * 2281701377          17  27  3
 * 3221225473          3   30  5
 * 75161927681         35  31  3
 * 77309411329         9   33  7
 * 206158430209        3   36  22
 * 2061584302081       15  37  7
 * 2748779069441       5   39  3
 * 6597069766657       3   41  5
 * 39582418599937      9   42  5
 * 79164837199873      9   43  5
 * 263882790666241     15  44  7
 * 1231453023109121    35  45  3
 * 1337006139375617    19  46  3
 * 3799912185593857    27  47  5
 * 4222124650659841    15  48  19
 * 7881299347898369    7   50  6
 * 31525197391593473   7   52  3
 * 180143985094819841  5   55  6
 * 1945555039024054273 27  56  5
 * 4179340454199820289 29  57  3
 */
